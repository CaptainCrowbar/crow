#include "crow/log-scale.hpp"
#include "crow/unit-test.hpp"
#include <compare>

using namespace Crow;

using LS = LogScale<double>;

void test_crow_log_scale_arithmetic() {

    LS x, y, z;

    TRY(x = 1.2345);

    TRY(z = + x);      TEST_NEAR(z.get(), 1.2345, 1e-6);
    TRY(z = - x);      TEST_NEAR(z.get(), -1.2345, 1e-6);
    TRY(z = + (- x));  TEST_NEAR(z.get(), -1.2345, 1e-6);
    TRY(z = - (- x));  TEST_NEAR(z.get(), 1.2345, 1e-6);

    TRY(y = 678.9);

    TRY(z = x + y);          TEST_NEAR(z.get(), 680.1345, 1e-6);
    TRY(z = (- x) + y);      TEST_NEAR(z.get(), 677.6655, 1e-6);
    TRY(z = x + (- y));      TEST_NEAR(z.get(), -677.6655, 1e-6);
    TRY(z = (- x) + (- y));  TEST_NEAR(z.get(), -680.1345, 1e-6);
    TRY(z = y + x);          TEST_NEAR(z.get(), 680.1345, 1e-6);
    TRY(z = (- y) + x);      TEST_NEAR(z.get(), -677.6655, 1e-6);
    TRY(z = y + (- x));      TEST_NEAR(z.get(), 677.6655, 1e-6);
    TRY(z = (- y) + (- x));  TEST_NEAR(z.get(), -680.1345, 1e-6);
    TRY(z = x - y);          TEST_NEAR(z.get(), -677.6655, 1e-6);
    TRY(z = (- x) - y);      TEST_NEAR(z.get(), -680.1345, 1e-6);
    TRY(z = x - (- y));      TEST_NEAR(z.get(), 680.1345, 1e-6);
    TRY(z = (- x) - (- y));  TEST_NEAR(z.get(), 677.6655, 1e-6);
    TRY(z = y - x);          TEST_NEAR(z.get(), 677.6655, 1e-6);
    TRY(z = (- y) - x);      TEST_NEAR(z.get(), -680.1345, 1e-6);
    TRY(z = y - (- x));      TEST_NEAR(z.get(), 680.1345, 1e-6);
    TRY(z = (- y) - (- x));  TEST_NEAR(z.get(), -677.6655, 1e-6);
    TRY(z = x * y);          TEST_NEAR(z.get(), 838.10205, 1e-6);
    TRY(z = (- x) * y);      TEST_NEAR(z.get(), -838.10205, 1e-6);
    TRY(z = x * (- y));      TEST_NEAR(z.get(), -838.10205, 1e-6);
    TRY(z = (- x) * (- y));  TEST_NEAR(z.get(), 838.10205, 1e-6);
    TRY(z = y * x);          TEST_NEAR(z.get(), 838.10205, 1e-6);
    TRY(z = (- y) * x);      TEST_NEAR(z.get(), -838.10205, 1e-6);
    TRY(z = y * (- x));      TEST_NEAR(z.get(), -838.10205, 1e-6);
    TRY(z = (- y) * (- x));  TEST_NEAR(z.get(), 838.10205, 1e-6);
    TRY(z = x / y);          TEST_NEAR(z.get(), 0.001818, 1e-6);
    TRY(z = (- x) / y);      TEST_NEAR(z.get(), -0.001818, 1e-6);
    TRY(z = x / (- y));      TEST_NEAR(z.get(), -0.001818, 1e-6);
    TRY(z = (- x) / (- y));  TEST_NEAR(z.get(), 0.001818, 1e-6);
    TRY(z = y / x);          TEST_NEAR(z.get(), 549.939247, 1e-6);
    TRY(z = (- y) / x);      TEST_NEAR(z.get(), -549.939247, 1e-6);
    TRY(z = y / (- x));      TEST_NEAR(z.get(), -549.939247, 1e-6);
    TRY(z = (- y) / (- x));  TEST_NEAR(z.get(), 549.939247, 1e-6);

    TRY(x = 1.23);
    TRY(y = 4.56e78);

    TRY(z = x + y);          TEST_EQUAL(z, y);
    TRY(z = (- x) + y);      TEST_EQUAL(z, y);
    TRY(z = x + (- y));      TEST_EQUAL(z, - y);
    TRY(z = (- x) + (- y));  TEST_EQUAL(z, - y);
    TRY(z = y + x);          TEST_EQUAL(z, y);
    TRY(z = (- y) + x);      TEST_EQUAL(z, - y);
    TRY(z = y + (- x));      TEST_EQUAL(z, y);
    TRY(z = (- y) + (- x));  TEST_EQUAL(z, - y);
    TRY(z = x - y);          TEST_EQUAL(z, - y);
    TRY(z = (- x) - y);      TEST_EQUAL(z, - y);
    TRY(z = x - (- y));      TEST_EQUAL(z, y);
    TRY(z = (- x) - (- y));  TEST_EQUAL(z, y);
    TRY(z = y - x);          TEST_EQUAL(z, y);
    TRY(z = (- y) - x);      TEST_EQUAL(z, - y);
    TRY(z = y - (- x));      TEST_EQUAL(z, y);
    TRY(z = (- y) - (- x));  TEST_EQUAL(z, - y);
    TRY(z = x * y);          TEST_NEAR(z.get(), 5.60880e78, 1e73);
    TRY(z = (- x) * y);      TEST_NEAR(z.get(), -5.60880e78, 1e73);
    TRY(z = x * (- y));      TEST_NEAR(z.get(), -5.60880e78, 1e73);
    TRY(z = (- x) * (- y));  TEST_NEAR(z.get(), 5.60880e78, 1e73);
    TRY(z = y * x);          TEST_NEAR(z.get(), 5.60880e78, 1e73);
    TRY(z = (- y) * x);      TEST_NEAR(z.get(), -5.60880e78, 1e73);
    TRY(z = y * (- x));      TEST_NEAR(z.get(), -5.60880e78, 1e73);
    TRY(z = (- y) * (- x));  TEST_NEAR(z.get(), 5.60880e78, 1e73);
    TRY(z = x / y);          TEST_NEAR(z.get(), 2.69737e-79, 1e-84);
    TRY(z = (- x) / y);      TEST_NEAR(z.get(), -2.69737e-79, 1e-84);
    TRY(z = x / (- y));      TEST_NEAR(z.get(), -2.69737e-79, 1e-84);
    TRY(z = (- x) / (- y));  TEST_NEAR(z.get(), 2.69737e-79, 1e-84);
    TRY(z = y / x);          TEST_NEAR(z.get(), 3.70732e78, 1e73);
    TRY(z = (- y) / x);      TEST_NEAR(z.get(), -3.70732e78, 1e73);
    TRY(z = y / (- x));      TEST_NEAR(z.get(), -3.70732e78, 1e73);
    TRY(z = (- y) / (- x));  TEST_NEAR(z.get(), 3.70732e78, 1e73);

}

void test_crow_log_scale_comparison() {

    LS x, y;

    TRY(x = 1.2345);   TRY(y = 0);       TEST(x > y);   TEST(y < x);   TEST(x.compare(y) == std::strong_ordering::greater);
    TRY(x = -1.2345);  TRY(y = 0);       TEST(x < y);   TEST(y > x);   TEST(x.compare(y) == std::strong_ordering::less);
    TRY(x = 1.2345);   TRY(y = x);       TEST(x == y);  TEST(y == x);  TEST(x.compare(y) == std::strong_ordering::equal);
    TRY(x = -1.2345);  TRY(y = x);       TEST(x == y);  TEST(y == x);  TEST(x.compare(y) == std::strong_ordering::equal);
    TRY(x = 1.2345);   TRY(y = - x);     TEST(x > y);   TEST(y < x);   TEST(x.compare(y) == std::strong_ordering::greater);
    TRY(x = -1.2345);  TRY(y = - x);     TEST(x < y);   TEST(y > x);   TEST(x.compare(y) == std::strong_ordering::less);
    TRY(x = 1.2345);   TRY(y = 6.789);   TEST(x < y);   TEST(y > x);   TEST(x.compare(y) == std::strong_ordering::less);
    TRY(x = 1.2345);   TRY(y = -6.789);  TEST(x > y);   TEST(y < x);   TEST(x.compare(y) == std::strong_ordering::greater);
    TRY(x = -1.2345);  TRY(y = 6.789);   TEST(x < y);   TEST(y > x);   TEST(x.compare(y) == std::strong_ordering::less);
    TRY(x = -1.2345);  TRY(y = -6.789);  TEST(x > y);   TEST(y < x);   TEST(x.compare(y) == std::strong_ordering::greater);

}

void test_crow_log_scale_elementary_functions() {

    LS x, y, z;
    int n = 0;

    TRY(x = 0);         TRY(y = abs(x));      TEST_EQUAL(y.get(), 0);
    TRY(x = 123.456);   TRY(y = abs(x));      TEST_NEAR(y.get(), 123.456, 1e-6);
    TRY(x = -123.456);  TRY(y = abs(x));      TEST_NEAR(y.get(), 123.456, 1e-6);
    TRY(x = 0);         TRY(y = exp(x));      TEST_EQUAL(y.get(), 1);
    TRY(x = 123.456);   TRY(y = exp(x));      TEST_NEAR(y.get(), 4.13294e53, 1e48);
    TRY(x = -123.456);  TRY(y = exp(x));      TEST_NEAR(y.get(), 2.41958e-54, 1e-59);
    TRY(x = 123.456);   TRY(y = log(x));      TEST_NEAR(y.get(), 4.815885, 1e-6);
    TRY(x = 0.123456);  TRY(y = log(x));      TEST_NEAR(y.get(), -2.091870, 1e-6);
    TRY(x = 123.456);   TRY(y = log2(x));     TEST_NEAR(y.get(), 6.947853, 1e-6);
    TRY(x = 0.123456);  TRY(y = log2(x));     TEST_NEAR(y.get(), -3.017931, 1e-6);
    TRY(x = 123.456);   TRY(y = log10(x));    TEST_NEAR(y.get(), 2.091512, 1e-6);
    TRY(x = 0.123456);  TRY(y = log10(x));    TEST_NEAR(y.get(), -0.908488, 1e-6);
    TRY(x = 0);         TRY(n = sign_of(x));  TEST_EQUAL(n, 0);
    TRY(x = 123.456);   TRY(n = sign_of(x));  TEST_EQUAL(n, 1);
    TRY(x = -123.456);  TRY(n = sign_of(x));  TEST_EQUAL(n, -1);

    TRY(x = 0);        TRY(y = modf(x, &z));  TEST_EQUAL(y.get(), 0);             TEST_EQUAL(z.get(), 0);
    TRY(x = 0.0123);   TRY(y = modf(x, &z));  TEST_NEAR(y.get(), 0.0123, 1e-6);   TEST_EQUAL(z.get(), 0);
    TRY(x = 0.123);    TRY(y = modf(x, &z));  TEST_NEAR(y.get(), 0.123, 1e-6);    TEST_EQUAL(z.get(), 0);
    TRY(x = 1.23);     TRY(y = modf(x, &z));  TEST_NEAR(y.get(), 0.23, 1e-6);     TEST_EQUAL(z.get(), 1);
    TRY(x = 12.3);     TRY(y = modf(x, &z));  TEST_NEAR(y.get(), 0.3, 1e-6);      TEST_EQUAL(z.get(), 12);
    TRY(x = -0.0123);  TRY(y = modf(x, &z));  TEST_NEAR(y.get(), -0.0123, 1e-6);  TEST_EQUAL(z.get(), -0);
    TRY(x = -0.123);   TRY(y = modf(x, &z));  TEST_NEAR(y.get(), -0.123, 1e-6);   TEST_EQUAL(z.get(), -0);
    TRY(x = -1.23);    TRY(y = modf(x, &z));  TEST_NEAR(y.get(), -0.23, 1e-6);    TEST_EQUAL(z.get(), -1);
    TRY(x = -12.3);    TRY(y = modf(x, &z));  TEST_NEAR(y.get(), -0.3, 1e-6);     TEST_EQUAL(z.get(), -12);

    TRY(x = LS(1000, 1));    TRY(y = modf(x, &z));  TEST_EQUAL(y, 0);            TEST_EQUAL(z.log(), 1000);  TEST_EQUAL(z.sign(), 1);
    TRY(x = LS(-1000, 1));   TRY(y = modf(x, &z));  TEST_EQUAL(y.log(), -1000);  TEST_EQUAL(y.sign(), 1);    TEST_EQUAL(z, 0);
    TRY(x = LS(1000, -1));   TRY(y = modf(x, &z));  TEST_EQUAL(y, 0);            TEST_EQUAL(z.log(), 1000);  TEST_EQUAL(z.sign(), -1);
    TRY(x = LS(-1000, -1));  TRY(y = modf(x, &z));  TEST_EQUAL(y.log(), -1000);  TEST_EQUAL(y.sign(), -1);   TEST_EQUAL(z, 0);

    TRY(x = 0);         TRY(y = 0.001);     TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 0);
    TRY(x = 0);         TRY(y = 1);         TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 0);
    TRY(x = 0);         TRY(y = 12345.6);   TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 0);
    TRY(x = 1);         TRY(y = 0);         TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = 0.001);     TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = 1);         TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = 12345.6);   TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = -0.001);    TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = -1);        TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 1);         TRY(y = -12345.6);  TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 123.456);   TRY(y = 0);         TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 123.456);   TRY(y = 0.789);     TRY(z = pow(x, y));  TEST_NEAR(z.get(), 44.689256, 1e-6);
    TRY(x = 123.456);   TRY(y = 1);         TRY(z = pow(x, y));  TEST_NEAR(z.get(), 123.456, 1e-6);
    TRY(x = 123.456);   TRY(y = 7.89);      TRY(z = pow(x, y));  TEST_NEAR(z.get(), 3.17710e16, 1e11);
    TRY(x = 123.456);   TRY(y = -0.789);    TRY(z = pow(x, y));  TEST_NEAR(z.get(), 0.022377, 1e-6);
    TRY(x = 123.456);   TRY(y = -1);        TRY(z = pow(x, y));  TEST_NEAR(z.get(), 0.008100, 1e-6);
    TRY(x = 123.456);   TRY(y = -7.89);     TRY(z = pow(x, y));  TEST_NEAR(z.get(), 3.14752e-17, 1e-22);
    TRY(x = 0.123456);  TRY(y = 0);         TRY(z = pow(x, y));  TEST_EQUAL(z.get(), 1);
    TRY(x = 0.123456);  TRY(y = 0.789);     TRY(z = pow(x, y));  TEST_NEAR(z.get(), 0.191957, 1e-6);
    TRY(x = 0.123456);  TRY(y = 1);         TRY(z = pow(x, y));  TEST_NEAR(z.get(), 0.123456, 1e-6);
    TRY(x = 0.123456);  TRY(y = 7.89);      TRY(z = pow(x, y));  TEST_NEAR(z.get(), 6.79253e-8, 1e-13);
    TRY(x = 0.123456);  TRY(y = -0.789);    TRY(z = pow(x, y));  TEST_NEAR(z.get(), 5.209510, 1e-6);
    TRY(x = 0.123456);  TRY(y = -1);        TRY(z = pow(x, y));  TEST_NEAR(z.get(), 8.100052, 1e-6);
    TRY(x = 0.123456);  TRY(y = -7.89);     TRY(z = pow(x, y));  TEST_NEAR(z.get(), 14722065, 1);

}
