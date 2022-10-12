# Colour

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/colour.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class ColourLayout: int {
    forward,        // e.g. RGB
    forward_alpha,  // e.g. RGBA
    alpha_forward,  // e.g. ARGB
    reverse,        // e.g. BGR
    reverse_alpha,  // e.g. BGRA
    alpha_reverse   // e.g. ABGR
};
```

Indicates the internal layout of a colour object.

```c++
enum class Pma: int {
    none = 0,
    first,
    second,
    result,
    all = first | second | result
};
```

Bitmask flags used in the `alpha_blend()` function to indicate which of the
colours involved have premultiplied alpha.

## Colour class

```c++
template <ArithmeticType VT = float, ColourSpace CS = LinearRGB,
    ColourLayout CL = ColourLayout::forward_alpha> class Colour;
```

This class represents a colour, using a vector of some value type to represent
the channels of a colour space, optionally with the addition of an alpha
channel.

If `VT` is an integer, and the colour space is a unit-cube or polar type, then
the channels are scaled up to the maximum value that can fit in the integer
type. For example, if `VT` is `uint8_t`, then a channel value of 255
corresponds to a value of 1 in the original abstract colour space. If `VT` is
floating point, or the colour space does not restrict its values to the unit
cube, then no scaling is done; integer channel values are assumed to be
integer approximations to the abstract colour space values.

```c++
template <typename C> concept ColourType;
```

True if `C` is an instantiation of the `Colour` template.

```c++
template <typename C> concept LinearColourType;
```

True if `C` is a colour type with a linear colour space.

### Colour type aliases

```c++
using Rgb8 = Colour<uint8_t, LinearRGB, ColourLayout::forward>;
using Rgb16 = Colour<uint16_t, LinearRGB, ColourLayout::forward>;
using Rgbf = Colour<float, LinearRGB, ColourLayout::forward>;
using Rgbd = Colour<double, LinearRGB, ColourLayout::forward>;
using sRgb8 = Colour<uint8_t, sRGB, ColourLayout::forward>;
using sRgb16 = Colour<uint16_t, sRGB, ColourLayout::forward>;
using sRgbf = Colour<float, sRGB, ColourLayout::forward>;
using sRgbd = Colour<double, sRGB, ColourLayout::forward>;
using Rgba8 = Colour<uint8_t, LinearRGB, ColourLayout::forward_alpha>;
using Rgba16 = Colour<uint16_t, LinearRGB, ColourLayout::forward_alpha>;
using Rgbaf = Colour<float, LinearRGB, ColourLayout::forward_alpha>;
using Rgbad = Colour<double, LinearRGB, ColourLayout::forward_alpha>;
using sRgba8 = Colour<uint8_t, sRGB, ColourLayout::forward_alpha>;
using sRgba16 = Colour<uint16_t, sRGB, ColourLayout::forward_alpha>;
using sRgbaf = Colour<float, sRGB, ColourLayout::forward_alpha>;
using sRgbad = Colour<double, sRGB, ColourLayout::forward_alpha>;
```

Convenience aliases for some commonly used colour types.

### Member constants

```c++
static constexpr int Colour::alpha_index;
```

The index of the alpha channel, or -1 if the colour does not have an alpha
channel.

```c++
static constexpr bool Colour::can_premultiply;
```

True if the concept of premultiplied alpha is applicable to this colour type.
This is true if the colour space is a linear RGB unit cube, and the colour
has an alpha channel.

```c++
static constexpr int Colour::channels;
```

The number of channels in the colour, including the alpha channel, if
present.

```c++
static constexpr int Colour::colour_space_channels;
```

The number of channels in the colour, not including the alpha channel. This is
equal to `CS::count`. It will be equal to `channels` if there is no alpha
channel.

```c++
static constexpr bool Colour::has_alpha;
```

True if the colour has an alpha channel. Equivalent to `alpha_index==-1`.

```c++
static constexpr bool Colour::is_hdr;
```

True if the value type is floating point.

```c++
static constexpr bool Colour::is_linear;
```

True if the colour space is linear.

```c++
static constexpr ColourLayout Colour::layout = CL;
```

The colour layout parameter.

```c++
static constexpr VT Colour::scale;
```

This is the channel value that corresponds to a value of 1 in the abstract
colour space. If the colour space is a unit-cube or polar space, and the
value type is an integer, this is equal to the maximum value of the value
type. Otherwise, this is 1.

### Member types

```c++
using Colour::colour_space = CS;
using Colour::iterator = VT*;
using Colour::const_iterator = const VT*;
using Colour::value_type = VT;
using Colour::vector_type = Vector<VT, channels>;
using Colour::partial_vector_type = Vector<VT, colour_space_channels>;
```

Member types.

### Life cycle functions

```c++
constexpr Colour::Colour() noexcept;
```

The default constructor leaves the channel values uninitialized.

```c++
explicit constexpr Colour::Colour(VT x) noexcept;
```

Sets all non-alpha channels to the given value. If the colour has an alpha
channel, that channel is set equal to `scale` (i.e. fully opaque).

```c++
constexpr Colour::Colour(VT x, VT a) noexcept;
```

Sets the non-alpha channels to one value, and the alpha channel to another.
This constructor is only defined if an alpha channel is present.

```c++
explicit constexpr Colour::Colour(vector_type v) noexcept;
```

Constructor from a vector containing the colour channels. This initializes the
channels in the order implied by the colour layout parameter, which may not
be the same order as the colour space.

```c++
template <typename... Args> constexpr Colour::Colour(Args... args) noexcept;
```

Constructor from an explicit list of channel values (in colour layout order,
not colour space order). This is defined only for an argument count equal to
the number of channels; for colours with an alpha channel, it is also defined
for the number of non-alpha channels, in which case the alpha channel is set
to full opacity.

```c++
explicit Colour::Colour(const std::string& str);
```

Constructor from a string, which is expected to hold a hexadecimal RGB or RGBA
colour; this will be converted to the current colour representation. This is
only defined for RGB colour spaces. The string is always assumed to be in RGB
or RGBA order regardless of the colour type's internal layout order.

Leading and trailing punctuation or whitespace is ignored; apart from that,
this will throw `std::invalid_argument` if the argument contains anything
other than 6 or 8 hex digits. If an alpha channel is present in the colour
type but not in the string, it will be set to full opacity.

```c++
constexpr Colour::Colour(const Colour& c) noexcept;
constexpr Colour::Colour(Colour&& c) noexcept;
constexpr Colour::~Colour() noexcept;
constexpr Colour& Colour::operator=(const Colour& c) noexcept;
constexpr Colour& Colour::operator=(Colour&& c) noexcept;
```

Other life cycle functions.

### Channel access functions

```c++
constexpr VT& Colour::alpha() noexcept;
constexpr const VT& Colour::alpha() const noexcept;
```

Accessors for the alpha channel. If no alpha channel is present, the non-const
version is not defined, and the const version will always return `scale`.


```c++
constexpr VT& Colour::A() noexcept;
constexpr const VT& Colour::A() const noexcept;
constexpr VT& Colour::a() noexcept;
constexpr const VT& Colour::a() const noexcept;
constexpr VT& Colour::B() noexcept;
constexpr const VT& Colour::B() const noexcept;
constexpr VT& Colour::b() noexcept;
constexpr const VT& Colour::b() const noexcept;
// ...
constexpr VT& Colour::Z() noexcept;
constexpr const VT& Colour::Z() const noexcept;
constexpr VT& Colour::z() noexcept;
constexpr const VT& Colour::z() const noexcept;
```

Accessors for the colour space channels. These are only defined if the
corresponding letter appears in the colour space's list of channels.

```c++
VT& Colour::operator[](int i) noexcept;
const VT& Colour::operator[](int i) const noexcept;
```

Channel accessors by index. The channels are indexed by colour layout order,
not colour space order. Behaviour is undefined if the index is out of
bounds.

```c++
constexpr VT& Colour::cs(int i) noexcept;
constexpr const VT& Colour::cs(int i) const noexcept;
```

Channel accessors in colour space order, regardless of layout order. The alpha
channel, if present, is always last. Behaviour is undefined if the index is
out of bounds.

```c++
constexpr VT* Colour::begin() noexcept;
constexpr const VT* Colour::begin() const noexcept;
constexpr VT* Colour::end() noexcept;
constexpr const VT* Colour::end() const noexcept;
```

Iterators over the channel values (in colour layout order).

```c++
constexpr vector_type Colour::as_vector() const noexcept;
```

Returns the channels as a vector (in colour layout order).

```c++
constexpr partial_vector_type Colour::partial_vector() const noexcept;
```

Returns the non-alpha channels as a vector.

### General member functions

```c++
constexpr void Colour::clamp() noexcept;
constexpr Colour Colour::clamped() const noexcept;
```

Clamp the colour to the range implied by the colour space. These will leave
the non-alpha channels unchanged if the colour space is not unit-cube or
polar. If an alpha channel is present, this will always be clamped to the `
[0,scale]` range, regardless of colour space.

```c++
constexpr bool Colour::empty() const noexcept;
```

Always false.

```c++
size_t Colour::hash() const noexcept;
struct std::hash<Colour>
```

Hash function.

```c++
std::string Colour::hex() const;
```

Returns the colour as a hex string, containing 6 or 8 digits, depending on
whether the colour type has an alpha channel. The string is always in RGB or
RGBA order regardless of the colour type's internal order. The output will be
nonsense if the colour is out of gamut. This will only compile for RGB colour
spaces.

```c++
constexpr bool Colour::is_clamped() const noexcept;
```

True if all of the colour's channels are within the range implied by the
colour space. If the colour space is not unit-cube or polar, this only checks
the alpha channel.

```c++
constexpr Colour multiply_alpha() const noexcept;
constexpr Colour unmultiply_alpha() const noexcept;
```

Convert the colour from unmultiplied alpha to premultiplied alpha, or vice
versa. Defined only if the colour space is a linear RGB unit cube, and the
colour has an alpha channel.

```c++
constexpr size_t Colour::size() const noexcept;
```

Returns the number of channels.

```c++
std::string Colour::str(FormatSpec spec = {}) const;
friend std::ostream& operator<<(std::ostream& out, const Colour& c);
```

Formats the colour as a string (in colour layout order). The format is the
same as for the corresponding vector type.

### Standard colours

```c++
static Colour Colour::clear() noexcept;    // rgba = (0,0,0,0)
static Colour Colour::black() noexcept;    // rgba = (0,0,0,1)
static Colour Colour::white() noexcept;    // rgba = (1,1,1,1)
static Colour Colour::red() noexcept;      // rgba = (1,0,0,1)
static Colour Colour::yellow() noexcept;   // rgba = (1,1,0,1)
static Colour Colour::green() noexcept;    // rgba = (0,1,0,1)
static Colour Colour::cyan() noexcept;     // rgba = (0,1,1,1)
static Colour Colour::blue() noexcept;     // rgba = (0,0,1,1)
static Colour Colour::magenta() noexcept;  // rgba = (1,0,1,1)
```

Some conventional colours. These are only defined for RGB colour spaces. The
`clear()` function is only defined if the colour has an alpha channel.

### Arithmetic operators

```c++
constexpr Colour operator+(Colour c) noexcept;
constexpr Colour operator-(Colour c) noexcept;
constexpr Colour operator+(Colour a, Colour b) noexcept;
constexpr Colour operator-(Colour a, Colour b) noexcept;
constexpr Colour operator*(Colour a, VT b) noexcept;
constexpr Colour operator*(VT a, Colour b) noexcept;
constexpr Colour operator/(Colour a, VT b) noexcept;
constexpr Colour operator*(Colour a, vector_type b) noexcept;
constexpr Colour operator*(vector_type a, Colour b) noexcept;
constexpr Colour operator/(Colour a, vector_type b) noexcept;
constexpr Colour& operator+=(Colour& a, Colour b) noexcept;
constexpr Colour& operator-=(Colour& a, Colour b) noexcept;
constexpr Colour& operator*=(Colour& a, VT b) noexcept;
constexpr Colour& operator/=(Colour& a, VT b) noexcept;
constexpr Colour& operator*=(Colour& a, vector_type b) noexcept;
constexpr Colour& operator/=(Colour& a, vector_type b) noexcept;
```

Colour arithmetic operators. All of these perform component-wise operations.
These are only defined for linear RGB colour spaces. Division by zero is
undefined behaviour.

```c++
constexpr Colour alpha_blend(Colour a, Colour b,
    Pma flags = Pma::none) noexcept;
```

Performs alpha blending (`a` over `b`). The flags indicate which of the
arguments is alpha premultiplied, and whether the result should be alpha
premultiplied.

### Comparison operators

```c++
constexpr bool operator==(Colour a, Colour b) noexcept;
constexpr bool operator!=(Colour a, Colour b) noexcept;
```

Comparison operators.

### Colour functions

```c++
template <ColourType CT1, ColourType CT2>
    void convert_colour(CT1 in, CT2& out) noexcept;
```

This converts between any two colours. This will call `convert_colour_space()`
if the colour spaces are different, as well as converting between different
channel types and internal layouts. Conversions from floating point to integer
will round to the nearest integer. If the output contains an alpha channel but
the input does not, the alpha channel will be set to its fully opaque value.
Behaviour is undefined if the input colour contains out-of-gamut values that
are not representable in the output channel type.

```c++
template <ColourType CT> CT css_colour(const std::string& str);
```

This function looks up a string in the standard list of CSS colours, and
returns the resulting colour. The return type must be explicitly specified,
and must be an instantiation of the `Colour` template.

Capitalization, punctuation, and whitespace are ignored. If the name is not
found in the list, it will be interpreted as a hexadecimal colour if it fits
the format. If both interpretations fail, the function will throw
`std::invalid_argument`. This function does not attempt to interpret any of
the other colour formats described in the CSS standard.

When interpreting a hex colour, this function differs from the string-based
constructor in that this will always interpret a hex colour using the sRGB
colour space (as CSS requires) and then convert to the target colour, while
the constructor uses the target colour's native colour space.

```c++
template <ColourType CT, std::floating_point FP>
    constexpr CT lerp(const CT& c1, const CT& c2, FP x) noexcept;
```

Performs linear interpolation between colours. If the colour's channel type is
floating point, `FP` must be the same type. If the channel type is an integer
type, the results are rounded to the nearest integer (halves round toward
positive infinity). Behaviour is undefined if the correct result would be out
of range for the channel type.
