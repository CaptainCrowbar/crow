# Probability

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/probability.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Probability class

```c++
template <std::floating_point T> class Probability;
```

This represents a probability, stored in a floating point value of type `T`.
The representation is optimised for values close to 0 or 1; values close to 1
will be preserved much more accurately than they would be in a raw `T`
value.

```c++
constexpr Probability::Probability() noexcept;
```

The default constructor sets the value to zero.

```c++
constexpr Probability::Probability(T t) noexcept;
```

The argument is clamped to the range `[0,1]`.

```c++
constexpr Probability::Probability(const Probability& p) noexcept;
constexpr Probability::Probability(Probability&& p) noexcept;
constexpr Probability::~Probability() noexcept;
constexpr Probability&
    Probability::operator=(const Probability& p) noexcept;
constexpr Probability&
    Probability::operator=(Probability&& p) noexcept;
```

Other life cycle operations.

```c++
constexpr T Probability::value() const noexcept;
constexpr explicit Probability::operator T() const noexcept;
```

Return the probability value.

```c++
constexpr T Probability::complement() const noexcept;
```

Returns the complement of the value `(1-value).` If the actual value is close
to 1, this will be more accurate than explicitly calculating `1-value()`.

```c++
static constexpr Probability Probability::complementary(T t) noexcept;
```

Returns the complementary probability `(1-t).` If `t` is close to zero, this
will store a more accurate value than explicitly constructing
`Probability(1-t).`

```c++
std::string Probability::str(FormatSpec spec = {}) const;
std::ostream& operator<<(std::ostream& out, Probability x);
```

Format the probability as a string. The format spec follows the same rules as
for ordinary floating point arguments, except that the default formatting
mode is `"pz6"`.

```c++
T Probability::z() const noexcept;
static Probability Probability::from_z(T z) noexcept;
```

Convert between a probability and the corresponding standard normal variate.
Probability values equal to 0 or 1, or `z` values equal to plus or minus
infinity, are handled correctly.

```c++
constexpr Probability Probability::operator~() const noexcept;
```

Returns the complementary probability `(1-p).`

```c++
constexpr Probability& Probability::operator+=(Probability y) noexcept;
constexpr Probability& Probability::operator-=(Probability y) noexcept;
constexpr Probability& Probability::operator*=(T y) noexcept;
constexpr Probability& Probability::operator/=(T y) noexcept;
constexpr Probability operator+(Probability x, Probability y) noexcept;
constexpr Probability operator-(Probability x, Probability y) noexcept;
constexpr Probability operator*(Probability x, T y) noexcept;
constexpr Probability operator*(T y, Probability x) noexcept;
constexpr Probability operator/(Probability x, T y) noexcept;
```

Arithmetic operators. These all have the natural meaning; the output is always
clamped to `[0,1]`. Where practical, arguments and results close to 1 will be
handled more accurately than simple arithmetic on the values. Behaviour is
undefined if the second argument to the division operator is zero.

```c++

constexpr bool operator==(Probability x, Probability y) noexcept;
constexpr bool operator==(Probability x, T y) noexcept;
constexpr bool operator==(T x, Probability y) noexcept;
constexpr bool operator!=(Probability x, Probability y) noexcept;
constexpr bool operator!=(Probability x, T y) noexcept;
constexpr bool operator!=(T x, Probability y) noexcept;
constexpr bool operator<(Probability x, Probability y) noexcept;
constexpr bool operator<(Probability x, T y) noexcept;
constexpr bool operator<(T x, Probability y) noexcept;
constexpr bool operator>(Probability x, Probability y) noexcept;
constexpr bool operator>(Probability x, T y) noexcept;
constexpr bool operator>(T x, Probability y) noexcept;
constexpr bool operator<=(Probability x, Probability y) noexcept;
constexpr bool operator<=(Probability x, T y) noexcept;
constexpr bool operator<=(T x, Probability y) noexcept;
constexpr bool operator>=(Probability x, Probability y) noexcept;
constexpr bool operator>=(Probability x, T y) noexcept;
constexpr bool operator>=(T x, Probability y) noexcept;
constexpr std::strong_ordering
    operator<=>(Probability x, Probability y) noexcept;
constexpr std::partial_ordering operator<=>(Probability x, T y) noexcept;
constexpr std::partial_ordering operator<=>(T x, Probability y) noexcept;
```

Comparison operators. These all have the natural meaning.

```c++
Probability pow(Probability x, T y) noexcept;
Probability cmp_pow(Probability x, T y) noexcept;
```

Power functions. The `pow()` function returns _x<sup>y</sup>_; `cmp_pow()`
returns the complementary power, _1-(1-x)<sup>y</sup>._ The output is always
clamped to `[0,1]`. Where practical, arguments and results close to 1 will be
handled more accurately than simply calling `std::pow(x.value(),y)`. Behaviour
is undefined for `pow()` if `x=0` and `y<=0`, and for `cmp_pow()` if `x=1` and
`y<=0`,
