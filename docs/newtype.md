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
template <typename T, typename Tag = void, Ntype Flags = Ntype::none>
    class Newtype;
```

This template wrapper creates a "hard type alias" -- a new type with the same
behaviour as `T`, as far as possible in C++. The new type will usually
support all the same operators as `T`, apart from the dereference operators,
which are used to access the underlying `T` value.

Most of the member functions of `T` can be called using the arrow operator. A
few are explicitly implemented as special cases, providing direct member
functions of `Newtype`(for example, if `T` has a `substr()` function, we
expect `Newtype::substr()` to return another `Newtype` instead of the plain
`T` that `Newtype->substr()` would return).

Explicit conversion operators always exist in both directions between `T` and
`Newtype<T>`, and between different instantiations of `Newtype` for the same
`T`.

The second template argument, `Tag`, can be used to distinguish different
instantiations with the same underlying type. This can be any type, and does
not need to be complete; only its identity is used. The convenience
definition `Ntag<int>` is supplied to make it easy to define multiple aliases
of the same type. When aliases from different sources may be used together
and distinguishing them is necessary, incomplete dummy types can be created
to give aliases a unique identity.

The third template argument, `Flags`, specifies optional behaviour for the
alias:

* `Ntype::compare` -- If this is supplied, heterogeneous comparison operators
  between `Newtype<T>` and `T` will be defined. By default, only homogeneous
  comparison operators are defined. This has no effect if `T` is not
  comparable.
* `Ntype::construct` -- If this is supplied, an implicit conversion
  constructor from `T` to `Newtype<T>` is defined. By default, this
  conversion is explicit.
* `Ntype::convert` -- If this is supplied, an implicit conversion operator
  from `Newtype<T>` to `T` is defined. By default, this conversion is
  explicit. Combining `construct` and `convert` is legal but likely to lead
  to ambiguous overload resolution issues.

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
[optionally explicit] Newtype::Newtype(T&& t);
Newtype& Newtype::operator=(const T& t);
Newtype& Newtype::operator=(T&& t);
```

Conversions from a `T`. The explicit conversion constructors are always
defined; implicit conversions and assignment operators are defined if the
`construct` flag is present and `T` has the necessary properties.

```c++
template <typename Tag2, Ntype F2>
    explicit Newtype::Newtype(const Newtype<T, Tag2, F2>& nt);
template <typename Tag2, Ntype F2>
    explicit Newtype::Newtype(Newtype<T, Tag2, F2>&& nt);
```

Conversion constructors between different hard type aliases based on the same
underlying type. These are always defined if the corresponding operation on
`T` is defined, and always explicit.

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

Hash functions. These are defined if `std::hash<T>` is defined, and return the
same value.

### Range access

```c++
[value type]& Newtype::operator[](size_t i);
const [value type]& Newtype::operator[](size_t i) const;
[value type] Newtype::operator[](size_t i) const;
```

Indexing operator. Defined if the corresponding operator for `T` is defined.
The `const` version can return either a reference or a value, whichever is
returned by the underlying operator.

```c++
[iterator] Newtype::begin();
[iterator] Newtype::begin() const;
[iterator or sentinel] Newtype::end();
[iterator or sentinel] Newtype::end() const;
```

Range access functions. Defined if `std::ranges::begin/end(T)` are defined,
and returning the same type as those functions.

```c++
size_t Newtype::size() const;
bool Newtype::empty() const;
```

Range property functions. Defined if `std::ranges::size/empty(T)` are defined.

### String functions

```c++
Newtype Newtype::substr(size_t pos, size_t len = npos) const;
```

Defined if `T::substr()` is defined. This returns a `Newtype` wrapped around
the substring, instead of the raw `T` substring that `Newtype->substr()` would
return.
