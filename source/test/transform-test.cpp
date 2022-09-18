#include "crow/transform.hpp"
#include "crow/matrix.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <cstdlib>

using namespace Crow;
using namespace Crow::Literals;

namespace {

    template <typename R>
    void fuzz(R& r) {
        for (auto& x: r)
            if (std::abs(x) < 1e-6)
                x = 0;
    }

    void check_2d_helper(double x, double y, double r, double theta) {

        Double2 car1 = {x, y};
        Double2 pol1 = {r, theta};
        Double2 car2 = polar_to_cartesian(pol1);
        Double2 pol2 = cartesian_to_polar(car1);

        for (int i = 0; i < 2; ++i) {
            TEST_NEAR(car2[i], car1[i], 1e-6);
            TEST_NEAR(pol2[i], pol1[i], 1e-6);
        }

    }

    void check_3d_helper(double x, double y, double z, double r, double phi, double theta, double rho) {

        Double3 car1 = {x, y, z};
        Double3 cyl1 = {rho, phi, z};
        Double3 sph1 = {r, phi, theta};
        Double3 car2 = cylindrical_to_cartesian(cyl1);
        Double3 car3 = spherical_to_cartesian(sph1);
        Double3 cyl2 = cartesian_to_cylindrical(car1);
        Double3 cyl3 = spherical_to_cylindrical(sph1);
        Double3 sph2 = cartesian_to_spherical(car1);
        Double3 sph3 = cylindrical_to_spherical(cyl1);

        for (int i = 0; i < 2; ++i) {
            TEST_NEAR(car2[i], car1[i], 1e-6);
            TEST_NEAR(car3[i], car1[i], 1e-6);
            TEST_NEAR(cyl2[i], cyl1[i], 1e-6);
            TEST_NEAR(cyl3[i], cyl1[i], 1e-6);
            TEST_NEAR(sph2[i], sph1[i], 1e-6);
            TEST_NEAR(sph3[i], sph1[i], 1e-6);
        }

    }

}

void test_crow_2d_transforms() {

    check_2d_helper(0,          0,          0,  0);
    check_2d_helper(1,          0,          1,  0);
    check_2d_helper(0.866025,   0.5,        1,  30_degd);
    check_2d_helper(0.5,        0.866025,   1,  60_degd);
    check_2d_helper(0,          1,          1,  90_degd);
    check_2d_helper(-0.5,       0.866025,   1,  120_degd);
    check_2d_helper(-0.866025,  0.5,        1,  150_degd);
    check_2d_helper(-1,         0,          1,  180_degd);
    check_2d_helper(-0.866025,  -0.5,       1,  -150_degd);
    check_2d_helper(-0.5,       -0.866025,  1,  -120_degd);
    check_2d_helper(0,          -1,         1,  -90_degd);
    check_2d_helper(0.5,        -0.866025,  1,  -60_degd);
    check_2d_helper(0.866025,   -0.5,       1,  -30_degd);

}

void test_crow_3d_transforms() {

    check_3d_helper(0,          0,          0,          0,  0,          0,         0);
    check_3d_helper(1,          0,          0,          1,  0,          90_degd,   1);
    check_3d_helper(0.866025,   0.5,        0,          1,  30_degd,    90_degd,   1);
    check_3d_helper(0.5,        0.866025,   0,          1,  60_degd,    90_degd,   1);
    check_3d_helper(0,          1,          0,          1,  90_degd,    90_degd,   1);
    check_3d_helper(-0.5,       0.866025,   0,          1,  120_degd,   90_degd,   1);
    check_3d_helper(-0.866025,  0.5,        0,          1,  150_degd,   90_degd,   1);
    check_3d_helper(-1,         0,          0,          1,  180_degd,   90_degd,   1);
    check_3d_helper(-0.866025,  -0.5,       0,          1,  -150_degd,  90_degd,   1);
    check_3d_helper(-0.5,       -0.866025,  0,          1,  -120_degd,  90_degd,   1);
    check_3d_helper(0,          -1,         0,          1,  -90_degd,   90_degd,   1);
    check_3d_helper(0.5,        -0.866025,  0,          1,  -60_degd,   90_degd,   1);
    check_3d_helper(0.866025,   -0.5,       0,          1,  -30_degd,   90_degd,   1);
    check_3d_helper(0.707107,   0,          0.707107,   1,  0,          45_degd,   0.707107);
    check_3d_helper(0.612372,   0.353553,   0.707107,   1,  30_degd,    45_degd,   0.707107);
    check_3d_helper(0.353553,   0.612372,   0.707107,   1,  60_degd,    45_degd,   0.707107);
    check_3d_helper(0,          0.707107,   0.707107,   1,  90_degd,    45_degd,   0.707107);
    check_3d_helper(-0.353553,  0.612372,   0.707107,   1,  120_degd,   45_degd,   0.707107);
    check_3d_helper(-0.612372,  0.353553,   0.707107,   1,  150_degd,   45_degd,   0.707107);
    check_3d_helper(-0.707107,  0,          0.707107,   1,  180_degd,   45_degd,   0.707107);
    check_3d_helper(-0.612372,  -0.353553,  0.707107,   1,  -150_degd,  45_degd,   0.707107);
    check_3d_helper(-0.353553,  -0.612372,  0.707107,   1,  -120_degd,  45_degd,   0.707107);
    check_3d_helper(0,          -0.707107,  0.707107,   1,  -90_degd,   45_degd,   0.707107);
    check_3d_helper(0.353553,   -0.612372,  0.707107,   1,  -60_degd,   45_degd,   0.707107);
    check_3d_helper(0.612372,   -0.353553,  0.707107,   1,  -30_degd,   45_degd,   0.707107);
    check_3d_helper(0.707107,   0,          -0.707107,  1,  0,          135_degd,  0.707107);
    check_3d_helper(0.612372,   0.353553,   -0.707107,  1,  30_degd,    135_degd,  0.707107);
    check_3d_helper(0.353553,   0.612372,   -0.707107,  1,  60_degd,    135_degd,  0.707107);
    check_3d_helper(0,          0.707107,   -0.707107,  1,  90_degd,    135_degd,  0.707107);
    check_3d_helper(-0.353553,  0.612372,   -0.707107,  1,  120_degd,   135_degd,  0.707107);
    check_3d_helper(-0.612372,  0.353553,   -0.707107,  1,  150_degd,   135_degd,  0.707107);
    check_3d_helper(-0.707107,  0,          -0.707107,  1,  180_degd,   135_degd,  0.707107);
    check_3d_helper(-0.612372,  -0.353553,  -0.707107,  1,  -150_degd,  135_degd,  0.707107);
    check_3d_helper(-0.353553,  -0.612372,  -0.707107,  1,  -120_degd,  135_degd,  0.707107);
    check_3d_helper(0,          -0.707107,  -0.707107,  1,  -90_degd,   135_degd,  0.707107);
    check_3d_helper(0.353553,   -0.612372,  -0.707107,  1,  -60_degd,   135_degd,  0.707107);
    check_3d_helper(0.612372,   -0.353553,  -0.707107,  1,  -30_degd,   135_degd,  0.707107);
    check_3d_helper(0,          0,          1,          1,  0,          0,         0);
    check_3d_helper(0,          0,          -1,         1,  0,          180_degd,  0);

}

void test_crow_projective_geometry() {

    Double3 v3;
    Double4 v4;

    TRY((v3 = {10.0,20.0,30.0}));       TRY(v4 = point4(v3));   TEST_EQUAL(v4.str(), "[10,20,30,1]");
    TRY((v3 = {10.0,20.0,30.0}));       TRY(v4 = normal4(v3));  TEST_EQUAL(v4.str(), "[10,20,30,0]");
    TRY((v4 = {10.0,20.0,30.0,40.0}));  TRY(v3 = point3(v4));   TEST_EQUAL(v3.str(), "[0.25,0.5,0.75]");
    TRY((v4 = {10.0,20.0,30.0,40.0}));  TRY(v3 = normal3(v4));  TEST_EQUAL(v3.str(), "[10,20,30]");

    Double3x3c cm3;
    Double4x4c cm4;
    Double3x3r rm3;
    Double4x4r rm4;

    TRY(cm3(0, 0) = rm3(0, 0) = 2);
    TRY(cm3(0, 1) = rm3(0, 1) = 3);
    TRY(cm3(0, 2) = rm3(0, 2) = 5);
    TRY(cm3(1, 0) = rm3(1, 0) = 7);
    TRY(cm3(1, 1) = rm3(1, 1) = 11);
    TRY(cm3(1, 2) = rm3(1, 2) = 13);
    TRY(cm3(2, 0) = rm3(2, 0) = 17);
    TRY(cm3(2, 1) = rm3(2, 1) = 19);
    TRY(cm3(2, 2) = rm3(2, 2) = 23);
    TRY((v3 = {100.0,200.0,300.0}));

    TRY(cm4 = make_transform(cm3, v3));
    TEST_EQUAL(cm4.str(),
        "[[2,3,5,100],"
        "[7,11,13,200],"
        "[17,19,23,300],"
        "[0,0,0,1]]"
    );
    TRY(cm4 = normal_transform(cm4));
    TRY(rm4 = make_transform(rm3, v3));
    TEST_EQUAL(rm4.str(),
        "[[2,3,5,100],"
        "[7,11,13,200],"
        "[17,19,23,300],"
        "[0,0,0,1]]"
    );
    TRY(rm4 = normal_transform(rm4));

}

void test_crow_primitive_transforms() {

    static const Double3 u = Double3::unit(0);
    static const Double3 v = Double3::unit(1);
    static const Double3 w = Double3::unit(2);
    static const Double3 p(2.0,3.0,5.0);

    Double3 r;
    Double3x3 m3;
    Double4x4 m4;

    TRY(m3 = rotate3(30_degd, 0));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[1,0,0],"
        "[0,0.866025,-0.5],"
        "[0,0.5,0.866025]]"
    );

    TRY(r = m3 * u);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[1,0,0]");
    TRY(r = m3 * v);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0.866025,0.5]");
    TRY(r = m3 * w);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,-0.5,0.866025]");

    TRY(m3 = rotate3(30_degd, 1));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,0,0.5],"
        "[0,1,0],"
        "[-0.5,0,0.866025]]"
    );

    TRY(r = m3 * u);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0,-0.5]");
    TRY(r = m3 * v);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,1,0]");
    TRY(r = m3 * w);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.5,0,0.866025]");

    TRY(m3 = rotate3(30_degd, 2));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,-0.5,0],"
        "[0.5,0.866025,0],"
        "[0,0,1]]"
    );

    TRY(r = m3 * u);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0.5,0]");
    TRY(r = m3 * v);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[-0.5,0.866025,0]");
    TRY(r = m3 * w);  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0,1]");

    TRY(m4 = rotate4(30_degd, 0));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[1,0,0,0],"
        "[0,0.866025,-0.5,0],"
        "[0,0.5,0.866025,0],"
        "[0,0,0,1]]"
    );

    TRY(r = point3(m4 * point4(u)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[1,0,0]");
    TRY(r = point3(m4 * point4(v)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0.866025,0.5]");
    TRY(r = point3(m4 * point4(w)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,-0.5,0.866025]");

    TRY(m4 = rotate4(30_degd, 1));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,0,0.5,0],"
        "[0,1,0,0],"
        "[-0.5,0,0.866025,0],"
        "[0,0,0,1]]"
    );

    TRY(r = point3(m4 * point4(u)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0,-0.5]");
    TRY(r = point3(m4 * point4(v)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,1,0]");
    TRY(r = point3(m4 * point4(w)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.5,0,0.866025]");

    TRY(m4 = rotate4(30_degd, 2));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,-0.5,0,0],"
        "[0.5,0.866025,0,0],"
        "[0,0,1,0],"
        "[0,0,0,1]]"
    );

    TRY(r = point3(m4 * point4(u)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0.5,0]");
    TRY(r = point3(m4 * point4(v)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[-0.5,0.866025,0]");
    TRY(r = point3(m4 * point4(w)));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0,1]");

    TRY(m3 = rotate3(30_degd, u));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[1,0,0],"
        "[0,0.866025,-0.5],"
        "[0,0.5,0.866025]]"
    );
    TRY(m3 = rotate3(30_degd, v));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,0,0.5],"
        "[0,1,0],"
        "[-0.5,0,0.866025]]"
    );
    TRY(m3 = rotate3(30_degd, w));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,-0.5,0],"
        "[0.5,0.866025,0],"
        "[0,0,1]]"
    );

    TRY(m4 = rotate4(30_degd, u));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[1,0,0,0],"
        "[0,0.866025,-0.5,0],"
        "[0,0.5,0.866025,0],"
        "[0,0,0,1]]"
    );
    TRY(m4 = rotate4(30_degd, v));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,0,0.5,0],"
        "[0,1,0,0],"
        "[-0.5,0,0.866025,0],"
        "[0,0,0,1]]"
    );
    TRY(m4 = rotate4(30_degd, w));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,-0.5,0,0],"
        "[0.5,0.866025,0,0],"
        "[0,0,1,0],"
        "[0,0,0,1]]"
    );

    TRY(m3 = scale3(42.0));
    TEST_EQUAL(m3.str(),
        "[[42,0,0],"
        "[0,42,0],"
        "[0,0,42]]"
    );
    TRY(r = m3 * p);
    TEST_EQUAL(r.str(), "[84,126,210]");

    TRY(m3 = scale3(Double3{10.0,20.0,30.0}));
    TEST_EQUAL(m3.str(),
        "[[10,0,0],"
        "[0,20,0],"
        "[0,0,30]]"
    );
    TRY(r = m3 * p);
    TEST_EQUAL(r.str(), "[20,60,150]");

    TRY(m4 = scale4(42.0));
    TEST_EQUAL(m4.str(),
        "[[42,0,0,0],"
        "[0,42,0,0],"
        "[0,0,42,0],"
        "[0,0,0,1]]"
    );
    TRY(r = point3(m4 * point4(p)));
    TEST_EQUAL(r.str(), "[84,126,210]");

    TRY(m4 = scale4(Double3{10.0,20.0,30.0}));
    TEST_EQUAL(m4.str(),
        "[[10,0,0,0],"
        "[0,20,0,0],"
        "[0,0,30,0],"
        "[0,0,0,1]]"
    );
    TRY(r = point3(m4 * point4(p)));
    TEST_EQUAL(r.str(), "[20,60,150]");

    TRY(m4 = translate4(Double3{10.0,20.0,30.0}));
    TEST_EQUAL(m4.str(),
        "[[1,0,0,10],"
        "[0,1,0,20],"
        "[0,0,1,30],"
        "[0,0,0,1]]"
    );
    TRY(r = point3(m4 * point4(p)));
    TEST_EQUAL(r.str(), "[12,23,35]");

}

void test_crow_quaternion_transforms() {

    static const Double3 z = Double3::null();
    static const Double3 u = Double3::unit(0);
    static const Double3 v = Double3::unit(1);
    static const Double3 w = Double3::unit(2);
    static const Double3 p(1.0,1.0,1.0);

    Double3 r;
    Double3x3 m3;
    Double4x4 m4;
    Qdouble q;

    TRY(q = q_rotate(120_degd, p));
    TEST_EQUAL(q.str(), "H[0.5,0.5,0.5,0.5]");

    TRY(q = q_rotate(120_degd, z));
    TEST_EQUAL(q.str(), "H[1,0,0,0]");
    TRY(m3 = rotate3(q));
    TEST_EQUAL(m3.str(),
        "[[1,0,0],"
        "[0,1,0],"
        "[0,0,1]]"
    );
    TRY(m4 = rotate4(q));
    TEST_EQUAL(m4.str(),
        "[[1,0,0,0],"
        "[0,1,0,0],"
        "[0,0,1,0],"
        "[0,0,0,1]]"
    );

    TRY(q = q_rotate(30_degd, u));
    TRY(m3 = rotate3(q));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[1,0,0],"
        "[0,0.866025,-0.5],"
        "[0,0.5,0.866025]]"
    );
    TRY(m4 = rotate4(q));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[1,0,0,0],"
        "[0,0.866025,-0.5,0],"
        "[0,0.5,0.866025,0],"
        "[0,0,0,1]]"
    );
    TRY(r = rotate(q, u));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[1,0,0]");
    TRY(r = rotate(q, v));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0.866025,0.5]");
    TRY(r = rotate(q, w));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,-0.5,0.866025]");

    TRY(q = q_rotate(30_degd, v));
    TRY(m3 = rotate3(q));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,0,0.5],"
        "[0,1,0],"
        "[-0.5,0,0.866025]]"
    );
    TRY(m4 = rotate4(q));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,0,0.5,0],"
        "[0,1,0,0],"
        "[-0.5,0,0.866025,0],"
        "[0,0,0,1]]"
    );
    TRY(r = rotate(q, u));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0,-0.5]");
    TRY(r = rotate(q, v));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,1,0]");
    TRY(r = rotate(q, w));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.5,0,0.866025]");

    TRY(q = q_rotate(30_degd, w));
    TRY(m3 = rotate3(q));
    TRY(fuzz(m3));
    TEST_EQUAL(m3.str(),
        "[[0.866025,-0.5,0],"
        "[0.5,0.866025,0],"
        "[0,0,1]]"
    );
    TRY(m4 = rotate4(q));
    TRY(fuzz(m4));
    TEST_EQUAL(m4.str(),
        "[[0.866025,-0.5,0,0],"
        "[0.5,0.866025,0,0],"
        "[0,0,1,0],"
        "[0,0,0,1]]"
    );
    TRY(r = rotate(q, u));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0.866025,0.5,0]");
    TRY(r = rotate(q, v));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[-0.5,0.866025,0]");
    TRY(r = rotate(q, w));  TRY(fuzz(r));  TEST_EQUAL(r.str(), "[0,0,1]");

}
