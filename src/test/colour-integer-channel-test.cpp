#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"

using namespace Crow;
using namespace Crow::Test;

void test_crow_colour_integral_elements() {

    Rgba8 c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0x40);      TEST_EQUAL(c.R(), 0x40);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0x80);      TEST_EQUAL(c.G(), 0x80);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0xc0);      TEST_EQUAL(c.B(), 0xc0);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 0xff);  TEST_EQUAL(c.alpha(), 0xff);

    TEST_VECTORS(c.as_vector(), Double4(0x40,0x80,0xc0,0xff), 0);
    TEST_VECTORS(c, Double4(0x40,0x80,0xc0,0xff), 0);
    TEST_EQUAL(c.str(), "[64,128,192,255]");

    TRY((c = {0x20,0x40,0x60}));       TEST_EQUAL(c.str(), "[32,64,96,255]");
    TRY((c = {0x20,0x40,0x60,0x80}));  TEST_EQUAL(c.str(), "[32,64,96,128]");

}

void test_crow_colour_integral_standard_colours() {

    TEST_EQUAL(Rgba8::black().str(),    "[0,0,0,255]");
    TEST_EQUAL(Rgba8::white().str(),    "[255,255,255,255]");
    TEST_EQUAL(Rgba8::red().str(),      "[255,0,0,255]");
    TEST_EQUAL(Rgba8::yellow().str(),   "[255,255,0,255]");
    TEST_EQUAL(Rgba8::green().str(),    "[0,255,0,255]");
    TEST_EQUAL(Rgba8::cyan().str(),     "[0,255,255,255]");
    TEST_EQUAL(Rgba8::blue().str(),     "[0,0,255,255]");
    TEST_EQUAL(Rgba8::magenta().str(),  "[255,0,255,255]");

}

void test_crow_colour_integral_arithmetic() {

    Rgba8 a = {10,20,30,40};
    Rgba8 b = {80,60,40,20};
    Rgba8 c;
    Vector<uint8_t, 4> d = {2,3,4,5};

    TRY(c = + a);    TEST_VECTORS(c, Int4(10,20,30,40), 0);
    TRY(c = - a);    TEST_VECTORS(c, Int4(246,236,226,216), 0);
    TRY(c = a + b);  TEST_VECTORS(c, Int4(90,80,70,60), 0);
    TRY(c = a - b);  TEST_VECTORS(c, Int4(186,216,246,20), 0);
    TRY(c = a * 2);  TEST_VECTORS(c, Int4(20,40,60,80), 0);
    TRY(c = 2 * a);  TEST_VECTORS(c, Int4(20,40,60,80), 0);
    TRY(c = a / 2);  TEST_VECTORS(c, Int4(5,10,15,20), 0);
    TRY(c = a * d);  TEST_VECTORS(c, Int4(20,60,120,200), 0);
    TRY(c = d * a);  TEST_VECTORS(c, Int4(20,60,120,200), 0);
    TRY(c = b / d);  TEST_VECTORS(c, Int4(40,20,10,4), 0);

}
