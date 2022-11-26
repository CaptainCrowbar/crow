#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_string_bin() {

    TEST_EQUAL(bin(uint8_t(0)),                      "00000000");
    TEST_EQUAL(bin(uint8_t(42)),                     "00101010");
    TEST_EQUAL(bin(uint8_t(255)),                    "11111111");
    TEST_EQUAL(bin(uint16_t(0)),                     "0000000000000000");
    TEST_EQUAL(bin(uint16_t(42)),                    "0000000000101010");
    TEST_EQUAL(bin(uint16_t(255)),                   "0000000011111111");
    TEST_EQUAL(bin(uint16_t(12'345)),                "0011000000111001");
    TEST_EQUAL(bin(uint16_t(65'535)),                "1111111111111111");
    TEST_EQUAL(bin(uint32_t(0)),                     "00000000000000000000000000000000");
    TEST_EQUAL(bin(uint32_t(42)),                    "00000000000000000000000000101010");
    TEST_EQUAL(bin(uint32_t(255)),                   "00000000000000000000000011111111");
    TEST_EQUAL(bin(uint32_t(12'345)),                "00000000000000000011000000111001");
    TEST_EQUAL(bin(uint32_t(65'535)),                "00000000000000001111111111111111");
    TEST_EQUAL(bin(uint32_t(1'000'000)),             "00000000000011110100001001000000");
    TEST_EQUAL(bin(uint32_t(4'294'967'295ull)),      "11111111111111111111111111111111");
    TEST_EQUAL(bin(uint32_t(0), 1),                  "0");
    TEST_EQUAL(bin(uint32_t(42), 1),                 "101010");
    TEST_EQUAL(bin(uint32_t(255), 1),                "11111111");
    TEST_EQUAL(bin(uint32_t(12'345), 1),             "11000000111001");
    TEST_EQUAL(bin(uint32_t(65'535), 1),             "1111111111111111");
    TEST_EQUAL(bin(uint32_t(1'000'000), 1),          "11110100001001000000");
    TEST_EQUAL(bin(uint32_t(4'294'967'295ull), 1),   "11111111111111111111111111111111");
    TEST_EQUAL(bin(uint32_t(0), 10),                 "0000000000");
    TEST_EQUAL(bin(uint32_t(42), 10),                "0000101010");
    TEST_EQUAL(bin(uint32_t(255), 10),               "0011111111");
    TEST_EQUAL(bin(uint32_t(12'345), 10),            "11000000111001");
    TEST_EQUAL(bin(uint32_t(65'535), 10),            "1111111111111111");
    TEST_EQUAL(bin(uint32_t(1'000'000), 10),         "11110100001001000000");
    TEST_EQUAL(bin(uint32_t(4'294'967'295ull), 10),  "11111111111111111111111111111111");
    TEST_EQUAL(bin(uint64_t(0)),                     "0000000000000000000000000000000000000000000000000000000000000000");
    TEST_EQUAL(bin(~ uint64_t(0)),                   "1111111111111111111111111111111111111111111111111111111111111111");
    TEST_EQUAL(bin(int64_t(-1)),                     "1111111111111111111111111111111111111111111111111111111111111111");

}

void test_crow_string_dec() {

    TEST_EQUAL(dec(uint8_t(0)),                     "0");
    TEST_EQUAL(dec(uint8_t(42)),                    "42");
    TEST_EQUAL(dec(uint8_t(255)),                   "255");
    TEST_EQUAL(dec(uint16_t(0)),                    "0");
    TEST_EQUAL(dec(uint16_t(42)),                   "42");
    TEST_EQUAL(dec(uint16_t(255)),                  "255");
    TEST_EQUAL(dec(uint16_t(12'345)),               "12345");
    TEST_EQUAL(dec(uint16_t(65'535)),               "65535");
    TEST_EQUAL(dec(uint32_t(0)),                    "0");
    TEST_EQUAL(dec(uint32_t(42)),                   "42");
    TEST_EQUAL(dec(uint32_t(255)),                  "255");
    TEST_EQUAL(dec(uint32_t(12'345)),               "12345");
    TEST_EQUAL(dec(uint32_t(65'535)),               "65535");
    TEST_EQUAL(dec(uint32_t(1'000'000)),            "1000000");
    TEST_EQUAL(dec(uint32_t(4'294'967'295ull)),     "4294967295");
    TEST_EQUAL(dec(uint32_t(0), 6),                 "000000");
    TEST_EQUAL(dec(uint32_t(42), 6),                "000042");
    TEST_EQUAL(dec(uint32_t(255), 6),               "000255");
    TEST_EQUAL(dec(uint32_t(12'345), 6),            "012345");
    TEST_EQUAL(dec(uint32_t(65'535), 6),            "065535");
    TEST_EQUAL(dec(uint32_t(1'000'000), 6),         "1000000");
    TEST_EQUAL(dec(uint32_t(4'294'967'295ull), 6),  "4294967295");
    TEST_EQUAL(dec(int8_t(0)),                      "0");
    TEST_EQUAL(dec(int8_t(127)),                    "127");
    TEST_EQUAL(dec(int8_t(-127)),                   "-127");
    TEST_EQUAL(dec(int8_t(-128)),                   "-128");
    TEST_EQUAL(dec(int16_t(0)),                     "0");
    TEST_EQUAL(dec(int16_t(32'767)),                "32767");
    TEST_EQUAL(dec(int16_t(-32'767)),               "-32767");
    TEST_EQUAL(dec(int16_t(-32'768)),               "-32768");
    TEST_EQUAL(dec(int32_t(0)),                     "0");
    TEST_EQUAL(dec(int32_t(2'147'483'647ll)),       "2147483647");
    TEST_EQUAL(dec(int32_t(-2'147'483'647ll)),      "-2147483647");
    TEST_EQUAL(dec(int32_t(-2'147'483'648ll)),      "-2147483648");

}

void test_crow_string_hex() {

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

void test_crow_string_roman_numerals() {

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
