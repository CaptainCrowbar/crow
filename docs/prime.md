# Prime Numbers

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/prime.hpp"
namespace Crow;
```

This module implements some basic prime number algorithms. Although they can
be used with extended precision integers as well as primitive types, they do
not implement the sophisticated algorithms required for practical use with
very large integers, and are not intended for cryptographic applications.

In all of the templates in this module, `T` may be a primitive integer type,
or any of the types in the [`fixed-binary`](fixed-binary.html) or
[`mp-integer`](mp-integer.html) modules.

## Contents

* TOC
{:toc}

## Prime number generators

```c++
template <typename T> class PrimeGenerator {
    // all standard life cycle functions
    T operator()();
    const T& get() const noexcept;
    void next();
};
```

`PrimeGenerator` generates prime numbers using
[Will Ness's recursive algorithm](https://stackoverflow.com/questions/2211990/how-to-implement-an-efficient-infinite-generator-of-prime-numbers-in-python).

Calling `next()` increments the generator to the next prime. The `get()`
function returns the current prime number. Behaviour is undefined if `get()`
is called before the first call to `next()` (this does not apply to the
iterator, whose constructor calls `next()`).

Time complexity for generating the first _n_ prime numbers is approximately
_O(n<sup>1.5</sup>)_; space complexity is _O(√n)_.

```c++
template <typename T> class PrimeIterator {
    PrimeIterator();
    explicit PrimeIterator(bool init);
    // all standard forward iterator functions
};
```

`PrimeIterator` wraps the prime generator in a forward iterator interface.
`PrimeIterator(false)` creates a null iterator that will compare unequal to
any valid iterator; behaviour is undefined if this iterator is dereferenced.

```c++
template <typename T> Irange<PrimeIterator<T>> prime_numbers();
```

Returns an infinite range of prime numbers (the second member of the range is
a null iterator).

```c++
template <typename T> T next_prime(T n);
template <typename T> T prev_prime(T n);
```

Return the next or previous prime (both of these will return `n` if it is
already prime). The `prev_prime()` function will return zero if `n<2`.

```c++
template <typename T> std::vector<T> prime_list(T n);
template <typename T> std::vector<T> prime_list(T m, T n);
```

Return a list of prime numbers up to `n`, or from `m` to `n` inclusive. These
will use a `PrimeGenerator` if `m<=2`; otherwise they will use a less
efficient algorithm that checks each number in the range for primality. They
will return an empty array if `m>n` or `n<2`.

## Primality testing functions

```c++
template <typename T> bool is_prime(T n);
```

True if the number is prime. If `T` is signed, this will always return false
for negative arguments.

## Prime factorization functions

```c++
template <typename T> std::vector<T> prime_factors(T n);
template <typename T> std::map<T, T> prime_factors_map(T n);
```

Determine the prime factorization of a number. The `prime_factors()` function
returns a list of all prime factors, including any duplicates, in ascending
order. The `prime_factors_map()` function returns the same list in the form
of a map, with each entry consisting of a prime factor and the number of
times it occurs.

Examples:

    auto vec = prime_factors(720);      // => [2,2,2,2,3,3,5]
    auto map = prime_factors_map(720);  // => {2:4,3:2,5:1}
