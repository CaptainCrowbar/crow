# Flexible Associative Containers

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/flexible-map.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Introduction

The `FlexibleMap` and `FlexibleSet` classes are associative containers that
can adapt their implementation to the properties of the key type. The
implementation will behave like a `std::map/set,` a `std::unordered_map/set,`
or a simple `vector` with linear search, depending on whether the key type
provides a comparison function, a hash function, or neither.

## Supporting types

```c++
enum class FlexImpl: int {
    none,
    ordered,
    hash,
    linear
};
```

A `FlexibleMap/Set` has a member constant `impl` of this type, to indicate
which implementation it uses. The `FlexibleMap/Set` templates also take an
optional template argument to indicate whether to prefer an ordered
implementation (the default) or a hash implementation when the key type
allows both (supplying `linear` here will fail to compile).

## Flexible associative container classes

### Complexity

`C` is the container type, `n` is the size of the container, and `k` is the
number of elements involved in the operation.

| Operation           | `ordered`        | `hash`  | `linear`  |
| ---------           | -------        | ------  | --------  |
| `Container()`       | `O(1)`         | `O(1)`  | `O(1)`    |
| `Container(value)`  | `O(1)`         | `O(1)`  | `O(1)`    |
| `Container(list)`   | `O(k)`         | `O(k)`  | `O(k)`    |
| `Container(i,j)`    | `O(k)`         | `O(k)`  | `O(k)`    |
| `operator[](key)`   | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `begin()`           | `O(1)`         | `O(1)`  | `O(1)`    |
| `end()`             | `O(1)`         | `O(1)`  | `O(1)`    |
| `size()`            | `O(1)`         | `O(1)`  | `O(1)`    |
| `empty()`           | `O(1)`         | `O(1)`  | `O(1)`    |
| `contains(key)`     | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `find(key)`         | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `insert(value)`     | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `insert(i,value)`   | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `insert(i,j)`       | `O(k.log(n))`  | `O(k)`  | `O(k.n)`  |
| `insert(list)`      | `O(k.log(n))`  | `O(k)`  | `O(k.n)`  |
| `erase(i)`          | `O(1)`         | `O(1)`  | `O(n)`    |
| `erase(key)`        | `O(log(n))`    | `O(1)`  | `O(n)`    |
| `clear()`           | `O(1)`         | `O(1)`  | `O(n)`    |

### FlexibleMap class

```c++
template <std::equality_comparable K, std::regular T,
    FlexImpl Prefer = FlexImpl::ordered>
class FlexibleMap {
    class const_iterator;
    class iterator;
    using key_type = K;
    using mapped_type = T;
    using value_type = std::pair<const K, T>;
    static constexpr FlexImpl impl;
    FlexibleMap();
    template <InputIteratorType II> FlexibleMap(II i, II j);
    FlexibleMap(std::initializer_list<std::pair<K, T>> list);
    FlexibleMap(const FlexibleMap& x);
    FlexibleMap(FlexibleMap&& x) noexcept;
    ~FlexibleMap() noexcept;
    FlexibleMap& operator=(const FlexibleMap& x);
    FlexibleMap& operator=(FlexibleMap&& x) noexcept;
    T& operator[](const K& k);
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    bool contains(const K& k) const;
    iterator find(const K& k);
    const_iterator find(const K& k) const;
    std::pair<iterator, bool> insert(const value_type& value);
    iterator insert(const_iterator i, const value_type& value);
    template <InputIteratorType II> void insert(II i, II j);
    void insert(std::initializer_list<value_type> list);
    void erase(const_iterator i);
    bool erase(const K& k);
    void clear() noexcept;
    void swap(FlexibleMap& m) noexcept;
};
```

### FlexibleSet class

```c++
template <std::equality_comparable K,
    FlexImpl Prefer = FlexImpl::ordered>
class FlexibleSet {
    class const_iterator;
    class iterator;
    using key_type = K;
    using value_type = K;
    static constexpr FlexImpl impl;
    FlexibleSet();
    template <InputIteratorType II> FlexibleSet(II i, II j);
    FlexibleSet(std::initializer_list<K> list);
    FlexibleSet(const FlexibleSet& x);
    FlexibleSet(FlexibleSet&& x) noexcept;
    ~FlexibleSet() noexcept;
    FlexibleSet& operator=(const FlexibleSet& x);
    FlexibleSet& operator=(FlexibleSet&& x) noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    bool contains(const K& k) const;
    const_iterator find(const K& k) const;
    std::pair<iterator, bool> insert(const K& k);
    iterator insert(const_iterator i, const K& k);
    template <InputIteratorType II> void insert(II i, II j);
    void insert(std::initializer_list<K> list);
    void erase(const_iterator i);
    bool erase(const K& k);
    void clear() noexcept;
    void swap(FlexibleSet& s) noexcept;
};
```

## Generic set operations

### Concepts

```c++
template <typename T> concept Setlike;
```

True if `T` is an instantiation of `std::set`, `std::unordered_set`, or
`FlexibleSet`.

### Set functions

```c++
template <Setlike S, Setlike T>
    S set_union(const S& s, const T& t);
template <Setlike S, Setlike T, Setlike U>
    void set_union(const S& s, const T& t, U& u);
template <Setlike S, Setlike T>
    S set_intersection(const S& s, const T& t);
template <Setlike S, Setlike T, Setlike U>
    void set_intersection(const S& s, const T& t, U& u);
template <Setlike S, Setlike T>
    S set_difference(const S& s, const T& t);
template <Setlike S, Setlike T, Setlike U>
    void set_difference(const S& s, const T& t, U& u);
template <Setlike S, Setlike T>
    S set_symmetric_difference(const S& s, const T& t);
template <Setlike S, Setlike T, Setlike U>
    void set_symmetric_difference(const S& s, const T& t, U& u);
```

These perform the corresponding set theoretic operations. The first
(two argument) version of each function returns a set of the same type as its
first argument; the second (three argument) version writes the result into
its third argument (discarding any existing content). In all cases any
combination of set-like types is supported, and aliasing between any
arguments is allowed.

Complexity is _O(n<sup>2</sup>)_ if both input arguments are linear sets,
otherwise _O(n)_, where _n=|s|+|t|_.
