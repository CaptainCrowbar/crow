# Formula Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/formula.hpp"
namespace Crow;
```

A collection of useful formulae, mostly astronomical. SI units are assumed for
all dimensioned quantities except stellar magnitude.

## Contents

* TOC
{:toc}

## Surface area and volume of a sphere

* _S = 4 π r<sup>2</sup>_
* _V = (4/3) π r<sup>3</sup>_

```c++
template <typename T> constexpr T sphere_area_from_radius(T radius) noexcept;
template <typename T> T sphere_radius_from_area(T area) noexcept;
template <typename T> T sphere_volume_from_radius(T radius) noexcept;
template <typename T> T sphere_radius_from_volume(T volume) noexcept;
```

## Mass and density

* _m = (4/3) π r<sup>3</sup> ρ_

```c++
template <typename T> T mass_from_radius_density(T radius, T density) noexcept;
template <typename T> T radius_from_mass_density(T mass, T density) noexcept;
template <typename T> T density_from_mass_radius(T mass, T radius) noexcept;
```

## Surface gravity

* _g = G m / r<sup>2</sup>_

```c++
template <typename T> constexpr T gravity_from_mass_radius(T mass, T radius) noexcept;
```

## Orbital period

* _G M P<sup>2</sup> = 4 π<sup>2</sup> a<sup>3</sup>_

```c++
template <typename T> T distance_from_mass_period(T mass, T period) noexcept;
template <typename T> T period_from_mass_distance(T mass, T distance) noexcept;
```

## Hill sphere

* _r = a (m<sub>s</sub> / 3 m<sub>p</sub>) <sup>1/3</sup>_

```c++
template <typename T> T hill_radius(T primary_mass, T secondary_mass, T distance) noexcept;
```

## Stellar luminosity

* _L = 4 π σ r<sup>2</sup> T<sup>4</sup>_

```c++
template <typename T> T luminosity_from_radius_temperature(T radius, T temperature) noexcept;
template <typename T> T radius_from_luminosity_temperature(T luminosity, T temperature) noexcept;
template <typename T> T temperature_from_luminosity_radius(T luminosity, T radius) noexcept;
```

## Main sequence lifetime

```c++
template <typename T> constexpr T main_sequence_lifetime(T mass, T luminosity) noexcept;
```

## Schwarzschild radius

* _r<sub>s</sub> = 2 G M / c<sup>2</sup>_

```c++
template <typename T> constexpr T schwarzschild_radius(T mass) noexcept;
```

## Magnitude and luminosity

```c++
template <typename T> T delta_magnitude_to_ratio(T delta_mag) noexcept;
template <typename T> T ratio_to_delta_magnitude(T ratio) noexcept;
template <typename T> T bolometric_magitude_to_luminosity(T mag) noexcept;
template <typename T> T bolometric_luminosity_to_magnitude(T luminosity) noexcept;
```

## Bolometric correction

Source: Guillermo Torres (2010), ["On The Use Of Empirical Bolometric Corrections For Stars"](https:##arxiv.org/abs/1008.3913v1)

```c++
template <typename T> T temperature_to_bc(T temperature) noexcept;
```

## Planet temperature and illumination

```c++
template <typename T> T planet_distance_to_temperature(T luminosity, T distance) noexcept;
template <typename T> T planet_temperature_to_distance(T luminosity, T temperature) noexcept;
template <typename T> T planet_distance_to_illumination(T luminosity, T distance) noexcept;
template <typename T> T planet_illumination_to_temperature(T illumination) noexcept;
```
