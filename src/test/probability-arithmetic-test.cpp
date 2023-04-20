#include "crow/probability.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

using PD = Probability<double>;
using PT = Detail::ProbabilityTest;

void test_crow_probability_complement() {

    PD x, y;

    TRY(x = 0);      TRY(y = ~ x);  TEST_EQUAL(y.value(), 1);      TEST_EQUAL(PT::get_data(y), 1);
    TRY(x = 0.125);  TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.875);  TEST_EQUAL(PT::get_data(y), -0.125);
    TRY(x = 0.25);   TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.75);   TEST_EQUAL(PT::get_data(y), -0.25);
    TRY(x = 0.375);  TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.625);  TEST_EQUAL(PT::get_data(y), -0.375);
    TRY(x = 0.5);    TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.5);    TEST_EQUAL(PT::get_data(y), 0.5);
    TRY(x = 0.625);  TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.375);  TEST_EQUAL(PT::get_data(y), 0.375);
    TRY(x = 0.75);   TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.25);   TEST_EQUAL(PT::get_data(y), 0.25);
    TRY(x = 0.875);  TRY(y = ~ x);  TEST_EQUAL(y.value(), 0.125);  TEST_EQUAL(PT::get_data(y), 0.125);
    TRY(x = 1);      TRY(y = ~ x);  TEST_EQUAL(y.value(), 0);      TEST_EQUAL(PT::get_data(y), 0);

}

void test_crow_probability_addition_subtraction() {

    PD x, y, z;

    TRY(x = 0);      TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.125);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.375);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 0.5);    TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.625);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.875);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0.125);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.125);  TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.25);   TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.5);    TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 0.625);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.75);   TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0.375);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.375);  TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.5);    TRY(z = x - y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.375);  TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.75);   TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 0.875);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0.5);    TRY(z = x - y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.5);    TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.625);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.5);    TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.875);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.5);    TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0.625);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.625);
    TRY(x = 0.625);  TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 0.75);   TRY(z = x - y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.625);  TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.625);  TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.625);  TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.875);  TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 0.875);  TRY(z = x - y);  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 0.875);  TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.75);
    TRY(x = 0.875);  TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.875);  TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.875);  TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.875);  TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.875);  TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 1);      TRY(y = 0);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      TRY(y = 0.125);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 1);      TRY(y = 0.375);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.625);
    TRY(x = 1);      TRY(y = 0.5);    TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 1);      TRY(y = 0.625);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 1);      TRY(y = 0.875);  TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 1);      TRY(y = 1);      TRY(z = x + y);  TEST_EQUAL(z.value(), 1);      TRY(z = x - y);  TEST_EQUAL(z.value(), 0);

}

void test_crow_probability_multiplication_division() {

    PD x, z;
    double y = 0;

    TRY(x = 0);      y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.015625);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.125);  y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.046875);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.3333333333, 1e-10);
    TRY(x = 0.125);  y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.0625);    TRY(z = x / y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.125);  y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.078125);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.2, 1e-10);
    TRY(x = 0.125);  y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.109375);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.1428571429, 1e-10);
    TRY(x = 0.125);  y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.125);     TRY(z = x / y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.125);  y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 0.15625);   TRY(z = x / y);  TEST_NEAR(z.value(), 0.1, 1e-10);
    TRY(x = 0.125);  y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.1875);    TRY(z = x / y);  TEST_NEAR(z.value(), 0.0833333333, 1e-10);
    TRY(x = 0.125);  y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.25);      TRY(z = x / y);  TEST_EQUAL(z.value(), 0.0625);
    TRY(x = 0.125);  y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.5);       TRY(z = x / y);  TEST_EQUAL(z.value(), 0.03125);
    TRY(x = 0.375);  y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.046875);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.375);  y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.140625);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.375);  y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.1875);    TRY(z = x / y);  TEST_EQUAL(z.value(), 0.75);
    TRY(x = 0.375);  y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.234375);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.6, 1e-10);
    TRY(x = 0.375);  y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.328125);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.4285714286, 1e-10);
    TRY(x = 0.375);  y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.375);     TRY(z = x / y);  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.375);  y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 0.46875);   TRY(z = x / y);  TEST_NEAR(z.value(), 0.3, 1e-10);
    TRY(x = 0.375);  y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.5625);    TRY(z = x / y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.375);  y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.75);      TRY(z = x / y);  TEST_EQUAL(z.value(), 0.1875);
    TRY(x = 0.375);  y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.09375);
    TRY(x = 0.5);    y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.0625);    TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.1875);    TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.25);      TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.3125);    TRY(z = x / y);  TEST_NEAR(z.value(), 0.8, 1e-10);
    TRY(x = 0.5);    y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.4375);    TRY(z = x / y);  TEST_NEAR(z.value(), 0.5714285714, 1e-10);
    TRY(x = 0.5);    y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.5);       TRY(z = x / y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.5);    y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 0.625);     TRY(z = x / y);  TEST_NEAR(z.value(), 0.4, 1e-10);
    TRY(x = 0.5);    y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.75);      TRY(z = x / y);  TEST_NEAR(z.value(), 0.3333333333, 1e-10);
    TRY(x = 0.5);    y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.5);    y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.625);  y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.078125);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.234375);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.3125);    TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.390625);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.546875);  TRY(z = x / y);  TEST_NEAR(z.value(), 0.7142857143, 1e-10);
    TRY(x = 0.625);  y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.625);     TRY(z = x / y);  TEST_EQUAL(z.value(), 0.625);
    TRY(x = 0.625);  y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 0.78125);   TRY(z = x / y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.625);  y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.9375);    TRY(z = x / y);  TEST_NEAR(z.value(), 0.4166666667, 1e-10);
    TRY(x = 0.625);  y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.3125);
    TRY(x = 0.625);  y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.15625);
    TRY(x = 0.875);  y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.109375);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.328125);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.4375);    TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.546875);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.765625);  TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 0.875);     TRY(z = x / y);  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 0.875);  y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_NEAR(z.value(), 0.7, 1e-10);
    TRY(x = 0.875);  y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_NEAR(z.value(), 0.5833333333, 1e-10);
    TRY(x = 0.875);  y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.4375);
    TRY(x = 0.875);  y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.21875);
    TRY(x = 1);      y = 0.125;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.125);     TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.375;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.375);     TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 0.5);       TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.625;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.625);     TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.875;  TRY(z = x * y);  TEST_EQUAL(z.value(), 0.875);     TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1.25;   TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_NEAR(z.value(), 0.8, 1e-10);
    TRY(x = 1);      y = 1.5;    TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_NEAR(z.value(), 0.6666666667, 1e-10);
    TRY(x = 1);      y = 2;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 1);      y = 4;      TRY(z = x * y);  TEST_EQUAL(z.value(), 1);         TRY(z = x / y);  TEST_EQUAL(z.value(), 0.25);

}
