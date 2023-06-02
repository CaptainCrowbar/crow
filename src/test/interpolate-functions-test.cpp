#include "crow/interpolate.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_lerp_linear_interpolation() {

    TEST_EQUAL(lerp(10.0, 20.0, -0.25),           7.5);
    TEST_EQUAL(lerp(10.0, 20.0, 0.00),            10.0);
    TEST_EQUAL(lerp(10.0, 20.0, 0.25),            12.5);
    TEST_EQUAL(lerp(10.0, 20.0, 0.50),            15.0);
    TEST_EQUAL(lerp(10.0, 20.0, 0.75),            17.5);
    TEST_EQUAL(lerp(10.0, 20.0, 1.00),            20.0);
    TEST_EQUAL(lerp(10.0, 20.0, 1.25),            22.5);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 0.75),  7.5);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 1.00),  10.0);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 1.25),  12.5);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 1.50),  15.0);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 1.75),  17.5);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 2.00),  20.0);
    TEST_EQUAL(lerp(1.0, 10.0, 2.0, 20.0, 2.25),  22.5);

}

void test_lerp_logarithmic_interpolation() {

    using LogLin = Lerp<Logs::X>;
    using LinLog = Lerp<Logs::Y>;
    using LogLog = Lerp<Logs::XY>;

    double eps = 1e-6;

    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 1.0),    7.5, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 2.0),    10.0, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 4.0),    12.5, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 8.0),    15.0, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 16.0),   17.5, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 32.0),   20.0, eps);
    TEST_NEAR(LogLin()(2.0, 10.0, 32.0, 20.0, 64.0),   22.5, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 0.75),   10.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 1.00),   20.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 1.25),   40.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 1.50),   80.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 1.75),   160.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 2.00),   320.0, eps);
    TEST_NEAR(LinLog()(1.0, 20.0, 2.0, 320.0, 2.25),   640.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 1.0),   10.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 2.0),   20.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 4.0),   40.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 8.0),   80.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 16.0),  160.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 32.0),  320.0, eps);
    TEST_NEAR(LogLog()(2.0, 20.0, 32.0, 320.0, 64.0),  640.0, eps);

}

void test_lerp_smoothstep() {

    // TODO

}
