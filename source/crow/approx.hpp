#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <ostream>
#include <string>
#include <string_view>

namespace Crow {

    namespace Detail {

        char approx_options(FormatSpec& spec, size_t tsize);
        void approx_extract(std::string_view str, std::string& vstr, std::string& estr);

    }

    CROW_ENUM_CLASS(Error, unsigned char, 0, abs, rel)

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

        // l = Show error as ulps (default)
        // r = Round based on error (error not shown)
        // x = Show error as explicit value

        char opt = Detail::approx_options(spec, sizeof(T));

        if (is_exact())
            return format_object(value_, spec);

        spec.no_option("z");

        // TODO
        (void)opt;
        return {};

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
