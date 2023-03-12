#pragma once

#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <compare>
#include <concepts>
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

    }

    template <std::floating_point T>
    class Probability {

    public:

        constexpr Probability() = default;
        constexpr Probability(T t);

        constexpr T value() const noexcept { return data_ + T(band() == 3); }
        constexpr explicit operator T() const noexcept { return value(); }

        std::string str(FormatSpec spec = {}) const;
        T z() const noexcept; // 0 -> -inf, 1 > +inf
        static Probability from_z(T z) noexcept; // -inf -> 0, +inf -> 1

        constexpr Probability operator~() const noexcept;
        constexpr Probability& operator+=(Probability q) noexcept { return *this = *this + q; }
        constexpr Probability& operator-=(Probability q) noexcept { return *this = *this - q; }
        constexpr Probability& operator*=(T t) noexcept { return *this = *this * t; }
        constexpr Probability& operator/=(T t) noexcept { return *this = *this / t; }

        friend constexpr Probability operator+(Probability p, Probability q) noexcept;
        friend constexpr Probability operator-(Probability p, Probability q) noexcept;
        friend constexpr Probability operator*(Probability p, T t) noexcept;
        friend constexpr Probability operator*(T t, Probability p) noexcept { return p * t; }
        friend constexpr Probability operator/(Probability p, T t) noexcept;
        friend constexpr bool operator==(Probability p, Probability q) noexcept { return p.data_ == q.data_; }
        friend constexpr std::strong_ordering operator<=>(Probability p, Probability q) noexcept { return do_compare(p, q); }
        friend std::ostream& operator<<(std::ostream& out, Probability p) { return out << p.str(); }

        friend Probability pow(Probability p, T t) noexcept { return do_power(p, t); }
        friend Probability compl_pow(Probability p, T t) noexcept { return ~ pow(~ p, t); }

    private:

        friend struct Detail::ProbabilityTest;

        static constexpr T half = T(0.5);

        // Band  Actual value  Stored value  Offset
        // 0     0             0             0
        // 1     (0,0.5)       (0,0.5)       0
        // 2     0.5           0.5           0
        // 3     (0.5,1)       (-0.5,0)      -1
        // 4     1             1             0

        T data_ = T(0);

        constexpr int band() const noexcept;

        static constexpr int band_t(T t) noexcept;
        static constexpr std::strong_ordering do_compare(Probability p, Probability q) noexcept;
        static constexpr Probability do_power(Probability p, T t) noexcept;

    };

        template <std::floating_point T>
        constexpr Probability<T>::Probability(T t) {
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
        std::string Probability<T>::str(FormatSpec spec) const {

            using namespace Detail;

            spec.default_mode('p');
            spec.default_prec(6);

            if (spec.lcmode() != 'p')
                return format_floating_point(value(), spec);

            bool pc = spec.mode() == 'P';

            if (data_ == 0)
                return "0";
            else if (data_ == 1)
                return pc ? "100" : "1";

            auto t = data_;

            if (pc)
                t *= 100;

            if (t >= 0)
                return format_float_d(t, spec);

            auto s = format_float_d(- t, spec);
            int a = 2 * '0' + 10;
            int b = a - 1;

            for (auto i = int(s.find_last_not_of("0.")); i >= 0; --i, a = b)
                if (ascii_isdigit(s[i]))
                    s[i] = char(a - s[i]);

            if (! pc)
                s[0] = '0';
            else if (s.size() == 1 || s[1] == '.')
                s.insert(0, 1, '9');

            return s;

        }

        template <std::floating_point T>
        T Probability<T>::z() const noexcept {
            // TODO
            return {};
        }

        template <std::floating_point T>
        Probability<T> Probability<T>::from_z(T z) noexcept {
            // TODO
            (void)z;
            return {};
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::operator~() const noexcept {
            Probability q;
            if (data_ == 0 || data_ == half || data_ == 1)
                q.data_ = 1 - data_;
            else
                q.data_ = - data_;
            return q;
        }

        template <std::floating_point T>
        constexpr Probability<T> operator+(Probability<T> p, Probability<T> q) noexcept {
            // TODO
            (void)p;
            (void)q;
            return {};
        }

        template <std::floating_point T>
        constexpr Probability<T> operator-(Probability<T> p, Probability<T> q) noexcept {
            // TODO
            (void)p;
            (void)q;
            return {};
        }

        template <std::floating_point T>
        constexpr Probability<T> operator*(Probability<T> p, T t) noexcept {
            // TODO
            (void)p;
            (void)t;
            return {};
        }

        template <std::floating_point T>
        constexpr Probability<T> operator/(Probability<T> p, T t) noexcept {
            // TODO
            (void)p;
            (void)t;
            return {};
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
        constexpr int Probability<T>::band_t(T t) noexcept {
            if (t <= 0)
                return 0;
            else if (t < half)
                return 1;
            else if (t == half)
                return 2;
            else if (t < 1)
                return 3;
            else
                return 4;
        }

        template <std::floating_point T>
        constexpr std::strong_ordering Probability<T>::do_compare(Probability p, Probability q) noexcept {
            int pb = p.band();
            int qb = q.band();
            if (pb != qb)
                return pb <=> qb;
            else if (p.data_ == q.data_)
                return std::strong_ordering::equal;
            else if (p.data_ < q.data_)
                return std::strong_ordering::less;
            else
                return std::strong_ordering::greater;
        }

        template <std::floating_point T>
        constexpr Probability<T> Probability<T>::do_power(Probability p, T t) noexcept {
            // TODO
            (void)p;
            (void)t;
            return {};
        }

}
