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
constexpr char32_t first_surrogate        = 0xd800;
constexpr char32_t last_surrogate         = 0xdfff;
constexpr char32_t byte_order_mark        = 0xfeff;
constexpr char32_t replacement_character  = 0xfffd;
constexpr char32_t max_unicode            = 0x10ffff;
constexpr char32_t not_unicode            = ~ char32_t(0);
```

Some code points defined for convenience. The `not_unicode` value is returned
by some of the functions below to indicate failure.

## Concepts

```c++
template <typename C> concept CharacterType;
```

This is satisfied only for `char`, `char16_t`, `char32_t`, and `wchar_t`
(`char8_t` is not supported yet).

## Exceptions

```c++
class UnicodeError:
public std::runtime_error {
    explicit UnicodeError(char32_t c);
    UnicodeError(std::string_view str, size_t pos);
    UnicodeError(std::u16string_view str, size_t pos);
    UnicodeError(std::u32string_view str, size_t pos);
    UnicodeError(std::wstring_view str, size_t pos);
    size_t pos() const noexcept { return pos_; }
};
```

This is thrown by some functions to report invalid UTF encoding in a string.

## Character encoding functions

```c++
constexpr bool is_unicode(char32_t c) noexcept;
```

True if the code point is a valid Unicode scalar value, in the range
`0-0xd7ff` or `0xe000-0x10ffff`.

```c++
template <CharacterType C>
    char32_t decode_char(const std::basic_string<C>& str,
        size_t& pos) noexcept;
template <CharacterType C>
    char32_t decode_char(std::basic_string_view<C> str,
        size_t& pos) noexcept;
```

Read one encoded Unicode character from the input string, starting at the
given position. On success, return the decoded character and advance the
position to the next character. On failure (when either the string contains
invalid Unicode, or the starting position is already at or past the end of the
string), return `not_unicode` and advance the position to the next point where
a character could start, up to the maximum number of code points possible for
an encoded character (or to the end of the string, if that comes first).

The UTF-32 version does no actual decoding, but checks that the next character
is valid. This also applies to all of the other UTF-32 based functions below.

```c++
template <CharacterType C>
    char32_t check_decode_char(const std::basic_string<C>& str,
        size_t& pos);
template <CharacterType C>
    char32_t check_decode_char(std::basic_string_view<C> str,
        size_t& pos);
```

These perform the same operation as `decode_char()` above, but throw an
exception (without advancing the position) instead of returning an error code
on failure. They will throw `std::out_of_range` if the initial position is at
or past the end of the string, or `UnicodeError` if invalid UTF encoding is
encountered.

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
`UnicodeError` instead of returning false on failure.

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
constexpr bool is_noncharacter(char32_t c) noexcept;
bool is_pattern_syntax(char32_t c);
bool is_xid_continue(char32_t c);
bool is_xid_start(char32_t c);
```

Selected Unicode character properties. The `is_noncharacter()` predicate is
true only for the permanently reserved noncharacter code points. These will
all return false if the argument is not a valid Unicode scalar value.

## Iterators

```c++
class UtfIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = char32_t;
    UtfIterator() noexcept;
    explicit UtfIterator(std::string_view utf8, size_t pos,
        bool checked = false);
    const char32_t& operator*() const noexcept;
    std::string_view view() const noexcept;
};
using UtfRange = Irange<UtfIterator>;
UtfIterator utf_begin(std::string_view utf8, bool checked = false) noexcept;
UtfIterator utf_end(std::string_view utf8, bool checked = false) noexcept;
UtfRange utf_range(std::string_view utf8, bool checked = false) noexcept;
```

An iterator over the encoded Unicode code points in a UTF-8 string. Standard
iterator members are not listed individually. Behaviour is undefined if
`pos>utf8.size()`.

The `checked` flag determines how invalid UTF-8 is handled. The default
behaviour is to substitute the replacement character (`U+FFFD`), with the
length of the invalid code unit sequence determined in the same way as for
`decode_char()` (above). If `checked` is true, the constructor or increment
operator will throw `UnicodeError` (these can be treated as `noexcept` if
`checked` is false). The equality test does not read the `checked` flag.

The `view()` method returns the substring that contains the current encoded
character.

```c++
class GraphemeIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string_view;
    GraphemeIterator() noexcept;
    explicit GraphemeIterator(std::string_view utf8, size_t pos,
        bool checked = false);
    const std::string_view& operator*() const noexcept;
};
using GraphemeRange = Irange<GraphemeIterator>;
GraphemeIterator graphemes_begin(std::string_view utf8, bool checked = false);
GraphemeIterator graphemes_end(std::string_view utf8, bool checked = false);
GraphemeRange graphemes(std::string_view utf8, bool checked = false);
```

An iterator over the extended grapheme clusters in a UTF-8 string. Standard
iterator members are not listed individually. The `checked` flag works as for
`UtfIterator`. Behaviour is undefined if `pos>utf8.size()`.

## String encoding functions

```c++
template <CharacterType C>
    std::u32string decode_string(const std::basic_string<C>& str);
template <CharacterType C>
    std::u32string decode_string(std::basic_string_view<C> str);
```

Convert an encoded Unicode string to a UTF-32 string. This will throw
`UnicodeError` if invalid UTF encoding is encountered.

```c++
template <CharacterType C>
    void encode_string(std::u32string_view src,
        std::basic_string<C>& dst);
std::string to_utf8(std::u32string_view utf32);
std::u16string to_utf16(std::u32string_view utf32);
std::u32string to_utf32(std::u32string_view utf32);
std::wstring to_wstring(std::u32string_view utf32);
```

These convert a UTF-32 string into an encoded string. They will throw
`UnicodeError` if an invalid Unicode scalar value is encountered.

```c++
template <CharacterType C>
    bool is_valid_utf(const std::basic_string<C>& str,
        bool hard = false);
template <CharacterType C>
    bool is_valid_utf(std::basic_string_view<C> str,
        bool hard = false);
```

True if the string contains valid UTF (or is empty). If the `hard` flag is
set, this will throw `UnicodeError` instead of returning false.

```c++
enum class Usize {
    units,      // Code units
    scalars,    // Scalar values (code points)
    graphemes,  // Grapheme clusters (including whitespace)
    columns     // Virtual columns (see below)
};
size_t utf_size(std::string_view str, Usize mode);
```

Returns various measures of the size of a UTF-8 string.

In `columns` mode, this returns the estimated width of the string when
presented in a nominally "fixed width" font, assuming the font uses double
width characters for East Asian scripts and emoji. The `graphemes` and
`columns` modes do not attempt to implement the full Unicode extended grapheme
cluster algorithm, but use a simplified algorithm that will usually give the
correct result for most text:

* _if General Category is Cc, Cf, Mn, or Sk_
    * _character width is 0_
* _else if East Asian Width is Fullwidth or Wide_
    * _character width is 2_
* _else_
    * _character width is 1_

(There is also some special casing for the regional flag symbols.)

In practise there would be little point in implementing a more sophisticated
algorithm since very few fonts and display engines reliably follow Unicode's
grapheme cluster and East Asian Width models consistently.

This function will not return a meaningful result if the string contains
layout control characters such as tabs and line feeds. In modes other than
`units` this will throw `UnicodeError` if invalid UTF-8 is encountered.

## Normalization functions

```c++
std::string to_nfc(std::string_view str);
std::u32string to_nfc(std::u32string_view str);
std::string to_nfd(std::string_view str);
std::u32string to_nfd(std::u32string_view str);
```

Convert UTF-8 or UTF-32 strings to NFC and NFD forms. These will throw
`UnicodeError` if invalid UTF encoding is encountered.
