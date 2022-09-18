# Fonts

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/font.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Constants

```c++
enum class FontStyle: int {
    regular = 0,
    bold,
    italic,
    fallback,
};
```

Style flags used in the `FontMap::find()` function.

## Font class

```c++
class Font;
```

A class that represents a single font. This may be loaded from an OTF or TTF
file, or may be one of several fonts loaded from a TTC file.

```c++
Font::Font();
```

The default constructor creates a null font with no name or glyphs.

```c++
explicit Font::Font(const IO::Path& file, int index = 0);
```

Load a font from a file (OTF, TTF, or TTC). The index indicates which font to
load from a TTC file. This will create a null font if the index is out of
range for a TTC file, or if a non-zero index is used with an OTF or TTF file.

```c++
virtual Font::~Font() noexcept;
Font::Font(const Font& f);
Font::Font(Font&& f) noexcept;
Font& Font::operator=(const Font& f);
Font& Font::operator=(Font&& f) noexcept;
```

Other life cycle functions.

```c++
explicit Font::operator bool() const noexcept;
```

True if the font is not null (i.e. a font was successfully loaded from a
file).

```c++
std::string Font::family() const;
std::string Font::subfamily() const;
std::string Font::name() const;
```

These return the family and subfamily names, and the full name of the font.

```c++
bool Font::has_glyph(char32_t c) const noexcept;
bool Font::has_glyphs(char32_t first, char32_t last) const noexcept;
template <typename Range> bool Font::has_glyphs(const Range& range) const;
```

These report whether or not a glyph, or a range of glyphs, is available in the
font. A glyph range can be supplied either as the first and last glyph in a
range (inclusive), or as an explicit list. The `has_glyphs()` functions will
return true only if every glyph in the range is present. All of these
functions will return false if any of the characters being queried is not a
valid Unicode scalar value.

```c++
static std::vector<Font> Font::load(const IO::Path& file);
```

Loads all of the available fonts from a TTC file. For OTF and TTF files this
will return a vector with zero or one font.

## Scaled font class

```c++
class ScaledFont: public Font;
```

This class represents a font scaled to a particular size.

```c++
ScaledFont::ScaledFont();
```

The default constructor creates a null font with no name, glyphs, or metrics.

```c++
ScaledFont::ScaledFont(const Font& font, int scale) noexcept;
ScaledFont::ScaledFont(const Font& font, Point scale) noexcept;
```

Construct a `ScaledFont` from a `Font` and a scale factor. The scale factor
indicates the scaled font's em size in pixels. This can be a single value, or
separate values for the X and Y scales. Behaviour is undefined if a scale
factor is less than or equal to zero.

```c++
virtual ScaledFont::~ScaledFont() noexcept;
ScaledFont::ScaledFont(const ScaledFont& sf);
ScaledFont::ScaledFont(ScaledFont&& sf) noexcept;
ScaledFont& ScaledFont::operator=(const ScaledFont& sf);
ScaledFont& ScaledFont::operator=(ScaledFont&& sf) noexcept;
```

Other life cycle functions.

```c++
Point ScaledFont::scale() const noexcept;
```

Returns the font scale (the em size in pixels).

```c++
int ScaledFont::ascent() const noexcept;
int ScaledFont::descent() const noexcept;
int ScaledFont::line_gap() const noexcept;
int ScaledFont::line_offset() const noexcept;
```

These return font metrics. The ascent is the maximum distance from the
baseline to the top of a glyph. The descent is the maximum distance from the
baseline to the bottom of a glyph, and is normally negative. The line gap is
the extra distance between lines, beyond what is implied by the ascent and
descent. The line offset is the distance between the baselines of successive
lines, and is equal to `ascent()-descent()+line_gap()`. All measures are in
pixels.

```c++
template <typename C, int F>
    void ScaledFont::render(Image<C, F>& image, Point& offset,
        const std::string& text, int line_shift = 0,
        C text_colour = C::black(), C background = [see below]) const;
```

This function renders text to a new image. The supplied `image` object will be
reset to the minimum size required to contain the rendered text. The image
must have a linear colour space.

Multiple lines of text are supported. The `offset` vector will be set to the
position of the image's top left corner relative to the initial reference
point, which is the beginning of the baseline of the first line of text.

If a `line_shift` is supplied, it will be added to the normal vertical spacing
between lines (this can be negative if you want lines to be closer together
than the default).

The text colour defaults to black; the background colour defaults to
transparent if an alpha channel is present, otherwise to white. Note that
non-RGB colours do not have `black()` and `white()` static methods; if you
use a non-RGB colour, you must supply both colours explicitly.

This will throw `std::invalid_argument` if the font is null or the text
contains invalid UTF-8. It will run without error if nothing was rendered
because the text is empty or contains only whitespace characters.

```c++
template <typename C, int F>
    void ScaledFont::render_to(Image<C, F>& image, Point ref_point,
        const std::string& text, int line_shift = 0,
        C text_colour = C::black()) const;
```

This function renders text to an existing image. The image must have a linear
colour space.

Multiple lines of text are supported. The supplied `ref_point` is used as the
beginning of the baseline of the first line of text (it should be at least
`ascent()` pixels from the top of the image to avoid clipping). Any part of
the text that falls outside the bounds of the image will be clipped.

If a `line_shift` is supplied, it will be added to the normal vertical spacing
between lines (this can be negative if you want lines to be closer together
than the default).

The text colour defaults to black. If an alpha channel is present, this will
be alpha blended with the existing pixels of the image; otherwise, simple
linear antialiasing is used.

This will throw `std::invalid_argument` if the font is null or the text
contains invalid UTF-8. It will run without error if nothing was rendered
because the text is empty or contains only whitespace characters, or the
rendered glyphs fall entirely outside the image.

```c++
Core::Box_i2 ScaledFont::text_box(const std::string& text,
    int line_shift = 0) const;
```

Returns the bounding box of the supplied text, with the origin at the
beginning of the baseline of the first line of text.

If a `line_shift` is supplied, it will be added to the normal vertical spacing
between lines (this can be negative if you want lines to be closer together
than the default).

This will throw `std::invalid_argument` if the font is null or the text
contains invalid UTF-8. It will return an empty box if the text is empty or
contains only whitespace characters.

```c++
size_t ScaledFont::text_fit(const std::string& text, size_t max_pixels) const;
```

Indicates how much of the given single line of text will fit into a space of
`max_pixels` width. It will return `npos` if the entire text will fit.
Otherwise, it returns the length of the longest substring that will fit into
the space, in bytes (this will always point to a UTF-8 character boundary).

This will throw `std::invalid_argument` if the font is null, or the text
contains invalid UTF-8 or any line feed characters.

```c++
size_t ScaledFont::text_wrap(const std::string& text_in,
    std::string& text_out, size_t max_pixels) const;
```

Wrap text at word boundaries (determined by ASCII whitespace) to fit into the
given pixel width. Leading and trailing line feeds are stripped from the
input text, but internal line breaks will be retained. The return value is
the number of lines in the wrapped text.

This will throw `std::invalid_argument` if the font is null or the text
contains invalid UTF-8. Behaviour is undefined if `text_in` and `text_out`
are the same string.

## Font map class

```c++
class FontMap;
```

A `FontMap` contains information about a set of fonts, typically all the fonts
in a given set of directories. Fonts are organised by family and subfamily
(the subfamily usually represents variations such as bold or italic).

All queries based on family or subfamily names are case insensitive for ASCII
characters.

```c++
FontMap::FontMap();
FontMap::~FontMap() noexcept;
FontMap::FontMap(const FontMap& fm);
FontMap::FontMap(FontMap&& fm) noexcept;
FontMap& FontMap::operator=(const FontMap& fm);
FontMap& FontMap::operator=(FontMap&& fm) noexcept;
```

Life cycle functions.

```c++
void FontMap::clear() noexcept;
```

Clears all fonts from the map.

```c++
bool FontMap::contains(const std::string& family) const noexcept;
bool FontMap::contains(const std::string& family,
    const std::string& subfamily) const noexcept;
```

True if the `FontMap` contains any fonts matching the given names.

```c++
bool FontMap::empty() const noexcept;
```

True if the `FontMap` contains no fonts (equivalent to `size()==0`).

```c++
std::vector<std::string> FontMap::families() const;
```

Returns a list of all font family names.

```c++
std::vector<std::string> FontMap::subfamilies(const std::string& family) const;
```

Returns a list of all subfamily names for the given family. This will return
an empty list if `contains(family)` is false.

```c++
Font FontMap::find(const std::vector<std::string>& families,
    FontStyle style = FontStyle::regular) const;
```

Loads the first matching font in the `FontMap`. This will first search the
family names, in the order given, for a font that matches the given style. If
this is not found, it may run through the family names again and return the
first font of any kind it finds; by default this fallback search is done only
if the style requested is regular. If the `FontStyle::fallback` option is
combined with one or both of the other style options, a fallback will be
accepted for those too.

```c++
Font FontMap::load(const std::string& family,
    const std::string& subfamily) const;
```

Loads the font with the given family and subfamily name. This will return a
null font if no matching font is present in the `FontMap`.

```c++
void FontMap::search(const std::string& dir,
    IO::Path::flag flags = IO::Path::flag::none);
```

Searches the given directory for all readable font files. This will do nothing
if `dir` does not exist, or exists but is not a directory. Searches are
cumulative; previously loaded fonts are not cleared before a search.

The `flags` argument can be `IO::Path::recurse` to search subdirectories
recursively.

```c++
void FontMap::search_system();
```

Searches the standard font directories for the operating system. Note that
this can take several seconds if you have a lot of fonts.

* **Apple**
    * `/System/Library/Fonts`
    * `/Library/Fonts`
    * `~/Library/Fonts`
* **Linux** (and generic Unix)
    * `/usr/X11R6/lib/X11/fonts`
    * `/usr/share/fonts`
    * `/usr/local/share/fonts`
    * `~/.fonts`
    * `~/.local/share/fonts`
* **Windows**
    * `%WINDIR%\Fonts`

```c++
size_t FontMap::size() const noexcept;
```

Returns the number of font families in the `FontMap`.
