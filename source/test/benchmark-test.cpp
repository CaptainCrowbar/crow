#include "crow/benchmark.hpp"
#include "crow/random.hpp"
#include "crow/unit-test.hpp"
#include <chrono>

using namespace Crow;
using namespace std::chrono;

void test_crow_benchmark() {

    Benchmark<> bench;
    Benchmark<>::result_type result;
    StdRng rng(42);

    TRY(result = bench.run([&] { return rng(); }, 100ms));
    TEST(result.average.count() > 0);
    TEST(result.average.count() < 1'000'000);

}
