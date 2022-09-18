# Mathematical Utilities

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/maths.hpp"
namespace Crow;
```

This header contains some utility functions that are not directly related to
graphics, but are used internally by the graphics library and may be useful
to users of it.

## Contents

* TOC
{:toc}

## Algorithms

Unless otherwise stated, these are defined for all primitive arithmetic types.
Most of them do not statically check this, to allow them to be used with
non-standard integer types.

```c++
template <typename T> constexpr T const_abs(T x) noexcept;
```

Absolute value function (defined here because `std::abs()` is not guaranteed
to be `constexpr`).

```c++
template <typename T2, typename T1> constexpr T2 const_round(T1 x) noexcept;
```

Round to nearest integer (defined here because `std::lround()` is not
guaranteed to be `constexpr`). `T1` must be a floating point type; `T2` must
be an integer type (and must be explicitly specified). Behaviour is undefined
if the result is not representable in the integer type.

```c++
template <typename T> constexpr
    std::pair<T, T> euclidean_divide(T x, T y) noexcept;
template <typename T> constexpr
    T euclidean_quotient(T x, T y) noexcept;
template <typename T> constexpr
    T euclidean_remainder(T x, T y) noexcept;
```

These perform Euclidean division, where the remainder is always in the range
`[0,|y|)`. `T` may be any integer or floating point arithmetic type. Behaviour
is undefined if `y=0`, or if the correct result is outside the range of `T`.

```c++
template <typename T> constexpr T fraction(T x) noexcept;
```

Returns the fractional part of a floating point number (defined as
`x-floor(x)`; equivalent to `euclidean_remainder(x,1)`). For integer types
this will always return zero.

```c++
template <typename X, typename Y>
    constexpr Y interpolate(X x1, Y y1, X x2, Y y2, X x3) noexcept;
```

Given two points, `(x1,y1)` and `(x2,y2)`, and a third input argument `x3`,
this calculates the value `y3` by linear interpolation or extrapolation. `X`
must be a floating point arithmetic type; `Y` only needs to define `Y-Y` and
`X*Y`, and need not be an arithmetic type. Behaviour is undefined if `x1=x2`.

```c++
template <typename T> constexpr
    std::pair<T, T> symmetric_divide(T x, T y) noexcept;
template <typename T> constexpr
    T symmetric_quotient(T x, T y) noexcept;
template <typename T> constexpr
    T symmetric_remainder(T x, T y) noexcept;
```

These perform symmetric division, where the remainder is in the range
`(-|y|/2,|y|/2]`. Behaviour is undefined if `y<=0` or if the correct result is
outside the range of `T`.

```c++
template <typename T> constexpr T to_degrees(T rad) noexcept;
template <typename T> constexpr T to_radians(T deg) noexcept;
```

Convert between degrees and radians. `T` must be floating point.

## Literals

```c++
namespace Literals {
    constexpr float operator""_degf(long double x) noexcept;
    constexpr float operator""_degf(unsigned long long x) noexcept;
    constexpr double operator""_degd(long double x) noexcept;
    constexpr double operator""_degd(unsigned long long x) noexcept;
    constexpr long double operator""_degld(long double x) noexcept;
    constexpr long double operator""_degld(unsigned long long x) noexcept;
}
```

Angle literals. These take a number expressed in degrees and return a value in
radians (for example, `360_deg` yields `2π`).
