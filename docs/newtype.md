# Hard Type Alias

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/newtype.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
template <int N> using Ntag = std::integral_constant<int, N>;
```

Convenience definition for simple integer-based tag types.

```c++
enum class Ntype: uint8_t {
    none       = 0,
    compare    = 1, // Generate comparison operators between T and Newtype
    construct  = 2, // Generate implicit conversion from T to Newtype
    convert    = 4, // Generate implicit conversion from Newtype to T
};
```

Bitmask enumeration for flags selecting features of the `Newtype` template.
All of the usual bitwise operators are defined.

## Newtype class template

```c++
template <typename T, typename Tag = Ntag<0>, Ntype Flags = Ntype::none>
    class Newtype;
```

TODO

### Parameter visibility

```c++
using Newtype::underlying_type = T;
using Newtype::tag_type = Tag;
```

Member types.

```c++
static constexpr Ntype Newtype::flags = Flags;
```

Member constants.

### Life cycle operations

```c++
Newtype::Newtype();
```

Default constructor. This is only defined if `T` is default constructible.

```c++
Newtype::Newtype(const Newtype& nt);
Newtype& Newtype::operator=(const Newtype& nt);
```

Copy operations. These are only defined if `T` is copyable.

```c++
Newtype::Newtype(Newtype&& nt);
Newtype& Newtype::operator=(Newtype&& nt);
```

Move operations. These are only defined if `T` is movable.

```c++
[optionally explicit] Newtype::Newtype(const T& t);
```

Conversion constructor from a `T`. The explicit conversion constructor is
always defined; an implicit conversion constructor is defined if the
`construct` flag is present.

```c++
template <typename Tag2, Ntype F2>
    explicit Newtype::Newtype(const Newtype<T, Tag2, F2>& nt);
```

Conversion constructors between different hard type aliases based on the same
underlying type. These are always defined and always explicit.

```c++
explicit Newtype::Newtype(TS&&... args);
```

Constructor from an arbitrary argument list. This is defined for any given
argument list if the corresponding constructor for `T` exists, and is always
explicit regardless of whether the underlying `T` constructor is explicit.

```c++
Newtype::~Newtype() noexcept;
```

Destructor.

### Conversion operators

```c++
[optionally explicit] Newtype::operator T() const;
```

Conversion operator to a `T`. The explicit conversion operator is always
defined; an implicit conversion operator is defined if the `convert` flag is
present.

```c++
[optionally explicit] Newtype::operator bool() const;
```

Conversion operator to `bool`. This is defined if the corresponding conversion
is defined for `T`, and will have the same explicitness.

### Access operators

```c++
T& Newtype::operator*() noexcept;
const T& Newtype::operator*() const noexcept;
T* Newtype::operator->() noexcept;
const T* Newtype::operator->() const noexcept;
```

Obtain a reference to the underlying `T` object.

### Arithmetic operators

```c++
Newtype Newtype::operator+() const;
Newtype Newtype::operator-() const;
Newtype& Newtype::operator++();
Newtype Newtype::operator++(int);
Newtype& Newtype::operator--();
Newtype Newtype::operator--(int);
Newtype& Newtype::operator+=(const Newtype& y);
Newtype& Newtype::operator-=(const Newtype& y);
Newtype& Newtype::operator*=(const Newtype& y);
Newtype& Newtype::operator/=(const Newtype& y);
Newtype& Newtype::operator%=(const Newtype& y);
Newtype operator+(const Newtype& x, const Newtype& y);
Newtype operator-(const Newtype& x, const Newtype& y);
Newtype operator*(const Newtype& x, const Newtype& y);
Newtype operator/(const Newtype& x, const Newtype& y);
Newtype operator%(const Newtype& x, const Newtype& y);
```

Arithmetic operators. Each of these is defined if the corresponding operator
is defined for `T`. Only homogeneous operators are defined.

### Bitwise operators

```c++
Newtype Newtype::operator~() const;
Newtype& Newtype::operator&=(const Newtype& y);
Newtype& Newtype::operator|=(const Newtype& y);
Newtype& Newtype::operator^=(const Newtype& y);
Newtype& Newtype::operator<<=(int y);
Newtype& Newtype::operator>>=(int y);
Newtype operator&(const Newtype& x, const Newtype& y);
Newtype operator|(const Newtype& x, const Newtype& y);
Newtype operator^(const Newtype& x, const Newtype& y);
Newtype operator<<(const Newtype& x, int y);
Newtype operator>>(const Newtype& x, int y);
```

Bitwise operators. Each of these is defined if the corresponding operator is
defined for `T`. Only homogeneous operators are defined, except for the shift
operators.

### Comparison operators

```c++
bool operator==(const Newtype& x, const Newtype& y);
bool operator!=(const Newtype& x, const Newtype& y);
bool operator<(const Newtype& x, const Newtype& y);
bool operator>(const Newtype& x, const Newtype& y);
bool operator<=(const Newtype& x, const Newtype& y);
bool operator>=(const Newtype& x, const Newtype& y);
[comparison type] operator<=>(const Newtype& x, const Newtype& y);
```

Homogeneous comparison operators. Each of these is defined if the
corresponding operator is defined for `T`. The three way comparison operator
returns the same type as the operator for `T`.

```c++
bool operator==(const Newtype& x, const T& y);
bool operator==(const T& x, const Newtype& y);
bool operator!=(const Newtype& x, const T& y);
bool operator!=(const T& x, const Newtype& y);
bool operator<(const Newtype& x, const T& y);
bool operator<(const T& x, const Newtype& y);
bool operator>(const Newtype& x, const T& y);
bool operator>(const T& x, const Newtype& y);
bool operator<=(const Newtype& x, const T& y);
bool operator<=(const T& x, const Newtype& y);
bool operator>=(const Newtype& x, const T& y);
bool operator>=(const T& x, const Newtype& y);
[comparison type] operator<=>(const Newtype& x, const T& y);
[comparison type] operator<=>(const T& x, const Newtype& y);
```

Homogeneous comparison operators. Each of these is defined if the `compare`
flag is present and the corresponding operator is defined for `T`.

### Formatting functions

```c++
std::istream& operator>>(std::istream& in, Newtype& x);
```

Input stream operator. Defined if the corresponding operator is defined for
`T`.

```c++
std::ostream& operator<<(std::ostream& out, const Newtype& x);
```

Output stream operator. Defined if the corresponding operator is defined for
`T`, or if `T` is a [`FixedFormatType`](format.html).

```c++
std::string Newtype::str() const;
std::string Newtype::str(const FormatSpec& spec) const;
```

String formatting functions. Defined if `T` is a
[`FixedFormatType`](format.html) or [`VariableFormatType`](format.html)
respectively.

### Hash functions

```c++
size_t Newtype::hash() const noexcept;
class std::hash<Newtype>;
```

Hash functions. Defined if `std::hash<T>` is defined.

### Range access

```c++
[value type]& Newtype::operator[](size_t i);
const [value type]& Newtype::operator[](size_t i) const;
[value type] Newtype::operator[](size_t i) const;
```

TODO

```c++
[iterator] Newtype::begin();
[iterator] Newtype::begin() const;
[iterator] Newtype::end();
[iterator] Newtype::end() const;
```

TODO

```c++
size_t Newtype::size() const;
bool Newtype::empty() const;
```

TODO

### String functions

```c++
Newtype Newtype::substr(size_t pos, size_t len = npos) const;
```

TODO
