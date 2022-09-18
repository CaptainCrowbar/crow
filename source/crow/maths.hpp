#pragma once

#include "crow/constants.hpp"
#include "crow/types.hpp"
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

namespace Crow {

    // Algorithms

    template <typename T>
    constexpr T const_abs(T x) noexcept {
        static_assert(std::is_arithmetic_v<T>);
        if (std::is_signed_v<T>)
            return x < 0 ? - x : x;
        else
            return x;
    }

    template <typename T2, typename T1>
    constexpr T2 const_round(T1 x) noexcept {
        static_assert(std::is_floating_point_v<T1>);
        static_assert(std::is_integral_v<T2>);
        T2 y = T2(x);
        T1 d = T1(y) - x;
        if (d <= T1(-0.5))
            ++y;
        else if (d > T1(0.5))
            --y;
        return y;
    }

    template <typename T>
    constexpr std::pair<T, T> euclidean_divide(T x, T y) noexcept {
        static_assert(! std::is_same_v<T, bool>);
        T q, r;
        if constexpr (std::is_floating_point_v<T>) {
            q = x / y;
            T sq = q < T(0) ? T(-1) : T(1);
            q *= sq;
            if (q <= T(std::numeric_limits<uintmax_t>::max()))
                q = T(uintmax_t(q));
            q *= sq;
            r = x - q * y;
        } else {
            q = x / y;
            r = x % y;
        }
        if constexpr (std::is_signed_v<T>) {
            if (r < T(0)) {
                q += y < T(0) ? T(1) : T(-1);
                r += y < T(0) ? - y : y;
            }
        }
        return {q, r};
    }

    template <typename T>
    constexpr T euclidean_quotient(T x, T y) noexcept {
        return euclidean_divide(x, y).first;
    }

    template <typename T>
    constexpr T euclidean_remainder(T x, T y) noexcept {
        return euclidean_divide(x, y).second;
    }

    template <typename T>
    constexpr T fraction(T x) noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            using limits64 = std::numeric_limits<int64_t>;
            if (x >= T(limits64::max()) || x <= T(limits64::min()))
                return 0;
            T f = x - int64_t(x);
            if (f < 0)
                f += 1;
            return f;
        } else {
            return 0;
        }
    }

    template <typename X, typename Y>
    constexpr Y interpolate(X x1, Y y1, X x2, Y y2, X x3) noexcept {
        static_assert(std::is_floating_point_v<X>);
        return y1 + (y2 - y1) * ((x3 - x1) / (x2 - x1));
    }

    template <typename T>
    constexpr std::pair<T, T> symmetric_divide(T x, T y) noexcept {
        auto qr = euclidean_divide(x, y);
        T ay = y < T(0) ? - y : y;
        if (T(2) * qr.second > ay) {
            qr.first += y < T(0) ? T(-1) : T(1);
            qr.second -= ay;
        }
        return qr;
    }

    template <typename T>
    constexpr T symmetric_quotient(T x, T y) noexcept {
        return symmetric_divide(x, y).first;
    }

    template <typename T>
    constexpr T symmetric_remainder(T x, T y) noexcept {
        return symmetric_divide(x, y).second;
    }

    template <typename T>
    constexpr T to_degrees(T rad) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return rad * (T(180) / pi_c<T>);
    }

    template <typename T>
    constexpr T to_radians(T deg) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return deg * (pi_c<T> / T(180));
    }

    // Literals

    namespace Literals {

        constexpr float operator""_degf(long double x) noexcept { return to_radians(float(x)); }
        constexpr float operator""_degf(unsigned long long x) noexcept { return to_radians(float(x)); }
        constexpr double operator""_degd(long double x) noexcept { return to_radians(double(x)); }
        constexpr double operator""_degd(unsigned long long x) noexcept { return to_radians(double(x)); }
        constexpr long double operator""_degld(long double x) noexcept { return to_radians(x); }
        constexpr long double operator""_degld(unsigned long long x) noexcept { return to_radians(static_cast<long double>(x)); }

    }

}
