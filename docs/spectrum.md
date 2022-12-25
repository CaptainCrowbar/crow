# Stellar Spectral Class

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/spectrum.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class LC: uint8_t {
    none,
    Ia,   // Bright supergiants
    Ib,   // Supergiants
    II,   // Bright giants
    III,  // Giants
    IV,   // Subgiants
    V,    // Main sequence
    VI,   // Subdwarfs
    VII,  // White dwarfs
    other
};
```

Constants representing luminosity classes.

```c++
enum class Sp: uint8_t {
    none,
    O, B, A, F, G, K, M,         // Main sequence and giants (HR diagram types)
    L, T, Y,                     // Brown dwarfs
    DA, DB, DC, DO, DQ, DX, DZ,  // White dwarfs
    WC, WN, WO,                  // Wolf-Rayet stars
    CH, CHd, CJ, CN, CR, S,      // Carbon stars
    NS, BH                       // Stellar remnants
};
```

Constants representing spectral classes.

## Spectrum class

```c++
class Spectrum;
```

This represents a spectral class, defined as a combination of a luminosity
class, a spectral class, and a subclass.

This class has several functions that return physical properties of a typical
star of that type. All types show considerable variation in these parameters;
the values supplied here are only intended to be approximations. Especially
for non-main-sequence types, the figure given is usually the midpoint of a
wide range.

```c++
constexpr Spectrum::Spectrum() noexcept;
```

The default constructor sets the luminosity class and spectral class to
`none`, and the subclass to 0, representing an unknown spectral class.

```c++
explicit Spectrum::Spectrum(Sp cls, int sub = 0, LC lc = LC::none);
```

Constructor from a spectral class, subclass, and luminosity class. If no
luminosity class is supplied, the default LC for the given spectral class
will be used. Subclasses greater than 9 are allowed (up to 99), but
(apart from white dwarfs) are treated as equivalent to 9. This will throw
`std::invalid_argument` if the combination is invalid.

| Class  | Subclass  | LC     | Default LC  |
| -----  | --------  | --     | ----------  |
| O-M    | 0-99      | Ia-VI  | V           |
| L-Y    | 0-99      | other  | other       |
| DA-DZ  | 0-99      | VII    | VII         |
| WC-S   | 0-99      | other  | other       |
| NS-BH  | 0         | other  | other       |

```c++
explicit Spectrum::Spectrum(std::string_view str);
```

Parses a spectral class in string format. An empty string will yield a default
constructed object. Additional elements (e.g. letters after the main spectral
class) will be accepted but not saved. Non-standard white dwarf types are
recorded as DA. Neutron stars are abbreviated NS or PSR, black holes as BH.
This will throw `std::invalid_argument` if the string is not recognised as a
valid spectral type.

```c++
Spectrum::Spectrum(const Spectrum& sp) noexcept;
Spectrum::Spectrum(Spectrum&& sp) noexcept;
Spectrum::~Spectrum() noexcept;
Spectrum& Spectrum::operator=(const Spectrum& sp) noexcept;
Spectrum& Spectrum::operator=(Spectrum&& sp) noexcept;
```

Other life cycle operations.

```c++
constexpr LC Spectrum::lc() const noexcept;
constexpr Sp Spectrum::cls() const noexcept;
constexpr int Spectrum::sub() const noexcept;
```

Return the luminosity class, spectral class, or subclass.

```c++
constexpr bool Spectrum::is_giant() const noexcept;
constexpr bool Spectrum::is_main_sequence() const noexcept;
constexpr bool Spectrum::is_hr_type() const noexcept;
constexpr bool Spectrum::is_brown_dwarf() const noexcept;
constexpr bool Spectrum::is_white_dwarf() const noexcept;
constexpr bool Spectrum::is_wolf_rayet_star() const noexcept;
constexpr bool Spectrum::is_carbon_star() const noexcept;
constexpr bool Spectrum::is_stellar_remnant() const noexcept;
```

Boolean properties. Spectral types O-F Ia-III and G-M Ia-IV are counted as
giants; O-F IV-VI and G-M V-VI are main sequence. The `is_hr_type()` function
is true for types on the HR diagram, i.e. O-M.

```c++
double Spectrum::bc() const;           // Bolometric correction (mag)
double Spectrum::b_v() const;          // B-V colour index (mag)
double Spectrum::luminosity() const;   // Stellar luminosity (W)
double Spectrum::mass() const;         // Stellar mass (kg)
double Spectrum::radius() const;       // Stellar radius (m)
double Spectrum::temperature() const;  // Effective temperature (K)
```

Stellar physical parameters. Apart from the colour parameters, these are in SI
units. For a default constructed object, all of these will return NaN. For
stellar remnant types, `bc()` and `b_v()` will return NaN; `luminosity()` and
`temperature()` will return zero.

```c++
double Spectrum::log_l() const;
double Spectrum::log_m() const;
double Spectrum::log_r() const;
double Spectrum::log_t() const;
```

Decimal logarithms of some of the physical parameters. For luminosity, mass,
and radius, this is given relative to the Sun. Where the original physical
parameter function returns zero or NaN, these will return NaN.

```c++
double Spectrum::M_bol() const;
double Spectrum::M_v() const;
```

Absolute bolometric and visual magnitudes.

```c++
size_t Spectrum::hash() const noexcept;
class std::hash<Spectrum>;
```

Hash functions.

```c++
std::string Spectrum::str() const;
std::ostream& operator<<(std::ostream& out, const Spectrum& sp);
```

Format the spectral type as a string. For a default constructed object, these
will yield an empty string.

```c++
static Spectrum Spectrum::from_l(double lum);
static Spectrum Spectrum::from_log_l(double log_l);
static Spectrum Spectrum::from_m(double mass);
static Spectrum Spectrum::from_log_m(double log_m);
```

Return the main sequence spectral class corresponding to a given luminosity or
mass. The result is clamped to the range from O0 to M9, and rounded to the
nearest integral subclass. As for the other physical property functions,
logarithmic values are scaled to the Sun's luminosity and mass, while
non-logarithmic values are in SI units.

```c++
constexpr bool operator==(Spectrum a, Spectrum b) noexcept;
constexpr bool operator!=(Spectrum a, Spectrum b) noexcept;
constexpr bool operator<(Spectrum a, Spectrum b) noexcept;
constexpr bool operator>(Spectrum a, Spectrum b) noexcept;
constexpr bool operator<=(Spectrum a, Spectrum b) noexcept;
constexpr bool operator>=(Spectrum a, Spectrum b) noexcept;
constexpr std::strong_ordering operator<=>(Spectrum a, Spectrum b) noexcept;
```

Comparison functions. Ordering is by luminosity class, then spectral class,
then subclass.

```c++
namespace Literals {
    Spectrum operator""_sp(const char* ptr, size_t len);
}
```

Literal operator, equivalent to the constructor from a string.
