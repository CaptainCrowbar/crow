#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_parsing() {

    std::string s = "Hello";
    int i = -123;
    unsigned u = 456;
    double d = 789.5;
    bool b = false;

    Options opt1("Hello", "", "Says hello.", "Also says goodbye.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(s, "string", 's', "String option"));
    TRY(opt1.add(i, "integer", 'i', "Integer option"));
    TRY(opt1.add(u, "unsigned", 'u', "Unsigned option"));
    TRY(opt1.add(d, "real", 'r', "Real option"));
    TRY(opt1.add(b, "boolean", 'b', "Boolean option"));

    {
        Options opt2 = opt1;
        TRY(opt2.auto_help());
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello\n"
            "\n"
            "Says hello.\n"
            "\n"
            "Options:\n"
            "    --string, -s <arg>     = String option (default \"Hello\")\n"
            "    --integer, -i <int>    = Integer option (default -123)\n"
            "    --unsigned, -u <uint>  = Unsigned option (default 456)\n"
            "    --real, -r <real>      = Real option (default 789.5)\n"
            "    --boolean, -b          = Boolean option\n"
            "    --help, -h             = Show usage information\n"
            "    --version, -v          = Show version information\n"
            "\n"
            "Also says goodbye.\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({}, out));
        TEST_EQUAL(out.str(), "");
        TEST(! opt2.found("string"));
        TEST(! opt2.found("integer"));
        TEST(! opt2.found("unsigned"));
        TEST(! opt2.found("real"));
        TEST(! opt2.found("boolean"));
        TEST_EQUAL(s, "Hello");
        TEST_EQUAL(i, -123);
        TEST_EQUAL(u, 456u);
        TEST_EQUAL(d, 789.5);
        TEST(! b);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--string", "Goodbye",
            "--integer", "86",
            "--unsigned", "99",
            "--real", "42.5",
            "--boolean",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("string"));
        TEST(opt2.found("integer"));
        TEST(opt2.found("unsigned"));
        TEST(opt2.found("real"));
        TEST(opt2.found("boolean"));
        TEST_EQUAL(s, "Goodbye");
        TEST_EQUAL(i, 86);
        TEST_EQUAL(u, 99u);
        TEST_EQUAL(d, 42.5);
        TEST(b);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--string=Farewell",
            "--integer=123",
            "--unsigned=456",
            "--real=789",
            "--boolean",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("string"));
        TEST(opt2.found("integer"));
        TEST(opt2.found("unsigned"));
        TEST(opt2.found("real"));
        TEST(opt2.found("boolean"));
        TEST_EQUAL(s, "Farewell");
        TEST_EQUAL(i, 123);
        TEST_EQUAL(u, 456u);
        TEST_EQUAL(d, 789);
        TEST(b);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-s", "Hello again",
            "-i", "987",
            "-u", "654",
            "-r", "321",
            "-b",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("string"));
        TEST(opt2.found("integer"));
        TEST(opt2.found("unsigned"));
        TEST(opt2.found("real"));
        TEST(opt2.found("boolean"));
        TEST_EQUAL(s, "Hello again");
        TEST_EQUAL(i, 987);
        TEST_EQUAL(u, 654u);
        TEST_EQUAL(d, 321);
        TEST(b);
    }

}
