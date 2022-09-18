# Algorithms

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/algorithm.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Arithmetic algorithms

```c++
template <typename T> constexpr T binomial(T a, T b) noexcept;
```

Returns the binomial coefficient of `(a,b)`, equal to `a!/b!(a-b)!` if
`b∈[0,a]`, otherwise zero). `T` must be an arithmetic type (floating point
types are allowed). Behaviour is undefined if the correct result would be out
of range for `T`, or if `T` is floating point and either argument is not an
integer value.

```c++
template <typename T, typename U>
    constexpr T integer_power(T x, U y) noexcept;
template <typename T, typename U, typename BinaryFunction>
    constexpr T integer_power(T x, U y, BinaryFunction f,
        T unit = T(1)) noexcept;
```

Raise `x` to the power of `y`, using an _O(log y)_ algorithm. `T` can be any
type supported by the binary function, which defaults to multiplication. `U`
must be a standard integer type. If `y` is zero, this will return `unit`,
regardless of the value of `x`; behaviour is undefined if `y` is negative.
The function and unit arguments must be supplied if `T` is not constructible
from an integer.

## Container algorithms

```c++
template <typename Container, typename T>
    void remove_in(Container& con, const T& t);
template <typename Container, typename Predicate>
    void remove_in_if(Container& con, Predicate p);
template <typename Container, typename Predicate>
    void remove_in_if_not(Container& con, Predicate p);
template <typename Container>
    void unique_in(Container& con);
template <typename Container, typename BinaryPredicate>
    void unique_in(Container& con, BinaryPredicate eq);
template <typename Container>
    void sort_unique_in(Container& con);
template <typename Container, typename BinaryPredicate>
    void sort_unique_in(Container& con, BinaryPredicate cmp);
```

These carry out the same algorithms as the similarly named STL functions,
except that unwanted elements are removed from the container rather than
shuffled to the end. The `sort_unique_in()` functions perform a sort followed
by removing equivalent elements from the container; like `std::sort()`, its
predicate has less-than semantics, where that of `unique_in()`, like
`std::unique()`, has equality semantics.

## Difference

```c++
template <typename RandomAccessRange> struct DiffEntry {
    using iterator = [range const iterator];
    using subrange = Irange<iterator>;
    subrange del;
    subrange ins;
};
template <typename RandomAccessRange>
    using DiffList = std::vector<DiffEntry<RandomAccessRange>>;
```

Supporting types.

```c++
template <typename RandomAccessRange>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs,
        const RandomAccessRange& rhs);
template <typename RandomAccessRange, typename EqualityPredicate>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs,
        const RandomAccessRange& rhs, EqualityPredicate eq);
```

This is an implementation of the widely used diff algorithm, based on
[Eugene Myers' 1986 paper](http://xmailserver.org/diff2.pdf).

The return value is a list of diffs, each consisting of two pairs of
iterators. The `del` member is a subrange of `lhs` indicating which elements
have been removed, and the `ins` member is a subrange of `rhs` indicating
which elements have been inserted in the same location. At least one subrange
in each diff entry will be non-empty.

Complexity: _O(k(m+n))_, where _m_ and _n_ are the lengths of the input ranges
and _k_ is the number of differences.

## Edit distance

```c++
template <typename ForwardRange1, typename ForwardRange2>
    int edit_distance(const ForwardRange1& range1,
        const ForwardRange2& range2);
template <typename ForwardRange1, typename ForwardRange2, typename T>
    T edit_distance(const ForwardRange1& range1,
        const ForwardRange2& range2, T ins, T del, T sub);
```

These return the edit distance (Levenshtein distance) between two ranges,
based on the number of insertions, deletions, and substitutions required to
transform one range into the other. By default, each operation is given a
weight of 1; optionally, explicit weights can be given to each operation. The
weight type `T` must be an arithmetic type. Behaviour is undefined if any of
the weights are negative.

Complexity: _O(mn)_, where _m_ and _n_ are the lengths of the input ranges.

## Hash table comparison

```c++
template <typename T, typename Hash1, typename Hash2, typename Eq>
    int hash_compare(const std::unordered_set<T, Hash1, Eq>& a,
        const std::unordered_set<T, Hash2, Eq>& b);
template <typename T, typename Hash1, typename Hash2, typename Eq,
        typename Cmp>
    int hash_compare(const std::unordered_set<T, Hash1, Eq>& a,
        const std::unordered_set<T, Hash2, Eq>& b, Cmp cmp);
```

Performs a three-way comparison between hash sets, returning -1 if _a&lt;b,_ 0
if _a=b,_ or 1 if _a&gt;b._ This always gives the same result as if the sets
were sorted, even if the two sets are in different orders or have different
hash functions. The comparison predicate defaults to `std::less<T>`. Behaviour
is undefined if the equality predicates are different, or are inconsistent
with the comparison predicate.

Complexity: _O(n)_.

## Interpolation

For all of these templates, `T` must be a floating point arithmetic type.

```c++
enum Interpolate: int {
    log_x = 1,
    log_y = 2,
};
```

The interpolation functions take a flags parameter, which is a combination of
these bitmask flags, to indicate which of the X and Y scales are
logarithmic.

```c++
template <typename T>
    T interpolate(T x1, T y1, T x2, T y2, T x, int flags = 0) noexcept;
```

Interpolate or extrapolate to a third point, given two points. Behaviour is
undefined if `x1==x2`; if the `log_x` flag is used and any of the X values
are less than or equal to zero; or if the `log_y` flag is used and either of
the Y values is less than or equal to zero.

```c++
template <typename T, int Flags = 0> class InterpolatedMap {
    InterpolatedMap();
    InterpolatedMap(std::initializer_list<std::pair<const T, T>> list);
    explicit InterpolatedMap(const std::vector<std::pair<T, T>> points);
    InterpolatedMap& insert(T x, T y);
    T operator()(T x) const noexcept;
};
```

This is a map containing a sequence of key-value pairs. The function call
operator performs interpolation between the nearest two points
(or extrapolation if the argument is outside the mapped range). If two points
have the same X value but different Y values, a later entry will overwrite an
earlier one. The constructors and `insert()` function will throw
`std::invalid_argument` if a zero or negative value is supplied for a
log-scaled parameter.

```c++
template <typename T, int Flags = 0> class CubicSplineMap {
    CubicSplineMap();
    CubicSplineMap(std::initializer_list<std::pair<T, T>> list);
    explicit CubicSplineMap(const std::vector<std::pair<T, T>> points);
    T operator()(T x) const noexcept;
};
```

This performs cubic spline interpolation. Its behaviour is otherwise similar
to `InterpolatedMap`. In addition to the usual log scale checking, the
constructors will throw `std::invalid_argument` if less than four distinct
points are supplied, or if two points have the same X value but different Y
values.

## Numerical algorithms

```c++
template <typename T, typename F>
    T line_integral(T x1, T x2, int k, F f);
```

Computes the integral of `f(x)` over the interval `[x1,x2]` by the trapezoid
algorithm, using `k` subdivisions. Behaviour is undefined if `k<1` or the
function has a pole within the interval.

```c++
template <typename T, int N, typename F>
    T volume_integral(Vector<T, N> x1, Vector<T, N> x2, int k, F f);
```

Computes the volume integral of `f(x)` over the rectangular prism whose
opposite corners are `x1` and `x2`, dividing each side into `k` subdivisions.
This has complexity _O(k<sup>N</sup>)._ Behaviour is undefined if `k<1` or
the function has a pole within the volume.

```c++
template <typename T> class PrecisionSum {
    using value_type = T;
    PrecisionSum& add(T t);
    PrecisionSum& operator()(T t); // same as add()
    void clear() noexcept;
    T get() const;
    operator T() const; // same as get()
};
template <typename SinglePassRange>
    [value type] precision_sum(const SinglePassRange& range);
```

Calculate the sum of a sequence of numbers using the high precision algorithm from
[Shewchuk](http://www-2.cs.cmu.edu/afs/cs/project/quake/public/papers/robust-arithmetic.ps)
and [Hettinger](http://code.activestate.com/recipes/393090/).

This can be called as either an accumulator to which values can be added one
at a time, or a range based function that calculates the sum in one call. The
range's value type must be a floating point arithmetic type. This is always
much more accurate than simple addition, and is guaranteed to give the
correct answer (the exact sum correctly rounded) if the value type implements
IEEE arithmetic (on GCC this requires the `-ffloat-store` option).

## Range algorithms

```c++
template <typename ForwardRange, typename UnaryFunction, typename Compare>
    [iterator] find_optimum(ForwardRange& range, UnaryFunction f,
        Compare cmp);
template <typename ForwardRange, typename UnaryFunction>
    [iterator] find_optimum(ForwardRange& range, UnaryFunction f);
```

Return an iterator identifying the range element for which `f(x)` has the
maximum value, according to the given comparison function. The comparison
function is expected to have less-than semantics, and defaults to
`std::greater<T>()`, where `T` is the return type of `f()`. Use
`std::less<T>()` to get the minimum value of `f(x)` instead of the maximum. If
there is more than one optimum element, the first one will be returned. These
will return an end iterator only if the range is empty.

## Subsets

```c++
template <typename RandomAccessContainer> std::vector<RandomAccessContainer>
    subsets(const RandomAccessContainer& con);
template <typename RandomAccessContainer> std::vector<RandomAccessContainer>
    subsets(const RandomAccessContainer& con, int k);
```

Return a list of all subsets of the container, or all subsets of a given size.
Container elements are always treated as distinct, with no equality checking,
so these will always return exactly _2<sup>n</sup>_ or _C(n,k)=n!/k!(n-k)!_
subsets (where _n_ is the container size).

No promises are made about what order the subsets will be listed in. For the
second function, behaviour is undefined if `k<0` or `k>n`.

Complexity: _O(2<sup>n</sup>)_ for the first version; _O(kC(n,k))_ for the
second.
