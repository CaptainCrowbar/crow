#include "crow/spectrum.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <limits>

using namespace Crow;
using namespace Crow::Literals;

namespace {

    constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

}

#define TEST_STAR_PROPERTIES(sp, ll, lm, lt) \
    if (std::isnan(ll)) { \
        TEST(std::isnan(sp.log_l())) \
    } else { \
        TEST_NEAR(sp.log_l(), ll, 0.001) \
    } \
    if (std::isnan(lm)) { \
        TEST(std::isnan(sp.log_m())) \
    } else { \
        TEST_NEAR(sp.log_m(), lm, 0.001) \
    } \
    if (std::isnan(lt)) { \
        TEST(std::isnan(sp.log_t())) \
    } else { \
        TEST_NEAR(sp.log_t(), lt, 0.001) \
    }

void test_crow_spectrum_basic_properties() {

    Spectrum sp;

    TRY(sp = "O5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "O5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "O5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "O5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "O5 IV"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "O5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "O5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "B5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "B5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "B5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "B5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "B5 IV"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "B5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "B5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "A5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "A5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "A5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "A5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "A5 IV"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "A5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "A5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "F5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "F5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "F5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "F5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "F5 IV"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "F5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "F5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "G5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "G5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "G5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "G5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "G5 IV"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "G5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "G5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "K5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "K5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "K5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "K5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "K5 IV"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "K5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "K5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "M5 Ia"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "M5 Ib"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "M5 II"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "M5 III"_sp);  TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "M5 IV"_sp);   TEST(sp.is_giant());    TEST(! sp.is_main_sequence())
    TRY(sp = "M5 V"_sp);    TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "M5 VI"_sp);   TEST(! sp.is_giant());  TEST(sp.is_main_sequence())
    TRY(sp = "L5"_sp);      TEST(! sp.is_giant());  TEST(! sp.is_main_sequence())
    TRY(sp = "DA5"_sp);     TEST(! sp.is_giant());  TEST(! sp.is_main_sequence())

}

void test_crow_spectrum_physical_properties() {

    TEST_STAR_PROPERTIES("G2 V"_sp,    0.0,     0.0,     3.761)
    TEST_STAR_PROPERTIES("O5 Ia"_sp,   6.50,    1.875,   4.62)
    TEST_STAR_PROPERTIES("B5 Ia"_sp,   5.50,    1.35,    4.18)
    TEST_STAR_PROPERTIES("A5 Ia"_sp,   5.00,    1.15,    3.91)
    TEST_STAR_PROPERTIES("F5 Ia"_sp,   5.00,    1.05,    3.82)
    TEST_STAR_PROPERTIES("G5 Ia"_sp,   5.00,    1.10,    3.74)
    TEST_STAR_PROPERTIES("K5 Ia"_sp,   5.00,    1.15,    3.64)
    TEST_STAR_PROPERTIES("M5 Ia"_sp,   5.50,    1.15,    3.51)
    TEST_STAR_PROPERTIES("O5 Ib"_sp,   6.40,    1.85,    4.62)
    TEST_STAR_PROPERTIES("B5 Ib"_sp,   5.00,    1.30,    4.18)
    TEST_STAR_PROPERTIES("A5 Ib"_sp,   4.50,    1.10,    3.91)
    TEST_STAR_PROPERTIES("F5 Ib"_sp,   4.50,    1.00,    3.82)
    TEST_STAR_PROPERTIES("G5 Ib"_sp,   4.50,    1.05,    3.74)
    TEST_STAR_PROPERTIES("K5 Ib"_sp,   4.50,    1.10,    3.64)
    TEST_STAR_PROPERTIES("M5 Ib"_sp,   5.00,    1.10,    3.51)
    TEST_STAR_PROPERTIES("O5 II"_sp,   6.30,    1.825,   4.62)
    TEST_STAR_PROPERTIES("B5 II"_sp,   4.50,    1.075,   4.18)
    TEST_STAR_PROPERTIES("A5 II"_sp,   3.75,    0.775,   3.91)
    TEST_STAR_PROPERTIES("F5 II"_sp,   3.35,    0.625,   3.82)
    TEST_STAR_PROPERTIES("G5 II"_sp,   3.10,    0.55,    3.74)
    TEST_STAR_PROPERTIES("K5 II"_sp,   3.35,    0.5875,  3.64)
    TEST_STAR_PROPERTIES("M5 II"_sp,   4.05,    0.6125,  3.51)
    TEST_STAR_PROPERTIES("O5 III"_sp,  6.20,    1.80,    4.62)
    TEST_STAR_PROPERTIES("B5 III"_sp,  4.00,    0.85,    4.18)
    TEST_STAR_PROPERTIES("A5 III"_sp,  3.00,    0.45,    3.91)
    TEST_STAR_PROPERTIES("F5 III"_sp,  2.20,    0.25,    3.82)
    TEST_STAR_PROPERTIES("G5 III"_sp,  1.70,    0.05,    3.74)
    TEST_STAR_PROPERTIES("K5 III"_sp,  2.20,    0.075,   3.64)
    TEST_STAR_PROPERTIES("M5 III"_sp,  3.10,    0.125,   3.51)
    TEST_STAR_PROPERTIES("O5 IV"_sp,   6.10,    1.775,   4.62)
    TEST_STAR_PROPERTIES("B5 IV"_sp,   3.50,    0.80,    4.18)
    TEST_STAR_PROPERTIES("A5 IV"_sp,   2.10,    0.375,   3.91)
    TEST_STAR_PROPERTIES("F5 IV"_sp,   2.00,    0.20,    3.82)
    TEST_STAR_PROPERTIES("G5 IV"_sp,   1.50,    0.025,   3.74)
    TEST_STAR_PROPERTIES("K5 IV"_sp,   2.00,    0.05,    3.64)
    TEST_STAR_PROPERTIES("M5 IV"_sp,   2.90,    0.10,    3.51)
    TEST_STAR_PROPERTIES("O5 V"_sp,    6.00,    1.75,    4.62)
    TEST_STAR_PROPERTIES("B5 V"_sp,    3.00,    0.75,    4.18)
    TEST_STAR_PROPERTIES("A5 V"_sp,    1.20,    0.30,    3.91)
    TEST_STAR_PROPERTIES("F5 V"_sp,    0.60,    0.15,    3.82)
    TEST_STAR_PROPERTIES("G5 V"_sp,    -0.12,   -0.03,   3.74)
    TEST_STAR_PROPERTIES("K5 V"_sp,    -0.72,   -0.18,   3.64)
    TEST_STAR_PROPERTIES("M5 V"_sp,    -2.22,   -0.68,   3.51)
    TEST_STAR_PROPERTIES("G5 VI"_sp,   -0.52,   -0.03,   3.74)
    TEST_STAR_PROPERTIES("K5 VI"_sp,   -1.12,   -0.18,   3.64)
    TEST_STAR_PROPERTIES("M5 VI"_sp,   -2.62,   -0.68,   3.51)
    TEST_STAR_PROPERTIES("L5"_sp,      -3.945,  -1.375,  3.275)
    TEST_STAR_PROPERTIES("T5"_sp,      -5.445,  -1.625,  2.90)
    TEST_STAR_PROPERTIES("Y2"_sp,      -6.645,  -1.80,   2.60)
    TEST_STAR_PROPERTIES("DA5"_sp,     -2.831,  -0.301,  4.003)
    TEST_STAR_PROPERTIES("DB5"_sp,     -2.831,  -0.301,  4.003)
    TEST_STAR_PROPERTIES("DC5"_sp,     -2.831,  -0.301,  4.003)
    TEST_STAR_PROPERTIES("WC5"_sp,     5.5,     1.15,    4.92)
    TEST_STAR_PROPERTIES("WN5"_sp,     5.8,     1.4,     4.78)
    TEST_STAR_PROPERTIES("WO4"_sp,     5.8,     1.4,     5.20)
    TEST_STAR_PROPERTIES("C5-H"_sp,    3.1,     0.5,     3.50)
    TEST_STAR_PROPERTIES("C5-Hd"_sp,   3.8,     0.5,     3.50)
    TEST_STAR_PROPERTIES("C5-J"_sp,    3.8,     0.7,     3.50)
    TEST_STAR_PROPERTIES("C5-N"_sp,    3.2,     0.6,     3.50)
    TEST_STAR_PROPERTIES("C5-R"_sp,    2.3,     0.1,     3.50)
    TEST_STAR_PROPERTIES("S5"_sp,      3.85,    0.45,    3.50)
    TEST_STAR_PROPERTIES("NS"_sp,      NaN,     0.176,   NaN)
    TEST_STAR_PROPERTIES("BH"_sp,      NaN,     1.0,     NaN)

}
