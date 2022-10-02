# LRU Cache

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/cache.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## LRU cache class

```c++
template <typename K, typename T> class LruCache;
```

A cache of key-value pairs that discards the least recently used entries if
the cache size grows too large.

```c++
using LruCache::key_type = K;
using LruCache::mapped_type = T;
```

Member types.

```c++
LruCache::LruCache();
explicit LruCache::LruCache(size_t cap);
LruCache::LruCache(LruCache&& c);
LruCache::~LruCache();
LruCache& LruCache::operator=(LruCache&& c);
```

Life cycle functions. The capacity defaults to `npos` (unlimited cache size).
`LruCache` objects are not copyable.

```c++
std::optional<T> LruCache::get(const K& k);
```

Retrieve the cached value corresponding to the given key, or an empty result
if the key is not present in the cache. On success, the key is moved to the
front of the cache.

```c++
void LruCache::set(const K& k, const T& t);
```

Insert this key-value pair at the front of the cache. Any existing pair with
this key will be discarded.

```c++
template <std::invocable<K> F>
    requires (std::convertible_to<std::invoke_result_t<F, K>, T>)
    T LruCache::item(const K& k, F&& f);
```

Retrieve the cached value corresponding to the given key, or create a new one
if the key is not present. If a new entry is created, the mapped value is
calculated by calling `f(k)`. This is equivalent to calling `get(k)` or
`set(k,f(k))` depending on whether the key was already present.

```c++
size_t LruCache::size() const noexcept;
```

Returns the number of key-value pairs currently in the cache. This will always
be less than or equal to the capacity.

```c++
size_t LruCache::capacity() const noexcept;
void LruCache::reserve(size_t cap) noexcept;
```

Query or set the capacity. If the new capacity is less than the current size,
the oldest key-value pairs will be discarded to make the size equal to the
capacity.

Setting the capacity to zero clears and disables the cache; no key-value pairs
will be saved until the capacity is changed. Setting the capacity to `npos`
puts no limit on the cache size (other than available memory); all key-value
pairs cached will be kept indefinitely, or until the capacity is changed.

```c++
void LruCache::clear() noexcept;
```

Clears all key-value pairs from the cache. This does not change the capacity.

## Function memoization class

```c++
template <typename Arg, typename Res> class FunctionCache;
```

A cache of function return values, to avoid calling an expensive function on
arguments whose return value has already been determined. This has the same
caching behaviour as `LruCache`.

```c++
using FunctionCache::argument_type = Arg;
using FunctionCache::result_type = Res;
```

Member types.

```c++
FunctionCache::FunctionCache();
template <std::invocable<Arg> F>
    requires (std::convertible_to<std::invoke_result_t<F, Arg>, Res>)
    explicit FunctionCache::FunctionCache(F&& f, size_t cap = npos);
FunctionCache::FunctionCache(FunctionCache&& c);
FunctionCache::~FunctionCache();
FunctionCache& FunctionCache::operator=(FunctionCache&& c);
```

Life cycle functions.

```c++
Res FunctionCache::operator()(const Arg& arg);
```

Function call operator. This will return the cached result if possible,
otherwise it will call the stored function and cache the result. This will
throw `std::bad_function_call` if the `FunctionCache` was default
constructed.

```c++
size_t FunctionCache::size() const noexcept;
size_t FunctionCache::capacity() const noexcept;
void FunctionCache::clear() noexcept;
void FunctionCache::reserve(size_t cap) noexcept;
```

These are equivalent to the corresponding functions on `LruCache`.
