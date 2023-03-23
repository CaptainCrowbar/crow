# Geometric Primitives

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/geometry.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Box class

```c++
template <ArithmeticType T, int N> class Box;
```

Represents a rectangular box in `N` dimensions.

```c++
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
```

Predefined type aliases.

```c++
using Box::value_type = T;
using Box::vector_type = Vector<T, N>;
```

Member types.

```c++
static constexpr int Box::dim = N;
```

Member constants.

```c++
constexpr Box::Box() noexcept;
constexpr Box::Box(const vector_type& p, const vector_type& v) noexcept;
```

The default constructor creates a zero-size box at the origin. The second
constructor creates a box with one corner at `p` and the diagonally opposite
corner at `p+v`.

```c++
constexpr Box::Box(const Box& b) noexcept;
constexpr Box::Box(Box&& b) noexcept;
constexpr Box::~Box() noexcept;
constexpr Box& Box::operator=(const Box& b) noexcept;
constexpr Box& Box::operator=(Box&& b) noexcept;
```

Other life cycle functions.

```c++
constexpr vector_type Box::base() const noexcept;
constexpr vector_type Box::apex() const noexcept;
```

Return the corners of the box with minimum or maximum coordinate values.

```c++
constexpr vector_type Box::centre() const noexcept;
```

Returns the centre of the box. If `T` is an integer type, half-integral values
are rounded toward negative infinity.

```c++
constexpr bool Box::contains(const vector_type& p) const noexcept;
constexpr bool Box::contains(const Box& b) const noexcept;
```

True if the point is inside the box, or if the second box is contained
entirely within this one. Coordinate intervals are treated as half-open: a
point that matches the box's minimum coordinate is considered to be inside
it, while one that matches the box's maximum coordinate is considered to be
outside.

```c++
constexpr bool Box::empty() const noexcept;
```

True if the box has zero volume.

```c++
constexpr vector_type Box::shape() const noexcept;
```

Returns a vector giving the dimensions of the box. All elements will be
positive or zero. `Box(base(),shape())` reproduces the original box, even if
`base()` and `shape()` do not match the original point and vector from which
the box was constructed (because the original vector had one or more negative
elements).

```c++
std::string Box::str(const FormatSpec& spec = {}) const;
std::ostream& operator<<(std::ostream& out, const Box& b);
```

The `str()` function formats a box as a string, in the form
`"B[base]+[shape]"`. The `spec` argument is interpreted in the usual way for
`T`. The output operator calls `b.str()`.

```c++
constexpr T Box::volume() const noexcept;
```

Returns the box's volume (area if `N=2`).

```c++
constexpr Box& Box::operator+=(const vector_type& v) noexcept;
constexpr Box& Box::operator-=(const vector_type& v) noexcept;
constexpr Box operator+(const Box& a, const vector_type& b) noexcept;
constexpr Box operator+(const vector_type& a, const Box& b) noexcept;
constexpr Box operator-(const Box& a, const vector_type& b) noexcept;
```

Adding or subtracting a vector moves the box by that vector.

```c++
constexpr bool operator==(const Box& a, const Box& b) noexcept;
constexpr bool operator!=(const Box& a, const Box& b) noexcept;
```

Comparison operators. Boxes with the same `base()` and `shape()` will compare
equal (with the usual caveats about floating point comparison), even if they
were not constructed from the same point and vector.

```c++
constexpr static Box Box::unit() noexcept;
```

Returns the unit cube (or square), with corners at `[0,0,...]` and
`[1,1,...]`.

## Sphere class

```c++
template <std::floating_point T, int N> class Sphere;
```

Represents a sphere in `N` dimensions.

```c++
using Sphere_f2 = Sphere<float, 2>;
using Sphere_f3 = Sphere<float, 3>;
using Sphere_f4 = Sphere<float, 4>;
using Sphere_d2 = Sphere<double, 2>;
using Sphere_d3 = Sphere<double, 3>;
using Sphere_d4 = Sphere<double, 4>;
using Sphere_ld2 = Sphere<long double, 2>;
using Sphere_ld3 = Sphere<long double, 3>;
using Sphere_ld4 = Sphere<long double, 4>;
```

Predefined type aliases.

```c++
using Sphere::value_type = T;
using Sphere::real_type = [see below];
using Sphere::vector_type = Vector<T, N>;
```

Member types. The `real_type` member type is the same as `T` if `T` is
floating point, or `double` if `T` is an integer.

```c++
static constexpr int Sphere::dim = N;
```

Member constants.

```c++
constexpr Sphere::Sphere() noexcept;
constexpr Sphere::Sphere(const vector_type& c, T r) noexcept;
```

The default constructor creates a sphere of zero radius at the origin. The
second constructor creates a sphere with the given centre and radius
(the absolute value of the radius is used).

```c++
constexpr Sphere::Sphere(const Sphere& s) noexcept;
constexpr Sphere::Sphere(Sphere&& s) noexcept;
constexpr Sphere::~Sphere() noexcept;
constexpr Sphere& Sphere::operator=(const Sphere& s) noexcept;
constexpr Sphere& Sphere::operator=(Sphere&& s) noexcept;
```

Other life cycle functions.

```c++
constexpr vector_type Sphere::centre() const noexcept;
constexpr T Sphere::radius() const noexcept;
```

The sphere's centre and radius. The `radius()` function will always return a
positive or zero value, even if a negative radius was supplied to the
constructor.

```c++
constexpr bool Sphere::contains(const vector_type& p) const noexcept;
constexpr bool Sphere::contains(const Sphere& s) const noexcept;
```

The first version is true if the given point is inside the sphere (a point on
the surface of a sphere is not considered to be inside it). The second
version is true the sphere `s` is entirely inside `*this` (only points inside
the spheres are considered, so this will still be true if the sphere's
surfaces make contact).

```c++
constexpr bool Sphere::disjoint(const Sphere& s) const noexcept;
```

True if the spheres have no points in common (only points inside the spheres
are considered, so this will still be true if the sphere's surfaces make
contact).

```c++
constexpr bool Sphere::empty() const noexcept;
```

True if the radius is zero.

```c++
std::string Sphere::str(const FormatSpec& spec = {}) const;
std::ostream& operator<<(std::ostream& out, const Sphere& s);
```

The `str()` function formats a sphere as a string, in the form
`"S[centre]+radius"`. The `spec` argument is interpreted in the usual way for
`T`. The output operator calls `s.str()`.

```c++
real_type Sphere::volume() const noexcept;
real_type Sphere::surface() const noexcept;
```

Return the sphere's volume or equivalent (area for a 2-sphere, i.e. a circle),
and the surface area or equivalent (circumference for a circle).

```c++
constexpr Sphere& Sphere::operator*=(T t) noexcept;
constexpr Sphere& Sphere::operator/=(T t) noexcept;
constexpr Sphere operator*(const Sphere& a, T b) noexcept;
constexpr Sphere operator*(T a, const Sphere& b) noexcept;
constexpr Sphere operator/(const Sphere& a, T b) noexcept;
```

Scalar arithmetic operators. Multiplying or dividing by a scalar scales the
radius by that factor, without moving the centre. Division by zero is
undefined behaviour.

```c++
constexpr Sphere& Sphere::operator+=(const vector_type& v) noexcept;
constexpr Sphere& Sphere::operator-=(const vector_type& v) noexcept;
constexpr Sphere operator+(const Sphere& a, const vector_type& b) noexcept;
constexpr Sphere operator+(const vector_type& a, const Sphere& b) noexcept;
constexpr Sphere operator-(const Sphere& a, const vector_type& b) noexcept;
```

Vector arithmetic operators. Adding or subtracting a vector moves the sphere
by that vector.

```c++
constexpr bool operator==(const Sphere& a, const Sphere& b) noexcept;
constexpr bool operator!=(const Sphere& a, const Sphere& b) noexcept;
```

Comparison operators.

```c++
constexpr static Sphere Sphere::unit() noexcept;
```

Returns the unit sphere (or circle), with centre at the origin and radius 1.
