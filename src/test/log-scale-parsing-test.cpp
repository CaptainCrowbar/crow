#include "crow/log-scale.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>

using namespace Crow;

using LS = LogScale<double>;

void test_crow_log_scale_parsing() {

    LS x;

    TEST(! x.try_parse(""));          TEST_EQUAL(x.get(), 0);
    TEST(x.try_parse("0"));           TEST_EQUAL(x.get(), 0);
    TEST(x.try_parse("0.00123456"));  TEST_NEAR(x.get(), 0.00123456, 1e-12);
    TEST(x.try_parse("0.0123456"));   TEST_NEAR(x.get(), 0.0123456, 1e-11);
    TEST(x.try_parse("0.123456"));    TEST_NEAR(x.get(), 0.123456, 1e-10);
    TEST(x.try_parse("1.23456"));     TEST_NEAR(x.get(), 1.23456, 1e-9);
    TEST(x.try_parse("12.3456"));     TEST_NEAR(x.get(), 12.3456, 1e-8);
    TEST(x.try_parse("123.456"));     TEST_NEAR(x.get(), 123.456, 1e-7);
    TEST(x.try_parse("1234.56"));     TEST_NEAR(x.get(), 1234.56, 1e-6);
    TEST(x.try_parse("12345.6"));     TEST_NEAR(x.get(), 12345.6, 1e-5);
    TEST(x.try_parse("123456"));      TEST_NEAR(x.get(), 123456, 1e-4);
    TEST(x.try_parse("1234560"));     TEST_NEAR(x.get(), 1234560, 1e-3);
    TEST(x.try_parse("12345600"));    TEST_NEAR(x.get(), 12345600, 1e-2);
    TEST(x.try_parse("123456000"));   TEST_NEAR(x.get(), 123456000, 1e-1);
    TEST(x.try_parse("1.2345e-20"));  TEST_NEAR(x.get(), 1.2345e-20, 1e-30);
    TEST(x.try_parse("1.2345e-15"));  TEST_NEAR(x.get(), 1.2345e-15, 1e-25);
    TEST(x.try_parse("1.2345e-10"));  TEST_NEAR(x.get(), 1.2345e-10, 1e-20);
    TEST(x.try_parse("1.2345e-5"));   TEST_NEAR(x.get(), 1.2345e-5, 1e-15);
    TEST(x.try_parse("1.2345e0"));    TEST_NEAR(x.get(), 1.2345e0, 1e-10);
    TEST(x.try_parse("1.2345e5"));    TEST_NEAR(x.get(), 1.2345e5, 1e-5);
    TEST(x.try_parse("1.2345e10"));   TEST_NEAR(x.get(), 1.2345e10, 1);
    TEST(x.try_parse("1.2345e15"));   TEST_NEAR(x.get(), 1.2345e15, 1e5);
    TEST(x.try_parse("1.2345e20"));   TEST_NEAR(x.get(), 1.2345e20, 1e10);
    TEST(x.try_parse("+42"));         TEST_NEAR(x.get(), 42, 1e-10);
    TEST(x.try_parse("-42"));         TEST_NEAR(x.get(), -42, 1e-10);
    TEST(x.try_parse("000042"));      TEST_NEAR(x.get(), 42, 1e-10);
    TEST(x.try_parse("+000042"));     TEST_NEAR(x.get(), 42, 1e-10);
    TEST(x.try_parse("-000042"));     TEST_NEAR(x.get(), -42, 1e-10);
    TEST(x.try_parse("+8.6e99"));     TEST_NEAR(x.get(), 8.6e99, 1e89);
    TEST(x.try_parse("-8.6e99"));     TEST_NEAR(x.get(), -8.6e99, 1e89);
    TEST(x.try_parse("8.6e+99"));     TEST_NEAR(x.get(), 8.6e99, 1e89);
    TEST(x.try_parse("+8.6e+99"));    TEST_NEAR(x.get(), 8.6e99, 1e89);
    TEST(x.try_parse("-8.6e+99"));    TEST_NEAR(x.get(), -8.6e99, 1e89);
    TEST(x.try_parse("8.6e-99"));     TEST_NEAR(x.get(), 8.6e-99, 1e-109);
    TEST(x.try_parse("+8.6e-99"));    TEST_NEAR(x.get(), 8.6e-99, 1e-109);
    TEST(x.try_parse("-8.6e-99"));    TEST_NEAR(x.get(), -8.6e-99, 1e-109);
    TEST(x.try_parse("8.6e0099"));    TEST_NEAR(x.get(), 8.6e99, 1e89);
    TEST(x.try_parse("8.6e+0099"));   TEST_NEAR(x.get(), 8.6e99, 1e89);
    TEST(x.try_parse("8.6e-0099"));   TEST_NEAR(x.get(), 8.6e-99, 1e-109);

    TRY(x.parse("0"));           TEST_EQUAL(x.get(), 0);
    TRY(x.parse("0.00123456"));  TEST_NEAR(x.get(), 0.00123456, 1e-12);
    TRY(x.parse("123456000"));   TEST_NEAR(x.get(), 123456000, 1e-1);
    TRY(x.parse("1.2345e-20"));  TEST_NEAR(x.get(), 1.2345e-20, 1e-30);
    TRY(x.parse("1.2345e20"));   TEST_NEAR(x.get(), 1.2345e20, 1e10);
    TRY(x.parse("+42"));         TEST_NEAR(x.get(), 42, 1e-10);
    TRY(x.parse("-42"));         TEST_NEAR(x.get(), -42, 1e-10);
    TRY(x.parse("+8.6e+99"));    TEST_NEAR(x.get(), 8.6e99, 1e89);
    TRY(x.parse("-8.6e+99"));    TEST_NEAR(x.get(), -8.6e99, 1e89);

    TEST_THROW(x.parse(""), std::invalid_argument);
    TEST_THROW(x.parse("xyz"), std::invalid_argument);
    TEST_THROW(x.parse("123xyz"), std::invalid_argument);

}

void test_crow_log_scale_parsing_extreme_values() {

    LS x;
    std::string s;

    TEST(x.try_parse("1.23456e123"));        TRY(s = x.str());  TEST_EQUAL(s, "1.23456e123");
    TEST(x.try_parse("1.23456e1234"));       TRY(s = x.str());  TEST_EQUAL(s, "1.23456e1234");
    TEST(x.try_parse("1.23456e12345"));      TRY(s = x.str());  TEST_EQUAL(s, "1.23456e12345");
    TEST(x.try_parse("1.23456e123456"));     TRY(s = x.str());  TEST_EQUAL(s, "1.23456e123456");
    TEST(x.try_parse("1.23456e1234567"));    TRY(s = x.str());  TEST_EQUAL(s, "1.23456e1234567");
    TEST(x.try_parse("1.23456e12345678"));   TRY(s = x.str());  TEST_EQUAL(s, "1.23456e12345678");
    TEST(x.try_parse("1.23456e123456789"));  TRY(s = x.str());  TEST_EQUAL(s, "1.23456e123456789");

}
