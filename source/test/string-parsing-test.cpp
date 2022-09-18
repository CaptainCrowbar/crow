#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <limits>
#include <stdexcept>
#include <string>

using namespace Crow;

void test_crow_string_to_boolean() {

    TEST(to_boolean("TRUE"));
    TEST(to_boolean("True"));
    TEST(to_boolean("true"));
    TEST(to_boolean("T"));
    TEST(to_boolean("YES"));
    TEST(to_boolean("Y"));
    TEST(to_boolean("ON"));
    TEST(to_boolean("1"));
    TEST(! to_boolean("FALSE"));
    TEST(! to_boolean("False"));
    TEST(! to_boolean("false"));
    TEST(! to_boolean("F"));
    TEST(! to_boolean("NO"));
    TEST(! to_boolean("N"));
    TEST(! to_boolean("OFF"));
    TEST(! to_boolean("0"));
    TEST(! to_boolean(""));

    TEST_THROW(to_boolean("tr"), std::invalid_argument);
    TEST_THROW(to_boolean("yess"), std::invalid_argument);
    TEST_THROW(to_boolean("fa"), std::invalid_argument);
    TEST_THROW(to_boolean("falsehood"), std::invalid_argument);
    TEST_THROW(to_boolean("2"), std::invalid_argument);

}

void test_crow_string_to_integer() {

    int max_int = std::numeric_limits<int>::max();
    unsigned max_uint = std::numeric_limits<unsigned>::max();
    std::string int_too_high = std::to_string(unsigned(max_int) + 1);
    std::string int_too_low = "-" + std::to_string(unsigned(max_int) + 2);
    std::string uint_too_high = std::to_string(uint64_t(max_uint) + 1);

    TEST_EQUAL(to_int("0"),           0);
    TEST_EQUAL(to_int("+0"),          0);
    TEST_EQUAL(to_int("-0"),          0);
    TEST_EQUAL(to_int("00000"),       0);
    TEST_EQUAL(to_int("42"),          42);
    TEST_EQUAL(to_int("00042"),       42);
    TEST_EQUAL(to_int("12345"),       12345);
    TEST_EQUAL(to_int("+12345"),      12345);
    TEST_EQUAL(to_int("-12345"),      -12345);
    TEST_EQUAL(to_int("0", 16),       0);
    TEST_EQUAL(to_int("abcd", 16),    43981);
    TEST_EQUAL(to_int("-abcd", 16),   -43981);
    TEST_EQUAL(to_int("0", 2),        0);
    TEST_EQUAL(to_int("101010", 2),   42);
    TEST_EQUAL(to_int("-101010", 2),  -42);

    TEST_THROW(to_int(""), std::invalid_argument);
    TEST_THROW(to_int("   "), std::invalid_argument);
    TEST_THROW(to_int("abc"), std::invalid_argument);
    TEST_THROW(to_int(int_too_high), std::invalid_argument);
    TEST_THROW(to_int(int_too_low), std::invalid_argument);
    TEST_THROW(to_int("0", 3), std::invalid_argument);

    TEST_EQUAL(to_uint("0"),          0u);
    TEST_EQUAL(to_uint("+0"),         0u);
    TEST_EQUAL(to_uint("00000"),      0u);
    TEST_EQUAL(to_uint("42"),         42u);
    TEST_EQUAL(to_uint("00042"),      42u);
    TEST_EQUAL(to_uint("12345"),      12345u);
    TEST_EQUAL(to_uint("+12345"),     12345u);
    TEST_EQUAL(to_uint("0", 16),      0u);
    TEST_EQUAL(to_uint("abcd", 16),   43981u);
    TEST_EQUAL(to_uint("0", 2),       0u);
    TEST_EQUAL(to_uint("101010", 2),  42u);

    TEST_THROW(to_uint(""), std::invalid_argument);
    TEST_THROW(to_uint("   "), std::invalid_argument);
    TEST_THROW(to_uint("abc"), std::invalid_argument);
    TEST_THROW(to_uint("-1"), std::invalid_argument);
    TEST_THROW(to_uint(uint_too_high), std::invalid_argument);
    TEST_THROW(to_uint("0", 3), std::invalid_argument);

}

void test_crow_string_to_floating_point() {

    TEST_EQUAL(to_double("0"),           0);
    TEST_EQUAL(to_double("+0"),          0);
    TEST_EQUAL(to_double("-0"),          0);
    TEST_EQUAL(to_double("00000"),       0);
    TEST_EQUAL(to_double("42"),          42);
    TEST_EQUAL(to_double("00042"),       42);
    TEST_EQUAL(to_double("12345.75"),    12345.75);
    TEST_EQUAL(to_double("+12345.75"),   12345.75);
    TEST_EQUAL(to_double("-12345.75"),   -12345.75);
    TEST_EQUAL(to_double("1.2345e10"),   1.2345e10);
    TEST_EQUAL(to_double("+1.2345e10"),  1.2345e10);
    TEST_EQUAL(to_double("-1.2345e10"),  -1.2345e10);
    TEST_EQUAL(to_double("6.25e-2"),     0.0625);
    TEST_EQUAL(to_double("+6.25e-2"),    0.0625);
    TEST_EQUAL(to_double("-6.25e-2"),    -0.0625);

    TEST_THROW(to_double(""), std::invalid_argument);
    TEST_THROW(to_double("   "), std::invalid_argument);
    TEST_THROW(to_double("abc"), std::invalid_argument);

}
