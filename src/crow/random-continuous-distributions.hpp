// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-engines.hpp"
#include "crow/constants.hpp"
#include "crow/enum.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <concepts>
#include <numbers>

namespace Crow {

    CROW_ENUM_SCOPED(LogMode, int,
        natural,  // Use base e logs
        common    // Use base 10 logs
    )

    template <std::floating_point T = double>
    class UniformReal {

    public:

        using result_type = T;

        constexpr UniformReal() noexcept: UniformReal(0, 1) {} // Defaults to 0-1
        constexpr explicit UniformReal(T range) noexcept: UniformReal(0, range) {} // 0 to range; UB if range<0
        constexpr UniformReal(T a, T b) noexcept: min_(a), max_(b) {} // a to b; UB if a>b

        template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return min_ + ((max_ - min_) / (T(rng.max()) + 1)) * rng();
        }

        constexpr T min() const noexcept { return min_; }
        constexpr T max() const noexcept { return max_; }
        constexpr T mean() const noexcept { return (min_ + max_) / 2; }
        constexpr T variance() const noexcept { T range = max_ - min_; return range * range / 12; }
        T sd() const noexcept { static const T isr12 = 1 / std::sqrt(T(12)); return isr12 * (max_ - min_); }

        constexpr T pdf(T x) const noexcept {
            if (x > min_ && x < max_)
                return 1 / (max_ - min_);
            else
                return 0;
        }

        constexpr T cdf(T x) const noexcept {
            if (x <= min_)
                return 0;
            else if (x < max_)
                return (x - min_) / (max_ - min_);
            else
                return 1;
        }

        constexpr T ccdf(T x) const noexcept {
            if (x <= min_)
                return 1;
            else if (x < max_)
                return (max_ - x) / (max_ - min_);
            else
                return 0;
        }

        constexpr T quantile(T p) const noexcept { return min_ + p * (max_ - min_); }
        constexpr T cquantile(T q) const noexcept { return max_ - q * (max_ - min_); }

    private:

        T min_;
        T max_;

    };

    template <std::floating_point T = double>
    class NormalDistribution {

    public:

        using result_type = T;

        NormalDistribution() noexcept {} // Defaults to (0,1)
        NormalDistribution(T mean, T sd) noexcept: unit_(), mean_(mean), sd_(sd) {}

        template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept {
            using std::numbers::pi_v;
            T u = 1 - unit_(rng); // to ensure log(u) doesn't fail
            T v = unit_(rng);
            T a = std::sqrt(-2 * std::log(u));
            T b = std::cos(2 * pi_v<T> * v);
            return a * b * sd_ + mean_;
        }

        constexpr T mean() const noexcept { return mean_; }
        constexpr T sd() const noexcept { return sd_; }
        constexpr T variance() const noexcept { return sd_ * sd_; }
        T pdf(T x) const noexcept { return pdf_z((x - mean_) / sd_); }
        T cdf(T x) const noexcept { return cdf_z((x - mean_) / sd_); }
        T ccdf(T x) const noexcept { return cdf_z((mean_ - x) / sd_); }
        T quantile(T p) const noexcept { return mean_ + sd_ * q_z(p); }
        T cquantile(T q) const noexcept { return mean_ - sd_ * q_z(q); }

    private:

        UniformReal<T> unit_;
        T mean_ = 0;
        T sd_ = 1;

        T pdf_z(T z) const noexcept {
            using namespace Detail;
            static constexpr T c = SN::inv_sqrtpi_v<T> / SN::sqrt2_v<T>;
            return c * std::exp(- z * z / 2);
        }

        T cdf_z(T z) const noexcept {
            using namespace Detail;
            static constexpr T c = 1 / SN::sqrt2_v<T>;
            return std::erfc(- c * z) / 2;
        }

        T q_z(T p) const noexcept {
            using namespace Detail;
            return - SN::sqrt2_v<T> * inverse_erfc(2 * p);
        }

    };

    template <std::floating_point T = double>
    class LogNormal {

    public:

        using result_type = T;

        LogNormal() noexcept {} // Defaults to (0,1)

        LogNormal(T m, T s, LogMode mode = LogMode::natural) noexcept {
            using namespace Detail;
            if (mode == LogMode::common) {
                m *= SN::ln10_v<T>;
                s *= SN::ln10_v<T>;
            }
            norm_ = NormalDistribution<T>(m, s);
        }

        template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept {
            return std::exp(norm_(rng));
        }

        T m() const noexcept { return norm_.mean(); }
        T s() const noexcept { return norm_.sd(); }
        T median() const noexcept { return std::exp(m()); }
        T s_factor() const noexcept { return std::exp(s()); }

    private:

        NormalDistribution<T> norm_;

    };

    template <std::floating_point T = double>
    class LogUniform {

    public:

        using result_type = T;

        constexpr LogUniform() noexcept: base_(1) {} // Defaults to 1-e
        constexpr LogUniform(T min, T max) noexcept: base_(std::log(min), std::log(max)) {} // UB if min<=0 or max<=0 or min>max

        template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return std::exp(base_(rng));
        }

        constexpr T min() const noexcept { return std::exp(base_.min()); }
        constexpr T max() const noexcept { return std::exp(base_.max()); }

    private:

        UniformReal<T> base_;

    };

}
