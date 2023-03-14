# Mathematical Utilities

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/maths.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Arithmetic functions

```c++
template <ArithmeticType T> constexpr T binomial(T a, T b) noexcept;
```

Returns the binomial coefficient of `(a,b)`, equal to `a!/b!(a-b)!` if
`b∈[0,a]`, otherwise zero). Behaviour is undefined if the correct result would
be out of range for `T`, or if `T` is floating point and either argument is
not an integer value.

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
the range `(-|y|/2,|y|/2]`. Behaviour is undefined if `y=0` or if the correct
result is outside the range of `T`.

The argument type is not constrained here in order to allow these to be used
with non-standard arithmetic types.

```c++
template <typename T, std::integral U>
    constexpr T integer_power(T x, U y);
template <typename T, std::integral U, typename BinaryFunction>
    constexpr T integer_power(T x, U y, BinaryFunction f, T unit = T(1));
```

Raise `x` to the power of `y`, using an _O(log y)_ algorithm. `T` can be any
type supported by the binary function, which defaults to multiplication. `U`
must be a standard integer type. If `y` is zero, this will return `unit`,
regardless of the value of `x`; behaviour is undefined if `y` is negative.
The function and unit arguments must be supplied if `T` is not constructible
from an integer.

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

## Numerical properties

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
template <ArithmeticType T> std::pair<T, T> emodf(T x) noexcept;
template <ArithmeticType T> T fraction(T x) noexcept;
```

A Euclidean version of `modf()`, returning the integer and fractional parts of
a number, with the fractional part always in the range `[0,1)`. If `T` is an
integer type, this will return `{x,0}`. The `fraction()` function is
shorthand for `emodf(x).second`.

```c++
template <typename T> constexpr int sign_of(T t) noexcept;
```

Returns the sign of `t` (-1, 0, or 1). `T` must be default constructible and
less-than comparable; the default constructed value is assumed to be zero.
Behaviour is undefined if `t` has an unordered value such as NaN.

## Special functions

```c++
template <std::floating_point T> T inverse_erf(T x) noexcept;
template <std::floating_point T> T inverse_erfc(T x) noexcept;
```

Inverse error function and complementary error function. Arguments of -1 or 1
for `inverse_erf(),` or 0 or 2 for `inverse_erfc(),` will correctly return
infinities. Behaviour is undefined if the argument is outside the domain
`[-1,1]` for `inverse_erf(),` or `[0,2]` for `inverse_erfc().`

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
