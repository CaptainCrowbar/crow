#include "crow/image.hpp"
#include "crow/colour.hpp"
#include "crow/unit-test.hpp"
#include "test/vector-test.hpp"
#include <algorithm>
#include <string>

using namespace Crow;

namespace {

    const std::string image_dir = "../source/test/images/";
    const std::string png_file = image_dir + "test-image.png";
    const std::string jpg_file = image_dir + "test-image.jpg";
    const std::string no_such_file = image_dir + "no-such-file.png";
    const std::string temp_file = "test-image-copy.png";

}

void test_crow_image_io_file_info() {

    ImageInfo info;
    std::string s;

    TRY(info = query_image(png_file));
    TEST(info);
    TEST_EQUAL(info.shape.x(), 20);
    TEST_EQUAL(info.shape.y(), 20);
    TEST_EQUAL(info.channels, 4);
    TEST_EQUAL(info.bits_per_channel, 8);
    TEST(info.has_alpha);
    TEST(! info.is_hdr);
    TRY(s = info.str());
    TEST_EQUAL(s, "Image:20x20,4ch@8bit,alpha");

    TRY(info = query_image(jpg_file));
    TEST(info);
    TEST_EQUAL(info.shape.x(), 20);
    TEST_EQUAL(info.shape.y(), 20);
    TEST_EQUAL(info.channels, 3);
    TEST_EQUAL(info.bits_per_channel, 8);
    TEST(! info.has_alpha);
    TEST(! info.is_hdr);
    TRY(s = info.str());
    TEST_EQUAL(s, "Image:20x20,3ch@8bit");

    TRY(info = query_image(no_such_file));
    TEST(! info);
    TRY(s = info.str());
    TEST_EQUAL(s, "<null>");

}

void test_crow_image_io_load() {

    Image<Rgbaf> hdr;
    Image<Rgba8> rgb;

    TEST_THROW_MATCH(hdr.load(no_such_file), ImageIoError, "no-such-file.png");

    TRY(hdr.load(png_file));
    TEST_EQUAL(hdr.width(), 20);
    TEST_EQUAL(hdr.height(), 20);
    REQUIRE(hdr.width() == 20 && hdr.height() == 20);

    TEST_EQUAL(hdr(0,0),    Rgbaf::clear());
    TEST_EQUAL(hdr(1,1),    Rgbaf::black());
    TEST_EQUAL(hdr(2,2),    Rgbaf::red());
    TEST_EQUAL(hdr(3,3),    Rgbaf::red());
    TEST_EQUAL(hdr(4,4),    Rgbaf::red());
    TEST_EQUAL(hdr(5,5),    Rgbaf::red());
    TEST_EQUAL(hdr(6,6),    Rgbaf::red());
    TEST_EQUAL(hdr(7,7),    Rgbaf::red());
    TEST_EQUAL(hdr(8,8),    Rgbaf::black());
    TEST_EQUAL(hdr(9,9),    Rgbaf::clear());
    TEST_EQUAL(hdr(10,10),  Rgbaf::clear());
    TEST_EQUAL(hdr(11,11),  Rgbaf::black());
    TEST_EQUAL(hdr(12,12),  Rgbaf::blue());
    TEST_EQUAL(hdr(13,13),  Rgbaf::blue());
    TEST_EQUAL(hdr(14,14),  Rgbaf::blue());
    TEST_EQUAL(hdr(15,15),  Rgbaf::blue());
    TEST_EQUAL(hdr(16,16),  Rgbaf::blue());
    TEST_EQUAL(hdr(17,17),  Rgbaf::blue());
    TEST_EQUAL(hdr(18,18),  Rgbaf::black());
    TEST_EQUAL(hdr(19,19),  Rgbaf::clear());
    TEST_EQUAL(hdr(19,0),   Rgbaf::clear());
    TEST_EQUAL(hdr(18,1),   Rgbaf::black());
    TEST_EQUAL(hdr(17,2),   Rgbaf::yellow());
    TEST_EQUAL(hdr(16,3),   Rgbaf::yellow());
    TEST_EQUAL(hdr(15,4),   Rgbaf::yellow());
    TEST_EQUAL(hdr(14,5),   Rgbaf::yellow());
    TEST_EQUAL(hdr(13,6),   Rgbaf::yellow());
    TEST_EQUAL(hdr(12,7),   Rgbaf::yellow());
    TEST_EQUAL(hdr(11,8),   Rgbaf::black());
    TEST_EQUAL(hdr(10,9),   Rgbaf::clear());
    TEST_EQUAL(hdr(9,10),   Rgbaf::clear());
    TEST_EQUAL(hdr(8,11),   Rgbaf::black());
    TEST_EQUAL(hdr(7,12),   Rgbaf::green());
    TEST_EQUAL(hdr(6,13),   Rgbaf::green());
    TEST_EQUAL(hdr(5,14),   Rgbaf::green());
    TEST_EQUAL(hdr(4,15),   Rgbaf::green());
    TEST_EQUAL(hdr(3,16),   Rgbaf::green());
    TEST_EQUAL(hdr(2,17),   Rgbaf::green());
    TEST_EQUAL(hdr(1,18),   Rgbaf::black());
    TEST_EQUAL(hdr(0,19),   Rgbaf::clear());

    TRY(rgb.load(png_file));
    TEST_EQUAL(rgb.width(), 20);
    TEST_EQUAL(rgb.height(), 20);
    REQUIRE(rgb.width() == 20 && rgb.height() == 20);

    TEST_EQUAL(rgb(0,0),    Rgba8::clear());
    TEST_EQUAL(rgb(1,1),    Rgba8::black());
    TEST_EQUAL(rgb(2,2),    Rgba8::red());
    TEST_EQUAL(rgb(3,3),    Rgba8::red());
    TEST_EQUAL(rgb(4,4),    Rgba8::red());
    TEST_EQUAL(rgb(5,5),    Rgba8::red());
    TEST_EQUAL(rgb(6,6),    Rgba8::red());
    TEST_EQUAL(rgb(7,7),    Rgba8::red());
    TEST_EQUAL(rgb(8,8),    Rgba8::black());
    TEST_EQUAL(rgb(9,9),    Rgba8::clear());
    TEST_EQUAL(rgb(10,10),  Rgba8::clear());
    TEST_EQUAL(rgb(11,11),  Rgba8::black());
    TEST_EQUAL(rgb(12,12),  Rgba8::blue());
    TEST_EQUAL(rgb(13,13),  Rgba8::blue());
    TEST_EQUAL(rgb(14,14),  Rgba8::blue());
    TEST_EQUAL(rgb(15,15),  Rgba8::blue());
    TEST_EQUAL(rgb(16,16),  Rgba8::blue());
    TEST_EQUAL(rgb(17,17),  Rgba8::blue());
    TEST_EQUAL(rgb(18,18),  Rgba8::black());
    TEST_EQUAL(rgb(19,19),  Rgba8::clear());
    TEST_EQUAL(rgb(19,0),   Rgba8::clear());
    TEST_EQUAL(rgb(18,1),   Rgba8::black());
    TEST_EQUAL(rgb(17,2),   Rgba8::yellow());
    TEST_EQUAL(rgb(16,3),   Rgba8::yellow());
    TEST_EQUAL(rgb(15,4),   Rgba8::yellow());
    TEST_EQUAL(rgb(14,5),   Rgba8::yellow());
    TEST_EQUAL(rgb(13,6),   Rgba8::yellow());
    TEST_EQUAL(rgb(12,7),   Rgba8::yellow());
    TEST_EQUAL(rgb(11,8),   Rgba8::black());
    TEST_EQUAL(rgb(10,9),   Rgba8::clear());
    TEST_EQUAL(rgb(9,10),   Rgba8::clear());
    TEST_EQUAL(rgb(8,11),   Rgba8::black());
    TEST_EQUAL(rgb(7,12),   Rgba8::green());
    TEST_EQUAL(rgb(6,13),   Rgba8::green());
    TEST_EQUAL(rgb(5,14),   Rgba8::green());
    TEST_EQUAL(rgb(4,15),   Rgba8::green());
    TEST_EQUAL(rgb(3,16),   Rgba8::green());
    TEST_EQUAL(rgb(2,17),   Rgba8::green());
    TEST_EQUAL(rgb(1,18),   Rgba8::black());
    TEST_EQUAL(rgb(0,19),   Rgba8::clear());

    // Don't test points close to borders in JPEG images

    TRY(hdr.load(jpg_file));
    TEST_EQUAL(hdr.width(), 20);
    TEST_EQUAL(hdr.height(), 20);
    REQUIRE(hdr.width() == 20 && hdr.height() == 20);

    TEST_VECTORS(hdr(0,0),    Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(1,1),    Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(3,3),    Rgbaf::red(),     0.1);
    TEST_VECTORS(hdr(4,4),    Rgbaf::red(),     0.1);
    TEST_VECTORS(hdr(5,5),    Rgbaf::red(),     0.1);
    TEST_VECTORS(hdr(6,6),    Rgbaf::red(),     0.1);
    TEST_VECTORS(hdr(8,8),    Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(9,9),    Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(10,10),  Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(11,11),  Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(13,13),  Rgbaf::blue(),    0.1);
    TEST_VECTORS(hdr(14,14),  Rgbaf::blue(),    0.1);
    TEST_VECTORS(hdr(15,15),  Rgbaf::blue(),    0.1);
    TEST_VECTORS(hdr(16,16),  Rgbaf::blue(),    0.1);
    TEST_VECTORS(hdr(18,18),  Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(19,19),  Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(19,0),   Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(18,1),   Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(16,3),   Rgbaf::yellow(),  0.1);
    TEST_VECTORS(hdr(15,4),   Rgbaf::yellow(),  0.1);
    TEST_VECTORS(hdr(14,5),   Rgbaf::yellow(),  0.1);
    TEST_VECTORS(hdr(13,6),   Rgbaf::yellow(),  0.1);
    TEST_VECTORS(hdr(11,8),   Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(10,9),   Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(9,10),   Rgbaf::white(),   0.1);
    TEST_VECTORS(hdr(8,11),   Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(6,13),   Rgbaf::green(),   0.1);
    TEST_VECTORS(hdr(5,14),   Rgbaf::green(),   0.1);
    TEST_VECTORS(hdr(4,15),   Rgbaf::green(),   0.1);
    TEST_VECTORS(hdr(3,16),   Rgbaf::green(),   0.1);
    TEST_VECTORS(hdr(1,18),   Rgbaf::black(),   0.1);
    TEST_VECTORS(hdr(0,19),   Rgbaf::white(),   0.1);

    TRY(rgb.load(jpg_file));
    TEST_EQUAL(rgb.width(), 20);
    TEST_EQUAL(rgb.height(), 20);
    REQUIRE(rgb.width() == 20 && rgb.height() == 20);

    TEST_VECTORS(rgb(0,0),    Rgba8::white(),   20);
    TEST_VECTORS(rgb(1,1),    Rgba8::black(),   20);
    TEST_VECTORS(rgb(3,3),    Rgba8::red(),     20);
    TEST_VECTORS(rgb(4,4),    Rgba8::red(),     20);
    TEST_VECTORS(rgb(5,5),    Rgba8::red(),     20);
    TEST_VECTORS(rgb(6,6),    Rgba8::red(),     20);
    TEST_VECTORS(rgb(8,8),    Rgba8::black(),   20);
    TEST_VECTORS(rgb(9,9),    Rgba8::white(),   20);
    TEST_VECTORS(rgb(10,10),  Rgba8::white(),   20);
    TEST_VECTORS(rgb(11,11),  Rgba8::black(),   20);
    TEST_VECTORS(rgb(13,13),  Rgba8::blue(),    20);
    TEST_VECTORS(rgb(14,14),  Rgba8::blue(),    20);
    TEST_VECTORS(rgb(15,15),  Rgba8::blue(),    20);
    TEST_VECTORS(rgb(16,16),  Rgba8::blue(),    20);
    TEST_VECTORS(rgb(18,18),  Rgba8::black(),   20);
    TEST_VECTORS(rgb(19,19),  Rgba8::white(),   20);
    TEST_VECTORS(rgb(19,0),   Rgba8::white(),   20);
    TEST_VECTORS(rgb(18,1),   Rgba8::black(),   20);
    TEST_VECTORS(rgb(16,3),   Rgba8::yellow(),  20);
    TEST_VECTORS(rgb(15,4),   Rgba8::yellow(),  20);
    TEST_VECTORS(rgb(14,5),   Rgba8::yellow(),  20);
    TEST_VECTORS(rgb(13,6),   Rgba8::yellow(),  20);
    TEST_VECTORS(rgb(11,8),   Rgba8::black(),   20);
    TEST_VECTORS(rgb(10,9),   Rgba8::white(),   20);
    TEST_VECTORS(rgb(9,10),   Rgba8::white(),   20);
    TEST_VECTORS(rgb(8,11),   Rgba8::black(),   20);
    TEST_VECTORS(rgb(6,13),   Rgba8::green(),   20);
    TEST_VECTORS(rgb(5,14),   Rgba8::green(),   20);
    TEST_VECTORS(rgb(4,15),   Rgba8::green(),   20);
    TEST_VECTORS(rgb(3,16),   Rgba8::green(),   20);
    TEST_VECTORS(rgb(1,18),   Rgba8::black(),   20);
    TEST_VECTORS(rgb(0,19),   Rgba8::white(),   20);

}

void test_crow_image_io_save() {

    Image<Rgbaf> hdr1, hdr2;
    Image<Rgba8> rgb1, rgb2;

    TRY(hdr1.load(png_file));
    TEST_EQUAL(hdr1.width(), 20);
    TEST_EQUAL(hdr1.height(), 20);
    REQUIRE(hdr1.width() == 20 && hdr1.height() == 20);
    TRY(hdr1.save(temp_file));
    TRY(hdr2.load(temp_file));
    TEST_EQUAL(hdr2.width(), 20);
    TEST_EQUAL(hdr2.height(), 20);
    REQUIRE(hdr2.width() == 20 && hdr2.height() == 20);

    TEST_EQUAL(hdr2(0,0),    Rgbaf::clear());
    TEST_EQUAL(hdr2(1,1),    Rgbaf::black());
    TEST_EQUAL(hdr2(2,2),    Rgbaf::red());
    TEST_EQUAL(hdr2(3,3),    Rgbaf::red());
    TEST_EQUAL(hdr2(4,4),    Rgbaf::red());
    TEST_EQUAL(hdr2(5,5),    Rgbaf::red());
    TEST_EQUAL(hdr2(6,6),    Rgbaf::red());
    TEST_EQUAL(hdr2(7,7),    Rgbaf::red());
    TEST_EQUAL(hdr2(8,8),    Rgbaf::black());
    TEST_EQUAL(hdr2(9,9),    Rgbaf::clear());
    TEST_EQUAL(hdr2(10,10),  Rgbaf::clear());
    TEST_EQUAL(hdr2(11,11),  Rgbaf::black());
    TEST_EQUAL(hdr2(12,12),  Rgbaf::blue());
    TEST_EQUAL(hdr2(13,13),  Rgbaf::blue());
    TEST_EQUAL(hdr2(14,14),  Rgbaf::blue());
    TEST_EQUAL(hdr2(15,15),  Rgbaf::blue());
    TEST_EQUAL(hdr2(16,16),  Rgbaf::blue());
    TEST_EQUAL(hdr2(17,17),  Rgbaf::blue());
    TEST_EQUAL(hdr2(18,18),  Rgbaf::black());
    TEST_EQUAL(hdr2(19,19),  Rgbaf::clear());
    TEST_EQUAL(hdr2(19,0),   Rgbaf::clear());
    TEST_EQUAL(hdr2(18,1),   Rgbaf::black());
    TEST_EQUAL(hdr2(17,2),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(16,3),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(15,4),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(14,5),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(13,6),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(12,7),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(11,8),   Rgbaf::black());
    TEST_EQUAL(hdr2(10,9),   Rgbaf::clear());
    TEST_EQUAL(hdr2(9,10),   Rgbaf::clear());
    TEST_EQUAL(hdr2(8,11),   Rgbaf::black());
    TEST_EQUAL(hdr2(7,12),   Rgbaf::green());
    TEST_EQUAL(hdr2(6,13),   Rgbaf::green());
    TEST_EQUAL(hdr2(5,14),   Rgbaf::green());
    TEST_EQUAL(hdr2(4,15),   Rgbaf::green());
    TEST_EQUAL(hdr2(3,16),   Rgbaf::green());
    TEST_EQUAL(hdr2(2,17),   Rgbaf::green());
    TEST_EQUAL(hdr2(1,18),   Rgbaf::black());
    TEST_EQUAL(hdr2(0,19),   Rgbaf::clear());

    TRY(rgb2.load(temp_file));
    TEST_EQUAL(rgb2.width(), 20);
    TEST_EQUAL(rgb2.height(), 20);
    REQUIRE(rgb2.width() == 20 && rgb2.height() == 20);

    TEST_EQUAL(rgb2(0,0),    Rgba8::clear());
    TEST_EQUAL(rgb2(1,1),    Rgba8::black());
    TEST_EQUAL(rgb2(2,2),    Rgba8::red());
    TEST_EQUAL(rgb2(3,3),    Rgba8::red());
    TEST_EQUAL(rgb2(4,4),    Rgba8::red());
    TEST_EQUAL(rgb2(5,5),    Rgba8::red());
    TEST_EQUAL(rgb2(6,6),    Rgba8::red());
    TEST_EQUAL(rgb2(7,7),    Rgba8::red());
    TEST_EQUAL(rgb2(8,8),    Rgba8::black());
    TEST_EQUAL(rgb2(9,9),    Rgba8::clear());
    TEST_EQUAL(rgb2(10,10),  Rgba8::clear());
    TEST_EQUAL(rgb2(11,11),  Rgba8::black());
    TEST_EQUAL(rgb2(12,12),  Rgba8::blue());
    TEST_EQUAL(rgb2(13,13),  Rgba8::blue());
    TEST_EQUAL(rgb2(14,14),  Rgba8::blue());
    TEST_EQUAL(rgb2(15,15),  Rgba8::blue());
    TEST_EQUAL(rgb2(16,16),  Rgba8::blue());
    TEST_EQUAL(rgb2(17,17),  Rgba8::blue());
    TEST_EQUAL(rgb2(18,18),  Rgba8::black());
    TEST_EQUAL(rgb2(19,19),  Rgba8::clear());
    TEST_EQUAL(rgb2(19,0),   Rgba8::clear());
    TEST_EQUAL(rgb2(18,1),   Rgba8::black());
    TEST_EQUAL(rgb2(17,2),   Rgba8::yellow());
    TEST_EQUAL(rgb2(16,3),   Rgba8::yellow());
    TEST_EQUAL(rgb2(15,4),   Rgba8::yellow());
    TEST_EQUAL(rgb2(14,5),   Rgba8::yellow());
    TEST_EQUAL(rgb2(13,6),   Rgba8::yellow());
    TEST_EQUAL(rgb2(12,7),   Rgba8::yellow());
    TEST_EQUAL(rgb2(11,8),   Rgba8::black());
    TEST_EQUAL(rgb2(10,9),   Rgba8::clear());
    TEST_EQUAL(rgb2(9,10),   Rgba8::clear());
    TEST_EQUAL(rgb2(8,11),   Rgba8::black());
    TEST_EQUAL(rgb2(7,12),   Rgba8::green());
    TEST_EQUAL(rgb2(6,13),   Rgba8::green());
    TEST_EQUAL(rgb2(5,14),   Rgba8::green());
    TEST_EQUAL(rgb2(4,15),   Rgba8::green());
    TEST_EQUAL(rgb2(3,16),   Rgba8::green());
    TEST_EQUAL(rgb2(2,17),   Rgba8::green());
    TEST_EQUAL(rgb2(1,18),   Rgba8::black());
    TEST_EQUAL(rgb2(0,19),   Rgba8::clear());

    TRY(rgb1.load(png_file));
    TEST_EQUAL(rgb1.width(), 20);
    TEST_EQUAL(rgb1.height(), 20);
    REQUIRE(rgb1.width() == 20 && rgb1.height() == 20);
    TRY(rgb1.save(temp_file));
    TRY(hdr2.load(temp_file));
    TEST_EQUAL(hdr2.width(), 20);
    TEST_EQUAL(hdr2.height(), 20);
    REQUIRE(hdr2.width() == 20 && hdr2.height() == 20);

    TEST_EQUAL(hdr2(0,0),    Rgbaf::clear());
    TEST_EQUAL(hdr2(1,1),    Rgbaf::black());
    TEST_EQUAL(hdr2(2,2),    Rgbaf::red());
    TEST_EQUAL(hdr2(3,3),    Rgbaf::red());
    TEST_EQUAL(hdr2(4,4),    Rgbaf::red());
    TEST_EQUAL(hdr2(5,5),    Rgbaf::red());
    TEST_EQUAL(hdr2(6,6),    Rgbaf::red());
    TEST_EQUAL(hdr2(7,7),    Rgbaf::red());
    TEST_EQUAL(hdr2(8,8),    Rgbaf::black());
    TEST_EQUAL(hdr2(9,9),    Rgbaf::clear());
    TEST_EQUAL(hdr2(10,10),  Rgbaf::clear());
    TEST_EQUAL(hdr2(11,11),  Rgbaf::black());
    TEST_EQUAL(hdr2(12,12),  Rgbaf::blue());
    TEST_EQUAL(hdr2(13,13),  Rgbaf::blue());
    TEST_EQUAL(hdr2(14,14),  Rgbaf::blue());
    TEST_EQUAL(hdr2(15,15),  Rgbaf::blue());
    TEST_EQUAL(hdr2(16,16),  Rgbaf::blue());
    TEST_EQUAL(hdr2(17,17),  Rgbaf::blue());
    TEST_EQUAL(hdr2(18,18),  Rgbaf::black());
    TEST_EQUAL(hdr2(19,19),  Rgbaf::clear());
    TEST_EQUAL(hdr2(19,0),   Rgbaf::clear());
    TEST_EQUAL(hdr2(18,1),   Rgbaf::black());
    TEST_EQUAL(hdr2(17,2),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(16,3),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(15,4),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(14,5),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(13,6),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(12,7),   Rgbaf::yellow());
    TEST_EQUAL(hdr2(11,8),   Rgbaf::black());
    TEST_EQUAL(hdr2(10,9),   Rgbaf::clear());
    TEST_EQUAL(hdr2(9,10),   Rgbaf::clear());
    TEST_EQUAL(hdr2(8,11),   Rgbaf::black());
    TEST_EQUAL(hdr2(7,12),   Rgbaf::green());
    TEST_EQUAL(hdr2(6,13),   Rgbaf::green());
    TEST_EQUAL(hdr2(5,14),   Rgbaf::green());
    TEST_EQUAL(hdr2(4,15),   Rgbaf::green());
    TEST_EQUAL(hdr2(3,16),   Rgbaf::green());
    TEST_EQUAL(hdr2(2,17),   Rgbaf::green());
    TEST_EQUAL(hdr2(1,18),   Rgbaf::black());
    TEST_EQUAL(hdr2(0,19),   Rgbaf::clear());

    TRY(rgb2.load(temp_file));
    TEST_EQUAL(rgb2.width(), 20);
    TEST_EQUAL(rgb2.height(), 20);
    REQUIRE(rgb2.width() == 20 && rgb2.height() == 20);

    TEST_EQUAL(rgb2(0,0),    Rgba8::clear());
    TEST_EQUAL(rgb2(1,1),    Rgba8::black());
    TEST_EQUAL(rgb2(2,2),    Rgba8::red());
    TEST_EQUAL(rgb2(3,3),    Rgba8::red());
    TEST_EQUAL(rgb2(4,4),    Rgba8::red());
    TEST_EQUAL(rgb2(5,5),    Rgba8::red());
    TEST_EQUAL(rgb2(6,6),    Rgba8::red());
    TEST_EQUAL(rgb2(7,7),    Rgba8::red());
    TEST_EQUAL(rgb2(8,8),    Rgba8::black());
    TEST_EQUAL(rgb2(9,9),    Rgba8::clear());
    TEST_EQUAL(rgb2(10,10),  Rgba8::clear());
    TEST_EQUAL(rgb2(11,11),  Rgba8::black());
    TEST_EQUAL(rgb2(12,12),  Rgba8::blue());
    TEST_EQUAL(rgb2(13,13),  Rgba8::blue());
    TEST_EQUAL(rgb2(14,14),  Rgba8::blue());
    TEST_EQUAL(rgb2(15,15),  Rgba8::blue());
    TEST_EQUAL(rgb2(16,16),  Rgba8::blue());
    TEST_EQUAL(rgb2(17,17),  Rgba8::blue());
    TEST_EQUAL(rgb2(18,18),  Rgba8::black());
    TEST_EQUAL(rgb2(19,19),  Rgba8::clear());
    TEST_EQUAL(rgb2(19,0),   Rgba8::clear());
    TEST_EQUAL(rgb2(18,1),   Rgba8::black());
    TEST_EQUAL(rgb2(17,2),   Rgba8::yellow());
    TEST_EQUAL(rgb2(16,3),   Rgba8::yellow());
    TEST_EQUAL(rgb2(15,4),   Rgba8::yellow());
    TEST_EQUAL(rgb2(14,5),   Rgba8::yellow());
    TEST_EQUAL(rgb2(13,6),   Rgba8::yellow());
    TEST_EQUAL(rgb2(12,7),   Rgba8::yellow());
    TEST_EQUAL(rgb2(11,8),   Rgba8::black());
    TEST_EQUAL(rgb2(10,9),   Rgba8::clear());
    TEST_EQUAL(rgb2(9,10),   Rgba8::clear());
    TEST_EQUAL(rgb2(8,11),   Rgba8::black());
    TEST_EQUAL(rgb2(7,12),   Rgba8::green());
    TEST_EQUAL(rgb2(6,13),   Rgba8::green());
    TEST_EQUAL(rgb2(5,14),   Rgba8::green());
    TEST_EQUAL(rgb2(4,15),   Rgba8::green());
    TEST_EQUAL(rgb2(3,16),   Rgba8::green());
    TEST_EQUAL(rgb2(2,17),   Rgba8::green());
    TEST_EQUAL(rgb2(1,18),   Rgba8::black());
    TEST_EQUAL(rgb2(0,19),   Rgba8::clear());

    std::remove(temp_file.data());

}
