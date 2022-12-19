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

    std::string dir, file, no_file, parent, parent_no_file;
    std::set<std::string> files;

    Options opt1("Hello", "", "Says hello.");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(dir, "dir", 'd', "Directory option", Options::dir_exists));
    TRY(opt1.add(file, "file", 'f', "File option", Options::file_exists));
    TRY(opt1.add(no_file, "no-file", 'n', "No file option", Options::not_exists));
    TRY(opt1.add(parent, "parent", 'p', "Parent option", Options::parent_exists));
    TRY(opt1.add(parent_no_file, "parent-no-file", 'q', "Parent but no file option", Options::not_exists | Options::parent_exists));
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
            "    --dir, -d <file>             = Directory option\n"
            "    --file, -f <file>            = File option\n"
            "    --no-file, -n <file>         = No file option\n"
            "    --parent, -p <file>          = Parent option\n"
            "    --parent-no-file, -q <file>  = Parent but no file option\n"
            "    --files, -F <file> ...       = Files option\n"
            "    --help, -h                   = Show usage information\n"
            "    --version, -v                = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-d", test_dir,
            "-f", test_file1,
            "-n", test_no_file,
            "-p", test_no_file,
            "-q", test_no_file,
            "-F", test_dir, test_file1, test_file2, test_file3,
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("dir"));
        TEST(opt2.found("file"));
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
        TEST(opt2.parse({"-p", test_file1}, out));
        TEST(opt2.found("parent"));
        TEST_EQUAL(parent, test_file1);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"-d", test_file1}, out),
            Options::user_error, quote(test_file1));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"-f", test_dir}, out),
            Options::user_error, quote(test_dir));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"-n", test_file1}, out),
            Options::user_error, quote(test_file1));
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW(opt2.parse({"-p", test_no_parent}, out), Options::user_error);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW(opt2.parse({"-q", test_file1}, out), Options::user_error);
    }

    {
        Options opt2 = opt1;
        std::ostringstream out;
        TEST_THROW_MATCH(opt2.parse({"-F", test_no_file}, out),
            Options::user_error, quote(test_no_file));
    }

}
