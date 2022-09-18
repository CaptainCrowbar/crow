#include "crow/fixed-binary.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_fixed_binary_small_binary_5() {

    using binary = SmallBinary<5>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(int(binary::max()), 31);

    TEST(c.fits_in<int>());  TEST_EQUAL(int(c), 10);  TEST(c);
    TEST(x.fits_in<int>());  TEST_EQUAL(int(x), 0);   TEST(! x);

    TRY(x = binary(25));
    TRY(y = binary(10));

    TEST_EQUAL(int(x), 25);
    TEST_EQUAL(int(y), 10);
    TEST_EQUAL(int(z), 0);

    TEST_EQUAL(x.bin(), "11001");
    TEST_EQUAL(y.bin(), "01010");
    TEST_EQUAL(z.bin(), "00000");
    TEST_EQUAL(x.dec(), "25");
    TEST_EQUAL(y.dec(), "10");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "19");
    TEST_EQUAL(y.hex(), "0a");
    TEST_EQUAL(z.hex(), "00");
    TEST_EQUAL(double(x), 25.0);
    TEST_EQUAL(double(y), 10.0);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 5u);
    TEST_EQUAL(y.significant_bits(), 4u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(int(z), 0);
    TRY(z = - x);  TEST_EQUAL(int(z), 7);
    TRY(z = - y);  TEST_EQUAL(int(z), 22);

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(int(z), 31);
    TRY(z = ~ x);  TEST_EQUAL(int(z), 6);
    TRY(z = ~ y);  TEST_EQUAL(int(z), 21);

    TRY(z = x + y);  TEST_EQUAL(int(z), 3);
    TRY(z = x - y);  TEST_EQUAL(int(z), 15);
    TRY(z = y - x);  TEST_EQUAL(int(z), 17);
    TRY(z = x * y);  TEST_EQUAL(int(z), 26);
    TRY(z = x / y);  TEST_EQUAL(int(z), 2);
    TRY(z = x % y);  TEST_EQUAL(int(z), 5);
    TRY(z = y / x);  TEST_EQUAL(int(z), 0);
    TRY(z = y % x);  TEST_EQUAL(int(z), 10);
    TRY(z = x & y);  TEST_EQUAL(int(z), 8);
    TRY(z = x | y);  TEST_EQUAL(int(z), 27);
    TRY(z = x ^ y);  TEST_EQUAL(int(z), 19);

    TRY(z = x << 0);   TEST_EQUAL(int(z), 25);  TRY(z = x >> 0);   TEST_EQUAL(int(z), 25);
    TRY(z = x << 1);   TEST_EQUAL(int(z), 18);  TRY(z = x >> 1);   TEST_EQUAL(int(z), 12);
    TRY(z = x << 2);   TEST_EQUAL(int(z), 4);   TRY(z = x >> 2);   TEST_EQUAL(int(z), 6);
    TRY(z = x << 3);   TEST_EQUAL(int(z), 8);   TRY(z = x >> 3);   TEST_EQUAL(int(z), 3);
    TRY(z = x << 4);   TEST_EQUAL(int(z), 16);  TRY(z = x >> 4);   TEST_EQUAL(int(z), 1);
    TRY(z = x << 5);   TEST_EQUAL(int(z), 0);   TRY(z = x >> 5);   TEST_EQUAL(int(z), 0);

    TEST(++x);    TEST_EQUAL(int(x), 26);
    TEST(--x);    TEST_EQUAL(int(x), 25);
    TEST(x--);    TEST_EQUAL(int(x), 24);
    TEST(x++);    TEST_EQUAL(int(x), 25);
    TEST(++y);    TEST_EQUAL(int(y), 11);
    TEST(--y);    TEST_EQUAL(int(y), 10);
    TEST(y--);    TEST_EQUAL(int(y), 9);
    TEST(y++);    TEST_EQUAL(int(y), 10);
    TEST(++z);    TEST_EQUAL(int(z), 1);
    TEST(! --z);  TEST_EQUAL(int(z), 0);
    TEST(! z--);  TEST_EQUAL(int(z), 31);
    TEST(z++);    TEST_EQUAL(int(z), 0);

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TRY(x = rotl(y, 0));   TEST_EQUAL(int(x), 10);
    TRY(x = rotl(y, 1));   TEST_EQUAL(int(x), 20);
    TRY(x = rotl(y, 2));   TEST_EQUAL(int(x), 9);
    TRY(x = rotl(y, 3));   TEST_EQUAL(int(x), 18);
    TRY(x = rotl(y, 4));   TEST_EQUAL(int(x), 5);
    TRY(x = rotl(y, 5));   TEST_EQUAL(int(x), 10);
    TRY(x = rotl(y, 6));   TEST_EQUAL(int(x), 20);
    TRY(x = rotl(y, 7));   TEST_EQUAL(int(x), 9);
    TRY(x = rotl(y, 8));   TEST_EQUAL(int(x), 18);
    TRY(x = rotl(y, 9));   TEST_EQUAL(int(x), 5);
    TRY(x = rotl(y, 10));  TEST_EQUAL(int(x), 10);

    TRY(x = rotr(y, 0));   TEST_EQUAL(int(x), 10);
    TRY(x = rotr(y, 1));   TEST_EQUAL(int(x), 5);
    TRY(x = rotr(y, 2));   TEST_EQUAL(int(x), 18);
    TRY(x = rotr(y, 3));   TEST_EQUAL(int(x), 9);
    TRY(x = rotr(y, 4));   TEST_EQUAL(int(x), 20);
    TRY(x = rotr(y, 5));   TEST_EQUAL(int(x), 10);
    TRY(x = rotr(y, 6));   TEST_EQUAL(int(x), 5);
    TRY(x = rotr(y, 7));   TEST_EQUAL(int(x), 18);
    TRY(x = rotr(y, 8));   TEST_EQUAL(int(x), 9);
    TRY(x = rotr(y, 9));   TEST_EQUAL(int(x), 20);
    TRY(x = rotr(y, 10));  TEST_EQUAL(int(x), 10);

    TRY(x = binary::from_double(25.0));
    TEST_EQUAL(x.dec(), "25");
    TEST_EQUAL(double(x), 25.0);
    TEST_EQUAL(int(x), 25);

}
