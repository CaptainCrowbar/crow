#include "crow/random-engines.hpp"
#include "crow/random-other-distributions.hpp"
#include "crow/uuid.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <string>

using namespace Crow;

void test_crow_random_choice_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    RandomChoice<char> choice;
    std::map<char, int> census;
    std::string alpha = "abcdefghij";
    char c;

    TRY(choice = random_choice(alpha));
    TEST_EQUAL(choice.size(), 10u);

    for (int i = 0; i < iterations; ++i) {
        TRY(c = choice(rng));
        ++census[c];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census['a'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['b'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['c'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['d'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['e'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['f'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['g'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['h'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['i'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['j'] / double(iterations), 0.1, 0.001);

    TRY(choice = random_choice({'v', 'w', 'x', 'y', 'z'}));
    TEST_EQUAL(choice.size(), 5u);
    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(c = choice(rng));
        ++census[c];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census['v'] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census['w'] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census['x'] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census['y'] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census['z'] / double(iterations), 0.2, 0.001);

}

void test_crow_random_weighted_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    WeightedChoice<std::string> wd;
    std::map<std::string, int> census;

    TEST_EQUAL(wd.total_weight(), 0);

    TRY(wd.add(1, "a"));
    TRY(wd.add(2, "b"));
    TRY(wd.add(3, "c"));
    TRY(wd.add(4, "d"));
    TRY(wd.add(5, "e"));
    TRY(wd.add(1, "f", "g", "h", "i", "j"));

    TEST_EQUAL(wd.total_weight(), 16);

    const std::string* sp = nullptr;

    for (int i = 0; i < iterations; ++i) {
        TRY(sp = &wd(rng));
        ++census[*sp];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census["a"] / double(iterations), 0.0625, 0.001);
    TEST_NEAR(census["b"] / double(iterations), 0.1250, 0.001);
    TEST_NEAR(census["c"] / double(iterations), 0.1875, 0.001);
    TEST_NEAR(census["d"] / double(iterations), 0.2500, 0.001);
    TEST_NEAR(census["e"] / double(iterations), 0.3125, 0.001);
    TEST_NEAR(census["f"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["g"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["h"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["i"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["j"] / double(iterations), 0.0125, 0.001);

    TRY((wd = {
        {10, "A"},
        {20, "B"},
        {30, "C"},
        {40, "D"},
        {50, "E"},
        {10, "F", "G", "H", "I", "J"},
    }));

    TEST_EQUAL(wd.total_weight(), 160);

    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(sp = &wd(rng));
        ++census[*sp];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census["A"] / double(iterations), 0.0625, 0.001);
    TEST_NEAR(census["B"] / double(iterations), 0.1250, 0.001);
    TEST_NEAR(census["C"] / double(iterations), 0.1875, 0.001);
    TEST_NEAR(census["D"] / double(iterations), 0.2500, 0.001);
    TEST_NEAR(census["E"] / double(iterations), 0.3125, 0.001);
    TEST_NEAR(census["F"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["G"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["H"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["I"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["J"] / double(iterations), 0.0125, 0.001);

}

void test_crow_random_uuid() {

    static constexpr int iterations = 1000;

    Pcg64 rng(42);
    RandomUuid ru;
    Uuid u;
    std::string s;

    for (int i = 0; i < iterations; ++i) {
        TRY(u = ru(rng));
        TRY(s = u.str());
        TEST_MATCH(s,
            "^[0-9a-f]{8}-"
            "[0-9a-f]{4}-"
            "4[0-9a-f]{3}-"
            "[89ab][0-9a-f]{3}-"
            "[0-9a-f]{12}$"
        );
    }

}
