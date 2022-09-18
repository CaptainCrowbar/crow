#include "crow/image.hpp"
#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "test/vector-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_image_resize_dimensions() {

    HdrImage in, out;

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

    Image8 in8, out8;
    Image16 in16, out16;
    HdrImage in32, out32;
    sImage8 in8s, out8s;
    sImage16 in16s, out16s;
    sHdrImage in32s, out32s;
    PmaImage8 in8sp, out8sp;
    PmaImage16 in16sp, out16sp;
    PmaHdrImage in32sp, out32sp;

    TRY(in8.reset({200, 100}, Rgba8::red()));
    TRY(in16.reset({200, 100}, Rgba16::green()));
    TRY(in32.reset({200, 100}, Rgbaf::blue()));
    TRY(in8s.reset({200, 100}, sRgba8::red()));
    TRY(in16s.reset({200, 100}, sRgba16::green()));
    TRY(in32s.reset({200, 100}, sRgbaf::blue()));
    TRY(in8sp.reset({200, 100}, Rgba8::red()));
    TRY(in16sp.reset({200, 100}, Rgba16::green()));
    TRY(in32sp.reset({200, 100}, Rgbaf::blue()));

    TRY(out8 = in8.resized({100, 50}));
    TRY(out16 = in16.resized({100, 50}));
    TRY(out32 = in32.resized({100, 50}));
    TRY(out8s = in8s.resized({100, 50}));
    TRY(out16s = in16s.resized({100, 50}));
    TRY(out32s = in32s.resized({100, 50}));
    TRY(out8sp = in8sp.resized({100, 50}));
    TRY(out16sp = in16sp.resized({100, 50}));
    TRY(out32sp = in32sp.resized({100, 50}));

    TEST_EQUAL(out8.width(), 100);     TEST_EQUAL(out8.height(), 50);
    TEST_EQUAL(out16.width(), 100);    TEST_EQUAL(out16.height(), 50);
    TEST_EQUAL(out32.width(), 100);    TEST_EQUAL(out32.height(), 50);
    TEST_EQUAL(out8s.width(), 100);    TEST_EQUAL(out8s.height(), 50);
    TEST_EQUAL(out16s.width(), 100);   TEST_EQUAL(out16s.height(), 50);
    TEST_EQUAL(out32s.width(), 100);   TEST_EQUAL(out32s.height(), 50);
    TEST_EQUAL(out8sp.width(), 100);   TEST_EQUAL(out8sp.height(), 50);
    TEST_EQUAL(out16sp.width(), 100);  TEST_EQUAL(out16sp.height(), 50);
    TEST_EQUAL(out32sp.width(), 100);  TEST_EQUAL(out32sp.height(), 50);

    // TODO

}
