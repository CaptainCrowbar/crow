# Log Scale Floating Point

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/log-scale.hpp"
namespace Crow;
```

## LogScale class

```c++
template <NonIntegralNumericType T> class LogScale;
```

This is an arithmetic class that stores the natural logarithm of its actual
value (and a sign). This allows it to store much larger values than the
underlying standard arithmetic type could handle.

```c++
using LogScale::data_type = T;
```

The underlying arithmetic type.

```c++
LogScale::LogScale() noexcept;
```

The default constructor sets the value to zero.

```c++
LogScale::LogScale(T x) noexcept;
```

Constructor from a value of the underlying arithmetic type. Behaviour is
undefined if the argument is an infinity or NaN value.

```c++
explicit LogScale::LogScale(T ln, int sign) noexcept;
```

Constructor from an explicit logarithm and sign. The sign will be clamped to
`[-1,1]`. If the sign is zero, the first argument is discarded and the value
is set to zero.

```c++
LogScale::LogScale(const LogScale& x) noexcept;
LogScale::LogScale(LogScale&& x) noexcept;
LogScale::~LogScale() noexcept;
LogScale& LogScale::operator=(const LogScale& x) noexcept;
LogScale& LogScale::operator=(LogScale&& x) noexcept;
```

Other life cycle functions.

```c++
T LogScale::get() const noexcept;
explicit LogScale::operator T() const noexcept;
```



```c++
size_t LogScale::hash() const noexcept;
template <NonIntegralNumericType T> class std::hash<LogScale<T>>;
```

Hash function. Note that `hash<T>()(t)` and `hash<LogScale<T>>()(t)` should
not be expected to return the same hash value.

```c++
bool LogScale::is_in_range() const noexcept;
```

True if the value is in range for the underlying type `T`.

```c++
T LogScale::log() const noexcept;
```

Returns the natural log value stored in the object (same as `log(*this)`).

```c++
void LogScale::parse(std::string_view str);
bool LogScale::try_parse(std::string_view str);
```

Parse a floating point number, in the usual format. If the string does not
contain a valid number (and nothing else), `try_parse()` will return false
(and leave the object unchanged), while `parse()` will throw
`std::invalid_argument`.

```c++
int LogScale::sign() const noexcept;
friend int LogScale::sign_of(LogScale x) noexcept;
```

Return the sign of the value.

```c++
std::string LogScale::str(FormatSpec spec = {}) const;
std::ostream& operator<<(std::ostream& out, LogScale x);
```

Converts the value to a string. The only format specifications recognised are
the modes `D/d, E/e, G/g` and the `z` option. The default format is the usual
`gz6`.

```c++
static LogScale LogScale::min_in_range() noexcept;
static LogScale LogScale::max_in_range() noexcept;
```

The minimum and maximum positive values for which `is_in_range()` is true.

```c++
explicit LogScale::operator bool() const noexcept;
```

True if the value is non-zero.

```c++
LogScale LogScale::operator+() const noexcept;
LogScale LogScale::operator-() const noexcept;
LogScale& LogScale::operator+=(LogScale y) noexcept;
LogScale& LogScale::operator-=(LogScale y) noexcept;
LogScale& LogScale::operator*=(LogScale y) noexcept;
LogScale& LogScale::operator/=(LogScale y) noexcept;
LogScale operator+(LogScale x, LogScale y) noexcept;
LogScale operator-(LogScale x, LogScale y) noexcept;
LogScale operator*(LogScale x, LogScale y) noexcept;
LogScale operator/(LogScale x, LogScale y) noexcept;
```

Arithmetic operators. All of these have their usual meaning. Behaviour is
undefined if the result would be out of range, or on division by zero.

```c++
bool operator==(LogScale x, LogScale y) noexcept;
bool operator!=(LogScale x, LogScale y) noexcept;
bool operator<(LogScale x, LogScale y) noexcept;
bool operator>(LogScale x, LogScale y) noexcept;
bool operator<=(LogScale x, LogScale y) noexcept;
bool operator>=(LogScale x, LogScale y) noexcept;
std::strong_ordering operator<=>(LogScale x, LogScale y) noexcept;
std::strong_ordering LogScale::compare(LogScale y) const noexcept;
```

Comparison operators. All of these have their usual meaning. The `compare()`
function is equivalent to `operator<=>`.

```c++
LogScale abs(LogScale x) noexcept;
LogScale exp(LogScale x) noexcept;
LogScale log(LogScale x) noexcept;
LogScale log2(LogScale x) noexcept;
LogScale log10(LogScale x) noexcept;
LogScale modf(LogScale x, LogScale* iptr) noexcept;
LogScale nextafter(LogScale x, LogScale y) noexcept;
LogScale pow(LogScale x, LogScale y) noexcept;
template <NumericType U> LogScale pow(LogScale x, U y) noexcept;
```

Elementary mathematical functions. All of these have their usual meaning.
Behaviour is undefined for arguments for which the result is not real valued.
For `pow()`, behaviour is undefined if the first argument is negative, or if
the first argument is zero and the second is not positive (the usual
exception for raising a negative value to an integer power is not allowed
here because the log scale representation makes it impossible to reliably
detect integer values).

```c++
template <NonIntegralNumericType T> class std::numeric_limits<LogScale<T>>:
```

Partial specialisation of `std::numeric_limits`.
