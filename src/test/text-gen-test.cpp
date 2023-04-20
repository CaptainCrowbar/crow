#include "crow/text-gen.hpp"
#include "crow/random.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <string>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_text_generation_null() {

    TextGen gen;
    StdRng rng(42);
    std::string s;

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_EQUAL(s, "");
    }

}

void test_crow_text_generation_constant() {

    TextGen gen;
    StdRng rng(42);
    std::string s;

    TRY(gen = "hello");

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_EQUAL(s, "hello");
    }

    TRY(gen = U'α');

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_EQUAL(s, "α");
    }

}

void test_crow_text_generation_number() {

    TextGen gen;
    StdRng rng(42);
    std::string s;

    TRY(gen = TextGen::number(100, 299));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^[12]\\d\\d$");
    }

}

void test_crow_text_generation_select() {

    TextGen gen;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(gen = TextGen::choice({"abc", "def", "ghi"}));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc|def|ghi)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["abc"], 333, 50);
    TEST_NEAR(census["def"], 333, 50);
    TEST_NEAR(census["ghi"], 333, 50);

    TextList list = {"rst", "uvw", "xyz"};
    TRY(gen = TextGen::choice(list));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(rst|uvw|xyz)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["rst"], 333, 50);
    TEST_NEAR(census["uvw"], 333, 50);
    TEST_NEAR(census["xyz"], 333, 50);

    TRY(gen = TextGen("abc") | "def" | "ghi" | "jkl");
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc|def|ghi|jkl)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["abc"], 250, 50);
    TEST_NEAR(census["def"], 250, 50);
    TEST_NEAR(census["ghi"], 250, 50);
    TEST_NEAR(census["jkl"], 250, 50);

    TRY(gen = TextGen::choice("αβγδε"));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(α|β|γ|δ|ε)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 5u);
    TEST_NEAR(census["α"], 200, 50);
    TEST_NEAR(census["β"], 200, 50);
    TEST_NEAR(census["γ"], 200, 50);
    TEST_NEAR(census["δ"], 200, 50);
    TEST_NEAR(census["ε"], 200, 50);

}

void test_crow_text_generation_weighted() {

    TextGen gen;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(gen = TextGen::choice({
        { "abc", 1 },
        { "def", 2 },
        { "ghi", 3 },
        { "jkl", 4 },
    }));


    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc|def|ghi|jkl)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["abc"], 100, 50);
    TEST_NEAR(census["def"], 200, 50);
    TEST_NEAR(census["ghi"], 300, 50);
    TEST_NEAR(census["jkl"], 400, 50);

    TextWeights weights = {
        { "opq", 1 },
        { "rst", 2 },
        { "uvw", 3 },
        { "xyz", 4 },
    };

    TRY(gen = TextGen::choice(weights));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(opq|rst|uvw|xyz)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["opq"], 100, 50);
    TEST_NEAR(census["rst"], 200, 50);
    TEST_NEAR(census["uvw"], 300, 50);
    TEST_NEAR(census["xyz"], 400, 50);

}

void test_crow_text_generation_sequence() {

    TextGen gen, u, v;
    StdRng rng(42);
    std::string s;

    TRY(gen = TextGen::choice({"abc", "def", "ghi"}));
    TRY(u = TextGen::choice({"123", "456", "789"}));
    TRY(v = gen + u);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = v(rng));
        TEST_MATCH(s, "^(abc|def|ghi)(123|456|789)$");
    }

    TRY(gen += u);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc|def|ghi)(123|456|789)$");
    }

}

void test_crow_text_generation_optional() {

    TextGen gen, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = "abc");
    TRY(gen = u % 0.75);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc)?$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 2u);
    TEST_NEAR(census["abc"], 750, 50);
    TEST_NEAR(census[""], 250, 50);

    TRY(u %= 0.75);
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = u(rng));
        TEST_MATCH(s, "^(abc)?$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 2u);
    TEST_NEAR(census["abc"], 750, 50);
    TEST_NEAR(census[""], 250, 50);

}

void test_crow_text_generation_repeat() {

    TextGen gen, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = "abc");
    TRY(gen = u * 3);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_EQUAL(s, "abcabcabc");
    }

    TRY(u = "xyz");
    TRY(gen = u * 1 * 5);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(xyz){1,5}$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 5u);
    TEST_NEAR(census["xyz"], 200, 50);
    TEST_NEAR(census["xyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyzxyzxyz"], 200, 50);

    TRY(u *= 3);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = u(rng));
        TEST_EQUAL(s, "xyzxyzxyz");
    }

}

void test_crow_text_generation_transform() {

    TextGen gen, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = TextGen("abc") | "def" | "ghi");
    TRY(gen = u >> ascii_uppercase);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(ABC|DEF|GHI)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["ABC"], 333, 50);
    TEST_NEAR(census["DEF"], 333, 50);
    TEST_NEAR(census["GHI"], 333, 50);

    TRY(gen = u >> [] (auto& s) { return "(" + s + ")"; });
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^\\((abc|def|ghi)\\)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["(abc)"], 333, 50);
    TEST_NEAR(census["(def)"], 333, 50);
    TEST_NEAR(census["(ghi)"], 333, 50);

    TRY(u >>= ascii_uppercase);
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = u(rng));
        TEST_MATCH(s, "^(ABC|DEF|GHI)$");
        ++census[s];
    }

}

void test_crow_text_generation_casing() {

    TextGen gen;
    StdRng rng(42);
    std::string s;

    TRY(gen = "hello and goodbye");

    TRY(gen.set(TextGen::option::none));      TRY(s = gen(rng));  TEST_EQUAL(s, "hello and goodbye");
    TRY(gen.set(TextGen::option::lower));     TRY(s = gen(rng));  TEST_EQUAL(s, "hello and goodbye");
    TRY(gen.set(TextGen::option::upper));     TRY(s = gen(rng));  TEST_EQUAL(s, "HELLO AND GOODBYE");
    TRY(gen.set(TextGen::option::title));     TRY(s = gen(rng));  TEST_EQUAL(s, "Hello And Goodbye");
    TRY(gen.set(TextGen::option::xtitle));    TRY(s = gen(rng));  TEST_EQUAL(s, "Hello and Goodbye");
    TRY(gen.set(TextGen::option::sentence));  TRY(s = gen(rng));  TEST_EQUAL(s, "Hello and goodbye");

    TRY(gen = "HELLO AND GOODBYE");

    TRY(gen.set(TextGen::option::none));      TRY(s = gen(rng));  TEST_EQUAL(s, "HELLO AND GOODBYE");
    TRY(gen.set(TextGen::option::lower));     TRY(s = gen(rng));  TEST_EQUAL(s, "hello and goodbye");
    TRY(gen.set(TextGen::option::upper));     TRY(s = gen(rng));  TEST_EQUAL(s, "HELLO AND GOODBYE");
    TRY(gen.set(TextGen::option::title));     TRY(s = gen(rng));  TEST_EQUAL(s, "Hello And Goodbye");
    TRY(gen.set(TextGen::option::xtitle));    TRY(s = gen(rng));  TEST_EQUAL(s, "Hello and Goodbye");
    TRY(gen.set(TextGen::option::sentence));  TRY(s = gen(rng));  TEST_EQUAL(s, "HELLO AND GOODBYE");

}

void test_crow_text_generation_literals() {

    TextGen gen;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(gen = "abc"_tg);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_EQUAL(s, "abc");
    }

    TRY(gen = "abc def ghi"_tg);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^(abc|def|ghi)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["abc"], 333, 50);
    TEST_NEAR(census["def"], 333, 50);
    TEST_NEAR(census["ghi"], 333, 50);

    TRY(gen = 5_tg);
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = gen(rng));
        TEST_MATCH(s, "^[1-5]$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 5u);
    TEST_NEAR(census["1"], 200, 50);
    TEST_NEAR(census["2"], 200, 50);
    TEST_NEAR(census["3"], 200, 50);
    TEST_NEAR(census["4"], 200, 50);
    TEST_NEAR(census["5"], 200, 50);

}
