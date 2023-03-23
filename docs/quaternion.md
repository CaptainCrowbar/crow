# Quaternion

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/quaternion.hpp"
namespace Crow;
```

## Quaternion class

```c++
template <std::floating_point T> class Quaternion;
```

A quaternion class based on the scalar type `T`.

```c++
using Qfloat = Quaternion<float>;
using Qdouble = Quaternion<double>;
using Qldouble = Quaternion<long double>;
```

Predefined type aliases.

```c++
using Quaternion::value_type = T;
```

Member types.

```c++
constexpr Quaternion::Quaternion() noexcept;
```

The default constructor leaves the components uninitialized.

```c++
constexpr Quaternion::Quaternion(T a) noexcept;
```

Sets the real component to the given value, and all other components to zero.

```c++
constexpr Quaternion::Quaternion(T a, T b, T c, T d) noexcept;
constexpr Quaternion::Quaternion(T a, const Vector<T, 3>& v) noexcept;
```

These set all four components explicitly.

```c++
constexpr Quaternion::Quaternion(const Quaternion& q) noexcept;
constexpr Quaternion::Quaternion(Quaternion&& q) noexcept;
constexpr Quaternion::~Quaternion() noexcept;
constexpr Quaternion& Quaternion::operator=(const Quaternion& q) noexcept;
constexpr Quaternion& Quaternion::operator=(Quaternion&& q) noexcept;
```

Other life cycle functions.

```c++
constexpr T& Quaternion::operator[](int i) noexcept;
constexpr const T& Quaternion::operator[](int i) const noexcept;
constexpr T& Quaternion::a() noexcept;
constexpr T Quaternion::a() const noexcept;
constexpr T& Quaternion::b() noexcept;
constexpr T Quaternion::b() const noexcept;
constexpr T& Quaternion::c() noexcept;
constexpr T Quaternion::c() const noexcept;
constexpr T& Quaternion::d() noexcept;
constexpr T Quaternion::d() const noexcept;
```

Element access functions. For the index operator, behaviour is undefined if
`i<0` or `i>3`.

```c++
constexpr T* Quaternion::begin() noexcept;
constexpr const T* Quaternion::begin() const noexcept;
constexpr T* Quaternion::end() noexcept;
constexpr const T* Quaternion::end() const noexcept;
```

These return pointers to the beginning and past the end of the internal
array.

```c++
constexpr Quaternion Quaternion::conj() const noexcept;
constexpr Quaternion Quaternion::conj(const Quaternion& q) const noexcept;
```

The first version returns the conjugate quaternion. The second version
performs the conjugation of `q` by `*this`, returning _pqp<sup>-1</sup>_
(where `p=*this`).

```c++
size_t Quaternion::hash() const noexcept;
struct std::hash<Quaternion>;
```

Hash functions.

```c++
T Quaternion::norm() const noexcept;
constexpr T Quaternion::norm2() const noexcept;
```

Returns the quaternion's norm, or its square.

```c++
constexpr Quaternion Quaternion::reciprocal() const noexcept;
```

Returns the reciprocal of the quaternion. Behaviour is undefined if the
quaternion is zero.

```c++
constexpr T Quaternion::scalar_part() const noexcept;
constexpr Vector<T, 3> Quaternion::vector_part() const noexcept;
```

Return the scalar and vector parts of the quaternion.

```c++
std::string Quaternion::str(const FormatSpec& spec = {}) const;
std::ostream& operator<<(std::ostream& out, const Quaternion& q);
```

The `str()` function formats a quaternion as a string, in the form
`"H[a,b,c,d]"`. The `spec` argument is interpreted in the usual way for `T`.
The output operator calls `q.str()`.

```c++
constexpr Vector<T, 4> Quaternion::to_vector() const noexcept;
constexpr static Quaternion Quaternion::from_vector(const Vector<T, 4>& v) noexcept;
```

Conversion between a quaternion and a 4-vector.

```c++
constexpr Quaternion Quaternion::versor() const noexcept;
```

Returns the quaternion's versor (unit quaternion). Behaviour is undefined if
the quaternion is zero.

```c++
constexpr Quaternion Quaternion::operator+() const noexcept;
constexpr Quaternion Quaternion::operator-() const noexcept;
constexpr Quaternion& Quaternion::operator+=(const Quaternion& y) noexcept;
constexpr Quaternion& Quaternion::operator-=(const Quaternion& y) noexcept;
constexpr Quaternion& Quaternion::operator*=(const Quaternion& y) noexcept;
constexpr Quaternion operator+(const Quaternion& x, const Quaternion& y) noexcept;
constexpr Quaternion operator-(const Quaternion& x, const Quaternion& y) noexcept;
constexpr Quaternion operator*(const Quaternion& x, const Quaternion& y) noexcept;
```

Quaternion arithmetic operators.

```c++
constexpr Quaternion& Quaternion::operator*=(T y) noexcept;
constexpr Quaternion& Quaternion::operator/=(T y) noexcept;
constexpr Quaternion operator*(const Quaternion& x, T y) noexcept;
constexpr Quaternion operator*(T x, const Quaternion& y) noexcept;
constexpr Quaternion operator/(const Quaternion& x, T y) noexcept;
```

Quaternion-scalar arithmetic operators. Behaviour is undefined on division by
zero.

```c++
constexpr bool operator==(const Quaternion& x, const Quaternion& y) noexcept;
constexpr bool operator!=(const Quaternion& x, const Quaternion& y) noexcept;
```

Comparison operators.
