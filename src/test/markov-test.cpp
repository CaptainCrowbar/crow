#include "crow/markov.hpp"
#include "crow/unit-test.hpp"
#include <random>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_markov_character_mode() {

    CMarkov m;
    std::minstd_rand rng(42);
    std::string s;

    TRY(m.add("aba"));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = m(rng));
        TEST_EQUAL(s, "aba");
    }

    TRY(m = {});
    TRY(m.add("ababa"));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = m(rng));
        TEST_MATCH(s, "^a(ba)*$");
    }

    TRY(m = CMarkov(2, 7, 10));
    TRY(m.add("ababa"));

    for (int i = 0; i < 100; ++i) {
        TRY(s = m(rng));
        TEST(s.size() == 7u || s.size() == 9u);
        TEST_MATCH(s, "^a(ba)*$");
    }

    TRY(m = CMarkov(2, 0, 4));
    TRY(m.add("ababa"));

    for (int i = 0; i < 100; ++i) {
        TRY(s = m(rng));
        TEST_EQUAL(s, "aba");
    }

}

void test_crow_markov_string_mode() {

    using V = std::vector<std::string>;

    SMarkov m;
    std::minstd_rand rng(42);
    V v;

    TRY(m.add({"alpha", "bravo", "alpha"}));

    for (int i = 0; i < 1000; ++i) {
        TRY(v = m(rng));
        TEST((v == V{"alpha", "bravo", "alpha"}));
    }

    TRY(m = {});
    TRY(m.add({"alpha", "bravo", "alpha", "bravo", "alpha"}));

    for (int i = 0; i < 1000; ++i) {
        TRY(v = m(rng));
        TEST(v.size() % 2 == 1u);
        for (size_t j = 0; j < v.size(); ++j) {
            if (j % 2 == 0u) {
                TEST_EQUAL(v[j], "alpha");
            } else {
                TEST_EQUAL(v[j], "bravo");
            }
        }
    }

    TRY(m = SMarkov(2, 7, 10));
    TRY(m.add({"alpha", "bravo", "alpha", "bravo", "alpha"}));

    for (int i = 0; i < 100; ++i) {
        TRY(v = m(rng));
        TEST(v.size() == 7 || v.size() == 9);
        for (size_t j = 0; j < v.size(); ++j) {
            if (j % 2 == 0u) {
                TEST_EQUAL(v[j], "alpha");
            } else {
                TEST_EQUAL(v[j], "bravo");
            }
        }
    }

    TRY(m = SMarkov(2, 0, 4));
    TRY(m.add({"alpha", "bravo", "alpha", "bravo", "alpha"}));

    for (int i = 0; i < 100; ++i) {
        TRY(v = m(rng));
        TEST(v.size() == 3);
        TEST((v == V{"alpha", "bravo", "alpha"}));
    }

}
