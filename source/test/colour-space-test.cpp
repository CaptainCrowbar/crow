#include "crow/colour-space.hpp"
#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"
#include <cmath>
#include <map>
#include <numbers>
#include <vector>

using namespace Crow;
using namespace Crow::Test;

using std::numbers::pi;

using Grey = Vector<double, 1>;

void test_crow_colour_space_ciexyy() {

    Double3 xyy, xyz;

    for (auto& sample: samples()) {
        TRY(xyy = CIExyY::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIExyY>(xyy));
        TEST_VECTORS(xyy, sample.CIExyY, 1e-4);
        TRY(xyz = CIExyY::to_base(sample.CIExyY));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-4);
    }

}

void test_crow_colour_space_cielab() {

    Double3 colour, xyz;

    for (auto& sample: samples()) {
        TRY(colour = CIELab::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELab>(colour));
        TEST_VECTORS(colour, sample.CIELab, 0.01);
        TRY(xyz = CIELab::to_base(sample.CIELab));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_crow_colour_space_cieluv() {

    Double3 colour, xyz;

    for (auto& sample: samples()) {
        TRY(colour = CIELuv::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELuv>(colour));
        TEST_VECTORS(colour, sample.CIELuv, 0.01);
        TRY(xyz = CIELuv::to_base(sample.CIELuv));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_crow_colour_space_hclab() {

    Double3 cie1, cie2, colour;

    for (auto& sample: samples()) {
        cie1 = sample.CIELab;
        TRY(colour = HCLab::from_base(cie1));
        TEST(is_colour_in_gamut<HCLab>(colour));
        TEST_NEAR(colour[0], fraction(std::atan2(cie1[2], cie1[1]) / (2 * pi)), 1e-6);
        TEST_NEAR(colour[1], std::hypot(cie1[1], cie1[2]), 1e-6);
        TEST_NEAR(colour[2], cie1[0], 1e-6);
        TRY(cie2 = HCLab::to_base(colour));
        TEST_VECTORS(cie2, cie1, 1e-6);
    }

}

void test_crow_colour_space_hcluv() {

    Double3 cie1, cie2, colour;

    for (auto& sample: samples()) {
        cie1 = sample.CIELuv;
        TRY(colour = HCLuv::from_base(cie1));
        TEST(is_colour_in_gamut<HCLuv>(colour));
        TEST_NEAR(colour[0], fraction(std::atan2(cie1[2], cie1[1]) / (2 * pi)), 1e-6);
        TEST_NEAR(colour[1], std::hypot(cie1[1], cie1[2]), 1e-6);
        TEST_NEAR(colour[2], cie1[0], 1e-6);
        TRY(cie2 = HCLuv::to_base(colour));
        TEST_VECTORS(cie2, cie1, 1e-6);
    }

}

void test_crow_colour_space_srgb() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
        TRY(linear = LinearRGB::from_base(sample.CIEXYZ));
        TRY(nonlinear = sRGB::from_base(linear));
        TEST(is_colour_in_gamut<sRGB>(nonlinear));
        TEST_VECTORS(nonlinear, sample.sRGB, 1e-4);
        TRY(linear = sRGB::to_base(sample.sRGB));
        TRY(xyz = LinearRGB::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-4);
    }

}

void test_crow_colour_space_adobe_rgb() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
        TRY(linear = LinearAdobeRGB::from_base(sample.CIEXYZ));
        TRY(nonlinear = AdobeRGB::from_base(linear));
        TEST(is_colour_in_gamut<AdobeRGB>(nonlinear));
        TEST_VECTORS(nonlinear, sample.AdobeRGB, 1e-3);
        TRY(linear = AdobeRGB::to_base(sample.AdobeRGB));
        TRY(xyz = LinearAdobeRGB::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-3);
    }

}

void test_crow_colour_space_pro_photo() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
        TRY(linear = LinearProPhoto::from_base(sample.CIEXYZ));
        TRY(nonlinear = ProPhoto::from_base(linear));
        TEST(is_colour_in_gamut<ProPhoto>(nonlinear));
        TRY(linear = ProPhoto::to_base(nonlinear));
        TRY(xyz = LinearProPhoto::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-6);
    }

}

void test_crow_colour_space_wide_gamut() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
        TRY(linear = LinearWideGamut::from_base(sample.CIEXYZ));
        TRY(nonlinear = WideGamut::from_base(linear));
        TEST(is_colour_in_gamut<WideGamut>(nonlinear));
        TRY(linear = WideGamut::to_base(nonlinear));
        TRY(xyz = LinearWideGamut::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-6);
    }

}

void test_crow_colour_space_hsl() {

    Double3 rgb, polar, xyz;

    for (auto& sample: samples()) {
        TRY(rgb = LinearRGB::from_base(sample.CIEXYZ));
        TRY(polar = HSL::from_base(rgb));
        TEST(is_colour_in_gamut<HSL>(polar));
        TEST_VECTORS(polar, sample.HSL, 1e-5);
        TRY(rgb = HSL::to_base(sample.HSL));
        TRY(xyz = LinearRGB::to_base(rgb));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-5);
    }

}

void test_crow_colour_space_hsv() {

    Double3 rgb, polar, xyz;

    for (auto& sample: samples()) {
        TRY(rgb = LinearRGB::from_base(sample.CIEXYZ));
        TRY(polar = HSV::from_base(rgb));
        TEST(is_colour_in_gamut<HSV>(polar));
        TEST_VECTORS(polar, sample.HSV, 1e-5);
        TRY(rgb = HSV::to_base(sample.HSV));
        TRY(xyz = LinearRGB::to_base(rgb));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-5);
    }

}

void test_crow_colour_space_greyscale() {

    std::map<double, Double3> expect = {
        { 0.0,  { 0.000000, 0.0, 0.000000 } },
        { 0.1,  { 0.095049, 0.1, 0.108884 } },
        { 0.2,  { 0.190098, 0.2, 0.217768 } },
        { 0.3,  { 0.285147, 0.3, 0.326652 } },
        { 0.4,  { 0.380196, 0.4, 0.435536 } },
        { 0.5,  { 0.475245, 0.5, 0.544420 } },
        { 0.6,  { 0.570293, 0.6, 0.653304 } },
        { 0.7,  { 0.665342, 0.7, 0.762188 } },
        { 0.8,  { 0.760391, 0.8, 0.871072 } },
        { 0.9,  { 0.855440, 0.9, 0.979956 } },
        { 1.0,  { 0.950489, 1.0, 1.088840 } },
    };

    Grey g;
    Double3 xyz1, xyz2;

    for (auto [y,xyz]: expect) {
        xyz1 = {0,y,0};
        TRY(g = Greyscale::from_base(xyz1));
        TEST(is_colour_in_gamut<Greyscale>(g));
        TEST_NEAR(g[0], y, 1e-6);
        TRY(xyz2 = Greyscale::to_base(g));
        TEST_VECTORS(xyz2, xyz, 1e-6);
    }

}

void test_crow_colour_space_sgreyscale() {

    std::map<double, double> expect = {
        { 0.0, 0.000000 },
        { 0.1, 0.349190 },
        { 0.2, 0.484529 },
        { 0.3, 0.583831 },
        { 0.4, 0.665185 },
        { 0.5, 0.735357 },
        { 0.6, 0.797738 },
        { 0.7, 0.854306 },
        { 0.8, 0.906332 },
        { 0.9, 0.954687 },
        { 1.0, 1.000000 },
    };

    Grey g1, g2, sg;

    for (auto [y,sy]: expect) {
        g1 = Grey(y);
        TRY(sg = sGreyscale::from_base(g1));
        TEST(is_colour_in_gamut<sGreyscale>(sg));
        TEST_NEAR(sg[0], sy, 1e-6);
        TRY(g2 = sGreyscale::to_base(sg));
        TEST_NEAR(g2[0], y, 1e-6);
    }

}

void test_crow_colour_space_conversion() {

    Double3 c;

    for (auto& sample: samples()) {

        TRY((c = convert_colour_space<sRGB, sRGB>(sample.sRGB)));              TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<sRGB, AdobeRGB>(sample.sRGB)));          TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<sRGB, CIEXYZ>(sample.sRGB)));            TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<sRGB, CIExyY>(sample.sRGB)));            TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<sRGB, CIELab>(sample.sRGB)));            TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<sRGB, CIELuv>(sample.sRGB)));            TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<sRGB, HSL>(sample.sRGB)));               TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<sRGB, HSV>(sample.sRGB)));               TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, sRGB>(sample.AdobeRGB)));      TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, AdobeRGB>(sample.AdobeRGB)));  TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIEXYZ>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIExyY>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIELab>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<AdobeRGB, CIELuv>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<AdobeRGB, HSL>(sample.AdobeRGB)));       TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, HSV>(sample.AdobeRGB)));       TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, sRGB>(sample.CIEXYZ)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, AdobeRGB>(sample.CIEXYZ)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIEXYZ>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIExyY>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIELab>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIEXYZ, CIELuv>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIEXYZ, HSL>(sample.CIEXYZ)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, HSV>(sample.CIEXYZ)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIExyY, sRGB>(sample.CIExyY)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIExyY, AdobeRGB>(sample.CIExyY)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIEXYZ>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIExyY>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIELab>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIExyY, CIELuv>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIExyY, HSL>(sample.CIExyY)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIExyY, HSV>(sample.CIExyY)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIELab, sRGB>(sample.CIELab)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIELab, AdobeRGB>(sample.CIELab)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIELab, CIEXYZ>(sample.CIELab)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIELab, CIExyY>(sample.CIELab)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIELab, CIELab>(sample.CIELab)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIELab, CIELuv>(sample.CIELab)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIELab, HSL>(sample.CIELab)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIELab, HSV>(sample.CIELab)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIELuv, sRGB>(sample.CIELuv)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIELuv, AdobeRGB>(sample.CIELuv)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIEXYZ>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIExyY>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIELab>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIELuv, CIELuv>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIELuv, HSL>(sample.CIELuv)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIELuv, HSV>(sample.CIELuv)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<HSL, sRGB>(sample.HSL)));                TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<HSL, AdobeRGB>(sample.HSL)));            TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<HSL, CIEXYZ>(sample.HSL)));              TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<HSL, CIExyY>(sample.HSL)));              TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<HSL, CIELab>(sample.HSL)));              TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<HSL, CIELuv>(sample.HSL)));              TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<HSL, HSL>(sample.HSL)));                 TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<HSL, HSV>(sample.HSL)));                 TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<HSV, sRGB>(sample.HSV)));                TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<HSV, AdobeRGB>(sample.HSV)));            TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<HSV, CIEXYZ>(sample.HSV)));              TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<HSV, CIExyY>(sample.HSV)));              TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<HSV, CIELab>(sample.HSV)));              TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<HSV, CIELuv>(sample.HSV)));              TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<HSV, HSL>(sample.HSV)));                 TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<HSV, HSV>(sample.HSV)));                 TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);

    }

}
