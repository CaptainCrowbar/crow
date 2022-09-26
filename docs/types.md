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

```c++
template <typename T> concept ArithmeticType;
```

Satisfied by any integral or floating point type, except `bool`.

```c++
template <typename T, typename U> concept SameBasicType;
```

Satisfied if `T` and `U` are the same type apart from any CV and reference
qualifications.

## Memory management types

```c++
struct FreeMem {
    void operator()(void* ptr) const;
};
```

A deleter (suitable for use with smart pointers) that calls `std::free()`.

## SFINAE support

```c++
template <typename T, bool B> struct SfinaeTrue {};
template <typename T> struct SfinaeTrue<T, true>: std::true_type {};
```

Creates an artificial dependency to allow `enable_if` to be used in contexts
that are not directly dependent.

## Static assert support

```c++
template <typename T> constexpr bool dependent_false = false;
```

Allows compile time failure in an `if constexpr` branch.

## Type traits

[TODO - most of these will be replaced with concepts]

```c++
template <typename T> constexpr bool is_iterator;
```

True if `T` is an iterator.

```c++
template <typename T> constexpr bool is_range;
```

True if `T` is a range, defined as having `begin(T)` and `end(T)` functions
that can be found either by ADL or in `namespace std`.

```c++
template <typename T> constexpr bool is_maplike_range;
```

True if `T` is a range, and its value type has `first` and `second` data
members.

```c++
template <typename T> using RangeIterator = [iterator type];
template <typename T> using RangeValue = [value type];
```

The iterator and value types of a range, or `void` if `T` is not a range.

```c++
template <typename T> constexpr bool is_input_iterator;
template <typename T> constexpr bool is_input_range;
template <typename T> constexpr bool is_output_iterator;
template <typename T> constexpr bool is_output_range;
template <typename T> constexpr bool is_forward_iterator;
template <typename T> constexpr bool is_forward_range;
template <typename T> constexpr bool is_bidirectional_iterator;
template <typename T> constexpr bool is_bidirectional_range;
template <typename T> constexpr bool is_random_access_iterator;
template <typename T> constexpr bool is_random_access_range;
```

These indicate whether an iterator type, or the iterator type of a range,
meets the requirements for one of the standard iterator concepts. Each of
these will be true if the iterator's category is at least equal to the named
category.
