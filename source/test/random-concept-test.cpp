#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"
#include <random>

using namespace Crow;

void test_crow_random_concepts() {

    TEST(RandomEngineType<std::minstd_rand>);
    TEST(RandomEngineType<std::mt19937>);
    TEST(RandomEngineType<std::mt19937_64>);
    TEST(RandomEngineType<std::random_device>);
    TEST(RandomEngineType<Lcg32>);
    TEST(RandomEngineType<Lcg64>);
    TEST(RandomEngineType<Pcg64>);
    TEST(RandomEngineType<Xoshiro>);
    TEST(RandomEngineType<StdRng>);

}
