#include "crow/vector.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include "test/vector-test.hpp"
#include <numbers>
#include <set>
#include <tuple>
#include <unordered_set>

using namespace Crow;
using namespace Crow::Literals;

using std::numbers::pi;

void test_crow_integer_vector_construction() {

    Int3 v1, v2, v3;
    const Int3& cv1 = v1;
    const Int3& cv2 = v2;
    const Int3& cv3 = v3;

    TEST_EQUAL(sizeof(Int3), 3 * sizeof(int));

    TRY(v1 = Int3(1));
    TRY(v2 = Int3(0));
    TEST(! v1.is_null());
    TEST(v2.is_null());

    TEST_EQUAL(v1.str(), "[1,1,1]");
    TEST_EQUAL(cv1.str(), "[1,1,1]");

    TEST_EQUAL(v1[0], 1);  TEST_EQUAL(cv1[0], 1);
    TEST_EQUAL(v1[1], 1);  TEST_EQUAL(cv1[1], 1);
    TEST_EQUAL(v1[2], 1);  TEST_EQUAL(cv1[2], 1);

    TRY(v2[0] = 99);
    TRY(v2[1] = 98);
    TRY(v2[2] = 97);
    TEST_EQUAL(v2.str(), "[99,98,97]");
    TEST_EQUAL(cv2.str(), "[99,98,97]");

    TEST_EQUAL(v2[0], 99);   TEST_EQUAL(cv2[0], 99);
    TEST_EQUAL(v2[1], 98);   TEST_EQUAL(cv2[1], 98);
    TEST_EQUAL(v2[2], 97);   TEST_EQUAL(cv2[2], 97);
    TEST_EQUAL(v2.x(), 99);  TEST_EQUAL(cv2.x(), 99);
    TEST_EQUAL(v2.y(), 98);  TEST_EQUAL(cv2.y(), 98);
    TEST_EQUAL(v2.z(), 97);  TEST_EQUAL(cv2.z(), 97);

    TRY((v3 = Int3{1,2,3}));
    TEST_EQUAL(v3.str(), "[1,2,3]");
    TEST_EQUAL(cv3.str(), "[1,2,3]");

    TRY((v3 = Int3(2,4,6)));
    TEST_EQUAL(v3.str(), "[2,4,6]");
    TEST_EQUAL(cv3.str(), "[2,4,6]");

    TRY((v3 = {10,20,30}));
    TEST_EQUAL(v3.str(), "[10,20,30]");
    TEST_EQUAL(cv3.str(), "[10,20,30]");

    TEST_EQUAL(v3[0], 10);          TEST_EQUAL(cv3[0], 10);
    TEST_EQUAL(v3[1], 20);          TEST_EQUAL(cv3[1], 20);
    TEST_EQUAL(v3[2], 30);          TEST_EQUAL(cv3[2], 30);
    TEST_EQUAL(v3.begin()[0], 10);  TEST_EQUAL(cv3.begin()[0], 10);
    TEST_EQUAL(v3.begin()[1], 20);  TEST_EQUAL(cv3.begin()[1], 20);
    TEST_EQUAL(v3.begin()[2], 30);  TEST_EQUAL(cv3.begin()[2], 30);

}

void test_crow_integer_vector_arithmetic() {

    int x = 0;
    Int3 v1, v2, v3, v4;

    TRY((v1 = {2,3,5}));
    TRY((v2 = {7,11,13}));
    TRY(v3 = Int3(0));
    TRY(v4 = Int3(0));
    TEST(v1 == v1);
    TEST(v1 != v2);
    TEST(v3 == v4);

    TRY(v3 = + v1);     TEST_EQUAL(v3.str(), "[2,3,5]");
    TRY(v3 = - v1);     TEST_EQUAL(v3.str(), "[-2,-3,-5]");
    TRY(v3 = v1 + v2);  TEST_EQUAL(v3.str(), "[9,14,18]");
    TRY(v3 = v1 - v2);  TEST_EQUAL(v3.str(), "[-5,-8,-8]");
    TRY(v3 = 17 * v1);  TEST_EQUAL(v3.str(), "[34,51,85]");
    TRY(v3 = v1 * 19);  TEST_EQUAL(v3.str(), "[38,57,95]");
    TRY(x = v1 % v2);   TEST_EQUAL(x, 112);
    TRY(v3 = v1 ^ v2);  TEST_EQUAL(v3.str(), "[-16,9,1]");
    TRY(v3 = v1 * v2);  TEST_EQUAL(v3.str(), "[14,33,65]");
    TRY(v3 = v2 / v1);  TEST_EQUAL(v3.str(), "[3,3,2]");

    TRY(v3 = Int3::unit(0));  TEST_EQUAL(v3.str(), "[1,0,0]");
    TRY(v3 = Int3::unit(1));  TEST_EQUAL(v3.str(), "[0,1,0]");
    TRY(v3 = Int3::unit(2));  TEST_EQUAL(v3.str(), "[0,0,1]");

    TRY((v1 = {1,5,9}));
    TRY((v2 = {2,3,4}));
    TRY((v3 = {4,6,8}));
    TRY(v1 = clampv(v1, v2, v3));
    TEST_EQUAL(v1.str(), "[2,5,8]");

    TRY((v1 = {1,3,5}));
    TRY((v2 = {2,3,4}));
    TRY(v3 = minv(v1, v2));
    TEST_EQUAL(v3.str(), "[1,3,4]");
    TRY(v3 = maxv(v1, v2));
    TEST_EQUAL(v3.str(), "[2,3,5]");
    TRY(std::tie(v3, v4) = minmaxv(v1, v2));
    TEST_EQUAL(v3.str(), "[1,3,4]");
    TEST_EQUAL(v4.str(), "[2,3,5]");

}

void test_crow_integer_vector_hash() {

    std::unordered_set<Int3> set;
    Int3 v1, v2, v3;

    TRY((v1 = {1,2,3}));
    TRY((v2 = {4,5,6}));
    TRY((v3 = {7,8,9}));
    TRY(set.insert(v1));
    TRY(set.insert(v2));
    TRY(set.insert(v1));
    TEST_EQUAL(set.size(), 2u);
    TEST_EQUAL(set.count(v1), 1u);
    TEST_EQUAL(set.count(v2), 1u);
    TEST_EQUAL(set.count(v3), 0u);

}

void test_crow_integer_vector_interpolation() {

    int x = 0;

    TRY(x = lerp(50, 100, -0.50));  TEST_EQUAL(x, 25);
    TRY(x = lerp(50, 100, -0.25));  TEST_EQUAL(x, 38);
    TRY(x = lerp(50, 100, 0.00));   TEST_EQUAL(x, 50);
    TRY(x = lerp(50, 100, 0.25));   TEST_EQUAL(x, 63);
    TRY(x = lerp(50, 100, 0.50));   TEST_EQUAL(x, 75);
    TRY(x = lerp(50, 100, 0.75));   TEST_EQUAL(x, 88);
    TRY(x = lerp(50, 100, 1.00));   TEST_EQUAL(x, 100);
    TRY(x = lerp(50, 100, 1.25));   TEST_EQUAL(x, 113);
    TRY(x = lerp(50, 100, 1.50));   TEST_EQUAL(x, 125);

    Int3 u = {10,20,30};
    Int3 v = {12,34,56};
    Int3 w;

    TRY(w = lerp(u, v, -0.50));  TEST_VECTORS(w, Int3(9,13,17), 0);
    TRY(w = lerp(u, v, -0.25));  TEST_VECTORS(w, Int3(10,17,24), 0);
    TRY(w = lerp(u, v, 0.00));   TEST_VECTORS(w, Int3(10,20,30), 0);
    TRY(w = lerp(u, v, 0.25));   TEST_VECTORS(w, Int3(11,24,37), 0);
    TRY(w = lerp(u, v, 0.50));   TEST_VECTORS(w, Int3(11,27,43), 0);
    TRY(w = lerp(u, v, 0.75));   TEST_VECTORS(w, Int3(12,31,50), 0);
    TRY(w = lerp(u, v, 1.00));   TEST_VECTORS(w, Int3(12,34,56), 0);
    TRY(w = lerp(u, v, 1.25));   TEST_VECTORS(w, Int3(13,38,63), 0);
    TRY(w = lerp(u, v, 1.50));   TEST_VECTORS(w, Int3(13,41,69), 0);

}

void test_crow_integer_vector_total_order() {

    std::set<Int3> set;
    Int3 v1, v2, v3;

    TRY((v1 = {1,2,3}));
    TRY((v2 = {4,5,6}));
    TRY((v3 = {7,8,9}));
    TRY(set.insert(v1));
    TRY(set.insert(v2));
    TRY(set.insert(v1));
    TEST_EQUAL(set.size(), 2u);
    TEST_EQUAL(set.count(v1), 1u);
    TEST_EQUAL(set.count(v2), 1u);
    TEST_EQUAL(set.count(v3), 0u);
    TEST_EQUAL(format_range(set), "[[1,2,3],[4,5,6]]");

}

void test_crow_floating_vector_construction() {

    Double3 v1, v2, v3;
    const Double3& cv1(v1);
    const Double3& cv2(v2);
    const Double3& cv3(v3);

    TEST_EQUAL(sizeof(Double3), 3 * sizeof(double));

    TRY(v1 = Double3(1.0));
    TRY(v2 = Double3(0.0));
    TEST(! v1.is_null());
    TEST(v2.is_null());

    TEST_EQUAL(v1.str(), "[1,1,1]");
    TEST_EQUAL(cv1.str(), "[1,1,1]");
    TEST_EQUAL(v1.str("e3"), "[1.00e0,1.00e0,1.00e0]");
    TEST_EQUAL(v1.str("f3"), "[1.000,1.000,1.000]");
    TEST_EQUAL(Formatter("{0:e3}")(v1), "[1.00e0,1.00e0,1.00e0]");
    TEST_EQUAL(Formatter("{0:f3}")(v1), "[1.000,1.000,1.000]");
    TEST_EQUAL("{0:e3}"_fmt(v1), "[1.00e0,1.00e0,1.00e0]");
    TEST_EQUAL("{0:f3}"_fmt(v1), "[1.000,1.000,1.000]");

    TEST_EQUAL(v1[0], 1);  TEST_EQUAL(cv1[0], 1);
    TEST_EQUAL(v1[1], 1);  TEST_EQUAL(cv1[1], 1);
    TEST_EQUAL(v1[2], 1);  TEST_EQUAL(cv1[2], 1);

    TRY(v2[0] = 99);
    TRY(v2[1] = 98);
    TRY(v2[2] = 97);
    TEST_EQUAL(v2.str(), "[99,98,97]");
    TEST_EQUAL(cv2.str(), "[99,98,97]");

    TEST_EQUAL(v2[0], 99);   TEST_EQUAL(cv2[0], 99);
    TEST_EQUAL(v2[1], 98);   TEST_EQUAL(cv2[1], 98);
    TEST_EQUAL(v2[2], 97);   TEST_EQUAL(cv2[2], 97);
    TEST_EQUAL(v2.x(), 99);  TEST_EQUAL(cv2.x(), 99);
    TEST_EQUAL(v2.y(), 98);  TEST_EQUAL(cv2.y(), 98);
    TEST_EQUAL(v2.z(), 97);  TEST_EQUAL(cv2.z(), 97);

    TRY((v3 = Double3{1,2,3}));
    TEST_EQUAL(v3.str(), "[1,2,3]");
    TEST_EQUAL(cv3.str(), "[1,2,3]");

    TRY((v3 = Double3(2,4,6)));
    TEST_EQUAL(v3.str(), "[2,4,6]");
    TEST_EQUAL(cv3.str(), "[2,4,6]");

    TRY((v3 = {10,20,30}));
    TEST_EQUAL(v3.str(), "[10,20,30]");
    TEST_EQUAL(cv3.str(), "[10,20,30]");

    TEST_EQUAL(v3[0], 10);          TEST_EQUAL(cv3[0], 10);
    TEST_EQUAL(v3[1], 20);          TEST_EQUAL(cv3[1], 20);
    TEST_EQUAL(v3[2], 30);          TEST_EQUAL(cv3[2], 30);
    TEST_EQUAL(v3.begin()[0], 10);  TEST_EQUAL(cv3.begin()[0], 10);
    TEST_EQUAL(v3.begin()[1], 20);  TEST_EQUAL(cv3.begin()[1], 20);
    TEST_EQUAL(v3.begin()[2], 30);  TEST_EQUAL(cv3.begin()[2], 30);

}

void test_crow_floating_vector_arithmetic() {

    double x = 0;
    Double3 v1, v2, v3, v4, v5;

    TRY((v1 = {2,3,5}));
    TRY((v2 = {7,11,13}));
    TRY(v3 = Double3(0.0));
    TRY(v4 = Double3(0.0));
    TEST(v1 == v1);
    TEST(v1 != v2);
    TEST(v3 == v4);

    TRY(v3 = + v1);     TEST_EQUAL(v3.str(), "[2,3,5]");
    TRY(v3 = - v1);     TEST_EQUAL(v3.str(), "[-2,-3,-5]");
    TRY(v3 = v1 + v2);  TEST_EQUAL(v3.str(), "[9,14,18]");
    TRY(v3 = v1 - v2);  TEST_EQUAL(v3.str(), "[-5,-8,-8]");
    TRY(v3 = 17 * v1);  TEST_EQUAL(v3.str(), "[34,51,85]");
    TRY(v3 = v1 * 19);  TEST_EQUAL(v3.str(), "[38,57,95]");
    TRY(v3 = v1 / 23);  TEST_EQUAL(v3.str(), "[0.0869565,0.130435,0.217391]");
    TRY(x = v1 % v2);   TEST_EQUAL(x, 112);
    TRY(v3 = v1 ^ v2);  TEST_EQUAL(v3.str(), "[-16,9,1]");
    TRY(v3 = v1 * v2);  TEST_EQUAL(v3.str(), "[14,33,65]");
    TRY(v3 = v1 / v2);  TEST_EQUAL(v3.str(), "[0.285714,0.272727,0.384615]");

    TEST_EQUAL(v1.r2(), 38);   TEST_NEAR(v1.r(), 6.164414, 1e-6);
    TEST_EQUAL(v2.r2(), 339);  TEST_NEAR(v2.r(), 18.411953, 1e-6);

    TRY(v3 = Double3::unit(0));  TEST_EQUAL(v3.str(), "[1,0,0]");
    TRY(v3 = Double3::unit(1));  TEST_EQUAL(v3.str(), "[0,1,0]");
    TRY(v3 = Double3::unit(2));  TEST_EQUAL(v3.str(), "[0,0,1]");

    TRY(v3 = Double3::null().dir());  TEST_EQUAL(v3.str(), "[0,0,0]");
    TRY(v3 = v1.dir());               TEST_EQUAL(v3.str(), "[0.324443,0.486664,0.811107]");
    TRY(v3 = v2.dir());               TEST_EQUAL(v3.str(), "[0.380188,0.597438,0.706063]");

    TRY((v1 = {1,2,3}));

    TRY((v2 = {0,0,0}));     TRY(x = v1.angle(v2));  TEST_EQUAL(x, 0);
    TRY((v2 = {2,4,6}));     TRY(x = v1.angle(v2));  TEST_EQUAL(x, 0);
    TRY((v2 = {-2,-4,-6}));  TRY(x = v1.angle(v2));  TEST_NEAR(x, pi, 1e-6);
    TRY((v2 = {3,2,1}));     TRY(x = v1.angle(v2));  TEST_NEAR(x, 0.775193, 1e-6);
    TRY((v2 = {-3,-2,-1}));  TRY(x = v1.angle(v2));  TEST_NEAR(x, 2.366399, 1e-6);

    TRY((v1 = {2,3,5}));
    TRY((v2 = {7,11,13}));
    TRY(v3 = v1.project(v2));
    TRY(v4 = v1.reject(v2));
    TRY(v5 = v3 + v4);
    TEST_VECTORS(v5, v1, 1e-10);
    TRY(v5 = v2 ^ v3);
    TEST_VECTORS(v5, Double3::null(), 1e-10);
    TEST_NEAR(v2 % v4, 0, 1e-6);
    TEST_NEAR(v3 % v4, 0, 1e-6);

    TRY((v1 = {1,5,9}));
    TRY((v2 = {2,3,4}));
    TRY((v3 = {4,6,8}));
    TRY(v1 = clampv(v1, v2, v3));
    TEST_EQUAL(v1.str(), "[2,5,8]");

    TRY((v1 = {1,3,5}));
    TRY((v2 = {2,3,4}));
    TRY(v3 = minv(v1, v2));
    TEST_EQUAL(v3.str(), "[1,3,4]");
    TRY(v3 = maxv(v1, v2));
    TEST_EQUAL(v3.str(), "[2,3,5]");
    TRY(std::tie(v3, v4) = minmaxv(v1, v2));
    TEST_EQUAL(v3.str(), "[1,3,4]");
    TEST_EQUAL(v4.str(), "[2,3,5]");

}

void test_crow_floating_vector_interpolation() {

    double x = 0;

    TRY(x = lerp(50.0, 100.0, -0.50));  TEST_EQUAL(x, 25.0);
    TRY(x = lerp(50.0, 100.0, -0.25));  TEST_EQUAL(x, 37.5);
    TRY(x = lerp(50.0, 100.0, 0.00));   TEST_EQUAL(x, 50.0);
    TRY(x = lerp(50.0, 100.0, 0.25));   TEST_EQUAL(x, 62.5);
    TRY(x = lerp(50.0, 100.0, 0.50));   TEST_EQUAL(x, 75.0);
    TRY(x = lerp(50.0, 100.0, 0.75));   TEST_EQUAL(x, 87.5);
    TRY(x = lerp(50.0, 100.0, 1.00));   TEST_EQUAL(x, 100.0);
    TRY(x = lerp(50.0, 100.0, 1.25));   TEST_EQUAL(x, 112.5);
    TRY(x = lerp(50.0, 100.0, 1.50));   TEST_EQUAL(x, 125.0);

    Double3 u = {10,20,30};
    Double3 v = {12,34,56};
    Double3 w;

    TRY(w = lerp(u, v, -0.50));  TEST_VECTORS(w, Double3(9.0,13.0,17.0), 0);
    TRY(w = lerp(u, v, -0.25));  TEST_VECTORS(w, Double3(9.5,16.5,23.5), 0);
    TRY(w = lerp(u, v, 0.00));   TEST_VECTORS(w, Double3(10.0,20.0,30.0), 0);
    TRY(w = lerp(u, v, 0.25));   TEST_VECTORS(w, Double3(10.5,23.5,36.5), 0);
    TRY(w = lerp(u, v, 0.50));   TEST_VECTORS(w, Double3(11.0,27.0,43.0), 0);
    TRY(w = lerp(u, v, 0.75));   TEST_VECTORS(w, Double3(11.5,30.5,49.5), 0);
    TRY(w = lerp(u, v, 1.00));   TEST_VECTORS(w, Double3(12.0,34.0,56.0), 0);
    TRY(w = lerp(u, v, 1.25));   TEST_VECTORS(w, Double3(12.5,37.5,62.5), 0);
    TRY(w = lerp(u, v, 1.50));   TEST_VECTORS(w, Double3(13.0,41.0,69.0), 0);

}
