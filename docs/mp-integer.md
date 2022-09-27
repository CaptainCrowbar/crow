# Multiple Precision Integers

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/mp-integer.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Multiple precision unsigned integer class

```c++
class MPN;
```

An arbitrary precision unsigned integer (natural number). Most of its methods
are conventional arithmetic operations with their natural behaviour.

```c++
MPN::MPN();
```

The default constructor sets the value to zero.

```c++
MPN::MPN(uint64_t x);
```

Constructor from a 64-bit value.

```c++
explicit MPN::MPN(std::string_view s, int base = 0);
```

The constructor from a string accepts base of 0 or 2-36; it will throw
`std::invalid_argument` if the base is not one of these. Base 0 will check
for a `"0b"` or `"0x"` prefix, assuming decimal otherwise. Input is case
insensitive; apostrophe delimiters are allowed.

```c++
MPN::MPN(const MPN& m);
MPN::MPN(MPN&& m) noexcept;
MPN::~MPN() noexcept;
MPN& MPN::operator=(const MPN& m);
MPN& MPN::operator=(MPN&& m) noexcept;
```

Other life cycle functions.

```c++
template <typename T> explicit MPN::operator T() const;
```

The explicit cast template returns the value converted to `T`, if possible.
`T` must be a primitive integer or floating point arithmetic type. Results
are unspecified if the value is outside the return type's representable
range.

```c++
explicit MPN::operator bool() const noexcept;
```

True if the value is not zero.

```c++
bool MPN::operator!() const noexcept;
MPN MPN::operator+() const;
MPN& MPN::operator++();
MPN MPN::operator++(int);
MPN& MPN::operator--();
MPN MPN::operator--(int);
MPN& MPN::operator+=(const MPN& y);
MPN& MPN::operator-=(const MPN& y);
MPN& MPN::operator*=(const MPN& y);
MPN& MPN::operator/=(const MPN& y);
MPN& MPN::operator%=(const MPN& y);
MPN operator+(const MPN& x, const MPN& y);
MPN operator-(const MPN& x, const MPN& y);
MPN operator*(const MPN& x, const MPN& y);
MPN operator/(const MPN& x, const MPN& y);
MPN operator%(const MPN& x, const MPN& y);
```

Arithmetic operators. For division operations, behaviour is undefined if the
divisor is zero.

```c++
MPN& MPN::operator&=(const MPN& y);
MPN& MPN::operator|=(const MPN& y);
MPN& MPN::operator^=(const MPN& y);
MPN& MPN::operator<<=(ptrdiff_t y);
MPN& MPN::operator>>=(ptrdiff_t y);
MPN operator&(const MPN& x, const MPN& y);
MPN operator|(const MPN& x, const MPN& y);
MPN operator^(const MPN& x, const MPN& y);
MPN operator<<(const MPN& x, size_t y);
MPN operator>>(const MPN& x, size_t y);
```

Bitwise operators.

```c++
std::strong_ordering operator<=>(const MPN& x, const MPN& y) noexcept;
bool operator==(const MPN& x, const MPN& y) noexcept;
bool operator!=(const MPN& x, const MPN& y) noexcept;
bool operator<(const MPN& x, const MPN& y) noexcept;
bool operator>(const MPN& x, const MPN& y) noexcept;
bool operator<=(const MPN& x, const MPN& y) noexcept;
bool operator>=(const MPN& x, const MPN& y) noexcept;
```

Comparison operators.

```c++
size_t MPN::bits() const noexcept;
size_t MPN::bits_set() const noexcept;
```

The `bits()` function returns the number of bits in the number's binary
representation, starting with the highest 1 bit; `bits_set()` returns the
number of 1 bits.

```c++
size_t MPN::bytes() const noexcept;
```

The `bytes()` function returns the number of bytes needed to hold the number.

```c++
int MPN::compare(const MPN& y) const noexcept;
```

Returns a three way comparison of `*this` and `y`, returning -1 if
`*this<y`, 0 if `*this=y`, and 1 if `*this>y`.

```c++
void MPN::flip_bit(size_t i);
bool MPN::get_bit(size_t i) const noexcept;
void MPN::set_bit(size_t i, bool b = true);
uint8_t MPN::get_byte(size_t i) const noexcept;
void MPN::set_byte(size_t i, uint8_t b);
```

The `get_bit/byte()`, `set_bit/byte()`, and `flip_bit()` functions read or
write a specific bit or byte, selected by zero based index from the least
significant end. If the index is too big for the stored value,
`get_bit/byte()` will return zero or false, while `set_bit/byte()` and
`flip_bit()` will extend the value to include any bits newly set to 1.

```c++
size_t MPN::hash() const noexcept;
struct std::hash<MPN>;
```

Hash functions.

```c++
bool MPN::is_even() const noexcept;
bool MPN::is_odd() const noexcept;
```

Parity functions.

```c++
MPN MPN::pow(const MPN& n) const;
```

Raises `*this` to the power of `n`.

```c++
int MPN::sign() const noexcept;
```

Returns the sign of `*this` (0 if zero, otherwise 1).

```c++
std::string MPN::str(Format::FormatSpec spec = {}) const;
std::ostream& operator<<(std::ostream& out, const MPN& x);
```

Formatting functions. `MPN::str()` respects the same flags as standard
integers.

```c++
void MPN::write_be(void* ptr, size_t n) const noexcept;
void MPN::write_le(void* ptr, size_t n) const noexcept;
static MPN MPN::read_be(const void* ptr, size_t n);
static MPN MPN::read_le(const void* ptr, size_t n);
```

These read and write an integer from a block of bytes, in big or little endian
order. The write functions will truncate the integer if it is too big to fit
in the specified number of bytes.

```c++
std::pair<MPN, MPN> divide(const MPN& x, const MPN& y);
```

Returns the quotient and remainder of `x/y`. Behaviour is undefined if the
divisor is zero.

```c++
static MPN MPN::from_double(double x);
```

Converts a `double` to an integer. Fractions are rounded down.

## Multiple precision signed integer class

```c++
class MPZ;
```

An arbitrary precision signed integer. Most of its operations do the same
thing as the corresponding functions on `MPN` or plain `int`.

```c++
using MPQ = Ratio<MPZ>;
```

An arbitrary precision rational number.

```c++
MPZ::MPZ();
```

The default constructor sets the value to zero.

```c++
MPZ::MPZ(int64_t x);
```

Constructor from a 64-bit value.

```c++
MPZ::MPZ(const MPN& x);
```

Conversion constructor from an unsigned multiple precision integer.

```c++
explicit MPZ::MPZ(std::string_view s, int base = 0);
```

The constructor from a string accepts base of 0 or 2-36; it will throw
`std::invalid_argument` if the base is not one of these. Base 0 will check
for a `"0b"` or `"0x"` prefix, assuming decimal otherwise. Input is case
insensitive; apostrophe delimiters are allowed.

```c++
MPZ::MPZ(const MPZ& m);
MPZ::MPZ(MPZ&& m) noexcept;
MPZ::~MPZ() noexcept;
MPZ& MPZ::operator=(const MPZ& m);
MPZ& MPZ::operator=(MPZ&& m) noexcept;
```

Other life cycle functions.

```c++
template <typename T> explicit MPZ::operator T() const;
```

The explicit cast template returns the value converted to `T`, if possible.
`T` must be a primitive integer or floating point arithmetic type. Results
are unspecified if the value is outside the return type's representable
range.

```c++
explicit MPZ::operator MPN() const;
```

The explicit conversion to `MPN` returns the absolute value of the number.

```c++
explicit MPZ::operator bool() const noexcept;
```

True if the value is not zero.

```c++
bool MPZ::operator!() const noexcept;
MPZ MPZ::operator+() const;
MPZ MPZ::operator-() const;
MPZ& MPZ::operator++();
MPZ MPZ::operator++(int);
MPZ& MPZ::operator--();
MPZ MPZ::operator--(int);
MPZ& MPZ::operator+=(const MPZ& y);
MPZ& MPZ::operator-=(const MPZ& y);
MPZ& MPZ::operator*=(const MPZ& y);
MPZ& MPZ::operator/=(const MPZ& y);
MPZ& MPZ::operator%=(const MPZ& y);
MPZ operator+(const MPZ& x, const MPZ& y);
MPZ operator-(const MPZ& x, const MPZ& y);
MPZ operator*(const MPZ& x, const MPZ& y);
MPZ operator/(const MPZ& x, const MPZ& y);
MPZ operator%(const MPZ& x, const MPZ& y);
```

Arithmetic operators. The division operators perform Euclidean division: if
the division is not exact, the remainder is always positive regardless of the
signs of the arguments, and the quotient is the integer that satisfies
`x=q*y+r`. Behaviour is undefined if the divisor is zero.

```c++
std::strong_ordering operator<=>(const MPZ& x, const MPZ& y) noexcept;
bool operator==(const MPZ& x, const MPZ& y) noexcept;
bool operator!=(const MPZ& x, const MPZ& y) noexcept;
bool operator<(const MPZ& x, const MPZ& y) noexcept;
bool operator>(const MPZ& x, const MPZ& y) noexcept;
bool operator<=(const MPZ& x, const MPZ& y) noexcept;
bool operator>=(const MPZ& x, const MPZ& y) noexcept;
```

Comparison operators.

```c++
MPN MPZ::abs() const;
```

Returns the absolute value of `*this`.

```c++
int MPZ::compare(const MPZ& y) const noexcept;
```

Returns a three way comparison of `*this` and `y`, returning -1 if
`*this<y`, 0 if `*this=y`, and 1 if `*this>y`.

```c++
size_t MPZ::hash() const noexcept;
struct std::hash<MPZ>;
```

Hash functions.

```c++
bool MPZ::is_even() const noexcept;
bool MPZ::is_odd() const noexcept;
```

Parity functions.

```c++
MPZ MPZ::pow(const MPZ& n) const;
```

Raises `*this` to the power of `n`. Behaviour is undefined if the exponent is
negative.

```c++
int MPZ::sign() const noexcept;
```

Returns the sign of `*this` (-1 if negative, 0 if zero, 1 if positive).

```c++
std::string MPZ::str(Format::FormatSpec spec = {}) const;
std::ostream& operator<<(std::ostream& out, const MPZ& x);
```

Formatting functions. `MPZ::str()` respects the same flags as standard
integers.

```c++
static std::pair<MPZ, MPZ> divide(const MPZ& x, const MPZ& y);
```

Returns the quotient and remainder of `x/y`. Division works as described above.

```c++
static MPZ MPZ::from_double(double x);
```

Converts a `double` to an integer. Fractions are rounded down.

## Integer literals

```c++
namespace Literals {
    MPN operator""_mpn(const char* raw);
    MPZ operator""_mpz(const char* raw);
}
```

Multiple precision integer literals. These allow literals such as `12345_mpz`
or `0xabcdef_mpn`. They perform the same conversions as the constructors with
base 0.
