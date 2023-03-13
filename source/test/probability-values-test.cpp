#include "crow/probability.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

using PD = Probability<double>;
using PT = Detail::ProbabilityTest;

void test_crow_probability_construction() {

    PD p;

    TRY(p = -0.125);                     TEST_EQUAL(p.value(), 0);      TEST_EQUAL(PT::get_data(p), 0);       TEST_EQUAL(p.complement(), 1);
    TRY(p = 0);                          TEST_EQUAL(p.value(), 0);      TEST_EQUAL(PT::get_data(p), 0);       TEST_EQUAL(p.complement(), 1);
    TRY(p = 0.125);                      TEST_EQUAL(p.value(), 0.125);  TEST_EQUAL(PT::get_data(p), 0.125);   TEST_EQUAL(p.complement(), 0.875);
    TRY(p = 0.25);                       TEST_EQUAL(p.value(), 0.25);   TEST_EQUAL(PT::get_data(p), 0.25);    TEST_EQUAL(p.complement(), 0.75);
    TRY(p = 0.375);                      TEST_EQUAL(p.value(), 0.375);  TEST_EQUAL(PT::get_data(p), 0.375);   TEST_EQUAL(p.complement(), 0.625);
    TRY(p = 0.5);                        TEST_EQUAL(p.value(), 0.5);    TEST_EQUAL(PT::get_data(p), 0.5);     TEST_EQUAL(p.complement(), 0.5);
    TRY(p = 0.625);                      TEST_EQUAL(p.value(), 0.625);  TEST_EQUAL(PT::get_data(p), -0.375);  TEST_EQUAL(p.complement(), 0.375);
    TRY(p = 0.75);                       TEST_EQUAL(p.value(), 0.75);   TEST_EQUAL(PT::get_data(p), -0.25);   TEST_EQUAL(p.complement(), 0.25);
    TRY(p = 0.875);                      TEST_EQUAL(p.value(), 0.875);  TEST_EQUAL(PT::get_data(p), -0.125);  TEST_EQUAL(p.complement(), 0.125);
    TRY(p = 1);                          TEST_EQUAL(p.value(), 1);      TEST_EQUAL(PT::get_data(p), 1);       TEST_EQUAL(p.complement(), 0);
    TRY(p = 1.125);                      TEST_EQUAL(p.value(), 1);      TEST_EQUAL(PT::get_data(p), 1);       TEST_EQUAL(p.complement(), 0);
    TRY(p = PD::complementary(-0.125));  TEST_EQUAL(p.value(), 1);      TEST_EQUAL(PT::get_data(p), 1);       TEST_EQUAL(p.complement(), 0);
    TRY(p = PD::complementary(0));       TEST_EQUAL(p.value(), 1);      TEST_EQUAL(PT::get_data(p), 1);       TEST_EQUAL(p.complement(), 0);
    TRY(p = PD::complementary(0.125));   TEST_EQUAL(p.value(), 0.875);  TEST_EQUAL(PT::get_data(p), -0.125);  TEST_EQUAL(p.complement(), 0.125);
    TRY(p = PD::complementary(0.25));    TEST_EQUAL(p.value(), 0.75);   TEST_EQUAL(PT::get_data(p), -0.25);   TEST_EQUAL(p.complement(), 0.25);
    TRY(p = PD::complementary(0.375));   TEST_EQUAL(p.value(), 0.625);  TEST_EQUAL(PT::get_data(p), -0.375);  TEST_EQUAL(p.complement(), 0.375);
    TRY(p = PD::complementary(0.5));     TEST_EQUAL(p.value(), 0.5);    TEST_EQUAL(PT::get_data(p), 0.5);     TEST_EQUAL(p.complement(), 0.5);
    TRY(p = PD::complementary(0.625));   TEST_EQUAL(p.value(), 0.375);  TEST_EQUAL(PT::get_data(p), 0.375);   TEST_EQUAL(p.complement(), 0.625);
    TRY(p = PD::complementary(0.75));    TEST_EQUAL(p.value(), 0.25);   TEST_EQUAL(PT::get_data(p), 0.25);    TEST_EQUAL(p.complement(), 0.75);
    TRY(p = PD::complementary(0.875));   TEST_EQUAL(p.value(), 0.125);  TEST_EQUAL(PT::get_data(p), 0.125);   TEST_EQUAL(p.complement(), 0.875);
    TRY(p = PD::complementary(1));       TEST_EQUAL(p.value(), 0);      TEST_EQUAL(PT::get_data(p), 0);       TEST_EQUAL(p.complement(), 1);
    TRY(p = PD::complementary(1.125));   TEST_EQUAL(p.value(), 0);      TEST_EQUAL(PT::get_data(p), 0);       TEST_EQUAL(p.complement(), 1);

}

void test_crow_probability_formatting() {

    PD p;
    std::string s;

    TRY(p = 0);                TRY(s = p.str());      TEST_EQUAL(s, "0");
    TRY(p = 0.0000123456789);  TRY(s = p.str());      TEST_EQUAL(s, "0.0000123457");
    TRY(p = 0.000123456789);   TRY(s = p.str());      TEST_EQUAL(s, "0.000123457");
    TRY(p = 0.00123456789);    TRY(s = p.str());      TEST_EQUAL(s, "0.00123457");
    TRY(p = 0.0123456789);     TRY(s = p.str());      TEST_EQUAL(s, "0.0123457");
    TRY(p = 0.123456789);      TRY(s = p.str());      TEST_EQUAL(s, "0.123457");
    TRY(p = 0.25);             TRY(s = p.str());      TEST_EQUAL(s, "0.250000");
    TRY(p = 0.5);              TRY(s = p.str());      TEST_EQUAL(s, "0.500000");
    TRY(p = 0.75);             TRY(s = p.str());      TEST_EQUAL(s, "0.750000");
    TRY(p = 0.87654321);       TRY(s = p.str());      TEST_EQUAL(s, "0.876543");
    TRY(p = 0.987654321);      TRY(s = p.str());      TEST_EQUAL(s, "0.9876543");
    TRY(p = 0.9987654321);     TRY(s = p.str());      TEST_EQUAL(s, "0.99876543");
    TRY(p = 0.99987654321);    TRY(s = p.str());      TEST_EQUAL(s, "0.999876543");
    TRY(p = 0.999987654321);   TRY(s = p.str());      TEST_EQUAL(s, "0.9999876543");
    TRY(p = 1);                TRY(s = p.str());      TEST_EQUAL(s, "1");
    TRY(p = 0);                TRY(s = p.str("P"));   TEST_EQUAL(s, "0");
    TRY(p = 0.0000123456789);  TRY(s = p.str("P"));   TEST_EQUAL(s, "0.00123457");
    TRY(p = 0.000123456789);   TRY(s = p.str("P"));   TEST_EQUAL(s, "0.0123457");
    TRY(p = 0.00123456789);    TRY(s = p.str("P"));   TEST_EQUAL(s, "0.123457");
    TRY(p = 0.0123456789);     TRY(s = p.str("P"));   TEST_EQUAL(s, "1.23457");
    TRY(p = 0.123456789);      TRY(s = p.str("P"));   TEST_EQUAL(s, "12.3457");
    TRY(p = 0.25);             TRY(s = p.str("P"));   TEST_EQUAL(s, "25.0000");
    TRY(p = 0.5);              TRY(s = p.str("P"));   TEST_EQUAL(s, "50.0000");
    TRY(p = 0.75);             TRY(s = p.str("P"));   TEST_EQUAL(s, "75.0000");
    TRY(p = 0.87654321);       TRY(s = p.str("P"));   TEST_EQUAL(s, "87.6543");
    TRY(p = 0.987654321);      TRY(s = p.str("P"));   TEST_EQUAL(s, "98.76543");
    TRY(p = 0.9987654321);     TRY(s = p.str("P"));   TEST_EQUAL(s, "99.876543");
    TRY(p = 0.99987654321);    TRY(s = p.str("P"));   TEST_EQUAL(s, "99.9876543");
    TRY(p = 0.999987654321);   TRY(s = p.str("P"));   TEST_EQUAL(s, "99.99876543");
    TRY(p = 1);                TRY(s = p.str("P"));   TEST_EQUAL(s, "100");
    TRY(p = 0);                TRY(s = p.str("p1"));  TEST_EQUAL(s, "0");
    TRY(p = 0.0000123456789);  TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.00001");
    TRY(p = 0.000123456789);   TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.0001");
    TRY(p = 0.00123456789);    TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.001");
    TRY(p = 0.0123456789);     TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.01");
    TRY(p = 0.123456789);      TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.1");
    TRY(p = 0.2);              TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.2");
    TRY(p = 0.5);              TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.5");
    TRY(p = 0.8);              TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.8");
    TRY(p = 0.87654321);       TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.9");
    TRY(p = 0.987654321);      TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.99");
    TRY(p = 0.9987654321);     TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.999");
    TRY(p = 0.99987654321);    TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.9999");
    TRY(p = 0.999987654321);   TRY(s = p.str("p1"));  TEST_EQUAL(s, "0.99999");
    TRY(p = 1);                TRY(s = p.str("p1"));  TEST_EQUAL(s, "1");
    TRY(p = 0);                TRY(s = p.str("P1"));  TEST_EQUAL(s, "0");
    TRY(p = 0.0000123456789);  TRY(s = p.str("P1"));  TEST_EQUAL(s, "0.001");
    TRY(p = 0.000123456789);   TRY(s = p.str("P1"));  TEST_EQUAL(s, "0.01");
    TRY(p = 0.00123456789);    TRY(s = p.str("P1"));  TEST_EQUAL(s, "0.1");
    TRY(p = 0.0123456789);     TRY(s = p.str("P1"));  TEST_EQUAL(s, "1");
    TRY(p = 0.123456789);      TRY(s = p.str("P1"));  TEST_EQUAL(s, "10");
    TRY(p = 0.2);              TRY(s = p.str("P1"));  TEST_EQUAL(s, "20");
    TRY(p = 0.5);              TRY(s = p.str("P1"));  TEST_EQUAL(s, "50");
    TRY(p = 0.8);              TRY(s = p.str("P1"));  TEST_EQUAL(s, "80");
    TRY(p = 0.87654321);       TRY(s = p.str("P1"));  TEST_EQUAL(s, "90");
    TRY(p = 0.987654321);      TRY(s = p.str("P1"));  TEST_EQUAL(s, "99");
    TRY(p = 0.9987654321);     TRY(s = p.str("P1"));  TEST_EQUAL(s, "99.9");
    TRY(p = 0.99987654321);    TRY(s = p.str("P1"));  TEST_EQUAL(s, "99.99");
    TRY(p = 0.999987654321);   TRY(s = p.str("P1"));  TEST_EQUAL(s, "99.999");
    TRY(p = 1);                TRY(s = p.str("P1"));  TEST_EQUAL(s, "100");

}

void test_crow_probability_comparison() {

    PD p, q;
    double a = 0, b = 0;

    TRY(a = 0);      TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 0.125);  TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.375);  TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.5);    TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.625);  TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.875);  TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 1);      TRY(b = 0);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 0.375);  TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.5);    TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.625);  TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.875);  TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 1);      TRY(b = 0.125);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.375);  TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 0.5);    TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.625);  TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.875);  TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 1);      TRY(b = 0.375);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.375);  TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.5);    TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 0.625);  TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0.875);  TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 1);      TRY(b = 0.5);    TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.375);  TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.5);    TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.625);  TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 0.875);  TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 1);      TRY(b = 0.625);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.375);  TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.5);    TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.625);  TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.875);  TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);
    TRY(a = 1);      TRY(b = 0.875);  TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p > q);   TEST(p >= q);  TEST(p > b);   TEST(a > q);
    TRY(a = 0);      TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.125);  TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.375);  TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.5);    TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.625);  TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 0.875);  TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p != q);  TEST(p < q);   TEST(p <= q);  TEST(p < b);   TEST(a < q);
    TRY(a = 1);      TRY(b = 1);      TRY(p = a);  TRY(q = b);  TEST(p == q);  TEST(p <= q);  TEST(p >= q);  TEST(p == b);  TEST(a == q);

}
