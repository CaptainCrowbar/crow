# Rational Numbers

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/rational.hpp"
namespace Crow;
```

## Rational number class

```c++
template <typename T> class Ratio;
```

The `Ratio` template implements rational numbers over an integer type. The
underlying type `T` must be an integer arithmetic type, but this is not
statically checked to allow `Ratio` to be used with non-standard integer
types.

Constructors and arithmetic operators will always reduce the result to its
lowest terms; the denominator will always be positive. Behaviour is undefined
if the denominator is zero, or in any other case of division by zero, or if
the result of an operation is not representable in the integer type.

```c++
using Rational = Ratio<int>;
using Rational64 = Ratio<int64_t>;
```

General purpose rational types.

```c++
using Ratio::integer_type = T;
```

The underlying integer type.

```c++
constexpr Ratio::Ratio() noexcept;
```

The default constructor sets the numerator to 0 and the denominator to 1.

```c++
constexpr Ratio::Ratio(T t) noexcept;
```

Sets the rational number equal to the given integer value.

```c++
constexpr Ratio::Ratio(T n, T d) noexcept;
```

Sets the rational number to `n/d`. The numerator and denominator will be
reduced to their lowest terms; the resulting rational number will always have
a positive denominator.

```c++
constexpr Ratio::~Ratio() noexcept;
constexpr Ratio::Ratio(const Ratio& r) noexcept;
constexpr Ratio::Ratio(Ratio&& r) noexcept;
constexpr Ratio& Ratio::operator=(const Ratio& r) noexcept;
constexpr Ratio& Ratio::operator=(Ratio&& r) noexcept;
```

Other life cycle functions.

```c++
constexpr explicit Ratio::operator bool() const noexcept;
```

True if the value is non-zero.

```c++
template <typename U> constexpr explicit Ratio::operator U() const noexcept;
```

Explicitly convert the rational to any arithmetic type. If `U` is an integer,
the result will be rounded toward negative infinity; otherwise, division in
`U` is performed.

```c++
constexpr Ratio Ratio::operator+() const noexcept;
constexpr Ratio Ratio::operator-() const noexcept;
constexpr Ratio& Ratio::operator+=(Ratio y) noexcept;
constexpr Ratio& Ratio::operator-=(Ratio y) noexcept;
constexpr Ratio& Ratio::operator*=(Ratio y) noexcept;
constexpr Ratio& Ratio::operator/=(Ratio y) noexcept;
constexpr Ratio operator+(Ratio x, Ratio y) noexcept;
constexpr Ratio operator-(Ratio x, Ratio y) noexcept;
constexpr Ratio operator*(Ratio x, Ratio y) noexcept;
constexpr Ratio operator/(Ratio x, Ratio y) noexcept;
```

Arithmetic operators.

```c++
std::strong_ordering operator<=>(Ratio x, Ratio y) noexcept;
constexpr bool operator==(Ratio x, Ratio y) noexcept;
constexpr bool operator!=(Ratio x, Ratio y) noexcept;
constexpr bool operator<(Ratio x, Ratio y) noexcept;
constexpr bool operator>(Ratio x, Ratio y) noexcept;
constexpr bool operator<=(Ratio x, Ratio y) noexcept;
constexpr bool operator>=(Ratio x, Ratio y) noexcept;
```

Comparison operators.

```c++
constexpr T Ratio::num() const noexcept;
constexpr T Ratio::den() const noexcept;
```

Return the numerator and denominator.

```c++
constexpr Ratio Ratio::abs() const noexcept;
```

Returns the absolute value of the rational number.

```c++
constexpr T Ratio::floor() const noexcept;
constexpr T Ratio::ceil() const noexcept;
constexpr T Ratio::round() const noexcept;
```

Convert a rational to an integer. These follow the rules implied by the names
of the functions; `round()` rounds halves toward positive infinity.

```c++
size_t Ratio::hash() const noexcept;
struct std::hash<Ratio>;
```

Hash functions.

```c++
constexpr bool Ratio::is_integer() const noexcept;
```

True if the value is an exact integer (equivalent to `den()==1`).

```c++
constexpr Ratio Ratio::reciprocal() const noexcept;
```

Returns the reciprocal of the rational number.

```c++
constexpr int Ratio::sign() const noexcept;
```

REturns the sign of the value (1 if positive, 0 if zero, -1 if negative).

```c++
constexpr T Ratio::whole() const noexcept;
constexpr Ratio Ratio::frac() const noexcept;
```

Return the whole and fractional parts of a rational number. The whole part is
truncated toward zero; the fractional part will have the same sign as the
original value, if the value was not an exact integer.

```c++
std::string Ratio::str(FormatSpec spec = {}) const;
std::ostream& operator<<(std::ostream& o, Ratio r);
```

Convert a rational number to a string.

* Format modes:
    * `[BbNnRrXx]` -- Format integers using these modes (default `n`)
    * `[DdEeFfGg]` -- Convert to floating point and format accordingly
* Format options (only applied to integer modes):
    * `m` -- Mixed fraction e.g. `"2 3/5"` (default)
    * `v` -- Vulgar fraction e.g. `"13/5"`

```c++
static Ratio Ratio::parse(const std::string& s);
static bool Ratio::try_parse(const std::string& s, Ratio& r);
```

Parse a string into a rational number. The string can be in integer, vulgar,
or mixed form (`"123"`, `"123/456"`, or `"123 456/789"`), with an optional
leading sign; non-significant whitespace is ignored. The `try_parse()`
function returns false if the parse fails, while `parse()` will throw
`std::invalid_argument`; in both cases the reference argument is left
unchanged.

```c++
template <typename T> class std::numeric_limits<Crow::Ratio<T>>;
```

Numeric limits specialization. This is specialized only if `numeric_limits<T>`
is specialized.
