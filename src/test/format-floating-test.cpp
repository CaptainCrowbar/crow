#include "crow/format-floating.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_format_floating_point_significant_digits_format() {

    TEST_EQUAL(format_floating_point(0,                  "d"),    "0.00000");
    TEST_EQUAL(format_floating_point(0,                  "d0"),   "0");
    TEST_EQUAL(format_floating_point(0,                  "d4"),   "0.000");
    TEST_EQUAL(format_floating_point(100,                "d"),    "100.000");
    TEST_EQUAL(format_floating_point(100,                "d0"),   "100");
    TEST_EQUAL(format_floating_point(100,                "d4"),   "100.0");
    TEST_EQUAL(format_floating_point(100,                "d8"),   "100.00000");
    TEST_EQUAL(format_floating_point(123.456,            "d"),    "123.456");
    TEST_EQUAL(format_floating_point(123.456,            "d0"),   "100");
    TEST_EQUAL(format_floating_point(123.456,            "d4"),   "123.5");
    TEST_EQUAL(format_floating_point(123.456,            "d8"),   "123.45600");
    TEST_EQUAL(format_floating_point(-123.456,           "d"),    "-123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "d0"),   "-100");
    TEST_EQUAL(format_floating_point(-123.456,           "d4"),   "-123.5");
    TEST_EQUAL(format_floating_point(-123.456,           "d8"),   "-123.45600");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "d"),    "123456000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "d0"),   "100000000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "d4"),   "123500000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "d8"),   "123456000");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "d"),    "0.000000123456");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "d0"),   "0.0000001");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "d4"),   "0.0000001235");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "d8"),   "0.00000012345600");
    TEST_EQUAL(format_floating_point(0,                  "ds"),   "+0.00000");
    TEST_EQUAL(format_floating_point(123.456,            "ds"),   "+123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "ds"),   "-123.456");
    TEST_EQUAL(format_floating_point(0,                  "dz"),   "0");
    TEST_EQUAL(format_floating_point(0,                  "dz0"),  "0");
    TEST_EQUAL(format_floating_point(0,                  "dz4"),  "0");
    TEST_EQUAL(format_floating_point(100,                "dz"),   "100");
    TEST_EQUAL(format_floating_point(100,                "dz0"),  "100");
    TEST_EQUAL(format_floating_point(100,                "dz4"),  "100");
    TEST_EQUAL(format_floating_point(100,                "dz8"),  "100");
    TEST_EQUAL(format_floating_point(123.456,            "dz"),   "123.456");
    TEST_EQUAL(format_floating_point(123.456,            "dz0"),  "100");
    TEST_EQUAL(format_floating_point(123.456,            "dz4"),  "123.5");
    TEST_EQUAL(format_floating_point(123.456,            "dz8"),  "123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "dz"),   "-123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "dz0"),  "-100");
    TEST_EQUAL(format_floating_point(-123.456,           "dz4"),  "-123.5");
    TEST_EQUAL(format_floating_point(-123.456,           "dz8"),  "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "dz"),   "123456000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "dz0"),  "100000000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "dz4"),  "123500000");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "dz8"),  "123456000");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "dz"),   "0.000000123456");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "dz0"),  "0.0000001");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "dz4"),  "0.0000001235");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "dz8"),  "0.000000123456");
    TEST_EQUAL(format_floating_point(0,                  "dsz"),  "+0");
    TEST_EQUAL(format_floating_point(123.456,            "dsz"),  "+123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "dsz"),  "-123.456");
    TEST_EQUAL(format_floating_point(0,                  "D"),    "0.00000");
    TEST_EQUAL(format_floating_point(100,                "D"),    "100.000");
    TEST_EQUAL(format_floating_point(123.456,            "D"),    "123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "D"),    "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "D"),    "123456000");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "D"),    "0.000000123456");

}

void test_crow_format_floating_point_scientific_notation() {

    TEST_EQUAL(format_floating_point(0,                  "e"),    "0.00000e0");
    TEST_EQUAL(format_floating_point(0,                  "e0"),   "0e0");
    TEST_EQUAL(format_floating_point(0,                  "e2"),   "0.0e0");
    TEST_EQUAL(format_floating_point(100,                "e"),    "1.00000e2");
    TEST_EQUAL(format_floating_point(100,                "e0"),   "1e2");
    TEST_EQUAL(format_floating_point(100,                "e2"),   "1.0e2");
    TEST_EQUAL(format_floating_point(100,                "e8"),   "1.0000000e2");
    TEST_EQUAL(format_floating_point(123.456,            "e"),    "1.23456e2");
    TEST_EQUAL(format_floating_point(123.456,            "e0"),   "1e2");
    TEST_EQUAL(format_floating_point(123.456,            "e2"),   "1.2e2");
    TEST_EQUAL(format_floating_point(123.456,            "e8"),   "1.2345600e2");
    TEST_EQUAL(format_floating_point(-123.456,           "e"),    "-1.23456e2");
    TEST_EQUAL(format_floating_point(-123.456,           "e0"),   "-1e2");
    TEST_EQUAL(format_floating_point(-123.456,           "e2"),   "-1.2e2");
    TEST_EQUAL(format_floating_point(-123.456,           "e8"),   "-1.2345600e2");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "e"),    "1.23456e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "e0"),   "1e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "e2"),   "1.2e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "e8"),   "1.2345600e8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "e"),    "1.23456e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "e0"),   "1e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "e2"),   "1.2e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "e8"),   "1.2345600e-7");
    TEST_EQUAL(format_floating_point(0,                  "es"),   "+0.00000e0");
    TEST_EQUAL(format_floating_point(123.456,            "es"),   "+1.23456e2");
    TEST_EQUAL(format_floating_point(-123.456,           "es"),   "-1.23456e2");
    TEST_EQUAL(format_floating_point(100,                "ez"),   "1e2");
    TEST_EQUAL(format_floating_point(100,                "ez0"),  "1e2");
    TEST_EQUAL(format_floating_point(100,                "ez2"),  "1e2");
    TEST_EQUAL(format_floating_point(100,                "ez8"),  "1e2");
    TEST_EQUAL(format_floating_point(123.456,            "ez"),   "1.23456e2");
    TEST_EQUAL(format_floating_point(123.456,            "ez0"),  "1e2");
    TEST_EQUAL(format_floating_point(123.456,            "ez2"),  "1.2e2");
    TEST_EQUAL(format_floating_point(123.456,            "ez8"),  "1.23456e2");
    TEST_EQUAL(format_floating_point(0,                  "E"),    "0.00000E0");
    TEST_EQUAL(format_floating_point(100,                "E"),    "1.00000E2");
    TEST_EQUAL(format_floating_point(123.456,            "E"),    "1.23456E2");
    TEST_EQUAL(format_floating_point(-123.456,           "E"),    "-1.23456E2");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "E"),    "1.23456E8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "E"),    "1.23456E-7");
    TEST_EQUAL(format_floating_point(0,                  "ES"),   "0.00000E+0");
    TEST_EQUAL(format_floating_point(100,                "ES"),   "1.00000E+2");
    TEST_EQUAL(format_floating_point(123.456,            "ES"),   "1.23456E+2");
    TEST_EQUAL(format_floating_point(-123.456,           "ES"),   "-1.23456E+2");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "ES"),   "1.23456E+8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "ES"),   "1.23456E-7");

}

void test_crow_format_floating_point_fixed_point_format() {

    TEST_EQUAL(format_floating_point(0,                   "f"),    "0.000000");
    TEST_EQUAL(format_floating_point(0,                   "f0"),   "0");
    TEST_EQUAL(format_floating_point(0,                   "f2"),   "0.00");
    TEST_EQUAL(format_floating_point(100,                 "f"),    "100.000000");
    TEST_EQUAL(format_floating_point(100,                 "f0"),   "100");
    TEST_EQUAL(format_floating_point(100,                 "f2"),   "100.00");
    TEST_EQUAL(format_floating_point(100,                 "f4"),   "100.0000");
    TEST_EQUAL(format_floating_point(123.456,             "f"),    "123.456000");
    TEST_EQUAL(format_floating_point(123.456,             "f0"),   "123");
    TEST_EQUAL(format_floating_point(123.456,             "f2"),   "123.46");
    TEST_EQUAL(format_floating_point(123.456,             "f4"),   "123.4560");
    TEST_EQUAL(format_floating_point(-123.456,            "f"),    "-123.456000");
    TEST_EQUAL(format_floating_point(-123.456,            "f0"),   "-123");
    TEST_EQUAL(format_floating_point(-123.456,            "f2"),   "-123.46");
    TEST_EQUAL(format_floating_point(-123.456,            "f4"),   "-123.4560");
    TEST_EQUAL(format_floating_point(123'456'000.0,       "f"),    "123456000.000000");
    TEST_EQUAL(format_floating_point(-123'456'000.0,      "f"),    "-123456000.000000");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,   "f"),    "0.000000");
    TEST_EQUAL(format_floating_point(-0.000'000'123'456,  "f"),    "0.000000");
    TEST_EQUAL(format_floating_point(0,                   "fs"),   "+0.000000");
    TEST_EQUAL(format_floating_point(123.456,             "fs"),   "+123.456000");
    TEST_EQUAL(format_floating_point(-123.456,            "fs"),   "-123.456000");
    TEST_EQUAL(format_floating_point(100,                 "fz"),   "100");
    TEST_EQUAL(format_floating_point(100,                 "fz0"),  "100");
    TEST_EQUAL(format_floating_point(100,                 "fz2"),  "100");
    TEST_EQUAL(format_floating_point(100,                 "fz4"),  "100");
    TEST_EQUAL(format_floating_point(123.456,             "fz"),   "123.456");
    TEST_EQUAL(format_floating_point(123.456,             "fz0"),  "123");
    TEST_EQUAL(format_floating_point(123.456,             "fz2"),  "123.46");
    TEST_EQUAL(format_floating_point(123.456,             "fz4"),  "123.456");
    TEST_EQUAL(format_floating_point(0,                   "F"),    "0.000000");
    TEST_EQUAL(format_floating_point(100,                 "F"),    "100.000000");
    TEST_EQUAL(format_floating_point(123.456,             "F"),    "123.456000");
    TEST_EQUAL(format_floating_point(-123.456,            "F"),    "-123.456000");

}

void test_crow_format_floating_point_general_format() {

    TEST_EQUAL(format_floating_point(0,                  "g"),    "0.00000");
    TEST_EQUAL(format_floating_point(0,                  "g0"),   "0");
    TEST_EQUAL(format_floating_point(0,                  "g4"),   "0.000");
    TEST_EQUAL(format_floating_point(100,                "g"),    "100.000");
    TEST_EQUAL(format_floating_point(100,                "g0"),   "100");
    TEST_EQUAL(format_floating_point(100,                "g4"),   "100.0");
    TEST_EQUAL(format_floating_point(100,                "g8"),   "100.00000");
    TEST_EQUAL(format_floating_point(123.456,            "g"),    "123.456");
    TEST_EQUAL(format_floating_point(123.456,            "g0"),   "100");
    TEST_EQUAL(format_floating_point(123.456,            "g4"),   "123.5");
    TEST_EQUAL(format_floating_point(123.456,            "g8"),   "123.45600");
    TEST_EQUAL(format_floating_point(-123.456,           "g"),    "-123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "g0"),   "-100");
    TEST_EQUAL(format_floating_point(-123.456,           "g4"),   "-123.5");
    TEST_EQUAL(format_floating_point(-123.456,           "g8"),   "-123.45600");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "g"),    "1.23456e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "g0"),   "1e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "g4"),   "1.235e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "g8"),   "1.2345600e8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "g"),    "1.23456e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "g0"),   "1e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "g4"),   "1.235e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "g8"),   "1.2345600e-7");
    TEST_EQUAL(format_floating_point(0,                  "gs"),   "+0.00000");
    TEST_EQUAL(format_floating_point(123.456,            "gs"),   "+123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "gs"),   "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "gs"),   "+1.23456e8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "gs"),   "+1.23456e-7");
    TEST_EQUAL(format_floating_point(0,                  "gz"),   "0");
    TEST_EQUAL(format_floating_point(0,                  "gz0"),  "0");
    TEST_EQUAL(format_floating_point(0,                  "gz4"),  "0");
    TEST_EQUAL(format_floating_point(100,                "gz"),   "100");
    TEST_EQUAL(format_floating_point(100,                "gz0"),  "100");
    TEST_EQUAL(format_floating_point(100,                "gz4"),  "100");
    TEST_EQUAL(format_floating_point(100,                "gz8"),  "100");
    TEST_EQUAL(format_floating_point(123.456,            "gz"),   "123.456");
    TEST_EQUAL(format_floating_point(123.456,            "gz0"),  "100");
    TEST_EQUAL(format_floating_point(123.456,            "gz4"),  "123.5");
    TEST_EQUAL(format_floating_point(123.456,            "gz8"),  "123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "gz"),   "-123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "gz0"),  "-100");
    TEST_EQUAL(format_floating_point(-123.456,           "gz4"),  "-123.5");
    TEST_EQUAL(format_floating_point(-123.456,           "gz8"),  "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "gz"),   "1.23456e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "gz0"),  "1e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "gz4"),  "1.235e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "gz8"),  "1.23456e8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "gz"),   "1.23456e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "gz0"),  "1e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "gz4"),  "1.235e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "gz8"),  "1.23456e-7");
    TEST_EQUAL(format_floating_point(0,                  "G"),    "0.00000");
    TEST_EQUAL(format_floating_point(100,                "G"),    "100.000");
    TEST_EQUAL(format_floating_point(123.456,            "G"),    "123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "G"),    "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "G"),    "1.23456E8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "G"),    "1.23456E-7");
    TEST_EQUAL(format_floating_point(0,                  "GS"),   "0.00000");
    TEST_EQUAL(format_floating_point(100,                "GS"),   "100.000");
    TEST_EQUAL(format_floating_point(123.456,            "GS"),   "123.456");
    TEST_EQUAL(format_floating_point(-123.456,           "GS"),   "-123.456");
    TEST_EQUAL(format_floating_point(123'456'000.0,      "GS"),   "1.23456E+8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  "GS"),   "1.23456E-7");
    TEST_EQUAL(format_floating_point(0,                  0),      "0");
    TEST_EQUAL(format_floating_point(0,                  4),      "0.000");
    TEST_EQUAL(format_floating_point(100,                0),      "100");
    TEST_EQUAL(format_floating_point(100,                4),      "100.0");
    TEST_EQUAL(format_floating_point(100,                8),      "100.00000");
    TEST_EQUAL(format_floating_point(123.456,            0),      "100");
    TEST_EQUAL(format_floating_point(123.456,            4),      "123.5");
    TEST_EQUAL(format_floating_point(123.456,            8),      "123.45600");
    TEST_EQUAL(format_floating_point(-123.456,           0),      "-100");
    TEST_EQUAL(format_floating_point(-123.456,           4),      "-123.5");
    TEST_EQUAL(format_floating_point(-123.456,           8),      "-123.45600");
    TEST_EQUAL(format_floating_point(123'456'000.0,      0),      "1e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      4),      "1.235e8");
    TEST_EQUAL(format_floating_point(123'456'000.0,      8),      "1.2345600e8");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  0),      "1e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  4),      "1.235e-7");
    TEST_EQUAL(format_floating_point(0.000'000'123'456,  8),      "1.2345600e-7");

}

void test_crow_format_floating_point_probability_format() {

    TEST_EQUAL(format_floating_point(0.0,              "p"),   "0");
    TEST_EQUAL(format_floating_point(0.0000123456789,  "p"),   "0.0000123457");
    TEST_EQUAL(format_floating_point(0.000123456789,   "p"),   "0.000123457");
    TEST_EQUAL(format_floating_point(0.00123456789,    "p"),   "0.00123457");
    TEST_EQUAL(format_floating_point(0.0123456789,     "p"),   "0.0123457");
    TEST_EQUAL(format_floating_point(0.123456789,      "p"),   "0.123457");
    TEST_EQUAL(format_floating_point(0.25,             "p"),   "0.250000");
    TEST_EQUAL(format_floating_point(0.5,              "p"),   "0.500000");
    TEST_EQUAL(format_floating_point(0.75,             "p"),   "0.750000");
    TEST_EQUAL(format_floating_point(0.87654321,       "p"),   "0.876543");
    TEST_EQUAL(format_floating_point(0.987654321,      "p"),   "0.9876543");
    TEST_EQUAL(format_floating_point(0.9987654321,     "p"),   "0.99876543");
    TEST_EQUAL(format_floating_point(0.99987654321,    "p"),   "0.999876543");
    TEST_EQUAL(format_floating_point(0.999987654321,   "p"),   "0.9999876543");
    TEST_EQUAL(format_floating_point(1.0,              "p"),   "1");
    TEST_EQUAL(format_floating_point(0.0,              "P"),   "0");
    TEST_EQUAL(format_floating_point(0.0000123456789,  "P"),   "0.00123457");
    TEST_EQUAL(format_floating_point(0.000123456789,   "P"),   "0.0123457");
    TEST_EQUAL(format_floating_point(0.00123456789,    "P"),   "0.123457");
    TEST_EQUAL(format_floating_point(0.0123456789,     "P"),   "1.23457");
    TEST_EQUAL(format_floating_point(0.123456789,      "P"),   "12.3457");
    TEST_EQUAL(format_floating_point(0.25,             "P"),   "25.0000");
    TEST_EQUAL(format_floating_point(0.5,              "P"),   "50.0000");
    TEST_EQUAL(format_floating_point(0.75,             "P"),   "75.0000");
    TEST_EQUAL(format_floating_point(0.87654321,       "P"),   "87.6543");
    TEST_EQUAL(format_floating_point(0.987654321,      "P"),   "98.76543");
    TEST_EQUAL(format_floating_point(0.9987654321,     "P"),   "99.876543");
    TEST_EQUAL(format_floating_point(0.99987654321,    "P"),   "99.9876543");
    TEST_EQUAL(format_floating_point(0.999987654321,   "P"),   "99.99876543");
    TEST_EQUAL(format_floating_point(1.0,              "P"),   "100");
    TEST_EQUAL(format_floating_point(0.0,              "p1"),  "0");
    TEST_EQUAL(format_floating_point(0.0000123456789,  "p1"),  "0.00001");
    TEST_EQUAL(format_floating_point(0.000123456789,   "p1"),  "0.0001");
    TEST_EQUAL(format_floating_point(0.00123456789,    "p1"),  "0.001");
    TEST_EQUAL(format_floating_point(0.0123456789,     "p1"),  "0.01");
    TEST_EQUAL(format_floating_point(0.123456789,      "p1"),  "0.1");
    TEST_EQUAL(format_floating_point(0.2,              "p1"),  "0.2");
    TEST_EQUAL(format_floating_point(0.5,              "p1"),  "0.5");
    TEST_EQUAL(format_floating_point(0.8,              "p1"),  "0.8");
    TEST_EQUAL(format_floating_point(0.87654321,       "p1"),  "0.9");
    TEST_EQUAL(format_floating_point(0.987654321,      "p1"),  "0.99");
    TEST_EQUAL(format_floating_point(0.9987654321,     "p1"),  "0.999");
    TEST_EQUAL(format_floating_point(0.99987654321,    "p1"),  "0.9999");
    TEST_EQUAL(format_floating_point(0.999987654321,   "p1"),  "0.99999");
    TEST_EQUAL(format_floating_point(1.0,              "p1"),  "1");
    TEST_EQUAL(format_floating_point(0.0,              "P1"),  "0");
    TEST_EQUAL(format_floating_point(0.0000123456789,  "P1"),  "0.001");
    TEST_EQUAL(format_floating_point(0.000123456789,   "P1"),  "0.01");
    TEST_EQUAL(format_floating_point(0.00123456789,    "P1"),  "0.1");
    TEST_EQUAL(format_floating_point(0.0123456789,     "P1"),  "1");
    TEST_EQUAL(format_floating_point(0.123456789,      "P1"),  "10");
    TEST_EQUAL(format_floating_point(0.2,              "P1"),  "20");
    TEST_EQUAL(format_floating_point(0.5,              "P1"),  "50");
    TEST_EQUAL(format_floating_point(0.8,              "P1"),  "80");
    TEST_EQUAL(format_floating_point(0.87654321,       "P1"),  "90");
    TEST_EQUAL(format_floating_point(0.987654321,      "P1"),  "99");
    TEST_EQUAL(format_floating_point(0.9987654321,     "P1"),  "99.9");
    TEST_EQUAL(format_floating_point(0.99987654321,    "P1"),  "99.99");
    TEST_EQUAL(format_floating_point(0.999987654321,   "P1"),  "99.999");
    TEST_EQUAL(format_floating_point(1.0,              "P1"),  "100");

}
