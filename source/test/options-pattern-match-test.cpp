#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_pattern_match() {

    std::string h = "Hello";
    std::string g = "Goodbye";
    std::string f = "Fubar";

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(h, "hello", 'h', "Hello option", 0, "", "He.*"));
    TRY(opt1.add(g, "goodbye", 'g', "Goodbye option", 0, "", "(?i)go.*"));
    TEST_THROW(opt1.add(f, "fubar", 'f', "Fubar option", 0, "", "*"), Options::setup_error);
    TEST_THROW(opt1.add(f, "fubar", 'f', "Fubar option", 0, "", "fu.*"), Options::setup_error);

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
            "    --hello, -h <arg>    = Hello option (default \"Hello\")\n"
            "    --goodbye, -g <arg>  = Goodbye option (default \"Goodbye\")\n"
            "    --help               = Show usage information\n"
            "    --version, -v        = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--hello", "Hellfire",
            "--goodbye", "Godzilla",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("hello"));
        TEST(opt2.found("goodbye"));
        TEST_EQUAL(h, "Hellfire");
        TEST_EQUAL(g, "Godzilla");
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({
            "--hello", "Hellfire",
            "--goodbye", "Grinch",
        }, out),
            Options::user_error, "Grinch");
    }

}
