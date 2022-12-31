#include "crow/options.hpp"
#include "crow/path.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <sstream>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_options_types() {

    #ifdef _WIN32
        std::string default_path = "C:\\Windows";
        std::string another_path = "C:\\Program Files";
    #else
        std::string default_path = "/usr/bin";
        std::string another_path = "/dev/null";
    #endif

    std::string default_uri = "https://github.com/";
    std::string another_uri = "https://reddit.com/";
    std::string s = "Hello";
    Path p = default_path;
    Uri u(default_uri);
    std::vector<Path> ps;
    std::vector<Uri> us;

    Options opt1("Hello", "", "Goes ding when there's stuff");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(s, "string", 's', "String option"));
    TRY(opt1.add(p, "path", 'p', "Path option"));
    TRY(opt1.add(u, "uri", 'u', "URI option"));
    TRY(opt1.add(ps, "paths", 'P', "Multiple path option"));
    TRY(opt1.add(us, "uris", 'U', "Multiple URI option"));

    {
        Options opt2 = opt1;
        TRY(opt2.auto_help());
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello\n"
            "\n"
            "Goes ding when there's stuff\n"
            "\n"
            "Options:\n"
            "    --string, -s <arg>     = String option (default \"Hello\")\n"
            "    --path, -p <arg>       = Path option (default \"" + default_path + "\")\n"
            "    --uri, -u <arg>        = URI option (default \"" + default_uri + "\")\n"
            "    --paths, -P <arg> ...  = Multiple path option\n"
            "    --uris, -U <arg> ...   = Multiple URI option\n"
            "    --help, -h             = Show usage information\n"
            "    --version, -v          = Show version information\n"
            "\n"
        );
    }

    {
        Options opt2 = opt1;
        s = "Hello";
        p = default_path;
        u = Uri(default_uri);
        ps.clear();
        us.clear();
        std::ostringstream out;
        TEST(opt2.parse({}, out));
        TEST_EQUAL(out.str(), "");
        TEST(! opt2.found("string"));
        TEST(! opt2.found("path"));
        TEST(! opt2.found("uri"));
        TEST(! opt2.found("paths"));
        TEST(! opt2.found("uris"));
        TEST_EQUAL(s, "Hello");
        TEST_EQUAL(p, Path(default_path));
        TEST_EQUAL(u, Uri(default_uri));
        TEST(ps.empty());
        TEST(us.empty());
    }

    {
        Options opt2 = opt1;
        s = "Hello";
        p = default_path;
        u = Uri(default_uri);
        ps.clear();
        us.clear();
        std::ostringstream out;
        TEST(opt2.parse({
            "--string", "Goodbye",
            "--path", another_path,
            "--uri", another_uri,
            "--paths", default_path, another_path,
            "--uris", default_uri, another_uri,
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("string"));
        TEST(opt2.found("path"));
        TEST(opt2.found("uri"));
        TEST_EQUAL(s, "Goodbye");
        TEST_EQUAL(p, Path(another_path));
        TEST_EQUAL(u, Uri(another_uri));
        TEST_EQUAL(ps.size(), 2u);
        TEST_EQUAL(us.size(), 2u);
        REQUIRE(ps.size() == 2);
        REQUIRE(us.size() == 2);
        TEST_EQUAL(ps[0], Path(default_path));
        TEST_EQUAL(ps[1], Path(another_path));
        TEST_EQUAL(us[0], Uri(default_uri));
        TEST_EQUAL(us[1], Uri(another_uri));
    }

}
