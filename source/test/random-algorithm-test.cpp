#include "crow/random-algorithms.hpp"
#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace Crow;

void test_crow_random_sample() {

    StdRng rng(42);
    std::vector<int> u(100);
    std::iota(u.begin(), u.end(), 0);
    std::vector<int> v, w;

    for (size_t k = 5; k < 100; k += 5) {
        for (int i = 0; i < 100; ++i) {
            w = v;
            TRY(v = sample(u, rng, k));
            TEST_EQUAL(v.size(), k);
            for (auto x: v)
                TEST_IN_RANGE(x, 0, 99);
            std::sort(v.begin(), v.end());
            TEST(v != w);
            for (size_t i = 0; i + 1 < k; ++i)
                TEST(v[i] < v[i + 1]);
        }
    }

}

void test_crow_random_shuffle() {

    StdRng rng(42);
    std::vector<int> u(10);
    std::iota(u.begin(), u.end(), 0);
    std::vector<int> v = u;
    std::vector<int> w;

    for (int i = 0; i < 1000; ++i) {
        w = v;
        TRY(shuffle(v, rng));
        TEST(v != w);
        w = v;
        std::sort(w.begin(), w.end());
        TEST_EQUAL_RANGES(w, u);
    }

}
