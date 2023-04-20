#include "crow/fixed-binary.hpp"
#include "crow/unit-test.hpp"
#include <unordered_set>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_fixed_binary_implementation_selection() {

    TEST_TYPE(Binary<1>, SmallBinary<1>);
    TEST_TYPE(Binary<5>, SmallBinary<5>);
    TEST_TYPE(Binary<10>, SmallBinary<10>);
    TEST_TYPE(Binary<20>, SmallBinary<20>);
    TEST_TYPE(Binary<30>, SmallBinary<30>);
    TEST_TYPE(Binary<40>, SmallBinary<40>);
    TEST_TYPE(Binary<50>, SmallBinary<50>);
    TEST_TYPE(Binary<60>, SmallBinary<60>);
    TEST_TYPE(Binary<64>, SmallBinary<64>);
    TEST_TYPE(Binary<65>, LargeBinary<65>);
    TEST_TYPE(Binary<70>, LargeBinary<70>);
    TEST_TYPE(Binary<80>, LargeBinary<80>);
    TEST_TYPE(Binary<90>, LargeBinary<90>);
    TEST_TYPE(Binary<100>, LargeBinary<100>);
    TEST_TYPE(Binary<200>, LargeBinary<200>);
    TEST_TYPE(Binary<500>, LargeBinary<500>);
    TEST_TYPE(Binary<1000>, LargeBinary<1000>);

}

namespace {

    template <typename T1, typename T2>
    void do_conversion_tests() {

        const auto ca = 0x0123456789abcdefull;
        const auto cb = ~ ca;
        const auto mask1 = uint64_t(T1::max());
        const auto mask2 = uint64_t(T2::max());

        T1 x1, y1, z1;
        T2 x2, y2, z2;

        TRY(x1 = static_cast<T1>(ca));  TEST_EQUAL(uint64_t(x1), ca & mask1);
        TRY(y2 = static_cast<T2>(x1));  TEST_EQUAL(uint64_t(y2), ca & mask1 & mask2);
        TRY(z1 = static_cast<T1>(y2));  TEST_EQUAL(uint64_t(z1), uint64_t(y2));
        TRY(x2 = static_cast<T2>(ca));  TEST_EQUAL(uint64_t(x2), ca & mask2);
        TRY(y1 = static_cast<T1>(x2));  TEST_EQUAL(uint64_t(y1), ca & mask2 & mask1);
        TRY(z2 = static_cast<T2>(y1));  TEST_EQUAL(uint64_t(z2), uint64_t(y1));
        TRY(x1 = static_cast<T1>(cb));  TEST_EQUAL(uint64_t(x1), cb & mask1);
        TRY(y2 = static_cast<T2>(x1));  TEST_EQUAL(uint64_t(y2), cb & mask1 & mask2);
        TRY(z1 = static_cast<T1>(y2));  TEST_EQUAL(uint64_t(z1), uint64_t(y2));
        TRY(x2 = static_cast<T2>(cb));  TEST_EQUAL(uint64_t(x2), cb & mask2);
        TRY(y1 = static_cast<T1>(x2));  TEST_EQUAL(uint64_t(y1), cb & mask2 & mask1);
        TRY(z2 = static_cast<T2>(y1));  TEST_EQUAL(uint64_t(z2), uint64_t(y1));

    }

}

void test_crow_fixed_binary_type_conversions() {

    do_conversion_tests<SmallBinary<10>, SmallBinary<31>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<32>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<33>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<63>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<64>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<31>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<32>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<33>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<63>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<64>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<31>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<32>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<33>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<63>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<64>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<31>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<32>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<33>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<63>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<64>>();

}

void test_crow_fixed_binary_string_parsing() {

    SmallBinary<35> x;
    LargeBinary<35> y;

    TEST(! SmallBinary<35>::try_parse_dec("", x));
    TEST(! SmallBinary<35>::try_parse_dec("a", x));
    TEST(! SmallBinary<35>::try_parse_hex("123456789xyz", x));
    TEST(SmallBinary<35>::try_parse_dec("0", x));                   TEST_EQUAL(uint64_t(x), 0u);
    TEST(SmallBinary<35>::try_parse_dec("42", x));                  TEST_EQUAL(uint64_t(x), 42u);
    TEST(SmallBinary<35>::try_parse_dec("1234567890", x));          TEST_EQUAL(uint64_t(x), 1234567890ull);
    TEST(SmallBinary<35>::try_parse_hex("123456789", x));           TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(SmallBinary<35>::try_parse_bin("101010110011001100", x));  TEST_EQUAL(uint64_t(x), 0x00002acccull);

    TEST(! LargeBinary<35>::try_parse_dec("", y));
    TEST(! LargeBinary<35>::try_parse_dec("a", y));
    TEST(! LargeBinary<35>::try_parse_hex("123456789xyz", y));
    TEST(LargeBinary<35>::try_parse_dec("0", y));                   TEST_EQUAL(uint64_t(y), 0u);
    TEST(LargeBinary<35>::try_parse_dec("42", y));                  TEST_EQUAL(uint64_t(y), 42u);
    TEST(LargeBinary<35>::try_parse_dec("1234567890", y));          TEST_EQUAL(uint64_t(y), 1234567890ull);
    TEST(LargeBinary<35>::try_parse_hex("123456789", y));           TEST_EQUAL(uint64_t(y), 0x123456789ull);
    TEST(LargeBinary<35>::try_parse_bin("101010110011001100", y));  TEST_EQUAL(uint64_t(y), 0x00002acccull);

    Uint128 u;

    TRY(u = 123456789123456789123456789123456789_u128);
    TEST_EQUAL(u.dec(), "123456789123456789123456789123456789");
    TRY(u = 0x123456789abcdef123456789abcdef_u128);
    TEST_EQUAL(u.hex(), "00123456789abcdef123456789abcdef");
    TRY(u = 0b10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010_u128);
    TEST_EQUAL(u.bin(), "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010");

    TRY(u = 123'456'789'123'456'789'123'456'789'123'456'789_u128);
    TEST_EQUAL(u.dec(), "123456789123456789123456789123456789");
    TRY(u = 0x12'3456'789a'bcde'f123'4567'89ab'cdef_u128);
    TEST_EQUAL(u.hex(), "00123456789abcdef123456789abcdef");
    TRY(u = 0b10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010'10101010_u128);
    TEST_EQUAL(u.bin(), "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010");

}

void test_crow_fixed_binary_hash_set() {

    std::unordered_set<SmallBinary<5>> set_s5;
    std::unordered_set<SmallBinary<50>> set_s50;
    std::unordered_set<LargeBinary<50>> set_l50;
    std::unordered_set<LargeBinary<500>> set_l500;

    TEST(set_s5.empty());
    TEST(set_s50.empty());
    TEST(set_l50.empty());
    TEST(set_l500.empty());

    for (int i = 1; i <= 10; ++i) {
        TRY(set_s5.insert(SmallBinary<5>(i)));
        TRY(set_s50.insert(SmallBinary<50>(i)));
        TRY(set_l50.insert(LargeBinary<50>(i)));
        TRY(set_l500.insert(LargeBinary<500>(i)));
    }

    TEST_EQUAL(set_s5.size(), 10u);
    TEST_EQUAL(set_s50.size(), 10u);
    TEST_EQUAL(set_l50.size(), 10u);
    TEST_EQUAL(set_l500.size(), 10u);

}
