# Map with Stable Order

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/stable-map.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## StableMap class template

```c++
template <typename Key, typename T, bool Dup = false,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>>
class StableMap
```

This behaves like a standard `unordered_map` or `unordered_multimap`
(depending on the `Dup` flag, indicating whether duplicates are allowed),
except that the order in which elements are inserted is preserved, with
iterators stepping through the map in order of insertion.

### Member types

```c++
class StableMap::iterator;
class StableMap::const_iterator;
```

Forward iterators.

```c++
using StableMap::key_type = Key;
using StableMap::mapped_type = T;
using StableMap::value_type = std::pair<const Key, T>;
using StableMap::hasher = Hash;
using StableMap::key_equal = KeyEqual;
using StableMap::reference = value_type&;
using StableMap::const_reference = const value_type&;
using StableMap::pointer = value_type*;
using StableMap::const_pointer = const value_type*;
using StableMap::size_type = size_t;
using StableMap::difference_type = ptrdiff_t;
```

Other member types.

### Constants

```c++
static constexpr bool StableMap::duplicates = Dup;
```

Indicates whether duplicates are allowed.

### Life cycle functions

```c++
StableMap::StableMap();
explicit StableMap::StableMap(Hash h, KeyEqual eq = {});
StableMap::StableMap(const StableMap& m);
StableMap::StableMap(StableMap&& m) noexcept;
StableMap::~StableMap() noexcept;
StableMap& StableMap::operator=(const StableMap& m);
StableMap& StableMap::operator=(StableMap&& m) noexcept;
```

Standard life cycle functions.

### Iterator functions

```c++
StableMap::iterator StableMap::begin() noexcept;
StableMap::const_iterator StableMap::begin() const noexcept;
StableMap::const_iterator StableMap::cbegin() const noexcept;
StableMap::iterator StableMap::end() noexcept;
StableMap::const_iterator StableMap::end() const noexcept;
StableMap::const_iterator StableMap::cend() const noexcept;
```

Standard iterator functions.

### Capacity functions

```c++
void StableMap::clear() noexcept;
bool StableMap::empty() const noexcept;
size_t StableMap::size() const noexcept;
```

Standard capacity functions.

```c++
void StableMap::rehash();
```

Cleans up the internal data structures. This is appropriate when the map has
reached its intended final size, or its size is likely to remain stable
through many lookup operations.

### Lookup functions

```c++
T& StableMap::operator[](const Key& k);
```

Returns a reference to the mapped value corresponding to the key, inserting
the key if it is not already present. This is defined only for unique maps.

```c++
bool StableMap::contains(const Key& k) const;
size_t StableMap::count(const Key& k) const;
```

These have their standard behaviour.

```c++
StableMap::iterator StableMap::find(const Key& k);
StableMap::const_iterator StableMap::find(const Key& k) const;
```

Return an iterator pointing to an entry with a matching key, or an end
iterator if the key is not found. For multimaps this returns the first
(oldest) entry if there is more than one with a matching key.

```c++
StableMap::iterator StableMap::find_last(const Key& k);
StableMap::const_iterator StableMap::find_last(const Key& k) const;
```

Return an iterator pointing to an entry with a matching key, or an end
iterator if the key is not found. For multimaps this returns the last
(most recent) entry if there is more than one with a matching key. For unique
maps this is a synonym for `find()`.

```c++
std::vector<StableMap::iterator>
    StableMap::find_all(const Key& k);
std::vector<StableMap::const_iterator>
    StableMap::find_all(const Key& k) const;
```

Return a list of iterators pointing to all entries with a matching key, if
any.

### Update functions

```c++
std::pair<StableMap::iterator, bool>
    StableMap::insert(const value_type& v);
std::pair<StableMap::iterator, bool>
    StableMap::insert(const Key& k, const T& t);
```

Insert a key-value pair, returning an iterator pointing to either the newly
inserted entry or the existing entry that prevented its creation, and a flag
indicating whether the insertion was successful. For multimaps the insertion
is always successful (unless an exception is thrown) and the flag is always
true.

```c++
std::pair<StableMap::iterator, bool>
    StableMap::update(const value_type& v);
std::pair<StableMap::iterator, bool>
    StableMap::update(const Key& k, const T& t);
```

Insert a key-value pair, or modify the mapped value if the key is already
present, returning an iterator pointing to the inserted or modified entry,
and a flag indicating whether a new entry was inserted. The position of a
modified entry in its original insertion order is not changed. This is
defined only for unique maps.

```c++
void StableMap::erase(const_iterator i) noexcept;
size_t StableMap::erase(const Key& k) noexcept;
void StableMap::swap(StableMap& m) noexcept;
void swap(StableMap& a, StableMap& b) noexcept;
```

These have their standard behaviour.

### Policy functions

```c++
Hash StableMap::hash_function() const;
KeyEqual StableMap::key_eq() const;
```

Standard policy query functions.
