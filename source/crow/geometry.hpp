#pragma once

#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <cmath>
#include <numbers>
#include <ostream>
#include <string>
#include <type_traits>

namespace Crow {

    template <typename T, int N>
    class Box {

    public:

        static_assert(std::is_arithmetic_v<T>);
        static_assert(N > 0);

        using scalar_type = T;
        using vector_type = Vector<T, N>;

        static constexpr int dim = N;

        constexpr Box() noexcept: corner_(T{0}), extent_(T{0}) {}
        constexpr Box(const vector_type& p, const vector_type& v) noexcept;

        constexpr vector_type base() const noexcept { return corner_; }
        constexpr vector_type apex() const noexcept { return corner_ + extent_; }
        constexpr vector_type centre() const noexcept { return corner_ + extent_ / T(2); }
        constexpr bool contains(const vector_type& p) const noexcept;
        constexpr bool contains(const Box& b) const noexcept;
        constexpr bool empty() const noexcept { for (auto x: extent_) if (x == T(0)) return true; return false; }
        constexpr T volume() const noexcept { T m = T(1); for (auto x: extent_) m *= x; return m; }
        constexpr vector_type shape() const noexcept { return extent_; }

        constexpr Box& operator+=(const vector_type& v) noexcept { corner_ += v; return *this; }
        constexpr Box& operator-=(const vector_type& v) noexcept { corner_ -= v; return *this; }
        constexpr friend Box operator+(const Box& a, const vector_type& b) noexcept { Box c = a; c += b; return c; }
        constexpr friend Box operator+(const vector_type& a, const Box& b) noexcept { Box c = b; c += a; return c; }
        constexpr friend Box operator-(const Box& a, const vector_type& b) noexcept { Box c = a; c -= b; return c; }
        constexpr friend bool operator==(const Box& a, const Box& b) noexcept { return a.corner_ == b.corner_ && a.extent_ == b.extent_; }
        constexpr friend bool operator!=(const Box& a, const Box& b) noexcept { return ! (a == b); }

        std::string str(const FormatSpec& spec = {}) const;
        friend std::ostream& operator<<(std::ostream& out, const Box& b) { return out << b.str(); }

        constexpr static Box unit() noexcept { return Box(vector_type::null(), vector_type(T(1))); }

    private:

        vector_type corner_;
        vector_type extent_;

    };

        template <typename T, int N>
        constexpr Box<T, N>::Box(const vector_type& p, const vector_type& v) noexcept:
        corner_(p), extent_(v) {
            for (int i = 0; i < N; ++i) {
                if (extent_[i] < T(0)) {
                    corner_[i] += extent_[i];
                    extent_[i] = - extent_[i];
                }
            }
        }

        template <typename T, int N>
        constexpr bool Box<T, N>::contains(const vector_type& p) const noexcept {
            for (int i = 0; i < N; ++i)
                if (p[i] < corner_[i] || p[i] >= corner_[i] + extent_[i])
                    return false;
            return true;
        }

        template <typename T, int N>
        constexpr bool Box<T, N>::contains(const Box& b) const noexcept {
            for (int i = 0; i < N; ++i)
                if (b.corner_[i] < corner_[i] || b.corner_[i] + b.extent_[i] > corner_[i] + extent_[i])
                    return false;
            return true;
        }

        template <typename T, int N>
        std::string Box<T, N>::str(const FormatSpec& spec) const {
            return 'B' + corner_.str(spec) + '+' + extent_.str(spec);
        }

    using Box_i2 = Box<int, 2>;
    using Box_i3 = Box<int, 3>;
    using Box_i4 = Box<int, 4>;
    using Box_f2 = Box<float, 2>;
    using Box_f3 = Box<float, 3>;
    using Box_f4 = Box<float, 4>;
    using Box_d2 = Box<double, 2>;
    using Box_d3 = Box<double, 3>;
    using Box_d4 = Box<double, 4>;
    using Box_ld2 = Box<long double, 2>;
    using Box_ld3 = Box<long double, 3>;
    using Box_ld4 = Box<long double, 4>;

    template <typename T, int N>
    class Sphere {

    public:

        static_assert(std::is_arithmetic_v<T>);
        static_assert(N > 0);

        using scalar_type = T;
        using real_type = std::conditional_t<std::is_floating_point_v<T>, T, double>;
        using vector_type = Vector<T, N>;

        static constexpr int dim = N;

        constexpr Sphere() noexcept: centre_(T{0}), radius_(T{0}) {}
        constexpr Sphere(const vector_type& c, T r) noexcept;

        constexpr vector_type centre() const noexcept { return centre_; }
        constexpr T radius() const noexcept { return radius_; }
        constexpr bool contains(const vector_type& p) const noexcept { return (p - centre_).r2() < radius_ * radius_; }
        constexpr bool contains(const Sphere& s) const noexcept;
        constexpr bool disjoint(const Sphere& s) const noexcept;
        constexpr bool empty() const noexcept { return radius_ == T(0); }
        real_type volume() const noexcept;
        real_type surface() const noexcept;

        constexpr Sphere& operator+=(const vector_type& v) noexcept { centre_ += v; return *this; }
        constexpr Sphere& operator-=(const vector_type& v) noexcept { centre_ -= v; return *this; }
        constexpr Sphere& operator*=(T t) noexcept { radius_ *= t; return *this; }
        constexpr Sphere& operator/=(T t) noexcept { radius_ /= t; return *this; }
        constexpr friend Sphere operator+(const Sphere& a, const vector_type& b) noexcept { Sphere c = a; c += b; return c; }
        constexpr friend Sphere operator+(const vector_type& a, const Sphere& b) noexcept { Sphere c = b; c += a; return c; }
        constexpr friend Sphere operator-(const Sphere& a, const vector_type& b) noexcept { Sphere c = a; c -= b; return c; }
        constexpr friend Sphere operator*(const Sphere& a, T b) noexcept { Sphere c = a; c *= b; return c; }
        constexpr friend Sphere operator*(T a, const Sphere& b) noexcept { Sphere c = b; c *= a; return c; }
        constexpr friend Sphere operator/(const Sphere& a, T b) noexcept { Sphere c = a; c /= b; return c; }
        constexpr friend bool operator==(const Sphere& a, const Sphere& b) noexcept { return a.corner_ == b.corner_ && a.extent_ == b.extent_; }
        constexpr friend bool operator!=(const Sphere& a, const Sphere& b) noexcept { return ! (a == b); }

        std::string str(const FormatSpec& spec = {}) const;
        friend std::ostream& operator<<(std::ostream& out, const Sphere& s) { return out << s.str(); }

        constexpr static Sphere unit() noexcept { return Sphere(vector_type::null(), T(1)); }

    private:

        vector_type centre_;
        T radius_;

    };

        template <typename T, int N>
        constexpr Sphere<T, N>::Sphere(const vector_type& c, T r) noexcept:
        centre_(c), radius_(r) {
            if constexpr (std::is_signed_v<T>)
                radius_ = const_abs(r);
        }

        template <typename T, int N>
        constexpr bool Sphere<T, N>::contains(const Sphere& s) const noexcept {
            if (s.radius_ >= radius_)
                return false;
            T x = radius_ - s.radius_;
            return (s.centre_ - centre_).r2() <= x * x;
        }

        template <typename T, int N>
        constexpr bool Sphere<T, N>::disjoint(const Sphere& s) const noexcept {
            T x = radius_ + s.radius_;
            return (s.centre_ - centre_).r2() >= x * x;
        }

        template <typename T, int N>
        typename Sphere<T, N>::real_type Sphere<T, N>::volume() const noexcept {
            using R = real_type;
            using std::numbers::pi_v;
            if constexpr (N == 1) {
                return R(2) * R(radius_);
            } else if constexpr (N == 2) {
                return pi_v<R> * radius_ * radius_;
            } else if constexpr (N == 3) {
                static const R c = pi_v<R> * R(4) / R(3);
                return c * radius_ * radius_ * radius_;
            } else {
                static const R log_pi = std::log(pi_v<R>);
                static const R a = R(N) / R(2);
                static const R b = a * log_pi - std::lgamma(a + R(1));
                static const R c = std::exp(b);
                return c * std::pow(R(radius_), R(N));
            }
        }

        template <typename T, int N>
        typename Sphere<T, N>::real_type Sphere<T, N>::surface() const noexcept {
            using R = real_type;
            using std::numbers::pi_v;
            if constexpr (N == 1) {
                return R(2);
            } else if constexpr (N == 2) {
                return R(2) * pi_v<R> * radius_;
            } else if constexpr (N == 3) {
                return R(4) * pi_v<R> * radius_ * radius_;
            } else {
                static const R log_pi = std::log(pi_v<R>);
                static const R a = R(N) / R(2);
                static const R b = a * log_pi - std::lgamma(a);
                static const R c = R(2) * std::exp(b);
                return c * std::pow(R(radius_), R(N - 1));
            }
        }

        template <typename T, int N>
        std::string Sphere<T, N>::str(const FormatSpec& spec) const {
            return 'S' + centre_.str(spec) + '+' + format_object(radius_, spec);
        }

    using Sphere_f2 = Sphere<float, 2>;
    using Sphere_f3 = Sphere<float, 3>;
    using Sphere_f4 = Sphere<float, 4>;
    using Sphere_d2 = Sphere<double, 2>;
    using Sphere_d3 = Sphere<double, 3>;
    using Sphere_d4 = Sphere<double, 4>;
    using Sphere_ld2 = Sphere<long double, 2>;
    using Sphere_ld3 = Sphere<long double, 3>;
    using Sphere_ld4 = Sphere<long double, 4>;

}
