#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"

using namespace Crow;
using namespace Crow::Test;

void test_crow_colour_floating_point_elements() {

    Rgbad c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0.25);   TEST_EQUAL(c.R(), 0.25);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0.5);    TEST_EQUAL(c.G(), 0.5);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0.75);   TEST_EQUAL(c.B(), 0.75);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 1);  TEST_EQUAL(c.alpha(), 1);

    TEST_VECTORS(c.as_vector(), Double4(0.25,0.5,0.75,1), 1e-15);
    TEST_VECTORS(c, Double4(0.25,0.5,0.75,1), 1e-15);
    TEST_EQUAL(c.str(), "[0.25,0.5,0.75,1]");

    TRY((c = {0.2,0.4,0.6}));      TEST_EQUAL(c.str(), "[0.2,0.4,0.6,1]");
    TRY((c = {0.2,0.4,0.6,0.8}));  TEST_EQUAL(c.str(), "[0.2,0.4,0.6,0.8]");

}

void test_crow_colour_floating_point_arithmetic() {

    Rgbad a = {0.1,0.2,0.3,0.4};
    Rgbad b = {0.8,0.6,0.4,0.2};
    Rgbad c;
    Double4 d = {2,3,4,5};

    TRY(c = + a);    TEST_VECTORS(c, Double4(0.1,0.2,0.3,0.4), 1e-10);
    TRY(c = - a);    TEST_VECTORS(c, Double4(-0.1,-0.2,-0.3,-0.4), 1e-10);
    TRY(c = a + b);  TEST_VECTORS(c, Double4(0.9,0.8,0.7,0.6), 1e-10);
    TRY(c = a - b);  TEST_VECTORS(c, Double4(-0.7,-0.4,-0.1,0.2), 1e-10);
    TRY(c = a * 2);  TEST_VECTORS(c, Double4(0.2,0.4,0.6,0.8), 1e-10);
    TRY(c = 2 * a);  TEST_VECTORS(c, Double4(0.2,0.4,0.6,0.8), 1e-10);
    TRY(c = a / 2);  TEST_VECTORS(c, Double4(0.05,0.1,0.15,0.2), 1e-10);
    TRY(c = a * d);  TEST_VECTORS(c, Double4(0.2,0.6,1.2,2), 1e-10);
    TRY(c = d * a);  TEST_VECTORS(c, Double4(0.2,0.6,1.2,2), 1e-10);
    TRY(c = b / d);  TEST_VECTORS(c, Double4(0.4,0.2,0.1,0.04), 1e-10);

}

void test_crow_colour_floating_point_standard_colours() {

    TEST_EQUAL(Rgbad::clear().str(),    "[0,0,0,0]");
    TEST_EQUAL(Rgbad::black().str(),    "[0,0,0,1]");
    TEST_EQUAL(Rgbad::white().str(),    "[1,1,1,1]");
    TEST_EQUAL(Rgbad::red().str(),      "[1,0,0,1]");
    TEST_EQUAL(Rgbad::yellow().str(),   "[1,1,0,1]");
    TEST_EQUAL(Rgbad::green().str(),    "[0,1,0,1]");
    TEST_EQUAL(Rgbad::cyan().str(),     "[0,1,1,1]");
    TEST_EQUAL(Rgbad::blue().str(),     "[0,0,1,1]");
    TEST_EQUAL(Rgbad::magenta().str(),  "[1,0,1,1]");

}
