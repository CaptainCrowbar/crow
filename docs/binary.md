# Bitwise Operations

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/binary.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Byte order

```c++
template <typename T> constexpr T swap_ends(T t) noexcept;
```

Returns the value with reversed byte order. `T` must be a primitive scalar
type.

```c++
template <typename T> constexpr T big_endian(T t) noexcept;
template <typename T> constexpr T little_endian(T t) noexcept;
```

These convert between big or little endian and the native byte order. `T` must
be a primitive scalar type.

```c++
template <typename T> constexpr T read_be(const void* ptr) noexcept;
template <typename T> constexpr T read_le(const void* ptr) noexcept;
template <typename T> constexpr void write_be(T t, void* ptr) noexcept;
template <typename T> constexpr void write_le(T t, void* ptr) noexcept;
```

Read or write a scalar value from a sequence of bytes, assuming it is stored
in big or little endian order. Behaviour is undefined if the pointer is null
or there is not enough space in the array being read or written.

## Overflow detection

```c++
template <typename T>
    constexpr std::optional<T> checked_add(T x, T y) noexcept;
template <typename T>
    constexpr std::optional<T> checked_subtract(T x, T y) noexcept;
```

These perform integer addition and subtraction with overflow detection. They
return the sum or difference if the operation is valid, or an empty value if
overflow or underflow occurs. These will compile only for primitive integer
types.
