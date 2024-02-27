#include "crow/enum.hpp"
#include "crow/unit-test.hpp"
#include <cstdint>

using namespace Crow;

namespace {

    CROW_ENUM_BITMASK(Mask, std::uint16_t,
        none     = 0,
        alpha    = 1,
        bravo    = 2,
        charlie  = 4,
    )

}

void test_crow_enum_bitmask() {

    Mask m = {};

    TEST(! m);
    TEST_EQUAL(~ m, Mask(65535));

    m = Mask::alpha & Mask::charlie;      TEST_EQUAL(m, Mask::none);   TEST_EQUAL(to_string(m), "none");
    m = Mask::alpha | Mask::charlie;      TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");
    m = Mask::alpha ^ Mask::charlie;      TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");
    m = Mask(7) & Mask::bravo;            TEST_EQUAL(m, Mask::bravo);  TEST_EQUAL(to_string(m), "bravo");
    m = Mask(7) | Mask::bravo;            TEST_EQUAL(m, Mask(7));      TEST_EQUAL(to_string(m), "7");
    m = Mask(7) ^ Mask::bravo;            TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");
    m = Mask::alpha; m &= Mask::charlie;  TEST_EQUAL(m, Mask::none);   TEST_EQUAL(to_string(m), "none");
    m = Mask::alpha; m |= Mask::charlie;  TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");
    m = Mask::alpha; m ^= Mask::charlie;  TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");
    m = Mask(7); m &= Mask::bravo;        TEST_EQUAL(m, Mask::bravo);  TEST_EQUAL(to_string(m), "bravo");
    m = Mask(7); m |= Mask::bravo;        TEST_EQUAL(m, Mask(7));      TEST_EQUAL(to_string(m), "7");
    m = Mask(7); m ^= Mask::bravo;        TEST_EQUAL(m, Mask(5));      TEST_EQUAL(to_string(m), "5");

}
