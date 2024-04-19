#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_string_formatting_bin() {

    TEST_EQUAL(bin(0_u8),                   "00000000");
    TEST_EQUAL(bin(42_u8),                  "00101010");
    TEST_EQUAL(bin(255_u8),                 "11111111");
    TEST_EQUAL(bin(0_u16),                  "0000000000000000");
    TEST_EQUAL(bin(42_u16),                 "0000000000101010");
    TEST_EQUAL(bin(255_u16),                "0000000011111111");
    TEST_EQUAL(bin(12'345_u16),             "0011000000111001");
    TEST_EQUAL(bin(65'535_u16),             "1111111111111111");
    TEST_EQUAL(bin(0_u32),                  "00000000000000000000000000000000");
    TEST_EQUAL(bin(42_u32),                 "00000000000000000000000000101010");
    TEST_EQUAL(bin(255_u32),                "00000000000000000000000011111111");
    TEST_EQUAL(bin(12'345_u32),             "00000000000000000011000000111001");
    TEST_EQUAL(bin(65'535_u32),             "00000000000000001111111111111111");
    TEST_EQUAL(bin(1'000'000_u32),          "00000000000011110100001001000000");
    TEST_EQUAL(bin(4'294'967'295_u32),      "11111111111111111111111111111111");
    TEST_EQUAL(bin(0_u32, 1),               "0");
    TEST_EQUAL(bin(42_u32, 1),              "101010");
    TEST_EQUAL(bin(255_u32, 1),             "11111111");
    TEST_EQUAL(bin(12'345_u32, 1),          "11000000111001");
    TEST_EQUAL(bin(65'535_u32, 1),          "1111111111111111");
    TEST_EQUAL(bin(1'000'000_u32, 1),       "11110100001001000000");
    TEST_EQUAL(bin(4'294'967'295_u32, 1),   "11111111111111111111111111111111");
    TEST_EQUAL(bin(0_u32, 10),              "0000000000");
    TEST_EQUAL(bin(42_u32, 10),             "0000101010");
    TEST_EQUAL(bin(255_u32, 10),            "0011111111");
    TEST_EQUAL(bin(12'345_u32, 10),         "11000000111001");
    TEST_EQUAL(bin(65'535_u32, 10),         "1111111111111111");
    TEST_EQUAL(bin(1'000'000_u32, 10),      "11110100001001000000");
    TEST_EQUAL(bin(4'294'967'295_u32, 10),  "11111111111111111111111111111111");
    TEST_EQUAL(bin(0_u64),                  "0000000000000000000000000000000000000000000000000000000000000000");
    TEST_EQUAL(bin(~ 0_u64),                "1111111111111111111111111111111111111111111111111111111111111111");
    TEST_EQUAL(bin(-1_s64),                 "1111111111111111111111111111111111111111111111111111111111111111");

}

void test_crow_string_formatting_dec() {

    TEST_EQUAL(dec(0_u8),                    "0");
    TEST_EQUAL(dec(42_u8),                   "42");
    TEST_EQUAL(dec(255_u8),                  "255");
    TEST_EQUAL(dec(0_u16),                   "0");
    TEST_EQUAL(dec(42_u16),                  "42");
    TEST_EQUAL(dec(255_u16),                 "255");
    TEST_EQUAL(dec(12'345_u16),              "12345");
    TEST_EQUAL(dec(65'535_u16),              "65535");
    TEST_EQUAL(dec(0_u32),                   "0");
    TEST_EQUAL(dec(42_u32),                  "42");
    TEST_EQUAL(dec(255_u32),                 "255");
    TEST_EQUAL(dec(12'345_u32),              "12345");
    TEST_EQUAL(dec(65'535_u32),              "65535");
    TEST_EQUAL(dec(1'000'000_u32),           "1000000");
    TEST_EQUAL(dec(4'294'967'295_u32),       "4294967295");
    TEST_EQUAL(dec(0_u32, 6),                "000000");
    TEST_EQUAL(dec(42_u32, 6),               "000042");
    TEST_EQUAL(dec(255_u32, 6),              "000255");
    TEST_EQUAL(dec(12'345_u32, 6),           "012345");
    TEST_EQUAL(dec(65'535_u32, 6),           "065535");
    TEST_EQUAL(dec(1'000'000_u32, 6),        "1000000");
    TEST_EQUAL(dec(4'294'967'295_u32, 6),    "4294967295");
    TEST_EQUAL(dec(0_s8),                    "0");
    TEST_EQUAL(dec(127_s8),                  "127");
    TEST_EQUAL(dec(-127_s8),                 "-127");
    TEST_EQUAL(dec(-127_s8 - 1),             "-128");
    TEST_EQUAL(dec(0_s16),                   "0");
    TEST_EQUAL(dec(32'767_s16),              "32767");
    TEST_EQUAL(dec(-32'767_s16),             "-32767");
    TEST_EQUAL(dec(-32'767_s16 - 1),         "-32768");
    TEST_EQUAL(dec(0_s32),                   "0");
    TEST_EQUAL(dec(2'147'483'647_s32),       "2147483647");
    TEST_EQUAL(dec(-2'147'483'647_s32),      "-2147483647");
    TEST_EQUAL(dec(-2'147'483'647_s32 - 1),  "-2147483648");

}

void test_crow_string_formatting_hex() {

    TEST_EQUAL(hex(0_u8),                  "00");
    TEST_EQUAL(hex(42_u8),                 "2a");
    TEST_EQUAL(hex(255_u8),                "ff");
    TEST_EQUAL(hex(0_u16),                 "0000");
    TEST_EQUAL(hex(42_u16),                "002a");
    TEST_EQUAL(hex(255_u16),               "00ff");
    TEST_EQUAL(hex(12'345_u16),            "3039");
    TEST_EQUAL(hex(65'535_u16),            "ffff");
    TEST_EQUAL(hex(0_u32),                 "00000000");
    TEST_EQUAL(hex(42_u32),                "0000002a");
    TEST_EQUAL(hex(255_u32),               "000000ff");
    TEST_EQUAL(hex(12'345_u32),            "00003039");
    TEST_EQUAL(hex(65'535_u32),            "0000ffff");
    TEST_EQUAL(hex(1'000'000_u32),         "000f4240");
    TEST_EQUAL(hex(4'294'967'295_u32),     "ffffffff");
    TEST_EQUAL(hex(0_u32, 1),              "0");
    TEST_EQUAL(hex(42_u32, 1),             "2a");
    TEST_EQUAL(hex(255_u32, 1),            "ff");
    TEST_EQUAL(hex(12'345_u32, 1),         "3039");
    TEST_EQUAL(hex(65'535_u32, 1),         "ffff");
    TEST_EQUAL(hex(1'000'000_u32, 1),      "f4240");
    TEST_EQUAL(hex(4'294'967'295_u32, 1),  "ffffffff");
    TEST_EQUAL(hex(0_u32, 6),              "000000");
    TEST_EQUAL(hex(42_u32, 6),             "00002a");
    TEST_EQUAL(hex(255_u32, 6),            "0000ff");
    TEST_EQUAL(hex(12'345_u32, 6),         "003039");
    TEST_EQUAL(hex(65'535_u32, 6),         "00ffff");
    TEST_EQUAL(hex(1'000'000_u32, 6),      "0f4240");
    TEST_EQUAL(hex(4'294'967'295_u32, 6),  "ffffffff");
    TEST_EQUAL(hex(0_u64),                 "0000000000000000");
    TEST_EQUAL(hex(~ 0_u64),               "ffffffffffffffff");
    TEST_EQUAL(hex(-1_s64),                "ffffffffffffffff");

}

void test_crow_string_formatting_roman() {

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
