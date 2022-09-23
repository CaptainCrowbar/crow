#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <numbers>
#include <tuple>

using namespace Crow;

void test_crow_maths_rounding() {

    TEST_EQUAL(const_round<int>(-4.00),  -4);
    TEST_EQUAL(const_round<int>(-3.75),  -4);
    TEST_EQUAL(const_round<int>(-3.50),  -3);
    TEST_EQUAL(const_round<int>(-3.25),  -3);
    TEST_EQUAL(const_round<int>(-3.00),  -3);
    TEST_EQUAL(const_round<int>(-2.75),  -3);
    TEST_EQUAL(const_round<int>(-2.50),  -2);
    TEST_EQUAL(const_round<int>(-2.25),  -2);
    TEST_EQUAL(const_round<int>(-2.00),  -2);
    TEST_EQUAL(const_round<int>(-1.75),  -2);
    TEST_EQUAL(const_round<int>(-1.50),  -1);
    TEST_EQUAL(const_round<int>(-1.25),  -1);
    TEST_EQUAL(const_round<int>(-1.00),  -1);
    TEST_EQUAL(const_round<int>(-0.75),  -1);
    TEST_EQUAL(const_round<int>(-0.50),  0);
    TEST_EQUAL(const_round<int>(-0.25),  0);
    TEST_EQUAL(const_round<int>(0.00),   0);
    TEST_EQUAL(const_round<int>(0.25),   0);
    TEST_EQUAL(const_round<int>(0.50),   1);
    TEST_EQUAL(const_round<int>(0.75),   1);
    TEST_EQUAL(const_round<int>(1.00),   1);
    TEST_EQUAL(const_round<int>(1.25),   1);
    TEST_EQUAL(const_round<int>(1.50),   2);
    TEST_EQUAL(const_round<int>(1.75),   2);
    TEST_EQUAL(const_round<int>(2.00),   2);
    TEST_EQUAL(const_round<int>(2.25),   2);
    TEST_EQUAL(const_round<int>(2.50),   3);
    TEST_EQUAL(const_round<int>(2.75),   3);
    TEST_EQUAL(const_round<int>(3.00),   3);
    TEST_EQUAL(const_round<int>(3.25),   3);
    TEST_EQUAL(const_round<int>(3.50),   4);
    TEST_EQUAL(const_round<int>(3.75),   4);
    TEST_EQUAL(const_round<int>(4.00),   4);

}

void test_crow_maths_euclidean_division() {

    {

        unsigned x = 0, y = 0, q = 0, r = 0;

        x = 0;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0u);  TEST_EQUAL(r, 0u);  TEST_EQUAL(q * y + r, x);
        x = 1;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0u);  TEST_EQUAL(r, 1u);  TEST_EQUAL(q * y + r, x);
        x = 2;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0u);  TEST_EQUAL(r, 2u);  TEST_EQUAL(q * y + r, x);
        x = 3;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1u);  TEST_EQUAL(r, 0u);  TEST_EQUAL(q * y + r, x);
        x = 4;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1u);  TEST_EQUAL(r, 1u);  TEST_EQUAL(q * y + r, x);
        x = 5;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1u);  TEST_EQUAL(r, 2u);  TEST_EQUAL(q * y + r, x);
        x = 6;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2u);  TEST_EQUAL(r, 0u);  TEST_EQUAL(q * y + r, x);
        x = 7;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2u);  TEST_EQUAL(r, 1u);  TEST_EQUAL(q * y + r, x);
        x = 8;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2u);  TEST_EQUAL(r, 2u);  TEST_EQUAL(q * y + r, x);
        x = 9;   y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 3u);  TEST_EQUAL(r, 0u);  TEST_EQUAL(q * y + r, x);
        x = 10;  y = 3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 3u);  TEST_EQUAL(r, 1u);  TEST_EQUAL(q * y + r, x);

    }

    {

        int x = 0, y = 0, q = 0, r = 0;

        x = -6;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -4;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 0;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 4;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -4;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 0;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 4;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);

    }

    {

        double x = 0, y = 0, q = 0, r = 0;

        x = -6;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -4;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 0;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 4;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = 3;   TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -4;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 0;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 4;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = -3;  TRY(std::tie(q, r) = euclidean_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);

    }

}

void test_crow_maths_fraction() {

    TEST_EQUAL(fraction(-3.00),  0.00);
    TEST_EQUAL(fraction(-2.75),  0.25);
    TEST_EQUAL(fraction(-2.50),  0.50);
    TEST_EQUAL(fraction(-2.25),  0.75);
    TEST_EQUAL(fraction(-2.00),  0.00);
    TEST_EQUAL(fraction(-1.75),  0.25);
    TEST_EQUAL(fraction(-1.50),  0.50);
    TEST_EQUAL(fraction(-1.25),  0.75);
    TEST_EQUAL(fraction(-1.00),  0.00);
    TEST_EQUAL(fraction(-0.75),  0.25);
    TEST_EQUAL(fraction(-0.50),  0.50);
    TEST_EQUAL(fraction(-0.25),  0.75);
    TEST_EQUAL(fraction(0.00),   0.00);
    TEST_EQUAL(fraction(0.25),   0.25);
    TEST_EQUAL(fraction(0.50),   0.50);
    TEST_EQUAL(fraction(0.75),   0.75);
    TEST_EQUAL(fraction(1.00),   0.00);
    TEST_EQUAL(fraction(1.25),   0.25);
    TEST_EQUAL(fraction(1.50),   0.50);
    TEST_EQUAL(fraction(1.75),   0.75);
    TEST_EQUAL(fraction(2.00),   0.00);
    TEST_EQUAL(fraction(2.25),   0.25);
    TEST_EQUAL(fraction(2.50),   0.50);
    TEST_EQUAL(fraction(2.75),   0.75);
    TEST_EQUAL(fraction(3.00),   0.00);

}

void test_crow_maths_interpolate() {

    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 0.0), 0.50);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 1.0), 0.75);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 2.0), 1.00);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 3.0), 1.25);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 4.0), 1.50);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 5.0), 1.75);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 6.0), 2.00);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 7.0), 2.25);
    TEST_EQUAL(interpolate(2.0, 1.0, 6.0, 2.0, 8.0), 2.50);

}

void test_crow_maths_symmetric_division() {

    {

        int x = 0, y = 0, q = 0, r = 0;

        x = -8;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -7;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = -4;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 0;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 4;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 7;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 8;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -8;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -7;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = -4;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 0;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 4;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 7;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 8;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);

    }

    {

        double x = 0, y = 0, q = 0, r = 0;

        x = -8;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -7;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = -4;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 0;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 4;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 7;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 8;   y = 4;   TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -8;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -7;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -6;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -5;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = -4;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = -3;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = -2;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = -1;  y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 0;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 1;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 2;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 3;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 4;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);
        x = 5;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);   TEST_EQUAL(q * y + r, x);
        x = 6;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);   TEST_EQUAL(q * y + r, x);
        x = 7;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, -1);  TEST_EQUAL(q * y + r, x);
        x = 8;   y = -4;  TRY(std::tie(q, r) = symmetric_divide(x, y));  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);   TEST_EQUAL(q * y + r, x);

    }

}

void test_crow_maths_numeric_literals() {

    using namespace Crow::Literals;
    using namespace std::numbers;

    TEST_NEAR(45_degf,    pi_v<float> / 4.0f, 1e-5);
    TEST_NEAR(90_degf,    pi_v<float> / 2.0f, 1e-5);
    TEST_NEAR(180_degf,   pi_v<float>, 1e-5);
    TEST_NEAR(360_degf,   pi_v<float> * 2.0f, 1e-5);
    TEST_NEAR(45_degd,    pi / 4.0, 1e-15);
    TEST_NEAR(90_degd,    pi / 2.0, 1e-15);
    TEST_NEAR(180_degd,   pi, 1e-15);
    TEST_NEAR(360_degd,   pi * 2.0, 1e-15);
    TEST_NEAR(45_degld,   pi_v<long double> / 4.0l, 1e-15);
    TEST_NEAR(90_degld,   pi_v<long double> / 2.0l, 1e-15);
    TEST_NEAR(180_degld,  pi_v<long double>, 1e-15);
    TEST_NEAR(360_degld,  pi_v<long double> * 2.0l, 1e-15);

}
