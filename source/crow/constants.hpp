#pragma once

#include "crow/types.hpp"

#define CROW_MATHS_CONSTANT(name, value) \
    constexpr float name ## _f = value ## f; \
    constexpr double name ## _d = value; \
    constexpr long double name ## _ld = value ## l; \
    template <typename T> constexpr T name ## _c = T(name ## _ld);

#define CROW_MATHS_CONSTANT_2(name, symbol, value) \
    constexpr float name ## _f = value ## f; \
    constexpr float symbol ## _f = name ## _f; \
    constexpr double name ## _d = value; \
    constexpr double symbol ## _d = name ## _d; \
    constexpr long double name ## _ld = value ## l; \
    constexpr long double symbol ## _ld = name ## _ld; \
    template <typename T> constexpr T name ## _c = T(name ## _ld); \
    template <typename T> constexpr T symbol ## _c = name ## _c<T>;

namespace Crow {

    // Mathematical constants

    CROW_MATHS_CONSTANT(e,                2.71828'18284'59045'23536'02874'71352'66249'77572'47093'69996);
    CROW_MATHS_CONSTANT(ln2,              0.69314'71805'59945'30941'72321'21458'17656'80755'00134'36026);
    CROW_MATHS_CONSTANT(ln10,             2.30258'50929'94045'68401'79914'54684'36420'76011'01488'62877);
    CROW_MATHS_CONSTANT(log2e,            1.44269'50408'88963'40735'99246'81001'89213'74266'45954'15299);
    CROW_MATHS_CONSTANT(log10e,           0.43429'44819'03251'82765'11289'18916'60508'22943'97005'80367);
    CROW_MATHS_CONSTANT(pi,               3.14159'26535'89793'23846'26433'83279'50288'41971'69399'37511);
    CROW_MATHS_CONSTANT(pi_over_2,        1.57079'63267'94896'61923'13216'91639'75144'20985'84699'68755);
    CROW_MATHS_CONSTANT(pi_over_4,        0.78539'81633'97448'30961'56608'45819'87572'10492'92349'84378);
    CROW_MATHS_CONSTANT(inv_pi,           0.31830'98861'83790'67153'77675'26745'02872'40689'19291'48091);
    CROW_MATHS_CONSTANT(inv_sqrtpi,       0.56418'95835'47756'28694'80794'51560'77258'58440'50629'32900);
    CROW_MATHS_CONSTANT(two_over_pi,      0.63661'97723'67581'34307'55350'53490'05744'81378'38582'96183);
    CROW_MATHS_CONSTANT(two_over_sqrtpi,  1.12837'91670'95512'57389'61589'03121'54517'16881'01258'65800);
    CROW_MATHS_CONSTANT(sqrt2,            1.41421'35623'73095'04880'16887'24209'69807'85696'71875'37695);
    CROW_MATHS_CONSTANT(sqrt3,            1.73205'08075'68877'29352'74463'41505'87236'69428'05253'81038);
    CROW_MATHS_CONSTANT(inv_sqrt2,        0.70710'67811'86547'52440'08443'62104'84903'92848'35937'68847);
    CROW_MATHS_CONSTANT(inv_sqrt3,        0.57735'02691'89625'76450'91487'80501'95745'56476'01751'27013);
    CROW_MATHS_CONSTANT(egamma,           0.57721'56649'01532'86060'65120'90082'40243'10421'59335'93992);
    CROW_MATHS_CONSTANT(phi,              1.61803'39887'49894'84820'45868'34365'63811'77203'09179'80576);

    // Conversion factors

    CROW_MATHS_CONSTANT(arcsec,         4.84813'68110'95359'93589'91410'2358e-6);  // rad
    CROW_MATHS_CONSTANT(arcmin,         2.90888'20866'57215'96153'94846'1415e-4);  // rad
    CROW_MATHS_CONSTANT(degree,         1.74532'92519'94329'57692'36907'6849e-2);  // rad
    CROW_MATHS_CONSTANT(inch,           0.0254);                                   // m
    CROW_MATHS_CONSTANT(foot,           0.3048);                                   // m
    CROW_MATHS_CONSTANT(yard,           0.9144);                                   // m
    CROW_MATHS_CONSTANT(mile,           1609.344);                                 // m
    CROW_MATHS_CONSTANT(nautical_mile,  1852.0);                                   // m
    CROW_MATHS_CONSTANT(ounce,          0.028'349'523'125);                        // kg
    CROW_MATHS_CONSTANT(pound,          0.453'592'37);                             // kg
    CROW_MATHS_CONSTANT(short_ton,      907.184'74);                               // kg
    CROW_MATHS_CONSTANT(long_ton,       1016.046'908'8);                           // kg
    CROW_MATHS_CONSTANT(pound_force,    4.448'221'615'260'5);                      // N
    CROW_MATHS_CONSTANT(erg,            1e-7);                                     // J
    CROW_MATHS_CONSTANT(foot_pound,     1.355'817'948'331'400'4);                  // J
    CROW_MATHS_CONSTANT(calorie,        4.184);                                    // J
    CROW_MATHS_CONSTANT(kilocalorie,    4184.0);                                   // J
    CROW_MATHS_CONSTANT(ton_tnt,        4.184e9);                                  // J
    CROW_MATHS_CONSTANT(horsepower,     745.699'871'582'270'22);                   // W
    CROW_MATHS_CONSTANT(mmHg,           133.322'387'415);                          // Pa
    CROW_MATHS_CONSTANT(atmosphere,     101'325.0);                                // Pa
    CROW_MATHS_CONSTANT(zero_celsius,   273.15);                                   // K

    // Physical constants
    // CODATA recommended values: https://physics.nist.gov/cuu/Constants/

    // Values related to quantity of substance are given in kilomoles to avoid
    // intrusive factors of 1000 in expressions.

    CROW_MATHS_CONSTANT_2(alpha_particle_mass,        m_alpha,    6.644'657'3357e-27);    // kg
    CROW_MATHS_CONSTANT_2(atomic_mass_constant,       m_u,        1.660'539'066'60e-27);  // kg
    CROW_MATHS_CONSTANT_2(avogadro_constant,          N_A,        6.022'140'76e26);       // kmol^-1
    CROW_MATHS_CONSTANT_2(boltzmann_constant,         k,          1.380'649e-23);         // J/K
    CROW_MATHS_CONSTANT_2(electron_mass,              m_e,        9.109'383'7015e-31);    // kg
    CROW_MATHS_CONSTANT_2(elementary_charge,          ec,         1.602'176'634e-19);     // C
    CROW_MATHS_CONSTANT_2(fine_structure_constant,    alpha,      7.297'352'5693e-3);     // dimensionless
    CROW_MATHS_CONSTANT_2(first_radiation_constant,   c_1,        3.741'771'852e-16);     // W m^2 (c_1 = 2π h c^2)
    CROW_MATHS_CONSTANT_2(gravitational_constant,     G,          6.674'28e-11);          // m^3 kg^-1 s^-2
    CROW_MATHS_CONSTANT_2(josephson_constant,         K_J,        4.835'978'484e14);      // Hz V^-1 (K_J = 2e/h)
    CROW_MATHS_CONSTANT_2(molar_gas_constant,         R,          8'314.462'618);         // J kmol^-1 K^-1 (R = N_A k)
    CROW_MATHS_CONSTANT_2(neutron_mass,               m_n,        1.674'927'498'04e-27);  // kg
    CROW_MATHS_CONSTANT_2(planck_constant,            h,          6.626'070'15e-34);      // J s
    CROW_MATHS_CONSTANT_2(proton_mass,                m_p,        1.672'621'923'69e-27);  // kg
    CROW_MATHS_CONSTANT_2(rydberg_constant,           R_inf,      10'973'731.568'160);    // m^-1 (R_∞ = m_e e^4/8 ε_0^2 h^3 c)
    CROW_MATHS_CONSTANT_2(second_radiation_constant,  c_2,        1.438'776'877e-2);      // m K (c_2 = h c/k)
    CROW_MATHS_CONSTANT_2(speed_of_light,             c,          299'792'458.0);         // m/s
    CROW_MATHS_CONSTANT_2(stefan_boltzmann_constant,  sigma,      5.670'374'419e-8);      // W m^-2 K^-4 (σ = π^2 k^4/60 ℏ^3 c^2)
    CROW_MATHS_CONSTANT_2(vacuum_impedance,           Z_0,        376.730'313'668);       // Ω (Z_0 = μ_0 c)
    CROW_MATHS_CONSTANT_2(vacuum_permeability,        mu_0,       1.256'637'062'12e-6);   // N A^-2 (μ_0 = 4π α ℏ/e^2 c)
    CROW_MATHS_CONSTANT_2(vacuum_permittivity,        epsilon_0,  8.854'187'812'8e-12);   // F m^-1 (ε_0 = 1/μ_0 c^2)
    CROW_MATHS_CONSTANT_2(von_klitzing_constant,      R_K,        25'812.807'45);         // Ω (R_K = h/e^2)
    CROW_MATHS_CONSTANT_2(wien_constant,              b_W,        2.897'771'955e-3);      // m K

    // Astronomical constants
    // IAU current best estimates: https://iau-a3.gitlab.io/NSFA/NSFA_cbe.html
    // IERS useful constants: https://hpiers.obspm.fr/eop-pc/models/constants.html

    CROW_MATHS_CONSTANT_2(astronomical_unit,               au,        1.495'978'707e11);        // m
    CROW_MATHS_CONSTANT_2(light_year,                      ly,        9.460'730'472'580'8e15);  // m
    CROW_MATHS_CONSTANT_2(parsec,                          pc,        3.085'677'581'49e16);     // m
    CROW_MATHS_CONSTANT_2(earth_mass,                      M_earth,   5.972'19e24);             // kg
    CROW_MATHS_CONSTANT_2(earth_radius,                    R_earth,   6'378'136.6);             // m
    CROW_MATHS_CONSTANT_2(standard_gravity,                g_0,       9.806'65);                // m s^-2
    CROW_MATHS_CONSTANT_2(julian_day,                      jd,        86'400.0);                // s
    CROW_MATHS_CONSTANT_2(sidereal_day,                    d_sid,     86'164.090'530'832'88);   // s
    CROW_MATHS_CONSTANT_2(tropical_year,                   y_trop,    31'556'925.25);           // s
    CROW_MATHS_CONSTANT_2(julian_year,                     jy,        31'557'600.0);            // s
    CROW_MATHS_CONSTANT_2(sidereal_year,                   y_sid,     31'558'149.76);           // s
    CROW_MATHS_CONSTANT_2(lunar_mass,                      M_moon,    7.345'81e22);             // kg
    CROW_MATHS_CONSTANT_2(lunar_radius,                    R_moon,    1.738'1e6);               // m
    CROW_MATHS_CONSTANT_2(lunar_distance,                  a_moon,    3.843'99e8);              // m
    CROW_MATHS_CONSTANT_2(sidereal_month,                  mon_sid,   2'360'591.6);             // s
    CROW_MATHS_CONSTANT_2(synodic_month,                   mon_syn,   2'551'442.9);             // s
    CROW_MATHS_CONSTANT_2(jupiter_mass,                    M_jup,     1.898'52e27);             // kg
    CROW_MATHS_CONSTANT_2(jupiter_radius,                  R_jup,     7.149'2e7);               // m
    CROW_MATHS_CONSTANT_2(solar_mass,                      M_sun,     1.988'42e30);             // kg
    CROW_MATHS_CONSTANT_2(solar_radius,                    R_sun,     6.957e8);                 // m
    CROW_MATHS_CONSTANT_2(solar_temperature,               T_sun,     5772.0);                  // K
    CROW_MATHS_CONSTANT_2(solar_luminosity,                L_sun,     3.828e26);                // W
    CROW_MATHS_CONSTANT_2(solar_visual_magnitude,          Mv_sun,    4.81);                    // mag
    CROW_MATHS_CONSTANT_2(solar_bolometric_magnitude,      Mbol_sun,  4.74);                    // mag
    CROW_MATHS_CONSTANT_2(solar_bolometric_correction,     BC_sun,    -0.07);                   // mag
    CROW_MATHS_CONSTANT_2(bolometric_luminosity_constant,  L_0,       3.012'8e28);              // W
    CROW_MATHS_CONSTANT_2(age_of_universe,                 t_0,       4.350'8e17);              // s

}
