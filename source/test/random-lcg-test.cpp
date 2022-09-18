#include "crow/random-engines.hpp"
#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"
#include <cmath>

using namespace Crow;

void test_crow_random_lcg_generators() {

    static constexpr int iterations = 1'000'000;

    static constexpr double max32 = double(~ uint32_t(0));
    static constexpr double max64 = double(~ uint64_t(0));
    static constexpr double mean32 = max32 / 2;
    static constexpr double mean64 = max64 / 2;
    static const double sd32 = std::sqrt((std::ldexp(1.0, 64) - 1) / 12);
    static const double sd64 = std::sqrt((std::ldexp(1.0, 128) - 1) / 12);

    Lcg32 rng32(42);
    Statistics<double> stats;
    uint32_t x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng32());
        stats(x);
    }

    TEST_NEAR(stats.mean(), mean32, 1e7);
    TEST_NEAR(stats.sd(), sd32, 1e7);

    Lcg64 rng64(42);
    double y = 0;
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(y = double(rng64()));
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean64, 1e16);
    TEST_NEAR(stats.sd(), sd64, 1e16);

}
