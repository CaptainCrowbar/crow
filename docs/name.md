# Name formatting

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/name.hpp"
namespace Crow;
```

## Formatted name class

```c++
class Name;
```

This holds a sequence of words that make up a name.

```c++
enum class Name::style {
    lower,        // "l "   = lower case with spaces
    kebab,        // "l-"   = lower-case-with-hyphens
    snake,        // "l_"   = lower_case_with_underscores
    sentence,     // "tl "  = Sentence case with spaces
    stroustrup,   // "tl_"  = Sentence_case_with_underscores
    camel,        // "lt"   = camelCaseWithNoDelimiters
    pascal,       // "lt"   = TitleCaseWithNoDelimiters
    title,        // "t "   = Title Case With Spaces
    title_kebab,  // "t-"   = Title-Case-With-Hyphens
    title_snake,  // "t_"   = Title_Case_With_Underscores
    upper,        // "u "   = UPPER CASE WITH SPACES
    upper_kebab,  // "u-"   = UPPER-CASE-WITH-HYPHENS
    upper_snake,  // "u_"   = UPPER_CASE_WITH_UNDERSCORES
};
using enum style;
```

Formatting style flags used in the `str()` function.

```c++
enum class Name::parse_flags {
    no_pf  = 0,
    apos   = 1,
};
using enum parse_flags;
```

Parsing flags used in the constructors.

```c++
enum class Name::format_flags {
    no_ff  = 0,
    stop   = 1,
};
using enum format_flags;
```

Formatting flags used in the `str()` function.

```c++
class Name::iterator;
```

A const random access iterator over the words parsed out of the original name.
These will retain the original casing.

```c++
Name::Name() noexcept;
```

The default constructor yields an empty name.

```c++
Name::Name(std::string_view s, parse_flags pf = no_pf);
Name::Name(const std::string& s, parse_flags pf = no_pf);
Name::Name(const char* s, parse_flags pf = no_pf);
```

Construct a `Name` object by parsing a string into words. Individual words may
be in upper, lower, or title case. Numbers are treated as separate words.
Non-alphanumeric characters are treated as delimiters and discarded.

If the `apos` flag is set, apostrophes will be considered part of a word.

```c++
Name::Name(const Name&);
Name::Name(Name&&) noexcept;
Name::~Name() noexcept;
Name& Name::operator=(const Name&);
Name& Name::operator=(Name&&) noexcept;
```

Other life cycle functions.

```c++
Name::iterator Name::begin() const noexcept { return words_.begin(); }
Name::iterator Name::end() const noexcept { return words_.end(); }
bool Name::empty() const noexcept { return words_.empty(); }
size_t Name::size() const noexcept { return words_.size(); }
```

Iterators and other queries on the broken-down name.

```c++
std::string Name::str(style s, format_flags ff = no_ff) const;
std::string Name::str(std::string_view format, format_flags ff = no_ff) const;
```

Format the name as a string. The formatting code can be one of the standard
styles, or a string representing a style in detail.

The format string must consist of:

* The letter `'l'`, `'t'`, or `'u'`, indicating the case to be used for the
  first word (lower, title, or upper).
* Optionally, a second letter to indicate the case of subsequent words. If
  this is absent, the same casing will be used for all words.
* Optionally, a single ASCII punctuation mark or space to be used as a
  delimiter between words. If this is absent, words will be directly
  concatenated with no delimiter.

If the `stop` flag is set, [stopwords](english.html) will be left in lower
case when title case is requested (except for the first word). The `stop`
flag has no effect if the output format does not include any titlecase
elements.

The second version of the `str()` function will throw `std::invalid_argument`
if the format string is invalid.
