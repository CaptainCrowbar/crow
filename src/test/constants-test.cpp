#include "crow/constants.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

namespace C = Crow::Constants;

void test_crow_constants() {

    TEST_TYPE(decltype(speed_of_light_v<float>),        const float);
    TEST_TYPE(decltype(speed_of_light_v<double>),       const double);
    TEST_TYPE(decltype(speed_of_light_v<long double>),  const long double);
    TEST_TYPE(decltype(speed_of_light),                 const double);

    TEST_NEAR(degree,                  0.017'453'292'52,  1e-11);
    TEST_NEAR(mile,                    1'609.344,         1e-6);
    TEST_NEAR(pound,                   0.453'592'37,      1e-10);
    TEST_NEAR(gravitational_constant,  6.674'28e-11,      1e-21);
    TEST_NEAR(C::G,                    6.674'28e-11,      1e-21);
    TEST_NEAR(planck_constant,         6.626'070'15e-34,  1e-44);
    TEST_NEAR(C::h,                    6.626'070'15e-34,  1e-44);
    TEST_NEAR(speed_of_light,          299'792'458.0,     0.01);
    TEST_NEAR(C::c,                    299'792'458.0,     0.01);
    TEST_NEAR(astronomical_unit,       1.495'978'707e11,  10);
    TEST_NEAR(C::au,                   1.495'978'707e11,  10);
    TEST_NEAR(julian_year,             31'557'600.0,      1e-3);
    TEST_NEAR(C::jy,                   31'557'600.0,      1e-3);
    TEST_NEAR(solar_mass,              1.988'42e30,       1e20);
    TEST_NEAR(C::M_sun,                1.988'42e30,       1e20);

}
