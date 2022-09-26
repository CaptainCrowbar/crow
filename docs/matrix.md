# Matrix

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/matrix.hpp"
namespace Crow;
```

## Matrix class

```c++
enum class MatrixLayout {
    row,
    column,
}
```

Flags indicating the internal layout of a matrix (row major or column major).
Column major is the default.

```c++
template <ArithmeticType T, int N, MatrixLayout L = MatrixLayout::column>
    class Matrix;
```

An `NxN` square matrix type.

```c++
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
```

Predefined type aliases.

```c++
using Matrix::scalar_type = T;
using Matrix::vector_type = Vector<T, N>;
using Matrix::alt_matrix = Matrix<T, N, ...>;
```

Member types. The `alt_matrix` type is a matrix of the same size and element
type, but with the opposite layout.

```c++
static constexpr int Matrix::dim = N;
static constexpr int Matrix::cells = N * N;
static constexpr MatrixLayout Matrix::layout = L;
```

Member constants.

```c++
constexpr Matrix::Matrix() noexcept;
```

The default constructor leaves the elements uninitialized.

```c++
constexpr explicit Matrix::Matrix(T x) noexcept;
```

Sets all elements to the same value.

```c++
constexpr Matrix::Matrix(T lead, T other) noexcept;
```

Sets the leading diagonal to one value, and all other elements to another.

```c++
constexpr Matrix::Matrix(const alt_matrix& m) noexcept;
```

Copies a matrix with the opposite layout.

```c++
template <typename... Args> constexpr Matrix::Matrix(Args... args) noexcept;
```

This constructor takes an explicit list of elements, which are copied in the
order of the matrix's internal layout (column by column or row by row); this
is defined only for the appropriate number of arguments.

```c++
constexpr Matrix::Matrix(const Matrix& m) noexcept;
constexpr Matrix::Matrix(Matrix&& m) noexcept;
constexpr Matrix::~Matrix() noexcept;
constexpr Matrix& Matrix::operator=(const Matrix& m) noexcept;
constexpr Matrix& Matrix::operator=(Matrix&& m) noexcept;
```

Other life cycle functions.

```c++
constexpr T& Matrix::operator()(int r, int c) noexcept;
constexpr const T& Matrix::operator()(int r, int c) const noexcept;
```

Element access functions. Behaviour is undefined if a row or column index is
negative or greater than or equal to `N`.

```c++
constexpr T* Matrix::begin() noexcept;
constexpr const T* Matrix::begin() const noexcept;
constexpr T* Matrix::end() noexcept;
constexpr const T* Matrix::end() const noexcept;
```

These return pointers to the beginning and past the end of the matrix's
internal data, in the order implied by the layout parameter.

```c++
constexpr vector_type Matrix::column(int c) const noexcept;
constexpr vector_type Matrix::row(int r) const noexcept;
constexpr void Matrix::set_column(int c, vector_type v) noexcept;
constexpr void Matrix::set_row(int r, vector_type v) noexcept;
```

Query or set a complete row or column as a vector. Behaviour is undefined if a
row or column index is out of range.

```c++
constexpr T Matrix::det() const noexcept;
```

Returns the determinant of the matrix. (Currently this is only implemented for
`N<=4`).

```c++
constexpr bool Matrix::empty() noexcept;
```

Always false.

```c++
size_t Matrix::hash() const noexcept;
struct std::hash<Matrix>;
```

Hash functions.

```c++
constexpr Matrix Matrix::inverse() const noexcept;
```

Returns the inverse of the matrix. Behaviour is undefined if `det()=0`.
(Currently this is only implemented for `N<=4`).

```c++
constexpr size_t Matrix::size() const noexcept;
```

Returns `N`.

```c++
constexpr Matrix Matrix::swap_columns(int c1, int c2) const noexcept;
constexpr Matrix Matrix::swap_rows(int r1, int r2) const noexcept;
```

Swap two rows or columns. Behaviour is undefined if a row or column index is
out of range.

```c++
std::string Matrix::str(const FormatSpec& spec = {}) const;
std::ostream& operator<<(std::ostream& out, const Matrix& m);
```

The `str()` function formats a matrux as a string, broken down by rows, in the
form `"[[a,b,c],[d,e,f],[g,h,i]]"`. The format spec is interpreted in the
usual way for `T`. The output operator calls `m.str()`.

```c++
constexpr Matrix Matrix::transposed() const noexcept;
```

Returns the transpose of the matrix.

```c++
constexpr Matrix Matrix::operator+() const noexcept;
constexpr Matrix Matrix::operator-() const noexcept;
constexpr Matrix& Matrix::operator+=(const Matrix& m) noexcept;
constexpr Matrix& Matrix::operator-=(const Matrix& m) noexcept;
constexpr Matrix& Matrix::operator*=(const Matrix& m) noexcept;
constexpr Matrix operator+(const Matrix& a, const Matrix& b) noexcept;
constexpr Matrix operator-(const Matrix& a, const Matrix& b) noexcept;
constexpr Matrix operator*(const Matrix& a, const Matrix& b) noexcept;
```

Matrix arithmetic operators.

```c++
constexpr vector_type
    operator*(const Matrix& a, const vector_type& b) noexcept;
constexpr vector_type
    operator*(const vector_type& a, const Matrix& b) noexcept;
```

Matrix-vector arithmetic operators.

```c++
constexpr Matrix& Matrix::operator*=(T x) noexcept;
constexpr Matrix& Matrix::operator/=(T x) noexcept;
constexpr Matrix operator*(const Matrix& a, T b) noexcept;
constexpr Matrix operator*(T a, const Matrix& b) noexcept;
constexpr Matrix operator/(const Matrix& a, T b) noexcept;
```

Matrix-scalar arithmetic operators.

```c++
constexpr bool operator==(const Matrix& a, const Matrix& b) noexcept;
constexpr bool operator!=(const Matrix& a, const Matrix& b) noexcept;
```

Comparison operators.

```c++
constexpr static Matrix identity() noexcept;
```

Returns the identity matrix (equivalent to `Matrix(1,0)`).

```c++
constexpr static Matrix null() noexcept;
```

Returns the zero matrix (equivalent to `Matrix(0)`).
