#include "crow/string.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_string_case_conversion() {

    TEST_EQUAL(ascii_uppercase(""),             "");
    TEST_EQUAL(ascii_uppercase("Hello World"),  "HELLO WORLD");
    TEST_EQUAL(ascii_lowercase(""),             "");
    TEST_EQUAL(ascii_lowercase("Hello World"),  "hello world");
    TEST_EQUAL(ascii_titlecase(""),             "");
    TEST_EQUAL(ascii_titlecase("HELLO WORLD"),  "Hello World");
    TEST_EQUAL(ascii_titlecase("hello world"),  "Hello World");
    TEST_EQUAL(ascii_titlecase("fool's gold"),  "Fool's Gold");

}
