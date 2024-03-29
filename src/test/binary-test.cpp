#include "crow/binary.hpp"
#include "crow/unit-test.hpp"
#include <optional>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_binary_bit_manipulation() {

    TEST(! has_bit(0, 0));  // 0000 0000
    TEST(! has_bit(1, 0));  // 0001 0000
    TEST(! has_bit(0, 1));  // 0000 0001
    TEST(has_bit(1, 1));    // 0001 0001
    TEST(! has_bit(2, 1));  // 0010 0001
    TEST(has_bit(3, 1));    // 0011 0001
    TEST(! has_bit(0, 6));  // 0000 0110
    TEST(! has_bit(1, 6));  // 0001 0110
    TEST(has_bit(2, 6));    // 0010 0110
    TEST(has_bit(3, 6));    // 0011 0110
    TEST(has_bit(4, 6));    // 0100 0110
    TEST(has_bit(5, 6));    // 0101 0110
    TEST(has_bit(6, 6));    // 0110 0110
    TEST(has_bit(7, 6));    // 0111 0110
    TEST(! has_bit(8, 6));  // 1000 0110
    TEST(! has_bit(9, 6));  // 1001 0110
    TEST(has_bit(10, 6));   // 1010 0110
    TEST(has_bit(11, 6));   // 1011 0110
    TEST(has_bit(12, 6));   // 1100 0110
    TEST(has_bit(13, 6));   // 1101 0110
    TEST(has_bit(14, 6));   // 1110 0110
    TEST(has_bit(15, 6));   // 1111 0110

    TEST(! has_bits(0, 0));   // 0000 0000
    TEST(! has_bits(1, 0));   // 0001 0000
    TEST(! has_bits(0, 1));   // 0000 0001
    TEST(has_bits(1, 1));     // 0001 0001
    TEST(! has_bits(2, 1));   // 0010 0001
    TEST(has_bits(3, 1));     // 0011 0001
    TEST(! has_bits(0, 6));   // 0000 0110
    TEST(! has_bits(1, 6));   // 0001 0110
    TEST(! has_bits(2, 6));   // 0010 0110
    TEST(! has_bits(3, 6));   // 0011 0110
    TEST(! has_bits(4, 6));   // 0100 0110
    TEST(! has_bits(5, 6));   // 0101 0110
    TEST(has_bits(6, 6));     // 0110 0110
    TEST(has_bits(7, 6));     // 0111 0110
    TEST(! has_bits(8, 6));   // 1000 0110
    TEST(! has_bits(9, 6));   // 1001 0110
    TEST(! has_bits(10, 6));  // 1010 0110
    TEST(! has_bits(11, 6));  // 1011 0110
    TEST(! has_bits(12, 6));  // 1100 0110
    TEST(! has_bits(13, 6));  // 1101 0110
    TEST(has_bits(14, 6));    // 1110 0110
    TEST(has_bits(15, 6));    // 1111 0110

}

void test_crow_binary_byte_order() {

    // The tests here assume a little endian system

    TEST_EQUAL(swap_ends(0x00_u8),                    0x00);
    TEST_EQUAL(swap_ends(0xff_u8),                    0xff);
    TEST_EQUAL(swap_ends(0x0000_u16),                 0x0000);
    TEST_EQUAL(swap_ends(0x1234_u16),                 0x3412);
    TEST_EQUAL(swap_ends(0x0000'0000_u32),            0x0000'0000ul);
    TEST_EQUAL(swap_ends(0x1234'5678_u32),            0x7856'3412ul);
    TEST_EQUAL(swap_ends(0x0000'0000'0000'0000_u64),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(swap_ends(0x1234'5678'9abc'def0_u64),  0xf0de'bc9a'7856'3412ull);

    TEST_EQUAL(little_endian(0x00_u8),                    0x00);
    TEST_EQUAL(little_endian(0xff_u8),                    0xff);
    TEST_EQUAL(little_endian(0x0000_u16),                 0x0000);
    TEST_EQUAL(little_endian(0x1234_u16),                 0x1234);
    TEST_EQUAL(little_endian(0x0000'0000_u32),            0x0000'0000ul);
    TEST_EQUAL(little_endian(0x1234'5678_u32),            0x1234'5678ul);
    TEST_EQUAL(little_endian(0x0000'0000'0000'0000_u64),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(little_endian(0x1234'5678'9abc'def0_u64),  0x1234'5678'9abc'def0ull);

    TEST_EQUAL(big_endian(0x00_u8),                    0x00);
    TEST_EQUAL(big_endian(0xff_u8),                    0xff);
    TEST_EQUAL(big_endian(0x0000_u16),                 0x0000);
    TEST_EQUAL(big_endian(0x1234_u16),                 0x3412);
    TEST_EQUAL(big_endian(0x0000'0000_u32),            0x0000'0000ul);
    TEST_EQUAL(big_endian(0x1234'5678_u32),            0x7856'3412ul);
    TEST_EQUAL(big_endian(0x0000'0000'0000'0000_u64),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(big_endian(0x1234'5678'9abc'def0_u64),  0xf0de'bc9a'7856'3412ull);

    static constexpr uint8_t a = 0x12;
    static constexpr uint16_t b = 0x1234;
    static constexpr uint32_t c = 0x1234'5678ul;
    static constexpr uint64_t d = 0x1234'5678'9abc'def0ull;

    uint8_t w;
    uint16_t x;
    uint32_t y;
    uint64_t z;

    TRY(w = read_le<uint8_t>(&a));   TEST_EQUAL(w, 0x12);
    TRY(x = read_le<uint16_t>(&b));  TEST_EQUAL(x, 0x1234);
    TRY(y = read_le<uint32_t>(&c));  TEST_EQUAL(y, 0x1234'5678ul);
    TRY(z = read_le<uint64_t>(&d));  TEST_EQUAL(z, 0x1234'5678'9abc'def0ull);
    TRY(w = read_be<uint8_t>(&a));   TEST_EQUAL(w, 0x12);
    TRY(x = read_be<uint16_t>(&b));  TEST_EQUAL(x, 0x3412);
    TRY(y = read_be<uint32_t>(&c));  TEST_EQUAL(y, 0x7856'3412ul);
    TRY(z = read_be<uint64_t>(&d));  TEST_EQUAL(z, 0xf0de'bc9a'7856'3412ull);

    TRY(write_le(a, &w));  TEST_EQUAL(w, 0x12);
    TRY(write_le(b, &x));  TEST_EQUAL(x, 0x1234);
    TRY(write_le(c, &y));  TEST_EQUAL(y, 0x1234'5678ul);
    TRY(write_le(d, &z));  TEST_EQUAL(z, 0x1234'5678'9abc'def0ull);
    TRY(write_be(a, &w));  TEST_EQUAL(w, 0x12);
    TRY(write_be(b, &x));  TEST_EQUAL(x, 0x3412);
    TRY(write_be(c, &y));  TEST_EQUAL(y, 0x7856'3412ul);
    TRY(write_be(d, &z));  TEST_EQUAL(z, 0xf0de'bc9a'7856'3412ull);

}

void test_crow_binary_signed_overflow_detection() {

    int16_t x = 0, y = 0;
    std::optional<int16_t> z;

    x = 32'000;   y = 766;      TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'766);
    x = 32'000;   y = 767;      TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'767);
    x = 32'000;   y = 768;      TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 32'000;   y = 769;      TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 766;      y = 32'000;   TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'766);
    x = 767;      y = 32'000;   TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'767);
    x = 768;      y = 32'000;   TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 769;      y = 32'000;   TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -32'000;  y = -767;     TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'767);
    x = -32'000;  y = -768;     TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'768);
    x = -32'000;  y = -769;     TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -32'000;  y = -770;     TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -767;     y = -32'000;  TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'767);
    x = -768;     y = -32'000;  TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'768);
    x = -769;     y = -32'000;  TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -770;     y = -32'000;  TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);

    x = 32'000;   y = -766;     TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'766);
    x = 32'000;   y = -767;     TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'767);
    x = 32'000;   y = -768;     TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 32'000;   y = -769;     TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 766;      y = -32'000;  TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'766);
    x = 767;      y = -32'000;  TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 32'767);
    x = 768;      y = -32'000;  TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 769;      y = -32'000;  TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -32'000;  y = 767;      TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'767);
    x = -32'000;  y = 768;      TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'768);
    x = -32'000;  y = 769;      TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -32'000;  y = 770;      TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -767;     y = 32'000;   TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'767);
    x = -768;     y = 32'000;   TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), -32'768);
    x = -769;     y = 32'000;   TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = -770;     y = 32'000;   TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);

}

void test_crow_binary_unsigned_overflow_detection() {

    uint16_t x = 0, y = 0;
    std::optional<uint16_t> z;

    x = 65'000;  y = 534;     TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 65'534);
    x = 65'000;  y = 535;     TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 65'535);
    x = 65'000;  y = 536;     TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 65'000;  y = 537;     TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 534;     y = 65'000;  TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 65'534);
    x = 535;     y = 65'000;  TRY(z = checked_add(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 65'535);
    x = 536;     y = 65'000;  TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 537;     y = 65'000;  TRY(z = checked_add(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);

    x = 5;  y = 4;  TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 1);
    x = 5;  y = 5;  TRY(z = checked_subtract(x, y));  TEST(z.has_value());    TEST_EQUAL(z.value(), 0);
    x = 5;  y = 6;  TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);
    x = 5;  y = 7;  TRY(z = checked_subtract(x, y));  TEST(! z.has_value());  TEST_EQUAL(z.value_or(999), 999);

}
