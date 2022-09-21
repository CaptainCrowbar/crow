# Formatting Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/format.hpp"
namespace Crow;
```

This header supplies the main formatting functions of the library.

## Contents

* TOC
{:toc}

## Formatting rules for standard types

Format specification strings are parsed by the `FormatSpec` class (see below).
A format string takes the form `"<mode>[<options>][<prec>]"`. It consists of
a single-letter mode, zero or more options, and an optional precision
(an unsigned integer). The mode and options are restricted to ASCII letters,
and are case sensitive. An empty string is also a valid format spec; this has
a default interpretation specific to the type being formatted.

Types not listed here are simply converted to strings by whatever means is
available (this is described in more detail below), with no formatting
control; any format specification supplied is ignored. User defined types can
be formatted by use of the customization points described below.

* **Type formatting**
    * Applies to: All types
    * Mode:
        * `T` = Return the name of the argument's type instead of formatting its value
    * Options:
        * `Z` = Use `"--"` in place of a null value (zero, an empty string, or a null pointer)
    * Notes:
        * This is used for any type if the format spec has mode `T`
        * Reference and `const` qualifiers are stripped from the type because
          the fully qualified original type of a function argument can't be
          easily retrieved
* **Range formatting**
    * Applies to: All range types except `string`
    * Options:
        * `N` = Leave out the enclosing brackets
* **Boolean formatting**
    * Applies to: `bool`
    * Default format is `"b"`
    * Mode:
        * `B,b` = `"True/False"` or `"true/false"`
        * `Y,y` = `"Yes/No"` or `"yes/no"`
    * Notes:
        * Integer formatting codes can also be used
* **Number formatting**
    * These are common to integer and floating point formats
    * Options:
        * `A,a` = Use apostrophes for digit grouping (C++ convention)
        * `C,c` = Use commas for digit grouping (English convention)
        * `U,u` = Use underscores for digit grouping (Ada convention)
        * `W,w` = Use whitespace for digit grouping (SI convention)
        * `E` = Elide delimiter if there is one more than a single group (e.g. `"1234"` but `"12,345"`)
        * `s` = Always show a sign
    * Notes:
        * If multiple grouping options are present, the first one will be used
        * For decimal numbers, lower case grouping options imply groups of 3,
          upper case groups of 5 (Abramowitz & Stegun convention)
        * Binary and hexadecimal numbers are always grouped by 4 digits
* **Integer formatting**
    * Applies to: Any standard integral arithmetic type
    * Default format is `"n1"`
    * Mode:
        * `B,b` = Binary integer
        * `N,n` = Decimal integer
        * `R,r` = Roman numerals (throws `invalid_argument` if less than 1, or 0 if `Z` is set)
        * `X,x` = Hexadecimal integer
    * Precision:
        * Show at least this many digits (ignored for Roman numerals)
    * Notes:
        * Floating point specs can also be used for integers
        * For `R/r` and `X/x` formats, the case of the mode is used for output
* **Floating point formatting**
    * Applies to: Any standard floating point arithmetic type
    * Default format is `"gz6"`
    * Mode:
        * `D,d` = Decimal format
        * `E,e` = Exponential (scientific) format
        * `F,f` = Fixed point format
        * `G,g` = Use `D/d` if `1e-3<=|value|<1e6` or `value=0`, otherwise `E/e`
        * `P,p` = Probability formatting (see below)
    * Options:
        * `e` = Use a comma for the decimal point (European convention)
        * `S` = Always show a sign on the exponent
        * `z` = Strip trailing zeroes
    * Precision:
        * For `F/f` formats, this is the number of decimal places to display
        * Otherwise, this is the number of significant figures to display
        * Precision defaults to 6 if not supplied
    * Notes:
        * For `E/e` and `G/g` formats, the case of the mode is used for output
        * Probability formatting:
            * Treats leading nines as non-significant
            * `P` format multiplies by 100 to show the value as a percentage
            * Throws `std::domain_error` if the argument is not 0-1
* **String formatting**
    * Applies to: `std::string` and `const char*`
    * Default format is `"s"`
    * Mode:
        * `Q,q` = Quote the string, with escapes (e.g. `"\"Hello\\n\""`)
        * `S,s` = Just print the literal string
        * `X,x` = Expand the string as hex byte values (e.g. `"48 65 6c 6c 6f 0a"`)
    * Options:
        * `z` = No spaces in `X/x` format
    * Precision:
        * If a precision is supplied, the string is truncated to that many characters (Unicode code points)
* **Time interval formatting**
    * Applies to: Any instantiation of `std::chrono::duration`
    * Default format is `"t"`
    * Mode:
        * `s` = Format as seconds (the common numerical options also apply here)
        * `t` = Expanded time format (e.g. `"12h34m56s"`)
    * Precision:
        * If a precision is supplied, seconds are shown to this many decimal places
* **Date formatting**
    * Applies to: `std::chrono::system_clock::time_point`
    * Default format is `"i"`
    * Mode:
        * `d` = Day, month, year, time (e.g. `"3 Feb 2021 04:05:06"`)
        * `D` = Weekday, day, month, year, time (e.g. `"Wed 3 Feb 2021 04:05:06"`)
        * `i` = ISO 8601 with space delimiter (e.g. `"2021-02-03 04:05:06"`)
        * `I` = ISO 8601 with T delimiter (e.g. `"2021-02-03T04:05:06"`)
    * Options:
        * `d` = Show only the date (no time of day)
        * `l` = Use the local time zone (default is UTC)
        * `t` = Show only the time of day (no date)
    * Precision:
        * If a precision is supplied, seconds are shown to this many decimal places

## Customization points

Formatting for a user defined type can be configured in any of three ways:

(1) Derive your class from `Formatted`:

```c++
class Formatted {
    virtual ~Formatted() noexcept;
    virtual std::string str(const FormatSpec& spec = {}) const = 0;
};
std::string to_string(const Formatted& obj, const FormatSpec& spec = {});
std::ostream& operator<<(std::ostream& out, const Formatted& obj);
```

Any class derived from `Formatted` must implement a `str()` function that
formats an object according to the `FormatSpec`. For any class `T` derived
from `Formatted`, instantiations of `FormatType`, `to_string(T)`, and
`operator<<(ostream&,T)` are automatically defined.

(2) Specialise the `FormatType` template:

```c++
template <typename T> struct FormatType {
    std::string operator()(const T& t, const FormatSpec& spec) const;
}
```

This template defines the basic formatting operation for any formattable
types. It handles all predefined format types (listed below), and can be
fully or partially specialized for user defined types.

(3) Give your class a `str()` method:

```c++
std::string T::str() const;
std::string T::str(const FormatSpec& spec) const;
```

Either of these signatures may be used: the `str()` function can use a
`FormatSpec` or just yield a single format if no options are required. If
both are present, the `FormatSpec` version will always be used.

## Formatting algorithm

Many common ways of providing formatting for a type are recognised. The
following algorithm is used to determine how to format an object:

* if the format spec uses `T` mode
    * ✉️ return the type name as described above
* if the type is `std::nullptr_t`
    * ✉️ return `"<null>"`
* else if the type is `bool`
    * 🎁 use boolean formatting as described above
* else if the type is `char`
    * 🎁 make a one-character string, then use string formatting as described above
* else if the type is `char16_t`, `char32_t`, or `wchar_t`
    * 🎁 convert it to a UTF-8 string, then use string formatting as described above
* else if the type is an integer (according to `std::is_integral<T>`)
    * 🎁 use integer formatting as described above
* else if the type is floating point (according to `std::is_floating_point<T>`)
    * 🎁 use floating point formatting as described above
* else if the type is an instantiation of `std::duration`
    * 🎁 use time span formatting as described above
* else if the type is `std::chrono::system_clock::time_point`
    * 🎁 use date formatting as described above
* else if the type is derived from `Formatted`
    * 🎁 call the object's `str()` function
* else if the type has a `str(FormatSpec)` function
    * 🎁 call the object's `str()` function
* else if the type has a `str()` function
    * ✉️ call the object's `str()` function
* else if a `to_string(T)` function is found (either by ADL or in `namespace std`)
    * ✉️ call `to_string(T)`
* else if the type is a string-like type (see below), or implicitly convertible to one
    * 🎁 return `"<null>"` if the object is a null pointer, otherwise use string formatting as described above
* else if the type is explicitly convertible to `std::string`
    * ✉️ convert it to a string and return that (string formatting is not applied here)
* else if the type is derived from `std::exception`
    * ✉️ return the object's `what()` string
* else if the object is a map-like range
    * 🎁 call `format_map(T)`
* else if the object is a range
    * 🎁 call `format_range(T)`
* else if the object is a pointer
    * ✉️ return `"<null>"` if the pointer is null, otherwise return the hex address as `"<0xHHH...>"`
* else if an output operator exists (`std::ostream << T`)
    * ✉️ use a `std::ostringstream` to format the object
* else
    * ✉️ return the object's type name and address

Notes:

* Entries marked 🎁 use the `FormatSpec`; those marked ✉️ do not.
* For the purposes of this algorithm, a "string-like type" is defined to be any of the following:
    * `std::string`
    * `std::string_view`
    * `char*` or `const char*`
    * A C-style array of `char`
    * The `char16_t`, `char32_t`, or `wchar_t` equivalents of any of the above
    * Any type that can be implicitly converted to any of the above

## Format specification

```c++
class FormatSpec;
```

This class parses a formatting specification string.

```c++
FormatSpec::FormatSpec(const std::string& str);
FormatSpec::FormatSpec(const char* str);
```

Parse a format spec containing a mode, zero to many options, and an optional
precision, as described above. This will throw `std::invalid_argument` if the
string is not a valid format spec. A valid spec consists of:

* An ASCII letter representing the mode, or `'*'` to select the default mode
* Zero or more letters representing options
* Zero or more digits representing a precision

An empty string is also a valid format spec.

```c++
FormatSpec::FormatSpec(char m, const std::string& o, int p);
```

Construct a format spec from a mode, a string of options, and a precision.
This will throw `std::invalid_argument` if `m` is not a letter or star, if
`o` contains any non-letter characters, or if `p<-1`.

```c++
FormatSpec::FormatSpec();
FormatSpec::FormatSpec(const FormatSpec& spec);
FormatSpec::FormatSpec(FormatSpec&& spec) noexcept;
FormatSpec::~FormatSpec() noexcept;
FormatSpec& FormatSpec::operator=(const FormatSpec& spec);
FormatSpec& FormatSpec::operator=(FormatSpec&& spec) noexcept;
```

Other life cycle functions.

```c++
bool FormatSpec::empty() const noexcept;
```

True if this is a default spec containing no user specified elements.

```c++
char FormatSpec::mode() const noexcept;
char FormatSpec::lcmode() const noexcept;
```

Return the mode letter, optionally converted to lower case. This will return a
null character if the original format spec was empty, or the mode was `'*'`.

```c++
bool FormatSpec::option(char c) const noexcept;
```

True if this option code is present.

```c++
std::string FormatSpec::options() const;
```

Returns the full option string.

```c++
char FormatSpec::find_mode(const std::string& chars) const noexcept;
```

If the mode letter is on the list, this returns the mode letter; otherwise, it
returns a null character.

```c++
char FormatSpec::find_option(const std::string& chars) const noexcept;
```

If any option letter is on the list, this returns the first matching letter in
the option string; otherwise, it returns a null character.

```c++
void FormatSpec::no_option(const std::string& chars) noexcept;
```

Removes any matching option letters from the spec.

```c++
int FormatSpec::prec() const noexcept;
```

Returns the precision, or -1 if no precision was supplied.

```c++
void FormatSpec::default_prec(int p);
```

If no precision was supplied (i.e. if the precision is -1), the precision is
changed to this value. If a non-negative precision has already been set, this
will not change it.

```c++
bool FormatSpec::has_prec() const noexcept;
```

True if an explicit precision was specified.

```c++
void FormatSpec::set_prec(int p) noexcept;
```

Sets the precision.

```c++
std::string FormatSpec::str() const;
```

Returns the format spec as a string.

## Type specific formatting functions

```c++
std::string format_boolean(bool b,
    const FormatSpec& spec);
template <typename T> std::string format_integer(T t,
    FormatSpec spec = {});
template <typename T> std::string format_floating_point(T t,
    FormatSpec spec = {});
template <typename T> std::string format_number(T t,
    FormatSpec spec = {});
```

Arithmetic type formatting functions. These format their arguments according
to the rules laid out above. The `format_number()` function will call
`format_integer()` or `format_floating_point()`, whichever is appropriate to
the argument type.

```c++
std::string format_string(const std::string& str,
    const FormatSpec& spec = {});
std::string format_string(const std::string_view& str,
    const FormatSpec& spec = {});
std::string format_string(const char* str,
    const FormatSpec& spec = {});
std::string format_string(const std::u16string& str,
    const FormatSpec& spec = {});
std::string format_string(const std::u16string_view& str,
    const FormatSpec& spec = {});
std::string format_string(const char16_t* str,
    const FormatSpec& spec = {});
std::string format_string(const std::u32string& str,
    const FormatSpec& spec = {});
std::string format_string(const std::u32string_view& str,
    const FormatSpec& spec = {});
std::string format_string(const char32_t* str,
    const FormatSpec& spec = {});
std::string format_string(const std::wstring& str,
    const FormatSpec& spec = {});
std::string format_string(const std::wstring_view& str,
    const FormatSpec& spec = {});
std::string format_string(const wchar_t* str,
    const FormatSpec& spec = {});
```

String formatting functions. All of these use the string formatting rules laid
out above. UTF-32 strings are first converted to UTF-8.

## Generic formatting functions

```c++
template <typename T> std::string format_object(const T& t,
    const FormatSpec& spec = {});
```

Calls `FormatType<T>()(t, spec)`.

```c++
template <typename Range> std::string format_range(const Range& r,
    const FormatSpec& spec = {});
template <typename Range> std::string format_map(const Range& r,
    const FormatSpec& spec = {});
```

These format a sequential range, or a map-like range. A range is anything that
could be used in a range-based `for` statement; a map-like range is any range
whose elements are a `std::pair` or anything else that has `first` and
`second` data members.

Ordinary ranges use a format based on JSON arrays:

```
vector { 1, 2, 3, 4, 5 } => "[1,2,3,4,5]"
```

Map-like ranges use a format based on JSON objects:

```
map { { 1, "abc" }, { 2, "def" }, { 3, "ghi" } } => "{1:abc,2:def,3:ghi}"
```

```c++
template <typename T> std::string format_via_stream(const T& t);
```

Formats an object with an output operator, by writing it to a
`std::ostringstream` and reading the string back.

## Formatter class

```c++
class Formatter {
    Formatter();
    explicit Formatter(const std::string& pattern);
    Formatter(const Formatter& f);
    Formatter(Formatter&& f) noexcept;
    ~Formatter() noexcept;
    Formatter& operator=(const Formatter& f);
    Formatter& operator=(Formatter&& f) noexcept;
    template <typename... Args>
        std::string operator()(const Args&... args) const;
};
template <typename... Args>
    std::string fmt(const std::string& pattern, const Args&... args);
```

This class parses a format string, and then inserts a set of arguments into
that string, returning the result.

A format string contains substitution fields enclosed in braces. Each field
consists of a number, indicating which argument goes in here (zero-based
indexing), optionally followed by a colon and a format spec. Backslashes can
be used to escape braces that are intended to be taken literally.

The constructor will throw `std::invalid_argument` if the format string is
malformed(mismatched braces or an invalid format spec). The function call
operator will throw `std::invalid_argument` if there are not enough arguments
to match the fields in the format string.

The `fmt()` function simply calls `Formatter(pattern)(args...)`.

Example:

```
fmt("Hello {2} {0} {1:f3}", 123, 456, "world");
    => "Hello world 123 456.000"
```

## Formatter literals

This is in `namespace Crow::Literals`.

```c++
Formatter operator""_fmt(const char* ptr, size_t len);
````

Returns `Formatter(std::string(ptr, len))`.

## Formatted I/O functions

The function signatures below are simplified for clarity. Ellipses represent
variadic template arguments, not old-style C varargs.

* `FMT = std::string format_pattern`
* `OS = std::ostream&`
* `FP = FILE*`

| Function                      | Function                       |
| --------                      | --------                       |
| `void printc(FMT, ...)`       | `void printcf(FMT, ...)`       |
| `void printct(OS, FMT, ...)`  | `void printcft(OS, FMT, ...)`  |
| `void printct(FP, FMT, ...)`  | `void printcft(FP, FMT, ...)`  |
| `void printc()`               | `void printcf()`               |
| `void printct(OS)`            | `void printcft(OS)`            |
| `void printct(FP)`            | `void printcft(FP)`            |
| `void printp(...)`            | `void printpf(...)`            |
| `void printpt(OS, ...)`       | `void printpft(OS, ...)`       |
| `void printpt(FP, ...)`       | `void printpft(FP, ...)`       |
| `void printp()`               | `void printpf()`               |
| `void printpt(OS)`            | `void printpft(OS)`            |
| `void printpt(FP)`            | `void printpft(FP)`            |

Functions with a `c` suffix take a formatting string, followed by a list of
arguments to format. Functions with a `p` suffix print their arguments,
formatted with `format_object()`, and delimited by spaces. The suffixes are
intended to suggest "works like C `printf()`" and "works like Python
`print()`".

Functions with an `f` suffix flush the output stream after writing.

Functions with a `t` suffix take an output stream or file pointer as their
first argument (the suffix stands for "to..."). Functions that do not take an
output stream write to standard output.

A line feed will be added to the output, if the output text did not already
end with one. The versions that take no arguments other than an output stream
just write a line break.

Output is atomic with respect to other threads; the text written by any call
will not be interleaved with any output from another thread.

```c++
template <typename... Args> std::string prints(const Args&... args);
```

This formats a space delimited list of arguments, in the same way as
`printp()`, but without the terminating line feed, and returns the formatted
string instead of writing it out.
