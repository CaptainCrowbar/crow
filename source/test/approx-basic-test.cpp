#include "crow/approx.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_approx_construction() {

    ApproxD a;

    TEST_EQUAL(a.value(), 0);
    TEST_EQUAL(a.error(), 0);
    TEST_EQUAL(a.rel_error(), 0);

    TRY(a = 1.25);
    TEST_EQUAL(a.value(), 1.25);
    TEST_EQUAL(a.error(), 0);
    TEST_EQUAL(a.rel_error(), 0);

    TRY((a = {2, 0.25}));
    TEST_EQUAL(a.value(), 2);
    TEST_EQUAL(a.error(), 0.25);
    TEST_EQUAL(a.rel_error(), 0.125);

    TRY((a = {2, 0.25, Error::rel}));
    TEST_EQUAL(a.value(), 2);
    TEST_EQUAL(a.error(), 0.5);
    TEST_EQUAL(a.rel_error(), 0.25);

    TRY((a = {-2, 0.25}));
    TEST_EQUAL(a.value(), -2);
    TEST_EQUAL(a.error(), 0.25);
    TEST_EQUAL(a.rel_error(), 0.125);

    TRY((a = {-2, 0.25, Error::rel}));
    TEST_EQUAL(a.value(), -2);
    TEST_EQUAL(a.error(), 0.5);
    TEST_EQUAL(a.rel_error(), 0.25);

    TRY((a = {2, -0.25}));
    TEST_EQUAL(a.value(), 2);
    TEST_EQUAL(a.error(), 0.25);
    TEST_EQUAL(a.rel_error(), 0.125);

    TRY((a = {2, -0.25, Error::rel}));
    TEST_EQUAL(a.value(), 2);
    TEST_EQUAL(a.error(), 0.5);
    TEST_EQUAL(a.rel_error(), 0.25);

}

void test_crow_approx_parsing() {

    ApproxD a;

    TRY(a = ApproxD("0"));                       TEST_EQUAL(a.value(), 0);                  TEST_NEAR(a.error(), 0.5, 1e-10);
    TRY(a = ApproxD("00000"));                   TEST_EQUAL(a.value(), 0);                  TEST_NEAR(a.error(), 0.5, 1e-10);
    TRY(a = ApproxD("0.0"));                     TEST_EQUAL(a.value(), 0);                  TEST_NEAR(a.error(), 0.05, 1e-10);
    TRY(a = ApproxD("0.00"));                    TEST_EQUAL(a.value(), 0);                  TEST_NEAR(a.error(), 0.005, 1e-10);
    TRY(a = ApproxD("0.000"));                   TEST_EQUAL(a.value(), 0);                  TEST_NEAR(a.error(), 0.0005, 1e-10);

    TRY(a = ApproxD("123456"));                  TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 0.5, 1e-5);
    TRY(a = ApproxD("12345.6"));                 TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 0.05, 1e-6);
    TRY(a = ApproxD("1234.56"));                 TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.005, 1e-7);
    TRY(a = ApproxD("123.456"));                 TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.0005, 1e-8);
    TRY(a = ApproxD("12.3456"));                 TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.00005, 1e-9);
    TRY(a = ApproxD("1.23456"));                 TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.000005, 1e-10);
    TRY(a = ApproxD("0.123456"));                TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.0000005, 1e-11);
    TRY(a = ApproxD("1.23456e10"));              TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 5e4, 1);
    TRY(a = ApproxD("1.23456e-10"));             TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 5e-16, 1e-20);

    TRY(a = ApproxD("123456(78)"));              TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 78, 1e-5);
    TRY(a = ApproxD("12345.6(78)"));             TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 7.8, 1e-6);
    TRY(a = ApproxD("1234.56(78)"));             TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.78, 1e-7);
    TRY(a = ApproxD("123.456(78)"));             TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.078, 1e-8);
    TRY(a = ApproxD("12.3456(78)"));             TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.0078, 1e-9);
    TRY(a = ApproxD("1.23456(78)"));             TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.00078, 1e-10);
    TRY(a = ApproxD("0.123456(78)"));            TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.000078, 1e-11);
    TRY(a = ApproxD("1.23456(78)e10"));          TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 7.8e6, 1);
    TRY(a = ApproxD("1.23456(78)e-10"));         TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 7.8e-14, 1e-20);

    TRY(a = ApproxD("123456±78"));               TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 78, 1e-5);
    TRY(a = ApproxD("12345.6±7.8"));             TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 7.8, 1e-6);
    TRY(a = ApproxD("1234.56±0.78"));            TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.78, 1e-7);
    TRY(a = ApproxD("123.456±0.078"));           TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.078, 1e-8);
    TRY(a = ApproxD("12.3456±0.0078"));          TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.0078, 1e-9);
    TRY(a = ApproxD("1.23456±0.00078"));         TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.00078, 1e-10);
    TRY(a = ApproxD("0.123456±0.000078"));       TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.000078, 1e-11);
    TRY(a = ApproxD("1.23456e10+/-7.8e6"));      TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 7.8e6, 1);
    TRY(a = ApproxD("1.23456e-10+/-7.8e-14"));   TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 7.8e-14, 1e-20);

    TRY(a = ApproxD("123'456"));                 TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 0.5, 1e-5);
    TRY(a = ApproxD("12'345.6"));                TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 0.05, 1e-6);
    TRY(a = ApproxD("1'234.56"));                TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.005, 1e-7);
    TRY(a = ApproxD("1'23.456"));                TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.0005, 1e-8);
    TRY(a = ApproxD("12.345'6"));                TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.00005, 1e-9);
    TRY(a = ApproxD("1.234'56"));                TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.000005, 1e-10);
    TRY(a = ApproxD("0.123'456"));               TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.0000005, 1e-11);
    TRY(a = ApproxD("1.234'56e10"));             TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 5e4, 1);
    TRY(a = ApproxD("1.234'56e-10"));            TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 5e-16, 1e-20);

    TRY(a = ApproxD("123'456(78)"));             TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 78, 1e-5);
    TRY(a = ApproxD("12'345.6(78)"));            TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 7.8, 1e-6);
    TRY(a = ApproxD("1'234.56(78)"));            TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.78, 1e-7);
    TRY(a = ApproxD("123.456(7'8)"));            TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.078, 1e-8);
    TRY(a = ApproxD("12.345'6(78)"));            TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.0078, 1e-9);
    TRY(a = ApproxD("1.234'56(78)"));            TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.00078, 1e-10);
    TRY(a = ApproxD("0.123'456(78)"));           TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.000078, 1e-11);
    TRY(a = ApproxD("1.234'56(78)e10"));         TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 7.8e6, 1);
    TRY(a = ApproxD("1.234'56(78)e-10"));        TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 7.8e-14, 1e-20);

    TRY(a = ApproxD("123'456±78"));              TEST_NEAR(a.value(), 123456, 1e-5);        TEST_NEAR(a.error(), 78, 1e-5);
    TRY(a = ApproxD("12'345.6±7.8"));            TEST_NEAR(a.value(), 12345.6, 1e-6);       TEST_NEAR(a.error(), 7.8, 1e-6);
    TRY(a = ApproxD("1'234.56±0.78"));           TEST_NEAR(a.value(), 1234.56, 1e-7);       TEST_NEAR(a.error(), 0.78, 1e-7);
    TRY(a = ApproxD("1'23.456±0.07'8"));         TEST_NEAR(a.value(), 123.456, 1e-8);       TEST_NEAR(a.error(), 0.078, 1e-8);
    TRY(a = ApproxD("12.345'6±0.007'8"));        TEST_NEAR(a.value(), 12.3456, 1e-9);       TEST_NEAR(a.error(), 0.0078, 1e-9);
    TRY(a = ApproxD("1.234'56±0.000'78"));       TEST_NEAR(a.value(), 1.23456, 1e-10);      TEST_NEAR(a.error(), 0.00078, 1e-10);
    TRY(a = ApproxD("0.123'456±0.000'078"));     TEST_NEAR(a.value(), 0.123456, 1e-11);     TEST_NEAR(a.error(), 0.000078, 1e-11);
    TRY(a = ApproxD("1.234'56e10+/-7.8e6"));     TEST_NEAR(a.value(), 1.23456e10, 1);       TEST_NEAR(a.error(), 7.8e6, 1);
    TRY(a = ApproxD("1.234'56e-10+/-7.8e-14"));  TEST_NEAR(a.value(), 1.23456e-10, 1e-20);  TEST_NEAR(a.error(), 7.8e-14, 1e-20);

}
