# Fixed-Size Binary Numbers

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/fixed-binary.hpp"
namespace Crow;
```

## Fixed binary classes

```c++
template <size_t N> class SmallBinary;
template <size_t N> class LargeBinary;
template <size_t N> using Binary = [SmallBinary if N<=64, otherwise LargeBinary];
```

All of these classes represent unsigned integers with `N` bits. `SmallBinary`
stores its value in a single native integer of an appropriate size,
`LargeBinary` in an array of integers, in little endian order. Normally you
should just use the `Binary` alias; nearly all of the functions described
below are common to both templates (except that, where a function is
documented as taking a `Binary` argument by value, usually the `LargeBinary`
version actually takes its argument by reference).

```c++
using Uint128 = Binary<128>;
using Uint256 = Binary<256>;
using Uint512 = Binary<512>;
using Uint1024 = Binary<1024>;
```

Defined for convenience

```c++
static constexpr size_t Binary::bits = N;
static constexpr size_t Binary::bytes = [total bytes in representation];
static constexpr size_t Binary::hex_digits = [maximum number of hex digits in value];
```

Member constants.

```c++
constexpr Binary::Binary() noexcept;
```

The default constructor sets the value to zero.

```c++
constexpr Binary::Binary(uint64_t x) noexcept;
```

Constructor from a primitive unsigned integer.

```c++
template <size_t M> constexpr explicit SmallBinary::SmallBinary(SmallBinary<M> x) noexcept;
template <size_t M> constexpr explicit LargeBinary::LargeBinary(SmallBinary<M> x) noexcept;
template <size_t M> constexpr explicit LargeBinary::LargeBinary(const LargeBinary<M>& x) noexcept;
```

Constructor from another `Binary` object.

```c++
constexpr Binary::Binary(std::initializer_list<uint64_t> init) noexcept;
```

Constructor from a list of 64-bit integers, in big-endian order. Any excess
bits are discarded.

```c++
explicit Binary::Binary(const std::string& str);
```

The string constructor calls `parse_dec()` and follows the same rules.

```c++
constexpr Binary::Binary(const Binary& x) noexcept;
constexpr Binary::Binary(Binary&& x) noexcept;
constexpr Binary::~Binary() noexcept;
constexpr Binary& Binary::operator=(const Binary& x) noexcept;
constexpr Binary& Binary::operator=(Binary&& x) noexcept;
```

Other life cycle operations.

```c++
std::string Binary::bin() const;
std::string Binary::dec() const;
std::string Binary::hex() const;
std::string to_string(Binary x);
std::ostream& operator<<(std::ostream& out, Binary x);
```

String formatting functions. The output operator and `to_string()` function
call `dec()`. The `bin()` and `hex()` functions always return the same number
of digits (equal to `bits` and `hex_digits` respectively); `dec()` writes only
the number of digits needed, with no leading zeroes (unless the value is
zero).

```c++
constexpr void Binary::clear() noexcept;
```

Sets the value to zero.

```c++
constexpr uint8_t* Binary::data() noexcept;
constexpr const uint8_t* Binary::data() const noexcept;
```

Pointers to the internal representation, which will consist of a number of
bytes equal to the `bytes` constant, in little endian order.

```c++
template <typename T> constexpr bool Binary::fits_in() const noexcept;
```

True if the current value of the `Binary` will fit in a `T` without loss of
information. `T` must be a primitive arithmetic type.

```c++
constexpr size_t Binary::hash() const noexcept;
class std::hash<Binary<T>>;
```

Hash function.

```c++
constexpr size_t Binary::significant_bits() const noexcept;
```

Returns the number of significant bits in the value.

```c++
constexpr explicit Binary::operator bool() const noexcept;
```

True if the value is not zero.

```c++
template <typename T> constexpr explicit Binary::operator T() const noexcept;
template <size_t M> constexpr explicit LargeBinary::operator SmallBinary<M>() const noexcept;
```

Converts a fixed binary number into a standard integer or floating point type,
or another fixed binary type. The usual arithmetic overflow rules apply if
the value is out of range for the result type. `T` must be a primitive
arithmetic type, Behaviour is undefined if `T` is a signed integer and
`fits_in<T>()` is false.

```c++
SmallBinary Binary::operator+() const noexcept;
Binary Binary::operator-() const noexcept;
Binary& Binary::operator++() noexcept;
Binary Binary::operator++(int) noexcept;
Binary& Binary::operator--() noexcept;
Binary Binary::operator--(int) noexcept;
Binary& Binary::operator+=(Binary y) noexcept;
Binary& Binary::operator-=(Binary y) noexcept;
Binary& Binary::operator*=(Binary y) noexcept;
Binary& Binary::operator/=(Binary y) noexcept;
Binary& Binary::operator%=(Binary y) noexcept;
Binary operator+(Binary x, Binary y) noexcept;
Binary operator-(Binary x, Binary y) noexcept;
Binary operator*(Binary x, Binary y) noexcept;
Binary operator/(Binary x, Binary y) noexcept;
Binary operator%(Binary x, Binary y) noexcept;
```

Arithmetic operations. Most of these just have the obvious semantics, with the
usual wraparound behaviour on overflow or underflow. For all of the division
and remainder operations, behaviour is undefined if the divisor is zero.

```c++
Binary Binary::operator~() const noexcept;
Binary& Binary::operator&=(Binary y) noexcept;
Binary& Binary::operator|=(Binary y) noexcept;
Binary& Binary::operator^=(Binary y) noexcept;
Binary& Binary::operator<<=(int y) noexcept;
Binary& Binary::operator>>=(int y) noexcept;
Binary operator&(Binary x, Binary y) noexcept;
Binary operator|(Binary x, Binary y) noexcept;
Binary operator^(Binary x, Binary y) noexcept;
Binary operator<<(Binary x, int y) noexcept;
Binary operator>>(Binary x, int y) noexcept;
Binary rotl(Binary x, int y) noexcept;
Binary rotr(Binary x, int y) noexcept;
```

Bitwise operations. These have their usual semantics. The bit count argument
to the shift and rotate operations can be any value, including out of range
or negative values; a negative shift or rotate is interpreted as a positive
one in the opposite direction.

```c++
static void Binary::divide(Binary x, Binary y, Binary& q, Binary& r) noexcept;
```

Calculates the quotient and remainder of `x/y`. Behaviour is undefined if
`y==0`.

```c++
static Binary Binary::from_double(double x) noexcept;
```

Conversion from a `double` yields the equivalent of converting the significand
exactly and then shifting by a number of bits based on the exponent. Behaviour
is undefined if the argument is negative.

```c++
static Binary Binary::max() noexcept;
```

Returns the maximum value of the type (the complement of zero).

```c++
static Binary Binary::parse_bin(const std::string& str);
static Binary Binary::parse_dec(const std::string& str);
static Binary Binary::parse_hex(const std::string& str);
static bool Binary::try_parse_bin(const std::string& str, Binary& x) noexcept;
static bool Binary::try_parse_dec(const std::string& str, Binary& x) noexcept;
static bool Binary::try_parse_hex(const std::string& str, Binary& x) noexcept;
```

These attempt to parse a string as an unsigned integer, in base 2, 10, or 16.
The `parse_*()` functions return the parsed value, and throw
`std::invalid_argument` on failure; teh `try_parse_*()` functions modify the
supplied reference and return true on success, and return false on failure.

```c++
std::strong_ordering operator<=>(const Binary& a, const Binary& b) noexcept;
bool operator==(Binary x, Binary y) noexcept;
bool operator!=(Binary x, Binary y) noexcept;
bool operator<(Binary x, Binary y) noexcept;
bool operator>(Binary x, Binary y) noexcept;
bool operator<=(Binary x, Binary y) noexcept;
bool operator>=(Binary x, Binary y) noexcept;
```

Comparison operators.
