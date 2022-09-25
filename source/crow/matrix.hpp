#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <array>
#include <ostream>
#include <string>
#include <type_traits>

namespace Crow {

    CROW_ENUM_CLASS(MatrixLayout, int, 0, row, column)

    namespace Detail {

        template <typename T, int N, MatrixLayout L>
        struct MatrixLayoutTraits;

        template <typename T, int N>
        struct MatrixLayoutTraits<T, N, MatrixLayout::column> {
            using V = Vector<T, N>;
            static constexpr T& get_ref(T* ptr, int r, int c) noexcept { return ptr[r + N * c]; }
            static constexpr const T& get_ref(const T* ptr, int r, int c) noexcept { return ptr[r + N * c]; }
            static constexpr V get_column(const T* ptr, int c) noexcept { return V(ptr + N * c); }
            static constexpr V get_row(const T* ptr, int r) noexcept { V v; for (int i = 0, j = r; i < N; ++i, j += N) v[i] = ptr[j]; return v; }
            static constexpr void set_column(T* ptr, int c, const T* src) noexcept { for (int i = 0, j = N * c; i < N; ++i, ++j) ptr[j] = src[i]; }
            static constexpr void set_row(T* ptr, int r, const T* src) noexcept { for (int i = r, j = 0; j < N; i += N, ++j) ptr[i] = src[j]; }
            static constexpr void swap_columns(T* ptr, int c1, int c2) noexcept {
                if (c1 != c2) {
                    int i = N * c1, j = N * c2;
                    for (int k = 0; k < N; ++k)
                        std::swap(ptr[i + k], ptr[j + k]);
                }
            }
            static constexpr void swap_rows(T* ptr, int r1, int r2) noexcept {
                if (r1 != r2)
                    for (; r1 < N * N; r1 += N, r2 += N)
                        std::swap(ptr[r1], ptr[r2]);
            }
        };

        template <typename T, int N>
        struct MatrixLayoutTraits<T, N, MatrixLayout::row> {
            using V = Vector<T, N>;
            static constexpr T& get_ref(T* ptr, int r, int c) noexcept { return ptr[N * r + c]; }
            static constexpr const T& get_ref(const T* ptr, int r, int c) noexcept { return ptr[N * r + c]; }
            static constexpr V get_column(const T* ptr, int c) noexcept { V v; for (int i = 0, j = c; i < N; ++i, j += N) v[i] = ptr[j]; return v; }
            static constexpr V get_row(const T* ptr, int r) noexcept { return V(ptr + N * r); }
            static constexpr void set_column(T* ptr, int c, const T* src) noexcept { for (int i = c, j = 0; j < N; i += N, ++j) ptr[i] = src[j]; }
            static constexpr void set_row(T* ptr, int r, const T* src) noexcept { for (int i = 0, j = N * r; i < N; ++i, ++j) ptr[j] = src[i]; }
            static constexpr void swap_columns(T* ptr, int c1, int c2) noexcept {
                if (c1 != c2)
                    for (; c1 < N * N; c1 += N, c2 += N)
                        std::swap(ptr[c1], ptr[c2]);
            }
            static constexpr void swap_rows(T* ptr, int r1, int r2) noexcept {
                if (r1 != r2) {
                    int i = N * r1, j = N * r2;
                    for (int k = 0; k < N; ++k)
                        std::swap(ptr[i + k], ptr[j + k]);
                }
            }
        };

    }

    template <typename T, int N, MatrixLayout L = MatrixLayout::column>
    class Matrix {

    private:

        static_assert(std::is_arithmetic_v<T>);
        static_assert(N >= 1);

        static constexpr MatrixLayout alt_layout = L == MatrixLayout::column ? MatrixLayout::row : MatrixLayout::column;

        using layout_traits = Detail::MatrixLayoutTraits<T, N, L>;

    public:

        using scalar_type = T;
        using vector_type = Vector<T, N>;
        using alt_matrix = Matrix<T, N, alt_layout>;

        static constexpr int dim = N;
        static constexpr int cells = N * N;
        static constexpr MatrixLayout layout = L;

        constexpr Matrix() noexcept: array_{} {}
        constexpr explicit Matrix(T x) noexcept: array_{} { for (auto& y: array_) y = x; }
        constexpr Matrix(T lead, T other) noexcept: Matrix(other) { for (int i = 0; i < cells; i += N + 1) array_[i] = lead; }
        constexpr Matrix(const alt_matrix& m) noexcept: array_{}
            { for (int r = 0; r < N; ++r) for (int c = 0; c < N; ++c) (*this)(r, c) = m(r, c); }
        template <typename... Args, typename U = T>
            constexpr Matrix(T x, std::enable_if_t<SfinaeTrue<U, sizeof...(Args) + 2 == cells>::value, T> y, Args... args):
            array_{{T(x), T(y), T(args)...}} {}

        constexpr Matrix operator+() const noexcept { return *this; }
        constexpr Matrix operator-() const noexcept { auto m = *this; for (auto& x: m) x = - x; return m; }
        constexpr Matrix& operator+=(const Matrix& m) noexcept { for (int i = 0; i < cells; ++i) array_[i] += m.array_[i]; return *this; }
        constexpr Matrix& operator-=(const Matrix& m) noexcept { for (int i = 0; i < cells; ++i) array_[i] -= m.array_[i]; return *this; }
        constexpr Matrix& operator*=(T x) noexcept { for (auto& y: *this) y *= x; return *this; }
        constexpr Matrix& operator/=(T x) noexcept { for (auto& y: *this) y /= x; return *this; }
        constexpr Matrix& operator*=(const Matrix& m) noexcept { *this = *this * m; return *this; }
        constexpr friend Matrix operator+(const Matrix& a, const Matrix& b) noexcept { auto m = a; m += b; return m; }
        constexpr friend Matrix operator-(const Matrix& a, const Matrix& b) noexcept { auto m = a; m -= b; return m; }
        constexpr friend Matrix operator*(const Matrix& a, T b) noexcept { auto m = a; m *= b; return m; }
        constexpr friend Matrix operator*(T a, const Matrix& b) noexcept { auto m = b; m *= a; return m; }
        constexpr friend Matrix operator/(const Matrix& a, T b) noexcept { auto m = a; m /= b; return m; }
        constexpr friend bool operator==(const Matrix& a, const Matrix& b) noexcept
            { for (int i = 0; i < cells; ++i) if (a.array_[i] != b.array_[i]) return false; return true; }
        constexpr friend bool operator!=(const Matrix& a, const Matrix& b) noexcept { return ! (a == b); }

        constexpr T& operator()(int r, int c) noexcept { return layout_traits::get_ref(begin(), r, c); }
        constexpr const T& operator()(int r, int c) const noexcept { return layout_traits::get_ref(begin(), r, c); }
        constexpr T* begin() noexcept { return array_.data(); }
        constexpr const T* begin() const noexcept { return array_.data(); }
        constexpr T* end() noexcept { return begin() + cells; }
        constexpr const T* end() const noexcept { return begin() + cells; }

        constexpr vector_type column(int c) const noexcept { return layout_traits::get_column(begin(), c); }
        constexpr vector_type row(int r) const noexcept { return layout_traits::get_row(begin(), r); }
        constexpr void set_column(int c, vector_type v) noexcept { layout_traits::set_column(begin(), c, v.begin()); }
        constexpr void set_row(int r, vector_type v) noexcept { layout_traits::set_row(begin(), r, v.begin()); }
        constexpr Matrix swap_columns(int c1, int c2) const noexcept;
        constexpr Matrix swap_rows(int r1, int r2) const noexcept;

        constexpr T det() const noexcept;
        constexpr bool empty() noexcept { return false; }
        constexpr Matrix inverse() const noexcept;
        constexpr size_t size() const noexcept { return cells; }
        constexpr Matrix transposed() const noexcept;

        size_t hash() const noexcept { return Detail::hash_range(array_); }
        std::string str(const FormatSpec& spec = {}) const;
        friend std::ostream& operator<<(std::ostream& out, const Matrix& m) { return out << m.str(); }

        constexpr static Matrix identity() noexcept { return Matrix(T(1), T(0)); }
        constexpr static Matrix null() noexcept { return Matrix(T(0)); }

    private:

        std::array<T, cells> array_;

    };

    using Float2x2 = Matrix<float, 2>;
    using Float3x3 = Matrix<float, 3>;
    using Float4x4 = Matrix<float, 4>;
    using Float2x2c = Matrix<float, 2, MatrixLayout::column>;
    using Float3x3c = Matrix<float, 3, MatrixLayout::column>;
    using Float4x4c = Matrix<float, 4, MatrixLayout::column>;
    using Float2x2r = Matrix<float, 2, MatrixLayout::row>;
    using Float3x3r = Matrix<float, 3, MatrixLayout::row>;
    using Float4x4r = Matrix<float, 4, MatrixLayout::row>;
    using Double2x2 = Matrix<double, 2>;
    using Double3x3 = Matrix<double, 3>;
    using Double4x4 = Matrix<double, 4>;
    using Double2x2c = Matrix<double, 2, MatrixLayout::column>;
    using Double3x3c = Matrix<double, 3, MatrixLayout::column>;
    using Double4x4c = Matrix<double, 4, MatrixLayout::column>;
    using Double2x2r = Matrix<double, 2, MatrixLayout::row>;
    using Double3x3r = Matrix<double, 3, MatrixLayout::row>;
    using Double4x4r = Matrix<double, 4, MatrixLayout::row>;
    using Ldouble2x2 = Matrix<long double, 2>;
    using Ldouble3x3 = Matrix<long double, 3>;
    using Ldouble4x4 = Matrix<long double, 4>;
    using Ldouble2x2c = Matrix<long double, 2, MatrixLayout::column>;
    using Ldouble3x3c = Matrix<long double, 3, MatrixLayout::column>;
    using Ldouble4x4c = Matrix<long double, 4, MatrixLayout::column>;
    using Ldouble2x2r = Matrix<long double, 2, MatrixLayout::row>;
    using Ldouble3x3r = Matrix<long double, 3, MatrixLayout::row>;
    using Ldouble4x4r = Matrix<long double, 4, MatrixLayout::row>;

    template <typename T, int N, MatrixLayout L>
    constexpr Matrix<T, N, L> operator*(const Matrix<T, N, L>& a, const Matrix<T, N, L>& b) noexcept {
        auto m = Matrix<T, N, L>::null();
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c)
                for (int i = 0; i < N; ++i)
                    m(r, c) += a(r, i) * b(i, c);
        return m;
    }

    template <typename T, int N, MatrixLayout L>
    constexpr Vector<T, N> operator*(const Matrix<T, N, L>& a, const Vector<T, N>& b) noexcept {
        auto v = Vector<T, N>::null();
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c)
                v[r] += a(r, c) * b[c];
        return v;
    }

    template <typename T, int N, MatrixLayout L>
    constexpr Vector<T, N> operator*(const Vector<T, N>& a, const Matrix<T, N, L>& b) noexcept {
        auto v = Vector<T, N>::null();
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c)
                v[c] += a[r] * b(r, c);
        return v;
    }

    template <typename T, int N, MatrixLayout L>
    constexpr Matrix<T, N, L> Matrix<T, N, L>::swap_columns(int c1, int c2) const noexcept {
        auto m = *this;
        layout_traits::swap_columns(m.begin(), c1, c2);
        return m;
    }

    template <typename T, int N, MatrixLayout L>
    constexpr Matrix<T, N, L> Matrix<T, N, L>::swap_rows(int r1, int r2) const noexcept {
        auto m = *this;
        layout_traits::swap_rows(m.begin(), r1, r2);
        return m;
    }

    template <typename T, int N, MatrixLayout L>
    constexpr T Matrix<T, N, L>::det() const noexcept {

        if constexpr (N == 1) {

            return (*this)(0,0);

        } else if constexpr (N == 2) {

            return (*this)(0,0) * (*this)(1,1) - (*this)(0,1) * (*this)(1,0);

        } else if constexpr (N == 3) {

            return (*this)(0,0) * (*this)(1,1) * (*this)(2,2)
                 + (*this)(0,1) * (*this)(1,2) * (*this)(2,0)
                 + (*this)(0,2) * (*this)(1,0) * (*this)(2,1)
                 - (*this)(0,2) * (*this)(1,1) * (*this)(2,0)
                 - (*this)(0,1) * (*this)(1,0) * (*this)(2,2)
                 - (*this)(0,0) * (*this)(1,2) * (*this)(2,1);

        } else if constexpr (N == 4) {

            return (*this)(0,0) * (*this)(1,1) * (*this)(2,2) * (*this)(3,3)
                 + (*this)(0,0) * (*this)(2,1) * (*this)(3,2) * (*this)(1,3)
                 + (*this)(0,0) * (*this)(3,1) * (*this)(1,2) * (*this)(2,3)
                 + (*this)(1,0) * (*this)(0,1) * (*this)(3,2) * (*this)(2,3)
                 + (*this)(1,0) * (*this)(2,1) * (*this)(0,2) * (*this)(3,3)
                 + (*this)(1,0) * (*this)(3,1) * (*this)(2,2) * (*this)(0,3)
                 + (*this)(2,0) * (*this)(0,1) * (*this)(1,2) * (*this)(3,3)
                 + (*this)(2,0) * (*this)(1,1) * (*this)(3,2) * (*this)(0,3)
                 + (*this)(2,0) * (*this)(3,1) * (*this)(0,2) * (*this)(1,3)
                 + (*this)(3,0) * (*this)(0,1) * (*this)(2,2) * (*this)(1,3)
                 + (*this)(3,0) * (*this)(1,1) * (*this)(0,2) * (*this)(2,3)
                 + (*this)(3,0) * (*this)(2,1) * (*this)(1,2) * (*this)(0,3)
                 - (*this)(0,0) * (*this)(1,1) * (*this)(3,2) * (*this)(2,3)
                 - (*this)(0,0) * (*this)(2,1) * (*this)(1,2) * (*this)(3,3)
                 - (*this)(0,0) * (*this)(3,1) * (*this)(2,2) * (*this)(1,3)
                 - (*this)(1,0) * (*this)(0,1) * (*this)(2,2) * (*this)(3,3)
                 - (*this)(1,0) * (*this)(2,1) * (*this)(3,2) * (*this)(0,3)
                 - (*this)(1,0) * (*this)(3,1) * (*this)(0,2) * (*this)(2,3)
                 - (*this)(2,0) * (*this)(0,1) * (*this)(3,2) * (*this)(1,3)
                 - (*this)(2,0) * (*this)(1,1) * (*this)(0,2) * (*this)(3,3)
                 - (*this)(2,0) * (*this)(3,1) * (*this)(1,2) * (*this)(0,3)
                 - (*this)(3,0) * (*this)(0,1) * (*this)(1,2) * (*this)(2,3)
                 - (*this)(3,0) * (*this)(1,1) * (*this)(2,2) * (*this)(0,3)
                 - (*this)(3,0) * (*this)(2,1) * (*this)(0,2) * (*this)(1,3);

        } else {

            static_assert(Detail::dependent_false<T>, "Matrix determinant is not implemented for N>4");

        }

    }

    template <typename T, int N, MatrixLayout L>
    constexpr Matrix<T, N, L> Matrix<T, N, L>::inverse() const noexcept {

        if constexpr (N == 1) {

            return {T(1) / (*this)(0,0)};

        } else if constexpr (N == 2) {

            Matrix c;
            c(0,0) = (*this)(1,1);
            c(0,1) = - (*this)(0,1);
            c(1,0) = - (*this)(1,0);
            c(1,1) = (*this)(0,0);
            return c / (*this).det();

        } else if constexpr (N == 3) {

            Matrix c;
            c(0,0) = (*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1);
            c(0,1) = (*this)(0,2) * (*this)(2,1) - (*this)(0,1) * (*this)(2,2);
            c(0,2) = (*this)(0,1) * (*this)(1,2) - (*this)(0,2) * (*this)(1,1);
            c(1,0) = (*this)(1,2) * (*this)(2,0) - (*this)(1,0) * (*this)(2,2);
            c(1,1) = (*this)(0,0) * (*this)(2,2) - (*this)(0,2) * (*this)(2,0);
            c(1,2) = (*this)(0,2) * (*this)(1,0) - (*this)(0,0) * (*this)(1,2);
            c(2,0) = (*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0);
            c(2,1) = (*this)(0,1) * (*this)(2,0) - (*this)(0,0) * (*this)(2,1);
            c(2,2) = (*this)(0,0) * (*this)(1,1) - (*this)(0,1) * (*this)(1,0);
            return c / (*this).det();

        } else if constexpr (N == 4) {

            // http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform

            T s0 = (*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1);
            T s1 = (*this)(0,0) * (*this)(1,2) - (*this)(1,0) * (*this)(0,2);
            T s2 = (*this)(0,0) * (*this)(1,3) - (*this)(1,0) * (*this)(0,3);
            T s3 = (*this)(0,1) * (*this)(1,2) - (*this)(1,1) * (*this)(0,2);
            T s4 = (*this)(0,1) * (*this)(1,3) - (*this)(1,1) * (*this)(0,3);
            T s5 = (*this)(0,2) * (*this)(1,3) - (*this)(1,2) * (*this)(0,3);
            T c5 = (*this)(2,2) * (*this)(3,3) - (*this)(3,2) * (*this)(2,3);
            T c4 = (*this)(2,1) * (*this)(3,3) - (*this)(3,1) * (*this)(2,3);
            T c3 = (*this)(2,1) * (*this)(3,2) - (*this)(3,1) * (*this)(2,2);
            T c2 = (*this)(2,0) * (*this)(3,3) - (*this)(3,0) * (*this)(2,3);
            T c1 = (*this)(2,0) * (*this)(3,2) - (*this)(3,0) * (*this)(2,2);
            T c0 = (*this)(2,0) * (*this)(3,1) - (*this)(3,0) * (*this)(2,1);
            T invdet = T(1) / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
            Matrix m;
            m(0,0) = ((*this)(1,1) * c5 - (*this)(1,2) * c4 + (*this)(1,3) * c3) * invdet;
            m(0,1) = (-(*this)(0,1) * c5 + (*this)(0,2) * c4 - (*this)(0,3) * c3) * invdet;
            m(0,2) = ((*this)(3,1) * s5 - (*this)(3,2) * s4 + (*this)(3,3) * s3) * invdet;
            m(0,3) = (-(*this)(2,1) * s5 + (*this)(2,2) * s4 - (*this)(2,3) * s3) * invdet;
            m(1,0) = (-(*this)(1,0) * c5 + (*this)(1,2) * c2 - (*this)(1,3) * c1) * invdet;
            m(1,1) = ((*this)(0,0) * c5 - (*this)(0,2) * c2 + (*this)(0,3) * c1) * invdet;
            m(1,2) = (-(*this)(3,0) * s5 + (*this)(3,2) * s2 - (*this)(3,3) * s1) * invdet;
            m(1,3) = ((*this)(2,0) * s5 - (*this)(2,2) * s2 + (*this)(2,3) * s1) * invdet;
            m(2,0) = ((*this)(1,0) * c4 - (*this)(1,1) * c2 + (*this)(1,3) * c0) * invdet;
            m(2,1) = (-(*this)(0,0) * c4 + (*this)(0,1) * c2 - (*this)(0,3) * c0) * invdet;
            m(2,2) = ((*this)(3,0) * s4 - (*this)(3,1) * s2 + (*this)(3,3) * s0) * invdet;
            m(2,3) = (-(*this)(2,0) * s4 + (*this)(2,1) * s2 - (*this)(2,3) * s0) * invdet;
            m(3,0) = (-(*this)(1,0) * c3 + (*this)(1,1) * c1 - (*this)(1,2) * c0) * invdet;
            m(3,1) = ((*this)(0,0) * c3 - (*this)(0,1) * c1 + (*this)(0,2) * c0) * invdet;
            m(3,2) = (-(*this)(3,0) * s3 + (*this)(3,1) * s1 - (*this)(3,2) * s0) * invdet;
            m(3,3) = ((*this)(2,0) * s3 - (*this)(2,1) * s1 + (*this)(2,2) * s0) * invdet;
            return m;

        } else {

            static_assert(Detail::dependent_false<T>, "Matrix inverse is not implemented for N>4");

        }

    }

    template <typename T, int N, MatrixLayout L>
    constexpr Matrix<T, N, L> Matrix<T, N, L>::transposed() const noexcept {
        Matrix m;
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c)
                m(r, c) = (*this)(c, r);
        return m;
    }

    template <typename T, int N, MatrixLayout L>
    std::string Matrix<T, N, L>::str(const FormatSpec& spec) const {
        std::string s;
        for (int r = 0; r < N; ++r) {
            s += r ? ',' : '[';
            for (int c = 0; c < N; ++c)
                s += (c ? ',' : '[') + format_object((*this)(r, c), spec);
            s += ']';
        }
        s += ']';
        return s;
    }

}

CROW_STD_HASH_3(Matrix, typename, int, Crow::MatrixLayout)
