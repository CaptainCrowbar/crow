#include "crow/binary.hpp"
#include "crow/unit-test.hpp"
#include <optional>

using namespace Crow;

void test_crow_binary_byte_order() {

    // The tests here assume a little endian system

    TEST_EQUAL(swap_ends(uint8_t(0x00)),                       0x00);
    TEST_EQUAL(swap_ends(uint8_t(0xff)),                       0xff);
    TEST_EQUAL(swap_ends(uint16_t(0x0000)),                    0x0000);
    TEST_EQUAL(swap_ends(uint16_t(0x1234)),                    0x3412);
    TEST_EQUAL(swap_ends(uint32_t(0x0000'0000ul)),             0x0000'0000ul);
    TEST_EQUAL(swap_ends(uint32_t(0x1234'5678ul)),             0x7856'3412ul);
    TEST_EQUAL(swap_ends(uint64_t(0x0000'0000'0000'0000ull)),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(swap_ends(uint64_t(0x1234'5678'9abc'def0ull)),  0xf0de'bc9a'7856'3412ull);

    TEST_EQUAL(little_endian(uint8_t(0x00)),                       0x00);
    TEST_EQUAL(little_endian(uint8_t(0xff)),                       0xff);
    TEST_EQUAL(little_endian(uint16_t(0x0000)),                    0x0000);
    TEST_EQUAL(little_endian(uint16_t(0x1234)),                    0x1234);
    TEST_EQUAL(little_endian(uint32_t(0x0000'0000ul)),             0x0000'0000ul);
    TEST_EQUAL(little_endian(uint32_t(0x1234'5678ul)),             0x1234'5678ul);
    TEST_EQUAL(little_endian(uint64_t(0x0000'0000'0000'0000ull)),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(little_endian(uint64_t(0x1234'5678'9abc'def0ull)),  0x1234'5678'9abc'def0ull);

    TEST_EQUAL(big_endian(uint8_t(0x00)),                       0x00);
    TEST_EQUAL(big_endian(uint8_t(0xff)),                       0xff);
    TEST_EQUAL(big_endian(uint16_t(0x0000)),                    0x0000);
    TEST_EQUAL(big_endian(uint16_t(0x1234)),                    0x3412);
    TEST_EQUAL(big_endian(uint32_t(0x0000'0000ul)),             0x0000'0000ul);
    TEST_EQUAL(big_endian(uint32_t(0x1234'5678ul)),             0x7856'3412ul);
    TEST_EQUAL(big_endian(uint64_t(0x0000'0000'0000'0000ull)),  0x0000'0000'0000'0000ull);
    TEST_EQUAL(big_endian(uint64_t(0x1234'5678'9abc'def0ull)),  0xf0de'bc9a'7856'3412ull);

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

void test_crow_binary_birwise_operations() {

    TEST_EQUAL(popcount(0), 0);
    TEST_EQUAL(popcount(1), 1);
    TEST_EQUAL(popcount(2), 1);
    TEST_EQUAL(popcount(3), 2);
    TEST_EQUAL(popcount(4), 1);
    TEST_EQUAL(popcount(5), 2);
    TEST_EQUAL(popcount(6), 2);
    TEST_EQUAL(popcount(7), 3);
    TEST_EQUAL(popcount(8), 1);
    TEST_EQUAL(popcount(9), 2);
    TEST_EQUAL(popcount(10), 2);
    TEST_EQUAL(popcount(0x7fff), 15);
    TEST_EQUAL(popcount(0x8000), 1);
    TEST_EQUAL(popcount(0x8888), 4);
    TEST_EQUAL(popcount(0xffff), 16);
    TEST_EQUAL(popcount(0x7fffffffffffffffull), 63);
    TEST_EQUAL(popcount(0x8000000000000000ull), 1);
    TEST_EQUAL(popcount(0xffffffffffffffffull), 64);

    TEST_EQUAL(bit_ceil(0), 0);
    TEST_EQUAL(bit_ceil(1), 1);
    TEST_EQUAL(bit_ceil(2), 2);
    TEST_EQUAL(bit_ceil(3), 4);
    TEST_EQUAL(bit_ceil(4), 4);
    TEST_EQUAL(bit_ceil(5), 8);
    TEST_EQUAL(bit_ceil(6), 8);
    TEST_EQUAL(bit_ceil(7), 8);
    TEST_EQUAL(bit_ceil(8), 8);
    TEST_EQUAL(bit_ceil(9), 16);
    TEST_EQUAL(bit_ceil(10), 16);
    TEST_EQUAL(bit_ceil(0x7ffful), 0x8000ul);
    TEST_EQUAL(bit_ceil(0x8000ul), 0x8000ul);
    TEST_EQUAL(bit_ceil(0x8001ul), 0x10000ul);
    TEST_EQUAL(bit_ceil(0xfffful), 0x10000ul);

    TEST_EQUAL(bit_floor(0), 0);
    TEST_EQUAL(bit_floor(1), 1);
    TEST_EQUAL(bit_floor(2), 2);
    TEST_EQUAL(bit_floor(3), 2);
    TEST_EQUAL(bit_floor(4), 4);
    TEST_EQUAL(bit_floor(5), 4);
    TEST_EQUAL(bit_floor(6), 4);
    TEST_EQUAL(bit_floor(7), 4);
    TEST_EQUAL(bit_floor(8), 8);
    TEST_EQUAL(bit_floor(9), 8);
    TEST_EQUAL(bit_floor(10), 8);
    TEST_EQUAL(bit_floor(0x7ffful), 0x4000ul);
    TEST_EQUAL(bit_floor(0x8000ul), 0x8000ul);
    TEST_EQUAL(bit_floor(0x8001ul), 0x8000ul);
    TEST_EQUAL(bit_floor(0xfffful), 0x8000ul);

    TEST_EQUAL(bit_width(0), 0);
    TEST_EQUAL(bit_width(1), 1);
    TEST_EQUAL(bit_width(2), 2);
    TEST_EQUAL(bit_width(3), 2);
    TEST_EQUAL(bit_width(4), 3);
    TEST_EQUAL(bit_width(5), 3);
    TEST_EQUAL(bit_width(6), 3);
    TEST_EQUAL(bit_width(7), 3);
    TEST_EQUAL(bit_width(8), 4);
    TEST_EQUAL(bit_width(9), 4);
    TEST_EQUAL(bit_width(10), 4);
    TEST_EQUAL(bit_width(0x7fff), 15);
    TEST_EQUAL(bit_width(0x8000), 16);
    TEST_EQUAL(bit_width(0x8888), 16);
    TEST_EQUAL(bit_width(0xffff), 16);
    TEST_EQUAL(bit_width(0x7fffffffffffffffull), 63);
    TEST_EQUAL(bit_width(0x8000000000000000ull), 64);
    TEST_EQUAL(bit_width(0xffffffffffffffffull), 64);

    TEST(! has_single_bit(0));
    TEST(has_single_bit(1));
    TEST(has_single_bit(2));
    TEST(! has_single_bit(3));
    TEST(has_single_bit(4));
    TEST(! has_single_bit(5));
    TEST(! has_single_bit(6));
    TEST(! has_single_bit(7));
    TEST(has_single_bit(8));
    TEST(! has_single_bit(9));
    TEST(! has_single_bit(10));
    TEST(! has_single_bit(0x7fff));
    TEST(has_single_bit(0x8000));
    TEST(! has_single_bit(0x8001));
    TEST(! has_single_bit(0xffff));
    TEST(! has_single_bit(0x7fffffffffffffffull));
    TEST(has_single_bit(0x8000000000000000ull));
    TEST(! has_single_bit(0x8000000000000001ull));
    TEST(! has_single_bit(0xffffffffffffffffull));

    uint16_t x = 0x1234;

    TEST_EQUAL(rotl(x, 0), 0x1234);    TEST_EQUAL(rotr(x, 0), 0x1234);
    TEST_EQUAL(rotl(x, 4), 0x2341);    TEST_EQUAL(rotr(x, 4), 0x4123);
    TEST_EQUAL(rotl(x, 8), 0x3412);    TEST_EQUAL(rotr(x, 8), 0x3412);
    TEST_EQUAL(rotl(x, 12), 0x4123);   TEST_EQUAL(rotr(x, 12), 0x2341);
    TEST_EQUAL(rotl(x, 16), 0x1234);   TEST_EQUAL(rotr(x, 16), 0x1234);
    TEST_EQUAL(rotl(x, 20), 0x2341);   TEST_EQUAL(rotr(x, 20), 0x4123);
    TEST_EQUAL(rotl(x, 24), 0x3412);   TEST_EQUAL(rotr(x, 24), 0x3412);
    TEST_EQUAL(rotl(x, 28), 0x4123);   TEST_EQUAL(rotr(x, 28), 0x2341);
    TEST_EQUAL(rotl(x, 32), 0x1234);   TEST_EQUAL(rotr(x, 32), 0x1234);
    TEST_EQUAL(rotl(x, -4), 0x4123);   TEST_EQUAL(rotr(x, -4), 0x2341);
    TEST_EQUAL(rotl(x, -8), 0x3412);   TEST_EQUAL(rotr(x, -8), 0x3412);
    TEST_EQUAL(rotl(x, -12), 0x2341);  TEST_EQUAL(rotr(x, -12), 0x4123);
    TEST_EQUAL(rotl(x, -16), 0x1234);  TEST_EQUAL(rotr(x, -16), 0x1234);
    TEST_EQUAL(rotl(x, -20), 0x4123);  TEST_EQUAL(rotr(x, -20), 0x2341);
    TEST_EQUAL(rotl(x, -24), 0x3412);  TEST_EQUAL(rotr(x, -24), 0x3412);
    TEST_EQUAL(rotl(x, -28), 0x2341);  TEST_EQUAL(rotr(x, -28), 0x4123);
    TEST_EQUAL(rotl(x, -32), 0x1234);  TEST_EQUAL(rotr(x, -32), 0x1234);

    uint32_t y = 0x12345678;

    TEST_EQUAL(rotl(y, 0), 0x1234'5678ul);    TEST_EQUAL(rotr(y, 0), 0x1234'5678ul);
    TEST_EQUAL(rotl(y, 4), 0x2345'6781ul);    TEST_EQUAL(rotr(y, 4), 0x8123'4567ul);
    TEST_EQUAL(rotl(y, 8), 0x3456'7812ul);    TEST_EQUAL(rotr(y, 8), 0x7812'3456ul);
    TEST_EQUAL(rotl(y, 12), 0x4567'8123ul);   TEST_EQUAL(rotr(y, 12), 0x6781'2345ul);
    TEST_EQUAL(rotl(y, 16), 0x5678'1234ul);   TEST_EQUAL(rotr(y, 16), 0x5678'1234ul);
    TEST_EQUAL(rotl(y, 20), 0x6781'2345ul);   TEST_EQUAL(rotr(y, 20), 0x4567'8123ul);
    TEST_EQUAL(rotl(y, 24), 0x7812'3456ul);   TEST_EQUAL(rotr(y, 24), 0x3456'7812ul);
    TEST_EQUAL(rotl(y, 28), 0x8123'4567ul);   TEST_EQUAL(rotr(y, 28), 0x2345'6781ul);
    TEST_EQUAL(rotl(y, 32), 0x1234'5678ul);   TEST_EQUAL(rotr(y, 32), 0x1234'5678ul);
    TEST_EQUAL(rotl(y, 36), 0x2345'6781ul);   TEST_EQUAL(rotr(y, 36), 0x8123'4567ul);
    TEST_EQUAL(rotl(y, 40), 0x3456'7812ul);   TEST_EQUAL(rotr(y, 40), 0x7812'3456ul);
    TEST_EQUAL(rotl(y, 44), 0x4567'8123ul);   TEST_EQUAL(rotr(y, 44), 0x6781'2345ul);
    TEST_EQUAL(rotl(y, 48), 0x5678'1234ul);   TEST_EQUAL(rotr(y, 48), 0x5678'1234ul);
    TEST_EQUAL(rotl(y, 52), 0x6781'2345ul);   TEST_EQUAL(rotr(y, 52), 0x4567'8123ul);
    TEST_EQUAL(rotl(y, 56), 0x7812'3456ul);   TEST_EQUAL(rotr(y, 56), 0x3456'7812ul);
    TEST_EQUAL(rotl(y, 60), 0x8123'4567ul);   TEST_EQUAL(rotr(y, 60), 0x2345'6781ul);
    TEST_EQUAL(rotl(y, 64), 0x1234'5678ul);   TEST_EQUAL(rotr(y, 64), 0x1234'5678ul);
    TEST_EQUAL(rotl(y, -4), 0x8123'4567ul);   TEST_EQUAL(rotr(y, -4), 0x2345'6781ul);
    TEST_EQUAL(rotl(y, -8), 0x7812'3456ul);   TEST_EQUAL(rotr(y, -8), 0x3456'7812ul);
    TEST_EQUAL(rotl(y, -12), 0x6781'2345ul);  TEST_EQUAL(rotr(y, -12), 0x4567'8123ul);
    TEST_EQUAL(rotl(y, -16), 0x5678'1234ul);  TEST_EQUAL(rotr(y, -16), 0x5678'1234ul);
    TEST_EQUAL(rotl(y, -20), 0x4567'8123ul);  TEST_EQUAL(rotr(y, -20), 0x6781'2345ul);
    TEST_EQUAL(rotl(y, -24), 0x3456'7812ul);  TEST_EQUAL(rotr(y, -24), 0x7812'3456ul);
    TEST_EQUAL(rotl(y, -28), 0x2345'6781ul);  TEST_EQUAL(rotr(y, -28), 0x8123'4567ul);
    TEST_EQUAL(rotl(y, -32), 0x1234'5678ul);  TEST_EQUAL(rotr(y, -32), 0x1234'5678ul);
    TEST_EQUAL(rotl(y, -36), 0x8123'4567ul);  TEST_EQUAL(rotr(y, -36), 0x2345'6781ul);
    TEST_EQUAL(rotl(y, -40), 0x7812'3456ul);  TEST_EQUAL(rotr(y, -40), 0x3456'7812ul);
    TEST_EQUAL(rotl(y, -44), 0x6781'2345ul);  TEST_EQUAL(rotr(y, -44), 0x4567'8123ul);
    TEST_EQUAL(rotl(y, -48), 0x5678'1234ul);  TEST_EQUAL(rotr(y, -48), 0x5678'1234ul);
    TEST_EQUAL(rotl(y, -52), 0x4567'8123ul);  TEST_EQUAL(rotr(y, -52), 0x6781'2345ul);
    TEST_EQUAL(rotl(y, -56), 0x3456'7812ul);  TEST_EQUAL(rotr(y, -56), 0x7812'3456ul);
    TEST_EQUAL(rotl(y, -60), 0x2345'6781ul);  TEST_EQUAL(rotr(y, -60), 0x8123'4567ul);
    TEST_EQUAL(rotl(y, -64), 0x1234'5678ul);  TEST_EQUAL(rotr(y, -64), 0x1234'5678ul);

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
