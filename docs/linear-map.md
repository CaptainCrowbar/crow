# Linear Interpolated Map

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/linear-map.hpp"
namespace Crow;
```

## Linear interpolated map class

```c++
template <typename X, typename Y = X> class LinearMap;
```

An `InterpolatedMap` holds a list of `(X,Y)` pairs, returning an interpolated
`Y` value for an arbitrary `X` input. The input type `X` must be a floating
point arithmetic type; the output type `Y` is the same as `X` by default, but
can be any type that allows addition (`Y+Y`), subtraction(`Y-Y`), and
multiplication by `X` (`X*Y`).

```c++
using LinearMap::key_type = X;
using LinearMap::mapped_type = Y;
```

Member types.

```c++
LinearMap::LinearMap();
LinearMap::LinearMap(std::initializer_list<private_type> list);
```

The constructor that takes an initializer list expects a list of braced
tuples, each containing one `X` value and one to three `Y` values,
interpreted in the same way as the `insert()` functions described below.
Example:

```c++
InterpolatedMap<double> map = {
    {10, 100},            // insert(10, 100)
    {20, 200, 300, 400},  // insert(20, 200, 300, 400)
    {30, 500, 600},       // insert(30, 500, 600)
};
```

If an `X` value is repeated, later entries overwrite earlier ones.

```c++
LinearMap::LinearMap(const LinearMap& lm);
LinearMap::LinearMap(LinearMap&& lm) noexcept;
LinearMap::~LinearMap() noexcept;
LinearMap& LinearMap::operator=(const LinearMap& lm);
LinearMap& LinearMap::operator=(LinearMap&& lm) noexcept;
```

Other life cycle functions.

```c++
Y LinearMap::operator[](X x) const;
```

The lookup operator returns the interpolated `Y` value corresponding to the
given `X`. If the map is empty (no `(X,Y)` pairs have been supplied since
default construction or the last call to `clear()`), a default constructed
`Y` will be returned. If the `X` value is out of range (less than the
smallest value supplied so far, or greater than the largest), the `Y` value
from the nearest end of the range will be returned.

```c++
void LinearMap::clear() noexcept;
```

Clears the map to its default constructed state.

```c++
bool LinearMap::empty() const noexcept;
```

True if the map is empty.

```c++
void LinearMap::insert(X x, Y y);
void LinearMap::insert(X x, Y yl, Y yr);
void LinearMap::insert(X x, Y yl, Y y, Y yr);
```

The first version of `insert()` adds a simple `(X,Y)` pair (overwriting an
existing pair if there was one with the exact same value of `X`). The second
and third versions allow a stepwise change in the output; `yl` will be used
as the `Y` for interpolation on the left of the given value, `yr` on the
right. Optionally a third value can be supplied to be used for this exact
value of `X`; otherwise, the average of the two `Y` values will be used.

```c++
void LinearMap::erase(X x) noexcept;
void LinearMap::erase(X x1, X x2) noexcept;
```

The first version of `erase()` deletes only an entry with this exact `X`
value, otherwise doing nothing; the second version erases all entries with
`X` values in the closed interval `[x1,x2]`. Behaviour is undefined if
`x1>x2`.

```c++
X LinearMap::min() const noexcept;
X LinearMap::max() const noexcept;
```

The `min()` and `max()` functions return the range of `X` values for which a
mapping has been defined; they will return zero if the map is empty.
