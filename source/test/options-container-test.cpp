#include "crow/options.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_container() {

    int f = 123;
    int s = 456;
    std::set<int> r;

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
        TEST(opt2.parse({
            "789", "789", "789", "789", "789",
            "100", "100", "100", "100", "100",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("first"));
        TEST(opt2.found("second"));
        TEST(opt2.found("rest"));
        TEST_EQUAL(f, 789);
        TEST_EQUAL(s, 789);
        TEST_EQUAL(format_range(r), "[100,789]");
    }

}

void test_crow_options_container_defaults() {

    std::vector<int> i = {42};
    std::vector<std::string> s = {"abc", "def", "ghi"};

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(i, "integers", 'i', "List of integers"));
    TRY(opt1.add(s, "strings", 's', "List of strings"));

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
            "    --integers, -i <int> ...  = List of integers (default 42)\n"
            "    --strings, -s <arg> ...   = List of strings (default [abc,def,ghi])\n"
            "    --help, -h                = Show usage information\n"
            "    --version, -v             = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-i", "12", "34", "56", "78",
            "-s", "opq", "rst", "uvw", "xyz",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("integers"));
        TEST(opt2.found("strings"));
        TEST_EQUAL(format_range(i), "[12,34,56,78]");
        TEST_EQUAL(format_range(s), "[opq,rst,uvw,xyz]");
    }

}
