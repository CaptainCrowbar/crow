#include "crow/random-engines.hpp"
#include "crow/random-other-distributions.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>

using namespace Crow;

void test_crow_random_constrained_distribution() {

    using constrained_int = ConstrainedDistribution<UniformInteger<int>>;

    Pcg64 rng(42);
    constrained_int ci(1, 10, -20, 20);
    int min = 9999;
    int max = -9999;
    int x = 0;

    TEST_EQUAL(ci.min(), 1);
    TEST_EQUAL(ci.max(), 10);

    for (int i = 0; i < 100'000; ++i) {
        TRY(x = ci(rng));
        min = std::min(min, x);
        max = std::max(max, x);
    }

    TEST_EQUAL(min, 1);
    TEST_EQUAL(max, 10);

}

void test_crow_random_unique_distribution() {

    static constexpr int iterations = 100'000;

    Pcg64 rng(42);
    std::string alpha = "abcdefghij";
    std::string prev;
    RandomChoice<char> choice(alpha);
    auto unique = UniqueChoice(choice);

    for (int i = 0; i < iterations; ++i) {
        std::string s;
        for (size_t i = 0; i < alpha.size(); ++i)
            TRY(s += unique(rng));
        TEST(s != prev);
        std::sort(s.begin(), s.end());
        TEST_EQUAL(s, alpha);
        prev = s;
        TRY(unique.reset());
    }

}
