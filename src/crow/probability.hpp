#pragma once

#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <functional>
#include <limits>
#include <numbers>
#include <ostream>
#include <string>

namespace Crow {

    template <std::floating_point T>
    class Probability;

    namespace Detail {

        struct ProbabilityTest {
            template <std::floating_point T>
                static constexpr T get_data(Probability<T> p) noexcept {
                    return p.data_;
                }
        };

        std::string format_probability_helper(FormatSpec spec, int band,
            std::function<std::string(FormatSpec)> g_format,
            std::function<std::string(FormatSpec)> d_format);

    }

    template <std::floating_point T>
    class Probability {

    public:

        constexpr Probability() = default;
        constexpr Probability(T t) noexcept;

        constexpr T value() const noexcept { return data_ + T(band() == 3); }
        constexpr T complement() const noexcept { return T(band() != 3) - data_; }
        constexpr explicit operator T() const noexcept { return value(); }
        static constexpr Probability complementary(T t) noexcept;

        std::string str(FormatSpec spec = {}) const;
        T z() const noexcept;
        static Probability from_z(T z) noexcept;

        constexpr Probability operator~() const noexcept;
        constexpr Probability& operator+=(Probability y) noexcept { return *this = do_add(*this, y); }
        constexpr Probability& operator-=(Probability y) noexcept { return *this = do_sub(*this, y); }
        constexpr Probability& operator*=(T y) noexcept { return *this = do_mul(*this, y); }
        constexpr Probability& operator/=(T y) noexcept { return *this = do_div(*this, y); }

        friend constexpr Probability operator+(Probability x, Probability y) noexcept { return do_add(x, y); }
        friend constexpr Probability operator-(Probability x, Probability y) noexcept { return do_sub(x, y); }
        friend constexpr Probability operator*(Probability x, T y) noexcept { return do_mul(x, y); }
        friend constexpr Probability operator*(T y, Probability x) noexcept { return do_mul(x, y); }
        friend constexpr Probability operator/(Probability x, T y) noexcept { return do_div(x, y); }
        friend constexpr bool operator==(Probability x, Probability y) noexcept { return x.data_ == y.data_; }
        friend constexpr bool operator==(Probability x, T y) noexcept { return x.value() == y; }
        friend constexpr bool operator==(T x, Probability y) noexcept { return x == y.value(); }
        friend constexpr std::strong_ordering operator<=>(Probability x, Probability y) noexcept { return do_compare(x, y); }
        friend constexpr std::partial_ordering operator<=>(Probability x, T y) noexcept { return x.value() <=> y; }
        friend constexpr std::partial_ordering operator<=>(T x, Probability y) noexcept { return x <=> y.value(); }
        friend std::ostream& operator<<(std::ostream& out, Probability x) { return out << x.str(); }

        friend Probability pow(Probability x, T y) noexcept { return do_power(x, y); }
        friend Probability cmp_pow(Probability x, T y) noexcept { return ~ pow(~ x, y); }

    private:

        friend struct Detail::ProbabilityTest;

        class raw_data {};

        static constexpr T half = T(0.5);
        static constexpr T inf = std::numeric_limits<T>::infinity();
        static constexpr raw_data raw = {};

        // Band  Actual value  Stored value  Offset
        // 0     0             0             0
        // 1     (0,0.5)       (0,0.5)       0
        // 2     0.5           0.5           0
        // 3     (0.5,1)       (-0.5,0)      -1
        // 4     1             1             0

        T data_ = T(0);

        constexpr Probability(T t, raw_data): data_(t) {}

        constexpr int band() const noexcept;

        static constexpr Probability do_add(Probability x, Probability y) noexcept;
        static constexpr Probability do_sub(Probability x, Probability y) noexcept;
        static constexpr Probability do_mul(Probability x, T y) noexcept;
        static constexpr Probability do_div(Probability x, T y) noexcept;
        static constexpr std::strong_ordering do_compare(Probability x, Probability y) noexcept;
        static Probability do_power(Probability x, T y) noexcept;

    };

        template <std::floating_point T>
        constexpr Probability<T>::Probability(T t) noexcept {
            if (t <= 0)
                data_ = 0;
            else if (t >= 1)
                data_ = 1;
            else if (t <= half)
                data_ = t;
            else
                data_ = t - 1;
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::complementary(T t) noexcept {
            Probability p;
            if (t <= 0)
                p.data_ = 1;
            else if (t >= 1)
                p.data_ = 0;
            else if (t < half)
                p.data_ = - t;
            else
                p.data_ = 1 - t;
            return p;
        }

        template <std::floating_point T>
        std::string Probability<T>::str(FormatSpec spec) const {
            using namespace Detail;
            return format_probability_helper(spec, band(),
                [this] (FormatSpec spec) { return format_floating_point(value(), spec); },
                [this] (FormatSpec spec) {
                    T t = std::abs(data_);
                    if (spec.mode() == 'P') t *= 100;
                    return format_float_d(t, spec);
                });
        }

        template <std::floating_point T>
        T Probability<T>::z() const noexcept {
            using namespace std::numbers;
            if (data_ == 0)
                return - inf;
            else if (data_ == 1)
                return inf;
            T z1 = sqrt2_v<T> * inverse_erfc(2 * std::abs(data_));
            if (data_ > 0)
                z1 = - z1;
            return z1;
        }

        template <std::floating_point T>
        Probability<T> Probability<T>::from_z(T z) noexcept {
            using namespace std::numbers;
            static constexpr T c = 1 / sqrt2_v<T>;
            if (z == inf)
                return 1;
            else if (z == - inf)
                return 0;
            auto p = std::erfc(c * std::abs(z)) / 2;
            if (z <= 0)
                return {p, raw};
            else
                return {- p, raw};
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::operator~() const noexcept {
            Probability y;
            if (data_ == 0 || data_ == half || data_ == 1)
                y.data_ = 1 - data_;
            else
                y.data_ = - data_;
            return y;
        }

        template <std::floating_point T>
        constexpr int Probability<T>::band() const noexcept {
            if (data_ == 0)
                return 0;
            else if (data_ == half)
                return 2;
            else if (data_ == 1)
                return 4;
            else if (data_ < 0)
                return 3;
            else
                return 1;
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::do_add(Probability x, Probability y) noexcept {
            int xb = x.band();
            int yb = y.band();
            if (xb == 0)
                return y;
            else if (yb == 0)
                return x;
            else if (xb == 4 || yb == 4)
                return {1, raw};
            else if (xb >= 2 && yb >= 2)
                return {1, raw};
            else if (xb == 2)
                return {y.data_ - half, raw};
            else if (yb == 2)
                return {x.data_ - half, raw};
            T sum = x.data_ + y.data_;
            if (xb == 1 && yb == 1)
                return sum;
            else if (sum < 0)
                return {sum, raw};
            else
                return {1, raw};
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::do_sub(Probability x, Probability y) noexcept {
            int xb = x.band();
            int yb = y.band();
            if (yb == 0)
                return x;
            else if (xb < yb)
                return {};
            else if (xb == 2)
                return {half - y.data_, raw};
            else if (xb == 4 && yb % 2 == 0)
                return {1 - y.data_, raw};
            else if (xb == 4)
                return {- y.data_, raw};
            else if (yb == 2)
                return {x.data_ + half, raw};
            T dif = x.data_ - y.data_;
            if (xb == yb)
                return dif;
            else
                return dif + 1;
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::do_mul(Probability x, T y) noexcept {
            if (y <= 0)
                return {};
            T p = x.data_ * y;
            if (x.data_ >= 0)
                return p;
            else if (y >= 2)
                return {1, raw};
            else if (- p > y - 1 && - p < y - half)
                return {p + (y - 1), raw};
            else
                return p + y;
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::do_div(Probability x, T y) noexcept {
            if (x.data_ < 0)
                return x * (1 / y);
            else
                return x.data_ / y;
        }

        template <std::floating_point T>
        constexpr std::strong_ordering Probability<T>::do_compare(Probability x, Probability y) noexcept {
            int xb = x.band();
            int yb = y.band();
            if (xb != yb)
                return xb <=> yb;
            else if (x.data_ == y.data_)
                return std::strong_ordering::equal;
            else if (x.data_ < y.data_)
                return std::strong_ordering::less;
            else
                return std::strong_ordering::greater;
        }

        template <std::floating_point T>
        Probability<T> Probability<T>::do_power(Probability x, T y) noexcept {
            using namespace std::numbers;
            if (x.data_ == 0)
                return {};
            else if (x.data_ == 1)
                return x;
            else if (x.data_ > 0)
                return std::pow(x.value(), y);
            T logx = std::log1p(x.data_);
            T logz = logx * y;
            if (logz >= 0)
                return {1, raw};
            else if (logz <= - ln2)
                return std::exp(logz);
            else
                return {std::expm1(logz), raw};
        }

}
