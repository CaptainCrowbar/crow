#include "crow/constants.hpp"
#include "crow/unit-test.hpp"
#include <type_traits>

using namespace Crow;

void test_crow_constants() {

    TEST_TYPE(std::decay_t<decltype(pi_f)>,               float);
    TEST_TYPE(std::decay_t<decltype(pi_d)>,               double);
    TEST_TYPE(std::decay_t<decltype(pi_ld)>,              long double);
    TEST_TYPE(std::decay_t<decltype(pi_c<float>)>,        float);
    TEST_TYPE(std::decay_t<decltype(pi_c<double>)>,       double);
    TEST_TYPE(std::decay_t<decltype(pi_c<long double>)>,  long double);

    TEST_NEAR(e_d,                       2.718'281'828,     1e-9);
    TEST_NEAR(pi_d,                      3.141'592'654,     1e-9);
    TEST_NEAR(sqrt2_d,                   1.414'213'562,     1e-9);
    TEST_NEAR(degree_d,                  0.017'453'292'52,  1e-11);
    TEST_NEAR(mile_d,                    1'609.344,         1e-6);
    TEST_NEAR(pound_d,                   0.453'592'37,      1e-10);
    TEST_NEAR(gravitational_constant_d,  6.674'28e-11,      1e-21);
    TEST_NEAR(G_d,                       6.674'28e-11,      1e-21);
    TEST_NEAR(planck_constant_d,         6.626'070'15e-34,  1e-44);
    TEST_NEAR(h_d,                       6.626'070'15e-34,  1e-44);
    TEST_NEAR(speed_of_light_d,          299'792'458.0,     0.01);
    TEST_NEAR(c_d,                       299'792'458.0,     0.01);
    TEST_NEAR(astronomical_unit_d,       1.495'978'707e11,  10);
    TEST_NEAR(au_d,                      1.495'978'707e11,  10);
    TEST_NEAR(julian_year_d,             31'557'600.0,      1e-3);
    TEST_NEAR(jy_d,                      31'557'600.0,      1e-3);
    TEST_NEAR(solar_mass_d,              1.988'42e30,       1e20);
    TEST_NEAR(M_sun_d,                   1.988'42e30,       1e20);

}
