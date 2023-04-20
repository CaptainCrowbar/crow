#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <sstream>
#include <string>

using namespace Crow;

void test_crow_options_required() {

    std::string s;
    int i = 0;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(s, "string", 's', "String option"));
    TRY(opt1.add(i, "integer", 'i', "Integer option", Options::required));

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
            "    --string, -s <arg>   = String option\n"
            "    --integer, -i <int>  = Integer option (required)\n"
            "    --help, -h           = Show usage information\n"
            "    --version, -v        = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW(opt2.parse({}, out), Options::user_error);
        TEST_EQUAL(out.str(), "");
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--integer", "42",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(! opt2.found("string"));
        TEST(opt2.found("integer"));
        TEST_EQUAL(s, "");
        TEST_EQUAL(i, 42);
    }

}
