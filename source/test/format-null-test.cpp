#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>

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
