# Transforms

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/transform.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Coordinate transformations

```c++
template <std::floating_point T> Vector<T, 2>
    cartesian_to_polar(const Vector<T, 2>& xy) noexcept;
        // (x,y) -> (r,θ)
template <std::floating_point T> Vector<T, 2>
    polar_to_cartesian(const Vector<T, 2>& rt) noexcept;
        // (r,θ) -> (x,y)
template <std::floating_point T> Vector<T, 3>
    cartesian_to_cylindrical(const Vector<T, 3>& xyz) noexcept;
        // (x,y,z) -> (ρ,φ,z)
template <std::floating_point T> Vector<T, 3>
    cartesian_to_spherical(const Vector<T, 3>& xyz) noexcept;
        // (x,y,z) -> (r,φ,θ)
template <std::floating_point T> Vector<T, 3>
    cylindrical_to_cartesian(const Vector<T, 3>& rpz) noexcept;
        // (ρ,φ,z) -> (x,y,z)
template <std::floating_point T> Vector<T, 3>
    cylindrical_to_spherical(const Vector<T, 3>& rpz) noexcept;
        // (ρ,φ,z) -> (r,φ,θ)
template <std::floating_point T> Vector<T, 3>
    spherical_to_cartesian(const Vector<T, 3>& rpt) noexcept;
        // (r,φ,θ) -> (x,y,z)
template <std::floating_point T> Vector<T, 3>
    spherical_to_cylindrical(const Vector<T, 3>& rpt) noexcept;
        // (r,φ,θ) -> (ρ,φ,z)
```

Transformations between coordinate systems in two or three dimensions.

## Projective geometry

```c++
template <std::floating_point T> Vector<T, 4>
    vector4(const Vector<T, 3>& v, T w) noexcept;
template <std::floating_point T> Vector<T, 4>
    point4(const Vector<T, 3>& v) noexcept;
template <std::floating_point T> Vector<T, 4>
    normal4(const Vector<T, 3>& v) noexcept;
```

Convert a 3-vector to a 4-vector. The `point4()` function sets `w` to 1;
`normal4()` sets it to zero.

```c++
template <std::floating_point T> Vector<T, 3>
    point3(const Vector<T, 4>& v) noexcept;
template <std::floating_point T> Vector<T, 3>
    normal3(const Vector<T, 4>& v) noexcept;
```

Convert a 4-vector to a 3-vector. The `point3()` function divides the first 3
coordinates by `w`, unless `w=0`; `normal3()` just discards `w` and returns
the truncated vector.

```c++
template <std::floating_point T, MatrixLayout L>
    Matrix<T, 4, L> make_transform(const Matrix<T, 3, L>& m,
        const Vector<T, 3>& v) noexcept;
```

Composes a `4x4` projective transformation matrix from a `3x3` matrix and a
translation vector.

    (a b c)             (a b c x)
    (d e f), (x y z) => (d e f y)
    (g h i)             (g h i z)
                        (0 0 0 1)

```c++
template <std::floating_point T, MatrixLayout L>
    Matrix<T, 4, L> normal_transform(const Matrix<T, 4, L>& m) noexcept;
```

Converts a point transform to a normal transform, returning the transpose of
the inverse of the matrix.

## Primitive transformations

```c++
template <std::floating_point T>
    Matrix<T, 3> rotate3(T angle, int index) noexcept;
template <std::floating_point T>
    Matrix<T, 4> rotate4(T angle, int index) noexcept;
```

Generate a rotation by the given angle as a 3D or projective matrix. The index
indicates the axis of rotation; behaviour is undefined if `index<0` or
`index>2`.

```c++
template <std::floating_point T> Matrix<T, 3>
    rotate3(T angle, const Vector<T, 3>& axis) noexcept;
template <std::floating_point T> Matrix<T, 4>
    rotate4(T angle, const Vector<T, 3>& axis) noexcept;
```

Generate a rotation by the given angle, about the given axis, as a 3D or
projective matrix. These will return an identity matrix if either argument is
null.

```c++
template <std::floating_point T>
    Matrix<T, 3> scale3(T t) noexcept;
template <std::floating_point T>
    Matrix<T, 3> scale3(const Vector<T, 3>& v) noexcept;
template <std::floating_point T>
    Matrix<T, 4> scale4(T t) noexcept;
template <std::floating_point T>
    Matrix<T, 4> scale4(const Vector<T, 3>& v) noexcept;
```

Generate a proportional or triaxial scaling transformation as a 3D or
projective matrix.

```c++
template <std::floating_point T> Matrix<T, 4>
    translate4(const Vector<T, 3>& v) noexcept;
```

Generates a translation as a projective matrix (equivalent to
`make_transform(Matrix<T,3>::identity(),v)`).

## Quaternion transformations

```c++
template <std::floating_point T> Vector<T, 3>
    rotate(const Quaternion<T>& q, const Vector<T, 3>& v) noexcept;
```

Apply the rotation represented by `q` to the vector `v`.

```c++
template <std::floating_point T> Quaternion<T>
    q_rotate(T angle, const Vector<T, 3>& axis) noexcept;
```

Generate the quaternion corresponding to a rotation by the given angle, about
the given axis.

```c++
template <std::floating_point T> Matrix<T, 3>
    rotate3(const Quaternion<T>& q) noexcept;
template <std::floating_point T> Matrix<T, 4>
    rotate4(const Quaternion<T>& q) noexcept;
```

Convert a quaternion into a 3-matrix or projective matrix representing the
same rotation.
