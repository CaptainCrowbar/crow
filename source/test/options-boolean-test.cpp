#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <sstream>
#include <string>

using namespace Crow;

void test_crow_options_boolean() {

    bool a = false;
    bool b = false;
    bool c = false;
    std::string d;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(a, "alpha", 'a', "Alpha option"));
    TRY(opt1.add(b, "bravo", 'b', "Bravo option"));
    TRY(opt1.add(c, "charlie", 'c', "Charlie option"));
    TRY(opt1.add(d, "delta", 'd', "Delta option", Options::anon));

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
            "    --alpha, -a          = Alpha option\n"
            "    --bravo, -b          = Bravo option\n"
            "    --charlie, -c        = Charlie option\n"
            "    [--delta, -d] <arg>  = Delta option\n"
            "    --help, -h           = Show usage information\n"
            "    --version, -v        = Show version information\n"
            "\n"
        );
    }

    {
        a = b = c = false;
        d.clear();
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-abc",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(opt2.found("bravo"));
        TEST(opt2.found("charlie"));
        TEST(! opt2.found("delta"));
        TEST(a);
        TEST(b);
        TEST(c);
    }

    {
        a = b = c = false;
        d.clear();
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-abcd", "hello"
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(opt2.found("bravo"));
        TEST(opt2.found("charlie"));
        TEST(opt2.found("delta"));
        TEST(a);
        TEST(b);
        TEST(c);
        TEST_EQUAL(d, "hello");
    }

    {
        a = b = c = false;
        d.clear();
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-abc", "hello"
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(opt2.found("bravo"));
        TEST(opt2.found("charlie"));
        TEST(opt2.found("delta"));
        TEST(a);
        TEST(b);
        TEST(c);
        TEST_EQUAL(d, "hello");
    }

    {
        a = b = c = false;
        d.clear();
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--alpha=true", "--bravo=false"
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(opt2.found("bravo"));
        TEST(! opt2.found("charlie"));
        TEST(! opt2.found("delta"));
        TEST(a);
        TEST(! b);
        TEST(! c);
    }

    {
        a = b = c = true;
        d.clear();
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--no-alpha", "--no-bravo"
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(opt2.found("bravo"));
        TEST(! opt2.found("charlie"));
        TEST(! opt2.found("delta"));
        TEST(! a);
        TEST(! b);
        TEST(c);
    }

}
