#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <compare>
#include <optional>

using namespace Crow;
using namespace std::literals;

void test_crow_format_null_values() {

    static constexpr const char* cptr0 = nullptr;
    static constexpr const char* cptr1 = "Hello";

    TEST_EQUAL(format_object(0,         "n"),    "0");
    TEST_EQUAL(format_object(0,         "nZ"),   "--");
    TEST_EQUAL(format_object(42,        "n"),    "42");
    TEST_EQUAL(format_object(42,        "nZ"),   "42");
    TEST_EQUAL(format_object(0,         "RZ"),   "--");
    TEST_EQUAL(format_object(42,        "R"),    "XLII");
    TEST_EQUAL(format_object(42,        "RZ"),   "XLII");
    TEST_EQUAL(format_object(0.0,       "f4"),   "0.0000");
    TEST_EQUAL(format_object(0.0,       "fZ4"),  "--");
    TEST_EQUAL(format_object(42.0,      "f4"),   "42.0000");
    TEST_EQUAL(format_object(42.0,      "fZ4"),  "42.0000");
    TEST_EQUAL(format_object('\0',      "s"),    "\0"s);
    TEST_EQUAL(format_object('\0',      "sZ"),   "--");
    TEST_EQUAL(format_object('X',       "s"),    "X");
    TEST_EQUAL(format_object('X',       "sZ"),   "X");
    TEST_EQUAL(format_object(U'\0',     "s"),    "\0"s);
    TEST_EQUAL(format_object(U'\0',     "sZ"),   "--");
    TEST_EQUAL(format_object(U'α',      "s"),    "α");
    TEST_EQUAL(format_object(U'α',      "sZ"),   "α");
    TEST_EQUAL(format_object(""s,       "s"),    "");
    TEST_EQUAL(format_object(""s,       "sZ"),   "--");
    TEST_EQUAL(format_object("Hello"s,  "s"),    "Hello");
    TEST_EQUAL(format_object("Hello"s,  "sZ"),   "Hello");
    TEST_EQUAL(format_object(cptr0,     "s"),    "<null>");
    TEST_EQUAL(format_object(cptr0,     "sZ"),   "--");
    TEST_EQUAL(format_object(cptr1,     "s"),    "Hello");
    TEST_EQUAL(format_object(cptr1,     "sZ"),   "Hello");
    TEST_EQUAL(format_object(nullptr,   "s"),    "<null>"s);
    TEST_EQUAL(format_object(nullptr,   "sZ"),   "--");

}

void test_crow_format_std_ordering() {

    TEST_EQUAL(format_object(std::partial_ordering::less),        "less");
    TEST_EQUAL(format_object(std::partial_ordering::equivalent),  "equivalent");
    TEST_EQUAL(format_object(std::partial_ordering::greater),     "greater");
    TEST_EQUAL(format_object(std::partial_ordering::unordered),   "unordered");
    TEST_EQUAL(format_object(std::strong_ordering::less),         "less");
    TEST_EQUAL(format_object(std::strong_ordering::equal),        "equal");
    TEST_EQUAL(format_object(std::strong_ordering::greater),      "greater");
    TEST_EQUAL(format_object(std::weak_ordering::less),           "less");
    TEST_EQUAL(format_object(std::weak_ordering::equivalent),     "equivalent");
    TEST_EQUAL(format_object(std::weak_ordering::greater),        "greater");

}

void test_crow_format_std_optional() {

    std::optional<int> oi;

    oi = {};  TEST_EQUAL(format_object(oi),        "<null>");
    oi = {};  TEST_EQUAL(format_object(oi, "NZ"),  "--");
    oi = 42;  TEST_EQUAL(format_object(oi),        "42");
    oi = 42;  TEST_EQUAL(format_object(oi, "NZ"),  "42");

}
