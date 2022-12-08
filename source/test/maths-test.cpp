#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <numbers>
#include <tuple>

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

    static const auto f = std::plus<std::string>();

    TEST_EQUAL(integer_power("A"s, 0, f, ""s),  "");
    TEST_EQUAL(integer_power("A"s, 1, f, ""s),  "A");
    TEST_EQUAL(integer_power("A"s, 2, f, ""s),  "AA");
    TEST_EQUAL(integer_power("A"s, 3, f, ""s),  "AAA");
    TEST_EQUAL(integer_power("A"s, 4, f, ""s),  "AAAA");
    TEST_EQUAL(integer_power("A"s, 5, f, ""s),  "AAAAA");

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

void test_crow_maths_sign() {

    TEST_EQUAL(sign_of(-42),    -1);
    TEST_EQUAL(sign_of(-1),     -1);
    TEST_EQUAL(sign_of(0),      0);
    TEST_EQUAL(sign_of(1),      1);
    TEST_EQUAL(sign_of(42),     1);
    TEST_EQUAL(sign_of(0u),     0);
    TEST_EQUAL(sign_of(1u),     1);
    TEST_EQUAL(sign_of(42u),    1);
    TEST_EQUAL(sign_of(-42.0),  -1);
    TEST_EQUAL(sign_of(-1.0),   -1);
    TEST_EQUAL(sign_of(0.0),    0);
    TEST_EQUAL(sign_of(1.0),    1);
    TEST_EQUAL(sign_of(42.0),   1);

}

void test_crow_maths_numeric_literals() {

    using namespace Crow::Literals;

    using std::numbers::pi_v;

    TEST_NEAR(45_degf,    pi_v<float> / 4.0f, 1e-5);
    TEST_NEAR(90_degf,    pi_v<float> / 2.0f, 1e-5);
    TEST_NEAR(180_degf,   pi_v<float>, 1e-5);
    TEST_NEAR(360_degf,   pi_v<float> * 2.0f, 1e-5);
    TEST_NEAR(45_degd,    pi_v<double> / 4.0, 1e-15);
    TEST_NEAR(90_degd,    pi_v<double> / 2.0, 1e-15);
    TEST_NEAR(180_degd,   pi_v<double>, 1e-15);
    TEST_NEAR(360_degd,   pi_v<double> * 2.0, 1e-15);
    TEST_NEAR(45_degld,   pi_v<long double> / 4.0l, 1e-15);
    TEST_NEAR(90_degld,   pi_v<long double> / 2.0l, 1e-15);
    TEST_NEAR(180_degld,  pi_v<long double>, 1e-15);
    TEST_NEAR(360_degld,  pi_v<long double> * 2.0l, 1e-15);

}
