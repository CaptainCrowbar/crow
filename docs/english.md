# English Language Tools

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/english.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Constants

```c++
const std::unordered_set<std::string>& stopwords();
    // a, an, and, at, for, in, is, of, on, or, the, to, with
```

The default list of stopwords used in `extended_titlecase()`.

## Case conversion functions

```c++
std::string extended_titlecase(const std::string& str, bool initial = true);
std::string extended_titlecase(const std::string& str,
    const std::unordered_set<std::string>& stop, bool initial = true);
```

Convert a string to title case, checking for common short English words that
are conventionally set in lower case. If the `initial` flag is set, the first
letter of the string will always be in upper case. Optionally a custom set of
stopwords can be supplied

```c++
std::string sentence_case(const std::string& str);
```

Convert a string to sentence case. The first ASCII letter after the start of
the string or a full stop is converted to upper case; the rest of the string
is left unchanged.

## List formatting functions

```c++
template <typename Range>
    std::string comma_list(const Range& range,
        const std::string& conj = {});
```

Format a list of strings as a comma delimited list, with an optional
conjunction between the last two items, following the usual English
conventions.

Examples:

    {"abc"}                       => "abc"
    {"abc", "def"}                => "abc, def"
    {"abc", "def", "ghi"}         => "abc, def, ghi"
    {"abc"}, "and"                => "abc"
    {"abc", "def"}, "and"         => "abc and def"
    {"abc", "def", "ghi"}, "and"  => "abc, def, and ghi"

## Number formatting functions

```c++
std::string cardinal(size_t n, size_t threshold = std::string::npos);
std::string ordinal(size_t n, size_t threshold = std::string::npos);
```

Generate the English name for a number, in cardinal (`"forty-two"`) or ordinal
(`"forty-second"`) form. Numbers greater than or equal to the threshold value
will be left in numeric form (with a suffix on the ordinal numbers).

```c++
std::string format_count(double n, int prec);
```

Formats a number to the specified number of significant digits. The number is
written directly, rounded to the nearest integer, if it is under
10<sup>4</sup>; followed by a name if it is under 10<sup>12</sup>; or in
scientific notation otherwise.

Examples:

    format_count(1.23, 3) == "1";
    format_count(12.3, 3) == "12";
    format_count(123.0, 3) == "123";
    format_count(1.23e3, 3) == "1230";
    format_count(1.23e4, 3) == "12.3 thousand";
    format_count(1.23e5, 3) == "123 thousand";
    format_count(1.23e6, 3) == "1.23 million";
    format_count(1.23e7, 3) == "12.3 million";
    format_count(1.23e8, 3) == "123 million";
    format_count(1.23e9, 3) == "1.23 billion";
    format_count(1.23e10, 3) == "12.3 billion";
    format_count(1.23e11, 3) == "123 billion";
    format_count(1.23e12, 3) == "1.23x10^12";
    format_count(1.23e13, 3) == "1.23x10^13";
    format_count(1.23e14, 3) == "1.23x10^14";
    format_count(1.23e15, 3) == "1.23x10^15";

```c++
std::string number_of(size_t n, const std::string& name,
    const std::string& plural_name = {}, size_t threshold = 21);
```

Combines a cardinal number with the singular or plural of a noun. The plural
form may be supplied explicitly; if not, it will be obtained by calling the
`plural()` function described below. The `threshold` argument has the same
meaning as for the `cardinal()` function.

Examples:

    number_of(0, "thing") == "no things";
    number_of(1, "thing") == "one thing";
    number_of(2, "thing") == "two things";
    number_of(3, "thing") == "three things";
    number_of(10, "thing") == "ten things";
    number_of(20, "thing") == "twenty things";
    number_of(30, "thing") == "30 things";

## Pluralization functions

```c++
std::string plural(const std::string& noun);
```

Returns the plural form of a singular noun. This function is aware of most of
the irregular plurals in English.

Partly based on [Damian Conway's work](http://www.csse.monash.edu.au/~damian/papers/HTML/Plurals.html).

## Text generators

```c++
class LoremIpsum {
    using result_type = std::string;
    LoremIpsum() = default;
    explicit LoremIpsum(size_t bytes, bool paras = true);
    template <typename RNG> std::string operator()(RNG& rng) const;
};
```

Generates random _Lorem Ipsum_ text with approximately the specified number of
bytes (the actual length will be between `bytes` and `bytes+15` inclusive;
`bytes` defaults to 1000). Normally it will be broken into paragraphs,
delimited by two line feeds; if `paras` is false, there will be no paragraph
breaks, and no trailing LF.
