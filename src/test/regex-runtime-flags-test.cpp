#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_regex_runtime_flags() {

    Regex r;
    Regex::match m;
    std::string s;

    // anchor = Anchor match at start of subject range

    TRY(r = Regex("[a-z]+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::anchor));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s, 0, Regex::anchor));
    TEST(m);
    TEST_EQUAL(m.str(), "hello");

    // full = Anchor match at both ends, matching complete string

    TRY(r = Regex("[a-z ]+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(m);
    TEST_EQUAL(m.str(), "hello world");
    s = "";
    TRY(m = r(s, 0, Regex::full));
    TEST(! m);

    TRY(r = Regex("([a-z ]+)?"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(m);
    TEST_EQUAL(m.str(), "hello world");
    TEST_EQUAL(m[1], "hello world");
    s = "";
    TRY(m = r(s, 0, Regex::full));
    TEST(m);
    TEST_EQUAL(m.str(), "");
    TEST_EQUAL(m[1], "");

    TRY(r = Regex("(?:[a-z ]+)?", Regex::full));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s, 0, Regex::full));
    TEST(m);
    TEST_EQUAL(m.str(), "hello world");
    s = "";
    TRY(m = r(s, 0, Regex::full));
    TEST(m);
    TEST_EQUAL(m.str(), "");

    // not_empty = Do not match empty string
    // not_empty_start = Do not match empty string at start of subject string

    TRY(r = Regex("\\b[A-Z]*(?=[a-z])"));
    s = "hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.offset(), 0u);
    TEST_EQUAL(m.count(), 0u);
    TEST_EQUAL(m.str(), "");
    TRY(m = r(s, 0, Regex::not_empty));
    TEST(! m);
    TRY(m = r(s, 0, Regex::not_empty_start));
    TEST(m);
    TEST_EQUAL(m.offset(), 6u);
    TEST_EQUAL(m.count(), 0u);
    TEST_EQUAL(m.str(), "");

    // not_line = Do not match ^ at start or $ at end

    TRY(r = Regex("^[A-Z][a-z]+", Regex::multiline));
    s = "Hello\nGoodbye";
    TRY(m = r(s, 0, Regex::not_line));
    TEST(m);
    TEST_EQUAL(m.str(), "Goodbye");
    TRY(r = Regex("[A-Z][a-z]+$", Regex::multiline));
    TRY(m = r(s, 0, Regex::not_line));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello");

    // partial_hard = Hard partial match, prefer partial to full match

    TRY(r = Regex("\\b[a-z]+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::partial_hard));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");
    TRY(r = Regex("\\b[a-z]+\\d+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::partial_hard));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");

    // partial_soft = Soft partial match, prefer full to partial match

    TRY(r = Regex("\\b[a-z]+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::partial_soft));
    TEST(m);
    TEST(m.full());
    TEST(! m.partial());
    TEST_EQUAL(m.str(), "world");
    TRY(r = Regex("\\b[a-z]+\\d+"));
    s = "Hello world";
    TRY(m = r(s, 0, Regex::partial_soft));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");

}
