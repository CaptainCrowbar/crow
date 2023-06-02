#include "crow/interpolate.hpp"
#include "crow/unit-test.hpp"
#include <utility>
#include <vector>

using namespace Crow;

void test_lerp_cubic_spline_map() {

    using cubic = CubicSplineMap<double>;

    cubic c;
    std::vector<std::pair<double, double>> points;
    double y = 0;
    double eps = 1e-4;

    points = {
        { 12, 5 },
        { 34, 25 },
        { 60, 8 },
        { 98, 28 },
    };

    TRY(c = cubic(points));

    TRY(y = c[1]);    TEST_NEAR(y, -8.6066, eps);
    TRY(y = c[12]);   TEST_NEAR(y, 5.0000, eps);
    TRY(y = c[23]);   TEST_NEAR(y, 18.6066, eps);
    TRY(y = c[34]);   TEST_NEAR(y, 25.0000, eps);
    TRY(y = c[47]);   TEST_NEAR(y, 18.1768, eps);
    TRY(y = c[60]);   TEST_NEAR(y, 8.0000, eps);
    TRY(y = c[79]);   TEST_NEAR(y, 10.8217, eps);
    TRY(y = c[98]);   TEST_NEAR(y, 28.0000, eps);
    TRY(y = c[117]);  TEST_NEAR(y, 45.1783, eps);

    TRY((c = {
        { 12, 5 },
        { 34, 25 },
        { 60, 8 },
        { 98, 28 },
        { 102, 28 },
        { 140, 8 },
        { 166, 25 },
        { 188, 5 },
    }));

    TRY(y = c[1]);    TEST_NEAR(y, -8.8157, eps);
    TRY(y = c[12]);   TEST_NEAR(y, 5.0000, eps);
    TRY(y = c[23]);   TEST_NEAR(y, 18.8157, eps);
    TRY(y = c[34]);   TEST_NEAR(y, 25.0000, eps);
    TRY(y = c[47]);   TEST_NEAR(y, 17.3904, eps);
    TRY(y = c[60]);   TEST_NEAR(y, 8.0000, eps);
    TRY(y = c[79]);   TEST_NEAR(y, 15.8511, eps);
    TRY(y = c[98]);   TEST_NEAR(y, 28.0000, eps);
    TRY(y = c[100]);  TEST_NEAR(y, 28.1625, eps);
    TRY(y = c[102]);  TEST_NEAR(y, 28.0000, eps);
    TRY(y = c[121]);  TEST_NEAR(y, 15.8511, eps);
    TRY(y = c[140]);  TEST_NEAR(y, 8.0000, eps);
    TRY(y = c[153]);  TEST_NEAR(y, 17.3904, eps);
    TRY(y = c[166]);  TEST_NEAR(y, 25.0000, eps);
    TRY(y = c[177]);  TEST_NEAR(y, 18.8157, eps);
    TRY(y = c[188]);  TEST_NEAR(y, 5.0000, eps);
    TRY(y = c[199]);  TEST_NEAR(y, -8.8157, eps);

}

void test_lerp_cubic_spline_map_log_log() {

    using cubic = CubicSplineMap<double, Logs::XY>;

    cubic c;
    double y = 0;
    double eps = 1e-6;

    TRY((c = {
        { 2, 1 },
        { 8, 1024 },
        { 64, 32 },
        { 512, 32768 },
    }));

    TRY(y = c[1]);     TEST_NEAR(y, 0.012835, eps);
    TRY(y = c[2]);     TEST_NEAR(y, 1, eps);
    TRY(y = c[4]);     TEST_NEAR(y, 77.912548, eps);
    TRY(y = c[8]);     TEST_NEAR(y, 1024, eps);
    TRY(y = c[16]);    TEST_NEAR(y, 726.341653, eps);
    TRY(y = c[32]);    TEST_NEAR(y, 115.108204, eps);
    TRY(y = c[64]);    TEST_NEAR(y, 32, eps);
    TRY(y = c[128]);   TEST_NEAR(y, 75.125523, eps);
    TRY(y = c[256]);   TEST_NEAR(y, 1013.397869, eps);
    TRY(y = c[512]);   TEST_NEAR(y, 32768, eps);
    TRY(y = c[1024]);  TEST_NEAR(y, 1059546.163, 1e-3);

}
