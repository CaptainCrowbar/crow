# Hash Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/hash.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Concepts

```c++
template <typename T> concept Hashable;
```

Matches types with a valid specialization of `std::hash` (after stripping CV
and reference qualifications from `T`).

## Hash mixing functions

```c++
template <Hashable... Args> size_t hash_mix(const Args&... args);
template <Hashable... TS> size_t hash_mix(const std::tuple<TS...>& t);
template <RangeType Range> size_t hash_mix(const Range& r);
```

Hash mixing functions. These return the combined hash of a list, tuple, or
range of objects, calling `std::hash` on each item. All of these will return
the same result for the same sequence of values. The first version cannot be
called with less than two arguments (to avoid overload resolution issues);
the others will return zero if the argument list is empty.

## Multiplicative hashes

```c++
template <uint32_t Initial, uint32_t Modulo>
class MultiplicativeHash {
    using result_type = uint32_t;
    static constexpr uint32_t initial = Initial;
    static constexpr uint32_t modulo = Modulo;
    constexpr uint32_t operator()(const void* ptr, size_t len) const noexcept;
};
using BernsteinHash = MultiplicativeHash<5381, 33>;
using KernighanHash = MultiplicativeHash<0, 31>;
```

Generic multiplicative hash, and instantiations for the widely used Bernstein
and K&R hash functions.

## SipHash

```c++
class SipHash {
    using result_type = uint64_t;
    constexpr SipHash() noexcept; // SipHash(0,0)
    constexpr SipHash(uint64_t key0, uint64_t key1) noexcept;
    constexpr uint64_t operator()(const void* ptr, size_t len) const noexcept;
};
```

[SipHash](https://github.com/veorq/SipHash) by Jean-Philippe Aumasson and
Daniel J. Bernstein is widely used as a hash table keying function to avoid
hash flooding attacks. This implements the most common variant,
SipHash-2-4-64.

## Cryptographic hash functions

```c++
class CryptographicHash {
    using result_type = std::string;
    virtual ~CryptographicHash() noexcept;
    std::string operator()(const void* ptr, size_t len);
    std::string operator()(const std::string& str);
    size_t bits() const noexcept;
    size_t bytes() const noexcept;
    void add(const void* ptr, size_t len);
    void add(const std::string& str);
    std::string get();
    void clear() noexcept;
};
class MD5: public CryptographicHash;     // 128 bits = 16 bytes
class SHA1: public CryptographicHash;    // 160 bits = 20 bytes
class SHA256: public CryptographicHash;  // 256 bits = 32 bytes
class SHA512: public CryptographicHash;  // 512 bits = 64 bytes
```

These classes generate cryptographic hashes by calling the operating system's
native cryptographic API. `CryptographicHash` is an abstract base class
inherited by the concrete algorithm classes. These classes are not copyable
or movable.

The hash is returned as a string containing a fixed number of bytes. The
`bits()` and `bytes()` functions return the hash size in bits or bytes.

These can be used in either immediate or progressive mode:

In immediate mode, the function call operator is used to hash a single block
of data and return the resulting hash value in one call. This always starts
from a clean slate; any progressive hash state already in the object will be
discarded. Calling `get()` after `operator()` will return the same value.

In progressive mode, a hash class object is default constructed or reset using
`clear()`. One or more blocks of data are processed by calling `add()` any
number of times. The hash value can then be retrieved using `get()`. The
`get()` function will return the same value if called multiple times with no
intervening calls to `clear(), add(),` or `operator()`. Behaviour is undefined
if `add()` is called after `get()` or `operator()` without an intervening call
to `clear()`.

Uses of the progressive and immediate mode APIs can be mixed on the same
object, provided `clear()` is always used to reset the state before a
progressive hash.
