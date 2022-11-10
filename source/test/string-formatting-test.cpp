#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_hex_string() {

    TEST_EQUAL(hex(uint8_t(0)),                     "00");
    TEST_EQUAL(hex(uint8_t(42)),                    "2a");
    TEST_EQUAL(hex(uint8_t(255)),                   "ff");
    TEST_EQUAL(hex(uint16_t(0)),                    "0000");
    TEST_EQUAL(hex(uint16_t(42)),                   "002a");
    TEST_EQUAL(hex(uint16_t(255)),                  "00ff");
    TEST_EQUAL(hex(uint16_t(12'345)),               "3039");
    TEST_EQUAL(hex(uint16_t(65'535)),               "ffff");
    TEST_EQUAL(hex(uint32_t(0)),                    "00000000");
    TEST_EQUAL(hex(uint32_t(42)),                   "0000002a");
    TEST_EQUAL(hex(uint32_t(255)),                  "000000ff");
    TEST_EQUAL(hex(uint32_t(12'345)),               "00003039");
    TEST_EQUAL(hex(uint32_t(65'535)),               "0000ffff");
    TEST_EQUAL(hex(uint32_t(1'000'000)),            "000f4240");
    TEST_EQUAL(hex(uint32_t(4'294'967'295ull)),     "ffffffff");
    TEST_EQUAL(hex(uint32_t(0), 1),                 "0");
    TEST_EQUAL(hex(uint32_t(42), 1),                "2a");
    TEST_EQUAL(hex(uint32_t(255), 1),               "ff");
    TEST_EQUAL(hex(uint32_t(12'345), 1),            "3039");
    TEST_EQUAL(hex(uint32_t(65'535), 1),            "ffff");
    TEST_EQUAL(hex(uint32_t(1'000'000), 1),         "f4240");
    TEST_EQUAL(hex(uint32_t(4'294'967'295ull), 1),  "ffffffff");
    TEST_EQUAL(hex(uint32_t(0), 6),                 "000000");
    TEST_EQUAL(hex(uint32_t(42), 6),                "00002a");
    TEST_EQUAL(hex(uint32_t(255), 6),               "0000ff");
    TEST_EQUAL(hex(uint32_t(12'345), 6),            "003039");
    TEST_EQUAL(hex(uint32_t(65'535), 6),            "00ffff");
    TEST_EQUAL(hex(uint32_t(1'000'000), 6),         "0f4240");
    TEST_EQUAL(hex(uint32_t(4'294'967'295ull), 6),  "ffffffff");
    TEST_EQUAL(hex(uint64_t(0)),                    "0000000000000000");
    TEST_EQUAL(hex(~ uint64_t(0)),                  "ffffffffffffffff");
    TEST_EQUAL(hex(int64_t(-1)),                    "ffffffffffffffff");

}

void test_crow_string_quote() {

    // MSVC 2022 has a bug with backslashes in raw strings

    TEST_EQUAL(quote(""),                  "\"\"");
    TEST_EQUAL(quote("Hello world"),       "\"Hello world\"");
    TEST_EQUAL(quote("Hello world\r\n"),   "\"Hello world\\r\\n\"");
    TEST_EQUAL(quote("\"Hello\\world\""),  "\"\\\"Hello\\\\world\\\"\"");
    TEST_EQUAL(quote("αβγδε"),             "\"αβγδε\"");
    TEST_EQUAL(quote("Hello\xffworld"),    "\"Hello\\xffworld\"");

}

void test_crow_roman_numerals() {

    TEST_EQUAL(roman(1),         "I");
    TEST_EQUAL(roman(42),        "XLII");
    TEST_EQUAL(roman(1111),      "MCXI");
    TEST_EQUAL(roman(2222),      "MMCCXXII");
    TEST_EQUAL(roman(3333),      "MMMCCCXXXIII");
    TEST_EQUAL(roman(4444),      "MMMMCDXLIV");
    TEST_EQUAL(roman(5555),      "MMMMMDLV");
    TEST_EQUAL(roman(6666),      "MMMMMMDCLXVI");
    TEST_EQUAL(roman(7777),      "MMMMMMMDCCLXXVII");
    TEST_EQUAL(roman(8888),      "MMMMMMMMDCCCLXXXVIII");
    TEST_EQUAL(roman(9999),      "MMMMMMMMMCMXCIX");
    TEST_EQUAL(roman(1, true),   "i");
    TEST_EQUAL(roman(42, true),  "xlii");

    TEST_THROW(roman(0), std::invalid_argument);
    TEST_THROW(roman(-1), std::invalid_argument);

}
