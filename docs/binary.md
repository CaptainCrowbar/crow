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

## Bitwise operations

All of these functions are defined only for integer types. Unless the
description says otherwise, behaviour is undefined if the argument is
negative.

```c++
template <typename T> constexpr int popcount(T t) noexcept;
```

Returns the number of 1 bits in the value.

```c++
template <typename T> constexpr int bit_width(T t) noexcept;
```

Returns the minimum number of bits required to contain the value;
equivalently, one plus the index of the highest 1 bit.

```c++
template <typename T> constexpr T bit_ceil(T t) noexcept;
template <typename T> constexpr T bit_floor(T t) noexcept;
```

The `bit_ceil()` function returns the smallest power of 2 greater than or
equal to the argument; `bit_floor()` returns the largest power of 2 less than
or equal to the argument(or 0 if the argument is 0). Behaviour is undefined
if the result of `bit_ceil()` would be too big to represent.

```c++
template <typename T> constexpr bool has_single_bit(T t) noexcept;
```

True if the value is an exact power of 2 (equivalent to `popcount(t)==1`).

```c++
template <typename T> constexpr T rotl(T t, int n) noexcept;
template <typename T> constexpr T rotr(T t, int n) noexcept;
```

Rotate the bits in the value left or right. The shift count can be arbitrarily
large, it is not restricted by the size of the argument. A negative shift
count will rotate in the opposite direction.

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
