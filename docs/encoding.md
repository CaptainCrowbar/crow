# Encoding Conversion

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/encoding.hpp"
namespace Crow;
```

This module supplies various ways of encoding a block of data as an ASCII string.

## Contents

* TOC
{:toc}

## Exceptions

```c++
class EncodingError: public std::runtime_error;
```

Reports that invalid encoding was encountered while decoding.

## Encoding base class

```c++
class Encoding;
```

Abstract base class for encoding classes. Virtual functions that are
overridden in derived classes are not repeated in the derived class's
documentation unless there is significant new information.

```c++
protected explicit Encoding::Encoding(size_t line) noexcept;
virtual Encoding::~Encoding() noexcept;
```

Life cycle functions. The output of `encode()` will be broken into lines if
the line length is not zero or `npos`. This setting has no effect on
`decode()`.

```c++
virtual std::string Encoding::name() const = 0;
```

REturns the name of the class.

```c++
std::string Encoding::encode(const void* in, size_t len) const;
std::string Encoding::encode(std::string_view in) const;
```

Encode a block of data as a string. These call `do_encode()`.

```c++
size_t Encoding::decode(std::string_view in, void* out, size_t maxlen) const;
std::string Encoding::decode(std::string_view in) const;
```

Decode a string back to the original block of data. These call `do_decode()`.

The first version writes the result through the `out` pointer, returning the
number of bytes written, or `npos` if the output length would have exceeded
`maxlen`. The second version returns the block as a string.

These will throw `EncodingError` if invalid encoding is encountered. If an
exception is thrown, or the first function returns `npos`, the contents of
the output buffer are unspecified.

```c++
size_t Encoding::line() const noexcept;
```

The number of characters in an output line.

```c++
protected virtual bool
    Encoding::can_break(std::string_view encoded, size_t pos) const noexcept;
```

This tells the `encode()` functions whether or not a line break is allowed at
a given position. The default implementation always returns true.

```c++
protected virtual std::string
    Encoding::do_encode(const void* in, size_t len) const = 0;
protected virtual size_t
    Encoding::do_decode(std::string_view in, void* out,
        size_t maxlen) const = 0;
```

These must be overridden to implement encoding and decoding. Their semantics
are the same as the versions of `encode()` and `decode()` with the same
signatures, except that `do_encode()` does not have to insert line breaks;
`encode()` will take care of that (but `do_decode()` does need to allow for
their presence).

## Hexadecimal encoding class

```c++
class Hexcode: public Encoding {
public:
    Hexcode() noexcept;
    explicit Hexcode(size_t line) noexcept;
    std::string name() const override { return "Hexcode"; }
};
```

Simple hex encoding, converting each byte to two hex digits.

## Base 64 encoding class

```c++
class Base64: public Encoding {
public:
    Base64() noexcept;
    explicit Base64(size_t line, const std::string& last2 = "+/",
        bool padding = true);
    std::string name() const override { return "Base64"; }
};
```

Base 64 encoding.

Besides the line length, the constructor can also set the two characters used
for digits 62 and 63 (defaulting to `"+/"`), and whether or not to pad the
output with `"="` to a multiple of 4 bytes (on by default). The constructor
will throw `std::invalid_argument` if `last2.size()!=2`, if either character
is not an ASCII punctuation mark, or if the two characters are the same.

The decoder will use the `last2` setting, but it can handle input with or
without line breaks or padding regardless of their settings.
