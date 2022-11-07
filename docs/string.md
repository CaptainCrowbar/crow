# String Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/string.hpp"
namespace Crow;
```

This header defines some useful string manipulation functions.

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr const char* ascii_whitespace = "\t\n\f\r ";
```

Constants defined for convenience.

## Character functions

```c++
constexpr bool ascii_isalnum(char c) noexcept;   // [0-9A-Za-z]
constexpr bool ascii_isalpha(char c) noexcept;   // [A-Za-z]
constexpr bool ascii_iscntrl(char c) noexcept;   // [\0-\x1f\x7f]
constexpr bool ascii_isdigit(char c) noexcept;   // [0-9]
constexpr bool ascii_isgraph(char c) noexcept;   // [!-~]
constexpr bool ascii_islower(char c) noexcept;   // [a-z]
constexpr bool ascii_isprint(char c) noexcept;   // [ -~]
constexpr bool ascii_ispunct(char c) noexcept;   // [!-/:-@\[-`{-~]
constexpr bool ascii_isspace(char c) noexcept;   // [\t\n\f\r ]
constexpr bool ascii_isupper(char c) noexcept;   // [A-Z]
constexpr bool ascii_isxdigit(char c) noexcept;  // [0-9A-Fa-f]
constexpr char ascii_tolower(char c) noexcept;
constexpr char ascii_toupper(char c) noexcept;
```

These do essentially the same thing as the similarly named functions in
`<cctype>`, but consistently apply only to ASCII characters regardless of
locale.

```c++
constexpr bool ascii_isalnum_w(char c) noexcept;  // [0-9A-Z_a-z]
constexpr bool ascii_isalpha_w(char c) noexcept;  // [A-Z_a-z]
constexpr bool ascii_ispunct_w(char c) noexcept;  // [!-/:-@\[-^`{-~]
```

These perform the same functions as the similarly named functions above,
except that the underscore character as counted as a letter instead of a
punctuation mark (like the `\w` escape code in regular expressions).

## String manipulation functions

```c++
struct AsciiIcaseEqual;
    bool AsciiIcaseEqual::operator()(std::string_view a,
        std::string_view b) const noexcept;
struct AsciiIcaseLess;
    bool AsciiIcaseLess::operator()(std::string_view a,
        std::string_view b) const noexcept;
```

Case insensitive comparison function objects.

```c++
std::string ascii_uppercase(std::string_view str);
std::string ascii_lowercase(std::string_view str);
std::string ascii_titlecase(std::string_view str);
```

Convert a string to upper, lower, or title case. Only ASCII characters are
affected. Any group of adjacent letters is considered a word for title
casing.

```c++
size_t common_prefix_length(std::string_view str1,
    std::string_view str2) noexcept;
```

Returns the length (in bytes) of the longest common prefix of the two
strings.

```c++
std::string dent(size_t level);
```

Returns a string of `4*size` spaces, for convenience when generating indented
text.

```c++
template <std::integral T> std::string hex(T t);
```

Returns a hexadecimal representation of a number. This will always consist of
`2*sizeof(T)` digits. If `T` is signed, the output represents the twos
complement representation.

```c++
std::string indent_lines(std::string_view str, size_t spaces = 4);
```

Adds the specified number of spaces on the left of every non-empty line in the
string.

```c++
template <typename Range>
    std::string join(const Range& range, std::string_view delimiter = {});
```

Concatenates all elements of the range (which are expected to be `std::string`
or implicitly convertible to it), separated by the delimiter string.

```c++
std::string pad_left(std::string str, size_t len, char c = ' ');
std::string pad_right(std::string str, size_t len, char c = ' ');
```

If the input string is shorter than `len` bytes, it will be padded on the left
or right to the specified length. The input string will be returned unchanged
if it's already long enough.

```c++
std::pair<std::string_view, std::string_view>
    partition(std::string_view str, std::string_view chars = ascii_whitespace);
std::pair<std::string_view, std::string_view>
    partition_at(std::string_view str, std::string_view delimiter);
```

Splits a string into two at the first substring that either consists of one or
more characters from `chars`, or that exactly matches `delimiter`. If no
matching substring is found, or if `chars` or `delimiter` is empty, they will
return the original string and an empty string.

```c++
std::string quote(std::string_view str);
```

Encloses the string in quotes. Within the string, backslash escape codes will
be used for quotes, backslashes, control characters, and invalid UTF-8 bytes.

```c++
std::string repeat(std::string_view str, size_t n);
```

Returns a string composed of `n` copies of the first string.

```c++
std::string replace(std::string_view str, std::string_view target,
    std::string_view replacement);
std::string remove(std::string_view str, std::string_view target);
```

Replaces all (non-overlapping) occurrences of `target` in the string with
`replacement`. The `remove()` function is shorthand for
`replace(str,target,"")`. These will return the original string unchanged if
`target` is empty.

```c++
template <std::integral T> std::string roman(T t, bool lcase = false);
```

Expresses a number in Roman numerals, optionally in lower case. This will
throw `std::invalid_argument` if the value is less than 1 or greater than one
million.

```c++
std::vector<std::string_view> split(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::vector<std::string> splits(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::vector<std::string_view> split_at(std::string_view str,
    std::string_view delimiter);
std::vector<std::string> splits_at(std::string_view str,
    std::string_view delimiter);
std::vector<std::string_view> split_lines(std::string_view str);
std::vector<std::string> splits_lines(std::string_view str);
```

Split a string into multiple pieces, delimited by any substring that either
consists of one or more characters from `chars`, or that exactly matches
`delimiter`. The `split[s]()` functions trim leading and trailing delimiters,
and will never have an empty string in the returned vector. The other
functions may include empty strings in the returned vector, if there are
leading, trailing, or adjacent delimiters.

The `split[s]_lines()` functions split the string into lines, delimited by
`"\n"` or `"\r\n"`, removing the trailing line breaks from each line.

```c++
std::string trim(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::string trim_left(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::string trim_right(std::string_view str,
    std::string_view chars = ascii_whitespace);
```

Trims any characters in the `chars` list from the left, right, or both ends of
the string.

```c++
std::string unqualify(std::string_view str,
    std::string_view delimiters = ".:");
```

Removes any qualifying prefix from a name, up to the last occurrence of any of
the delimiter characters. The string will be returned unchanged if it does
not contain any delimiters.

```c++
std::string unwrap_lines(std::string_view str);
```

Adjacent lines (not separated by at least one empty line) are concatenated,
with a space replacing the line break between them. Empty lines are left
unchanged.

```c++
std::string wrap_lines(std::string_view str, size_t width = 78,
    size_t margin = npos);
```

Paragraphs are word wrapped to the given width. Words are delimited by ASCII
whitespace; paragraphs are delimited by at least one empty line (the empty
lines appear in the output). If `margin=npos`, the indentation of the first
line of each paragraph is used for the whole paragraph; otherwise, the
`margin` value is used for indenting all text, and individual paragraph
indentation in the input is ignored.

By default, individual words too long to fit on one line will be allowed to
violate the right margin. If the `checked` flag is set, this will throw
`std::length_error` if a word is too long, or if the margin is too big for
the width.

## String parsing functions

```c++
bool to_boolean(const std::string& str);
```

Any of `"true"`, `"t"`, `"yes"`, `"y"`, `"on"`, `"1"`, or an empty string
return true; `"false"`, `"f"`, `"no"`, `"n"`, `"off"`, or `"0"` return false
(case insensitive). Any other argument throws `std::invalid_argument`.

```c++
template <std::integral T> T to_integer(const std::string& str, int base = 10);
short to_short(const std::string& str, int base = 10);
unsigned short to_ushort(const std::string& str, int base = 10);
int to_int(const std::string& str, int base = 10);
unsigned to_uint(const std::string& str, int base = 10);
long to_long(const std::string& str, int base = 10);
unsigned long to_ulong(const std::string& str, int base = 10);
long long to_llong(const std::string& str, int base = 10);
unsigned long long to_ullong(const std::string& str, int base = 10);
int8_t to_int8(const std::string& str, int base = 10);
uint8_t to_uint8(const std::string& str, int base = 10);
int16_t to_int16(const std::string& str, int base = 10);
uint16_t to_uint16(const std::string& str, int base = 10);
int32_t to_int32(const std::string& str, int base = 10);
uint32_t to_uint32(const std::string& str, int base = 10);
int64_t to_int64(const std::string& str, int base = 10);
uint64_t to_uint64(const std::string& str, int base = 10);
ptrdiff_t to_ptrdiff(const std::string& str, int base = 10);
size_t to_size(const std::string& str, int base = 10);
```

Convert a string, expressed in a given base, to an integer. Only base 2, 10,
and 16 are supported. These will throw `std::invalid_argument` if the base is
invalid or the string is not a valid integer with the appropriate base,
signedness, and range.

```c++
template <std::floating_point T> T to_floating(const std::string& str);
float to_float(const std::string& str);
double to_double(const std::string& str);
long double to_ldouble(const std::string& str);
```

Convert a string, expressed in decimal, to a floating point number. These will
throw `std::invalid_argument` if the string is not a valid number of the
specified type.

## String query functions

```c++
std::pair<size_t, size_t> line_and_column(std::string_view str, size_t pos,
    Usize mode, size_t tab_size = 4);
```

Returns the line and column number corresponding to a given position within a
string. Return values are zero-based. The input position is a byte offset,
which will be clamped to the length of the string. The returned line number is
based on line feed characters only. The `mode` argument determines how columns
are counted.

In modes other than `units`, this will throw `UnicodeError` if invalid UTF-8
encoding is encountered, or if the input position is not on a code point
boundary.

## Type functions

```c++
template <typename T> std::string type_name();
```

Returns a human readable type name, derived from `std::type_info::name()`.
MSVC already supplies human readable names, so those are simply passed through
unchanged. On GCC, and GCC-like compilers such as Clang and Xcode, this will
attempt to demangle the name by calling `abi::__cxa_demangle()`; if this fails
it will give up and return the mangled name.

## String literals

These are in `namespace Crow::Literals`.

```c++
std::string operator""_doc(const char* ptr, size_t len);
```

The `""_doc` literal removes a leading blank line (if present), strips all
trailing whitespace, and strips leading indentation from all non-empty lines,
using the indentation of the last line to indicate where the left margin is
intended to be. Results are unspecified if the last line of the original
string contains anything other than whitespace, or if indentation in the
original string contains a mixture of spaces and tabs.

Example:

```c++
R"(
    Hello world.
        Hello again.
    Goodbye.
    )"_doc
```

Result:

```c++
"Hello world.\n"
"    Hello again.\n"
"Goodbye.\n"
```

```c++
std::vector<std::string> operator""_qw(const char* ptr, size_t len);
```

The `""_qw` literal splits a string into words delimited by whitespace.

Example:

```c++
" abc def ghi "_qw
```

Result:

```c++
std::vector<std::string> {"abc", "def", "ghi"}
```
