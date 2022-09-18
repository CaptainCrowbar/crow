#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_help() {

    Options opt1("Hello", "1.0", "Says hello.", "Also says goodbye.");
    TRY(opt1.auto_help());

    {
        Options opt2 = opt1;
        TRY(opt2.set_colour(false));
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello 1.0\n"
            "\n"
            "Says hello.\n"
            "\n"
            "Options:\n"
            "    --help, -h     = Show usage information\n"
            "    --version, -v  = Show version information\n"
            "\n"
            "Also says goodbye.\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        TRY(opt2.set_colour(true));
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_MATCH(out.str(),
            R"(\n)"
            R"(\x1b\[1m\x1b\[38;5;\d+mHello 1.0\x1b\[0m\n)"
            R"(\n)"
            R"(\x1b\[38;5;\d+mSays hello.\x1b\[0m\n)"
            R"(\n)"
            R"(\x1b\[38;5;\d+mOptions:\x1b\[0m\n)"
            R"(    \x1b\[38;5;\d+m--help, -h     \x1b\[38;5;\d+m= Show usage information\x1b\[0m\n)"
            R"(    \x1b\[38;5;\d+m--version, -v  \x1b\[38;5;\d+m= Show version information\x1b\[0m\n)"
            R"(\n)"
            R"(\x1b\[38;5;\d+mAlso says goodbye.\x1b\[0m\n)"
            R"(\n)"
        );
    }

    int a = 86;
    int b = 99;

    TRY(opt1.add(a, "alpha", 'a', "Alpha option"));
    TRY(opt1.add(b, "bravo", 'b', "Bravo option", Options::no_default));

    {
        Options opt2 = opt1;
        TRY(opt2.set_colour(false));
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello 1.0\n"
            "\n"
            "Says hello.\n"
            "\n"
            "Options:\n"
            "    --alpha, -a <int>  = Alpha option (default 86)\n"
            "    --bravo, -b <int>  = Bravo option\n"
            "    --help, -h         = Show usage information\n"
            "    --version, -v      = Show version information\n"
            "\n"
            "Also says goodbye.\n"
            "\n"
        );
    }

}
