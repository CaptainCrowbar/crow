#include "crow/projection.hpp"
#include "crow/colour.hpp"
#include "crow/image.hpp"
#include "crow/string.hpp"
#include "crow/thread.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Literals;

namespace {

    constexpr double epsilon = 1e-10;
    constexpr Double2 zero = {0, 0};
    constexpr Double2 equator = {0, pi_d / 2};
    constexpr Double2 pcentre = {pi_d / 2, pi_d / 2};
    constexpr Double2 pt_north = {pi_d / 8, pi_d / 4};
    constexpr Double2 pt_south = {pi_d / 8, 3 * pi_d / 4};
    constexpr Double2 xy_north = {0.1, 0.1};
    constexpr Double2 xy_south = {0.1, -0.1};

    const std::string docs_image_dir = "../docs/images/";
    const std::string test_image_dir = "../source/test/images/";
    const std::string earth_map = test_image_dir + "nasa-earth.png";

}

void test_crow_projection_polar_coordinate_normalisation() {

    using namespace Crow::Detail;

    struct test_item {
        Double2 polar;
        Double2 ref;
        Double2 expect;
    };

    static const std::vector<test_item> test_vector = {

        { {60_degd,75_degd},  {0,90_degd},        {60_degd,75_degd}  },
        { {0,30_degd},        {0,60_degd},        {0,60_degd}        },
        { {0,90_degd},        {0,60_degd},        {0,120_degd}       },
        { {75_degd,30_degd},  {75_degd,60_degd},  {0,60_degd}        },
        { {75_degd,90_degd},  {75_degd,60_degd},  {0,120_degd}       },

    };

    Double2 polar;
    PolarReduce<double> pr;

    for (auto& item: test_vector) {
        TRY(pr = PolarReduce(item.ref));
        TRY(polar = pr.reduce_to_polar(item.polar));
        TEST_NEAR(polar[0], item.expect[0], epsilon);
        TEST_NEAR(polar[1], item.expect[1], epsilon);
        TRY(polar = pr.inverse_from_polar(item.expect));
        TEST_NEAR(polar[0], item.polar[0], epsilon);
        TEST_NEAR(polar[1], item.polar[1], epsilon);
    }

}

void test_crow_projection_azimuthal_equidistant() {

    AzimuthalEquidistantProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "azimuthal equidistant projection");
    TEST_EQUAL(proj.properties(), Maps::azimuthal | Maps::sphere | Maps::circle | Maps::hemisphere_circle);
    TEST_EQUAL(to_string(proj.properties()), "azimuthal,sphere,circle,hemisphere-circle");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d, epsilon);
    TEST_NEAR(proj.max_y(), pi_d, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_gnomonic() {

    GnomonicProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "gnomonic projection");
    TEST_EQUAL(proj.properties(), Maps::azimuthal | Maps::sub_hemisphere| Maps::plane );
    TEST_EQUAL(to_string(proj.properties()), "azimuthal,sub-hemisphere,plane");
    TEST(! proj.has_min_x());
    TEST(! proj.has_max_x());
    TEST(! proj.has_min_y());
    TEST(! proj.has_max_y());
    TEST_EQUAL(proj.min_x(), 0);
    TEST_EQUAL(proj.max_x(), 0);
    TEST_EQUAL(proj.min_y(), 0);
    TEST_EQUAL(proj.max_y(), 0);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_lambert_azimuthal() {

    LambertAzimuthalProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Lambert azimuthal projection");
    TEST_EQUAL(proj.properties(), Maps::azimuthal | Maps::sphere | Maps::circle | Maps::equal_area | Maps::hemisphere_circle);
    TEST_EQUAL(to_string(proj.properties()), "azimuthal,sphere,circle,equal-area,hemisphere-circle");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), -2, epsilon);
    TEST_NEAR(proj.max_x(), 2, epsilon);
    TEST_NEAR(proj.min_y(), -2, epsilon);
    TEST_NEAR(proj.max_y(), 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_orthographic() {

    OrthographicProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "orthographic projection");
    TEST_EQUAL(proj.properties(), Maps::azimuthal | Maps::hemisphere | Maps::circle | Maps::hemisphere_circle);
    TEST_EQUAL(to_string(proj.properties()), "azimuthal,hemisphere,circle,hemisphere-circle");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), -1, epsilon);
    TEST_NEAR(proj.max_x(), 1, epsilon);
    TEST_NEAR(proj.min_y(), -1, epsilon);
    TEST_NEAR(proj.max_y(), 1, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_stereographic() {

    StereographicProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "stereographic projection");
    TEST_EQUAL(proj.properties(), Maps::azimuthal | Maps::sub_sphere | Maps::plane | Maps::conformal | Maps::hemisphere_circle);
    TEST_EQUAL(to_string(proj.properties()), "azimuthal,sub-sphere,plane,conformal,hemisphere-circle");
    TEST(! proj.has_min_x());
    TEST(! proj.has_max_x());
    TEST(! proj.has_min_y());
    TEST(! proj.has_max_y());
    TEST_EQUAL(proj.min_x(), 0);
    TEST_EQUAL(proj.max_x(), 0);
    TEST_EQUAL(proj.min_y(), 0);
    TEST_EQUAL(proj.max_y(), 0);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_cylindrical_equidistant() {

    CylindricalEquidistantProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "cylindrical equidistant projection");
    TEST_EQUAL(proj.properties(), Maps::cylindrical | Maps::sphere | Maps::rectangle);
    TEST_EQUAL(to_string(proj.properties()), "cylindrical,sphere,rectangle");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_gall_peters() {

    GallPetersProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Gall-Peters projection");
    TEST_EQUAL(proj.properties(), Maps::cylindrical | Maps::sphere | Maps::rectangle | Maps::equal_area);
    TEST_EQUAL(to_string(proj.properties()), "cylindrical,sphere,rectangle,equal-area");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), -2, epsilon);
    TEST_NEAR(proj.max_y(), 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_lambert_cylindrical() {

    LambertCylindricalProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Lambert cylindrical projection");
    TEST_EQUAL(proj.properties(), Maps::cylindrical | Maps::sphere | Maps::rectangle | Maps::equal_area);
    TEST_EQUAL(to_string(proj.properties()), "cylindrical,sphere,rectangle,equal-area");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), -1, epsilon);
    TEST_NEAR(proj.max_y(), 1, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_mercator() {

    MercatorProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Mercator projection");
    TEST_EQUAL(proj.properties(), Maps::cylindrical | Maps::sub_sphere | Maps::other_shape | Maps::conformal);
    TEST_EQUAL(to_string(proj.properties()), "cylindrical,sub-sphere,other-shape,conformal");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(! proj.has_min_y());
    TEST(! proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_EQUAL(proj.min_y(), 0);
    TEST_EQUAL(proj.max_y(), 0);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_eckert_iv() {

    Eckert4Projection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Eckert IV projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area | Maps::numerical);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,other-shape,equal-area,numerical");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_mollweide() {

    MollweideProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "Mollweide projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::ellipse | Maps::equal_area | Maps::hemisphere_circle | Maps::numerical);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,ellipse,equal-area,hemisphere-circle,numerical");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_sinusoidal() {

    SinusoidalProjection<double> proj(pcentre);

    TEST_EQUAL(proj.name(), "sinusoidal projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,other-shape,equal-area");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(pcentre));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.origin().y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), pcentre.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), pcentre.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(pcentre).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_interrupted_eckert_iv() {

    InterruptedProjection<Eckert4Projection<double>> proj;

    TEST_EQUAL(proj.name(), "interrupted Eckert IV projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area | Maps::interrupted | Maps::numerical);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,other-shape,equal-area,interrupted,numerical");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(equator));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), equator.x(), epsilon);
    TEST_NEAR(proj.origin().y(), equator.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), equator.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), equator.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(equator).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(equator).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_interrupted_mollweide() {

    InterruptedProjection<MollweideProjection<double>> proj;

    TEST_EQUAL(proj.name(), "interrupted Mollweide projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area | Maps::interrupted | Maps::numerical);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,other-shape,equal-area,interrupted,numerical");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(equator));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), equator.x(), epsilon);
    TEST_NEAR(proj.origin().y(), equator.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), equator.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), equator.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(equator).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(equator).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

void test_crow_projection_interrupted_sinusoidal() {

    InterruptedProjection<SinusoidalProjection<double>> proj;

    TEST_EQUAL(proj.name(), "interrupted sinusoidal projection");
    TEST_EQUAL(proj.properties(), Maps::pseudocylindrical | Maps::sphere | Maps::other_shape | Maps::equal_area | Maps::interrupted);
    TEST_EQUAL(to_string(proj.properties()), "pseudocylindrical,sphere,other-shape,equal-area,interrupted");
    TEST(proj.has_min_x());
    TEST(proj.has_max_x());
    TEST(proj.has_min_y());
    TEST(proj.has_max_y());
    TEST_NEAR(proj.min_x(), - pi_d, epsilon);
    TEST_NEAR(proj.max_x(), pi_d, epsilon);
    TEST_NEAR(proj.min_y(), - pi_d / 2, epsilon);
    TEST_NEAR(proj.max_y(), pi_d / 2, epsilon);
    TEST(proj.is_on_globe(equator));
    TEST(proj.is_on_map(zero));
    TEST_NEAR(proj.origin().x(), equator.x(), epsilon);
    TEST_NEAR(proj.origin().y(), equator.y(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).x(), equator.x(), epsilon);
    TEST_NEAR(proj.map_to_globe(zero).y(), equator.y(), epsilon);
    TEST_NEAR(proj.globe_to_map(equator).x(), 0, epsilon);
    TEST_NEAR(proj.globe_to_map(equator).y(), 0, epsilon);
    TEST(proj.map_to_globe(xy_north)[1] < 90_degd);
    TEST(proj.map_to_globe(xy_south)[1] > 90_degd);
    TEST(proj.globe_to_map(pt_north)[1] > 0);
    TEST(proj.globe_to_map(pt_south)[1] < 0);

}

namespace {

    constexpr int max_size = 500;

    class Globe {
    public:
        Globe(): image_(), x_scale_(), y_scale_() {
            image_.load(earth_map);
            x_scale_ = image_.width() / (2 * pi_d);
            y_scale_ = image_.height() / pi_d;
        }
        Rgba8 operator()(const Double2& polar) const {
            double x = euclidean_remainder(polar[0] + pi_d, 2 * pi_d);
            double y = std::clamp(pi_d - polar[1], 0.0, pi_d);
            auto i = int(std::floor(x * x_scale_));
            auto j = image_.height() - 1 - int(std::floor(y * y_scale_));
            return image_[{i, j}];
        }
    private:
        Image8 image_;
        double x_scale_;
        double y_scale_;
    };

    void make_map(std::shared_ptr<BasicMapProjection<double>> proj) {

        static const std::vector<double> cut_north = {-30_degd};
        static const std::vector<double> cut_south = {-15_degd, 75_degd};

        auto iproj = std::dynamic_pointer_cast<InterruptedProjectionBase<double>>(proj);
        if (iproj)
            iproj->interrupt(cut_north, cut_south);
        std::string name = ascii_lowercase(proj->name());
        std::replace(name.begin(), name.end(), ' ', '-');

        double max_x = proj->has_max_x() ? proj->max_x() : 0;
        double max_y = proj->has_max_y() ? proj->max_y() : 0;
        if (max_x == 0 && max_y == 0)
            max_x = max_y = pi_d;
        else if (max_x == 0)
            max_x = max_y;
        else if (max_y == 0)
            max_y = max_x;
        double x_scale_ = 0.5 * max_size / max_x;
        double y_scale_ = 0.5 * max_size / max_y;
        double scale = std::min(x_scale_, y_scale_);
        int width = 2 * int(std::round(scale * max_x));
        int height = 2 * int(std::round(scale * max_y));
        double width_offset = 0.5 * width - 0.5;
        double height_offset = 0.5 * height - 0.5;

        Globe globe;
        Image8 image({width, height}, Rgba8::clear());
        auto it = image.begin();

        for (int iy = 0; iy < height; ++iy) {
            double fy = (height_offset - iy) / scale;
            for (int ix = 0; ix < width; ++ix, ++it) {
                double fx = (ix - width_offset) / scale;
                if (proj->is_on_map({fx, fy})) {
                    auto polar = proj->map_to_globe({fx, fy});
                    *it = globe(polar);
                }
            }
        }

        std::string file = docs_image_dir + "map-" + name + ".png";
        image.save(file);
        std::string message = "... " + proj->name() + "\n";
        std::cout << message;

    }

}

void test_crow_projection_sample_maps() {

    std::vector<Thread> threads;

    threads.emplace_back([] { make_map(std::make_shared<AzimuthalEquidistantProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<GnomonicProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<LambertAzimuthalProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<OrthographicProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<StereographicProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<CylindricalEquidistantProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<GallPetersProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<LambertCylindricalProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<MercatorProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<Eckert4Projection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<MollweideProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<SinusoidalProjection<double>>()); });
    threads.emplace_back([] { make_map(std::make_shared<InterruptedProjection<Eckert4Projection<double>>>()); });
    threads.emplace_back([] { make_map(std::make_shared<InterruptedProjection<MollweideProjection<double>>>()); });
    threads.emplace_back([] { make_map(std::make_shared<InterruptedProjection<SinusoidalProjection<double>>>()); });

}
