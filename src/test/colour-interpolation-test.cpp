#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "test/vector-test.hpp"

using namespace Crow;

void test_crow_colour_interpolation() {

    Rgb8 a = {10,20,30};
    Rgb8 b = {12,34,56};
    Rgb8 c;

    TRY(c = lerp(a, b, -0.50));  TEST_VECTORS(c, Rgb8(9,13,17), 0);
    TRY(c = lerp(a, b, -0.25));  TEST_VECTORS(c, Rgb8(10,17,24), 0);
    TRY(c = lerp(a, b, 0.00));   TEST_VECTORS(c, Rgb8(10,20,30), 0);
    TRY(c = lerp(a, b, 0.25));   TEST_VECTORS(c, Rgb8(11,24,37), 0);
    TRY(c = lerp(a, b, 0.50));   TEST_VECTORS(c, Rgb8(11,27,43), 0);
    TRY(c = lerp(a, b, 0.75));   TEST_VECTORS(c, Rgb8(12,31,50), 0);
    TRY(c = lerp(a, b, 1.00));   TEST_VECTORS(c, Rgb8(12,34,56), 0);
    TRY(c = lerp(a, b, 1.25));   TEST_VECTORS(c, Rgb8(13,38,63), 0);
    TRY(c = lerp(a, b, 1.50));   TEST_VECTORS(c, Rgb8(13,41,69), 0);

    Rgbad d = {10,20,30,40};
    Rgbad e = {12,34,56,78};
    Rgbad f;

    TRY(f = lerp(d, e, -0.50));  TEST_VECTORS(f, Rgbad(9.0,13.0,17.0,21.0), 0);
    TRY(f = lerp(d, e, -0.25));  TEST_VECTORS(f, Rgbad(9.5,16.5,23.5,30.5), 0);
    TRY(f = lerp(d, e, 0.00));   TEST_VECTORS(f, Rgbad(10.0,20.0,30.0,40.0), 0);
    TRY(f = lerp(d, e, 0.25));   TEST_VECTORS(f, Rgbad(10.5,23.5,36.5,49.5), 0);
    TRY(f = lerp(d, e, 0.50));   TEST_VECTORS(f, Rgbad(11.0,27.0,43.0,59.0), 0);
    TRY(f = lerp(d, e, 0.75));   TEST_VECTORS(f, Rgbad(11.5,30.5,49.5,68.5), 0);
    TRY(f = lerp(d, e, 1.00));   TEST_VECTORS(f, Rgbad(12.0,34.0,56.0,78.0), 0);
    TRY(f = lerp(d, e, 1.25));   TEST_VECTORS(f, Rgbad(12.5,37.5,62.5,87.5), 0);
    TRY(f = lerp(d, e, 1.50));   TEST_VECTORS(f, Rgbad(13.0,41.0,69.0,97.0), 0);

}
