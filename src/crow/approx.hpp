#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <ostream>
#include <string>
#include <string_view>

namespace Crow {

    namespace Detail {

        void approx_extract(std::string_view str, std::string& vstr, std::string& estr);

        constexpr size_t digits_in(std::string_view str) noexcept {
            return std::count_if(str.begin(), str.end(), ascii_isdigit);
        }

    }

    CROW_ENUM_SCOPED(Error, unsigned char, abs, rel)

    template <std::floating_point T>
    class Approx {

    public:

        constexpr Approx() noexcept = default;
        constexpr Approx(T t) noexcept: value_(t) {}
        constexpr Approx(T t, T e) noexcept: value_(t), error_(const_abs(e)) {}
        constexpr Approx(T t, T e, Error etype) noexcept: Approx(t, etype == Error::rel ? t * e : e) {}
        explicit Approx(std::string_view str);

        constexpr T value() const noexcept { return value_; }
        constexpr explicit operator T() const noexcept { return value_; }
        constexpr T error() const noexcept { return error_; }
        constexpr T rel_error() const noexcept { return error_ == 0 ? 0 : error_ / const_abs(value_); }
        constexpr bool is_exact() const noexcept { return error_ == 0; }
        std::string str(FormatSpec spec = {}) const;

        constexpr Approx operator+() const noexcept { return *this; }
        constexpr Approx operator-() const noexcept { return {- value_, error_, raw()}; }
        constexpr Approx& operator+=(Approx b) noexcept { return *this = *this + b; }
        constexpr Approx& operator-=(Approx b) noexcept { return *this = *this - b; }
        constexpr Approx& operator*=(Approx b) noexcept { return *this = *this * b; }
        constexpr Approx& operator/=(Approx b) noexcept { return *this = *this / b; }

        friend constexpr Approx operator+(Approx a, Approx b) noexcept;
        friend constexpr Approx operator-(Approx a, Approx b) noexcept;
        friend constexpr Approx operator*(Approx a, Approx b) noexcept;
        friend constexpr Approx operator/(Approx a, Approx b) noexcept;
        friend constexpr bool operator==(Approx a, Approx b) noexcept { return a.value_ == b.value_; }
        friend constexpr auto operator<=>(Approx a, Approx b) noexcept { return a.value_ <=> b.value_; }
        friend std::ostream& operator<<(std::ostream& out, Approx a) { return out << a.str(); }

    private:

        static constexpr size_t default_prec = sizeof(T) <= sizeof(float) ? 6 : 10;
        static constexpr size_t error_digits = 2;

        class raw {};

        T value_ = 0;
        T error_ = 0;

        constexpr Approx(T v, T e, raw): value_(v), error_(e) {}

    };

    using ApproxF = Approx<float>;
    using ApproxD = Approx<double>;
    using ApproxLD = Approx<long double>;

    template <std::floating_point T>
    Approx<T>::Approx(std::string_view str) {
        std::string vstr, estr;
        Detail::approx_extract(str, vstr, estr);
        value_ = to_floating<T>(vstr);
        if (! estr.empty())
            error_ = to_floating<T>(estr);
    }

    template <std::floating_point T>
    std::string Approx<T>::str(FormatSpec spec) const {

        spec.exclude_mode("Pp");
        spec.default_prec(default_prec);

        char opt = spec.find_option("TtUuVv");
        spec.no_option("AaCcTtUuVvWw");

        if (is_exact())
            return format_object(value_, spec);

        if (opt == 'T' || opt == 't') {

            // T/t = Round based on error

            // TODO
            return {};

        } else if (opt == 'V' || opt == 'v') {

            // v = Show error as explicit value
            // V = Same but with +/-

            // TODO
            return {};

        } else {

            // U/u = Show error as ulps (default)

            // TODO
            return {};

        }

    }

    template <std::floating_point T>
    constexpr Approx<T> operator+(Approx<T> a, Approx<T> b) noexcept {
        // TODO
        (void)a;
        (void)b;
        return {};
    }

    template <std::floating_point T>
    constexpr Approx<T> operator-(Approx<T> a, Approx<T> b) noexcept {
        // TODO
        (void)a;
        (void)b;
        return {};
    }

    template <std::floating_point T>
    constexpr Approx<T> operator*(Approx<T> a, Approx<T> b) noexcept {
        // TODO
        (void)a;
        (void)b;
        return {};
    }

    template <std::floating_point T>
    constexpr Approx<T> operator/(Approx<T> a, Approx<T> b) noexcept {
        // TODO
        (void)a;
        (void)b;
        return {};
    }

}
