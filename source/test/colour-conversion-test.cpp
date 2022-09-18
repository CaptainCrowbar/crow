#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"

using namespace Crow;
using namespace Crow::Test;

void test_crow_colour_conversion_between_colour_spaces() {

    using C_sRGB = Colour<double, sRGB, ColourLayout::forward>;
    using C_AdobeRGB = Colour<double, AdobeRGB, ColourLayout::forward>;
    using C_CIEXYZ = Colour<double, CIEXYZ, ColourLayout::forward>;
    using C_CIExyY = Colour<double, CIExyY, ColourLayout::forward>;
    using C_CIELab = Colour<double, CIELab, ColourLayout::forward>;
    using C_CIELuv = Colour<double, CIELuv, ColourLayout::forward>;
    using C_HSL = Colour<double, HSL, ColourLayout::forward>;
    using C_HSV = Colour<double, HSV, ColourLayout::forward>;

    C_sRGB sRGB_1, sRGB_2;
    C_AdobeRGB AdobeRGB_1, AdobeRGB_2;
    C_CIEXYZ CIEXYZ_1, CIEXYZ_2;
    C_CIExyY CIExyY_1, CIExyY_2;
    C_CIELab CIELab_1, CIELab_2;
    C_CIELuv CIELuv_1, CIELuv_2;
    C_HSL HSL_1, HSL_2;
    C_HSV HSV_1, HSV_2;

    for (auto& sample: samples()) {

        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, sRGB_2));          TEST_VECTORS(sRGB_2, sample.sRGB, 1e-10);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, AdobeRGB_2));      TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIEXYZ_2));        TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIExyY_2));        TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIELab_2));        TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIELuv_2));        TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, HSL_2));           TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, HSV_2));           TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, sRGB_2));      TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, AdobeRGB_2));  TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 1e-10);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIEXYZ_2));    TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIExyY_2));    TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIELab_2));    TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIELuv_2));    TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, HSL_2));       TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, HSV_2));       TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 1e-10);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 1e-10);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 1e-10);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 1e-10);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, sRGB_2));           TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, AdobeRGB_2));       TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIEXYZ_2));         TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIExyY_2));         TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIELab_2));         TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIELuv_2));         TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, HSL_2));            TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 1e-10);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, HSV_2));            TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, sRGB_2));           TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, AdobeRGB_2));       TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIEXYZ_2));         TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIExyY_2));         TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIELab_2));         TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIELuv_2));         TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, HSL_2));            TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, HSV_2));            TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 1e-10);

    }

}

void test_crow_colour_conversion_between_representations() {

    Colour<double, LinearRGB, ColourLayout::forward>          cdf;
    Colour<double, LinearRGB, ColourLayout::forward_alpha>    cdfa;
    Colour<double, LinearRGB, ColourLayout::alpha_forward>    cdaf;
    Colour<double, LinearRGB, ColourLayout::reverse>          cdr;
    Colour<double, LinearRGB, ColourLayout::reverse_alpha>    cdra;
    Colour<double, LinearRGB, ColourLayout::alpha_reverse>    cdar;
    Colour<uint16_t, LinearRGB, ColourLayout::forward>        cuf;
    Colour<uint16_t, LinearRGB, ColourLayout::forward_alpha>  cufa;
    Colour<uint16_t, LinearRGB, ColourLayout::alpha_forward>  cuaf;
    Colour<uint16_t, LinearRGB, ColourLayout::reverse>        cur;
    Colour<uint16_t, LinearRGB, ColourLayout::reverse_alpha>  cura;
    Colour<uint16_t, LinearRGB, ColourLayout::alpha_reverse>  cuar;

    TRY((cdfa = {0.2,0.4,0.6,0.8}));

    TRY(convert_colour(cdfa, cuf));
    TRY(convert_colour(cdfa, cufa));
    TRY(convert_colour(cdfa, cuaf));
    TRY(convert_colour(cdfa, cur));
    TRY(convert_colour(cdfa, cura));
    TRY(convert_colour(cdfa, cuar));

    TEST_VECTORS(cuf,   Int3(0x3333,0x6666,0x9999),         0);
    TEST_VECTORS(cufa,  Int4(0x3333,0x6666,0x9999,0xcccc),  0);
    TEST_VECTORS(cuaf,  Int4(0xcccc,0x3333,0x6666,0x9999),  0);
    TEST_VECTORS(cur,   Int3(0x9999,0x6666,0x3333),         0);
    TEST_VECTORS(cura,  Int4(0x9999,0x6666,0x3333,0xcccc),  0);
    TEST_VECTORS(cuar,  Int4(0xcccc,0x9999,0x6666,0x3333),  0);

    TRY(convert_colour(cuf, cdar));   TEST_VECTORS(cdar, Double4(1.0,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cufa, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cuaf, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cur, cdar));   TEST_VECTORS(cdar, Double4(1.0,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cura, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cuar, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);

    TRY((cufa = {0x3333,0x6666,0x9999,0xcccc}));

    TRY(convert_colour(cufa, cdf));
    TRY(convert_colour(cufa, cdfa));
    TRY(convert_colour(cufa, cdaf));
    TRY(convert_colour(cufa, cdr));
    TRY(convert_colour(cufa, cdra));
    TRY(convert_colour(cufa, cdar));

    TEST_VECTORS(cdf,   Double3(0.2,0.4,0.6),      1e-10);
    TEST_VECTORS(cdfa,  Double4(0.2,0.4,0.6,0.8),  1e-10);
    TEST_VECTORS(cdaf,  Double4(0.8,0.2,0.4,0.6),  1e-10);
    TEST_VECTORS(cdr,   Double3(0.6,0.4,0.2),      1e-10);
    TEST_VECTORS(cdra,  Double4(0.6,0.4,0.2,0.8),  1e-10);
    TEST_VECTORS(cdar,  Double4(0.8,0.6,0.4,0.2),  1e-10);

    TRY(convert_colour(cdf, cuar));   TEST_VECTORS(cuar, Int4(0xffff,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdfa, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdaf, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdr, cuar));   TEST_VECTORS(cuar, Int4(0xffff,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdra, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdar, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);

}
