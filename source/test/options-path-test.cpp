#include "crow/options.hpp"
#include "crow/format.hpp"
#include "crow/path.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <string>

using namespace Crow;

namespace {

    #ifdef _WIN32

        // TODO

        constexpr const char* test_dir =
        constexpr const char* test_file1 =
        constexpr const char* test_file2 =
        constexpr const char* test_file3 =
        constexpr const char* test_no_file =
        constexpr const char* test_no_parent =

    #else

        constexpr const char* test_dir = "/usr/bin";
        constexpr const char* test_file1 = "/usr/bin/cd";
        constexpr const char* test_file2 = "/usr/bin/env";
        constexpr const char* test_file3 = "/usr/bin/tr";
        constexpr const char* test_no_file = "/usr/bin/no_such_thing";
        constexpr const char* test_no_parent = "/abc/def";

    #endif

}

void test_crow_options_path() {

    std::string file, dir, parent, no_file;
    std::set<std::string> files;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(file, "file", 'f', "File option", Options::file_exists));
    TRY(opt1.add(dir, "dir", 'd', "Directory option", Options::dir_exists));
    TRY(opt1.add(parent, "parent", 'p', "Parent option", Options::parent_exists));
    TRY(opt1.add(no_file, "no-file", 'n', "No file option", Options::not_exists));
    TRY(opt1.add(files, "files", 'F', "Files option", Options::file_exists | Options::dir_exists));

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
            "    --file, -f <arg>       = File option\n"
            "    --dir, -d <arg>        = Directory option\n"
            "    --parent, -p <arg>     = Parent option\n"
            "    --no-file, -n <arg>    = No file option\n"
            "    --files, -F <arg> ...  = Files option\n"
            "    --help, -h             = Show usage information\n"
            "    --version, -v          = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-f", test_file1,
            "-d", test_dir,
            "-p", test_no_file,
            "-n", test_no_file,
            "-F", test_dir, test_file1, test_file2, test_file3
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("file"));
        TEST(opt2.found("dir"));
        TEST(opt2.found("parent"));
        TEST(opt2.found("no-file"));
        TEST(opt2.found("files"));
        TEST_EQUAL(file, test_file1);
        TEST_EQUAL(dir, test_dir);
        TEST_EQUAL(parent, test_no_file);
        TEST_EQUAL(no_file, test_no_file);
        TEST_EQUAL(format_range(files), fmt("[{0},{1},{2},{3}]", test_dir, test_file1, test_file2, test_file3));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"--file", test_dir}, out),
            Options::user_error, quote(test_dir));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"--dir", test_file1}, out),
            Options::user_error, quote(test_file1));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW(opt2.parse({"--parent", test_no_parent}, out), Options::user_error);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"--no-file", test_file1}, out),
            Options::user_error, quote(test_file1));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"--files", test_no_file}, out),
            Options::user_error, quote(test_no_file));
    }

}
