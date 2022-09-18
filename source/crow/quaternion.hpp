#pragma once

#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <array>
#include <cmath>
#include <cstdlib>
#include <ostream>
#include <string>

namespace Crow {

    template <typename T>
    class Quaternion {

    public:

        static_assert(std::is_arithmetic_v<T>);

        using scalar_type = T;

        constexpr Quaternion() noexcept: array_{} {}
        constexpr Quaternion(T a) noexcept: array_{{a, T(0), T(0), T(0)}} {}
        constexpr Quaternion(T a, T b, T c, T d) noexcept: array_{{a, b, c, d}} {}
        constexpr Quaternion(T a, const Vector<T, 3>& v) noexcept: array_{{a, v.x(), v.y(), v.z()}} {}

        constexpr T& operator[](int i) noexcept { return array_[i]; }
        constexpr const T& operator[](int i) const noexcept { return array_[i]; }
        constexpr T& a() noexcept { return array_[0]; }
        constexpr T a() const noexcept { return array_[0]; }
        constexpr T& b() noexcept { return array_[1]; }
        constexpr T b() const noexcept { return array_[1]; }
        constexpr T& c() noexcept { return array_[2]; }
        constexpr T c() const noexcept { return array_[2]; }
        constexpr T& d() noexcept { return array_[3]; }
        constexpr T d() const noexcept { return array_[3]; }
        constexpr T* begin() noexcept { return array_.data(); }
        constexpr const T* begin() const noexcept { return array_.data(); }
        constexpr T* end() noexcept { return begin() + 4; }
        constexpr const T* end() const noexcept { return begin() + 4; }

        constexpr Quaternion operator+() const noexcept { return *this; }
        constexpr Quaternion operator-() const noexcept { return {- a(), b(), c(), d()}; }
        constexpr Quaternion& operator+=(const Quaternion& y) noexcept { for (int i = 0; i < 4; ++i) array_[i] += y.array_[i]; return *this; }
        constexpr Quaternion& operator-=(const Quaternion& y) noexcept { for (int i = 0; i < 4; ++i) array_[i] -= y.array_[i]; return *this; }
        constexpr Quaternion& operator*=(T y) noexcept { for (auto& x: *this) x *= y; return *this; }
        constexpr Quaternion& operator/=(T y) noexcept { for (auto& x: *this) x /= y; return *this; }
        constexpr Quaternion& operator*=(const Quaternion& y) noexcept { *this = *this * y; return *this; }
        constexpr friend Quaternion operator+(const Quaternion& x, const Quaternion& y) noexcept { auto q = x; q += y; return q; }
        constexpr friend Quaternion operator-(const Quaternion& x, const Quaternion& y) noexcept { auto q = x; q -= y; return q; }
        constexpr friend Quaternion operator*(const Quaternion& x, T y) noexcept { auto q = x; q *= y; return q; }
        constexpr friend Quaternion operator*(T x, const Quaternion& y) noexcept { auto q = y; q *= x; return q; }
        constexpr friend Quaternion operator/(const Quaternion& x, T y) noexcept { auto q = x; q /= y; return q; }
        constexpr friend bool operator==(const Quaternion& x, const Quaternion& y) noexcept
            { for (int i = 0; i < 4; ++i) if (x[i] != y[i]) return false; return true; }
        constexpr friend bool operator!=(const Quaternion& x, const Quaternion& y) noexcept { return ! (x == y); }

        constexpr Quaternion conj() const noexcept { return {a(), - b(), - c(), - d()}; }
        constexpr Quaternion conj(const Quaternion& p) const noexcept { return *this * p * conj(); }
        T norm() const noexcept { return std::sqrt(norm2()); }
        constexpr T norm2() const noexcept { T sum = T(0); for (auto x: *this) sum += x * x; return sum; }
        constexpr Quaternion reciprocal() const noexcept { return conj() / norm2(); }
        constexpr T scalar_part() const noexcept { return a(); }
        constexpr Vector<T, 3> vector_part() const noexcept { return {b(), c(), d()}; }
        constexpr Quaternion versor() const noexcept { return *this / norm(); }
        constexpr Vector<T, 4> to_vector() const noexcept { return {a(), b(), c(), d()}; }
        constexpr static Quaternion from_vector(const Vector<T, 4>& v) noexcept { return {v.x(), v.y(), v.z(), v.w()}; }

        size_t hash() const noexcept { return Detail::hash_range(array_); }
        std::string str(const FormatSpec& spec = {}) const { return "H" + format_range(*this, spec); }
        friend std::ostream& operator<<(std::ostream& out, const Quaternion& q) { return out << q.str(); }

    private:

        std::array<T, 4> array_;

    };

    using Qfloat = Quaternion<float>;
    using Qdouble = Quaternion<double>;
    using Qldouble = Quaternion<long double>;

    template <typename T>
    constexpr Quaternion<T> operator*(const Quaternion<T>& x, const Quaternion<T>& y) noexcept {
        return {
            x.a() * y.a() - x.b() * y.b() - x.c() * y.c() - x.d() * y.d(),
            x.a() * y.b() + x.b() * y.a() + x.c() * y.d() - x.d() * y.c(),
            x.a() * y.c() - x.b() * y.d() + x.c() * y.a() + x.d() * y.b(),
            x.a() * y.d() + x.b() * y.c() - x.c() * y.b() + x.d() * y.a()
        };
    }

}

namespace std {

    template <typename T>
    struct hash<Crow::Quaternion<T>> {
        size_t operator()(const Crow::Quaternion<T>& q) const noexcept { return q.hash(); }
    };

}
