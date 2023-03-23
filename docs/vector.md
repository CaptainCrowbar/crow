# Vector

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/vector.hpp"
namespace Crow;
```

## Vector class

```c++
template <ArithmeticType T, int N> class Vector;
```

An `N`-dimensional vector type.

```c++
using Byte2 = Vector<uint8_t, 2>;
using Byte3 = Vector<uint8_t, 3>;
using Byte4 = Vector<uint8_t, 4>;
using Int2 = Vector<int, 2>;
using Int3 = Vector<int, 3>;
using Int4 = Vector<int, 4>;
using Float2 = Vector<float, 2>;
using Float3 = Vector<float, 3>;
using Float4 = Vector<float, 4>;
using Double2 = Vector<double, 2>;
using Double3 = Vector<double, 3>;
using Double4 = Vector<double, 4>;
using Ldouble2 = Vector<long double, 2>;
using Ldouble3 = Vector<long double, 3>;
using Ldouble4 = Vector<long double, 4>;
```

Predefined type aliases.

```c++
using Vector::value_type = T;
using Vector::iterator = T*;
using Vector::const_iterator = const T*;
```

Member types.

```c++
static constexpr int Vector::dim = N;
```

Member constants.

```c++
constexpr Vector::Vector() noexcept;
```

The default constructor leaves the vector's contents uninitialized.

```c++
constexpr explicit Vector::Vector(T x) noexcept;
```

Sets all of the vector's elements to the same value.

```c++
template <typename... Args> constexpr Vector::Vector(Args... args) noexcept;
```

This takes an explicit list of elements, and is only defined for the
appropriate number of arguments.

```c++
constexpr explicit Vector::Vector(const T* ptr) noexcept;
```

This copies the elements from the pointed-to data. Behaviour is undefined if
the pointer is null or does not point to an array of at least `N` elements.

```c++
template <ArithmeticType U>
    constexpr explicit Vector::Vector(const Vector<U, N>& v) noexcept;
```

Converts a vector from one element type to another.

```c++
constexpr Vector::Vector(const Vector& v) noexcept;
constexpr Vector::Vector(Vector&& v) noexcept;
constexpr Vector::~Vector() noexcept;
constexpr Vector& Vector::operator=(const Vector& v) noexcept;
constexpr Vector& Vector::operator=(Vector&& v) noexcept;
```

Other life cycle functions.

```c++
constexpr T& Vector::operator[](int i) noexcept;
constexpr const T& Vector::operator[](int i) const noexcept;
constexpr T& Vector::x() noexcept;
constexpr T Vector::x() const noexcept;
constexpr T& Vector::y() noexcept;
constexpr T Vector::y() const noexcept;
constexpr T& Vector::z() noexcept;
constexpr T Vector::z() const noexcept;
constexpr T& Vector::w() noexcept;
constexpr T Vector::w() const noexcept;
```

Element access functions. For the index operator, behaviour is undefined if
`i<0` or `i>=N`. The `x()`, `y()`, `z()`, and `w()` functions return the
first four elements (or references to them), and are only defined if `N` is
large enough.

```c++
constexpr T* Vector::begin() noexcept;
constexpr const T* Vector::begin() const noexcept;
constexpr T* Vector::end() noexcept;
constexpr const T* Vector::end() const noexcept;
```

These return pointers to the beginning and past the end of the vector.

```c++
constexpr Vector Vector::operator+() const noexcept;
constexpr Vector Vector::operator-() const noexcept;
constexpr Vector& Vector::operator+=(const Vector& v) noexcept;
constexpr Vector& Vector::operator-=(const Vector& v) noexcept;
constexpr Vector& Vector::operator*=(const Vector& v) noexcept;
constexpr Vector& Vector::operator/=(const Vector& v) noexcept;
constexpr Vector operator+(const Vector& a, const Vector& b) noexcept;
constexpr Vector operator-(const Vector& a, const Vector& b) noexcept;
constexpr Vector operator*(const Vector& a, const Vector& b) noexcept;
constexpr Vector operator/(const Vector& a, const Vector& b) noexcept;
```

Element-wise arithmetic operators. Behaviour is undefined on division by
zero.

```c++
constexpr Vector& Vector::operator*=(T x) noexcept;
constexpr Vector& Vector::operator/=(T x) noexcept;
constexpr Vector operator*(const Vector& a, T b) noexcept;
constexpr Vector operator*(T a, const Vector& b) noexcept;
constexpr Vector operator/(const Vector& a, T b) noexcept;
```

Vector-scalar arithmetic operators. Behaviour is undefined on division by
zero.

```c++
constexpr T Vector::dot(const Vector& v) const noexcept;
constexpr Vector Vector::cross(const Vector& v) const noexcept;
constexpr T operator%(const Vector& a, const Vector& b) noexcept;
constexpr Vector& operator^=(Vector& a, const Vector& b) noexcept;
constexpr Vector operator^(const Vector& a, const Vector& b) noexcept;
```

Vector arithmetic operations. The `%` and `^` operators are synonyms for
`dot()` and `cross()` respectively. The `cross()` function and the `^`
operators are only defined if `N=3`.

```c++
constexpr bool operator==(const Vector& a, const Vector& b) noexcept;
constexpr bool operator!=(const Vector& a, const Vector& b) noexcept;
```

Comparison operators.

```c++
constexpr T Vector::angle(const Vector& v) const noexcept;
```

Returns the angle between two vectors, in the range `[0,π]`. This will return
zero if either vector is null. Behaviour is undefined if `T` is not a floating
point type.

```c++
constexpr Vector Vector::dir() const noexcept;
```

Returns a unit vector parallel to this vector (or an approximation to it,
given the limits of floating point arithmetic). Returns null if the vector is
null. Behaviour is undefined if `T` is not a floating point type.

```c++
constexpr bool Vector::empty() noexcept;
```

Always false.

```c++
constexpr bool Vector::is_null() const noexcept;
```

True if the vector is null (all elements are zero).

```c++
constexpr Vector Vector::project(const Vector& v) const noexcept;
constexpr Vector Vector::reject(const Vector& v) const noexcept;
```

These return the projection and rejection of this vector onto `v` (the
components of `*this` parallel and orthogonal to `v`, respectively). If `v` is
null, `project()` returns null and `reject()` returns `*this`.

```c++
T Vector::r() const noexcept;
constexpr T Vector::r2() const noexcept;
```

These return the length of the vector, or its square. The `r2()` function will
work with any arithmetic type (provided the result does not overflow), but
behaviour is undefined for `r()` if `T` is not a floating point type.

```c++
constexpr size_t Vector::size() const noexcept;
```

Returns `N`.

```c++
size_t Vector::hash() const noexcept;
template <ArithmeticType T, int N> struct std::hash<Crow::Vector<T, N>>;
```

Hash function.

```c++
std::string Vector::str(const FormatSpec& spec = {}) const;
std::ostream& operator<<(std::ostream& out, const Vector& v);
```

The `str()` function formats a vector as a string, in the form `"[x,y,...]"`,
with the format spec interpreted in the usual way for `T`. The output operator
calls `v.str()`.

```c++
constexpr static Vector Vector::null() noexcept;
```

Returns a null vector (equivalent to `Vector(0)`).

```c++
constexpr static Vector Vector::unit(int i) noexcept;
```

Returns a unit vector along the specified axis. Behaviour is undefined if the
index is out of range (zero to `N-1`).

```c++
constexpr Vector clampv(const Vector& x,
    const Vector& min, const Vector& max) noexcept;
constexpr Vector minv(const Vector& x, const Vector& y) noexcept;
constexpr Vector maxv(const Vector& x, const Vector& y) noexcept;
constexpr std::pair<Vector, Vector>
    minmaxv(const Vector& x, const Vector& y) noexcept;
```

These perform element-wise `clamp()`, `min()`, and `max()` operations on
vectors.

```c++
template <ArithmeticType T, int N, std::floating_point U>
    constexpr Vector<T, N> lerp(const Vector<T, N>& a, const Vector<T, N>& b,
        U x) noexcept;
```

Elementwise linear interpolation. If `T` is an integer type, the results are
rounded to the nearest integer (halves round toward positive infinity).

```c++
template <ArithmeticType T, int N> struct std::greater<Vector<T, N>>;
template <ArithmeticType T, int N> struct std::less<Vector<T, N>>;
```

Ordered comparison operators are not provided for vectors, since they have no
intrinsic order, but specializations of `std::greater` and `std::less` are
supplied to allow vectors to be used as the keys of a map. These perform
simple lexicographical comparison.
