#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"
#include <stdexcept>

using namespace Crow;
using namespace Crow::Test;

void test_crow_colour_hex_representation() {

    Rgb8 a;
    Rgba8 b;
    Rgbd c;
    Rgbad d;

    sRgb8 e;
    sRgba8 f;
    sRgbd g;
    sRgbad h;

    TRY(a = Rgb8("123456"));        TEST_EQUAL(a.R(), 0x12);  TEST_EQUAL(a.G(), 0x34);  TEST_EQUAL(a.B(), 0x56);  TEST_EQUAL(a.alpha(), 0xff);
    TRY(b = Rgba8("123456"));       TEST_EQUAL(b.R(), 0x12);  TEST_EQUAL(b.G(), 0x34);  TEST_EQUAL(b.B(), 0x56);  TEST_EQUAL(b.alpha(), 0xff);
    TRY(a = Rgb8("(789abcde)"));    TEST_EQUAL(a.R(), 0x78);  TEST_EQUAL(a.G(), 0x9a);  TEST_EQUAL(a.B(), 0xbc);  TEST_EQUAL(a.alpha(), 0xff);
    TRY(b = Rgba8("(789abcde)"));   TEST_EQUAL(b.R(), 0x78);  TEST_EQUAL(b.G(), 0x9a);  TEST_EQUAL(b.B(), 0xbc);  TEST_EQUAL(b.alpha(), 0xde);
    TRY(e = sRgb8("123456"));       TEST_EQUAL(e.R(), 0x12);  TEST_EQUAL(e.G(), 0x34);  TEST_EQUAL(e.B(), 0x56);  TEST_EQUAL(e.alpha(), 0xff);
    TRY(f = sRgba8("123456"));      TEST_EQUAL(f.R(), 0x12);  TEST_EQUAL(f.G(), 0x34);  TEST_EQUAL(f.B(), 0x56);  TEST_EQUAL(f.alpha(), 0xff);
    TRY(e = sRgb8("(789abcde)"));   TEST_EQUAL(e.R(), 0x78);  TEST_EQUAL(e.G(), 0x9a);  TEST_EQUAL(e.B(), 0xbc);  TEST_EQUAL(e.alpha(), 0xff);
    TRY(f = sRgba8("(789abcde)"));  TEST_EQUAL(f.R(), 0x78);  TEST_EQUAL(f.G(), 0x9a);  TEST_EQUAL(f.B(), 0xbc);  TEST_EQUAL(f.alpha(), 0xde);

    TRY(c = Rgbd("123456"));
    TEST_NEAR(c.R(), 0.070588, 1e-6);
    TEST_NEAR(c.G(), 0.203922, 1e-6);
    TEST_NEAR(c.B(), 0.337255, 1e-6);
    TEST_EQUAL(c.alpha(), 1);
    TRY(d = Rgbad("123456"));
    TEST_NEAR(d.R(), 0.070588, 1e-6);
    TEST_NEAR(d.G(), 0.203922, 1e-6);
    TEST_NEAR(d.B(), 0.337255, 1e-6);
    TEST_EQUAL(d.alpha(), 1);
    TRY(c = Rgbd("(789abcde)"));
    TEST_NEAR(c.R(), 0.470588, 1e-6);
    TEST_NEAR(c.G(), 0.603922, 1e-6);
    TEST_NEAR(c.B(), 0.737255, 1e-6);
    TEST_EQUAL(c.alpha(), 1);
    TRY(d = Rgbad("(789abcde)"));
    TEST_NEAR(d.R(), 0.470588, 1e-6);
    TEST_NEAR(d.G(), 0.603922, 1e-6);
    TEST_NEAR(d.B(), 0.737255, 1e-6);
    TEST_NEAR(d.alpha(), 0.870588, 1e-6);
    TRY(g = sRgbd("123456"));
    TEST_NEAR(g.R(), 0.070588, 1e-6);
    TEST_NEAR(g.G(), 0.203922, 1e-6);
    TEST_NEAR(g.B(), 0.337255, 1e-6);
    TEST_EQUAL(g.alpha(), 1);
    TRY(h = sRgbad("123456"));
    TEST_NEAR(h.R(), 0.070588, 1e-6);
    TEST_NEAR(h.G(), 0.203922, 1e-6);
    TEST_NEAR(h.B(), 0.337255, 1e-6);
    TEST_EQUAL(h.alpha(), 1);
    TRY(g = sRgbd("(789abcde)"));
    TEST_NEAR(g.R(), 0.470588, 1e-6);
    TEST_NEAR(g.G(), 0.603922, 1e-6);
    TEST_NEAR(g.B(), 0.737255, 1e-6);
    TEST_EQUAL(g.alpha(), 1);
    TRY(h = sRgbad("(789abcde)"));
    TEST_NEAR(h.R(), 0.470588, 1e-6);
    TEST_NEAR(h.G(), 0.603922, 1e-6);
    TEST_NEAR(h.B(), 0.737255, 1e-6);
    TEST_NEAR(h.alpha(), 0.870588, 1e-6);

    TEST_THROW(Rgba8(""), std::invalid_argument);
    TEST_THROW(Rgba8("12345"), std::invalid_argument);
    TEST_THROW(Rgba8("1234567"), std::invalid_argument);
    TEST_THROW(Rgba8("123456789"), std::invalid_argument);
    TEST_THROW(Rgba8("abcdefgh"), std::invalid_argument);

    TRY((a = {0xfe,0xdc,0xba}));
    TRY((b = {0x98,0x76,0x54,0x32}));
    TRY((c = {0.2,0.4,0.6}));
    TRY((d = {0.8,0.6,0.4,0.2}));
    TRY((e = {0xfe,0xdc,0xba}));
    TRY((f = {0x98,0x76,0x54,0x32}));
    TRY((g = {0.2,0.4,0.6}));
    TRY((h = {0.8,0.6,0.4,0.2}));

    TEST_EQUAL(a.hex(), "fedcba");
    TEST_EQUAL(b.hex(), "98765432");
    TEST_EQUAL(c.hex(), "336699");
    TEST_EQUAL(d.hex(), "cc996633");
    TEST_EQUAL(e.hex(), "fedcba");
    TEST_EQUAL(f.hex(), "98765432");
    TEST_EQUAL(g.hex(), "336699");
    TEST_EQUAL(h.hex(), "cc996633");

}

void test_crow_colour_css_colours() {

    sRgba8 a;
    Rgba8 b;
    sRgbad c;
    Rgbad d;

    TRY(a = css_colour<sRgba8>("Crimson"));       TEST_EQUAL(a,    sRgba8(0xdc, 0x14, 0x3c, 0xff));
    TRY(b = css_colour<Rgba8>("Crimson"));        TEST_EQUAL(b,    Rgba8(0xb7, 0x02, 0x0c, 0xff));
    TRY(c = css_colour<sRgbad>("Crimson"));       TEST_VECTORS(c,  sRgbad(0.862745, 0.0784314, 0.235294, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Crimson"));        TEST_VECTORS(d,  Rgbad(0.715694, 0.00699541, 0.0451862, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Forest Green"));  TEST_EQUAL(a,    sRgba8(0x22, 0x8b, 0x22, 0xff));
    TRY(b = css_colour<Rgba8>("Forest Green"));   TEST_EQUAL(b,    Rgba8(0x04, 0x42, 0x04, 0xff));
    TRY(c = css_colour<sRgbad>("Forest Green"));  TEST_VECTORS(c,  sRgbad(0.133333, 0.545098, 0.133333, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Forest Green"));   TEST_VECTORS(d,  Rgbad(0.0159963, 0.258183, 0.0159963, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Royal Blue"));    TEST_EQUAL(a,    sRgba8(0x41, 0x69, 0xe1, 0xff));
    TRY(b = css_colour<Rgba8>("Royal Blue"));     TEST_EQUAL(b,    Rgba8(0x0d, 0x24, 0xc0, 0xff));
    TRY(c = css_colour<sRgbad>("Royal Blue"));    TEST_VECTORS(c,  sRgbad(0.254902, 0.411765, 0.882353, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Royal Blue"));     TEST_VECTORS(d,  Rgbad(0.0528606, 0.141263, 0.752942, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Saddle Brown"));  TEST_EQUAL(a,    sRgba8(0x8b, 0x45, 0x13, 0xff));
    TRY(b = css_colour<Rgba8>("Saddle Brown"));   TEST_EQUAL(b,    Rgba8(0x42, 0x0f, 0x02, 0xff));
    TRY(c = css_colour<sRgbad>("Saddle Brown"));  TEST_VECTORS(c,  sRgbad(0.545098, 0.270588, 0.0745098, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Saddle Brown"));   TEST_VECTORS(d,  Rgbad(0.258183, 0.0595112, 0.00651209, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Transparent"));   TEST_EQUAL(a,    sRgba8(0, 0, 0, 0));
    TRY(b = css_colour<Rgba8>("Transparent"));    TEST_EQUAL(b,    Rgba8(0, 0, 0, 0));
    TRY(c = css_colour<sRgbad>("Transparent"));   TEST_EQUAL(c,    sRgbad(0, 0, 0, 0));
    TRY(d = css_colour<Rgbad>("Transparent"));    TEST_EQUAL(d,    Rgbad(0, 0, 0, 0));
    TRY(a = css_colour<sRgba8>("123456"));        TEST_EQUAL(a,    sRgba8(0x12, 0x34, 0x56, 0xff));
    TRY(b = css_colour<Rgba8>("123456"));         TEST_EQUAL(b,    Rgba8(0x02, 0x09, 0x18, 0xff));
    TRY(c = css_colour<sRgbad>("123456"));        TEST_VECTORS(c,  sRgbad(0.070588, 0.203922, 0.337255, 1.000000), 1e-6);
    TRY(d = css_colour<Rgbad>("123456"));         TEST_VECTORS(d,  Rgbad(0.006049,0.034340,0.093059,1.000000), 1e-6);
    TRY(a = css_colour<sRgba8>("(789abcde)"));    TEST_EQUAL(a,    sRgba8(0x78, 0x9a, 0xbc, 0xde));
    TRY(b = css_colour<Rgba8>("(789abcde)"));     TEST_EQUAL(b,    Rgba8(0x30, 0x52, 0x80, 0xde));
    TRY(c = css_colour<sRgbad>("(789abcde)"));    TEST_VECTORS(c,  sRgbad(0.470588, 0.603922, 0.737255, 0.870588), 1e-6);
    TRY(d = css_colour<Rgbad>("(789abcde)"));     TEST_VECTORS(d,  Rgbad(0.187821,0.323144,0.502887,0.870588), 1e-6);

}
