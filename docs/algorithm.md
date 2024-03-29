# Algorithms

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/algorithm.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Container algorithms

### Map lookup

```c++
template <AssociativeContainerType C,
    std::convertible_to<typename C::key_type> K>
    typename C::mapped_type lookup(const C& map, const K& key);
template <AssociativeContainerType C,
    std::convertible_to<typename C::key_type> K,
    std::convertible_to<typename C::mapped_type> T>
    typename C::mapped_type lookup(const C& map, const K& key,
        const T& def);
```

Look up the key in the map, returning the mapped value if found, otherwise
either the supplied default value, or a default constructed value.

### Uniqueness algorithms

```c++
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

### Myers diff

```c++
template <RandomAccessRangeType Range> struct DiffEntry {
    using iterator = [range const iterator];
    using subrange = Irange<iterator>;
    subrange del;
    subrange ins;
};
template <RandomAccessRangeType Range>
    using DiffList = std::vector<DiffEntry<Range>>;
```

Supporting types.

```c++
template <RandomAccessRangeType Range>
    DiffList<Range> diff(const Range& lhs,
        const Range& rhs);
template <RandomAccessRangeType Range, std::equivalence_relation<...> ER>
    DiffList<Range> diff(const Range& lhs,
        const Range& rhs, ER eq);
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

### Levenshtein distance

```c++
template <ArithmeticType T = double> class Levenshtein {
    Levenshtein() noexcept;
    Levenshtein(T ins, T del, T sub);
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
        T operator()(const Range1& a, const Range2& b) const;
};
```

[Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance)
between two strings, based on the number of insertions, deletions, and
substitutions required to transform one range into the other. The constructor
arguments are the weights for insertion, deletion, and substitution
operations; all of these default to 1.

The constructor will throw `std::invalid_argument` if any of the weights are
negative.

Complexity: _O(mn)_, where _m_ and _n_ are the lengths of the input strings.

### Damerau–Levenshtein distance

```c++
template <ArithmeticType T = double> class DamerauLevenshtein {
    DamerauLevenshtein() noexcept;
    DamerauLevenshtein(T ins, T del, T sub, T exch);
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
        T operator()(const Range1& a, const Range2& b) const;
};
```

[Damerau-Levenshtein distance](https://en.wikipedia.org/wiki/Damerau–Levenshtein_distance)
between two strings, which allows insertion, deletion, substitution, as well
as transposition of adjacent characters. The constructor arguments are the
weights for insertion, deletion, substitution, and transposition operations;
all of these default to 1.

The constructor will throw `std::invalid_argument` if any of the weights are
negative, or if `2*exch<ins+del`.

Complexity: _O(mn)_, where _m_ and _n_ are the lengths of the input strings.

### Jaro–Winkler distance

```c++
template <std::floating_point T = double> class JaroWinkler {
    JaroWinkler() noexcept;
    JaroWinkler(size_t prefix, T threshold, T scale);
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
        T operator()(const Range1& a, const Range2& b) const;
};
```

[Jaro-Winkler distance](https://en.wikipedia.org/wiki/Jaro–Winkler_distance)
between two strings, which gives a higher weight to matches within a prefix of
the strings. This is normalized to a unit scale, returning 0 for no similarity
and 1 for identical strings.

The constructor arguments are:

* `prefix` = Prefix length to be checked by the Winkler modification (default 4)
* `threshold` = Apply the Winkler modification only if the distance is within this threshold (default 0.3)
* `scale` = Scale factor applied to the Winkler modification (default 0.1)

The constructor will throw `std::invalid_argument` if any of the weights are
negative, if `threshold>1`, or if `scale*prefix>1`.

Complexity: _O(mn)_, where _m_ and _n_ are the lengths of the input strings.

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

## Numerical algorithms

### Line and volume integrals

```c++
template <std::floating_point T, std::invocable<T> F>
    T line_integral(T x1, T x2, int k, F f);
```

Computes the integral of `f(x)` over the interval `[x1,x2]` by the trapezoid
algorithm, using `k` subdivisions. Behaviour is undefined if `k<1` or the
function has a pole within the interval.

```c++
template <std::floating_point T, int N, std::invocable<T> F>
    T volume_integral(Vector<T, N> x1, Vector<T, N> x2, int k, F f);
```

Computes the volume integral of `f(x)` over the rectangular prism whose
opposite corners are `x1` and `x2`, dividing each side into `k` subdivisions.
This has complexity _O(k<sup>N</sup>)._ Behaviour is undefined if `k<1` or
the function has a pole within the volume.

### Precision sum

```c++
template <std::floating_point T> class PrecisionSum {
    using value_type = T;
    PrecisionSum& add(T t);
    PrecisionSum& operator()(T t); // same as add()
    void clear() noexcept;
    T get() const;
    operator T() const; // same as get()
};
template <InputRangeType Range>
    [value type] precision_sum(const Range& range);
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

### Cartesian power

```c++
template <ForwardRangeType Range> class CartesianPowerIterator {
    using difference_type = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::vector<[range value type]>;
    using pointer = const value_type*;
    using reference = const value_type&;
    CartesianPowerIterator();
    CartesianPowerIterator(const Range& range, size_t k);
    const value_type& operator*() const noexcept;
    const value_type* operator->() const noexcept;
    CartesianPowerIterator& operator++();
    CartesianPowerIterator operator++(int);
    bool operator==(const CartesianPowerIterator& i) const noexcept;
    bool operator!=(const CartesianPowerIterator& i) const noexcept;
};
template <ForwardRangeType Range>
    Irange<CartesianPowerIterator<Range>>
        cartesian_power(const Range& range, size_t k);
```

This iterates over the Cartesian power `k` of the input range, i.e.
`range×range×...×range`. The dereferenced vector contains `k` elements from
the input range. The output range contains _n<sup>k</sup>_ elements, where
`n` is the size of the input range. The output elements are in their natural
lexical order, based on the order of the input range, with the last element
varying quickest.

### Find optimum

```c++
template <ForwardRangeType Range, std::invocable<...> UnaryFunction,
        std::strict_weak_ordering<...> Compare>
    [iterator type] find_optimum(Range& range, UnaryFunction f,
        Compare cmp);
template <ForwardRangeType Range, std::invocable<...> UnaryFunction>
    [iterator type] find_optimum(Range& range, UnaryFunction f);
```

Return an iterator identifying the range element for which `f(x)` has the
maximum value, according to the given comparison function. The comparison
function defaults to `std::greater<T>()`, where `T` is the return type of
`f()`. Use `std::less<T>()` to get the minimum value of `f(x)` instead of the
maximum. If there is more than one optimum element, the first one will be
returned. These will return an end iterator only if the range is empty.

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

Complexity: _O(2<sup>n</sup>)_ for the first version; _O(k.C(n,k))_ for the
second.
