# Regular Expressions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/regex.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Introduction

This module provides Unicode-aware regular expressions, and related classes
and functions. It calls the widely available [PCRE2](http://www.pcre.org/)
(Perl Compatible Regular Expressions) library. The `Regex` class uses the
8-bit version of the PCRE library, which must have been built with the
optional UTF-8 support enabled (PCRE2 can also be built as 16 and 32-bit
libraries, but these are not used here).

### Regular expression behaviour

Refer to the PCRE2 documentation for details of the regular expression
syntax.

The `Regex` class supports both UTF-8 and byte oriented matching. UTF-8 is the
default. Byte regexes (selected with the `Regex::byte` flag) simply treat a
string as a sequence of arbitrary bytes, with no assumptions about content
encoding, and will work with non-Unicode strings. The `\xHH` escape code
(where H is a hexadecimal digit) always matches a single byte even if the
value is greater than `\x7f` (in a UTF-8 regex this would match a multibyte
encoded character); the extended `\x{hex}` escape code can still be used, but
it will be treated as a syntax error if the value is greater than `\x{ff}`.

In addition to the exceptions explicitly described in the documentation, it
should be assumed that any function not marked `noexcept` can throw
`std::bad_alloc` if memory allocation fails.

### Crow::Regex vs std::regex

For several reasons, standard regular expressions are inadequate for use with
Unicode strings.

The most obvious reason is that standard C++ regexes are not actually required
to support Unicode strings at all. Unlike `std::basic_string`, for which
specializations for 8, 16, and 32 bit characters are required to exist, only
two specializations of `std::basic_regex` are mandated, for `char`
(the system's native multibyte encoding, which may or may not be UTF-8, but
see below for a caveat on this) and `wchar_t` (the system's wide character
encoding, which can reasonably be expected to be either UTF-16 or UTF-32, but
which one varies with the OS). In short, standard regexes can only be relied
on to support one of the three UTF encodings, and we don't know which one. An
implementation is allowed to instantiate `std::basic_regex` for other
character types, but in practise most do not.

The second problem with standard regexes is that, by the rules of the C++
standard, they _cannot_ properly support UTF-8 strings. The regex grammar
(based on that of JavaScript/EcmaScript, with a few changes) matches on an
element by element basis; a "character", as far as regex matching is
concerned, is a single code unit, not a Unicode scalar value that may be
represented by more than one code unit. This still allows literal matching of
multi-unit UTF-8 characters (the encoding will be the same in the regex and
the subject string, so they will match unit for unit), but makes it
impossible to match multi-unit characters to non-literal regex elements; for
example, even if the system encoding is UTF-8, `std::regex(".")` will still
not match `"€"` (which is three bytes long). For the same reason, it is
impossible to specify a character range that includes multibyte characters;
for example, `std::regex("[à-ÿ]")` will not do what you probably expected.

Finally, standard regexes don't support the `\p{...}` and `\P{...}` character
classes, which match on Unicode properties. This may be a minor obstacle
compared to either of the above showstoppers, but even by itself it would be
a serious handicap in a library intended to support Unicode.

## Version information

```c++
std::array<int, 3> version() noexcept;
```

Returns an array containing the major, minor, and patch version numbers.

```c++
std::string version_string();
```

Returns the version numbers as a string (e.g. `"1.23.456"`).

## Regex class

### Member types

```c++
enum class Regex::flag_type;
using enum flag_type;
```

The type used for bitmask flags.

| Flag               | Description                                                         | Usage        |
| ----               | -----------                                                         | -----        |
| `none`             | No flags                                                            |              |
| `anchor`           | Anchor the match at the start of the subject string                 | Match        |
| `ascii`            | Character classes match only ASCII                                  | Compile      |
| `byte`             | Treat the pattern and subject string as bytes instead of UTF-8      | Compile      |
| `crlf`             | Lines end with `CR+LF` instead of just `LF`                         | Compile      |
| `dollar_end`       | `$` does not match line breaks at the end of the subject string     | Compile      |
| `dot_all`          | `.` matches all characters including line breaks                    | Compile      |
| `extended`         | Free-form layout, ignore whitespace and comments starting with `#`  | Compile      |
| `first_line`       | The match must start in the first line of the subject string        | Compile      |
| `full`             | Match the complete subject string                                   | Match        |
| `global`           | Replace all matches instead of only the first                       | Replacement  |
| `hard_fail`        | Throw an exception if the match fails                               | Match        |
| `icase`            | Matching is case insensitive                                        | Compile      |
| `line`             | Only match a complete line                                          | Compile      |
| `multiline`        | `^` and `$` match the beginning and end of every line               | Compile      |
| `no_capture`       | Parenthesized groups are not captured (named captures still work)   | Compile      |
| `not_empty`        | Do not match an empty string                                        | Match        |
| `not_empty_start`  | Do not match an empty string at the start of the subject string     | Match        |
| `not_line`         | Do not match `^` or `$` at the start or end of the subject string   | Match        |
| `partial_hard`     | Enable hard partial matching (prefer partial match to full match)   | Match        |
| `partial_soft`     | Enable soft partial matching (prefer full match to partial match)   | Match        |
| `word`             | Only match a complete word                                          | Compile      |

These are bitmask flags. Wherever a regex function takes a `flags` argument,
the value is expected to be a bitwise-or combination of these. The usage
column indicates whether the flag is applied at compile time (when the regex
is constructed), at match time (when a string is matched against the regex),
or at replacement time (when the regex and a format string are used to modify
a string).

The compile-time flags must be supplied when the `Regex` object is
constructed, while the other flags can be supplied either then or later, in a
call to a matching or substitution function. Match-time and replacement-time
flags that were supplied at compile time will be saved and applied at the
appropriate later time. Flags supplied at compile time cannot be overridden
at match or replacement time.

Notes on specific flags:

* The `ascii` flag causes backslash character classes (`\[BbDdRSsWw]`), and
  some Posix character classes, to match only ASCII characters instead of
  matching Unicode properties. This has no effect when combined with the
  `byte` flag, since only ASCII characters are matched in byte mode anyway.
* The `global` flag causes all matches to be replaced in `replace()` and
  `transform::operator()`, instead of only the first match. It has no effect
  in any other context.

```c++
class Regex::error;
class Regex::match;
class Regex::token_error;
class Regex::transform;
```

These are individually described below.

```c++
using Regex::version_type = std::vector<int>;
```

The type returned by the version query functions.

```c++
class Regex::match_iterator;
struct Regex::match_range;
```

A forward iterator over the non-overlapping matches found within a subject
string for a given regex, returned by `Regex::grep()`. Its value type is
`Regex::match`. The range type has `begin()`, `end()`, and `empty()` functions
with the usual semantics.

```c++
class Regex::split_iterator;
struct Regex::split_range;
```

A forward iterator over the substrings between matches for a given regex,
returned by `Regex::split()`. Its value type is `std::string_view`. The range
type has `begin()`, `end()`, and `empty()` functions with the usual semantics.

```c++
class Regex::token_iterator;
struct Regex::token_range;
```

A forward iterator over the tokens found within a subject string by
`Regex::tokenize()`. Its value type is `Regex::match`. The range type has
`begin()`, `end()`, and `empty()` functions with the usual semantics.

```c++
struct Regex::partition_type {
    std::string_view left;
    std::string_view mid;
    std::string_view right;
};
```

The result type returned from `Regex::partition()`.

### Life cycle functions

```c++
Regex::Regex();
```

Creates a null regex that will never match anything.

```c++
explicit Regex::Regex(std::string_view pattern, flag_type flags = 0);
```

This constructor takes a pattern string, and optionally a flags bitmask, and
compiles the regular expression. It will throw `Regex::error` if the pattern
syntax is invalid, or an invalid combination of flags is passed.

```c++
Regex::Regex(const Regex& r);
Regex::Regex(Regex&& r) noexcept;
Regex::~Regex() noexcept;
Regex& Regex::operator=(const Regex& r);
Regex& Regex::operator=(Regex&& r) noexcept;
```

Standard life cycle functions.

### Member functions

```c++
std::string Regex::pattern() const;
Regex::flag_type Regex::flags() const noexcept;
std::ostream& operator<<(std::ostream& out, const Regex& re);
```

Return the pattern string or flags supplied to the constructor. The output
operator returns the `pattern()` string. If the `Regex` object was default
constructed, `pattern()` is empty and `flags()` is zero, but this is not
equivalent to a regex constructed from an empty string.

```c++
bool Regex::empty() const noexcept;
bool Regex::is_null() const noexcept;
```

The `is_null()` function is true if the regex was default constructed;
`empty()` is true if `is_null()` is true or the pattern is an empty string.

```c++
size_t Regex::groups() const noexcept;
```

Returns the number of capture groups in the pattern, including the complete
match (`$0`).

```c++
size_t Regex::named(std::string_view name) const;
```

Returns the capture group number corresponding to the given group name, or
`npos` if the name was not present.

```c++
Regex::match Regex::search(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
Regex::match Regex::operator()(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

This is the basic regex matching function (the operator is a synonym for the
`search()` function). When a non-zero offset is passed, the search begins at
the specified point in the string, but the text preceding it will still be
taken into account in lookbehind assertions. Matching will always fail if the
starting position is beyond the end of the subject string (but will succeed
if it is at the end and the pattern can match an empty substring).

This will throw `Regex::error` if the flags are inconsistent, if the starting
offset is not at a character boundary in a UTF-8 string (in the absence of
the `byte` flag), if the match fails and the `hard_error` flag was set, or if
a very complicated matching task exceeds PCRE2's internal limits.

```c++
size_t Regex::count(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

Returns the number of non-overlapping matches in the subject string.

```c++
Regex::match_range Regex::grep(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

Returns a pair of iterators over all matches in the subject string, optionally
starting at a given byte. It can throw the same exceptions as `search()`.

```c++
Regex::partition_type Regex::partition(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

Finds the first match in the string, and returns three string views covering
the part of the subject string before the match (including anything before the
starting offset), the match itself, and the part after the match. If there is
no match, the first field in the return value will be the full subject string,
and the other two will be empty (but will point to the end of the string
instead of being null). This can throw the same exceptions as `search()`.

```c++
std::string Regex::replace(std::string_view str, std::string_view fmt,
    size_t pos = 0, flag_type flags = 0) const;
void Regex::replace_in(std::string& str, std::string_view fmt,
    size_t pos = 0, flag_type flags = 0) const;
```

Replace the first match (if any) with the given format string, following
PCRE2's full set of replacement rules. The `replace()` function returns the
modified string, while `replace_in()` modifies the string in place. If the
`global` flag is used, this will replace all matches instead of only the
first. This can throw the same exceptions as `search()`.

```c++
Regex::split_range Regex::split(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

Returns a pair of iterators over the parts of the string between matches. This
can throw the same exceptions as `search()`.

### Static member functions

```c++
static Regex::version_type Regex::compile_version() noexcept;
static Regex::version_type Regex::runtime_version() noexcept;
static Regex::version_type Regex::unicode_version() noexcept;
```

PCRE2 version number queries. These return the PCRE2 version used when this
library was compiled, the current version available at runtime, and the
latest Unicode version supported.

```c++
static std::string Regex::escape(std::string_view str);
```

Inserts escape characters (backslashes) where necessary to return a pattern
that will match the argument string literally.

```c++
static Regex::token_range Regex::tokenize(const Regex& token,
    const Regex& delimiter, std::string_view str, size_t pos = 0,
    flag_type flags = 0);
```

Tokenizes a string, using two regexes to match the tokens and the delimiters
between them. Empty tokens or delimiters are allowed, but behaviour is
undefined if both patterns can match an empty string at the same point (this
does not apply if they can both match empty strings but not simultaneously,
e.g. different lookahead patterns).

The `tokenize()` function, or the iterator's increment operator, will throw
`Regex::token_error` if token or delimiter matching fails. Behaviour is
undefined if an iterator is dereferenced or incremented after an exception is
thrown.

The token iterators carry references to both of the `Regex` objects it was
created from. Behaviour is undefined if a token iterator is used after either
of the `Regex` objects is modified or destroyed.

### Exceptions

```c++
class Regex::error: public std::runtime_error {
    explicit error(int code);
    int code() const noexcept;
};
```

This is thrown from a regex constructor or matching function when the
underlying PCRE2 call reports an error. The `cdoe` parameter is the PCRE error
code.

```c++
class Regex::token_error: public std::runtime_error {
    explicit token_error(size_t pos);
    size_t pos() const noexcept;
};
```

This is thrown by `Regex::tokenize()` when no valid token or delimiter can be
found. The `pos` parameter is the byte offset into the subject string.

### Regex match class

```c++
class Regex::match;
```

The regex matching functions return a `Regex::match` object to report the
result of a matching attempt. Behaviour is undefined if a `Regex::match`
object is used after the `Regex` it was created from is modified or
destroyed.

```c++
match::match() noexcept;
match::match(const match& m) noexcept;
match::match(match&& m) noexcept;
match::~match() noexcept;
match& match::operator=(const match& m) noexcept;
match& match::operator=(match&& m) noexcept;
```

Life cycle operations. The default constructor returns the equivalent of a
failed match. Normally a match object will not be constructed explicitly by
users (apart from default construction), but returned from matching functions
in the `Regex` class.

```c++
bool match::matched(size_t i = 0) const noexcept;
bool match::full() const noexcept;
bool match::partial() const noexcept;
explicit match::operator bool() const noexcept;
```

These report whether the match was successful. If a non-zero index is
supplied, the result indicates whether a specific capture group was matched
(this will be false if the capture group does not exist). The boolean
operator is equivalent to `matched()`.

The `full()` and `partial()` functions can be used to distinguish between full
and partial matches; `matched()` is equivalent to `full() || partial()`. If
neither of the partial matching flags was used, `partial()` is always false
and `matched()` is equivalent to `full()`.

```c++
const char* match::begin(size_t i = 0) const noexcept;
const char* match::end(size_t i = 0) const noexcept;
size_t match::offset(size_t i = 0) const noexcept;
size_t match::endpos(size_t i = 0) const noexcept;
size_t match::count(size_t i = 0) const noexcept;
bool match::empty(size_t i = 0) const noexcept;
```

These report the position and length of the match, or of a numbered capture
group, in the subject string. The `offset()` and `endpos()` functions return
the start and end positions; `count()` returns the length of the match
(offsets are measured in code units, not characters, from the start of the
subject string, not the position from which matching was started). If the
match failed, or if a specific capture group was not matched, `begin()` and
`end()` return null pointers, `offset()` and `endpos()` return `npos`, and
`count()` returns zero. The `empty()` function returns true if the match
failed or matched an empty substring.

```c++
std::string_view match::str(size_t i = 0) const noexcept;
std::string_view match::operator[](size_t i) const noexcept;
match::operator std::string() const;
match::operator std::string_view() const noexcept;
```

These return the matching substring, for the complete match or a specific
capture group. All of these will return an empty string view if the match
failed, or the capture group was not matched (note that an empty string can
also be the legitimate result of a successful match).

```c++
bool match::matched(std::string_view name) const;
const char* match::begin(std::string_view name) const;
const char* match::end(std::string_view name) const;
size_t match::offset(std::string_view name) const;
size_t match::endpos(std::string_view name) const;
size_t match::count(std::string_view name) const;
bool match::empty(std::string_view name) const;
std::string_view match::str(std::string_view name) const;
std::string_view match::operator[](std::string_view name) const;
```

These return the same results as the functions above with the same names, but
the capture group is identified by name instead of number. If the named group
does not exist in the pattern, the result is the same as if the match
failed.

```c++
size_t match::first() const noexcept;
size_t match::last() const noexcept;
```

These return the indices of the first and last non-empty capture group (not
counting the complete match), or `npos` if the match failed or there are no
non-empty captures.

```c++
std::string_view match::last_mark() const noexcept;
```

Returns the name of the most recent named `(*COMMIT)`, `(*MARK)`, `(*PRUNE)`,
or `(*THEN)` tag in the pattern, or an empty string if no suitable tag was
found.

### Regex transform class

```c++
class Regex::transform
```

The `Regex::transform` class contains both a regex and a format string. It
provides operations equivalent to the `Regex::replace[_in]()` functions, but
may be more efficient if the same formatting operation is going to be applied
many times.

```c++
transform::transform() noexcept;
transform::transform(const Regex& pattern, std::string_view fmt,
    flag_type flags = 0);
transform::transform(std::string_view pattern, std::string_view fmt,
    flag_type flags = 0);
transform::transform(const transform& t);
transform::transform(transform&& t) noexcept;
transform::~transform() noexcept;
transform& transform::operator=(const transform& t);
transform& transform::operator=(transform&& t) noexcept;
```

Life cycle functions. The object is constructed from a regex (supplied either
as a precompiled regex or a pattern string), a format string, and optionally a
flag bitmask. In the second constructor, only matching and replacement flags
can be supplied; the third constructor can also take regex compilation flags,
and can throw the same exceptions as the corresponding regex constructor.

```c++
std::string transform::pattern() const;
std::string transform::format() const;
Regex::flag_type transform::flags() const noexcept;
```

These return the construction parameters.

```c++
std::string transform::replace(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
void transform::replace_in(std::string& str, size_t pos = 0,
    flag_type flags = 0) const;
std::string transform::operator()(std::string_view str, size_t pos = 0,
    flag_type flags = 0) const;
```

Replacement functions equivalent to `Regex::replace[_in]()`. The function call
operator is equivalent to `replace()`.
