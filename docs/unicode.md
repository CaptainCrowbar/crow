# Unicode Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/unicode.hpp"
namespace Crow;
```

This header defines functions related to Unicode and UTF-8 encoding.

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr char32_t not_unicode = ~ char32_t(0);
```

This is returned by some of the functions below to indicate failure.

## Concepts

```c++
template <typename C> concept CharacterType;
```

This is satisfied only for `char`, `char16_t`, `char32_t`, and `wchar_t`
(`char8_t` is not supported yet).

## Character functions

```c++
constexpr bool is_unicode(char32_t c) noexcept;
```

True if the code point is a valid Unicode scalar value, in the range
`0-0xd7ff` or `0xe000-0x10ffff`.

```c++
template <CharacterType C>
    char32_t decode_char(const std::basic_string<C>& str, size_t& pos) noexcept;
```

Reads one encoded Unicode character from the input string, starting at the
given position. On success, returns the decoded character and advances the
position to the next character. On failure (when either the string contains
invalid Unicode, or the starting position is already at or past the end of the
string), it returns `not_unicode` and advances the position to the next point
where a character could start, up to the maximum number of code points
possible for an encoded character (or to the end of the string, if that comes
first).

The UTF-32 version does no actual decoding, but checks that the next character
is valid. This also applies to all of the other UTF-32 based functions below.

```c++
template <CharacterType C>
    char32_t check_decode_char(const std::basic_string<C>& str, size_t& pos);
```

This performs the same operation as `decode_char()` above, but throws an
exception (and does not advance the position) instead of returning an error
code on failure. This will throw `std::out_of_range` if the initial position
is at or past the end of the string, or `std::invalid_argument` if invalid
Unicode is encountered.

```c++
template <CharacterType C>
    bool encode_char(char32_t c, std::basic_string<C>& str);
```

Encodes a Unicode code point, appending the encoded character to the string.
This will return true on success, false if the character argument is not a
valid Unicode scalar value.

```c++
template <CharacterType C>
    void check_encode_char(char32_t c, std::basic_string<C>& str);
```

This performs the same operation as `encode_char()` above, but throws
`std::invalid_argument` instead of returning false on failure.

## String functions

```c++
template <CharacterType C>
    std::u32string decode_string(const std::basic_string<C>& str);
```

Convert an encoded Unicode string to a UTF-32 string. This will throw
`std::invalid_argument` if invalid encoding is encountered.

```c++
template <CharacterType C>
    void encode_string(const std::u32string& src, std::basic_string<C>& dst);
std::string to_utf8(const std::u32string& utf32);
std::u16string to_utf16(const std::u32string& utf32);
std::u32string to_utf32(const std::u32string& utf32);
std::wstring to_wstring(const std::u32string& utf32);
```

These convert a UTF-32 string into an encoded string. They will throw
`std::invalid_argument` if an invalid Unicode scalar value is encountered.

```c++
template <CharacterType C>
    bool is_valid_utf(const std::basic_string<C>& str);
```

True if the string contains valid UTF (or is empty).

```c++
template <CharacterType C>
    size_t utf_length(const std::basic_string<C>& str);
```

Returns the number of encoded Unicode scalar values in the string. It will
throw `std::invalid_argument` if invalid encoding is encountered.

```c++
template <CharacterType C>
    size_t utf_width(const std::basic_string<C>& str);
```

This function attempts to determine the actual display width of a string, when
presented in a nominally "fixed width" font (which will normally include
double width characters for East Asian scripts). It does not attempt to
implement the full Unicode extended grapheme cluster algorithm, but uses a
simplified version that isn't completely reliable but works for the
characters encountered in most Unicode text:

* if `General_Category = Cc` (Control), `Cf` (Format), or `Mn` (Nonspacing_Mark)
    * and `White_Space` and `Pattern_White_Space` are both false
        * width = 0
* else if `East_Asian_Width = F` (Fullwidth) or `W` (Wide)
    * width = 2
* else
    * width = 1

This will throw `std::invalid_argument` if invalid encoding is encountered.
