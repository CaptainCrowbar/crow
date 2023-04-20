#include "crow/geometry.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"

using namespace Crow;

void test_crow_geometry_box() {

    Box_i2 b1, b2;
    Int2 p, v;

    TEST_EQUAL(b1.base().x(), 0);
    TEST_EQUAL(b1.base().y(), 0);
    TEST_EQUAL(b1.apex().x(), 0);
    TEST_EQUAL(b1.apex().y(), 0);
    TEST_EQUAL(b1.shape().x(), 0);
    TEST_EQUAL(b1.shape().y(), 0);
    TEST(b1.empty());
    TEST_EQUAL(b1.volume(), 0);
    TEST_EQUAL(b1.str(), "B[0,0]+[0,0]");

    TRY((p = {0, 0}));  TEST((! b1.contains(p)));
    TRY((p = {5, 5}));  TEST((! b1.contains(p)));

    TRY((b1 = {Int2(10, 20), Int2(30, 40)}));
    TEST_EQUAL(b1.base().x(), 10);
    TEST_EQUAL(b1.base().y(), 20);
    TEST_EQUAL(b1.apex().x(), 40);
    TEST_EQUAL(b1.apex().y(), 60);
    TEST_EQUAL(b1.shape().x(), 30);
    TEST_EQUAL(b1.shape().y(), 40);
    TEST_EQUAL(b1.centre().x(), 25);
    TEST_EQUAL(b1.centre().y(), 40);
    TEST(! b1.empty());
    TEST_EQUAL(b1.volume(), 1200);
    TEST_EQUAL(b1.str(), "B[10,20]+[30,40]");

    TRY((p = {20, 40}));  TEST((b1.contains(p)));
    TRY((p = {5, 40}));   TEST((! b1.contains(p)));
    TRY((p = {45, 40}));  TEST((! b1.contains(p)));
    TRY((p = {20, 15}));  TEST((! b1.contains(p)));
    TRY((p = {20, 65}));  TEST((! b1.contains(p)));

    TRY((b1 = {Int2(40, 80), Int2(-30, -60)}));
    TEST_EQUAL(b1.base().x(), 10);
    TEST_EQUAL(b1.base().y(), 20);
    TEST_EQUAL(b1.apex().x(), 40);
    TEST_EQUAL(b1.apex().y(), 80);
    TEST_EQUAL(b1.shape().x(), 30);
    TEST_EQUAL(b1.shape().y(), 60);

    TRY((b1 = {Int2(10, 20), Int2(30, 40)}));
    TRY(b2 = b1);
    TEST_EQUAL(b1, b2);
    TRY((b2 = {Int2(10, 20), Int2(30, 50)}));
    TEST(b1 != b2);

    TRY((b2 = {Int2(0, 30), Int2(20, 10)}));   TEST(! b1.contains(b2));
    TRY((b2 = {Int2(10, 30), Int2(25, 10)}));  TEST(b1.contains(b2));
    TRY((b2 = {Int2(10, 30), Int2(30, 10)}));  TEST(b1.contains(b2));
    TRY((b2 = {Int2(10, 30), Int2(35, 10)}));  TEST(! b1.contains(b2));
    TRY((b2 = {Int2(30, 30), Int2(10, 10)}));  TEST(b1.contains(b2));
    TRY((b2 = {Int2(30, 30), Int2(15, 10)}));  TEST(! b1.contains(b2));
    TRY((b2 = {Int2(40, 30), Int2(10, 10)}));  TEST(! b1.contains(b2));

    v = {5, 25};

    TRY(b2 = b1 + v);  TEST_EQUAL(b2.str(), "B[15,45]+[30,40]");
    TRY(b2 = b2 - v);  TEST_EQUAL(b2.str(), "B[10,20]+[30,40]");

    TRY((b1 = {Int2(10, 20), Int2(-30, 40)}));
    TEST_EQUAL(b1.base().x(), -20);
    TEST_EQUAL(b1.base().y(), 20);
    TEST_EQUAL(b1.shape().x(), 30);
    TEST_EQUAL(b1.shape().y(), 40);
    TEST_EQUAL(b1.str(), "B[-20,20]+[30,40]");

    TRY((b1 = {Int2(10, 20), Int2(30, -40)}));
    TEST_EQUAL(b1.base().x(), 10);
    TEST_EQUAL(b1.base().y(), -20);
    TEST_EQUAL(b1.shape().x(), 30);
    TEST_EQUAL(b1.shape().y(), 40);
    TEST_EQUAL(b1.str(), "B[10,-20]+[30,40]");

    TRY((b1 = {Int2(10, 20), Int2(-30, -40)}));
    TEST_EQUAL(b1.base().x(), -20);
    TEST_EQUAL(b1.base().y(), -20);
    TEST_EQUAL(b1.shape().x(), 30);
    TEST_EQUAL(b1.shape().y(), 40);
    TEST_EQUAL(b1.str(), "B[-20,-20]+[30,40]");

    TRY(b1 = Box_i2::unit());
    TEST_EQUAL(b1.base().x(), 0);
    TEST_EQUAL(b1.base().y(), 0);
    TEST_EQUAL(b1.shape().x(), 1);
    TEST_EQUAL(b1.shape().y(), 1);
    TEST(! b1.empty());
    TEST_EQUAL(b1.volume(), 1);
    TEST_EQUAL(b1.str(), "B[0,0]+[1,1]");

}

void test_crow_geometry_sphere() {

    Sphere_d3 s1, s2;
    Double3 p;
    double a = 0.0, v = 0.0;

    TEST_EQUAL(s1.centre(), Double3::null());
    TEST_EQUAL(s1.radius(), 0);
    TEST(s1.empty());
    TEST_EQUAL(s1.volume(), 0);
    TEST_EQUAL(s1.surface(), 0);

    TRY((p = {0.0,0.0,0.0}));  TEST(! s1.contains(p));
    TRY((p = {1.0,0.0,0.0}));  TEST(! s1.contains(p));

    TRY(s1 = Sphere_d3::unit());
    TEST_EQUAL(s1.str(), "S[0,0,0]+1");
    TEST_EQUAL(s1.centre(), Double3::null());
    TEST_EQUAL(s1.radius(), 1);
    TEST(! s1.empty());
    TEST_NEAR(s1.volume(), 4.188790, 1e-6);
    TEST_NEAR(s1.surface(), 12.566371, 1e-6);

    TRY((p = {0.0,0.0,0.0}));  TEST(s1.contains(p));
    TRY((p = {0.5,0.0,0.0}));  TEST(s1.contains(p));
    TRY((p = {1.0,0.0,0.0}));  TEST(! s1.contains(p));
    TRY((p = {1.5,0.0,0.0}));  TEST(! s1.contains(p));
    TRY((p = {2.0,0.0,0.0}));  TEST(! s1.contains(p));

    TRY((s1 = {{10.0,20.0,30.0}, -5.0}));
    TEST_EQUAL(s1.str(), "S[10,20,30]+5");
    TEST_EQUAL(s1.centre(), Double3(10.0,20.0,30.0));
    TEST_EQUAL(s1.radius(), 5);
    TEST(! s1.empty());
    TEST_NEAR(s1.volume(), 523.598776, 1e-6);
    TEST_NEAR(s1.surface(), 314.159265, 1e-6);

    TRY((p = {0.0,0.0,0.0}));     TEST(! s1.contains(p));
    TRY((p = {10.0,20.0,30.0}));  TEST(s1.contains(p));
    TRY((p = {12.0,22.0,32.0}));  TEST(s1.contains(p));
    TRY((p = {13.0,23.0,33.0}));  TEST(! s1.contains(p));

    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{0.0,0.0,0.0}, 1.0}));  TEST(s1.contains(s2));    TEST(! s1.disjoint(s2));
    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{1.0,0.0,0.0}, 1.0}));  TEST(s1.contains(s2));    TEST(! s1.disjoint(s2));
    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{2.0,0.0,0.0}, 1.0}));  TEST(s1.contains(s2));    TEST(! s1.disjoint(s2));
    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{3.0,0.0,0.0}, 1.0}));  TEST(! s1.contains(s2));  TEST(! s1.disjoint(s2));
    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{4.0,0.0,0.0}, 1.0}));  TEST(! s1.contains(s2));  TEST(s1.disjoint(s2));
    TRY((s1 = {{0.0,0.0,0.0}, 3.0})); TRY((s2 = {{5.0,0.0,0.0}, 1.0}));  TEST(! s1.contains(s2));  TEST(s1.disjoint(s2));

    TRY((v = Sphere<double, 1>::unit().volume()));   TEST_NEAR(v, 2.000000, 1e-6);
    TRY((v = Sphere<double, 2>::unit().volume()));   TEST_NEAR(v, 3.141593, 1e-6);
    TRY((v = Sphere<double, 3>::unit().volume()));   TEST_NEAR(v, 4.188790, 1e-6);
    TRY((v = Sphere<double, 4>::unit().volume()));   TEST_NEAR(v, 4.934802, 1e-6);
    TRY((v = Sphere<double, 5>::unit().volume()));   TEST_NEAR(v, 5.263789, 1e-6);
    TRY((v = Sphere<double, 6>::unit().volume()));   TEST_NEAR(v, 5.167713, 1e-6);
    TRY((v = Sphere<double, 7>::unit().volume()));   TEST_NEAR(v, 4.724766, 1e-6);
    TRY((v = Sphere<double, 8>::unit().volume()));   TEST_NEAR(v, 4.058712, 1e-6);
    TRY((v = Sphere<double, 9>::unit().volume()));   TEST_NEAR(v, 3.298509, 1e-6);
    TRY((v = Sphere<double, 10>::unit().volume()));  TEST_NEAR(v, 2.550164, 1e-6);

    TRY((a = Sphere<double, 1>::unit().surface()));   TEST_NEAR(a, 2.000000, 1e-6);
    TRY((a = Sphere<double, 2>::unit().surface()));   TEST_NEAR(a, 6.283185, 1e-6);
    TRY((a = Sphere<double, 3>::unit().surface()));   TEST_NEAR(a, 12.566371, 1e-6);
    TRY((a = Sphere<double, 4>::unit().surface()));   TEST_NEAR(a, 19.739209, 1e-6);
    TRY((a = Sphere<double, 5>::unit().surface()));   TEST_NEAR(a, 26.318945, 1e-6);
    TRY((a = Sphere<double, 6>::unit().surface()));   TEST_NEAR(a, 31.006277, 1e-6);
    TRY((a = Sphere<double, 7>::unit().surface()));   TEST_NEAR(a, 33.073362, 1e-6);
    TRY((a = Sphere<double, 8>::unit().surface()));   TEST_NEAR(a, 32.469697, 1e-6);
    TRY((a = Sphere<double, 9>::unit().surface()));   TEST_NEAR(a, 29.686580, 1e-6);
    TRY((a = Sphere<double, 10>::unit().surface()));  TEST_NEAR(a, 25.501640, 1e-6);

}
