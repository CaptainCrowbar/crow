#include "crow/random-engines.hpp"
#include "crow/fixed-binary.hpp"
#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"
#include <cmath>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_random_lcg_32() {

    static constexpr int bits = 32;
    static constexpr int iterations = 1'000'000;
    static constexpr double max = double(~ 0_u32);
    static constexpr double mean = max / 2;
    static const double sd = std::sqrt((std::ldexp(1.0, 2 * bits) - 1) / 12);
    static const double epsilon = 2 * mean / std::sqrt(double(iterations));

    Lcg32 rng(42);
    uint32_t x = 0;
    double y = 0;
    Statistics<double> stats;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng());
        y = double(x);
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean, epsilon);
    TEST_NEAR(stats.sd(), sd, epsilon);

}

void test_crow_random_lcg_64() {

    static constexpr int bits = 64;
    static constexpr int iterations = 1'000'000;
    static constexpr double max = double(~ 0_u64);
    static constexpr double mean = max / 2;
    static const double sd = std::sqrt((std::ldexp(1.0, 2 * bits) - 1) / 12);
    static const double epsilon = 2 * mean / std::sqrt(double(iterations));

    Lcg64 rng(42);
    uint64_t x = 0;
    double y = 0;
    Statistics<double> stats;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng());
        y = double(x);
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean, epsilon);
    TEST_NEAR(stats.sd(), sd, epsilon);

}

void test_crow_random_lcg_128() {

    static constexpr int bits = 128;
    static constexpr int iterations = 1'000'000;
    static constexpr double max = double(~ Uint128(0));
    static constexpr double mean = max / 2;
    static const double sd = std::sqrt((std::ldexp(1.0, 2 * bits) - 1) / 12);
    static const double epsilon = 2 * mean / std::sqrt(double(iterations));

    Lcg128 rng(42);
    Uint128 x = 0;
    double y = 0;
    Statistics<double> stats;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng());
        y = double(x);
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean, epsilon);
    TEST_NEAR(stats.sd(), sd, epsilon);

    rng.seed(86, 99);
    x = 0;
    y = 0;
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng());
        y = double(x);
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean, epsilon);
    TEST_NEAR(stats.sd(), sd, epsilon);

}
