#include "crow/format-floating.hpp"
#include "crow/unit-test.hpp"
#include <complex>

using namespace Crow;

using C = std::complex<double>;

void test_crow_format_complex_default() {

    TEST_EQUAL(format_complex(0.0,              ""),  "(0,0)");
    TEST_EQUAL(format_complex(0.25,             ""),  "(0.25,0)");
    TEST_EQUAL(format_complex(-0.25,            ""),  "(-0.25,0)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     ""),  "(0,0.75)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    ""),  "(0,-0.75)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    ""),  "(0.25,0.75)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   ""),  "(0.25,-0.75)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   ""),  "(-0.25,0.75)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  ""),  "(-0.25,-0.75)");

    TEST_EQUAL(format_complex(0.0,              "f4"),  "(0.0000,0.0000)");
    TEST_EQUAL(format_complex(0.25,             "f4"),  "(0.2500,0.0000)");
    TEST_EQUAL(format_complex(-0.25,            "f4"),  "(-0.2500,0.0000)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "f4"),  "(0.0000,0.7500)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "f4"),  "(0.0000,-0.7500)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "f4"),  "(0.2500,0.7500)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "f4"),  "(0.2500,-0.7500)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "f4"),  "(-0.2500,0.7500)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "f4"),  "(-0.2500,-0.7500)");

    TEST_EQUAL(format_complex(0.0,              "fo4"),  "0.0000");
    TEST_EQUAL(format_complex(0.25,             "fo4"),  "0.2500");
    TEST_EQUAL(format_complex(-0.25,            "fo4"),  "-0.2500");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "fo4"),  "(0.0000,0.7500)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "fo4"),  "(0.0000,-0.7500)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "fo4"),  "(0.2500,0.7500)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "fo4"),  "(0.2500,-0.7500)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "fo4"),  "(-0.2500,0.7500)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "fo4"),  "(-0.2500,-0.7500)");

}

void test_crow_format_complex_cartesian() {

    TEST_EQUAL(format_complex(0.0,              "gzi"),  "0+0i");
    TEST_EQUAL(format_complex(0.25,             "gzi"),  "0.25+0i");
    TEST_EQUAL(format_complex(-0.25,            "gzi"),  "-0.25+0i");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "gzi"),  "0+0.75i");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "gzi"),  "0-0.75i");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "gzi"),  "0.25+0.75i");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "gzi"),  "0.25-0.75i");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "gzi"),  "-0.25+0.75i");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "gzi"),  "-0.25-0.75i");

    TEST_EQUAL(format_complex(0.0,              "fj4"),  "0.0000+0.0000j");
    TEST_EQUAL(format_complex(0.25,             "fj4"),  "0.2500+0.0000j");
    TEST_EQUAL(format_complex(-0.25,            "fj4"),  "-0.2500+0.0000j");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "fj4"),  "0.0000+0.7500j");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "fj4"),  "0.0000-0.7500j");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "fj4"),  "0.2500+0.7500j");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "fj4"),  "0.2500-0.7500j");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "fj4"),  "-0.2500+0.7500j");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "fj4"),  "-0.2500-0.7500j");

    TEST_EQUAL(format_complex(0.0,              "fio4"),  "0.0000");
    TEST_EQUAL(format_complex(0.25,             "fio4"),  "0.2500");
    TEST_EQUAL(format_complex(-0.25,            "fio4"),  "-0.2500");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "fio4"),  "0.7500i");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "fio4"),  "-0.7500i");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "fio4"),  "0.2500+0.7500i");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "fio4"),  "0.2500-0.7500i");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "fio4"),  "-0.2500+0.7500i");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "fio4"),  "-0.2500-0.7500i");

}

void test_crow_format_complex_polar() {

    TEST_EQUAL(format_complex(0.0,              "gzp"),  "(0,0)");
    TEST_EQUAL(format_complex(0.25,             "gzp"),  "(0.25,0)");
    TEST_EQUAL(format_complex(-0.25,            "gzp"),  "(0.25,3.14159)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "gzp"),  "(0.75,1.5708)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "gzp"),  "(0.75,-1.5708)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "gzp"),  "(0.790569,1.24905)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "gzp"),  "(0.790569,-1.24905)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "gzp"),  "(0.790569,1.89255)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "gzp"),  "(0.790569,-1.89255)");

    TEST_EQUAL(format_complex(0.0,              "fp4"),  "(0.0000,0.0000)");
    TEST_EQUAL(format_complex(0.25,             "fp4"),  "(0.2500,0.0000)");
    TEST_EQUAL(format_complex(-0.25,            "fp4"),  "(0.2500,3.1416)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "fp4"),  "(0.7500,1.5708)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "fp4"),  "(0.7500,-1.5708)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "fp4"),  "(0.7906,1.2490)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "fp4"),  "(0.7906,-1.2490)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "fp4"),  "(0.7906,1.8925)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "fp4"),  "(0.7906,-1.8925)");

    TEST_EQUAL(format_complex(0.0,              "fpo4"),  "0.0000");
    TEST_EQUAL(format_complex(0.25,             "fpo4"),  "0.2500");
    TEST_EQUAL(format_complex(-0.25,            "fpo4"),  "(0.2500,3.1416)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "fpo4"),  "(0.7500,1.5708)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "fpo4"),  "(0.7500,-1.5708)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "fpo4"),  "(0.7906,1.2490)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "fpo4"),  "(0.7906,-1.2490)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "fpo4"),  "(0.7906,1.8925)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "fpo4"),  "(0.7906,-1.8925)");

    TEST_EQUAL(format_complex(0.0,              "FP4"),  "(0.0000,0.0000)");
    TEST_EQUAL(format_complex(0.25,             "FP4"),  "(0.2500,0.0000)");
    TEST_EQUAL(format_complex(-0.25,            "FP4"),  "(0.2500,3.1416)");
    TEST_EQUAL(format_complex(C(0.0, 0.75),     "FP4"),  "(0.7500,1.5708)");
    TEST_EQUAL(format_complex(C(0.0, -0.75),    "FP4"),  "(0.7500,4.7124)");
    TEST_EQUAL(format_complex(C(0.25, 0.75),    "FP4"),  "(0.7906,1.2490)");
    TEST_EQUAL(format_complex(C(0.25, -0.75),   "FP4"),  "(0.7906,5.0341)");
    TEST_EQUAL(format_complex(C(-0.25, 0.75),   "FP4"),  "(0.7906,1.8925)");
    TEST_EQUAL(format_complex(C(-0.25, -0.75),  "FP4"),  "(0.7906,4.3906)");

}
