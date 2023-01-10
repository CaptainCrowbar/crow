#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

namespace {

    template <typename RNG>
    void seed_from_device_test() {

        RNG rng;
        auto prev = rng();
        auto next = prev;

        for (int i = 0; i < 1000; ++i) {
            TRY(seed_from_device(rng));
            TRY(next = rng());
            TEST(next != prev);
            prev = next;
        }

    }

}

void test_crow_random_seed_from_device() {

    seed_from_device_test<Lcg32>();
    seed_from_device_test<Lcg64>();
    seed_from_device_test<Lcg128>();
    seed_from_device_test<Pcg64>();
    seed_from_device_test<Xoshiro>();

}
