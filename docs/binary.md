# Bitwise Operations

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/binary.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Bit manipulation

```c++
template <BitwiseType T>
    constexpr bool has_bit(T flags, T required) noexcept;
template <BitwiseType T>
    constexpr bool has_bits(T flags, T required) noexcept;
```

Test for the presence of specific bits. The `has_bit()` function returns true
if `flags` and `required` have any bits in common; `has_bits()` returns true
if `flags` contains all of the bits in `required`. Both functions will return
false if `required=0`.

## Byte order

```c++
template <PrimitiveScalarType T> constexpr T swap_ends(T t) noexcept;
```

Returns the value with reversed byte order.

```c++
template <PrimitiveScalarType T> constexpr T big_endian(T t) noexcept;
template <PrimitiveScalarType T> constexpr T little_endian(T t) noexcept;
```

These convert between big or little endian and the native byte order.

```c++
template <PrimitiveScalarType T> constexpr T read_be(const void* ptr) noexcept;
template <PrimitiveScalarType T> constexpr T read_le(const void* ptr) noexcept;
template <PrimitiveScalarType T> constexpr void write_be(T t, void* ptr) noexcept;
template <PrimitiveScalarType T> constexpr void write_le(T t, void* ptr) noexcept;
```

Read or write a value from a sequence of bytes, assuming it is stored in big
or little endian order. Behaviour is undefined if the pointer is null or
there is not enough space in the array being read or written.

## Overflow detection

```c++
template <std::integral T>
    constexpr std::optional<T> checked_add(T x, T y) noexcept;
template <std::integral T>
    constexpr std::optional<T> checked_subtract(T x, T y) noexcept;
```

These perform integer addition and subtraction with overflow detection. They
return the sum or difference if the operation is valid, or an empty value if
overflow or underflow occurs.
