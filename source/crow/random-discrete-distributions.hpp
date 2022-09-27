// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-continuous-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/rational.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

namespace Crow {

    template <std::integral T>
    class UniformInteger {

    public:

        using result_type = T;

        constexpr UniformInteger() noexcept {} // Defaults to 0-1
        constexpr explicit UniformInteger(T range) noexcept: min_(0), max_(range - 1) {} // 0 to range-1; UB if range<1
        constexpr UniformInteger(T a, T b) noexcept: min_(a), max_(b) {} // a to b inclusive; UB if a>b

        template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return generate(rng, min(), max());
        }

        constexpr T min() const noexcept { return min_; }
        constexpr T max() const noexcept { return max_; }
        constexpr Ratio<T> mean() const noexcept { return {max_ + min_, 2}; }
        constexpr Ratio<T> variance() const noexcept { T range = max_ - min_ + 1; return {range * range - 1, 12}; }
        double sd() const noexcept { return std::sqrt(double(variance())); }

        double pdf(T x) const noexcept {
            if (x >= min_ && x <= max_)
                return 1 / double(max_ - min_ + 1);
            else
                return 0;
        }

        double cdf(T x) const noexcept {
            if (x < min_)
                return 0;
            else if (x < max_)
                return double(x - min_ + 1) / double(max_ - min_ + 1);
            else
                return 1;
        }

        double ccdf(T x) const noexcept {
            if (x <= min_)
                return 1;
            else if (x <= max_)
                return double(max_ - x + 1) / double(max_ - min_ + 1);
            else
                return 0;
        }

    private:

        T min_ = 0;
        T max_ = 1;

        template <RandomEngineType RNG>
        constexpr static T generate(RNG& rng, T min, T max) noexcept {

            // We need an unsigned integer type big enough for both the RNG
            // and the output range.

            using output_range_type = std::make_unsigned_t<T>;
            using rng_result_type = typename RNG::result_type;
            using working_type = std::common_type_t<output_range_type, rng_result_type, unsigned>;

            if (min == max)
                return min;

            // Compare the input range (max-min of the values generated by the
            // RNG) with the output range (max-min of the possible results).

            auto rng_min = working_type(rng.min());
            auto rng_range = working_type(rng.max()) - rng_min;
            auto out_range = working_type(output_range_type(max) - output_range_type(min));
            working_type result;

            if (out_range < rng_range) {

                // The RNG range is larger than the output range. Divide the
                // output of the RNG by the rounded down quotient of the
                // ranges. If one range is not an exact multiple of the
                // other, this may yield a value too large; try again.

                working_type ratio = (rng_range - out_range) / (out_range + 1) + 1;
                working_type limit = ratio * (out_range + 1) - 1;
                do result = working_type(rng() - rng_min);
                    while (result > limit);
                result /= ratio;

            } else if (out_range == rng_range) {

                // The trivial case where the two ranges are equal.

                result = working_type(rng() - rng_min);

            } else {

                // The output range is larger than the RNG range. Split the
                // output range into a quotient and remainder modulo the RNG
                // range +1; call this function recursively for the quotient,
                // then call the RNG directly for the remainder. Try again if
                // the result is too large.

                working_type high = 0, low = 0;
                T ratio = T((out_range - rng_range) / (rng_range + 1));
                do {
                    high = generate(rng, working_type(0), ratio) * (rng_range + 1);
                    low = working_type(rng() - rng_min);
                } while (low > out_range - high);
                result = high + low;

            }

            return min + T(result);

        }

    };

    class BernoulliDistribution {

    public:

        using result_type = bool;

        constexpr BernoulliDistribution() noexcept: BernoulliDistribution(0.5) {}
        constexpr explicit BernoulliDistribution(double p) noexcept: prob_(p) {}
        template <typename T> constexpr explicit BernoulliDistribution(Ratio<T> p) noexcept: prob_(double(p)) {}

        template <RandomEngineType RNG> constexpr bool operator()(RNG& rng) const noexcept { return UniformReal<double>()(rng) < prob_; }

        constexpr double p() const noexcept { return prob_; }

    private:

        double prob_;

    };

    template <std::integral T>
    class DiscreteNormal {

    public:

        using result_type = T;

        DiscreteNormal() = default; // Defaults to (0,1)
        DiscreteNormal(double mean, double sd) noexcept: norm_(mean, sd) {}

        template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept {
            return T(std::lround(norm_(rng)));
        }

        double mean() const noexcept { return norm_.mean(); }
        double sd() const noexcept { return norm_.sd(); }

    private:

        NormalDistribution<double> norm_;

    };

    template <std::integral T>
    class PoissonDistribution {

    public:

        using result_type = T;

        PoissonDistribution() noexcept {}
        explicit PoissonDistribution(double lambda) noexcept: lambda_(lambda), log_lambda_(std::log(lambda)) {}

        template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept {

            // https://www.johndcook.com/blog/2010/06/14/generating-poisson-random-values/

            using std::numbers::pi;

            T n = 0;
            UniformReal<double> unit;

            if (lambda_ <= 30) {

                // Knuth algorithm

                double xl = std::exp(- lambda_);
                T k = 0;
                double p = 1;

                do {
                    ++k;
                    double u = unit(rng);
                    p *= u;
                } while (p > xl);

                n = k - 1;

            } else {

                // Atkinson algorithm

                double c = 0.767 - 3.36 / lambda_;
                double beta = pi / std::sqrt(3 * lambda_);
                double alpha = beta * lambda_;
                double k = std::log(c) - lambda_ - std::log(beta);
                double a = 1;
                double b = 0;

                while (a > b) {
                    double u = unit(rng);
                    double x = (alpha - std::log((1 - u) / u)) / beta;
                    n = T(std::floor(x + 0.5));
                    if (n < 0)
                        continue;
                    double v = unit(rng);
                    double y = alpha - beta * x;
                    double z = 1 + std::exp(y);
                    a = y + std::log(v / (z * z));
                    b = k + n * log_lambda_ - std::lgamma(1.0 + n);
                }

            }

            return n;

        }

        constexpr double mean() const noexcept { return lambda_; }
        constexpr double variance() const noexcept { return lambda_; }
        double sd() const noexcept { return std::sqrt(lambda_); }
        double pdf(T x) const noexcept { return std::exp(x * log_lambda_ - lambda_ - std::lgamma(1.0 + x)); }
        double cdf(T x) const noexcept { return x <= lambda_ + 5 ? make_cdf(x) : 1 - make_ccdf(x + 1); }
        double ccdf(T x) const noexcept { return x <= lambda_ + 5 ? 1 - make_cdf(x - 1) : make_ccdf(x); }

    private:

        double lambda_ = 1;
        double log_lambda_ = 0;

        double make_cdf(T x) const noexcept {
            if (x < 0)
                return 0;
            double s = 0;
            for (T y = 0; y <= x; ++y)
                s += pdf(y);
            return s;
        }

        double make_ccdf(T x) const noexcept {
            if (x <= 0)
                return 1;
            double s = 0;
            T y = x;
            for (;;) {
                double t = s + pdf(y);
                if (t == s)
                    return s;
                s = t;
                ++y;
            }
        }

    };

}
