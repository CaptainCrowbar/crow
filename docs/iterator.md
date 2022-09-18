# Iterator Types and Algorithms

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/iterator.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Iterator classes

```c++
template <typename Container> class AppendIterator;
```

An output iterator that appends items to the container.

```c++
template <typename Container> AppendIterator<Container> append(Container& c);
template <typename Container> AppendIterator<Container> overwrite(Container& c);
```

The `append()` function returns an append iterator for the container. The
`overwrite()` function is a convenience function that clears the container
and then returns an append iterator.

```c++
template <typename Iterator> class DereferenceIterator;
template <typename Iterator> auto dereference_iterator(Iterator i);
template <typename Range> auto dereference_range(Range& r);
```

`DereferenceIterator` is a thin wrapper around `Iterator` that provides all of
the same behaviour(iterator category and mutability), except that it adds
another level of dereferencing over the original iterator's value type, which
must be a type with a unary `*` operator. For example, if `Iterator` has
value type `T*`, then `DereferenceIterator` has value type `T`.

```c++
template <typename T> class IotaIterator {
    IotaIterator();
    explicit IotaIterator(T start);
    IotaIterator(T start, T delta);
};
template <typename T>
    Irange<IotaIterator<T>> iota_range(T stop);
template <typename T>
    Irange<IotaIterator<T>> iota_range(T start, T stop);
template <typename T>
    Irange<IotaIterator<T>> iota_range(T start, T stop, T delta);
```

A forward iterator over a sequence of integers, or an integer-like type. `T`
must have an addition operator. If an iterator or range constructor is used
that does not supply all three parameters, an explicit conversion from `int`
to `T` must exist.

The first version of `iota_range()` iterates from zero to `stop-1`. The second
version iterates from `start` to `stop-1`, or from `start` down to `stop+1`
if `start>stop`. The third version iterates from `start` to `stop-delta` in
increments of `delta`. Behaviour is undefined if `stop-start` is not a
multiple of `delta`.

## Mixin classes

```c++
template <typename T, typename CV> class InputIterator;
template <typename T> class OutputIterator;
template <typename T, typename CV> class ForwardIterator;
template <typename T, typename CV> class BidirectionalIterator;
template <typename T, typename CV> class RandomAccessIterator;
template <typename T, typename CV, typename Category> class FlexibleIterator:

```

These are intended to be used as base classes, following the well known CRTP
pattern, and will supply the boilerplate to define several common member or
non-member functions given certain pre-defined functions of the derived type
`T`. `CV` is either `V` or `const V`, where `V` is the value type; this
determines whether a mutable or const iterator is generated.

The `FlexibleIterator` class can be used for iterators whose category varies
depending on static conditions. Behaviour is undefined if `Category` is not
one of the standard iterator category tags.

In the table below, `t` and `u` are objects of type `T` (the derived iterator
type), `x` is an object of the iterator's value type, and `n` is an integer.

| Mixin                    | Requires                    | Defines                                                                  |
| -----                    | --------                    | -------                                                                  |
| `InputIterator`          | `*t ++t t==u`               | `t-> t++ t!=u`                                                           |
| `OutputIterator`         | `t=x`                       | `*t ++t t++`                                                             |
| `ForwardIterator`        | `*t ++t t==u`               | `t-> t++ t!=u`                                                           |
| `BidirectionalIterator`  | `*t ++t --t t==u`           | `t-> t++ t-- t!=u`                                                       |
| `RandomAccessIterator`   | `*t t+=n t-u`               | `t-> t[n] ++t t++ --t t-- t-=n t+n n+t t-n t==u t!=u t<u t>u t<=u t>=u`  |
| `FlexibleIterator`       | `*t ++t --t t+=n t-u t==u`  | `t-> t[n] t++ t-- t+n n+t t-=n t-n t!=u t<u t>u t<=u t>=u`               |

## Range classes

```c++
template <typename Iterator> struct Irange {
    Iterator first, second;
    Iterator begin() const { return first; }
    Iterator end() const { return second; }
    bool empty() const noexcept { return first == second; }
};
template <typename Iterator> Irange<Iterator>
    irange(Iterator i, Iterator j);
template <typename Iterator> Irange<Iterator>
    irange(std::pair<Iterator, Iterator> p);
```

This turns a pair of iterators into a usable range.

```c++
template <typename Range>
    Irange<RangeIterator<Range>> subrange(Range& range, int offset);
template <typename Range>
    Irange<RangeIterator<Range>> subrange(Range& range, int offset,
        int offset2);
```

Return a range representing a subset of the original range. If `offset` is
positive or zero, it's counted from the start of the range; if it's negative,
it's counted backwards from the end of the range. If `offset2` is positive or
zero, it's counted from the `offset` position; if it's negative, it's counted
backwards from the end of the range. If an offset would be past the end of
the original range, the subrange is truncated to the original range. If a
negative offset would reach a position before the beginning of the subrange,
an empty subrange is returned.

Behaviour is undefined if either offset is negative and the iterator type is
not at least bidirectional.
