#include "crow/log-scale.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

using LS = LogScale<double>;

void test_crow_log_scale_format_default() {

    LS x;
    std::string s;

    TRY(x = 1.234e-6);    TRY(s = x.str());  TEST_EQUAL(s, "1.234e-6");
    TRY(x = 1.234e6);     TRY(s = x.str());  TEST_EQUAL(s, "1.234e6");
    TRY(x = 0);           TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = -123.456);    TRY(s = x.str());  TEST_EQUAL(s, "-123.456");

}

void test_crow_log_scale_format_digits() {

    LS x;
    std::string s;

    TRY(x = 1.234e-6);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.000001234");
    TRY(x = 1.234e-5);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.00001234");
    TRY(x = 1.234e-4);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.0001234");
    TRY(x = 1.234e-3);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.001234");
    TRY(x = 1.234e-2);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.01234");
    TRY(x = 1.234e-1);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.1234");
    TRY(x = 1.234e0);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "1.234");
    TRY(x = 1.234e1);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "12.34");
    TRY(x = 1.234e2);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "123.4");
    TRY(x = 1.234e3);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "1234");
    TRY(x = 1.234e4);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "12340");
    TRY(x = 1.234e5);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "123400");
    TRY(x = 1.234e6);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "1234000");
    TRY(x = 0);           TRY(s = x.str("d0"));  TEST_EQUAL(s, "0");
    TRY(x = 0);           TRY(s = x.str("d1"));  TEST_EQUAL(s, "0");
    TRY(x = 0);           TRY(s = x.str("d2"));  TEST_EQUAL(s, "0.0");
    TRY(x = 0);           TRY(s = x.str("d3"));  TEST_EQUAL(s, "0.00");
    TRY(x = 0);           TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.000");
    TRY(x = 42);          TRY(s = x.str("d0"));  TEST_EQUAL(s, "40");
    TRY(x = 42);          TRY(s = x.str("d1"));  TEST_EQUAL(s, "40");
    TRY(x = 42);          TRY(s = x.str("d2"));  TEST_EQUAL(s, "42");
    TRY(x = 42);          TRY(s = x.str("d3"));  TEST_EQUAL(s, "42.0");
    TRY(x = 42);          TRY(s = x.str("d4"));  TEST_EQUAL(s, "42.00");
    TRY(x = 123.456);     TRY(s = x.str("d0"));  TEST_EQUAL(s, "100");
    TRY(x = 123.456);     TRY(s = x.str("d1"));  TEST_EQUAL(s, "100");
    TRY(x = 123.456);     TRY(s = x.str("d2"));  TEST_EQUAL(s, "120");
    TRY(x = 123.456);     TRY(s = x.str("d3"));  TEST_EQUAL(s, "123");
    TRY(x = 123.456);     TRY(s = x.str("d4"));  TEST_EQUAL(s, "123.5");
    TRY(x = 0.123456);    TRY(s = x.str("d0"));  TEST_EQUAL(s, "0.1");
    TRY(x = 0.123456);    TRY(s = x.str("d1"));  TEST_EQUAL(s, "0.1");
    TRY(x = 0.123456);    TRY(s = x.str("d2"));  TEST_EQUAL(s, "0.12");
    TRY(x = 0.123456);    TRY(s = x.str("d3"));  TEST_EQUAL(s, "0.123");
    TRY(x = 0.123456);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.1235");
    TRY(x = 1.23456e6);   TRY(s = x.str("d0"));  TEST_EQUAL(s, "1000000");
    TRY(x = 1.23456e6);   TRY(s = x.str("d1"));  TEST_EQUAL(s, "1000000");
    TRY(x = 1.23456e6);   TRY(s = x.str("d2"));  TEST_EQUAL(s, "1200000");
    TRY(x = 1.23456e6);   TRY(s = x.str("d3"));  TEST_EQUAL(s, "1230000");
    TRY(x = 1.23456e6);   TRY(s = x.str("d4"));  TEST_EQUAL(s, "1235000");
    TRY(x = 1.23456e-6);  TRY(s = x.str("d0"));  TEST_EQUAL(s, "0.000001");
    TRY(x = 1.23456e-6);  TRY(s = x.str("d1"));  TEST_EQUAL(s, "0.000001");
    TRY(x = 1.23456e-6);  TRY(s = x.str("d2"));  TEST_EQUAL(s, "0.0000012");
    TRY(x = 1.23456e-6);  TRY(s = x.str("d3"));  TEST_EQUAL(s, "0.00000123");
    TRY(x = 1.23456e-6);  TRY(s = x.str("d4"));  TEST_EQUAL(s, "0.000001235");
    TRY(x = -123.456);    TRY(s = x.str("d0"));  TEST_EQUAL(s, "-100");
    TRY(x = -123.456);    TRY(s = x.str("d1"));  TEST_EQUAL(s, "-100");
    TRY(x = -123.456);    TRY(s = x.str("d2"));  TEST_EQUAL(s, "-120");
    TRY(x = -123.456);    TRY(s = x.str("d3"));  TEST_EQUAL(s, "-123");
    TRY(x = -123.456);    TRY(s = x.str("d4"));  TEST_EQUAL(s, "-123.5");

    TRY(x = 1.234e-6);    TRY(s = x.str("D4"));  TEST_EQUAL(s, "0.000001234");
    TRY(x = 1.234e6);     TRY(s = x.str("D4"));  TEST_EQUAL(s, "1234000");
    TRY(x = 0);           TRY(s = x.str("D4"));  TEST_EQUAL(s, "0.000");
    TRY(x = -123.456);    TRY(s = x.str("D4"));  TEST_EQUAL(s, "-123.5");

}

void test_crow_log_scale_format_exponential() {

    LS x;
    std::string s;

    TRY(x = 1.234e-6);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-6");
    TRY(x = 1.234e-5);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-5");
    TRY(x = 1.234e-4);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-4");
    TRY(x = 1.234e-3);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-3");
    TRY(x = 1.234e-2);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-2");
    TRY(x = 1.234e-1);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e-1");
    TRY(x = 1.234e0);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e0");
    TRY(x = 1.234e1);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e1");
    TRY(x = 1.234e2);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e2");
    TRY(x = 1.234e3);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e3");
    TRY(x = 1.234e4);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e4");
    TRY(x = 1.234e5);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e5");
    TRY(x = 1.234e6);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.234e6");
    TRY(x = 0);           TRY(s = x.str("e0"));  TEST_EQUAL(s, "0e0");
    TRY(x = 0);           TRY(s = x.str("e1"));  TEST_EQUAL(s, "0e0");
    TRY(x = 0);           TRY(s = x.str("e2"));  TEST_EQUAL(s, "0.0e0");
    TRY(x = 0);           TRY(s = x.str("e3"));  TEST_EQUAL(s, "0.00e0");
    TRY(x = 0);           TRY(s = x.str("e4"));  TEST_EQUAL(s, "0.000e0");
    TRY(x = 42);          TRY(s = x.str("e0"));  TEST_EQUAL(s, "4e1");
    TRY(x = 42);          TRY(s = x.str("e1"));  TEST_EQUAL(s, "4e1");
    TRY(x = 42);          TRY(s = x.str("e2"));  TEST_EQUAL(s, "4.2e1");
    TRY(x = 42);          TRY(s = x.str("e3"));  TEST_EQUAL(s, "4.20e1");
    TRY(x = 42);          TRY(s = x.str("e4"));  TEST_EQUAL(s, "4.200e1");
    TRY(x = 1234.56);     TRY(s = x.str("e0"));  TEST_EQUAL(s, "1e3");
    TRY(x = 1234.56);     TRY(s = x.str("e1"));  TEST_EQUAL(s, "1e3");
    TRY(x = 1234.56);     TRY(s = x.str("e2"));  TEST_EQUAL(s, "1.2e3");
    TRY(x = 1234.56);     TRY(s = x.str("e3"));  TEST_EQUAL(s, "1.23e3");
    TRY(x = 1234.56);     TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.235e3");
    TRY(x = 0.123456);    TRY(s = x.str("e0"));  TEST_EQUAL(s, "1e-1");
    TRY(x = 0.123456);    TRY(s = x.str("e1"));  TEST_EQUAL(s, "1e-1");
    TRY(x = 0.123456);    TRY(s = x.str("e2"));  TEST_EQUAL(s, "1.2e-1");
    TRY(x = 0.123456);    TRY(s = x.str("e3"));  TEST_EQUAL(s, "1.23e-1");
    TRY(x = 0.123456);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.235e-1");
    TRY(x = 1.23456e6);   TRY(s = x.str("e0"));  TEST_EQUAL(s, "1e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("e1"));  TEST_EQUAL(s, "1e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("e2"));  TEST_EQUAL(s, "1.2e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("e3"));  TEST_EQUAL(s, "1.23e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.235e6");
    TRY(x = 1.23456e-3);  TRY(s = x.str("e0"));  TEST_EQUAL(s, "1e-3");
    TRY(x = 1.23456e-3);  TRY(s = x.str("e1"));  TEST_EQUAL(s, "1e-3");
    TRY(x = 1.23456e-3);  TRY(s = x.str("e2"));  TEST_EQUAL(s, "1.2e-3");
    TRY(x = 1.23456e-3);  TRY(s = x.str("e3"));  TEST_EQUAL(s, "1.23e-3");
    TRY(x = 1.23456e-3);  TRY(s = x.str("e4"));  TEST_EQUAL(s, "1.235e-3");
    TRY(x = -1234.56);    TRY(s = x.str("e0"));  TEST_EQUAL(s, "-1e3");
    TRY(x = -1234.56);    TRY(s = x.str("e1"));  TEST_EQUAL(s, "-1e3");
    TRY(x = -1234.56);    TRY(s = x.str("e2"));  TEST_EQUAL(s, "-1.2e3");
    TRY(x = -1234.56);    TRY(s = x.str("e3"));  TEST_EQUAL(s, "-1.23e3");
    TRY(x = -1234.56);    TRY(s = x.str("e4"));  TEST_EQUAL(s, "-1.235e3");

    TRY(x = 1.234e-6);    TRY(s = x.str("E4"));  TEST_EQUAL(s, "1.234E-6");
    TRY(x = 1.234e6);     TRY(s = x.str("E4"));  TEST_EQUAL(s, "1.234E6");
    TRY(x = 0);           TRY(s = x.str("E4"));  TEST_EQUAL(s, "0.000E0");
    TRY(x = -1234.56);    TRY(s = x.str("E4"));  TEST_EQUAL(s, "-1.235E3");

}

void test_crow_log_scale_format_general() {

    LS x;
    std::string s;

    TRY(x = 1.234e-6);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.234e-6");
    TRY(x = 1.234e-5);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.234e-5");
    TRY(x = 1.234e-4);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.234e-4");
    TRY(x = 1.234e-3);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "0.001234");
    TRY(x = 1.234e-2);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "0.01234");
    TRY(x = 1.234e-1);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "0.1234");
    TRY(x = 1.234e0);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.234");
    TRY(x = 1.234e1);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "12.34");
    TRY(x = 1.234e2);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "123.4");
    TRY(x = 1.234e3);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "1234");
    TRY(x = 1.234e4);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "12340");
    TRY(x = 1.234e5);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "123400");
    TRY(x = 1.234e6);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.234e6");
    TRY(x = 0);           TRY(s = x.str("g0"));  TEST_EQUAL(s, "0");
    TRY(x = 0);           TRY(s = x.str("g1"));  TEST_EQUAL(s, "0");
    TRY(x = 0);           TRY(s = x.str("g2"));  TEST_EQUAL(s, "0.0");
    TRY(x = 0);           TRY(s = x.str("g3"));  TEST_EQUAL(s, "0.00");
    TRY(x = 0);           TRY(s = x.str("g4"));  TEST_EQUAL(s, "0.000");
    TRY(x = 42);          TRY(s = x.str("g0"));  TEST_EQUAL(s, "40");
    TRY(x = 42);          TRY(s = x.str("g1"));  TEST_EQUAL(s, "40");
    TRY(x = 42);          TRY(s = x.str("g2"));  TEST_EQUAL(s, "42");
    TRY(x = 42);          TRY(s = x.str("g3"));  TEST_EQUAL(s, "42.0");
    TRY(x = 42);          TRY(s = x.str("g4"));  TEST_EQUAL(s, "42.00");
    TRY(x = 123.456);     TRY(s = x.str("g0"));  TEST_EQUAL(s, "100");
    TRY(x = 123.456);     TRY(s = x.str("g1"));  TEST_EQUAL(s, "100");
    TRY(x = 123.456);     TRY(s = x.str("g2"));  TEST_EQUAL(s, "120");
    TRY(x = 123.456);     TRY(s = x.str("g3"));  TEST_EQUAL(s, "123");
    TRY(x = 123.456);     TRY(s = x.str("g4"));  TEST_EQUAL(s, "123.5");
    TRY(x = 0.123456);    TRY(s = x.str("g0"));  TEST_EQUAL(s, "0.1");
    TRY(x = 0.123456);    TRY(s = x.str("g1"));  TEST_EQUAL(s, "0.1");
    TRY(x = 0.123456);    TRY(s = x.str("g2"));  TEST_EQUAL(s, "0.12");
    TRY(x = 0.123456);    TRY(s = x.str("g3"));  TEST_EQUAL(s, "0.123");
    TRY(x = 0.123456);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "0.1235");
    TRY(x = 1.23456e6);   TRY(s = x.str("g0"));  TEST_EQUAL(s, "1e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("g1"));  TEST_EQUAL(s, "1e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("g2"));  TEST_EQUAL(s, "1.2e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("g3"));  TEST_EQUAL(s, "1.23e6");
    TRY(x = 1.23456e6);   TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.235e6");
    TRY(x = 1.23456e-6);  TRY(s = x.str("g0"));  TEST_EQUAL(s, "1e-6");
    TRY(x = 1.23456e-6);  TRY(s = x.str("g1"));  TEST_EQUAL(s, "1e-6");
    TRY(x = 1.23456e-6);  TRY(s = x.str("g2"));  TEST_EQUAL(s, "1.2e-6");
    TRY(x = 1.23456e-6);  TRY(s = x.str("g3"));  TEST_EQUAL(s, "1.23e-6");
    TRY(x = 1.23456e-6);  TRY(s = x.str("g4"));  TEST_EQUAL(s, "1.235e-6");
    TRY(x = -123.456);    TRY(s = x.str("g0"));  TEST_EQUAL(s, "-100");
    TRY(x = -123.456);    TRY(s = x.str("g1"));  TEST_EQUAL(s, "-100");
    TRY(x = -123.456);    TRY(s = x.str("g2"));  TEST_EQUAL(s, "-120");
    TRY(x = -123.456);    TRY(s = x.str("g3"));  TEST_EQUAL(s, "-123");
    TRY(x = -123.456);    TRY(s = x.str("g4"));  TEST_EQUAL(s, "-123.5");

    TRY(x = 1.234e-6);    TRY(s = x.str("G4"));  TEST_EQUAL(s, "1.234E-6");
    TRY(x = 1.234e6);     TRY(s = x.str("G4"));  TEST_EQUAL(s, "1.234E6");
    TRY(x = 0);           TRY(s = x.str("G4"));  TEST_EQUAL(s, "0.000");
    TRY(x = -123.456);    TRY(s = x.str("G4"));  TEST_EQUAL(s, "-123.5");

}

void test_crow_log_scale_format_extreme_values() {

    LS x;
    std::string s;

    TRY(x = 123456);  TRY(s = x.str());  TEST_EQUAL(s, "123456");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "1.52414e10");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.323e20");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "5.39632e40");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.91203e81");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "8.47989e162");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "7.19086e325");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "5.17085e651");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.67377e1303");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "7.14903e2606");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "5.11087e5213");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.61209e10427");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "6.82304e20854");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "4.65539e41709");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.16726e83419");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "4.69703e166838");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.20621e333677");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "4.86734e667354");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "2.3691e1334709");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "5.61264e2669418");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "3.15017e5338837");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "9.92357e10677674");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "9.84773e21355349");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "9.69778e42710699");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "9.40469e85421399");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "8.84483e170842799");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "7.8231e341685599");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "6.12008e683371199");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "3.74554e1366742399");
    TRY(x *= x);      TRY(s = x.str());  TEST_EQUAL(s, "1.40291e2733484799");

}

void test_crow_log_scale_format_zeroes() {

    LS x;
    std::string s;

    TRY(x = 0);        TRY(s = x.str("d6"));   TEST_EQUAL(s, "0.00000");
    TRY(x = 0);        TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0");
    TRY(x = 1);        TRY(s = x.str("d6"));   TEST_EQUAL(s, "1.00000");
    TRY(x = 1);        TRY(s = x.str("dz6"));  TEST_EQUAL(s, "1");
    TRY(x = 42);       TRY(s = x.str("d6"));   TEST_EQUAL(s, "42.0000");
    TRY(x = 42);       TRY(s = x.str("dz6"));  TEST_EQUAL(s, "42");
    TRY(x = -42);      TRY(s = x.str("d6"));   TEST_EQUAL(s, "-42.0000");
    TRY(x = -42);      TRY(s = x.str("dz6"));  TEST_EQUAL(s, "-42");
    TRY(x = 1.23e-6);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.00000123");
    TRY(x = 1.23e-5);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.0000123");
    TRY(x = 1.23e-4);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.000123");
    TRY(x = 1.23e-3);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.00123");
    TRY(x = 1.23e-2);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.0123");
    TRY(x = 1.23e-1);  TRY(s = x.str("dz6"));  TEST_EQUAL(s, "0.123");
    TRY(x = 1.23e0);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "1.23");
    TRY(x = 1.23e1);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "12.3");
    TRY(x = 1.23e2);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "123");
    TRY(x = 1.23e3);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "1230");
    TRY(x = 1.23e4);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "12300");
    TRY(x = 1.23e5);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "123000");
    TRY(x = 1.23e6);   TRY(s = x.str("dz6"));  TEST_EQUAL(s, "1230000");
    TRY(x = 1.23e-6);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-6");
    TRY(x = 1.23e-5);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-5");
    TRY(x = 1.23e-4);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-4");
    TRY(x = 1.23e-3);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-3");
    TRY(x = 1.23e-2);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-2");
    TRY(x = 1.23e-1);  TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e-1");
    TRY(x = 1.23e0);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e0");
    TRY(x = 1.23e1);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e1");
    TRY(x = 1.23e2);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e2");
    TRY(x = 1.23e3);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e3");
    TRY(x = 1.23e4);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e4");
    TRY(x = 1.23e5);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e5");
    TRY(x = 1.23e6);   TRY(s = x.str("ez6"));  TEST_EQUAL(s, "1.23e6");
    TRY(x = 1.23e-6);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1.23e-6");
    TRY(x = 1.23e-5);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1.23e-5");
    TRY(x = 1.23e-4);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1.23e-4");
    TRY(x = 1.23e-3);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "0.00123");
    TRY(x = 1.23e-2);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "0.0123");
    TRY(x = 1.23e-1);  TRY(s = x.str("gz6"));  TEST_EQUAL(s, "0.123");
    TRY(x = 1.23e0);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1.23");
    TRY(x = 1.23e1);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "12.3");
    TRY(x = 1.23e2);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "123");
    TRY(x = 1.23e3);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1230");
    TRY(x = 1.23e4);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "12300");
    TRY(x = 1.23e5);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "123000");
    TRY(x = 1.23e6);   TRY(s = x.str("gz6"));  TEST_EQUAL(s, "1.23e6");

}
