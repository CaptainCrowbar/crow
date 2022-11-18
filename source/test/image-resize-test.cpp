#include "crow/image.hpp"
#include "crow/colour.hpp"
#include "crow/geometry.hpp"
#include "crow/unit-test.hpp"
#include "test/vector-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_image_resize_dimensions() {

    Image<Rgbaf> in, out;

    TRY(in.reset({200, 100}, Rgbaf::blue()));

    TRY(out = in.resized({100, 50}));                       TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({100, 0}));                        TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({100, 60}));                       TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({100, 40}));                       TEST_EQUAL(out.width(), 80);   TEST_EQUAL(out.height(), 40);
    TRY(out = in.resized({0, 50}));                         TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({120, 50}));                       TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({80, 50}));                        TEST_EQUAL(out.width(), 80);   TEST_EQUAL(out.height(), 40);
    TRY(out = in.resized({100, 50}, ImageResize::unlock));  TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({100, 60}, ImageResize::unlock));  TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 60);
    TRY(out = in.resized({100, 40}, ImageResize::unlock));  TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 40);
    TRY(out = in.resized({120, 50}, ImageResize::unlock));  TEST_EQUAL(out.width(), 120);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized({80, 50}, ImageResize::unlock));   TEST_EQUAL(out.width(), 80);   TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized(0.5));                             TEST_EQUAL(out.width(), 100);  TEST_EQUAL(out.height(), 50);
    TRY(out = in.resized(2));                               TEST_EQUAL(out.width(), 400);  TEST_EQUAL(out.height(), 200);

    TEST_THROW(out = in.resized({-1, 50}),                       std::invalid_argument);
    TEST_THROW(out = in.resized({100, -1}),                      std::invalid_argument);
    TEST_THROW(out = in.resized({0, 0}),                         std::invalid_argument);
    TEST_THROW(out = in.resized({0, 50}, ImageResize::unlock),   std::invalid_argument);
    TEST_THROW(out = in.resized({100, 0}, ImageResize::unlock),  std::invalid_argument);
    TEST_THROW(out = in.resized(0),                              std::invalid_argument);
    TEST_THROW(out = in.resized(-0.5),                           std::invalid_argument);

}

void test_crow_image_resize_content() {

    Image<Rgba8> lin8a, lin8b;
    Image<Rgba16> lin16a, lin16b;
    Image<Rgbaf> lin32a, lin32b;
    Image<sRgba8> srgb8a, srgb8b;
    Image<sRgba16> srgb16a, srgb16b;
    Image<sRgbaf> srgb32a, srgb32b;
    Image<Rgba8, ImageFlags::pma> pma8a, pma8b;
    Image<Rgba16, ImageFlags::pma> pma16a, pma16b;
    Image<Rgbaf, ImageFlags::pma> pma32a, pma32b;

    TRY(lin8a.reset({200, 100}, Rgba8::red()));
    TRY(lin16a.reset({200, 100}, Rgba16::green()));
    TRY(lin32a.reset({200, 100}, Rgbaf::blue()));
    TRY(srgb8a.reset({200, 100}, sRgba8::red()));
    TRY(srgb16a.reset({200, 100}, sRgba16::green()));
    TRY(srgb32a.reset({200, 100}, sRgbaf::blue()));
    TRY(pma8a.reset({200, 100}, Rgba8::red()));
    TRY(pma16a.reset({200, 100}, Rgba16::green()));
    TRY(pma32a.reset({200, 100}, Rgbaf::blue()));

    TRY(lin8b = lin8a.resized({100, 50}));
    TRY(lin16b = lin16a.resized({100, 50}));
    TRY(lin32b = lin32a.resized({100, 50}));
    TRY(srgb8b = srgb8a.resized({100, 50}));
    TRY(srgb16b = srgb16a.resized({100, 50}));
    TRY(srgb32b = srgb32a.resized({100, 50}));
    TRY(pma8b = pma8a.resized({100, 50}));
    TRY(pma16b = pma16a.resized({100, 50}));
    TRY(pma32b = pma32a.resized({100, 50}));

    TEST_EQUAL(lin8b.width(), 100);    TEST_EQUAL(lin8b.height(), 50);
    TEST_EQUAL(lin16b.width(), 100);   TEST_EQUAL(lin16b.height(), 50);
    TEST_EQUAL(lin32b.width(), 100);   TEST_EQUAL(lin32b.height(), 50);
    TEST_EQUAL(srgb8b.width(), 100);   TEST_EQUAL(srgb8b.height(), 50);
    TEST_EQUAL(srgb16b.width(), 100);  TEST_EQUAL(srgb16b.height(), 50);
    TEST_EQUAL(srgb32b.width(), 100);  TEST_EQUAL(srgb32b.height(), 50);
    TEST_EQUAL(pma8b.width(), 100);    TEST_EQUAL(pma8b.height(), 50);
    TEST_EQUAL(pma16b.width(), 100);   TEST_EQUAL(pma16b.height(), 50);
    TEST_EQUAL(pma32b.width(), 100);   TEST_EQUAL(pma32b.height(), 50);

}

void test_crow_image_segment() {

    Image<Rgb8> img1(100, 100);
    Image<Rgb8> img2;

    for (int x = 0; x < 100; ++x)
        for (int y = 0; y < 100; ++y)
            img1(x, y) = Rgb8(x, y, 0);

    TRY(img2 = img1.segment({{20, 50}, {60, 40}}));
    TEST_EQUAL(img2.width(), 60);
    TEST_EQUAL(img2.height(), 40);
    REQUIRE(img2.width() == 60 && img2.height() == 40);

    for (int x = 0; x < 60; ++x)
        for (int y = 0; y < 40; ++y)
            TEST_EQUAL(img2(x, y), Rgb8(x + 20, y + 50, 0));

    TEST_THROW(img1.segment({{20, 150}, {60, 40}}), std::invalid_argument);

}
