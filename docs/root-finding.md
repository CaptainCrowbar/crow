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
    T error() const noexcept;
    int count() const noexcept;
protected:
    RootFinder();
    virtual T do_solve(T y, T x1, T x2) = 0;
    void set_error(T e) noexcept;
    void set_count(int n) noexcept;
};
```

The base class for root finding algorithms.

The `solve()` functions perform the actual root finding, solving `f(x)=y`.
These call the virtual `do_solve()` function, which implements the actual root
finding algorithm in derived classes. For algorithms that require two initial
values, if only one is supplied, `x+1` will be used as the second initial
value. For algorithms that require only one initial value, the second one is
ignored. Implementations may throw `std::invalid_argument` if it is possible
to detect that the problem cannot be solved from the given initial values.

The `epsilon` parameter defines the maximum error allowed before the root
finder reports success. The default epsilon is 10<sup>-4</sup> if `T` is
`float`, or 10<sup>-10</sup> if `T` is `double` or `long double`. Behaviour
is undefined if `epsilon<=0`.

The `limit` parameter defines the maximum number of cycles of the root finding
algorithm to run before giving up. The default limit is 100. Calling `solve()`
will yield meaningless results if `limit<1`.

The `error()` and `count()` functions retrieve the actual error(absolute value
of the difference between `f(x)` and `y`) and number of cycles for the last
call to `solve()`. Initially they will both return 0.

## Newton-Raphson algorithm

```c++
template <std::floating_point T, typename F, typename DF>
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
