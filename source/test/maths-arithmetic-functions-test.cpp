#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <functional>
#include <tuple>
#include <utility>

using namespace Crow;
using namespace std::literals;

void test_crow_maths_binomial_coefficients() {

    TEST_EQUAL(binomial(-1, -1),  0);
    TEST_EQUAL(binomial(-1, 0),   0);
    TEST_EQUAL(binomial(-1, 1),   0);
    TEST_EQUAL(binomial(0, -1),   0);
    TEST_EQUAL(binomial(0, 0),    1);
    TEST_EQUAL(binomial(0, 1),    0);
    TEST_EQUAL(binomial(1, -1),   0);
    TEST_EQUAL(binomial(1, 0),    1);
    TEST_EQUAL(binomial(1, 1),    1);
    TEST_EQUAL(binomial(1, 2),    0);
    TEST_EQUAL(binomial(2, -1),   0);
    TEST_EQUAL(binomial(2, 0),    1);
    TEST_EQUAL(binomial(2, 1),    2);
    TEST_EQUAL(binomial(2, 2),    1);
    TEST_EQUAL(binomial(2, 3),    0);
    TEST_EQUAL(binomial(3, -1),   0);
    TEST_EQUAL(binomial(3, 0),    1);
    TEST_EQUAL(binomial(3, 1),    3);
    TEST_EQUAL(binomial(3, 2),    3);
    TEST_EQUAL(binomial(3, 3),    1);
    TEST_EQUAL(binomial(3, 4),    0);
    TEST_EQUAL(binomial(4, -1),   0);
    TEST_EQUAL(binomial(4, 0),    1);
    TEST_EQUAL(binomial(4, 1),    4);
    TEST_EQUAL(binomial(4, 2),    6);
    TEST_EQUAL(binomial(4, 3),    4);
    TEST_EQUAL(binomial(4, 4),    1);
    TEST_EQUAL(binomial(4, 5),    0);
    TEST_EQUAL(binomial(5, -1),   0);
    TEST_EQUAL(binomial(5, 0),    1);
    TEST_EQUAL(binomial(5, 1),    5);
    TEST_EQUAL(binomial(5, 2),    10);
    TEST_EQUAL(binomial(5, 3),    10);
    TEST_EQUAL(binomial(5, 4),    5);
    TEST_EQUAL(binomial(5, 5),    1);
    TEST_EQUAL(binomial(5, 6),    0);

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

void test_crow_maths_integer_power() {

    TEST_EQUAL(integer_power(0ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(0ll, 1ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 2ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 3ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 4ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 5ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 10ll),   0ll);
    TEST_EQUAL(integer_power(0ll, 15ll),   0ll);
    TEST_EQUAL(integer_power(0ll, 20ll),   0ll);
    TEST_EQUAL(integer_power(1ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 1ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 2ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 3ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 4ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 5ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 10ll),   1ll);
    TEST_EQUAL(integer_power(1ll, 15ll),   1ll);
    TEST_EQUAL(integer_power(1ll, 20ll),   1ll);
    TEST_EQUAL(integer_power(2ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(2ll, 1ll),    2ll);
    TEST_EQUAL(integer_power(2ll, 2ll),    4ll);
    TEST_EQUAL(integer_power(2ll, 3ll),    8ll);
    TEST_EQUAL(integer_power(2ll, 4ll),    16ll);
    TEST_EQUAL(integer_power(2ll, 5ll),    32ll);
    TEST_EQUAL(integer_power(2ll, 10ll),   1'024ll);
    TEST_EQUAL(integer_power(2ll, 15ll),   32'768ll);
    TEST_EQUAL(integer_power(2ll, 20ll),   1'048'576ll);
    TEST_EQUAL(integer_power(5ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(5ll, 1ll),    5ll);
    TEST_EQUAL(integer_power(5ll, 2ll),    25ll);
    TEST_EQUAL(integer_power(5ll, 3ll),    125ll);
    TEST_EQUAL(integer_power(5ll, 4ll),    625ll);
    TEST_EQUAL(integer_power(5ll, 5ll),    3'125ll);
    TEST_EQUAL(integer_power(5ll, 10ll),   9'765'625ll);
    TEST_EQUAL(integer_power(5ll, 15ll),   30'517'578'125ll);
    TEST_EQUAL(integer_power(5ll, 20ll),   95'367'431'640'625ll);
    TEST_EQUAL(integer_power(-1ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 1ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 2ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 3ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 4ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 5ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 10ll),  1ll);
    TEST_EQUAL(integer_power(-1ll, 15ll),  -1ll);
    TEST_EQUAL(integer_power(-1ll, 20ll),  1ll);
    TEST_EQUAL(integer_power(-2ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-2ll, 1ll),   -2ll);
    TEST_EQUAL(integer_power(-2ll, 2ll),   4ll);
    TEST_EQUAL(integer_power(-2ll, 3ll),   -8ll);
    TEST_EQUAL(integer_power(-2ll, 4ll),   16ll);
    TEST_EQUAL(integer_power(-2ll, 5ll),   -32ll);
    TEST_EQUAL(integer_power(-2ll, 10ll),  1'024ll);
    TEST_EQUAL(integer_power(-2ll, 15ll),  -32'768ll);
    TEST_EQUAL(integer_power(-2ll, 20ll),  1'048'576ll);
    TEST_EQUAL(integer_power(-5ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-5ll, 1ll),   -5ll);
    TEST_EQUAL(integer_power(-5ll, 2ll),   25ll);
    TEST_EQUAL(integer_power(-5ll, 3ll),   -125ll);
    TEST_EQUAL(integer_power(-5ll, 4ll),   625ll);
    TEST_EQUAL(integer_power(-5ll, 5ll),   -3'125ll);
    TEST_EQUAL(integer_power(-5ll, 10ll),  9'765'625ll);
    TEST_EQUAL(integer_power(-5ll, 15ll),  -30'517'578'125ll);
    TEST_EQUAL(integer_power(-5ll, 20ll),  95'367'431'640'625ll);

}

void test_crow_maths_integer_power_generalised() {

    std::string a = "A";
    std::string s, z;
    auto f = std::plus<std::string>();

    TRY(s = integer_power(a, 0, f, z));    TEST_EQUAL(s, "");
    TRY(s = integer_power(a, 1, f, z));    TEST_EQUAL(s, "A");
    TRY(s = integer_power(a, 2, f, z));    TEST_EQUAL(s, "AA");
    TRY(s = integer_power(a, 3, f, z));    TEST_EQUAL(s, "AAA");
    TRY(s = integer_power(a, 4, f, z));    TEST_EQUAL(s, "AAAA");
    TRY(s = integer_power(a, 5, f, z));    TEST_EQUAL(s, "AAAAA");

}

void test_crow_maths_interpolation() {

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
