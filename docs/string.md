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

## String formatting functions

```c++
template <std::integral T> std::string bin(T t, size_t digits = 8 * sizeof(T));
template <std::integral T> std::string dec(T t, size_t digits = 1);
template <std::integral T> std::string hex(T t, size_t digits = 2 * sizeof(T));
```

Return a binary, decimal, or hexadecimal representation of a number, with at
least the specified number of digits. For `bin()` and `hex()`, if `T` is
signed, the output represents the twos complement representation.

```c++
template <std::integral T> std::string roman(T t, bool lcase = false);
```

Expresses a number in Roman numerals, optionally in lower case. This will
throw `std::invalid_argument` if the value is less than 1 or greater than one
million.

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
std::vector<std::string> splits(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::vector<std::string_view> splitv(std::string_view str,
    std::string_view chars = ascii_whitespace);
std::vector<std::string> splits_at(std::string_view str,
    std::string_view delimiter);
std::vector<std::string_view> splitv_at(std::string_view str,
    std::string_view delimiter);
std::vector<std::string> splits_lines(std::string_view str,
    bool keep = false);
std::vector<std::string_view> splitv_lines(std::string_view str,
    bool keep = false);
```

Split a string into multiple pieces, delimited by any substring that either
consists of one or more characters from `chars`, or that exactly matches
`delimiter`. The `split[sv]()` functions trim leading and trailing delimiters,
and will never include an empty string in the returned vector. The other
functions may include empty strings in the returned vector, if there are
leading, trailing, or adjacent delimiters.

The `split[sv]_lines()` functions split the string into lines, delimited by
`"\n"` or `"\r\n"`. The trailing line breaks from each line will be retained
if the `keep` flag is set, otherwise discarded.

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
std::string wrap_lines(std::string_view str, Usize mode = Usize::columns,
    size_t width = npos, size_t margin = npos, bool checked = false);
```

Paragraphs are word wrapped to the given width. Words are delimited by ASCII
whitespace; paragraphs are delimited by at least one empty line (the empty
lines appear in the output).

The `mode` argument determines how line lengths are measured (see
[`crow/unicode`](unicode.html)).

If `width=npos`, the width defaults to the value of the `COLUMNS` environment
variable minus 1. If `COLUMNS` is not set to a positive integer value, the
function will behave as if `COLUMNS=80`.

If `margin=npos`, the indentation of the first line of each paragraph is used
for the whole paragraph; otherwise, the `margin` value is used for indenting
all text, and individual paragraph indentation in the input is ignored. If the
`checked` flag is set, the function will throw `std::length_error` if
`margin>=width`.

If an individual word is too long to fit on one line, the default behaviour is
to allow it to violate the right margin. If the `checked` flag is set, the
function will throw `std::length_error` in this situation.

## String parsing functions

All of these come in three versions, taking a string, a string view, or a
character pointer, to avoid unnecessary copying. Apart from `to_boolean()`, an
empty string or a null pointer will throw `std::invalid_argument`.

```c++
bool to_boolean(const char* str);
bool to_boolean(const std::string& str);
bool to_boolean(std::string_view str);
```

Any of `"true"`, `"t"`, `"yes"`, `"y"`, `"on"`, or `"1"` return true;
`"false"`, `"f"`, `"no"`, `"n"`, `"off"`, `"0"`, an empty string, or a null
pointer return false (case insensitive). Any other argument throws
`std::invalid_argument`.

```c++
template <std::integral T> T to_integer(const std::string& str, int base = 10);
template <std::integral T> T to_integer(std::string_view str, int base = 10);
template <std::integral T> T to_integer(const char*str, int base = 10);
short to_short(const std::string& str, int base = 10);
short to_short(std::string_view str, int base = 10);
short to_short(const char*str, int base = 10);
unsigned short to_ushort(const std::string& str, int base = 10);
unsigned short to_ushort(std::string_view str, int base = 10);
unsigned short to_ushort(const char*str, int base = 10);
int to_int(const std::string& str, int base = 10);
int to_int(std::string_view str, int base = 10);
int to_int(const char*str, int base = 10);
unsigned to_uint(const std::string& str, int base = 10);
unsigned to_uint(std::string_view str, int base = 10);
unsigned to_uint(const char*str, int base = 10);
long to_long(const std::string& str, int base = 10);
long to_long(std::string_view str, int base = 10);
long to_long(const char*str, int base = 10);
unsigned long to_ulong(const std::string& str, int base = 10);
unsigned long to_ulong(std::string_view str, int base = 10);
unsigned long to_ulong(const char*str, int base = 10);
long long to_llong(const std::string& str, int base = 10);
long long to_llong(std::string_view str, int base = 10);
long long to_llong(const char*str, int base = 10);
unsigned long long to_ullong(const std::string& str, int base = 10);
unsigned long long to_ullong(std::string_view str, int base = 10);
unsigned long long to_ullong(const char*str, int base = 10);
int8_t to_int8(const std::string& str, int base = 10);
int8_t to_int8(std::string_view str, int base = 10);
int8_t to_int8(const char*str, int base = 10);
uint8_t to_uint8(const std::string& str, int base = 10);
uint8_t to_uint8(std::string_view str, int base = 10);
uint8_t to_uint8(const char*str, int base = 10);
int16_t to_int16(const std::string& str, int base = 10);
int16_t to_int16(std::string_view str, int base = 10);
int16_t to_int16(const char*str, int base = 10);
uint16_t to_uint16(const std::string& str, int base = 10);
uint16_t to_uint16(std::string_view str, int base = 10);
uint16_t to_uint16(const char*str, int base = 10);
int32_t to_int32(const std::string& str, int base = 10);
int32_t to_int32(std::string_view str, int base = 10);
int32_t to_int32(const char*str, int base = 10);
uint32_t to_uint32(const std::string& str, int base = 10);
uint32_t to_uint32(std::string_view str, int base = 10);
uint32_t to_uint32(const char*str, int base = 10);
int64_t to_int64(const std::string& str, int base = 10);
int64_t to_int64(std::string_view str, int base = 10);
int64_t to_int64(const char*str, int base = 10);
uint64_t to_uint64(const std::string& str, int base = 10);
uint64_t to_uint64(std::string_view str, int base = 10);
uint64_t to_uint64(const char*str, int base = 10);
ptrdiff_t to_ptrdiff(const std::string& str, int base = 10);
ptrdiff_t to_ptrdiff(std::string_view str, int base = 10);
ptrdiff_t to_ptrdiff(const char*str, int base = 10);
size_t to_size(const std::string& str, int base = 10);
size_t to_size(std::string_view str, int base = 10);
size_t to_size(const char*str, int base = 10);
```

Convert a string, expressed in a given base, to an integer. Only base 2, 10,
and 16 are supported. These will throw `std::invalid_argument` if the base is
invalid or the string is not a valid integer with the appropriate base,
signedness, and range.

```c++
template <std::floating_point T> T to_floating(const std::string& str);
template <std::floating_point T> T to_floating(std::string_view str);
template <std::floating_point T> T to_floating(const char* str);
float to_float(const std::string& str);
float to_float(std::string_view str);
float to_float(const char* str);
double to_double(const std::string& str);
double to_double(std::string_view str);
double to_double(const char* str);
long double to_ldouble(const std::string& str);
long double to_ldouble(std::string_view str);
long double to_ldouble(const char* str);
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

## String view functions

These are some convenience functions for manipulating string views.

```c++
constexpr const char* view_endptr(std::string_view view) noexcept;
```

Returns a pointer to the end of the string, equal to `data()+size()`.

```c++
constexpr bool view_is_null(std::string_view view) noexcept;
```

True if the view is null (equivalent to a default constructed view). This is
false for views that are empty but have a non-null data pointer.

```c++
constexpr bool view_is_substring(std::string_view view,
    std::string_view str) noexcept;
```

True if `view` is a part of (or identical to) `str`. This compares pointers,
not data; it will return false if `view.data()` is not inside `str`, even if
`str.find(view)` would return a valid position. It will return true if both
views are null, otherwise false if either one is null.

```c++
constexpr size_t view_pos(std::string_view str,
    std::string_view view) noexcept;
constexpr size_t view_endpos(std::string_view str,
    std::string_view view) noexcept;
```

Return the position of the start or end of a view relative to its enclosing
string. These will return zero if both arguments are null. Otherwise,
behaviour is undefined if `view` is not a substring of `str`.

```c++
constexpr std::string_view view_begin(std::string_view view) noexcept;
constexpr std::string_view view_end(std::string_view view) noexcept;
```

These return empty views at the start or end of the given view. They will
return a null view if the argument is null.

```c++
constexpr std::string_view view_cat(std::string_view left,
    std::string_view right) noexcept;
```

Concatenate two views, returning a view that stretches from the beginning of
`left` to the end of `right`. If either argument is null, the other argument
will be returned unchanged. Behaviour is undefined if the arguments are not
substrings of the same underlying string, or if the end of `right` is to the
left of the beginning of `left`.

```c++
constexpr std::string_view view_extend(std::string_view view,
    size_t add) noexcept;
constexpr std::string_view view_extend(std::string_view str,
    std::string_view view, size_t add) noexcept;
```

Add the given number of bytes to the end of the given view. The first version
extends the length unconditionally; behaviour is undefined if this would
extend it beyond the actual end of the underlying string. The second version
limits the extension to the end of the underlying string; behaviour is
undefined if `view` is not a substring of `str`. For both versions, behaviour
is undefined if any of the string view arguments are null.

```c++
constexpr std::string_view view_left_of(std::string_view str,
    std::string_view view) noexcept;
constexpr std::string_view view_right_of(std::string_view str,
    std::string_view view) noexcept;
constexpr std::string_view view_from_left(std::string_view str,
    std::string_view view) noexcept;
constexpr std::string_view view_from_right(std::string_view str,
    std::string_view view) noexcept;
```

Return the substrings of `str` to the left or right of `view`, excluding
(`view_left/right_of()`) or including (`view_from_left/right()`) `view`
itself. These will return a null view if both arguments are null; otherwise,
behaviour is undefined if `view` is not a substring of `str`.

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
