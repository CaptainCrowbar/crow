# UUID

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/uuid.hpp"
namespace Crow;
```

## UUID class

```c++
class Uuid;
```

This class holds a standard 16 byte universally unique identifier (UUID).

```c++
constexpr Uuid::Uuid() noexcept;
```

The default constructor sets all bytes to zero.

```c++
constexpr Uuid::Uuid(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
    uint8_t i, uint8_t j, uint8_t k, uint8_t l, uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept;
```

This constructor accepts an explicit list of 16 bytes.

```c++
constexpr Uuid::Uuid(const void* ptr, size_t len) noexcept;
```

This constructor copies bytes from the given location. If `n<16`, the extra
space is filled with zeros; if `n>16`, the extra data is ignored. Behaviour
is undefined if the pointer is null.

```c++
explicit Uuid::Uuid(const std::string& str);
```

This constructor parses the string representation of a UUID. It expects the
string to hold exactly 32 hex digits, in groups each containing an even
number of digits (optionally prefixed with `"0x"`), and will treat any
characters that are not ASCII alphanumerics as delimiters. It will throw
`std::invalid_argument` if the string is not a valid UUID.

```c++
constexpr explicit Uuid::Uuid(Uint128 u) noexcept;
```

Constructor from a 128-bit integer. This copies the integer's representation
in little-endian order.

```c++
constexpr Uuid::Uuid(const Uuid& u) noexcept;
constexpr Uuid::Uuid(Uuid&& u) noexcept;
constexpr Uuid::~Uuid() noexcept;
constexpr Uuid& Uuid::operator=(const Uuid& u) noexcept;
constexpr Uuid& Uuid::operator=(Uuid&& u) noexcept;
```

Other life cycle functions.

```c++
constexpr uint8_t& Uuid::operator[](size_t i) noexcept;
constexpr const uint8_t& Uuid::operator[](size_t i) const noexcept;
constexpr uint8_t* Uuid::begin() noexcept;
constexpr const uint8_t* Uuid::begin() const noexcept;
constexpr uint8_t* Uuid::end() noexcept;
constexpr const uint8_t* Uuid::end() const noexcept;
```

The index operator and the `begin()` and `end()` functions grant access to the
byte representation. Behaviour is undefined if the index to `operator[]` is
greater than 15.

```c++
constexpr Uint128 Uuid::as_uint() const noexcept;
constexpr explicit Uuid::operator Uint128() const noexcept;
```

Conversion to a 128-bit integer. This copies the UUID into the integer's
representation in little-endian order.

```c++
constexpr size_t Uuid::hash() const noexcept;
struct std::hash<Uuid>;
```

Hash functions.

```c++
std::string Uuid::str() const;
explicit Uuid::operator std::string() const;
std::ostream& operator<<(std::ostream& out, const Uuid& u);
```

Format the UUID in the standard broken down hex representation, e.g.
`"01234567-89ab-cdef-0123-456789abcdef"`.

```c++
std::strong_ordering operator<=>(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator==(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator!=(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator<(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator>(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator<=(const Uuid& u, const Uuid& v) noexcept;
constexpr bool operator>=(const Uuid& u, const Uuid& v) noexcept;
```

The comparison operators perform the natural bytewise lexicographical
comparisons.
