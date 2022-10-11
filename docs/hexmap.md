# Hex Mapping

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/hexmap.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Hexmap class

```c++
class Hexmap;
```

This class holds a simple hexmap, and draws it using ASCII art. Each non-empty
hex has a text label and a colour associated with it. Setting the text to an
empty string is the same as deleting the hex. Colours are expressed in the
format used by[`Xterm`](../rs-format/terminal.html).

A map is treated as a series of concentric rings of hexes around a central
hex. Hexes are identified by ring and step numbers. The centre hex in a map
is ring 0, step 0. Ring numbers increment by 1 for each step outwards; the
number of hexes in a ring (other than ring 0) is 6 times the ring number.
Step numbers start at 0 at the northernmost hex of a ring, and increment
clockwise.

Example:

               ____
          ____/2,0 \____
     ____/2,11\____/2,1 \____
    /2,10\____/1,0 \____/2,2 \
    \____/1,5 \____/1,1 \____/
    /2,9 \____/0,0 \____/2,3 \
    \____/1,4 \____/1,2 \____/
    /2,8 \____/1,3 \____/2,4 \
    \____/2,7 \____/2,5 \____/
         \____/2,6 \____/
              \____/

### Member types

```c++
enum class Hexmap::side: int {
    north,
    northeast,
    southeast,
    south,
    southwest,
    northwest,
};
```

Used to identify one side of a hex.

```c++
enum class Hexmap::xt: int {
    implicit = -1,
    no,
    yes,
};
```

Used to determine whether Xterm colours are used in formatting a hex map.
Implicit colouring is the default; this means that colours will only be used
if the output is going directly to standard output. This can be overridden by
setting the Xterm flag to `yes` or `no` to force colour on or off regardless
of where the output is going.

```c++
using Hexmap::hex = Int2;
using Hexmap::hexlist = std::vector<hex>;
using Hexmap::xcolour = Int3;
```

The `hex` types is a pair of integers used to identify a specific hex by its
ring and step numbers. The `xcolour` type is an Xterm compatible RGB colour,
with each channel ranging from 0 to 5(values outside this range will be
clamped to the nearest end of the range).

### Constants

```c++
static constexpr xcolour Hexmap::black = {0,0,0};
static constexpr xcolour Hexmap::white = {5,5,5};
```

Black and white colours.

### Life cycle functions

```c++
Hexmap::Hexmap();
```

The default constructor creates an empty hexmap.

```c++
Hexmap::Hexmap(std::initializer_list<...> list);
```

Constructor from an initializer list. The list element type is private; it can
be constructed from the same arguments as the `set()` function. The
initializer list constructor is equivalent to a series of calls to `set()`.

```c++
Hexmap::Hexmap(const Hexmap& hm);
Hexmap::Hexmap(Hexmap&& hm) noexcept;
Hexmap::~Hexmap() noexcept;
Hexmap& Hexmap::operator=(const Hexmap& hm);
Hexmap& Hexmap::operator=(Hexmap&& hm) noexcept;
```

Other life cycle functions.

### Hexmap geometry queries

```c++
hexlist Hexmap::all() const;
```

Lists all set hexes (hexes with at least one attribute), in lexical order.

```c++
hexlist Hexmap::border_in() const;
```

Lists all set hexes touching the inside of the map's current border, in
lexical order.

```c++
hexlist Hexmap::border_out() const;
```

Lists all unset hexes touching the outside of the map's current border, in
lexical order.

```c++
bool Hexmap::contains(hex h) const noexcept;
```

True if the hex is set.

```c++
bool Hexmap::empty() const noexcept;
```

True if there are no set hexes. Equivalent to `size()==0`.

```c++
hexlist Hexmap::neighbours_set(hex h) const;
hexlist Hexmap::neighbours_unset(hex h) const;
std::pair<hexlist, hexlist> Hexmap::neighbours_set_unset(hex h) const;
```

List all set or unset neighbours of a specific hex, in clockwise order.

```c++
hexlist Hexmap::reachable(hex h) const;
```

List all set hexes reachable from `h` (including `h` itself if set), in
lexical order.

```c++
int Hexmap::rings() const;
```

The maximum radius of the set hexes in the map. Equal to the ring number of
the outermost set hex, plus 1.

```c++
int Hexmap::size() const noexcept;
```

The number of set hexes.

### Hexmap geometry modifiers

```c++
void Hexmap::clear() noexcept;
```

Clears all set hexes. This clears the actual map to its empty state, but the
`Hexmap` object is not restored to a default constructed state because the
formatting parameters are not reset.

```c++
void Hexmap::erase(hex h) noexcept;
```

Erases a specific hex. Equivalent to `set_label(h,"")` or `set(h,"")`.

### Individual hex properties

```c++
xcolour Hexmap::colour(hex h) const noexcept;
void Hexmap::clear_colour(hex h) noexcept;
bool Hexmap::set_colour(hex h, xcolour c) noexcept;
```

Query or set the colour of a specific hex. Unset hexes (ones with no label) do
not have a colour; querying the colour of an unset hex will return black, and
setting its colour will do nothing. The `set_colour()` function returns true
if the colour was successfully set, false if it failed because the hex is
unset.

```c++
std::string Hexmap::label(hex h) const;
void Hexmap::set_label(hex h, const std::string& label);
```

Query or set the label of a hex. Setting the label to an empty string is
equivalent to erasing the hex. The `set_label()` function differs from `set()`
in that it does not change the colour of the hex (unless the label is empty).

```c++
void Hexmap::set(hex h, const std::string& label);
void Hexmap::set(hex h, const std::string& label, xcolour c);
```

Set a hex's label and colour; the colour defaults to white.

### Formatting functions

```c++
int Hexmap::radius() const noexcept;
void Hexmap::set_radius(int r = 1) noexcept;
```

Query or set the minimum map radius. The map will always be drawn with at
least this many rings (including ring 0), even if some of the outer rings are
empty. By default, if no radius is set, the map will be drawn only to as many
rings as are necessary to show all non-empty hexes (unless the map is
completely empty).

```c++
int Hexmap::width() const noexcept;
void Hexmap::set_width(int w = 2) noexcept;
```

Query or set the minimum width of a hex, in character units (not counting the
sloped edges). The actual hex width may be greater than this if a larger
width is necessary to show all of the labels.

```c++
xt Hexmap::xterm() const noexcept;
void Hexmap::set_xterm(bool b) noexcept;
void Hexmap::set_xterm(xt x) noexcept;
```

Query or set the flag that determines whether Xterm colours will be used.

```c++
std::string Hexmap::str() const;
std::ostream& operator<<(std::ostream& out, const Hexmap& hm);
```

Format the hexmap as plain text.

### Static geometry functions

```c++
static int Hexmap::area(int radius) noexcept;
```

The number of hexes out to this radius (this is 1 greater than the maximum
ring number).

```c++
static int Hexmap::distance(hex h1, hex h2) noexcept;
```

Calculates the distance (number of steps) between two hexes.

```c++
static bool Hexmap::is_adjacent(hex h1, hex h2) noexcept;
```

True if the two hexes are adjacent (or the same).

```c++
static bool Hexmap::is_valid(hex h) noexcept;
```

True if the coordinates are valid.

```c++
static hex Hexmap::neighbour(hex h, side s) noexcept;
```

Returns the neighbouring hex in the given direction.

```c++
static hexlist Hexmap::neighbours(hex h);
```

Lists all 6 neighbours of a hex, in clockwise order.

```c++
static int Hexmap::perimeter(int radius) noexcept;
```

Returns the number of hexes on the circumference of a map with this radius.
Returns 0 if `radius<1`.

```c++
static hex Hexmap::principal(hex h) noexcept;
```

Reduces hex coordinates to their principal values.

```c++
static int Hexmap::ring_size(int r) noexcept;
```

Returns the number of hexes on this ring number (equal to `perimeter(r+1)`).
