#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_mutual_exclusion() {

    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int e = 50;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(a, "alpha", 'a', "Alpha option"));
    TRY(opt1.add(b, "bravo", 'b', "Bravo option", 0, "group1"));
    TRY(opt1.add(c, "charlie", 'c', "Charlie option", 0, "group1"));
    TRY(opt1.add(d, "delta", 'd', "Delta option", 0, "group1"));
    TRY(opt1.add(e, "echo", 'e', "Echo option"));

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
            "    --alpha, -a <int>    = Alpha option (default 10)\n"
            "    --bravo, -b <int>    = Bravo option (default 20)\n"
            "    --charlie, -c <int>  = Charlie option (default 30)\n"
            "    --delta, -d <int>    = Delta option (default 40)\n"
            "    --echo, -e <int>     = Echo option (default 50)\n"
            "    --help, -h           = Show usage information\n"
            "    --version, -v        = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({}, out));
        TEST_EQUAL(out.str(), "");
        TEST(! opt2.found("alpha"));
        TEST(! opt2.found("bravo"));
        TEST(! opt2.found("charlie"));
        TEST(! opt2.found("delta"));
        TEST(! opt2.found("echo"));
        TEST_EQUAL(a, 10);
        TEST_EQUAL(b, 20);
        TEST_EQUAL(c, 30);
        TEST_EQUAL(d, 40);
        TEST_EQUAL(e, 50);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "--alpha", "111",
            "--charlie", "333",
            "--echo", "555",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("alpha"));
        TEST(! opt2.found("bravo"));
        TEST(opt2.found("charlie"));
        TEST(! opt2.found("delta"));
        TEST(opt2.found("echo"));
        TEST_EQUAL(a, 111);
        TEST_EQUAL(b, 20);
        TEST_EQUAL(c, 333);
        TEST_EQUAL(d, 40);
        TEST_EQUAL(e, 555);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({
            "--alpha", "111",
            "--charlie", "333",
            "--delta", "444",
        }, out),
            Options::user_error, "--bravo, --charlie, --delta");
    }

}
