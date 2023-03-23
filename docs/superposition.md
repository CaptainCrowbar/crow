# Superposition of States

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/superposition.hpp"
namespace Crow;
```

## Superposition classes

```c++
template <std::equality_comparable T> class FlatSuperposition;
template <std::equality_comparable T, NonIntegralNumericType W = double>
    class WeightedSuperposition;
```

These classes represent a set of discrete possible values for a variable.
Operations on one or more superposition objects return a set of the results
of all possible combinations of operands. A superposition may be empty; any
operation on an empty superposition returns another empty one.

A `FlatSuperposition` contains just a set of values, while a
`WeightedSuperposition` contains a set of _(value,weight)_ pairs, with the
weights adding up to 1 (if the set is not empty).

In the member documentation below, where a type is given simply as
`Superposition` without a prefix, the described feature applies to both class
templates.

```c++
using FlatSuperposition::value_type = T;
using WeightedSuperposition::value_type = T;
using WeightedSuperposition::weight_type = W;
```

Member types.

```c++
class Superposition::iterator;
```

A bidirectional iterator over the elements of the superposition. For a
`WeightedSuperposition` this dereferences to a _(value,weight)_ pair. If `T`
is totally ordered, the elements will be visited in their natural order;
otherwise, the order is arbitrary, and is not guaranteed to be the same
between two superpositions containing the same set of values.

```c++
Superposition::Superposition();
```

The default constructor creates an empty superposition.

```c++
Superposition::Superposition(const T& x);
```

Constructs a superposition containing a single value (with weight 1 if the
superposition is weighted).

```c++
FlatSuperposition::FlatSuperposition(std::initializer_list<T> list);
```

Construct a flat superposition from a list of values.

```c++
FlatSuperposition::FlatSuperposition
    (std::initializer_list<FlatSuperposition> list);
```

Construct a flat superposition from the set union of a list of
superpositions.

```c++
template <typename... Args>
    WeightedSuperposition::WeightedSuperposition(Args&&... args);
```

Construct a weighted superposition from a list of values and weights. The
argument list is expected to consist of alternating values and weights; the
value arguments can be either objects of type `T` (or convertible to it), or
existing `WeightedSuperposition` objects. The weights need not add up to
exactly 1; they will be normalised to 1 after construction. Values with a
zero or negative weight will be ignored.

```c++
Superposition::Superposition(const Superposition& s);
Superposition::Superposition(Superposition&& s) noexcept;
Superposition::~Superposition() noexcept;
Superposition& Superposition::operator=(const Superposition& s);
Superposition& Superposition::operator=(Superposition&& s) noexcept;
```

Other life cycle functions. A moved-from superposition will become empty.

```c++
template <std::regular_invocable<const T&> UF>
    Superposition<std::invoke_result_t<UF, T>>
    auto Superposition::apply(UF uf) const;
template <std::regular_invocable<const T&, const T&> BF,
        std::equality_comparable U>
    Superposition<std::invoke_result_t<BF, T, U>>
    auto Superposition::apply(BF bf, const Superposition<U>& s) const;
```

These apply a unary or binary function to the elements of a superposition, or
to the Cartesian product of two superpositions, and return the set of results
as a new superposition.

```c++
Superposition::iterator Superposition::begin() const;
Superposition::iterator Superposition::end() const;
```

Iterators over the superposition's values.

```c++
RandomChoice<T> FlatSuperposition::choice() const;
WeightedChoice<T> WeightedSuperposition::choice() const;
```

These construct a random distribution with the same values and weights as the
superposition. Behaviour is undefined if the superposition is empty.

```c++
Superposition<std::strong_ordering> Superposition::compare(const Superposition& s) const;
std::partial_ordering Superposition::partial_compare(const Superposition& s) const;
bool operator==(const Superposition& x, const Superposition& y);
bool operator!=(const Superposition& x, const Superposition& y);
std::partial_ordering operator<=>(const Superposition& x, const Superposition& y);
```

Comparison operations. The `compare()` function returns a superposition of
comparison results. The `partial_compare()` function returns a partial
ordering, giving a definite result only if the result is the same for all
possible combinations of values. The equality operator returns true if both
arguments are unique and have the same value, or if both are empty. The `<=>`
operator is a synonym for `partial_compare()`.

```c++
W WeightedSuperposition::mean() const;
W WeightedSuperposition::sd() const;
```

Calculate the mean and standard deviation of the weighted values in the
superposition. These are defined only if `T` is convertible to `W`. Behaviour
is undefined if the superposition is empty.

```c++
T Superposition::min() const;
T Superposition::max() const;
```

Return the minimum and maximum values in the superposition. These are defined
only if `T` is totally ordered. Behaviour is undefined if the superposition
is empty.

```c++
FlatSuperposition FlatSuperposition::set_difference
    (const FlatSuperposition& y) const;
FlatSuperposition FlatSuperposition::set_intersection
    (const FlatSuperposition& y) const;
FlatSuperposition FlatSuperposition::set_symmetric_difference
    (const FlatSuperposition& y) const;
FlatSuperposition FlatSuperposition::set_union
    (const FlatSuperposition& y) const;
```

These return the results of set operations on two superpositions.

```c++
size_t Superposition::size() const noexcept;
```

The number of distinct values in the superposition.

```c++
bool Superposition::empty() const noexcept;
```

True if the superposition is empty; equivalent to `size()==0`.

```c++
bool Superposition::is_unique() const noexcept;
```

True if the superposition contains exactly one value; equivalent to
`size()==1`.

```c++
std::string FlatSuperposition::str(const FormatSpec& spec = {}) const;
std::string WeightedSuperposition::str(const FormatSpec& spec1 = {},
    const FormatSpec& spec2 = {}) const;
std::ostream& operator<<(std::ostream& out, const Superposition& s);
```

Format the superposition's values. Empty superpositions are written as
`"null"`. If `is_unique()` is true, the single value is formatted using the
type's normal rules. If there is more than one value, these use the same
formats as `format_range()` or `format_map()`.

```c++
const T& Superposition::value() const;
const T& Superposition::operator*() const;
```

Return the superposition's unique value, if it has one. If `is_unique()` is
false, `value()` will throw `std::invalid_argument`; behaviour of
`operator*()` is undefined.

```c++
Superposition Superposition::operator+() const;
Superposition Superposition::operator-() const;
template <typename U>
    Superposition& Superposition::operator+=(const Superposition<U>& y);
template <typename U>
    Superposition& Superposition::operator-=(const Superposition<U>& y);
template <typename U>
    Superposition& Superposition::operator*=(const Superposition<U>& y);
template <typename U>
    Superposition& Superposition::operator/=(const Superposition<U>& y);
Superposition operator+(const Superposition& x, const Superposition& y);
Superposition operator-(const Superposition& x, const Superposition& y);
Superposition operator*(const Superposition& x, const Superposition& y);
Superposition operator/(const Superposition& x, const Superposition& y);
```

Arithmetic operators. These compute all possible results of the corresponding
operators on `T`.
