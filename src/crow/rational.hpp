#pragma once

#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <compare>
#include <functional>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T>
        constexpr T gcd(T x, T y) noexcept {
            if constexpr (! std::is_unsigned_v<T>)
                if (x < 0)
                    x = - x;
            return y == 0 ? x : gcd(y, x % y);
        }

        bool parse_rational(const std::string& s, std::vector<std::string>& parts, bool& neg) noexcept;

    }

    template <typename T>
    class Ratio {

    public:

        using integer_type = T;

        constexpr Ratio() noexcept: num_(0), den_(1) {}
        constexpr Ratio(T t) noexcept: num_(t), den_(1) {}
        constexpr Ratio(T n, T d) noexcept: num_(n), den_(d) { reduce(); }

        constexpr explicit operator bool() const noexcept { return num_ != 0; }

        template <typename U> constexpr explicit operator U() const noexcept {
            if constexpr (std::is_floating_point_v<U>)
                return U(num_) / U(den_);
            else
                return euclidean_quotient(U(num_), U(den_));
        }

        constexpr Ratio operator+() const noexcept { return *this; }
        constexpr Ratio operator-() const noexcept { return {- num_, den_}; }
        constexpr Ratio& operator+=(Ratio y) noexcept { return *this = *this + y; }
        constexpr Ratio& operator-=(Ratio y) noexcept { return *this = *this - y; }
        constexpr Ratio& operator*=(Ratio y) noexcept { return *this = *this * y; }
        constexpr Ratio& operator/=(Ratio y) noexcept { return *this = *this / y; }

        friend constexpr Ratio operator+(Ratio x, Ratio y) noexcept {
            T n = x.num_ * y.den_ + y.num_ * x.den_;
            T d = x.den_ * y.den_;
            return {n, d};
        }

        friend constexpr Ratio operator-(Ratio x, Ratio y) noexcept {
            T n = x.num_ * y.den_ - y.num_ * x.den_;
            T d = x.den_ * y.den_;
            return {n, d};
        }

        friend constexpr Ratio operator*(Ratio x, Ratio y) noexcept {
            T n = x.num_ * y.num_;
            T d = x.den_ * y.den_;
            return {n, d};
        }

        friend constexpr Ratio operator/(Ratio x, Ratio y) noexcept {
            T n = x.num_ * y.den_;
            T d = x.den_ * y.num_;
            return {n, d};
        }

        friend constexpr bool operator==(Ratio x, Ratio y) noexcept { return x.num_ == y.num_ && x.den_ == y.den_; }
        friend constexpr auto operator<=>(Ratio x, Ratio y) noexcept { return x.num_ * y.den_ <=> y.num_ * x.den_; }

        constexpr T num() const noexcept { return num_; }
        constexpr T den() const noexcept { return den_; }

        constexpr Ratio abs() const noexcept;
        constexpr T floor() const noexcept { return euclidean_quotient(num_, den_); }
        constexpr T ceil() const noexcept;
        constexpr T round() const noexcept;
        size_t hash() const noexcept;
        constexpr bool is_integer() const noexcept { return den_ == 1; }
        constexpr Ratio reciprocal() const noexcept;
        constexpr int sign() const noexcept { return num_ > 0 ? 1 : num_ == 0 ? 0 : -1; }
        constexpr T whole() const noexcept;
        constexpr Ratio frac() const noexcept;

        std::string str(FormatSpec spec = {}) const;
        friend std::ostream& operator<<(std::ostream& o, Ratio r) { return o << r.str(); }

        static Ratio parse(const std::string& s);
        static bool try_parse(const std::string& s, Ratio& r);

    private:

        T num_;
        T den_;

        std::string format_mixed(FormatSpec spec) const;
        std::string format_vulgar(FormatSpec spec) const;
        constexpr void reduce() noexcept;

        static T parse_integer(const std::string& s) noexcept;

    };

        using Rational = Ratio<int>;
        using Rational64 = Ratio<int64_t>;

        template <typename T>
        constexpr Ratio<T> Ratio<T>::abs() const noexcept {
            auto r = *this;
            if constexpr (! std::is_unsigned_v<T>)
                if (num_ < 0)
                    r.num_ = - num_;
            return r;
        }

        template <typename T>
        constexpr T Ratio<T>::ceil() const noexcept {
            auto qr = euclidean_divide(num_, den_);
            T q = qr.first;
            if (qr.second != 0)
                q += 1;
            return q;
        }

        template <typename T>
        constexpr T Ratio<T>::round() const noexcept {
            auto qr = euclidean_divide(num_, den_);
            T q = qr.first;
            if (T(2) * qr.second >= den_)
                q += 1;
            return q;
        }

        template <typename T>
        size_t Ratio<T>::hash() const noexcept {
            size_t h1 = std::hash<T>()(num_);
            size_t h2 = std::hash<T>()(den_);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }

        template <typename T>
        constexpr Ratio<T> Ratio<T>::reciprocal() const noexcept {
            auto r = *this;
            std::swap(r.num_, r.den_);
            return r;
        }

        template <typename T>
        constexpr T Ratio<T>::whole() const noexcept {
            if (num_ >= 0)
                return num_ / den_;
            else
                return - (- num_ / den_);
        }

        template <typename T>
        constexpr Ratio<T> Ratio<T>::frac() const noexcept {
            auto r = *this;
            if (num_ >= 0)
                r.num_ = num_ % den_;
            else
                r.num_ = - (- num_ % den_);
            return r;
        }

        template <typename T>
        std::string Ratio<T>::str(FormatSpec spec) const {
            if (spec.find_mode("DdEeFfGg") != 0)
                return format_object(double(*this), spec);
            else if (spec.option('v'))
                return format_vulgar(spec);
            else
                return format_mixed(spec);
        }

        template <typename T>
        Ratio<T> Ratio<T>::parse(const std::string& s) {
            Ratio r;
            if (! try_parse(s, r))
                throw std::invalid_argument("Invalid rational number: " + quote(s));
            return r;
        }

        template <typename T>
        bool Ratio<T>::try_parse(const std::string& s, Ratio& r) {
            std::vector<std::string> parts;
            bool neg = false;
            if (! Detail::parse_rational(s, parts, neg))
                return false;
            Ratio q;
            q.num_ = parse_integer(parts[0]);
            if (parts.size() == 1)
                q.den_ = 1;
            else
                q.den_ = parse_integer(parts.back());
            if (parts.size() == 3)
                q.num_ = q.num_ * q.den_ + parse_integer(parts[1]);
            if (neg)
                q.num_ = - q.num_;
            q.reduce();
            r = q;
            return true;
        }

        template <typename T>
        std::string Ratio<T>::format_mixed(FormatSpec spec) const {
            using std::to_string;
            T w = whole();
            auto f = frac();
            bool wx = bool(w), fx = bool(f);
            std::string s;
            if (wx || ! fx) {
                s = format_object(w, spec);
                f = f.abs();
            }
            if (wx && fx)
                s += ' ';
            if (fx)
                s += f.format_vulgar(spec);
            return s;
        }

        template <typename T>
        std::string Ratio<T>::format_vulgar(FormatSpec spec) const {
            std::string s = format_object(num_, spec);
            s += '/';
            s += format_object(den_, spec);
            return s;
        }

        template <typename T>
        constexpr void Ratio<T>::reduce() noexcept {
            T d = Detail::gcd(num_, den_);
            num_ /= d;
            den_ /= d;
        }

        template <typename T>
        T Ratio<T>::parse_integer(const std::string& s) noexcept {
            T t = 0;
            for (char c: s)
                t = 10 * t + T(c - '0');
            return t;
        }

}

CROW_STD_HASH_1(Ratio, typename)

namespace std {

    template <typename T>
    requires (std::numeric_limits<T>::is_specialized)
    class numeric_limits<Crow::Ratio<T>>:
    public numeric_limits<T> {

    private:

        using R = Crow::Ratio<T>;
        using LT = numeric_limits<T>;

    public:

        static constexpr bool is_integer = false;                  // Integer
        static constexpr bool is_modulo = false;                   // Modulo arithmetic
        constexpr R lowest() noexcept { return R(LT::lowest()); }  // Min finite value
        constexpr R max() noexcept { return R(LT::max()); }        // Max ﬁnite value
        constexpr R min() noexcept { return R(LT::min()); }        // Min positive float or min finite int

    };

}
