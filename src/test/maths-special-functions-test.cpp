#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <limits>

using namespace Crow;

constexpr auto inf = std::numeric_limits<double>::infinity();

void test_crow_maths_inverse_erf() {

    double x = 0, y = 0, z = 0;

    x = -inf;  y = std::erf(x);  TEST_EQUAL(y, -1);                      TRY(z = inverse_erf(y));  TEST_EQUAL(z, x);
    x = -4.0;  y = std::erf(x);  TEST_NEAR(y, -0.999'999'984'6, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -3.5;  y = std::erf(x);  TEST_NEAR(y, -0.999'999'256'9, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -3.0;  y = std::erf(x);  TEST_NEAR(y, -0.999'977'909'5, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -2.5;  y = std::erf(x);  TEST_NEAR(y, -0.999'593'048'0, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -2.0;  y = std::erf(x);  TEST_NEAR(y, -0.995'322'265'0, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -1.5;  y = std::erf(x);  TEST_NEAR(y, -0.966'105'146'5, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -1.0;  y = std::erf(x);  TEST_NEAR(y, -0.842'700'792'9, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = -0.5;  y = std::erf(x);  TEST_NEAR(y, -0.520'499'877'8, 1e-10);  TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 0.0;   y = std::erf(x);  TEST_EQUAL(y, 0);                       TRY(z = inverse_erf(y));  TEST_EQUAL(z, x);
    x = 0.5;   y = std::erf(x);  TEST_NEAR(y, 0.520'499'877'8, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 1.0;   y = std::erf(x);  TEST_NEAR(y, 0.842'700'792'9, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 1.5;   y = std::erf(x);  TEST_NEAR(y, 0.966'105'146'5, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 2.0;   y = std::erf(x);  TEST_NEAR(y, 0.995'322'265'0, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 2.5;   y = std::erf(x);  TEST_NEAR(y, 0.999'593'048'0, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 3.0;   y = std::erf(x);  TEST_NEAR(y, 0.999'977'909'5, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 3.5;   y = std::erf(x);  TEST_NEAR(y, 0.999'999'256'9, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = 4.0;   y = std::erf(x);  TEST_NEAR(y, 0.999'999'984'6, 1e-10);   TRY(z = inverse_erf(y));  TEST_NEAR(z, x, 1e-9);
    x = inf;   y = std::erf(x);  TEST_EQUAL(y, 1);                       TRY(z = inverse_erf(y));  TEST_EQUAL(z, x);

    x = -inf;  y = std::erfc(x);  TEST_EQUAL(y, 2);                               TRY(z = inverse_erfc(y));  TEST_EQUAL(z, x);
    x = -4.0;  y = std::erfc(x);  TEST_NEAR(y, 1.999'999'985, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -3.5;  y = std::erfc(x);  TEST_NEAR(y, 1.999'999'257, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -3.0;  y = std::erfc(x);  TEST_NEAR(y, 1.999'977'910, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -2.5;  y = std::erfc(x);  TEST_NEAR(y, 1.999'593'048, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -2.0;  y = std::erfc(x);  TEST_NEAR(y, 1.995'322'265, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -1.5;  y = std::erfc(x);  TEST_NEAR(y, 1.966'105'146, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -1.0;  y = std::erfc(x);  TEST_NEAR(y, 1.842'700'793, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = -0.5;  y = std::erfc(x);  TEST_NEAR(y, 1.520'499'878, 1e-9);              TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 0.0;   y = std::erfc(x);  TEST_EQUAL(y, 1);                               TRY(z = inverse_erfc(y));  TEST_EQUAL(z, x);
    x = 0.5;   y = std::erfc(x);  TEST_NEAR(y, 0.479'500'122'2, 1e-10);           TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 1.0;   y = std::erfc(x);  TEST_NEAR(y, 0.157'299'207'1, 1e-10);           TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 1.5;   y = std::erfc(x);  TEST_NEAR(y, 0.033'894'853'52, 1e-11);          TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 2.0;   y = std::erfc(x);  TEST_NEAR(y, 0.004'677'734'981, 1e-12);         TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 2.5;   y = std::erfc(x);  TEST_NEAR(y, 0.000'406'952'017'4, 1e-13);       TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 3.0;   y = std::erfc(x);  TEST_NEAR(y, 0.000'022'090'497'00, 1e-14);      TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 3.5;   y = std::erfc(x);  TEST_NEAR(y, 0.000'000'743'098'372'3, 1e-16);   TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = 4.0;   y = std::erfc(x);  TEST_NEAR(y, 0.000'000'015'417'257'90, 1e-17);  TRY(z = inverse_erfc(y));  TEST_NEAR(z, x, 1e-9);
    x = inf;   y = std::erfc(x);  TEST_EQUAL(y, 0);                               TRY(z = inverse_erfc(y));  TEST_EQUAL(z, x);

}
