# Spatial Index

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/spatial-index.hpp"
namespace Crow;
```

## SpatialIndex class

```c++
template <RegularHashable T, std::floating_point S, int N>
    requires (N > 0)
    class SpatialIndex;
```

This holds a set of objects of type `T`, each with a position in N-dimensional
space. The structure is optimised for quickly finding neighbours within a
certain distance of a point.

`T` is assumed to be relatively lightweight and cheap to copy and compare.
Using a `shared_ptr` is recommended if the items being stored are
heavyweight.

```c++
using SpatialIndex::object_type = T;
using SpatialIndex::scalar_type = S;
using SpatialIndex::vector_type = Vector<S, N>;
using SpatialIndex::value_type = std::pair<T, vector_type>;
```

Member types.

```c++
class SpatialIndex::iterator;
```

A forward iterator over the entries in a spatial index, dereferencing to a
`const value_type&`. The order of iteration is arbitrary.

```c++
static constexpr int SpatialIndex::dimension = N;
```

The number of dimensions.

```c++
SpatialIndex::SpatialIndex();
explicit SpatialIndex::SpatialIndex(S scale);
```

Constructors. The `scale` argument is the size of the cells used in the
spatial index, defaulting to 1. Behaviour is undefined if `scale<=0`.

The exact choice of scale is not critical, provided it is not so large that
few queries will search more than one cell. A reasonable value would be
on the order of 1/10 of the search radius of a typical query.

```c++
SpatialIndex::SpatialIndex(const SpatialIndex& obj);
SpatialIndex::SpatialIndex(SpatialIndex&& obj) noexcept;
SpatialIndex::~SpatialIndex() noexcept;
SpatialIndex& SpatialIndex::operator=(const SpatialIndex& obj);
SpatialIndex& SpatialIndex::operator=(SpatialIndex&& obj) noexcept;
```

Other life cycle functions.

```c++
bool SpatialIndex::contains(const T& t) const;
```

True if the given value is present in the index.

```c++
bool SpatialIndex::empty() const noexcept;
```

True if the index is empty.

```c++
size_t SpatialIndex::size() const noexcept;
```

Returns the number of entries in the index.

```c++
S SpatialIndex::scale() const noexcept;
```

Returns the internal cell scale.

```c++
S SpatialIndex::outer_limit() const noexcept;
```

Returns the maximum radius of the index. This is equal to the scale multiplied
by the maximum value of a signed 64-bit integer.

```c++
iterator SpatialIndex::begin() const;
iterator SpatialIndex::end() const;
```

Iterators over the contents of the index.

```c++
iterator SpatialIndex::find(const T& t) const;
```

Search the index for a given value. This will return an iterator pointing to
the requested value and its position if found, otherwise `end()`.

```c++
std::optional<vector_type>
    SpatialIndex::position_of(const T& t) const;
```

Returns the position of the given entry, or an empty option if the value is
not found in the index.

```c++
std::pair<iterator, bool>
    SpatialIndex::insert(const T& t, vector_type pos);
std::pair<iterator, bool>
    SpatialIndex::update(const T& t, vector_type pos);
```

Insert or modify an entry in the index. The `insert()` method will fail if the
key value already exists, while `update()` will change the position
associated with that value. Their behaviour is otherwise the same. They
return an iterator pointing to the entry with the given key value, and a flag
indicating whether the entry was newly created.

These will throw `std::out_of_range` if the position is further than
`outer_limit()` units from the origin.

```c++
bool SpatialIndex::erase(const T& t) noexcept;
void SpatialIndex::erase(iterator it) noexcept;
```

Remove an entry from the index. The first version will remove the given key
value and return true, or do nothing and return false if the entry is not
found. The second version will remove the entry pointed to by the iterator;
behaviour is undefined if `it=end()`.

```c++
void SpatialIndex::clear() noexcept;
```

Erase all entries from the index.

```c++
std::vector<iterator>
    SpatialIndex::search(vector_type origin, S radius) const;
std::vector<iterator>
    SpatialIndex::search(const T& origin, S radius) const;
std::vector<iterator>
    SpatialIndex::search(iterator origin, S radius) const;
```

Search for all entries within a given distance of a given point. The origin
point of the search can be supplied as an explicit position, as a key value,
or as an iterator. The return value is a list of all entries within the
search volume(including the key value or iterator, if supplied).

If a key value is supplied as the origin, the returned list will be empty if
the value is not found in the index. Behaviour is undefined if `radius<0` or,
for the iterator version, if `origin=end()`.

Complexity is `O(k.(radius/scale)^N)` where `k` is the average number of
objects per cell.
