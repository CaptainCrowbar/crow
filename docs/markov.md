# Markov Text Generator

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/markov.hpp"
namespace Crow;
```

## Markov class

```c++
enum class MarkovFlags: int {
    none       = 0,
    exclusive  = 1,
};
```

Bitmask flags controlling the behaviour of the Markov generator.

```c++
template <typename T, typename S = [see below]> class Markov {
    using string_type = S;
    using value_type = T;
    Markov() = default;
    explicit Markov(size_t context, size_t min_length = 1,
        size_t max_length = npos, MarkovFlags flags = none);
    void add(const S& example);
    template <typename RNG> S operator()(RNG& rng) const;
};
```

Markov chain based text generator. This can be used with any sequential
container of a regular type. `T` is the element type; `S` is the container
type. If `T` is a character type, `S` defaults to the corresponding string
type; otherwise, `S` defaults to `std::vector<T>`.

The constructor arguments are:

* `context` -- Number of preceding elements matched when generating the next one (default 2).
* `min_length` -- Minimum length of an output sequence (default 1).
* `max_length` -- Maximum length of an output sequence.
* `flags` -- Behaviour flags.

If the `exclusive` flag is set, the generator will remember the list of sample
sequences and will not generating any matching outputs.

The constructor will throw `std::invalid_argument` if `context=0,`
`min_length>max_length,` or either length is zero.

The `add()` function adds a sample sequence to the generator's corpus. Adding
an empty sequence is ignored.

The function call operator generates a new output sequence.

```c++
using CMarkov = Markov<char>;
using SMarkov = Markov<std::string>;
```

Aliases for common instantiations.
