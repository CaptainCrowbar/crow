# Root Finding

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/root-finding.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Root finder base class

```c++
template <std::floating_point T> class RootFinder {
public:
    virtual ~RootFinder() noexcept;
    T solve(T y = 0, T x = 0);
    T solve(T y, T x1, T x2);
    T epsilon() const noexcept;
    void set_epsilon(T e) noexcept;
    int limit() const noexcept;
    void set_limit(int n) noexcept;
    bool strict() const noexcept;
    void set_strict(bool b = true) noexcept;
    T error() const noexcept;
    int count() const noexcept;
protected:
    RootFinder();
    virtual T do_solve(T y, T x1, T x2) = 0;
    void reset() noexcept;
    void set_error(T err) noexcept;
    void increment() noexcept;
};
```

The abstract base class for root finding algorithms.

The `solve()` functions perform root finding, solving `f(x)=y`. These call the
virtual `do_solve()` function, which implements the actual root finding
algorithm in derived classes. For algorithms that require two initial values,
if only one is supplied, `x+1` will be used as the second initial value. For
algorithms that require only one initial value, the second one is ignored.
Implementations may throw `std::domain_error` if it is possible to detect
that the problem cannot be solved from the given initial values, but such
detection cannot be guaranteed.

The `epsilon` parameter defines the maximum error allowed before the root
finder reports success. The default epsilon is 10<sup>-4</sup> if `T` is
`float`, otherwise 10<sup>-10</sup>. Behaviour is undefined if `epsilon<=0`.

The `limit` parameter defines the maximum number of cycles of the root finding
algorithm to run before giving up. The default limit is 100. Behaviour is
undefined if `limit<1`.

If the `strict` flag is set, the `solve()` functions will throw
`std::range_error` if the algorithm has not converged when the iteration
limit is reached. This is off by default.

The `error()` and `count()` functions retrieve the actual error (absolute
value of the difference between `f(x)` and `y`) and number of cycles for the
last call to `solve()`. Initially they will both return 0.

## Bisection algorithm

```c++
template <std::floating_point T, std::invocable<T> F>
requires requires (T t, F f) {
    { f(t) } -> std::convertible_to<T>;
}
class Bisection:
public RootFinder<T> {
    explicit Bisection(F f);
};
```

This implements the bisection algorithm to find a root of a function. The
`solve()` functions require two initial values. If neither of the initial
values is an immediate solution, `solve()` will throw `std::domain_error` if
`x1=x2`, or if `f(x1)-y` and `f(x2)-y` have the same sign.

```c++
template <std::floating_point T, std::invocable<T> F>
std::unique_ptr<RootFinder<T>> bisection(F f) {
    return std::make_unique<Bisection<T, F>>(f);
}
```

This function can be used to construct a root finder more simply (note that
the type `T` must still be specified explicitly).

## False position algorithm

```c++
template <std::floating_point T, std::invocable<T> F>
requires requires (T t, F f) {
    { f(t) } -> std::convertible_to<T>;
}
class FalsePosition:
public RootFinder<T> {
    explicit FalsePosition(F f);
};
```

This implements the false position algorithm to find a root of a function. The
`solve()` functions require two initial values. If neither of the initial
values is an immediate solution, `solve()` will throw `std::domain_error` if
`x1=x2`, or if `f(x1)-y` and `f(x2)-y` have the same sign.

```c++
template <std::floating_point T, std::invocable<T> F>
std::unique_ptr<RootFinder<T>> false_position(F f) {
    return std::make_unique<FalsePosition<T, F>>(f);
}
```

This function can be used to construct a root finder more simply (note that
the type `T` must still be specified explicitly).

## Newton-Raphson algorithm

```c++
template <typename T, typename F, typename DF>
concept NewtonRaphsonArgumentTypes =
    std::floating_point<T>
    && requires (T t, F f, DF df) {
        { f(t) } -> std::convertible_to<T>;
        { df(t) } -> std::convertible_to<T>;
    };
```

Requirements on the argument types for the Newton-Raphson algorithm.

```c++
template <std::floating_point T, typename F, typename DF>
requires (NewtonRaphsonArgumentTypes<T, F, DF>)
class NewtonRaphson: public RootFinder<T> {
    NewtonRaphson(F f, DF df);
};
```

This implements the
[Newton-Raphson](https://en.wikipedia.org/wiki/Newton%27s_method) algorithm to
find a root of a function. The arguments are the function and its derivative.

```c++
template <std::floating_point T, typename F, typename DF>
    std::unique_ptr<RootFinder<T>> newton_raphson(F f, DF df);
```

This function can be used to construct a root finder more simply (note that
the type `T` must still be specified explicitly).
