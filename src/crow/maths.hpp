#pragma once

#include "crow/constants.hpp"
#include "crow/types.hpp"
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <functional>
#include <limits>
#include <numbers>
#include <type_traits>
#include <utility>

namespace Crow {

    // Arithmetic functions

    template <ArithmeticType T>
    constexpr T binomial(T a, T b) noexcept {
        if (b < 0 || b > a)
            return 0;
        if (b == 0 || b == a)
            return 1;
        if (b > a / 2)
            b = a - b;
        T n = 1;
        T d = 1;
        while (b > 0) {
            n *= a;
            d *= b;
            a -= 1;
            b -= 1;
        }
        return n / d;
    }

    template <typename T>
    constexpr std::pair<T, T> euclidean_divide(T x, T y) noexcept {
        T q, r;
        if constexpr (std::floating_point<T>) {
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

    template <typename T, std::integral U, typename BinaryFunction>
    requires std::invocable<BinaryFunction, T, T>
        && requires (T t, BinaryFunction f) {
            { f(t, t) } -> std::convertible_to<T>;
        }
    constexpr T integer_power(T x, U y, BinaryFunction f, T unit = T(1)) {
        static_assert(std::is_integral_v<U>);
        using V = std::make_unsigned_t<U>;
        if (y < U(1))
            return unit;
        T z = x;
        V v = V(y);
        for (int mask = std::bit_floor(v) >> 1; mask != 0; mask >>= 1) {
            z = f(z, z);
            if ((v & mask) != 0)
                z = f(z, x);
        }
        return z;
    }

    template <typename T, std::integral U>
    constexpr T integer_power(T x, U y) {
        return integer_power(x, y, std::multiplies<T>());
    }

    template <std::floating_point X, typename Y>
    requires requires (X x, Y y) {
        { y - y } -> std::convertible_to<Y>;
        { x * y } -> std::convertible_to<Y>;
    }
    constexpr Y interpolate(X x1, Y y1, X x2, Y y2, X x3) noexcept {
        return y1 + (y2 - y1) * ((x3 - x1) / (x2 - x1));
    }

    template <std::floating_point T>
    constexpr T to_degrees(T rad) noexcept {
        using std::numbers::pi_v;
        return rad * (T(180) / pi_v<T>);
    }

    template <std::floating_point T>
    constexpr T to_radians(T deg) noexcept {
        using std::numbers::pi_v;
        return deg * (pi_v<T> / T(180));
    }

    // Numerical properties

    template <ArithmeticType T>
    constexpr T const_abs(T x) noexcept {
        if (std::is_signed_v<T>)
            return x < 0 ? - x : x;
        else
            return x;
    }

    template <std::integral T2, std::floating_point T1>
    constexpr T2 const_round(T1 x) noexcept {
        T2 y = T2(x);
        T1 d = T1(y) - x;
        if (d <= T1(-0.5))
            ++y;
        else if (d > T1(0.5))
            --y;
        return y;
    }

    template <ArithmeticType T>
    std::pair<T, T> emodf(T x) noexcept {
        if constexpr (std::floating_point<T>) {
            T i, f;
            f = std::modf(x, &i);
            if (f < T(0)) {
                f += T(1);
                i -= T(1);
            }
            return {i, f};
        } else {
            return {x, {}};
        }
    }

    template <ArithmeticType T>
    T fraction(T x) noexcept {
        return emodf(x).second;
    }

    template <typename T>
    constexpr int sign_of(T t) noexcept {
        return t > T() ? 1 : t == T() ? 0 : -1;
    }

    // Special functions

    template <std::floating_point T>
    T inverse_erf(T x) noexcept {

        using namespace std::numbers;

        static constexpr T epsilon = 2 * std::numeric_limits<T>::epsilon();
        static constexpr T sqrtpi_over_2 = 1 / (2 * inv_sqrtpi_v<T>);

        static const auto inv_deriv = [] (T x) { return sqrtpi_over_2 * std::exp(x * x); };

        if (x < 0)
            return - inverse_erf(- x);

        T y = std::sqrt(- std::log1p(- x));

        for (;;) {
            T f = std::erf(y) - x;
            if (f == 0)
                return y;
            T delta = - f * inv_deriv(y);
            if (std::abs(delta) < epsilon * std::abs(y))
                return y + delta / 2;
            y += delta;
        }

    }

    template <std::floating_point T>
    T inverse_erfc(T x) noexcept {

        using namespace std::numbers;

        static constexpr T epsilon = 2 * std::numeric_limits<T>::epsilon();
        static constexpr T sqrtpi_over_2 = 1 / (2 * inv_sqrtpi_v<T>);

        static const auto inv_deriv = [] (T x) { return - sqrtpi_over_2 * std::exp(x * x); };

        if (x > 1)
            return - inverse_erfc(2 - x);

        T y = std::sqrt(- std::log(x));

        for (;;) {
            T f = std::erfc(y) - x;
            if (f == 0)
                return y;
            T delta = - f * inv_deriv(y);
            if (std::abs(delta) < epsilon * std::abs(y))
                return y + delta / 2;
            y += delta;
        }

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
