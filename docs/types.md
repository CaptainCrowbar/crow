# Basic Types

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/types.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr size_t npos = std::string::npos;
```

Imported for convenience.

## Concepts

(Several of these are replacements for C++20 standard concepts that are not
yet supported by all compilers, and will be removed when this is fixed.)

```c++
template <typename T> concept ArithmeticType;
```

Satisfied by any integral or floating point type, except `bool`.

```c++
template <typename T> concept IteratorType;
```

Satisfied if `T` is an iterator.

```c++
template <typename T> concept RangeType;
```

Satisfied if `T` is a range, defined as having `begin(T)` and `end(T)`
functions that can be found either by ADL or in `namespace std`.

```c++
template <typename T> concept MaplikeRangeType;
```

Satisfied if `T` is a range, and its value type has `first` and `second` data
members.

```c++
template <typename T> concept InputIteratorType;
template <typename T> concept InputRangeType;
template <typename T> concept OutputIteratorType;
template <typename T> concept OutputRangeType;
template <typename T> concept ForwardIteratorType;
template <typename T> concept ForwardRangeType;
template <typename T> concept BidirectionalIteratorType;
template <typename T> concept BidirectionalRangeType;
template <typename T> concept RandomAccessIteratorType;
template <typename T> concept RandomAccessRangeType;
```

These indicate whether an iterator type, or the iterator type of a range,
meets the requirements for one of the standard iterator concepts. Each of
these will be satisfied if the iterator's category is at least equal to the
named category.

```c++
template <typename T> concept PrimitiveScalarType;
```

Satisfied if `T` is a primitive scalar type.

```c++
template <typename T, typename U> concept SameBasicType;
```

Satisfied if `T` and `U` are the same type apart from any CV and reference
qualifications.

```c++
template <typename T> concept ThreeWayComparable;
```

Satisfied if `T` supports the `<=>` operator.

## Comparison functions

```c++
template <std::totally_ordered T>
    std::strong_ordering compare3way(const T& a, const T& b) noexcept;
```

Performs a 3-way comparison by calling `operator<=>` if available, otherwise
`operator==` and `operator<`.

```c++
constexpr std::strong_ordering to_order(ArithmeticType auto t) noexcept;
```

Converts a number to a strong ordering value: negative numbers map to `less`,
zero to `equal`, and positive numbers to `greater`.

## Memory management types

```c++
struct FreeMem {
    void operator()(void* ptr) const;
};
```

A deleter (suitable for use with smart pointers) that calls `std::free()`.

## Metaprogramming support

```c++
template <typename T> using RangeIterator = [iterator type];
template <typename T> using RangeValue = [value type];
```

The iterator and value types of a range, or `void` if `T` is not a range.

```c++
template <typename T, bool B> struct SfinaeTrue {};
template <typename T> struct SfinaeTrue<T, true>: std::true_type {};
```

Creates an artificial dependency to allow `enable_if` to be used in contexts
that are not directly dependent.

```c++
template <typename T> constexpr bool dependent_false = false;
```

Allows compile time failure in an `if constexpr` branch.
