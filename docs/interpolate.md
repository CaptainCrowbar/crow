# Interpolation and Shaping Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/interpolate.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class Logs: uint8_t {
    none  = 0,
    X     = 1,
    Y     = 2,
    XY    = X | Y,
};
```

Bitmask flags to indicate which of the X and Y scales are logarithmic.

## Concepts

```c++
template <typename X, typename Y, Logs L>
concept LerpTypes =
    std::floating_point<X>
    && (has_bit(L, Logs::Y) ? std::floating_point<Y> : std::regular<Y>)
    && requires (X x, Y y) {
        { y + x * (y - y) } -> std::convertible_to<Y>;
    };
template <typename X, typename Y, Logs L>
concept ConstexprLerpTypes =
    LerpTypes<X, Y, L>
    && requires {
        { std::bool_constant<(Y() + X() * (Y() - Y()), true)>() }
            -> std::same_as<std::true_type>;
    };
```

Requirements on the argument types for linear or logarithmic interpolation.

`LerpTypes` detects whether the types `X` and `Y` are valid argument types
with the given flags. The X type must always be floating point. For linear or
log-linear interpolation, the Y type only requires that expressions of the
form _Y=Y+X(Y-Y)_ are valid (note that, while the subexpression _(Y-Y)_ must
be valid, it is not required to return a Y as long as the expression as a
whole is valid). For lin-log or log-log interpolation, both types must be
floating point.

`ConstexprLerpTypes` is true if `LerpTypes` is true and the relevant
expressions involving `X` and `Y` are `constexpr` (using [Fedor's trick][1] to
detect a `constexpr` expression in a concept).

[1]: https://stackoverflow.com/questions/63326542/checking-for-constexpr-in-a-concept

## Interpolation functions

For each function in this section that is not marked `constexpr`, a
`constexpr` version is also defined if the argument types meet the
requirements of the `ConstexprLerpTypes` concept.

### Two-point interpolation

```c++
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y lerp(Y y1, Y y2, X x);
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y lerp(X x1, Y y1, X x2, Y y2, X x);
```

Interpolate or extrapolate to a third point, given two points. The first
version interpolates between `(0,y1)` and `(1,y2)`. Behaviour is undefined if
`x1==x2`.

```c++
template <Logs L = Logs::none>
struct Lerp {
    static constexpr Logs flags = L;
    template <typename X, typename Y>
        requires (LerpTypes<X, Y, L>)
            Y operator()(Y y1, Y y2, X x) const;
    template <typename X, typename Y>
        requires (LerpTypes<X, Y, L>)
            Y operator()(X x1, Y y1, X x2, Y y2, X x) const;
};
```

Linear or logarithmic interpolation functions provided as a function object.
If no `Logs` flags are used, these are identical to the `lerp()` functions
above. If one or both of the `Logs` flags is used, behaviour is undefined if
any of the corresponding arguments are less than or equal to zero.

### Related functions

```c++
template <std::floating_point X>
    constexpr X smoothstep(X x) noexcept;
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y smoothstep(Y y1, Y y2, X x);
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y smoothstep(X x1, Y y1, X x2, Y y2, X x);
```

The standard `smoothstep()` function that interpolates between two fixed
values using a smooth curve with the formula _3x<sup>2</sup>-2x<sup>3</sup>_.
The first version interpolates between _(0,0)_ and _(1,1),_ the second between
_(0,y1)_ and _(1,y2)._

```c++
template <std::floating_point X>
    constexpr X smootherstep(X x) noexcept;
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y smootherstep(Y y1, Y y2, X x);
template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
        Y smootherstep(X x1, Y y1, X x2, Y y2, X x);
```

Ken Perlin's `smootherstep()` function, which uses the formula
_6x<sup>5</sup>-15x<sup>4</sup>+10x<sup>3</sup>_. This has zero first and
second order derivatives and its endpoints.

## Interpolated associative containers

### Simple interpolated map

```c++
template <typename X, typename Y = X, Logs L = Logs::none>
    requires (LerpTypes<X, Y, L>)
class LerpMap {
    using key_type = X;
    using value_type = Y;
    static constexpr Logs flags = L;
    LerpMap();
    LerpMap(std::initializer_list<std::pair<const X, Y>> list);
    explicit LerpMap(const std::vector<std::pair<X, Y>> points);
    LerpMap& insert(X x, Y y);
    Y operator[](X x) const;
};
```

This is a map containing a sequence of key-value pairs. The indexing operator
performs interpolation between the nearest two points (or extrapolation if
the argument is outside the mapped range). If two points have the same X
value but different Y values, a later entry will overwrite an earlier one.

The constructors and `insert()` function will throw `std::invalid_argument` if
a zero or negative value is supplied for a log-scaled parameter. If the
`Logs::X` flag is used, behaviour is undefined if the argument to `operator[]`
is less than or equal to zero.

### Piecewise interpolated map

```c++
template <typename X, typename Y = X, Logs L = Logs::none>
    requires (LerpTypes<X, Y, L>)
class SegmentMap {
    using key_type = X;
    using value_type = Y;
    static constexpr Logs flags = L;
    SegmentMap();
    SegmentMap(std::initializer_list<init_type> list);
    Y operator[](X x) const;
    void clear() noexcept;
    bool empty() const noexcept;
    void insert(X x, Y y);
    void insert(X x, Y y1, Y y2);
    void insert(X x, Y y1, Y y2, Y y3);
    void erase(X x) noexcept;
    void erase(X x1, X x2) noexcept;
    X min() const noexcept;
    X max() const noexcept;
};
```

A `SegmentMap` holds a list of `(X,Y)` pairs, returning an interpolated Y
value for an arbitrary X input.

Each X value can be associated with up to three Y values. If two values are
supplied, the first value is used for arguments less than X, the second for
arguments greater than X; an argument exactly equal to X will return the
average of the two values (geometric mean if `Logs::Y` is set). If three
values are supplied, the first value is used for arguments less than X, the
second for an argument equal to X, and the third for arguments greater than
X.

The constructor that takes an initializer list expects a list of braced
tuples, each containing one X value and one to three Y values, interpreted in
the same way as the `insert()` functions described below.

Example:

```c++
SegmentMap<double> map = {
    {10, 100},            // insert(10, 100)
    {20, 200, 300, 400},  // insert(20, 200, 300, 400)
    {30, 500, 600},       // insert(30, 500, 600)
};
```

The lookup operator returns the interpolated Y value corresponding to the
given X. If the map is empty (no `(X,Y)` pairs have been supplied since
default construction or the last call to `clear()`), a default constructed Y
will be returned. If the X value is out of range (less than the smallest value
supplied so far, or greater than the largest), the Y value from the nearest
end of the range will be returned. If the `Logs::X` flag is used, behaviour is
undefined if the argument to `operator[]` is less than or equal to zero.

The first version of `insert()` adds a simple `(X,Y)` pair (overwriting an
existing pair if there was one with the same value of X). The second and
third versions allow a stepwise change in the output as described above. If
two points have the same X value but different Y values, a later entry will
overwrite an earlier one. The `insert()` functions (and the constructors)
will throw `std::invalid_argument` if a zero or negative value is supplied
for a log-scaled parameter.

The first version of `erase()` deletes only an entry with this exact X value,
otherwise doing nothing. The second version erases all entries with X values
in the closed interval `[x1,x2]` (it will do nothing if `x1>x2`).

The `min()` and `max()` functions return the range of X values for which a
mapping has been defined. They will return zero if the map is empty.

### Cubic spline interpolated map

```c++
template <std::floating_point T, Logs L = Logs::none>
class CubicSplineMap {
    using value_type = T;
    static constexpr Logs flags = L;
    CubicSplineMap();
    CubicSplineMap
        (std::initializer_list<std::pair<T, T>> list);
    explicit CubicSplineMap
        (const std::vector<std::pair<T, T>> points);
    T operator[](T x) const noexcept;
};
```

This performs cubic spline interpolation. Its behaviour is otherwise similar
to `LerpMap`. The constructors will throw `std::invalid_argument` if a zero or
negative value is supplied for a log-scaled parameter, or if less than four
distinct points are supplied. If the `Logs::X` flag is used, behaviour is
undefined if the argument to `operator[]` is less than or equal to zero.
