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

## Character encoding functions

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

## Character property functions

```c++

enum class GC {
    Cc,  // Control
    Cf,  // Format
    Cn,  // Unassigned
    Co,  // Private use
    Cs,  // Surrogate
    Ll,  // Lowercase letter
    Lm,  // Modifier letter
    Lo,  // Other letter
    Lt,  // Titlecase letter
    Lu,  // Uppercase letter
    Mc,  // Spacing mark
    Me,  // Enclosing mark
    Mn,  // Nonspacing mark
    Nd,  // Decimal number
    Nl,  // Letter number
    No,  // Other number
    Pc,  // Connector punctuation
    Pd,  // Dash punctuation
    Pe,  // Close punctuation
    Pf,  // Final punctuation
    Pi,  // Initial punctuation
    Po,  // Other punctuation
    Ps,  // Open punctuation
    Sc,  // Currency symbol
    Sk,  // Modifier symbol
    Sm,  // Math symbol
    So,  // Other symbol
    Zl,  // Line separator
    Zp,  // Paragraph separator
    Zs   // Space separator
};
GC general_category(char32_t c);
```

Unicode general category property.

```c++
bool is_pattern_syntax(char32_t c);
bool is_xid_continue(char32_t c);
bool is_xid_start(char32_t c);
```

Selected Unicode character properties. These will all return false if the
argument is not a valid Unicode scalar value.

## String encoding functions

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
double width characters for East Asian scripts and emoji). It does not attempt
to implement the full Unicode extended grapheme cluster algorithm, but uses a
simplified algorithm:

* _if General Category is Cc, Cf, Mn, or Sk_
    * _width is 0_
* _else if East Asian Width is Fullwidth or Wide_
    * _width is 2_
* _else_
    * _width is 1_

This will not return a meaningful result if the string contains layout control
characters such as tabs or line feeds. It will throw `std::invalid_argument`
if invalid encoding is encountered.

## Normalization functions

```c++
std::string to_nfc(std::string str);
std::u32string to_nfc(std::u32string str);
std::string to_nfd(std::string str);
std::u32string to_nfd(std::u32string str);
```

Convert UTF-8 or UTF-32 strings to NFC and NFD forms. These will throw
`std::invalid_argument` if invalid Unicode is encountered.
