#include "crow/options.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <sstream>
#include <vector>

using namespace Crow;

void test_crow_options_anonymous() {

    int f = 123;
    int s = 456;
    std::vector<int> r;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(f, "first", 'f', "First option", Options::anon));
    TRY(opt1.add(s, "second", 's', "Second option", Options::anon));
    TRY(opt1.add(r, "rest", 'r', "Rest of the options", Options::anon));

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
            "    [--first, -f] <int>     = First option (default 123)\n"
            "    [--second, -s] <int>    = Second option (default 456)\n"
            "    [--rest, -r] <int> ...  = Rest of the options\n"
            "    --help, -h              = Show usage information\n"
            "    --version, -v           = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({}, out));
        TEST_EQUAL(out.str(), "");
        TEST(! opt2.found("first"));
        TEST(! opt2.found("second"));
        TEST(! opt2.found("rest"));
        TEST_EQUAL(f, 123);
        TEST_EQUAL(s, 456);
        TEST_EQUAL(format_range(r), "[]");
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({"12", "34", "56", "78", "90"}, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("first"));
        TEST(opt2.found("second"));
        TEST(opt2.found("rest"));
        TEST_EQUAL(f, 12);
        TEST_EQUAL(s, 34);
        TEST_EQUAL(format_range(r), "[56,78,90]");
    }

}
