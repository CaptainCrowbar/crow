#include "crow/format-integer.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_format_signed_integers() {

    TEST_EQUAL(format_integer(0,           ""),     "0");
    TEST_EQUAL(format_integer(42,          ""),     "42");
    TEST_EQUAL(format_integer(-42,         ""),     "-42");
    TEST_EQUAL(format_integer(123456789,   ""),     "123456789");
    TEST_EQUAL(format_integer(-123456789,  ""),     "-123456789");
    TEST_EQUAL(format_integer(0,           "n"),    "0");
    TEST_EQUAL(format_integer(42,          "n"),    "42");
    TEST_EQUAL(format_integer(-42,         "n"),    "-42");
    TEST_EQUAL(format_integer(123456789,   "n"),    "123456789");
    TEST_EQUAL(format_integer(-123456789,  "n"),    "-123456789");
    TEST_EQUAL(format_integer(0,           "ns"),   "+0");
    TEST_EQUAL(format_integer(42,          "ns"),   "+42");
    TEST_EQUAL(format_integer(-42,         "ns"),   "-42");
    TEST_EQUAL(format_integer(123456789,   "ns"),   "+123456789");
    TEST_EQUAL(format_integer(-123456789,  "ns"),   "-123456789");
    TEST_EQUAL(format_integer(0,           "n4"),   "0000");
    TEST_EQUAL(format_integer(42,          "n4"),   "0042");
    TEST_EQUAL(format_integer(-42,         "n4"),   "-0042");
    TEST_EQUAL(format_integer(123456789,   "n4"),   "123456789");
    TEST_EQUAL(format_integer(-123456789,  "n4"),   "-123456789");
    TEST_EQUAL(format_integer(0,           "n0"),   "");
    TEST_EQUAL(format_integer(42,          "n0"),   "42");
    TEST_EQUAL(format_integer(-42,         "n0"),   "-42");
    TEST_EQUAL(format_integer(0,           "x"),    "0");
    TEST_EQUAL(format_integer(42,          "x"),    "2a");
    TEST_EQUAL(format_integer(-42,         "x"),    "-2a");
    TEST_EQUAL(format_integer(123456789,   "x"),    "75bcd15");
    TEST_EQUAL(format_integer(-123456789,  "x"),    "-75bcd15");
    TEST_EQUAL(format_integer(0,           "x4"),   "0000");
    TEST_EQUAL(format_integer(42,          "x4"),   "002a");
    TEST_EQUAL(format_integer(-42,         "x4"),   "-002a");
    TEST_EQUAL(format_integer(123456789,   "x4"),   "75bcd15");
    TEST_EQUAL(format_integer(-123456789,  "x4"),   "-75bcd15");
    TEST_EQUAL(format_integer(0,           "X"),    "0");
    TEST_EQUAL(format_integer(42,          "X"),    "2A");
    TEST_EQUAL(format_integer(-42,         "X"),    "-2A");
    TEST_EQUAL(format_integer(123456789,   "X"),    "75BCD15");
    TEST_EQUAL(format_integer(-123456789,  "X"),    "-75BCD15");
    TEST_EQUAL(format_integer(0,           "b"),    "0");
    TEST_EQUAL(format_integer(42,          "b"),    "101010");
    TEST_EQUAL(format_integer(-42,         "b"),    "-101010");
    TEST_EQUAL(format_integer(123456789,   "b"),    "111010110111100110100010101");
    TEST_EQUAL(format_integer(-123456789,  "b"),    "-111010110111100110100010101");
    TEST_EQUAL(format_integer(0,           "b16"),  "0000000000000000");
    TEST_EQUAL(format_integer(42,          "b16"),  "0000000000101010");
    TEST_EQUAL(format_integer(-42,         "b16"),  "-0000000000101010");
    TEST_EQUAL(format_integer(123456789,   "b16"),  "111010110111100110100010101");
    TEST_EQUAL(format_integer(-123456789,  "b16"),  "-111010110111100110100010101");
    TEST_EQUAL(format_integer(1,           "R"),    "I");
    TEST_EQUAL(format_integer(42,          "R"),    "XLII");
    TEST_EQUAL(format_integer(1111,        "R"),    "MCXI");
    TEST_EQUAL(format_integer(9999,        "R"),    "MMMMMMMMMCMXCIX");
    TEST_EQUAL(format_integer(42,          "r"),    "xlii");
    TEST_EQUAL(format_integer(0,           4),      "0000");
    TEST_EQUAL(format_integer(42,          4),      "0042");
    TEST_EQUAL(format_integer(-42,         4),      "-0042");
    TEST_EQUAL(format_integer(123456789,   4),      "123456789");
    TEST_EQUAL(format_integer(-123456789,  4),      "-123456789");
    TEST_EQUAL(format_integer(0,           0),      "");
    TEST_EQUAL(format_integer(42,          0),      "42");
    TEST_EQUAL(format_integer(-42,         0),      "-42");

}

void test_crow_format_unsigned_integers() {

    TEST_EQUAL(format_integer(0u,          ""),     "0");
    TEST_EQUAL(format_integer(42u,         ""),     "42");
    TEST_EQUAL(format_integer(123456789u,  ""),     "123456789");
    TEST_EQUAL(format_integer(0u,          "n"),    "0");
    TEST_EQUAL(format_integer(42u,         "n"),    "42");
    TEST_EQUAL(format_integer(123456789u,  "n"),    "123456789");
    TEST_EQUAL(format_integer(0u,          "ns"),   "+0");
    TEST_EQUAL(format_integer(42u,         "ns"),   "+42");
    TEST_EQUAL(format_integer(123456789u,  "ns"),   "+123456789");
    TEST_EQUAL(format_integer(0u,          "n4"),   "0000");
    TEST_EQUAL(format_integer(42u,         "n4"),   "0042");
    TEST_EQUAL(format_integer(123456789u,  "n4"),   "123456789");
    TEST_EQUAL(format_integer(0u,          "n0"),   "");
    TEST_EQUAL(format_integer(42u,         "n0"),   "42");
    TEST_EQUAL(format_integer(0u,          "x"),    "0");
    TEST_EQUAL(format_integer(42u,         "x"),    "2a");
    TEST_EQUAL(format_integer(123456789u,  "x"),    "75bcd15");
    TEST_EQUAL(format_integer(0u,          "x4"),   "0000");
    TEST_EQUAL(format_integer(42u,         "x4"),   "002a");
    TEST_EQUAL(format_integer(123456789u,  "x4"),   "75bcd15");
    TEST_EQUAL(format_integer(0u,          "X"),    "0");
    TEST_EQUAL(format_integer(42u,         "X"),    "2A");
    TEST_EQUAL(format_integer(123456789u,  "X"),    "75BCD15");
    TEST_EQUAL(format_integer(0u,          "b"),    "0");
    TEST_EQUAL(format_integer(42u,         "b"),    "101010");
    TEST_EQUAL(format_integer(123456789u,  "b"),    "111010110111100110100010101");
    TEST_EQUAL(format_integer(0u,          "b16"),  "0000000000000000");
    TEST_EQUAL(format_integer(42u,         "b16"),  "0000000000101010");
    TEST_EQUAL(format_integer(123456789u,  "b16"),  "111010110111100110100010101");
    TEST_EQUAL(format_integer(1u,          "R"),    "I");
    TEST_EQUAL(format_integer(42u,         "R"),    "XLII");
    TEST_EQUAL(format_integer(1111u,       "R"),    "MCXI");
    TEST_EQUAL(format_integer(9999u,       "R"),    "MMMMMMMMMCMXCIX");
    TEST_EQUAL(format_integer(42u,         "r"),    "xlii");
    TEST_EQUAL(format_integer(0u,          4),      "0000");
    TEST_EQUAL(format_integer(42u,         4),      "0042");
    TEST_EQUAL(format_integer(123456789u,  4),      "123456789");
    TEST_EQUAL(format_integer(0u,          0),      "");
    TEST_EQUAL(format_integer(42u,         0),      "42");

}
