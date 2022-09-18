# Text Generators

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/text-gen.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
using StringFunction = std::function<std::string(const std::string&)>;
using StringList = std::vector<std::string>;
using TextList = std::vector<TextGen>;
using TextWeights = std::vector<std::pair<TextGen, double>>;
```

Aliases for convenience.

## TextGen class

```c++
class TextGen;
```

The text generator class.

```c++
using result_type = std::string;
```

Return type.

```c++
enum case TextGen::option: int {
    none      = 0,
    lower     = 1,
    upper     = 2,
    title     = 4,
    xtitle    = 8,
    sentence  = 16,
};
```

Bitmask flags controlling the transformation of the generator's output.

```c++
TextGen();
```

A default constructed generator will always return an empty string.

```c++
TextGen(const std::string& str);
TextGen(const std::string_view& str);
TextGen(const char* str);
TextGen(char c);
TextGen(char32_t c);
```

All of these construct a generator that always returns the same string.

```c++
TextGen(const TextGen& g);
TextGen(TextGen&& g) noexcept;
~TextGen() noexcept;
TextGen& operator=(const TextGen& g);
TextGen& operator=(TextGen&& g) noexcept;
```

Other life cycle functions.

```c++
std::string operator()(Sci::StdRng& rng) const;
```

Generates a string.

```c++
void TextGen::set(option opt);
```

Set the transformation behaviour of the final output. This will throw
`std::invalid_argument` if multiple casing transforms are requested.

```c++
static TextGen TextGen::number(int min, int max);
```

Generates a random integer between `min` and `max` inclusive, and returns it
as a decimal string. This will throw `std::invalid_argument` if `min>max`.

```c++
static TextGen TextGen::choice(const std::string& list);
static TextGen TextGen::choice(const StringList& list);
static TextGen TextGen::choice(const TextList& list);
static TextGen TextGen::choice(std::initializer_list<TextGen> list);
static TextGen TextGen::choice(const TextWeights& weights);
static TextGen TextGen::choice(std::initializer_list<
    std::pair<TextGen, double>> weights);
```

These create a generator that calls one of a set of generators, chosen at
random. The first four versions choose any of their generators with equal
probability (the first version chooses one of the Unicode characters in the
string). The last two choose with probabilities in proportion to the
specified weights (weights need not add up to 1).

All of these will throw `std::invalid_argument` if the argument container is
empty. The first version will throw if the string is not valid UTF-8. The
weighted versions will throw if any weight is negative, or if all weights are
zero.

## Generator combinators

```c++
TextGen operator+(const TextGen& a, const TextGen& b);
TextGen& operator+=(TextGen& a, const TextGen& b);
```

Concatenates the output of two generators.

```c++
TextGen operator&(const TextGen& a, const TextGen& b);
TextGen& operator&=(TextGen& a, const TextGen& b);
```

Same as `operator+()`, except that the two concatenated strings are guaranteed
to be different.

```c++
TextGen operator|(const TextGen& a, const TextGen& b);
TextGen& operator|=(TextGen& a, const TextGen& b);
```

Returns the result of either generator at random. This operator can be chained
to create multiple choices with equal probability, as in the unweighted
versions of `choice()`; for example, `A|B|C|D` will call one of the generators
with probability 1/4, equivalent to `choice(A,B,C,D)`.

```c++
TextGen operator*(const TextGen& g, int n);
TextGen& operator*=(TextGen& g, int n);
```

`G*n` calls the generator `n` times and concatenates the results; `G*m*n`
generates a random number from `m` to `n` and concatenates that many calls.
This will throw `std::invalid_argument` if `m>n`.

```c++
TextGen operator%(const TextGen& g, double p);
TextGen& operator%=(TextGen& g, double p);
```

Calls the generator with probability `p`, otherwise returns an empty string.
This will throw `std::invalid_argument` if `p<0` or `p>1`.

```c++
TextGen operator>>(const TextGen& g, StringFunction f);
TextGen& operator>>=(TextGen& g, StringFunction f);
```

Calls the function on the output of the generator and returns the result.

## Generator literals

```c++
namespace Literals {
    TextGen operator""_tg(const char* ptr, size_t len);
    TextGen operator""_tg(unsigned long long n);
}
```

The string-based literal splits the string into words delimited by whitespace,
and constructs a generator that will return one of the strings at random. The
integer-based literal is equivalent to `TextGen::number(1,n)`.
