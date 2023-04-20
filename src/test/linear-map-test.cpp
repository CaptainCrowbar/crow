#include "crow/linear-map.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

namespace {

    constexpr double eps = 1e-10;

}

void test_crow_linear_map() {

    LinearMap<double> map;

    TEST(map.empty());
    TEST_EQUAL(map.min(), 0);
    TEST_EQUAL(map.max(), 0);
    TEST_NEAR(map[0], 0, eps);
    TEST_NEAR(map[10], 0, eps);

    TRY(map.insert(5, 10));
    TEST(! map.empty());
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 5);
    TEST_NEAR(map[0], 10, eps);
    TEST_NEAR(map[5], 10, eps);
    TEST_NEAR(map[10], 10, eps);

    TRY(map.insert(10, 20));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 10);
    TEST_NEAR(map[4], 10, eps);
    TEST_NEAR(map[5], 10, eps);
    TEST_NEAR(map[6], 12, eps);
    TEST_NEAR(map[7], 14, eps);
    TEST_NEAR(map[8], 16, eps);
    TEST_NEAR(map[9], 18, eps);
    TEST_NEAR(map[10], 20, eps);
    TEST_NEAR(map[11], 20, eps);

    TRY(map.insert(15, 100, 10));
    TRY(map.insert(20, -10));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 20);
    TEST_NEAR(map[4], 10, eps);
    TEST_NEAR(map[5], 10, eps);
    TEST_NEAR(map[6], 12, eps);
    TEST_NEAR(map[7], 14, eps);
    TEST_NEAR(map[8], 16, eps);
    TEST_NEAR(map[9], 18, eps);
    TEST_NEAR(map[10], 20, eps);
    TEST_NEAR(map[11], 36, eps);
    TEST_NEAR(map[12], 52, eps);
    TEST_NEAR(map[13], 68, eps);
    TEST_NEAR(map[14], 84, eps);
    TEST_NEAR(map[15], 55, eps);
    TEST_NEAR(map[16], 6, eps);
    TEST_NEAR(map[17], 2, eps);
    TEST_NEAR(map[18], -2, eps);
    TEST_NEAR(map[19], -6, eps);
    TEST_NEAR(map[20], -10, eps);
    TEST_NEAR(map[21], -10, eps);

    TRY(map.erase(10));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 20);
    TEST_NEAR(map[4], 10, eps);
    TEST_NEAR(map[5], 10, eps);
    TEST_NEAR(map[6], 19, eps);
    TEST_NEAR(map[7], 28, eps);
    TEST_NEAR(map[8], 37, eps);
    TEST_NEAR(map[9], 46, eps);
    TEST_NEAR(map[10], 55, eps);
    TEST_NEAR(map[11], 64, eps);
    TEST_NEAR(map[12], 73, eps);
    TEST_NEAR(map[13], 82, eps);
    TEST_NEAR(map[14], 91, eps);
    TEST_NEAR(map[15], 55, eps);
    TEST_NEAR(map[16], 6, eps);
    TEST_NEAR(map[17], 2, eps);
    TEST_NEAR(map[18], -2, eps);
    TEST_NEAR(map[19], -6, eps);
    TEST_NEAR(map[20], -10, eps);
    TEST_NEAR(map[21], -10, eps);

    TRY(map.erase(5, 15));
    TEST_EQUAL(map.min(), 20);
    TEST_EQUAL(map.max(), 20);
    TEST_NEAR(map[4], -10, eps);
    TEST_NEAR(map[5], -10, eps);
    TEST_NEAR(map[6], -10, eps);
    TEST_NEAR(map[7], -10, eps);
    TEST_NEAR(map[8], -10, eps);
    TEST_NEAR(map[9], -10, eps);
    TEST_NEAR(map[10], -10, eps);
    TEST_NEAR(map[11], -10, eps);
    TEST_NEAR(map[12], -10, eps);
    TEST_NEAR(map[13], -10, eps);
    TEST_NEAR(map[14], -10, eps);
    TEST_NEAR(map[15], -10, eps);
    TEST_NEAR(map[16], -10, eps);
    TEST_NEAR(map[17], -10, eps);
    TEST_NEAR(map[18], -10, eps);
    TEST_NEAR(map[19], -10, eps);
    TEST_NEAR(map[20], -10, eps);
    TEST_NEAR(map[21], -10, eps);

    TRY(map.erase(0, 100));
    TEST(map.empty());
    TEST_EQUAL(map.min(), 0);
    TEST_EQUAL(map.max(), 0);
    TEST_NEAR(map[0], 0, eps);
    TEST_NEAR(map[10], 0, eps);

    TRY((map = {
        {10, 100},
        {20, 200, 300, 400},
        {30, 500, 600},
    }));
    TEST(! map.empty());
    TEST_EQUAL(map.min(), 10);
    TEST_EQUAL(map.max(), 30);
    TEST_NEAR(map[5], 100, eps);
    TEST_NEAR(map[10], 100, eps);
    TEST_NEAR(map[15], 150, eps);
    TEST_NEAR(map[20], 300, eps);
    TEST_NEAR(map[25], 450, eps);
    TEST_NEAR(map[30], 550, eps);
    TEST_NEAR(map[35], 600, eps);

}
