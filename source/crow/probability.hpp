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

        template <std::floating_point T>
        struct ProbabilityTest {
            static constexpr T get_data(Probability<T> p) noexcept { return p.data_; }
        };

    }

    template <std::floating_point T>
    class Probability {

    public:

        constexpr Probability() = default;
        constexpr Probability(T t);

        constexpr T value() const noexcept;
        constexpr explicit operator T() const noexcept { return value(); }

        std::string str(FormatSpec spec = {}) const;
        T z() const noexcept; // 0 -> -inf, 1 > +inf
        static Probability from_z(T z) noexcept; // -inf -> 0, +inf -> 1

        constexpr Probability operator~() const noexcept;
        constexpr Probability& operator+=(Probability q) noexcept { return *this = *this + q; }
        constexpr Probability& operator-=(Probability q) noexcept { return *this = *this - q; }
        constexpr Probability& operator*=(Probability q) noexcept { return *this = *this * q; }
        constexpr Probability& operator/=(Probability q) noexcept { return *this = *this / q; }

        friend Probability pow(Probability p, T t) noexcept;
        friend Probability compl_pow(Probability p, T t) noexcept { return ~ pow(~ p, t); }

        friend constexpr Probability operator+(Probability p, Probability q) noexcept;
        friend constexpr Probability operator-(Probability p, Probability q) noexcept;
        friend constexpr Probability operator*(Probability p, Probability q) noexcept;
        friend constexpr Probability operator/(Probability p, Probability q) noexcept;
        friend constexpr bool operator==(Probability p, Probability q) noexcept { return p.data_ == q.data_; }
        friend constexpr std::strong_ordering operator<=>(Probability p, Probability q) noexcept;
        friend std::ostream& operator<<(std::ostream& out, Probability p) { return out << p.str(); }

    private:

        friend struct Detail::ProbabilityTest<T>;

        static constexpr T half = T(0.5);

        // Block  Actual value  Stored value  Offset
        // A      [0,0.5]       [0,0.5]       0
        // B      (0.5,1)       (-0.5,0)      -1
        // C      1             1             0

        T data_ = T(0);

        constexpr char block() const noexcept;

        static constexpr char block_from_t(T t) noexcept;

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
        constexpr T Probability<T>::value() const noexcept {
            if (block() == 'B')
                return data_ + 1;
            else
                return data_;
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
            // TODO
            return {};
        }

        template <std::floating_point T>
        Probability<T> pow(Probability<T> p, T t) noexcept {
            // TODO
            (void)p;
            (void)t;
            return {};
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
        constexpr Probability<T> operator*(Probability<T> p, Probability<T> q) noexcept {
            // TODO
            (void)p;
            (void)q;
            return {};
        }

        template <std::floating_point T>
        constexpr Probability<T> operator/(Probability<T> p, Probability<T> q) noexcept {
            // TODO
            (void)p;
            (void)q;
            return {};
        }

        template <std::floating_point T>
        constexpr std::strong_ordering operator<=>(Probability<T> p, Probability<T> q) noexcept {
            // TODO
            (void)p;
            (void)q;
            return std::strong_ordering::equal;
        }

        template <std::floating_point T>
        constexpr char Probability<T>::block() const noexcept {
            if (data_ < 0)
                return 'B';
            else if (data_ == 1)
                return 'C';
            else
                return 'A';
        }

        template <std::floating_point T>
        constexpr char Probability<T>::block_from_t(T t) noexcept {
            if (t <= half)
                return 'A';
            else if (t < 1)
                return 'B';
            else
                return 'C';
        }

}
