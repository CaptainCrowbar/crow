#pragma once

#include "crow/maths.hpp"
#include "crow/matrix.hpp"
#include "crow/quaternion.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <cmath>
#include <type_traits>

namespace Crow {

    // Coordinate transformations

    template <typename T>
    Vector<T, 2> cartesian_to_polar(const Vector<T, 2>& xy) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {xy.r(), std::atan2(xy[1], xy[0])};
    }

    template <typename T>
    Vector<T, 2> polar_to_cartesian(const Vector<T, 2>& rt) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {rt[0] * std::cos(rt[1]), rt[0] * std::sin(rt[1])};
    }

    template <typename T>
    Vector<T, 3> cartesian_to_cylindrical(const Vector<T, 3>& xyz) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1]), std::atan2(xyz[1], xyz[0]), xyz[2]};
    }

    template <typename T>
    Vector<T, 3> cartesian_to_spherical(const Vector<T, 3>& xyz) noexcept {
        static_assert(std::is_floating_point_v<T>);
        T rho = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1]);
        return {xyz.r(), std::atan2(xyz[1], xyz[0]), std::atan2(rho, xyz[2])};
    }

    template <typename T>
    Vector<T, 3> cylindrical_to_cartesian(const Vector<T, 3>& rpz) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {rpz[0] * std::cos(rpz[1]), rpz[0] * std::sin(rpz[1]), rpz[2]};
    }

    template <typename T>
    Vector<T, 3> cylindrical_to_spherical(const Vector<T, 3>& rpz) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {std::sqrt(rpz[0] * rpz[0] + rpz[2] * rpz[2]), rpz[1], std::atan2(rpz[0], rpz[2])};
    }

    template <typename T>
    Vector<T, 3> spherical_to_cartesian(const Vector<T, 3>& rpt) noexcept {
        static_assert(std::is_floating_point_v<T>);
        T rho = rpt[0] * std::sin(rpt[2]);
        return {rho * std::cos(rpt[1]), rho * std::sin(rpt[1]), rpt[0] * std::cos(rpt[2])};
    }

    template <typename T>
    Vector<T, 3> spherical_to_cylindrical(const Vector<T, 3>& rpt) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {rpt[0] * std::sin(rpt[2]), rpt[1], rpt[0] * std::cos(rpt[2])};
    }

    // Projective geometry

    template <typename T>
    inline Vector<T, 4> vector4(const Vector<T, 3>& v, T w) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {v.x(), v.y(), v.z(), w};
    }

    template <typename T>
    inline Vector<T, 4> point4(const Vector<T, 3>& v) noexcept {
        return vector4(v, T(1));
    }

    template <typename T>
    inline Vector<T, 4> normal4(const Vector<T, 3>& v) noexcept {
        return vector4(v, T(0));
    }

    template <typename T>
    inline Vector<T, 3> point3(const Vector<T, 4>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Vector<T, 3> u{v.x(), v.y(), v.z()};
        if (v.w() != T(0))
            u /= v.w();
        return u;
    }

    template <typename T>
    inline Vector<T, 3> normal3(const Vector<T, 4>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return {v.x(), v.y(), v.z()};
    }

    template <typename T, MatrixLayout L>
    Matrix<T, 4, L> make_transform(const Matrix<T, 3, L>& m, const Vector<T, 3>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        auto t = Matrix<T, 4, L>::identity();
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c)
                t(r, c) = m(r, c);
            t(r, 3) = v[r];
        }
        return t;
    }

    template <typename T, MatrixLayout L>
    inline Matrix<T, 4, L> normal_transform(const Matrix<T, 4, L>& m) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return m.inverse().transposed();
    }

    // Primitive transformations

    template <typename T>
    Matrix<T, 3> rotate3(T angle, int index) noexcept {
        static_assert(std::is_floating_point_v<T>);
        auto c = std::cos(angle), s = std::sin(angle);
        auto m = Matrix<T, 3>::identity();
        switch (index) {
            case 0: m(1,1) = c; m(1,2) = -s; m(2,1) = s; m(2,2) = c; break;
            case 1: m(0,0) = c; m(0,2) = s; m(2,0) = -s; m(2,2) = c; break;
            case 2: m(0,0) = c; m(0,1) = -s; m(1,0) = s; m(1,1) = c; break;
            default: break;
        }
        return m;
    }

    template <typename T>
    Matrix<T, 4> rotate4(T angle, int index) noexcept {
        static_assert(std::is_floating_point_v<T>);
        auto c = std::cos(angle), s = std::sin(angle);
        auto m = Matrix<T, 4>::identity();
        switch (index) {
            case 0: m(1,1) = c; m(1,2) = -s; m(2,1) = s; m(2,2) = c; break;
            case 1: m(0,0) = c; m(0,2) = s; m(2,0) = -s; m(2,2) = c; break;
            case 2: m(0,0) = c; m(0,1) = -s; m(1,0) = s; m(1,1) = c; break;
            default: break;
        }
        return m;
    }

    template <typename T>
    Matrix<T, 3> scale3(T t) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return Matrix<T, 3>(t, T(0));
    }

    template <typename T>
    Matrix<T, 3> scale3(const Vector<T, 3>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Matrix<T, 3> m;
        for (int i = 0; i < 3; ++i)
            m(i, i) = v[i];
        return m;
    }

    template <typename T>
    Matrix<T, 4> scale4(T t) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Matrix<T, 4> m(t, T(0));
        m(3, 3) = T(1);
        return m;
    }

    template <typename T>
    Matrix<T, 4> scale4(const Vector<T, 3>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Matrix<T, 4> m;
        for (int i = 0; i < 3; ++i)
            m(i, i) = v[i];
        m(3, 3) = T(1);
        return m;
    }

    template <typename T>
    Matrix<T, 4> translate4(const Vector<T, 3>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        auto m = Matrix<T, 4>::identity();
        for (int i = 0; i < 3; ++i)
            m(i, 3) = v[i];
        return m;
    }

    // Quaternion transformations

    namespace Detail {

        template <typename T, int N>
        void build_matrix(const Quaternion<T>& q, Matrix<T, N>& m) noexcept {
            m(0, 0) = q.a() * q.a() + q.b() * q.b() - q.c() * q.c() - q.d() * q.d();
            m(0, 1) = T(2) * q.b() * q.c() - T(2) * q.a() * q.d();
            m(0, 2) = T(2) * q.a() * q.c() + T(2) * q.b() * q.d();
            m(1, 0) = T(2) * q.a() * q.d() + T(2) * q.b() * q.c();
            m(1, 1) = q.a() * q.a() - q.b() * q.b() + q.c() * q.c() - q.d() * q.d();
            m(1, 2) = T(2) * q.c() * q.d() - T(2) * q.a() * q.b();
            m(2, 0) = T(2) * q.b() * q.d() - T(2) * q.a() * q.c();
            m(2, 1) = T(2) * q.a() * q.b() + T(2) * q.c() * q.d();
            m(2, 2) = q.a() * q.a() - q.b() * q.b() - q.c() * q.c() + q.d() * q.d();
        }

    }

    template <typename T>
    Vector<T, 3> rotate(const Quaternion<T>& q, const Vector<T, 3>& v) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return q.conj({T(0), v}).vector_part();
    }

    template <typename T>
    Quaternion<T> q_rotate(T angle, const Vector<T, 3>& axis) noexcept {
        static_assert(std::is_floating_point_v<T>);
        if (axis.is_null())
            return T(1);
        angle /= T(2);
        return {std::cos(angle), std::sin(angle) * axis.dir()};
    }

    template <typename T>
    Matrix<T, 3> rotate3(const Quaternion<T>& q) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Matrix<T, 3> m;
        Detail::build_matrix(q, m);
        return m;
    }

    template <typename T>
    Matrix<T, 4> rotate4(const Quaternion<T>& q) noexcept {
        static_assert(std::is_floating_point_v<T>);
        Matrix<T, 4> m;
        Detail::build_matrix(q, m);
        m(3, 3) = T(1);
        return m;
    }

    template <typename T>
    Matrix<T, 3> rotate3(T angle, const Vector<T, 3>& axis) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return rotate3(q_rotate(angle, axis));
    }

    template <typename T>
    Matrix<T, 4> rotate4(T angle, const Vector<T, 3>& axis) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return rotate4(q_rotate(angle, axis));
    }

}
