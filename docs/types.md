# Basic Types

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/types.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Target system properties

All of these are always defined as `constexpr bool` constants. They are set to
true or false to reflect the compiler and target system properties.

| Property                    | True constants                         |
| --------                    | --------------                         |
| **Byte order**              |                                        |
| Big endian                  | `big_endian_target`                    |
| Little endian               | `little_endian_target`                 |
| **CPU architecture**        |                                        |
| 32-bit ARM CPU              | `arm_cpu, arm_32_cpu`                  |
| 64-bit ARM CPU              | `arm_cpu, arm_64_cpu`                  |
| 32-bit x86 CPU              | `x86_cpu, x86_32_cpu`                  |
| 64-bit x86-64 CPU           | `x86_cpu, x86_64_cpu`                  |
| **Operating system**        |                                        |
| Apple operating system      | `unix_os, apple_os`                    |
| Linux                       | `unix_os, linux_os`                    |
| Other Unix                  | `unix_os`                              |
| Microsoft Windows           | `windows_os`                           |
| **Compiler**                |                                        |
| Clang (non-Apple versions)  | `gcc_family_compiler, clang_compiler`  |
| Clang/Xcode                 | `gcc_family_compiler, xcode_compiler`  |
| GCC                         | `gcc_family_compiler, gcc_compiler`    |
| Intel C++                   | `intel_compiler` _(see below)_         |
| Microsoft Visual Studio     | `microsoft_compiler`                   |

If `intel_compiler` is set, one of `gcc_family_compiler` or
`microsoft_compiler` may also be set, depending on the Intel compiler's
compatibility options.

## Other constants

```c++
constexpr size_t npos = std::string::npos;
```

Imported for convenience.

# Memory management types

```c++
struct FreeMem {
    void operator()(void* ptr) const;
};
```

A deleter (suitable for use with smart pointers) that calls `std::free()`.

## Mixin classes

These are intended to be used as base classes, following the well known CRTP
pattern, and will supply the boilerplate to define several common member or
non-member functions given certain pre-defined functions of the derived
type `T`.

```c++
template <typename T> class BinaryOperators;
```

Given some or all of the fused arithmetic assignment operators
`+= -= *= /= %= &= |= ^=` for `T`, this defines the corresponding plain
binary operators `+ - * / % & | ^`. The shift operators are not included
because the RHS is frequently not the same type.

```c++
template <typename T> class TotalOrder;
```

Given `==` and `<` operators for `T`, this defines the other comparison
operators: `!= > <= >=`.

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
