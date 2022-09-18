#include "crow/formula.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_formula_sphere_geometry() {

    double r = 10;
    double a = 1256.6371;
    double v = 4188.7902;

    TEST_NEAR(sphere_area_from_radius(r),    a,  1e-4);
    TEST_NEAR(sphere_radius_from_area(a),    r,  1e-4);
    TEST_NEAR(sphere_volume_from_radius(r),  v,  1e-4);
    TEST_NEAR(sphere_radius_from_volume(v),  r,  1e-4);

}

void test_crow_formula_mass_density() {

    double d = 5000;
    double m = 20'943'952;
    double r = 10;

    TEST_NEAR(mass_from_radius_density(r, d),  m,  1);
    TEST_NEAR(radius_from_mass_density(m, d),  r,  1e-4);
    TEST_NEAR(density_from_mass_radius(m, r),  d,  0.01);

}

void test_crow_formula_surface_gravity() {

    TEST_NEAR(gravity_from_mass_radius(M_earth_d, R_earth_d), 9.8, 0.01);

}

void test_crow_formula_orbital_period() {

    TEST_NEAR(distance_from_mass_period(M_sun_d, jy_d),  au_d,  1e7);
    TEST_NEAR(period_from_mass_distance(M_sun_d, au_d),  jy_d,  1000);

}

void test_crow_formula_hill_sphere() {

    TEST_NEAR(hill_radius(M_sun_d, M_earth_d, au_d),          1.497e9,   1e6);
    TEST_NEAR(hill_radius(M_sun_d, M_jup_d, 5.20336 * au_d),  5.315e10,  1e7);

}

void test_crow_formula_stellar_luminosity() {

    TEST_NEAR(luminosity_from_radius_temperature(R_sun_d, T_sun_d),  L_sun_d,  1e21);
    TEST_NEAR(radius_from_luminosity_temperature(L_sun_d, T_sun_d),  R_sun_d,  1000);
    TEST_NEAR(temperature_from_luminosity_radius(L_sun_d, R_sun_d),  T_sun_d,  0.01);

}

void test_crow_formula_stellar_lifetime() {

    TEST_NEAR(main_sequence_lifetime(M_sun_d, L_sun_d), 1e10 * jy_d, jy_d);

}

void test_crow_formula_schwarzschild_radius() {

    TEST_NEAR(schwarzschild_radius(M_sun_d), 2953, 1);
}

void test_crow_formula_magnitude_luminosity() {

    TEST_NEAR(delta_magnitude_to_ratio(-5.0),   100,   1e-4);
    TEST_EQUAL(delta_magnitude_to_ratio(0.0),   1);
    TEST_NEAR(delta_magnitude_to_ratio(5.0),    0.01,  1e-8);
    TEST_NEAR(ratio_to_delta_magnitude(100.0),  -5,    1e-6);
    TEST_EQUAL(ratio_to_delta_magnitude(1.0),   0);
    TEST_NEAR(ratio_to_delta_magnitude(0.01),   5,     1e-6);

    TEST_NEAR(bolometric_magitude_to_luminosity(Mbol_sun_d - 5),   L_sun_d * 100,   L_sun_d * 1e-4);
    TEST_NEAR(bolometric_magitude_to_luminosity(Mbol_sun_d),       L_sun_d,         L_sun_d * 1e-6);
    TEST_NEAR(bolometric_magitude_to_luminosity(Mbol_sun_d + 5),   L_sun_d * 0.01,  L_sun_d * 1e-8);
    TEST_NEAR(bolometric_luminosity_to_magnitude(L_sun_d * 100),   Mbol_sun_d - 5,  1e-6);
    TEST_NEAR(bolometric_luminosity_to_magnitude(L_sun_d),         Mbol_sun_d,      1e-6);
    TEST_NEAR(bolometric_luminosity_to_magnitude(L_sun_d * 0.01),  Mbol_sun_d + 5,  1e-6);

}

void test_crow_formula_bolometric_correction() {

    TEST_NEAR(temperature_to_bc(50'000.0),  -4.4824,  1e-4);
    TEST_NEAR(temperature_to_bc(20'000.0),  -1.8922,  1e-4);
    TEST_NEAR(temperature_to_bc(10'000.0),  -0.2492,  1e-4);
    TEST_NEAR(temperature_to_bc(7500.0),    +0.0338,  1e-4);
    TEST_NEAR(temperature_to_bc(T_sun_d),   -0.0810,  1e-4);
    TEST_NEAR(temperature_to_bc(5000.0),    -0.2856,  1e-4);
    TEST_NEAR(temperature_to_bc(4000.0),    -1.1236,  1e-4);
    TEST_NEAR(temperature_to_bc(3000.0),    -5.0084,  1e-4);

}

void test_crow_formula_planet_temperature() {

    TEST_NEAR(planet_distance_to_temperature(L_sun_d, au_d * 0.01),   2783.30,      0.01);
    TEST_NEAR(planet_distance_to_temperature(L_sun_d, au_d),          278.330,      0.001);
    TEST_NEAR(planet_distance_to_temperature(L_sun_d, au_d * 100),    27.8330,      1e-4);
    TEST_NEAR(planet_temperature_to_distance(L_sun_d, 2783.30),       au_d * 0.01,  1e-8 * au_d);
    TEST_NEAR(planet_temperature_to_distance(L_sun_d, 278.330),       au_d,         1e-6 * au_d);
    TEST_NEAR(planet_temperature_to_distance(L_sun_d, 27.8330),       au_d * 100,   1e-4 * au_d);
    TEST_NEAR(planet_distance_to_illumination(L_sun_d, au_d * 0.01),  1.36117e7,    100);
    TEST_NEAR(planet_distance_to_illumination(L_sun_d, au_d),         1361.17,      0.01);
    TEST_NEAR(planet_distance_to_illumination(L_sun_d, au_d * 100),   0.136117,     1e-6);
    TEST_NEAR(planet_illumination_to_temperature(1.36117e7),          2783.30,      0.01);
    TEST_NEAR(planet_illumination_to_temperature(1361.17),            278.330,      0.001);
    TEST_NEAR(planet_illumination_to_temperature(0.136117),           27.8330,      1e-4);

}
