#include "crow/fixed-binary.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_fixed_binary_small_binary_35() {

    using binary = SmallBinary<35>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(uint64_t(binary::max()), 0x7ffffffffull);
    TEST_EQUAL(binary::max().hex(), "7ffffffff");

    TEST(c.fits_in<int64_t>());  TEST_EQUAL(int64_t(c), 42);  TEST(c);
    TEST(x.fits_in<int64_t>());  TEST_EQUAL(int64_t(x), 0);   TEST(! x);

    TRY(x = binary(0x123456789ull));
    TRY(y = binary(0xabcdefull));

    TEST_EQUAL(int64_t(x), 4886718345);
    TEST_EQUAL(int64_t(y), 11259375);
    TEST_EQUAL(int64_t(z), 0);

    TEST_EQUAL(x.bin(), "00100100011010001010110011110001001");
    TEST_EQUAL(y.bin(), "00000000000101010111100110111101111");
    TEST_EQUAL(z.bin(), "00000000000000000000000000000000000");
    TEST_EQUAL(x.dec(), "4886718345");
    TEST_EQUAL(y.dec(), "11259375");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "123456789");
    TEST_EQUAL(y.hex(), "000abcdef");
    TEST_EQUAL(z.hex(), "000000000");
    TEST_EQUAL(double(x), 4886718345.0);
    TEST_EQUAL(double(y), 11259375.0);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 33u);
    TEST_EQUAL(y.significant_bits(), 24u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = - x);  TEST_EQUAL(uint64_t(z), 0x6dcba9877ull);
    TRY(z = - y);  TEST_EQUAL(uint64_t(z), 0x7ff543211ull);

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(uint64_t(z), 0x7ffffffffull);
    TRY(z = ~ x);  TEST_EQUAL(uint64_t(z), 0x6dcba9876ull);
    TRY(z = ~ y);  TEST_EQUAL(uint64_t(z), 0x7ff543210ull);

    TRY(z = x + y);  TEST_EQUAL(uint64_t(z), 0x123f13578ull);
    TRY(z = x - y);  TEST_EQUAL(uint64_t(z), 0x12299999aull);
    TRY(z = y - x);  TEST_EQUAL(uint64_t(z), 0x6dd666666ull);
    TRY(z = x * y);  TEST_EQUAL(uint64_t(z), 0x2aa375de7ull);
    TRY(z = x / y);  TEST_EQUAL(uint64_t(z), 0x0000001b2ull);
    TRY(z = x % y);  TEST_EQUAL(uint64_t(z), 0x00002485bull);
    TRY(z = y / x);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = y % x);  TEST_EQUAL(uint64_t(z), 0x000abcdefull);
    TRY(z = x & y);  TEST_EQUAL(uint64_t(z), 0x000014589ull);
    TRY(z = x | y);  TEST_EQUAL(uint64_t(z), 0x123efefefull);
    TRY(z = x ^ y);  TEST_EQUAL(uint64_t(z), 0x123eeaa66ull);

    TRY(z = x << 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);  TRY(z = x >> 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);
    TRY(z = x << 5);   TEST_EQUAL(uint64_t(z), 0x468acf120ull);  TRY(z = x >> 5);   TEST_EQUAL(uint64_t(z), 0x0091a2b3cull);
    TRY(z = x << 10);  TEST_EQUAL(uint64_t(z), 0x5159e2400ull);  TRY(z = x >> 10);  TEST_EQUAL(uint64_t(z), 0x00048d159ull);
    TRY(z = x << 15);  TEST_EQUAL(uint64_t(z), 0x2b3c48000ull);  TRY(z = x >> 15);  TEST_EQUAL(uint64_t(z), 0x00002468aull);
    TRY(z = x << 20);  TEST_EQUAL(uint64_t(z), 0x678900000ull);  TRY(z = x >> 20);  TEST_EQUAL(uint64_t(z), 0x000001234ull);
    TRY(z = x << 25);  TEST_EQUAL(uint64_t(z), 0x712000000ull);  TRY(z = x >> 25);  TEST_EQUAL(uint64_t(z), 0x000000091ull);
    TRY(z = x << 30);  TEST_EQUAL(uint64_t(z), 0x240000000ull);  TRY(z = x >> 30);  TEST_EQUAL(uint64_t(z), 0x000000004ull);
    TRY(z = x << 35);  TEST_EQUAL(uint64_t(z), 0u);              TRY(z = x >> 35);  TEST_EQUAL(uint64_t(z), 0u);

    TEST(++x);    TEST_EQUAL(uint64_t(x), 0x12345678aull);
    TEST(--x);    TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(x--);    TEST_EQUAL(uint64_t(x), 0x123456788ull);
    TEST(x++);    TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(++y);    TEST_EQUAL(uint64_t(y), 0xabcdf0ull);
    TEST(--y);    TEST_EQUAL(uint64_t(y), 0xabcdefull);
    TEST(y--);    TEST_EQUAL(uint64_t(y), 0xabcdeeull);
    TEST(y++);    TEST_EQUAL(uint64_t(y), 0xabcdefull);
    TEST(++z);    TEST_EQUAL(uint64_t(z), 1u);
    TEST(! --z);  TEST_EQUAL(uint64_t(z), 0u);
    TEST(! z--);  TEST_EQUAL(uint64_t(z), 0x7ffffffffull);
    TEST(z++);    TEST_EQUAL(uint64_t(z), 0u);

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TRY(x = rotl(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 5));   TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 10));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 15));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 20));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 25));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 30));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 40));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 45));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 50));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 55));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 60));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 65));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = rotr(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 5));   TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 10));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 15));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 20));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 25));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 30));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 40));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 45));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 50));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 55));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 60));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 65));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = binary::from_double(12345678901.0));
    TEST_EQUAL(x.dec(), "12345678901");
    TEST_EQUAL(double(x), 12345678901.0);
    TEST_EQUAL(uint64_t(x), 12345678901ull);

    TRY(x = 0ull);      TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 127ull);    TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 128ull);    TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32767ull);  TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32768ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());

    TRY(x = 0ull);      TEST_EQUAL(int8_t(x), 0);    TEST_EQUAL(int16_t(x), 0);      TEST_EQUAL(int32_t(x), 0);      TEST_EQUAL(int64_t(x), 0ll);
    TRY(x = 127ull);    TEST_EQUAL(int8_t(x), 127);  TEST_EQUAL(int16_t(x), 127);    TEST_EQUAL(int32_t(x), 127);    TEST_EQUAL(int64_t(x), 127ll);
    TRY(x = 128ull);    /**/                         TEST_EQUAL(int16_t(x), 128);    TEST_EQUAL(int32_t(x), 128);    TEST_EQUAL(int64_t(x), 128ll);
    TRY(x = 32767ull);  /**/                         TEST_EQUAL(int16_t(x), 32767);  TEST_EQUAL(int32_t(x), 32767);  TEST_EQUAL(int64_t(x), 32767ll);
    TRY(x = 32768ull);  /**/                         /**/                            TEST_EQUAL(int32_t(x), 32768);  TEST_EQUAL(int64_t(x), 32768ll);

}
