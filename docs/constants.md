# Constants

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/constants.hpp"
namespace Crow;
```

All of the constants defined here follow the conventions of the standard
`<numbers>` header. Each constant is defined in two forms:

```c++
constexpr double name = [value as double];
template <std::floating_point T> constexpr T name_v = [value as T];
```

For the physical and astronomical constants, long and short versions of the
constant names are provided. The short symbol-named constants (but not the
full-named equivalents) are in the `Constants` sub-namespace to avoid name
collisions.

The values quoted here are approximate; the values actually supplied are
accurate to the precision of the type or to the best known precision,
whichever is appropriate.

## Contents

* TOC
{:toc}

## Conversion factors

| Constant         | Value                        |
| --------         | -----                        |
| `arcsec`         | 4.84814×10<sup>-6</sup> rad  |
| `arcmin`         | 2.90888×10<sup>-4</sup> rad  |
| `degree`         | 0.0174533 rad                |
| `inch`           | 0.0254 m                     |
| `foot`           | 0.3048 m                     |
| `yard`           | 0.9144 m                     |
| `mile`           | 1609.344 m                   |
| `nautical_mile`  | 1852 m                       |
| `ounce`          | 0.0283495 kg                 |
| `pound`          | 0.453592 kg                  |
| `short_ton`      | 907.185 kg                   |
| `long_ton`       | 1016.05 kg                   |
| `pound_force`    | 4.44822 N                    |
| `erg`            | 10<sup>-7</sup> J            |
| `foot_pound`     | 1.35582 J                    |
| `calorie`        | 4.184 J                      |
| `kilocalorie`    | 4184 J                       |
| `ton_tnt`        | 4.184×10<sup>9</sup> J       |
| `horsepower`     | 745.700 W                    |
| `mmHg`           | 133.322 Pa                   |
| `atmosphere`     | 101325 Pa                    |
| `zero_celsius`   | 273.15 K                     |

## Physical constants

Sources:

* [CODATA recommended values](https://physics.nist.gov/cuu/Constants/)

Values related to quantity of substance are given in kilomoles to avoid
intrusive factors of 1000 in expressions.

| Constant                     | Symbol       | Value                                                                  |
| --------                     | ------       | -----                                                                  |
| `alpha_particle_mass`        | `m_alpha`    | 6.64466×10<sup>-27</sup> kg                                            |
| `atomic_mass_constant`       | `m_u`        | 1.66054×10<sup>-27</sup> kg                                            |
| `avogadro_constant`          | `N_A`        | 6.02214×10<sup>26</sup> kmol<sup>-1</sup>                              |
| `boltzmann_constant`         | `k`          | 1.38065×10<sup>-23</sup> J/K                                           |
| `electron_mass`              | `m_e`        | 9.10938×10<sup>-31</sup> kg                                            |
| `elementary_charge`          | `ec`         | 1.60218×10<sup>-19</sup> C                                             |
| `fine_structure_constant`    | `alpha`      | 7.29735×10<sup>-3</sup> dimensionless                                  |
| `first_radiation_constant`   | `c_1`        | 3.74177×10<sup>-16</sup> W m<sup>2</sup>                               |
| `gravitational_constant`     | `G`          | 6.67428×10<sup>-11</sup> m<sup>3</sup> kg<sup>-1</sup> s<sup>-2</sup>  |
| `josephson_constant`         | `K_J`        | 4.83598×10<sup>14</sup> Hz V<sup>-1</sup>                              |
| `molar_gas_constant`         | `R`          | 8314.46 J kmol<sup>-1</sup> K<sup>-1</sup>                             |
| `neutron_mass`               | `m_n`        | 1.67493×10<sup>-27</sup> kg                                            |
| `planck_constant`            | `h`          | 6.62607×10<sup>-34</sup> J s                                           |
| `proton_mass`                | `m_p`        | 1.67262×10<sup>-27</sup> kg                                            |
| `rydberg_constant`           | `R_inf`      | 1.09737×10<sup>7</sup> m<sup>-1</sup>                                  |
| `second_radiation_constant`  | `c_2`        | 0.0143878×10<sup>-2</sup> m K                                          |
| `speed_of_light`             | `c`          | 2.99792×10<sup>8</sup> m/s                                             |
| `stefan_boltzmann_constant`  | `sigma`      | 5.67037×10<sup>-8</sup> W m<sup>-2</sup> K<sup>-4</sup>                |
| `vacuum_impedance`           | `Z_0`        | 376.730 Ω                                                              |
| `vacuum_permeability`        | `mu_0`       | 1.25664×10<sup>-6</sup> N A<sup>-2</sup>                               |
| `vacuum_permittivity`        | `epsilon_0`  | 8.85419×10<sup>-12</sup> F m<sup>-1</sup>                              |
| `von_klitzing_constant`      | `R_K`        | 25812.8 Ω                                                              |
| `wien_constant`              | `b_W`        | 2.89777×10<sup>-3</sup> m K                                            |

## Astronomical constants

Sources:

* [IAU current best estimates](https://iau-a3.gitlab.io/NSFA/NSFA_cbe.html)
* [IERS useful constants](https://hpiers.obspm.fr/eop-pc/models/constants.html)

| Constant                          | Symbol      | Value                       |
| --------                          | ------      | -----                       |
| `astronomical_unit`               | `au`        | 1.49598×10<sup>11</sup> m   |
| `light_year`                      | `ly`        | 9.46073×10<sup>15</sup> m   |
| `parsec`                          | `pc`        | 3.08568×10<sup>16</sup> m   |
| `earth_mass`                      | `M_earth`   | 5.97219×10<sup>24</sup> kg  |
| `earth_radius`                    | `R_earth`   | 6.37814×10<sup>6</sup> m    |
| `standard_gravity`                | `g_0`       | 9.80665 m s<sup>-2</sup>    |
| `julian_day`                      | `jd`        | 86400 s                     |
| `sidereal_day`                    | `d_sid`     | 86164.1 s                   |
| `tropical_year`                   | `y_trop`    | 3.15569×10<sup>7</sup> s    |
| `julian_year`                     | `jy`        | 3.15576×10<sup>7</sup> s    |
| `sidereal_year`                   | `y_sid`     | 3.15581×10<sup>7</sup> s    |
| `lunar_mass`                      | `M_moon`    | 7.34581×10<sup>22</sup> kg  |
| `lunar_radius`                    | `R_moon`    | 1.7381×10<sup>6</sup> m     |
| `lunar_distance`                  | `a_moon`    | 3.84399×10<sup>8</sup> m    |
| `sidereal_month`                  | `sid_mon`   | 2.36059×10<sup>6</sup> s    |
| `synodic_month`                   | `syn_mon`   | 2.55144×10<sup>6</sup> s    |
| `jupiter_mass`                    | `M_jup`     | 1.89852×10<sup>27</sup> kg  |
| `jupiter_radius`                  | `R_jup`     | 7.1492×10<sup>7</sup> m     |
| `solar_mass`                      | `M_sun`     | 1.98842×10<sup>30</sup> kg  |
| `solar_radius`                    | `R_sun`     | 6.957×10<sup>8</sup> m      |
| `solar_temperature`               | `T_sun`     | 5772 K                      |
| `solar_luminosity`                | `L_sun`     | 3.828×10<sup>26</sup> W     |
| `solar_visual_magnitude`          | `Mv_sun`    | 4.81 mag                    |
| `solar_bolometric_magnitude`      | `Mbol_sun`  | 4.74 mag                    |
| `solar_bolometric_correction`     | `BC_sun`    | -0.07 mag                   |
| `bolometric_luminosity_constant`  | `L_0`       | 3.0128×10<sup>28</sup> W    |
| `age_of_universe`                 | `t_0`       | 4.3508×10<sup>17</sup> s    |
