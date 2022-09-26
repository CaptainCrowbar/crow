# Mathematical Utilities

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/maths.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Algorithms

```c++
template <ArithmeticType T> constexpr T const_abs(T x) noexcept;
```

Absolute value function (defined here because `std::abs()` is not guaranteed
to be `constexpr`).

```c++
template <std::integral T2, std::floating_point T1>
    constexpr T2 const_round(T1 x) noexcept;
```

Round to nearest integer (defined here because `std::lround()` is not
guaranteed to be `constexpr`). Behaviour is undefined if the result is not
representable in `T2`.

```c++
template <typename T>
    constexpr std::pair<T, T> euclidean_divide(T x, T y) noexcept;
template <typename T>
    constexpr T euclidean_quotient(T x, T y) noexcept;
template <typename T>
    constexpr T euclidean_remainder(T x, T y) noexcept;
template <typename T>
    constexpr std::pair<T, T> symmetric_divide(T x, T y) noexcept;
template <typename T>
    constexpr T symmetric_quotient(T x, T y) noexcept;
template <typename T>
    constexpr T symmetric_remainder(T x, T y) noexcept;
```

The Euclidean division functions perform division in which the remainder is
always in the range `[0,|y|)`. Behaviour is undefined if `y=0`, or if the
correct result is outside the range of `T`.

The symmetric division functions perform division in which the remainder is in
the range `(-|y|/2,|y|/2]`. Behaviour is undefined if `y<=0` or if the correct
result is outside the range of `T`.

The argument type is not constrained here in order to allow these to be used
with non-standard integer types.

```c++
template <ArithmeticType T> constexpr T fraction(T x) noexcept;
```

Returns the fractional part of a floating point number (defined as
`x-floor(x)`; equivalent to `euclidean_remainder(x,1)`). For integer types
this will always return zero.

```c++
template <std::floating_point X, ArithmeticType Y>
    constexpr Y interpolate(X x1, Y y1, X x2, Y y2, X x3) noexcept;
```

Given two points, `(x1,y1)` and `(x2,y2)`, and a third input argument `x3`,
this calculates the value `y3` by linear interpolation or extrapolation. `X`
must be a floating point arithmetic type; `Y` only needs to define `Y-Y` and
`X*Y`, and need not be an arithmetic type. Behaviour is undefined if `x1=x2`.

```c++
template <std::floating_point T> constexpr T to_degrees(T rad) noexcept;
template <std::floating_point T> constexpr T to_radians(T deg) noexcept;
```

Convert between degrees and radians.

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
