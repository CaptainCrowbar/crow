#include "crow/mp-integer.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <tuple>
#include <unordered_set>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_mp_integer_literals() {

    MPZ i;
    MPN n;

    TRY(i = 0_mpz);                                                                TEST_EQUAL(i.str(), "0");
    TRY(i = 123456789_mpz);                                                        TEST_EQUAL(i.str(), "123456789");
    TRY(i = -123456789_mpz);                                                       TEST_EQUAL(i.str(), "-123456789");
    TRY(i = 123456789123456789_mpz);                                               TEST_EQUAL(i.str(), "123456789123456789");
    TRY(i = -123456789123456789_mpz);                                              TEST_EQUAL(i.str(), "-123456789123456789");
    TRY(i = 123456789123456789123456789123456789123456789_mpz);                    TEST_EQUAL(i.str(), "123456789123456789123456789123456789123456789");
    TRY(i = -123456789123456789123456789123456789123456789_mpz);                   TEST_EQUAL(i.str(), "-123456789123456789123456789123456789123456789");
    TRY(i = 0b10101010101010101010101010101010101010101010101010_mpz);             TEST_EQUAL(i.str(), "750599937895082");
    TRY(i = -0b10101010101010101010101010101010101010101010101010_mpz);            TEST_EQUAL(i.str(), "-750599937895082");
    TRY(i = 0x123456789abcdef123456789abcdef123456789abcdef123456789abcdef_mpz);   TEST_EQUAL(i.str(), "125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(i = -0x123456789abcdef123456789abcdef123456789abcdef123456789abcdef_mpz);  TEST_EQUAL(i.str(), "-125642457939796217460094503631385345882379387509263401568735420576681455");

    TRY(i = 123'456'789_mpz);                                                                    TEST_EQUAL(i.str(), "123456789");
    TRY(i = -123'456'789_mpz);                                                                   TEST_EQUAL(i.str(), "-123456789");
    TRY(i = 123'456'789'123'456'789_mpz);                                                        TEST_EQUAL(i.str(), "123456789123456789");
    TRY(i = -123'456'789'123'456'789_mpz);                                                       TEST_EQUAL(i.str(), "-123456789123456789");
    TRY(i = 123'456'789'123'456'789'123'456'789'123'456'789'123'456'789_mpz);                    TEST_EQUAL(i.str(), "123456789123456789123456789123456789123456789");
    TRY(i = -123'456'789'123'456'789'123'456'789'123'456'789'123'456'789_mpz);                   TEST_EQUAL(i.str(), "-123456789123456789123456789123456789123456789");
    TRY(i = 0b10101'01010'10101'01010'10101'01010'10101'01010'10101'01010_mpz);                  TEST_EQUAL(i.str(), "750599937895082");
    TRY(i = -0b10101'01010'10101'01010'10101'01010'10101'01010'10101'01010_mpz);                 TEST_EQUAL(i.str(), "-750599937895082");
    TRY(i = 0x1234'5678'9abc'def1'2345'6789'abcd'ef12'3456'789a'bcde'f123'4567'89ab'cdef_mpz);   TEST_EQUAL(i.str(), "125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(i = -0x1234'5678'9abc'def1'2345'6789'abcd'ef12'3456'789a'bcde'f123'4567'89ab'cdef_mpz);  TEST_EQUAL(i.str(), "-125642457939796217460094503631385345882379387509263401568735420576681455");

    TRY(n = 0_mpn);                                                                                          TEST_EQUAL(n.str(), "0");
    TRY(n = 0x12345678_mpn);                                                                                 TEST_EQUAL(n.str(), "305419896");
    TRY(n = 0x123456789abcdef0_mpn);                                                                         TEST_EQUAL(n.str(), "1311768467463790320");
    TRY(n = 0b110011000011110111111011111001011100011101100011001111101111100000001000101111100010101_mpn);  TEST_EQUAL(n.str(), "123456789123456789123456789");
    TRY(n = 123456789123456789123456789_mpn);                                                                TEST_EQUAL(n.str(), "123456789123456789123456789");
    TRY(n = 0x661efdf2e3b19f7c045f15_mpn);                                                                   TEST_EQUAL(n.str(), "123456789123456789123456789");

    TRY(n = 0x12'345'678_mpn);                                                                               TEST_EQUAL(n.str(), "305419896");
    TRY(n = 0x1234'5678'9abc'def0_mpn);                                                                      TEST_EQUAL(n.str(), "1311768467463790320");
    TRY(n = 0b110011000011110111111011111001011100011101100011001111101111100000001000101111100010101_mpn);  TEST_EQUAL(n.str(), "123456789123456789123456789");
    TRY(n = 123'456'789'123'456'789'123'456'789_mpn);                                                        TEST_EQUAL(n.str(), "123456789123456789123456789");
    TRY(n = 0x66'1efd'f2e3'b19f'7c04'5f15_mpn);                                                              TEST_EQUAL(n.str(), "123456789123456789123456789");

}

void test_crow_mp_integer_core_functions() {

    MPN a = 42, b = 99, c, d;
    MPZ w = 42, x = -99, y, z;

    TRY(c = std::clamp(a, MPN(0), MPN(10)));  TEST_EQUAL(c, 10);
    TRY(y = std::clamp(w, MPZ(0), MPZ(10)));  TEST_EQUAL(y, 10);

    TRY(c = b / a);  TEST_EQUAL(c, 2);   TRY(c = b % a);  TEST_EQUAL(c, 15);
    TRY(y = x / w);  TEST_EQUAL(y, -3);  TRY(y = x % w);  TEST_EQUAL(y, 27);

    TRY(std::tie(c, d) = divide(b, a));  TEST_EQUAL(c, 2);   TEST_EQUAL(d, 15);
    TRY(std::tie(y, z) = divide(x, w));  TEST_EQUAL(y, -3);  TEST_EQUAL(z, 27);

}

void test_crow_mp_integer_hash_set() {

    std::unordered_set<MPN> uset;
    std::unordered_set<MPZ> iset;

    TEST(uset.empty());
    TEST(iset.empty());

    for (int i = 1; i <= 10; ++i) {
        TRY(uset.insert(MPN(i)));
        TRY(iset.insert(MPZ(i)));
    }

    TEST_EQUAL(uset.size(), 10u);
    TEST_EQUAL(iset.size(), 10u);

}
