#include "crow/mp-integer.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <vector>

using namespace Crow;

namespace {

    using ByteVector = std::vector<uint8_t>;

    std::string hexdump(const ByteVector& v) {
        std::string s(reinterpret_cast<const char*>(v.data()), v.size());
        return format_string(s, "x");
    }

}

void test_crow_mp_integer_unsigned_arithmetic() {

    MPN x, y, z, q, r;
    std::string s;

    TRY(x = 0);
    TEST_EQUAL(x.bits(), 0u);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "0");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "0");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "0");

    TRY(y = x + 15);
    TEST_EQUAL(y.bits(), 4u);
    TRY(s = to_string(y));  TEST_EQUAL(s, "15");
    TRY(s = y.str("x"));    TEST_EQUAL(s, "f");

    TRY(y = 15 - x);
    TEST_EQUAL(y.bits(), 4u);
    TRY(s = to_string(y));  TEST_EQUAL(s, "15");
    TRY(s = y.str("x"));    TEST_EQUAL(s, "f");

    TRY(x = 0x123456789abcdef0ull);
    TRY(y = 0xffffffffffffffffull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(y.bits(), 64u);
    TRY(s = to_string(x));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = to_string(y));  TEST_EQUAL(s, "18446744073709551615");

    TRY(z = x + 15);
    TEST_EQUAL(z.bits(), 61u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "1311768467463790335");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "123456789abcdeff");

    TRY(z = x + y);
    TEST_EQUAL(z.bits(), 65u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "19758512541173341935");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "1123456789abcdeef");

    TRY(z = y - 15);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "18446744073709551600");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "fffffffffffffff0");

    TRY(z = y - x);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "17134975606245761295");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "edcba9876543210f");

    TRY(z = x * y);
    TEST_EQUAL(z.bits(), 125u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "24197857203266734862169780735577366800");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "123456789abcdeefedcba98765432110");

    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("123456789123456789123456789123456789"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789000000000000000000000000000000000000"));
    TRY(y = MPN("123456789123456789123456789123456789000000000"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789"));
    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("1357913579135791357913579"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789123456789122098875544320997765543210"));

    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("123456789"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("1000000001000000001000000001000000001"));
    TEST_EQUAL(r, MPN("0"));
    TRY(y = MPN("987654321"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("124999998985937499000175780249997801"));
    TEST_EQUAL(r, MPN("725308668"));
    TRY(y = MPN("987654321987654321987654321987654321987654321"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("0"));
    TEST_EQUAL(r, MPN("123456789123456789123456789123456789123456789"));
    TRY(y = {});

}

void test_crow_mp_integer_unsigned_arithmetic_powers() {

    MPN x, y;
    std::string s;

    TRY(x = 0);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 0);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "0");
    TRY(x = 1);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1");
    TRY(x = 10);  TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 10);  TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "10");
    TRY(x = 10);  TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "100");
    TRY(x = 10);  TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1000");
    TRY(x = 10);  TRY(y = x.pow(4));   TEST_EQUAL(y.str(), "10000");
    TRY(x = 10);  TRY(y = x.pow(5));   TEST_EQUAL(y.str(), "100000");
    TRY(x = 10);  TRY(y = x.pow(6));   TEST_EQUAL(y.str(), "1000000");
    TRY(x = 10);  TRY(y = x.pow(7));   TEST_EQUAL(y.str(), "10000000");
    TRY(x = 10);  TRY(y = x.pow(8));   TEST_EQUAL(y.str(), "100000000");
    TRY(x = 10);  TRY(y = x.pow(9));   TEST_EQUAL(y.str(), "1000000000");
    TRY(x = 10);  TRY(y = x.pow(10));  TEST_EQUAL(y.str(), "10000000000");
    TRY(x = 10);  TRY(y = x.pow(11));  TEST_EQUAL(y.str(), "100000000000");
    TRY(x = 10);  TRY(y = x.pow(12));  TEST_EQUAL(y.str(), "1000000000000");
    TRY(x = 10);  TRY(y = x.pow(13));  TEST_EQUAL(y.str(), "10000000000000");
    TRY(x = 10);  TRY(y = x.pow(14));  TEST_EQUAL(y.str(), "100000000000000");
    TRY(x = 10);  TRY(y = x.pow(15));  TEST_EQUAL(y.str(), "1000000000000000");
    TRY(x = 10);  TRY(y = x.pow(16));  TEST_EQUAL(y.str(), "10000000000000000");
    TRY(x = 10);  TRY(y = x.pow(17));  TEST_EQUAL(y.str(), "100000000000000000");
    TRY(x = 10);  TRY(y = x.pow(18));  TEST_EQUAL(y.str(), "1000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(19));  TEST_EQUAL(y.str(), "10000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(20));  TEST_EQUAL(y.str(), "100000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(21));  TEST_EQUAL(y.str(), "1000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(22));  TEST_EQUAL(y.str(), "10000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(23));  TEST_EQUAL(y.str(), "100000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(24));  TEST_EQUAL(y.str(), "1000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(25));  TEST_EQUAL(y.str(), "10000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(26));  TEST_EQUAL(y.str(), "100000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(27));  TEST_EQUAL(y.str(), "1000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(28));  TEST_EQUAL(y.str(), "10000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(29));  TEST_EQUAL(y.str(), "100000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(30));  TEST_EQUAL(y.str(), "1000000000000000000000000000000");

}

void test_crow_mp_integer_unsigned_bit_operations() {

    MPN x, y, z;
    std::string s;

    TEST_EQUAL(x.bits_set(), 0u);
    TEST(x.is_even());
    TEST(! x.is_odd());

    TRY(x = 0x123456789abcdef0ull);
    TRY(y = 0xffffffffffffffffull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(y.bits(), 64u);
    TEST_EQUAL(x.bits_set(), 32u);
    TEST_EQUAL(y.bits_set(), 64u);
    TEST(x.is_even());
    TEST(! x.is_odd());
    TEST(! y.is_even());
    TEST(y.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = to_string(y));  TEST_EQUAL(s, "18446744073709551615");

    TRY(z = x & y);
    TEST_EQUAL(z.bits(), 61u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "123456789abcdef0");

    TRY(z = x | y);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "18446744073709551615");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "ffffffffffffffff");

    TRY(z = x ^ y);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "17134975606245761295");
    TRY(s = z.str("x"));    TEST_EQUAL(s, "edcba9876543210f");

    TRY(z = x >> 0);   TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef0");
    TRY(z = x >> 1);   TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f78");
    TRY(z = x >> 2);   TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc");
    TRY(z = x >> 3);   TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde");
    TRY(z = x >> 31);  TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf1");
    TRY(z = x >> 32);  TRY(s = z.str("x"));  TEST_EQUAL(s, "12345678");
    TRY(z = x >> 33);  TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c");
    TRY(z = x >> 58);  TRY(s = z.str("x"));  TEST_EQUAL(s, "4");
    TRY(z = x >> 59);  TRY(s = z.str("x"));  TEST_EQUAL(s, "2");
    TRY(z = x >> 60);  TRY(s = z.str("x"));  TEST_EQUAL(s, "1");
    TRY(z = x >> 61);  TRY(s = z.str("x"));  TEST_EQUAL(s, "0");
    TRY(z = x >> 62);  TRY(s = z.str("x"));  TEST_EQUAL(s, "0");
    TRY(z = x >> 63);  TRY(s = z.str("x"));  TEST_EQUAL(s, "0");
    TRY(z = x >> 64);  TRY(s = z.str("x"));  TEST_EQUAL(s, "0");
    TRY(z = x >> 65);  TRY(s = z.str("x"));  TEST_EQUAL(s, "0");

    TRY(z = x << 0);   TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef0");
    TRY(z = x << 1);   TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde0");
    TRY(z = x << 2);   TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc0");
    TRY(z = x << 3);   TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f780");
    TRY(z = x << 4);   TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef00");
    TRY(z = x << 5);   TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde00");
    TRY(z = x << 6);   TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc00");
    TRY(z = x << 7);   TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f7800");
    TRY(z = x << 8);   TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef000");
    TRY(z = x << 9);   TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde000");
    TRY(z = x << 10);  TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc000");
    TRY(z = x << 11);  TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f78000");
    TRY(z = x << 12);  TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef0000");
    TRY(z = x << 13);  TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde0000");
    TRY(z = x << 14);  TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc0000");
    TRY(z = x << 15);  TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f780000");
    TRY(z = x << 16);  TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef00000");
    TRY(z = x << 17);  TRY(s = z.str("x"));  TEST_EQUAL(s, "2468acf13579bde00000");
    TRY(z = x << 18);  TRY(s = z.str("x"));  TEST_EQUAL(s, "48d159e26af37bc00000");
    TRY(z = x << 19);  TRY(s = z.str("x"));  TEST_EQUAL(s, "91a2b3c4d5e6f7800000");
    TRY(z = x << 20);  TRY(s = z.str("x"));  TEST_EQUAL(s, "123456789abcdef000000");

    TRY(x = {});
    TEST(! x.get_bit(0));
    TEST(! x.get_bit(100));

    TRY(x.set_bit(16));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(15));
    TEST(x.get_bit(16));
    TEST(! x.get_bit(17));

    TRY(x.set_bit(80));
    TEST_EQUAL(x, MPN("0x100000000000000010000"));
    TEST(! x.get_bit(79));
    TEST(x.get_bit(80));
    TEST(! x.get_bit(81));

    TRY(x.set_bit(80, false));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(80));

    TRY(x.flip_bit(80));
    TEST_EQUAL(x, MPN("0x100000000000000010000"));
    TEST(x.get_bit(80));

    TRY(x.flip_bit(80));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(80));

}

void test_crow_mp_integer_unsigned_byte_operations() {

    MPN a, b;
    ByteVector v;

    TEST_EQUAL(a.bytes(), 0u);

    TRY(a = MPN("0x12"));                    TEST_EQUAL(a.bytes(), 1u);
    TRY(a = MPN("0x1234"));                  TEST_EQUAL(a.bytes(), 2u);
    TRY(a = MPN("0x123456"));                TEST_EQUAL(a.bytes(), 3u);
    TRY(a = MPN("0x12345678"));              TEST_EQUAL(a.bytes(), 4u);
    TRY(a = MPN("0x123456789a"));            TEST_EQUAL(a.bytes(), 5u);
    TRY(a = MPN("0x123456789abc"));          TEST_EQUAL(a.bytes(), 6u);
    TRY(a = MPN("0x123456789abcde"));        TEST_EQUAL(a.bytes(), 7u);
    TRY(a = MPN("0x123456789abcdef1"));      TEST_EQUAL(a.bytes(), 8u);
    TRY(a = MPN("0x123456789abcdef123"));    TEST_EQUAL(a.bytes(), 9u);
    TRY(a = MPN("0x123456789abcdef12345"));  TEST_EQUAL(a.bytes(), 10u);

    TEST_EQUAL(a.get_byte(0), 0x45);
    TEST_EQUAL(a.get_byte(1), 0x23);
    TEST_EQUAL(a.get_byte(2), 0xf1);
    TEST_EQUAL(a.get_byte(3), 0xde);
    TEST_EQUAL(a.get_byte(4), 0xbc);
    TEST_EQUAL(a.get_byte(5), 0x9a);
    TEST_EQUAL(a.get_byte(6), 0x78);
    TEST_EQUAL(a.get_byte(7), 0x56);
    TEST_EQUAL(a.get_byte(8), 0x34);
    TEST_EQUAL(a.get_byte(9), 0x12);
    TEST_EQUAL(a.get_byte(10), 0u);
    TEST_EQUAL(a.get_byte(11), 0u);
    TEST_EQUAL(a.get_byte(12), 0u);
    TEST_EQUAL(a.get_byte(13), 0u);
    TEST_EQUAL(a.get_byte(14), 0u);
    TEST_EQUAL(a.get_byte(15), 0u);
    TEST_EQUAL(a.get_byte(16), 0u);

    TRY(a.set_byte(1, 0xff));   TEST_EQUAL(a.str("x"), "123456789abcdef1ff45");
    TRY(a.set_byte(3, 0xff));   TEST_EQUAL(a.str("x"), "123456789abcfff1ff45");
    TRY(a.set_byte(5, 0xff));   TEST_EQUAL(a.str("x"), "12345678ffbcfff1ff45");
    TRY(a.set_byte(7, 0xff));   TEST_EQUAL(a.str("x"), "1234ff78ffbcfff1ff45");
    TRY(a.set_byte(9, 0xff));   TEST_EQUAL(a.str("x"), "ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(11, 0xff));  TEST_EQUAL(a.str("x"), "ff00ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(13, 0xff));  TEST_EQUAL(a.str("x"), "ff00ff00ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(15, 0xff));  TEST_EQUAL(a.str("x"), "ff00ff00ff00ff34ff78ffbcfff1ff45");

    TRY(a = 0);
    TRY(b = MPN("0x123456789abcdef12345"));

    v.resize(7);

    TRY(a.write_be(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "00 00 00 00 00 00 00");
    TRY(b.write_be(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "78 9a bc de f1 23 45");

    TRY(a.write_le(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "00 00 00 00 00 00 00");
    TRY(b.write_le(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "45 23 f1 de bc 9a 78");

    v = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

    TRY(a = MPN::read_be(v.data(), v.size()));  TEST_EQUAL(a.str("x"), "112233445566778899aabbccddeeff");
    TRY(a = MPN::read_le(v.data(), v.size()));  TEST_EQUAL(a.str("x"), "ffeeddccbbaa998877665544332211");

}
