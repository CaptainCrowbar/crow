#include "crow/mp-integer.hpp"
#include "crow/rational.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_mp_integer_rational_basics() {

    MPQ r;

    TEST_EQUAL(r.num(), 0);
    TEST_EQUAL(r.den(), 1);
    TEST(! bool(r));
    TEST(! r);
    TEST_EQUAL(int(r), 0);
    TEST_EQUAL(double(r), 0);
    TEST(r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), 0);
    TEST_EQUAL(r.frac().den(), 1);
    TEST_EQUAL(r.str(), "0");
    TEST_EQUAL(r.str("nv"), "0/1");

    TRY(r = MPQ(5, 3));
    TEST_EQUAL(r.num(), 5);
    TEST_EQUAL(r.den(), 3);
    TEST_EQUAL(int(r), 1);
    TEST_NEAR(double(r), 1.666667, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 1);
    TEST_EQUAL(r.frac().num(), 2);
    TEST_EQUAL(r.frac().den(), 3);
    TEST_EQUAL(r.str(), "1 2/3");
    TEST_EQUAL(r.str("nv"), "5/3");

    TRY(r = MPQ(-7, 9));
    TEST_EQUAL(r.num(), -7);
    TEST_EQUAL(r.den(), 9);
    TEST_EQUAL(int(r), -1);
    TEST_NEAR(double(r), -0.777778, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), -7);
    TEST_EQUAL(r.frac().den(), 9);
    TEST_EQUAL(r.str(), "-7/9");
    TEST_EQUAL(r.str("nv"), "-7/9");

}

void test_crow_mp_integer_rational_parsing() {

    MPQ r;

    TRY(r = MPQ::parse("0"));       TEST_EQUAL(r.num(), 0);   TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("5"));       TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("-5"));      TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("1/3"));     TEST_EQUAL(r.num(), 1);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("4/6"));     TEST_EQUAL(r.num(), 2);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-1/3"));    TEST_EQUAL(r.num(), -1);  TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-4/6"));    TEST_EQUAL(r.num(), -2);  TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("1 2/3"));   TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-1 2/3"));  TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 3);

    TEST_THROW(r = MPQ::parse(""), std::invalid_argument);
    TEST_THROW(r = MPQ::parse("1 2"), std::invalid_argument);
    TEST_THROW(r = MPQ::parse("1 -2/3"), std::invalid_argument);

}
