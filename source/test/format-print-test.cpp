#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <sstream>
#include <vector>

using namespace Crow;

void test_crow_print() {

    std::vector<int> v = {123,456,789};

    TEST_EQUAL(prints(),                      "");
    TEST_EQUAL(prints(123, 456, 789),         "123 456 789");
    TEST_EQUAL(prints("abc", 86, "xyz", 99),  "abc 86 xyz 99");
    TEST_EQUAL(prints("vec", v),              "vec [123,456,789]");

    {
        std::ostringstream out;
        TRY(printct(out, "abc {0} def {1}", 86, 99));
        TEST_EQUAL(out.str(), "abc 86 def 99\n");
    }

    {
        std::ostringstream out;
        TRY(printct(out, "abc {0:x4} def {1:f2}", 86, 99));
        TEST_EQUAL(out.str(), "abc 0056 def 99.00\n");
    }

    {
        std::ostringstream out;
        TRY(printct(out, "vec {0}", v));
        TEST_EQUAL(out.str(), "vec [123,456,789]\n");
    }

    {
        std::ostringstream out;
        TRY(printct(out, "vec {0:x4}", v));
        TEST_EQUAL(out.str(), "vec [007b,01c8,0315]\n");
    }

    {
        std::ostringstream out;
        TRY(printct(out, "vec {0:f2}", v));
        TEST_EQUAL(out.str(), "vec [123.00,456.00,789.00]\n");
    }

    {
        std::ostringstream out;
        TRY(printpt(out, "uvw", 86, "xyz", 99));
        TEST_EQUAL(out.str(), "uvw 86 xyz 99\n");
    }

    {
        std::ostringstream out;
        TRY(printpt(out, "vec", v));
        TEST_EQUAL(out.str(), "vec [123,456,789]\n");
    }

}
