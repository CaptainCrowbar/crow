#include "crow/maths.hpp"
#include "crow/unit-test.hpp"
#include <utility>

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

void test_crow_maths_emodf() {

    std::pair<double, double> p;

    TRY(p = emodf(-3.00));  TEST_EQUAL(p.first, -3);  TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(-2.75));  TEST_EQUAL(p.first, -3);  TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(-2.50));  TEST_EQUAL(p.first, -3);  TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(-2.25));  TEST_EQUAL(p.first, -3);  TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(-2.00));  TEST_EQUAL(p.first, -2);  TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(-1.75));  TEST_EQUAL(p.first, -2);  TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(-1.50));  TEST_EQUAL(p.first, -2);  TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(-1.25));  TEST_EQUAL(p.first, -2);  TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(-1.00));  TEST_EQUAL(p.first, -1);  TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(-0.75));  TEST_EQUAL(p.first, -1);  TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(-0.50));  TEST_EQUAL(p.first, -1);  TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(-0.25));  TEST_EQUAL(p.first, -1);  TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(0.00));   TEST_EQUAL(p.first, 0);   TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(0.25));   TEST_EQUAL(p.first, 0);   TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(0.50));   TEST_EQUAL(p.first, 0);   TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(0.75));   TEST_EQUAL(p.first, 0);   TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(1.00));   TEST_EQUAL(p.first, 1);   TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(1.25));   TEST_EQUAL(p.first, 1);   TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(1.50));   TEST_EQUAL(p.first, 1);   TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(1.75));   TEST_EQUAL(p.first, 1);   TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(2.00));   TEST_EQUAL(p.first, 2);   TEST_EQUAL(p.second, 0.00);
    TRY(p = emodf(2.25));   TEST_EQUAL(p.first, 2);   TEST_EQUAL(p.second, 0.25);
    TRY(p = emodf(2.50));   TEST_EQUAL(p.first, 2);   TEST_EQUAL(p.second, 0.50);
    TRY(p = emodf(2.75));   TEST_EQUAL(p.first, 2);   TEST_EQUAL(p.second, 0.75);
    TRY(p = emodf(3.00));   TEST_EQUAL(p.first, 3);   TEST_EQUAL(p.second, 0.00);

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
