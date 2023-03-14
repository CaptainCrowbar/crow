#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <numbers>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_maths_numeric_literals() {

    using std::numbers::pi_v;

    TEST_NEAR(45_degf,    pi_v<float> / 4.0f, 1e-5);
    TEST_NEAR(90_degf,    pi_v<float> / 2.0f, 1e-5);
    TEST_NEAR(180_degf,   pi_v<float>, 1e-5);
    TEST_NEAR(360_degf,   pi_v<float> * 2.0f, 1e-5);
    TEST_NEAR(45_degd,    pi_v<double> / 4.0, 1e-15);
    TEST_NEAR(90_degd,    pi_v<double> / 2.0, 1e-15);
    TEST_NEAR(180_degd,   pi_v<double>, 1e-15);
    TEST_NEAR(360_degd,   pi_v<double> * 2.0, 1e-15);
    TEST_NEAR(45_degld,   pi_v<long double> / 4.0l, 1e-15);
    TEST_NEAR(90_degld,   pi_v<long double> / 2.0l, 1e-15);
    TEST_NEAR(180_degld,  pi_v<long double>, 1e-15);
    TEST_NEAR(360_degld,  pi_v<long double> * 2.0l, 1e-15);

}
