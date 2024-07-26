#include "crow/fixed-binary.hpp"
#include "crow/unit-test.hpp"
#include <limits>

using namespace Crow;

namespace {

    const auto max_int32 = std::numeric_limits<int32_t>::max();
    const auto max_int64 = std::numeric_limits<int64_t>::max();

}

void test_crow_fixed_binary_large_binary_100() {

    using binary = LargeBinary<100>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(binary::max().hex(), "fffffffffffffffffffffffff");

    TEST(c.fits_in<int64_t>());  TEST_EQUAL(int64_t(c), 42);  TEST(c);
    TEST(x.fits_in<int64_t>());  TEST_EQUAL(int64_t(x), 0);   TEST(! x);

    TRY((x = binary{0xfedcba987ull, 0x6543210fedcba987ull}));
    TRY((y = binary{0x123456789ull, 0xabcdef0123456789ull}));

    TEST_EQUAL(uint64_t(x), 0x6543210fedcba987ull);
    TEST_EQUAL(uint64_t(y), 0xabcdef0123456789ull);
    TEST_EQUAL(uint64_t(z), 0u);

    TEST_EQUAL(x.bin(), "1111111011011100101110101001100001110110010101000011001000010000111111101101110010111010100110000111");
    TEST_EQUAL(y.bin(), "0001001000110100010101100111100010011010101111001101111011110000000100100011010001010110011110001001");
    TEST_EQUAL(z.bin(), "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    TEST_EQUAL(x.dec(), "1262016597560548382007796410759");
    TEST_EQUAL(y.dec(), "90144042682896311822508713865");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TEST_NEAR(double(x), 1.262016598e30, 1e21);
    TEST_NEAR(double(y), 9.014404268e28, 1e19);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 100u);
    TEST_EQUAL(y.significant_bits(), 97u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TRY(z = - x);  TEST_EQUAL(z.hex(), "0123456789abcdef012345679");
    TRY(z = - y);  TEST_EQUAL(z.hex(), "edcba9876543210fedcba9877");

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(z.hex(), "fffffffffffffffffffffffff");
    TRY(z = ~ x);  TEST_EQUAL(z.hex(), "0123456789abcdef012345678");
    TRY(z = ~ y);  TEST_EQUAL(z.hex(), "edcba9876543210fedcba9876");

    TRY(z = x + y);  TEST_EQUAL(z.hex(), "1111111111111101111111110");
    TRY(z = x - y);  TEST_EQUAL(z.hex(), "eca8641fdb975320eca8641fe");
    TRY(z = y - x);  TEST_EQUAL(z.hex(), "13579be02468acdf13579be02");
    TRY(z = x * y);  TEST_EQUAL(z.hex(), "9efd92c744933bccc59960a3f");
    TRY(z = x / y);  TEST_EQUAL(z.hex(), "000000000000000000000000e");
    TRY(z = x % y);  TEST_EQUAL(z.hex(), "00000000000000f0000000009");
    TRY(z = y / x);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TRY(z = y % x);  TEST_EQUAL(z.hex(), "123456789abcdef0123456789");
    TRY(z = x & y);  TEST_EQUAL(z.hex(), "1214121812141210121412181");
    TRY(z = x | y);  TEST_EQUAL(z.hex(), "fefcfef8fefcfef0fefcfef8f");
    TRY(z = x ^ y);  TEST_EQUAL(z.hex(), "ece8ece0ece8ece0ece8ece0e");

    TRY(z = x << 0);    TEST_EQUAL(z.hex(), "fedcba9876543210fedcba987");  TRY(z = x >> 0);    TEST_EQUAL(z.hex(), "fedcba9876543210fedcba987");
    TRY(z = x << 10);   TEST_EQUAL(z.hex(), "72ea61d950c843fb72ea61c00");  TRY(z = x >> 10);   TEST_EQUAL(z.hex(), "003fb72ea61d950c843fb72ea");
    TRY(z = x << 20);   TEST_EQUAL(z.hex(), "a9876543210fedcba98700000");  TRY(z = x >> 20);   TEST_EQUAL(z.hex(), "00000fedcba9876543210fedc");
    TRY(z = x << 30);   TEST_EQUAL(z.hex(), "1d950c843fb72ea61c0000000");  TRY(z = x >> 30);   TEST_EQUAL(z.hex(), "00000003fb72ea61d950c843f");
    TRY(z = x << 40);   TEST_EQUAL(z.hex(), "543210fedcba9870000000000");  TRY(z = x >> 40);   TEST_EQUAL(z.hex(), "0000000000fedcba987654321");
    TRY(z = x << 50);   TEST_EQUAL(z.hex(), "c843fb72ea61c000000000000");  TRY(z = x >> 50);   TEST_EQUAL(z.hex(), "0000000000003fb72ea61d950");
    TRY(z = x << 60);   TEST_EQUAL(z.hex(), "0fedcba987000000000000000");  TRY(z = x >> 60);   TEST_EQUAL(z.hex(), "000000000000000fedcba9876");
    TRY(z = x << 70);   TEST_EQUAL(z.hex(), "b72ea61c00000000000000000");  TRY(z = x >> 70);   TEST_EQUAL(z.hex(), "000000000000000003fb72ea6");
    TRY(z = x << 80);   TEST_EQUAL(z.hex(), "ba98700000000000000000000");  TRY(z = x >> 80);   TEST_EQUAL(z.hex(), "00000000000000000000fedcb");
    TRY(z = x << 90);   TEST_EQUAL(z.hex(), "61c0000000000000000000000");  TRY(z = x >> 90);   TEST_EQUAL(z.hex(), "00000000000000000000003fb");
    TRY(z = x << 100);  TEST_EQUAL(z.hex(), "0000000000000000000000000");  TRY(z = x >> 100);  TEST_EQUAL(z.hex(), "0000000000000000000000000");

    TEST(++x);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba988");
    TEST(--x);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST(x--);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba986");
    TEST(x++);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST(++y);    TEST_EQUAL(y.hex(), "123456789abcdef012345678a");
    TEST(--y);    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST(y--);    TEST_EQUAL(y.hex(), "123456789abcdef0123456788");
    TEST(y++);    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST(++z);    TEST_EQUAL(z.hex(), "0000000000000000000000001");
    TEST(! --z);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TEST(! z--);  TEST_EQUAL(z.hex(), "fffffffffffffffffffffffff");
    TEST(z++);    TEST_EQUAL(z.hex(), "0000000000000000000000000");

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TEST_EQUAL(x.dec(), "1262016597560548382007796410759");
    TEST_EQUAL(y.dec(), "90144042682896311822508713865");
    TEST_EQUAL(z.dec(), "0");

    TRY(x = rotl(y, 0));    TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotl(y, 10));   TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotl(y, 20));   TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotl(y, 30));   TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotl(y, 40));   TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotl(y, 50));   TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotl(y, 60));   TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotl(y, 70));   TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotl(y, 80));   TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotl(y, 90));   TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotl(y, 100));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotl(y, 110));  TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotl(y, 120));  TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotl(y, 130));  TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotl(y, 140));  TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotl(y, 150));  TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotl(y, 160));  TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotl(y, 170));  TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotl(y, 180));  TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotl(y, 190));  TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotl(y, 200));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");

    TRY(x = rotr(y, 0));    TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotr(y, 10));   TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotr(y, 20));   TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotr(y, 30));   TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotr(y, 40));   TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotr(y, 50));   TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotr(y, 60));   TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotr(y, 70));   TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotr(y, 80));   TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotr(y, 90));   TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotr(y, 100));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotr(y, 110));  TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotr(y, 120));  TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotr(y, 130));  TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotr(y, 140));  TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotr(y, 150));  TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotr(y, 160));  TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotr(y, 170));  TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotr(y, 180));  TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotr(y, 190));  TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotr(y, 200));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");

    TRY(x = binary::from_double(1.23456789e30));
    TEST_MATCH(x.dec(), "^12345678\\d{23}$");
    TEST_NEAR(double(x), 1.23456789e30, 1e21);

    TRY(x = 0ull);                    TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 127ull);                  TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 128ull);                  TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 32767ull);                TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 32768ull);                TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 2147483647ull);           TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 2147483648ull);           TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 9223372036854775807ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 9223372036854775808ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(! x.fits_in<int64_t>());

    TRY(x = 0ull);                    TEST_EQUAL(int8_t(x), 0);    TEST_EQUAL(int16_t(x), 0);      TEST_EQUAL(int32_t(x), 0);          TEST_EQUAL(int64_t(x), 0);
    TRY(x = 127ull);                  TEST_EQUAL(int8_t(x), 127);  TEST_EQUAL(int16_t(x), 127);    TEST_EQUAL(int32_t(x), 127);        TEST_EQUAL(int64_t(x), 127);
    TRY(x = 128ull);                  /**/                         TEST_EQUAL(int16_t(x), 128);    TEST_EQUAL(int32_t(x), 128);        TEST_EQUAL(int64_t(x), 128);
    TRY(x = 32767ull);                /**/                         TEST_EQUAL(int16_t(x), 32767);  TEST_EQUAL(int32_t(x), 32767);      TEST_EQUAL(int64_t(x), 32767);
    TRY(x = 32768ull);                /**/                         /**/                            TEST_EQUAL(int32_t(x), 32768);      TEST_EQUAL(int64_t(x), 32768);
    TRY(x = 2147483647ull);           /**/                         /**/                            TEST_EQUAL(int32_t(x), max_int32);  TEST_EQUAL(int64_t(x), 2147483647);
    TRY(x = 2147483648ull);           /**/                         /**/                            /**/                                TEST_EQUAL(int64_t(x), 2147483648);
    TRY(x = 9223372036854775807ull);  /**/                         /**/                            /**/                                TEST_EQUAL(int64_t(x), max_int64);

}
