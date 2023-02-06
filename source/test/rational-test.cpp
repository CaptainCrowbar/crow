#include "crow/rational.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace Crow;

void test_crow_rational_basics() {

    Rational r;

    TEST(NonIntegralNumericType<Rational>);

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

    TRY(r = Rational(5, 3));
    TEST_EQUAL(r.num(), 5);
    TEST_EQUAL(r.den(), 3);
    TEST_EQUAL(int(r), 1);
    TEST_NEAR(double(r), 1.666667, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 1);
    TEST_EQUAL(r.frac().num(), 2);
    TEST_EQUAL(r.frac().den(), 3);

    TRY(r = Rational(-7, 9));
    TEST_EQUAL(r.num(), -7);
    TEST_EQUAL(r.den(), 9);
    TEST_EQUAL(int(r), -1);
    TEST_NEAR(double(r), -0.777778, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), -7);
    TEST_EQUAL(r.frac().den(), 9);

}

void test_crow_rational_reduction() {

    Rational r;

    TRY(r = Rational(0, 6));    TEST_EQUAL(r.num(), 0);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(0, 6));
    TRY(r = Rational(1, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(1, 6));
    TRY(r = Rational(2, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(2, 6));
    TRY(r = Rational(3, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(3, 6));
    TRY(r = Rational(4, 6));    TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(4, 6));
    TRY(r = Rational(5, 6));    TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Rational(5, 6));
    TRY(r = Rational(6, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(0, 6));
    TRY(r = Rational(7, 6));    TEST_EQUAL(r.num(), 7);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(1, 6));
    TRY(r = Rational(8, 6));    TEST_EQUAL(r.num(), 4);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(2, 6));
    TRY(r = Rational(9, 6));    TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(3, 6));
    TRY(r = Rational(10, 6));   TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(4, 6));
    TRY(r = Rational(11, 6));   TEST_EQUAL(r.num(), 11);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Rational(5, 6));
    TRY(r = Rational(12, 6));   TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 2);   TEST_EQUAL(r.frac(), Rational(0, 6));
    TRY(r = Rational(-1, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Rational(-1, 6));
    TRY(r = Rational(-2, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Rational(-2, 6));
    TRY(r = Rational(-3, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Rational(-3, 6));
    TRY(r = Rational(-4, 6));   TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Rational(-4, 6));
    TRY(r = Rational(-5, 6));   TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Rational(-5, 6));
    TRY(r = Rational(-6, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-0, 6));
    TRY(r = Rational(-7, 6));   TEST_EQUAL(r.num(), -7);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-1, 6));
    TRY(r = Rational(-8, 6));   TEST_EQUAL(r.num(), -4);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-2, 6));
    TRY(r = Rational(-9, 6));   TEST_EQUAL(r.num(), -3);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-3, 6));
    TRY(r = Rational(-10, 6));  TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-4, 6));
    TRY(r = Rational(-11, 6));  TEST_EQUAL(r.num(), -11);  TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Rational(-5, 6));
    TRY(r = Rational(-12, 6));  TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -2);  TEST_EQUAL(r.frac(), Rational(-0, 6));

}

void test_crow_rational_arithmetic() {

    Rational r, s, t;

    TRY(r = Rational(5, 3));
    TRY(s = Rational(7, 9));

    TRY(t = + r);             TEST_EQUAL(t.num(), 5);    TEST_EQUAL(t.den(), 3);
    TRY(t = - r);             TEST_EQUAL(t.num(), -5);   TEST_EQUAL(t.den(), 3);
    TRY(t = r + s);           TEST_EQUAL(t.num(), 22);   TEST_EQUAL(t.den(), 9);
    TRY(t = r - s);           TEST_EQUAL(t.num(), 8);    TEST_EQUAL(t.den(), 9);
    TRY(t = r * s);           TEST_EQUAL(t.num(), 35);   TEST_EQUAL(t.den(), 27);
    TRY(t = r / s);           TEST_EQUAL(t.num(), 15);   TEST_EQUAL(t.den(), 7);
    TRY(t = s + 6);           TEST_EQUAL(t.num(), 61);   TEST_EQUAL(t.den(), 9);
    TRY(t = 6 + s);           TEST_EQUAL(t.num(), 61);   TEST_EQUAL(t.den(), 9);
    TRY(t = s - 6);           TEST_EQUAL(t.num(), -47);  TEST_EQUAL(t.den(), 9);
    TRY(t = 6 - s);           TEST_EQUAL(t.num(), 47);   TEST_EQUAL(t.den(), 9);
    TRY(t = s * 6);           TEST_EQUAL(t.num(), 14);   TEST_EQUAL(t.den(), 3);
    TRY(t = 6 * s);           TEST_EQUAL(t.num(), 14);   TEST_EQUAL(t.den(), 3);
    TRY(t = s / 6);           TEST_EQUAL(t.num(), 7);    TEST_EQUAL(t.den(), 54);
    TRY(t = 6 / s);           TEST_EQUAL(t.num(), 54);   TEST_EQUAL(t.den(), 7);
    TRY(t = r.reciprocal());  TEST_EQUAL(t.num(), 3);    TEST_EQUAL(t.den(), 5);

}

void test_crow_rational_properties() {

    Rational r, s, t;

    TRY(r = Rational(5, 3));
    TRY(s = Rational(-7, 9));

    TEST_EQUAL(r.abs(), Rational(5, 3));
    TEST_EQUAL(s.abs(), Rational(7, 9));
    TEST_EQUAL(t.abs(), Rational(0));

    TRY(r = Rational(-6, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = Rational(-5, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = Rational(-4, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-3, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-2, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-1, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(0, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(1, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(2, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(3, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(4, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(5, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Rational(6, 3));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Rational(-8, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = Rational(-7, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = Rational(-6, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-5, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-4, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-3, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = Rational(-2, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(-1, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(0, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(1, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = Rational(2, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(3, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(4, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(5, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = Rational(6, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Rational(7, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Rational(8, 4));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);

}

void test_crow_rational_comparison() {

    Rational r, s;

    TRY(r = Rational(5, 6));   TRY(s = Rational(7, 9));     TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = Rational(5, 6));   TRY(s = Rational(8, 9));     TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = Rational(5, 6));   TRY(s = Rational(10, 12));   TEST(r == s);  TEST(r <= s);  TEST(r >= s);
    TRY(r = Rational(-5, 6));  TRY(s = Rational(-7, 9));    TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = Rational(-5, 6));  TRY(s = Rational(-8, 9));    TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = Rational(-5, 6));  TRY(s = Rational(-10, 12));  TEST(r == s);  TEST(r <= s);  TEST(r >= s);

}

void test_crow_rational_mixed() {

    Rational r;

    TRY(r = 42);    TEST_EQUAL(r.num(), 42);   TEST_EQUAL(r.den(), 1);
    TRY(r += 100);  TEST_EQUAL(r.num(), 142);  TEST_EQUAL(r.den(), 1);
    TRY(r *= 2);    TEST_EQUAL(r.num(), 284);  TEST_EQUAL(r.den(), 1);

    TRY(r = 42);
    TEST(r == 42);
    TEST(r <= 42);
    TEST(r >= 42);
    TEST(r != 100);
    TEST(r < 100);
    TEST(r <= 100);

}

void test_crow_rational_formatting() {

    Rational r;

    TEST_EQUAL(r.str(), "0");
    TEST_EQUAL(r.str("nv"), "0/1");
    TEST_EQUAL(r.str("f4"), "0.0000");
    TRY(r = Rational(5, 3));
    TEST_EQUAL(r.str(), "1 2/3");
    TEST_EQUAL(r.str("nv"), "5/3");
    TEST_EQUAL(r.str("f4"), "1.6667");
    TRY(r = Rational(-7, 9));
    TEST_EQUAL(r.str(), "-7/9");
    TEST_EQUAL(r.str("nv"), "-7/9");
    TEST_EQUAL(r.str("f4"), "-0.7778");

    std::vector<Rational> v;

    TEST_EQUAL(format_object(v), "[]");
    TEST_EQUAL(format_object(v, "nv"), "[]");
    TEST_EQUAL(format_object(v, "f4"), "[]");
    TEST_EQUAL(format_object(v, "*N"), "");
    TRY(v.emplace_back(5, 3));
    TEST_EQUAL(format_object(v), "[1 2/3]");
    TEST_EQUAL(format_object(v, "nv"), "[5/3]");
    TEST_EQUAL(format_object(v, "f4"), "[1.6667]");
    TEST_EQUAL(format_object(v, "*N"), "1 2/3");
    TRY(v.emplace_back(-7, 9));
    TEST_EQUAL(format_object(v), "[1 2/3,-7/9]");
    TEST_EQUAL(format_object(v, "nv"), "[5/3,-7/9]");
    TEST_EQUAL(format_object(v, "f4"), "[1.6667,-0.7778]");
    TEST_EQUAL(format_object(v, "*N"), "1 2/3,-7/9");
    TRY(v.emplace_back(1, 10));
    TEST_EQUAL(format_object(v), "[1 2/3,-7/9,1/10]");
    TEST_EQUAL(format_object(v, "nv"), "[5/3,-7/9,1/10]");
    TEST_EQUAL(format_object(v, "f4"), "[1.6667,-0.7778,0.1000]");
    TEST_EQUAL(format_object(v, "*N"), "1 2/3,-7/9,1/10");

}

void test_crow_rational_parsing() {

    Rational r;

    TRY(r = Rational::parse("0"));       TEST_EQUAL(r.num(), 0);   TEST_EQUAL(r.den(), 1);
    TRY(r = Rational::parse("5"));       TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 1);
    TRY(r = Rational::parse("-5"));      TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 1);
    TRY(r = Rational::parse("1/3"));     TEST_EQUAL(r.num(), 1);   TEST_EQUAL(r.den(), 3);
    TRY(r = Rational::parse("4/6"));     TEST_EQUAL(r.num(), 2);   TEST_EQUAL(r.den(), 3);
    TRY(r = Rational::parse("-1/3"));    TEST_EQUAL(r.num(), -1);  TEST_EQUAL(r.den(), 3);
    TRY(r = Rational::parse("-4/6"));    TEST_EQUAL(r.num(), -2);  TEST_EQUAL(r.den(), 3);
    TRY(r = Rational::parse("1 2/3"));   TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 3);
    TRY(r = Rational::parse("-1 2/3"));  TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 3);

    TEST_THROW(r = Rational::parse(""), std::invalid_argument);
    TEST_THROW(r = Rational::parse("1 2"), std::invalid_argument);
    TEST_THROW(r = Rational::parse("1 -2/3"), std::invalid_argument);

}

void test_crow_rational_hash() {

    std::unordered_set<Rational> ratset;

    TEST(ratset.empty());

    for (int i = 1; i <= 10; ++i)
        TRY(ratset.insert(Rational(i)));

    TEST_EQUAL(ratset.size(), 10u);

}
