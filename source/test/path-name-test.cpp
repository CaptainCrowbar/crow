#include "crow/path.hpp"
#include "crow/format.hpp"
#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;
using namespace std::literals;

void test_crow_path_legal_names() {

    TEST(Path().is_legal());
    TEST(Path("foo").is_legal());
    TEST(Path("foo/bar").is_legal());
    TEST(! Path("foo\0bar"s).is_legal());

    TRY(Path("", Path::legal_name));
    TRY(Path("foo", Path::legal_name));
    TRY(Path("foo/bar", Path::legal_name));
    TEST_THROW(Path("foo\0bar"s, Path::legal_name), std::invalid_argument);

    #ifdef _XOPEN_SOURCE

        TEST(Path("\x80\xff").is_legal());
        TEST(Path("/").is_legal());
        TEST(Path("/foo").is_legal());
        TEST(Path("/foo/bar").is_legal());
        TEST(Path("//foo/bar").is_legal());
        TEST(Path("//foo/\x80\xff").is_legal());

    #else

        TEST(Path("C:/").is_legal());
        TEST(Path("C:/foo").is_legal());
        TEST(Path("C:/foo/bar").is_legal());
        TEST(Path("C:foo/bar").is_legal());
        TEST(Path("/foo/bar").is_legal());
        TEST(Path("//foo/bar").is_legal());
        TEST(! Path("<foo>").is_legal());
        TEST(! Path("@:/").is_legal());
        TEST(! Path("C:/<foo>").is_legal());

    #endif

}

void test_crow_path_unicode_names() {

    Path file;

    TRY(file = "αβγδε.txt");
    TEST_EQUAL(file.name(), "αβγδε.txt");

    #ifdef _WIN32

        std::wstring crap = {0xdc00, 0xd800};

        TRY(file = L"C:/foo/bar" + crap);
        TEST(file.os_name() == L"C:\\foo\\bar" + crap);
        TEST_THROW(file.name(), std::invalid_argument);
        TEST_THROW(file.as_url(), std::invalid_argument);

    #endif

}

void test_crow_path_name_normalization() {

    Path file;

    #ifdef _XOPEN_SOURCE

        TRY(file = "hello/world");      TEST_EQUAL(file.name(), "hello/world");
        TRY(file = "/hello/world");     TEST_EQUAL(file.name(), "/hello/world");
        TRY(file = "//hello/world");    TEST_EQUAL(file.name(), "/hello/world");
        TRY(file = "///hello/world");   TEST_EQUAL(file.name(), "/hello/world");
        TRY(file = "////hello/world");  TEST_EQUAL(file.name(), "/hello/world");
        TRY(file = "hello/./world");    TEST_EQUAL(file.name(), "hello/world");
        TRY(file = "hello/world/");     TEST_EQUAL(file.name(), "hello/world");
        TRY(file = "hello/world/.");    TEST_EQUAL(file.name(), "hello/world");
        TRY(file = "hello//world");     TEST_EQUAL(file.name(), "hello/world");

    #else

        TRY(file = "hello/world");      TEST_EQUAL(file.name(), "hello\\world");
        TRY(file = "/hello/world");     TEST_EQUAL(file.name(), "\\hello\\world");
        TRY(file = "//hello/world");    TEST_EQUAL(file.name(), "\\\\hello\\world");
        TRY(file = "///hello/world");   TEST_EQUAL(file.name(), "\\\\hello\\world");
        TRY(file = "////hello/world");  TEST_EQUAL(file.name(), "\\\\hello\\world");
        TRY(file = "hello/./world");    TEST_EQUAL(file.name(), "hello\\world");
        TRY(file = "hello/world/");     TEST_EQUAL(file.name(), "hello\\world");
        TRY(file = "hello/world/.");    TEST_EQUAL(file.name(), "hello\\world");
        TRY(file = "hello//world");     TEST_EQUAL(file.name(), "hello\\world");
        TRY(file = "//hello");          TEST_EQUAL(file.name(), "\\\\hello\\");
        TRY(file = "C:/hello/world");   TEST_EQUAL(file.name(), "C:\\hello\\world");
        TRY(file = "c:/hello/world");   TEST_EQUAL(file.name(), "C:\\hello\\world");

    #endif

}

void test_crow_path_name_properties() {

    Path file;

    #ifdef _XOPEN_SOURCE

        TRY(file = "");           TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(! file.is_root());
        TRY(file = "/");          TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(file.is_root());
        TRY(file = "foo");        TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_root());
        TRY(file = "/foo");       TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_root());
        TRY(file = "foo/bar");    TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_root());
        TRY(file = "/foo/bar");   TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_root());
        TRY(file = "//foo/bar");  TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_root());

        TRY(file = "");           TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "/");          TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "foo");        TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "/foo");       TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "foo/bar");    TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "/foo/bar");   TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);
        TRY(file = "//foo/bar");  TEST_EQUAL(int(file.is_empty()) + int(file.is_absolute()) + int(file.is_relative()), 1);

        TRY(file = "");           TEST_EQUAL(file.path_form(), Path::form::empty);
        TRY(file = "/");          TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "foo");        TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "/foo");       TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "foo/bar");    TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "/foo/bar");   TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//foo/bar");  TEST_EQUAL(file.path_form(), Path::form::absolute);

        TRY(file = "");         TEST(! file.is_leaf());
        TRY(file = "/");        TEST(! file.is_leaf());
        TRY(file = "/foo");     TEST(! file.is_leaf());
        TRY(file = "foo");      TEST(file.is_leaf());
        TRY(file = "foo/bar");  TEST(! file.is_leaf());

        TRY(file = "/");         TEST_EQUAL(file.as_url(), "file:///");
        TRY(file = "/foo");      TEST_EQUAL(file.as_url(), "file:///foo");
        TRY(file = "/foo/bar");  TEST_EQUAL(file.as_url(), "file:///foo/bar");

        TEST_THROW(Path().as_url(), std::invalid_argument);

    #else

        TRY(file = "");               TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "foo");            TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "foo/bar");        TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "C:/");            TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());
        TRY(file = "C:/foo");         TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "C:");             TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(file.is_drive_relative());    TEST(! file.is_root());
        TRY(file = "C:foo");          TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(file.is_drive_relative());    TEST(! file.is_root());
        TRY(file = "/");              TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(file.is_drive_absolute());    TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "/foo");           TEST(! file.is_absolute());  TEST(! file.is_relative());  TEST(file.is_drive_absolute());    TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//foo/bar");      TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//foo/");         TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());
        TRY(file = "//foo");          TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());
        TRY(file = "//?/foo");        TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?/foo/bar");    TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?/C:/");        TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());
        TRY(file = "//?/C:/foo");     TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?/C:");         TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?/C:foo");      TEST(! file.is_absolute());  TEST(file.is_relative());    TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?///foo/bar");  TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(! file.is_root());
        TRY(file = "//?///foo/");     TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());
        TRY(file = "//?///foo");      TEST(file.is_absolute());    TEST(! file.is_relative());  TEST(! file.is_drive_absolute());  TEST(! file.is_drive_relative());  TEST(file.is_root());

        TRY(file = "");               TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "foo");            TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "foo/bar");        TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "C:/");            TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "C:/foo");         TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "C:");             TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "C:foo");          TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "/");              TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "/foo");           TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//foo/bar");      TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//foo/");         TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//foo");          TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/foo");        TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/foo/bar");    TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/C:/");        TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/C:/foo");     TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/C:");         TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?/C:foo");      TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?///foo/bar");  TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?///foo/");     TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);
        TRY(file = "//?///foo");      TEST_EQUAL(int(file.empty()) + int(file.is_absolute()) + int(file.is_relative()) + int(file.is_drive_absolute()) + int(file.is_drive_relative()), 1);

        TRY(file = "");               TEST_EQUAL(file.path_form(), Path::form::empty);
        TRY(file = "foo");            TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "foo/bar");        TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "C:/");            TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "C:/foo");         TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "C:");             TEST_EQUAL(file.path_form(), Path::form::drive_relative);
        TRY(file = "C:foo");          TEST_EQUAL(file.path_form(), Path::form::drive_relative);
        TRY(file = "/");              TEST_EQUAL(file.path_form(), Path::form::drive_absolute);
        TRY(file = "/foo");           TEST_EQUAL(file.path_form(), Path::form::drive_absolute);
        TRY(file = "//foo/bar");      TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//foo/");         TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//foo");          TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//?/foo");        TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "//?/foo/bar");    TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "//?/C:/");        TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//?/C:/foo");     TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//?/C:");         TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "//?/C:foo");      TEST_EQUAL(file.path_form(), Path::form::relative);
        TRY(file = "//?///foo/bar");  TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//?///foo/");     TEST_EQUAL(file.path_form(), Path::form::absolute);
        TRY(file = "//?///foo");      TEST_EQUAL(file.path_form(), Path::form::absolute);

        TRY(file = "");         TEST(! file.is_leaf());
        TRY(file = "C:/");      TEST(! file.is_leaf());
        TRY(file = "C:/foo");   TEST(! file.is_leaf());
        TRY(file = "foo");      TEST(file.is_leaf());
        TRY(file = "foo/bar");  TEST(! file.is_leaf());

        TRY(file = "C:/");         TEST_EQUAL(file.as_url(), "file:///C:/");
        TRY(file = "C:/foo");      TEST_EQUAL(file.as_url(), "file:///C:/foo");
        TRY(file = "C:/foo/bar");  TEST_EQUAL(file.as_url(), "file:///C:/foo/bar");

        TEST_THROW(Path().as_url(), std::invalid_argument);

    #endif

}

void test_crow_path_name_breakdown() {

    Path file;
    std::vector<std::string> vec;

    #ifdef _XOPEN_SOURCE

        TRY(file = "");            TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(format_range(vec), "[]");           TEST_EQUAL(Path::join(vec), file);
        TRY(file = "/");           TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(format_range(vec), "[/]");          TEST_EQUAL(Path::join(vec), file);
        TRY(file = "foo");         TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(format_range(vec), "[foo]");        TEST_EQUAL(Path::join(vec), file);
        TRY(file = "/foo");        TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(format_range(vec), "[/,foo]");      TEST_EQUAL(Path::join(vec), file);
        TRY(file = "foo/bar");     TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(format_range(vec), "[foo,bar]");    TEST_EQUAL(Path::join(vec), file);
        TRY(file = "/foo/bar");    TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[/,foo,bar]");  TEST_EQUAL(Path::join(vec), file);
        TRY(file = "///foo/bar");  TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[/,foo,bar]");  TEST_EQUAL(Path::join(vec), file);
        TRY(file = "//foo/bar");   TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[/,foo,bar]");  TEST_EQUAL(Path::join(vec), file);

        TRY(file = "");                         TEST_EQUAL(file.split_leaf().first, "");             TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "hello.txt");                TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "hello.world.txt");          TEST_EQUAL(file.split_leaf().first, "hello.world");  TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "hello");                    TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = ".hello");                   TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "/hello.txt");               TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/hello.txt");        TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/hello");            TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "abc/def/hello.world.txt");  TEST_EQUAL(file.split_leaf().first, "hello.world");  TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/.hello");           TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "abc/def/.hello.txt");       TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, ".txt");

        TRY(file = "");                         TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path());
        TRY(file = "hello.txt");                TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "hello.world.txt");          TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello.world.txt"));
        TRY(file = "hello");                    TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello"));
        TRY(file = ".hello");                   TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path(".hello"));
        TRY(file = "/hello.txt");               TEST_EQUAL(file.split_path().first, Path("/"));        TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "abc/def/hello.txt");        TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "abc/def/hello");            TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello"));
        TRY(file = "abc/def/hello.world.txt");  TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello.world.txt"));
        TRY(file = "abc/def/.hello");           TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path(".hello"));
        TRY(file = "abc/def/.hello.txt");       TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path(".hello.txt"));

        TRY(file = "");          TEST_EQUAL(file.split_root().first, "");   TEST_EQUAL(file.split_root().second, "");
        TRY(file = "/");         TEST_EQUAL(file.split_root().first, "/");  TEST_EQUAL(file.split_root().second, "");
        TRY(file = "/foo");      TEST_EQUAL(file.split_root().first, "/");  TEST_EQUAL(file.split_root().second, "foo");
        TRY(file = "/foo/bar");  TEST_EQUAL(file.split_root().first, "/");  TEST_EQUAL(file.split_root().second, "foo/bar");
        TRY(file = "foo/bar");   TEST_EQUAL(file.split_root().first, "");   TEST_EQUAL(file.split_root().second, "foo/bar");

    #else

        TRY(file = "");                TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(format_range(vec), "[]");                   TEST_EQUAL(Path::join(vec), file);
        TRY(file = "C:/");             TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(format_range(vec), "[C:\\]");               TEST_EQUAL(Path::join(vec), file);
        TRY(file = "foo");             TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(format_range(vec), "[foo]");                TEST_EQUAL(Path::join(vec), file);
        TRY(file = "C:/foo");          TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(format_range(vec), "[C:\\,foo]");           TEST_EQUAL(Path::join(vec), file);
        TRY(file = "foo/bar");         TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(format_range(vec), "[foo,bar]");            TEST_EQUAL(Path::join(vec), file);
        TRY(file = "C:/foo/bar");      TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[C:\\,foo,bar]");       TEST_EQUAL(Path::join(vec), file);
        TRY(file = "C:foo/bar");       TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[C:,foo,bar]");         TEST_EQUAL(Path::join(vec), file);
        TRY(file = "/foo/bar");        TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[\\,foo,bar]");         TEST_EQUAL(Path::join(vec), file);
        TRY(file = "//foo/");          TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(format_range(vec), "[\\\\foo\\]");          TEST_EQUAL(Path::join(vec), file);
        TRY(file = "//foo/bar");       TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(format_range(vec), "[\\\\foo\\,bar]");      TEST_EQUAL(Path::join(vec), file);
        TRY(file = "//foo/bar/zap");   TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[\\\\foo\\,bar,zap]");  TEST_EQUAL(Path::join(vec), file);
        TRY(file = "///foo/bar/zap");  TRY(vec = file.breakdown());  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(format_range(vec), "[\\\\foo\\,bar,zap]");  TEST_EQUAL(Path::join(vec), file);

        TRY(file = "");                         TEST_EQUAL(file.split_leaf().first, "");             TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "hello.txt");                TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "hello.world.txt");          TEST_EQUAL(file.split_leaf().first, "hello.world");  TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "hello");                    TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = ".hello");                   TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "C:/hello.txt");             TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "C:hello.txt");              TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/hello.txt");        TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/hello");            TEST_EQUAL(file.split_leaf().first, "hello");        TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "abc/def/hello.world.txt");  TEST_EQUAL(file.split_leaf().first, "hello.world");  TEST_EQUAL(file.split_leaf().second, ".txt");
        TRY(file = "abc/def/.hello");           TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, "");
        TRY(file = "abc/def/.hello.txt");       TEST_EQUAL(file.split_leaf().first, ".hello");       TEST_EQUAL(file.split_leaf().second, ".txt");

        TRY(file = "");                         TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path());
        TRY(file = "hello.txt");                TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "hello.world.txt");          TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello.world.txt"));
        TRY(file = "hello");                    TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path("hello"));
        TRY(file = ".hello");                   TEST_EQUAL(file.split_path().first, Path());           TEST_EQUAL(file.split_path().second, Path(".hello"));
        TRY(file = "C:/hello.txt");             TEST_EQUAL(file.split_path().first, Path("C:/"));      TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "C:hello.txt");              TEST_EQUAL(file.split_path().first, Path("C:"));       TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "abc/def/hello.txt");        TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello.txt"));
        TRY(file = "abc/def/hello");            TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello"));
        TRY(file = "abc/def/hello.world.txt");  TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path("hello.world.txt"));
        TRY(file = "abc/def/.hello");           TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path(".hello"));
        TRY(file = "abc/def/.hello.txt");       TEST_EQUAL(file.split_path().first, Path("abc/def"));  TEST_EQUAL(file.split_path().second, Path(".hello.txt"));

        TRY(file = "");            TEST_EQUAL(file.split_root().first, Path());       TEST_EQUAL(file.split_root().second, Path());
        TRY(file = "C:/");         TEST_EQUAL(file.split_root().first, Path("C:/"));  TEST_EQUAL(file.split_root().second, Path());
        TRY(file = "C:/foo");      TEST_EQUAL(file.split_root().first, Path("C:/"));  TEST_EQUAL(file.split_root().second, Path("foo"));
        TRY(file = "C:/foo/bar");  TEST_EQUAL(file.split_root().first, Path("C:/"));  TEST_EQUAL(file.split_root().second, Path("foo/bar"));
        TRY(file = "foo/bar");     TEST_EQUAL(file.split_root().first, Path());       TEST_EQUAL(file.split_root().second, Path("foo/bar"));
        TRY(file = "C:foo/bar");   TEST_EQUAL(file.split_root().first, Path("C:"));   TEST_EQUAL(file.split_root().second, Path("foo/bar"));
        TRY(file = "/foo/bar");    TEST_EQUAL(file.split_root().first, Path("/"));    TEST_EQUAL(file.split_root().second, Path("foo/bar"));

    #endif

}

void test_crow_path_name_combination() {

    Path file1, file2, file3;

    #ifdef _XOPEN_SOURCE

        TRY(file1 = "");         TRY(file1 /= "");         TEST_EQUAL(file1.name(), "");
        TRY(file1 = "hello");    TRY(file1 /= "");         TEST_EQUAL(file1.name(), "hello");
        TRY(file1 = "");         TRY(file1 /= "world");    TEST_EQUAL(file1.name(), "world");
        TRY(file1 = "hello");    TRY(file1 /= "world");    TEST_EQUAL(file1.name(), "hello/world");
        TRY(file1 = "hello");    TRY(file1 /= "world/");   TEST_EQUAL(file1.name(), "hello/world");
        TRY(file1 = "hello");    TRY(file1 /= "/world");   TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "hello");    TRY(file1 /= "/world/");  TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "hello/");   TRY(file1 /= "world");    TEST_EQUAL(file1.name(), "hello/world");
        TRY(file1 = "hello/");   TRY(file1 /= "world/");   TEST_EQUAL(file1.name(), "hello/world");
        TRY(file1 = "hello/");   TRY(file1 /= "/world");   TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "hello/");   TRY(file1 /= "/world/");  TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "/hello");   TRY(file1 /= "world");    TEST_EQUAL(file1.name(), "/hello/world");
        TRY(file1 = "/hello");   TRY(file1 /= "world/");   TEST_EQUAL(file1.name(), "/hello/world");
        TRY(file1 = "/hello");   TRY(file1 /= "/world");   TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "/hello");   TRY(file1 /= "/world/");  TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "/hello/");  TRY(file1 /= "world");    TEST_EQUAL(file1.name(), "/hello/world");
        TRY(file1 = "/hello/");  TRY(file1 /= "world/");   TEST_EQUAL(file1.name(), "/hello/world");
        TRY(file1 = "/hello/");  TRY(file1 /= "/world");   TEST_EQUAL(file1.name(), "/world");
        TRY(file1 = "/hello/");  TRY(file1 /= "/world/");  TEST_EQUAL(file1.name(), "/world");

        TRY(file1 = "");                 TRY(file2 = "");                   TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");
        TRY(file1 = "");                 TRY(file2 = "foo");                TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");
        TRY(file1 = "");                 TRY(file2 = "/foo");               TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");
        TRY(file1 = "foo");              TRY(file2 = "foo");                TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo");
        TRY(file1 = "foo");              TRY(file2 = "bar");                TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");
        TRY(file1 = "foo");              TRY(file2 = "foo/bar");            TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo");
        TRY(file1 = "foo/bar");          TRY(file2 = "foo/bar");            TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo/bar");
        TRY(file1 = "foo/bar");          TRY(file2 = "foo/bar/zap");        TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo/bar");
        TRY(file1 = "foo/bar");          TRY(file2 = "foozle/bar");         TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");
        TRY(file1 = "foo/bar");          TRY(file2 = "foo/barfle");         TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo");
        TRY(file1 = "foo/bar/zapgun");   TRY(file2 = "foo/bar/zapruder");   TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "foo/bar");
        TRY(file1 = "/foo");             TRY(file2 = "/foo");               TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo");
        TRY(file1 = "/foo");             TRY(file2 = "/bar");               TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/");
        TRY(file1 = "/foo");             TRY(file2 = "/foo/bar");           TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo");
        TRY(file1 = "/foo/bar");         TRY(file2 = "/foo/bar");           TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo/bar");
        TRY(file1 = "/foo/bar");         TRY(file2 = "/foo/bar/zap");       TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo/bar");
        TRY(file1 = "/foo/bar");         TRY(file2 = "/foozle/bar");        TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/");
        TRY(file1 = "/foo/bar");         TRY(file2 = "/foo/barfle");        TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo");
        TRY(file1 = "/foo/bar/zapgun");  TRY(file2 = "/foo/bar/zapruder");  TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "/foo/bar");
        TRY(file1 = "foo");              TRY(file2 = "/foo");               TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3.name(), "");

        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), ".");
        TRY(file1 = "/a");          TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../..");
        TRY(file1 = "/a/b");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "..");
        TRY(file1 = "/a/b/c/d");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "d");
        TRY(file1 = "/a/b/c/d/e");  TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "d/e");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a");          TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "b/c");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b");        TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "c");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c/d");    TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "..");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c/d/e");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../..");
        TRY(file1 = "/a/b/x");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../x");
        TRY(file1 = "/a/b/x/y");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../x/y");
        TRY(file1 = "/a/x");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x");
        TRY(file1 = "/a/x/y");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x/y");
        TRY(file1 = "/a/x/y/z");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x/y/z");
        TRY(file1 = "/x");          TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x");
        TRY(file1 = "/x/y");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x/y");
        TRY(file1 = "/x/y/z");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x/y/z");
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), ".");
        TRY(file1 = "a");           TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../..");
        TRY(file1 = "a/b");         TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "..");
        TRY(file1 = "a/b/c/d");     TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "d");
        TRY(file1 = "a/b/c/d/e");   TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "d/e");
        TRY(file1 = "a/b/c");       TRY(file2 = "a");           TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "b/c");
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "c");
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b/c/d");     TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "..");
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b/c/d/e");   TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../..");
        TRY(file1 = "a/b/x");       TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../x");
        TRY(file1 = "a/b/x/y");     TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../x/y");
        TRY(file1 = "a/x");         TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x");
        TRY(file1 = "a/x/y");       TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x/y");
        TRY(file1 = "a/x/y/z");     TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../x/y/z");
        TRY(file1 = "x");           TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x");
        TRY(file1 = "x/y");         TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x/y");
        TRY(file1 = "x/y/z");       TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3.name(), "../../../x/y/z");

        TRY(file1 = "");        TRY(file2 = "");        TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "");        TRY(file2 = "a/b/c");   TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "");        TRY(file2 = "/a/b/c");  TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "a/b/c");   TRY(file2 = "");        TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "/a/b/c");  TRY(file2 = "");        TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "a/b/c");   TRY(file2 = "/a/b/c");  TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "/a/b/c");  TRY(file2 = "a/b/c");   TEST_THROW(file1.relative_to(file2), std::invalid_argument);

        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), ".");
        TRY(file1 = "/a");          TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/b");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/b/c/d");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "d");
        TRY(file1 = "/a/b/c/d/e");  TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "d/e");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a");          TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "b/c");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b");        TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "c");
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c/d");    TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/b/c");      TRY(file2 = "/a/b/c/d/e");  TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/b/x");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/b/x/y");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/x");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/x/y");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/a/x/y/z");    TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/x");          TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/x/y");        TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "/x/y/z");      TRY(file2 = "/a/b/c");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), file1);
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), ".");
        TRY(file1 = "a/b/c/d");     TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "d");
        TRY(file1 = "a/b/c/d/e");   TRY(file2 = "a/b/c");       TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "d/e");
        TRY(file1 = "a/b/c");       TRY(file2 = "a");           TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "b/c");
        TRY(file1 = "a/b/c");       TRY(file2 = "a/b");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3.name(), "c");

        TRY(file1 = "a");        TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b");      TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/c");    TRY(file2 = "a/b/c/d");    TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/c");    TRY(file2 = "a/b/c/d/e");  TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/x");    TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/x/y");  TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/x");      TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/x/y");    TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/x/y/z");  TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x");        TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x/y");      TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x/y/z");    TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");         TRY(file2 = "");           TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");         TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");         TRY(file2 = "/a/b/c");     TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/c");    TRY(file2 = "");           TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "/a/b/c");   TRY(file2 = "");           TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a/b/c");    TRY(file2 = "/a/b/c");     TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "/a/b/c");   TRY(file2 = "a/b/c");      TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);

    #else

        TRY(file1 = "");             TRY(file1 /= "");             TEST_EQUAL(file1.name(), "");
        TRY(file1 = "hello");        TRY(file1 /= "");             TEST_EQUAL(file1.name(), "hello");
        TRY(file1 = "");             TRY(file1 /= "world");        TEST_EQUAL(file1.name(), "world");
        TRY(file1 = "hello");        TRY(file1 /= "world");        TEST_EQUAL(file1.name(), "hello\\world");
        TRY(file1 = "hello");        TRY(file1 /= "world\\");      TEST_EQUAL(file1.name(), "hello\\world");
        TRY(file1 = "hello");        TRY(file1 /= "C:\\world");    TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "hello");        TRY(file1 /= "C:\\world\\");  TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "hello\\");      TRY(file1 /= "world");        TEST_EQUAL(file1.name(), "hello\\world");
        TRY(file1 = "hello\\");      TRY(file1 /= "world\\");      TEST_EQUAL(file1.name(), "hello\\world");
        TRY(file1 = "hello\\");      TRY(file1 /= "C:\\world");    TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "hello\\");      TRY(file1 /= "C:\\world\\");  TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "C:\\hello");    TRY(file1 /= "world");        TEST_EQUAL(file1.name(), "C:\\hello\\world");
        TRY(file1 = "C:\\hello");    TRY(file1 /= "world\\");      TEST_EQUAL(file1.name(), "C:\\hello\\world");
        TRY(file1 = "C:\\hello");    TRY(file1 /= "C:\\world");    TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "C:\\hello");    TRY(file1 /= "C:\\world\\");  TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "C:\\hello\\");  TRY(file1 /= "world");        TEST_EQUAL(file1.name(), "C:\\hello\\world");
        TRY(file1 = "C:\\hello\\");  TRY(file1 /= "world\\");      TEST_EQUAL(file1.name(), "C:\\hello\\world");
        TRY(file1 = "C:\\hello\\");  TRY(file1 /= "C:\\world");    TEST_EQUAL(file1.name(), "C:\\world");
        TRY(file1 = "C:\\hello\\");  TRY(file1 /= "C:\\world\\");  TEST_EQUAL(file1.name(), "C:\\world");

        TRY(file1 = "");                      TRY(file2 = "");                        TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "");                      TRY(file2 = "foo");                     TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "");                      TRY(file2 = "C:\\foo");                 TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "foo");                   TRY(file2 = "foo");                     TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo");
        TRY(file1 = "foo");                   TRY(file2 = "bar");                     TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "foo");                   TRY(file2 = "foo\\bar");                TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo");
        TRY(file1 = "foo\\bar");              TRY(file2 = "foo\\bar");                TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo\\bar");
        TRY(file1 = "foo\\bar");              TRY(file2 = "foo\\bar\\zap");           TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo\\bar");
        TRY(file1 = "foo\\bar");              TRY(file2 = "foozle\\bar");             TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "foo\\bar");              TRY(file2 = "foo\\barfle");             TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo");
        TRY(file1 = "foo\\bar\\zapgun");      TRY(file2 = "foo\\bar\\zapruder");      TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "foo\\bar");
        TRY(file1 = "C:\\foo");               TRY(file2 = "C:\\foo");                 TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo");
        TRY(file1 = "C:\\foo");               TRY(file2 = "C:\\bar");                 TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\");
        TRY(file1 = "C:\\foo");               TRY(file2 = "C:\\foo\\bar");            TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo");
        TRY(file1 = "C:\\foo\\bar");          TRY(file2 = "C:\\foo\\bar");            TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo\\bar");
        TRY(file1 = "C:\\foo\\bar");          TRY(file2 = "C:\\foo\\bar\\zap");       TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo\\bar");
        TRY(file1 = "C:\\foo\\bar");          TRY(file2 = "C:\\foozle\\bar");         TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\");
        TRY(file1 = "C:\\foo\\bar");          TRY(file2 = "C:\\foo\\barfle");         TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo");
        TRY(file1 = "C:\\foo\\bar\\zapgun");  TRY(file2 = "C:\\foo\\bar\\zapruder");  TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:\\foo\\bar");
        TRY(file1 = "foo");                   TRY(file2 = "C:\\foo");                 TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "C:\\");                  TRY(file2 = "D:\\");                    TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "C:");                    TRY(file2 = "C:");                      TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:");
        TRY(file1 = "C:foo");                 TRY(file2 = "C:foo");                   TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "C:foo");
        TRY(file1 = "C:");                    TRY(file2 = "D:");                      TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "");
        TRY(file1 = "\\");                    TRY(file2 = "\\");                      TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "\\");
        TRY(file1 = "\\foo");                 TRY(file2 = "\\foo");                   TRY(file3 = Path::common(file1, file2));  TEST_EQUAL(file3, "\\foo");

        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, ".");
        TRY(file1 = "C:\\a");               TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..");
        TRY(file1 = "C:\\a\\b");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..");
        TRY(file1 = "C:\\a\\b\\c\\d");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "d");
        TRY(file1 = "C:\\a\\b\\c\\d\\e");   TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "d\\e");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a");               TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "b\\c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b");            TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c\\d");      TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c\\d\\e");   TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..");
        TRY(file1 = "C:\\a\\b\\x");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\x");
        TRY(file1 = "C:\\a\\b\\x\\y");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\x\\y");
        TRY(file1 = "C:\\a\\x");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x");
        TRY(file1 = "C:\\a\\x\\y");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x\\y");
        TRY(file1 = "C:\\a\\x\\y\\z");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x\\y\\z");
        TRY(file1 = "C:\\x");               TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x");
        TRY(file1 = "C:\\x\\y");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x\\y");
        TRY(file1 = "C:\\x\\y\\z");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x\\y\\z");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, ".");
        TRY(file1 = "a");                   TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..");
        TRY(file1 = "a\\b");                TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..");
        TRY(file1 = "a\\b\\c\\d");          TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "d");
        TRY(file1 = "a\\b\\c\\d\\e");       TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "d\\e");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a");                   TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "b\\c");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b");                TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "c");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c\\d");          TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c\\d\\e");       TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..");
        TRY(file1 = "a\\b\\x");             TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\x");
        TRY(file1 = "a\\b\\x\\y");          TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\x\\y");
        TRY(file1 = "a\\x");                TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x");
        TRY(file1 = "a\\x\\y");             TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x\\y");
        TRY(file1 = "a\\x\\y\\z");          TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\x\\y\\z");
        TRY(file1 = "x");                   TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x");
        TRY(file1 = "x\\y");                TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x\\y");
        TRY(file1 = "x\\y\\z");             TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x\\y\\z");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "D:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "C:\\a\\b\\c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "C:\\a\\b\\c");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, ".");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\hello");           TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "a\\b\\c");
        TRY(file1 = "\\\\hello");           TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..");
        TRY(file1 = "\\\\hello\\x\\y\\z");  TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "..\\..\\..\\x\\y\\z");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "\\\\hello\\a\\b\\c");
        TRY(file1 = "\\\\hello");           TRY(file2 = "\\\\world");           TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "\\\\hello");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\world\\a\\b\\c");  TRY(file3 = file1.relative_to(file2));  TEST_EQUAL(file3, "\\\\hello\\a\\b\\c");

        TRY(file1 = "");             TRY(file2 = "");             TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "");             TRY(file2 = "a\\b\\c");      TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "");             TRY(file2 = "C:\\a\\b\\c");  TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "a\\b\\c");      TRY(file2 = "");             TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "C:\\a\\b\\c");  TRY(file2 = "");             TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "a\\b\\c");      TRY(file2 = "C:\\a\\b\\c");  TEST_THROW(file1.relative_to(file2), std::invalid_argument);
        TRY(file1 = "C:\\a\\b\\c");  TRY(file2 = "a\\b\\c");      TEST_THROW(file1.relative_to(file2), std::invalid_argument);

        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, ".");
        TRY(file1 = "C:\\a");               TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b\\c\\d");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "d");
        TRY(file1 = "C:\\a\\b\\c\\d\\e");   TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "d\\e");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a");               TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "b\\c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b");            TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c\\d");      TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "C:\\a\\b\\c\\d\\e");   TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b\\x");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b\\x\\y");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\x");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\x\\y");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\x\\y\\z");      TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\x");               TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\x\\y");            TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\x\\y\\z");         TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, ".");
        TRY(file1 = "a\\b\\c\\d");          TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "d");
        TRY(file1 = "a\\b\\c\\d\\e");       TRY(file2 = "a\\b\\c");             TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "d\\e");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a");                   TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "b\\c");
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b");                TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "c");
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "D:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\hello\\a\\b\\c");  TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, ".");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\hello");           TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, "a\\b\\c");
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "C:\\a\\b\\c");         TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "\\\\hello");           TRY(file2 = "\\\\world");           TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);
        TRY(file1 = "\\\\hello\\a\\b\\c");  TRY(file2 = "\\\\world\\a\\b\\c");  TRY(file3 = file1.relative_to(file2, Path::no_backtrack));  TEST_EQUAL(file3, file1);

        TRY(file1 = "a");                   TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b");                TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c\\d");          TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\c");             TRY(file2 = "a\\b\\c\\d\\e");       TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\x");             TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\x\\y");          TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\x");                TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\x\\y");             TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\x\\y\\z");          TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x");                   TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x\\y");                TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "x\\y\\z");             TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "\\\\hello");           TRY(file2 = "\\\\hello\\a\\b\\c");  TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "\\\\hello\\x\\y\\z");  TRY(file2 = "\\\\hello\\a\\b\\c");  TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");                    TRY(file2 = "");                    TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");                    TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "");                    TRY(file2 = "C:\\a\\b\\c");         TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\c");             TRY(file2 = "");                    TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "");                    TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "a\\b\\c");             TRY(file2 = "C:\\a\\b\\c");         TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);
        TRY(file1 = "C:\\a\\b\\c");         TRY(file2 = "a\\b\\c");             TEST_THROW(file1.relative_to(file2, Path::no_backtrack), std::invalid_argument);

    #endif

}

void test_crow_path_name_manipulation() {

    Path file1, file2;

    TRY(file1 = "hello");            TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "hello.doc");
    TRY(file1 = "hello");            TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "hello.doc");
    TRY(file1 = "hello");            TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "hello");
    TRY(file1 = "hello.txt");        TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "hello.doc");
    TRY(file1 = "hello.txt");        TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "hello.doc");
    TRY(file1 = "hello.txt");        TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "hello");
    TRY(file1 = "hello.world.txt");  TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "hello.world.doc");
    TRY(file1 = "hello.world.txt");  TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "hello.world.doc");
    TRY(file1 = "hello.world.txt");  TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "hello.world");
    TRY(file1 = ".hello");           TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), ".hello.doc");
    TRY(file1 = ".hello");           TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), ".hello.doc");
    TRY(file1 = ".hello");           TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), ".hello");

    TRY(file1 = "");  TEST_THROW(file1.change_ext(".doc"), std::invalid_argument);

    #ifdef _XOPEN_SOURCE

        TRY(file1 = "/foo/bar/hello");      TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "/foo/bar/hello.doc");
        TRY(file1 = "/foo/bar/hello");      TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "/foo/bar/hello.doc");
        TRY(file1 = "/foo/bar/hello");      TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "/foo/bar/hello");
        TRY(file1 = "/foo/bar/hello.txt");  TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "/foo/bar/hello.doc");
        TRY(file1 = "/foo/bar/hello.txt");  TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "/foo/bar/hello.doc");
        TRY(file1 = "/foo/bar/hello.txt");  TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "/foo/bar/hello");

        TRY(file1 = "/");  TEST_THROW(file1.change_ext(".doc"), std::invalid_argument);

    #else

        TRY(file1 = "\\foo\\bar\\hello");      TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "\\foo\\bar\\hello.doc");
        TRY(file1 = "\\foo\\bar\\hello");      TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "\\foo\\bar\\hello.doc");
        TRY(file1 = "\\foo\\bar\\hello");      TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "\\foo\\bar\\hello");
        TRY(file1 = "\\foo\\bar\\hello.txt");  TRY(file2 = file1.change_ext(".doc"));  TEST_EQUAL(file2.name(), "\\foo\\bar\\hello.doc");
        TRY(file1 = "\\foo\\bar\\hello.txt");  TRY(file2 = file1.change_ext("doc"));   TEST_EQUAL(file2.name(), "\\foo\\bar\\hello.doc");
        TRY(file1 = "\\foo\\bar\\hello.txt");  TRY(file2 = file1.change_ext(""));      TEST_EQUAL(file2.name(), "\\foo\\bar\\hello");

        TRY(file1 = "\\");    TEST_THROW(file1.change_ext(".doc"), std::invalid_argument);
        TRY(file1 = "C:\\");  TEST_THROW(file1.change_ext(".doc"), std::invalid_argument);

    #endif

}

void test_crow_path_name_comparison() {

    Path file1, file2, file3;

    TRY(file1 = "foo/bar/hello.txt");
    TRY(file2 = "foo/bar/Hello.txt");
    TRY(file3 = "foo/bar/goodbye.txt");

    TEST(! Path::equal(Path::cmp::cased)(file1, file2));
    TEST(! Path::equal(Path::cmp::cased)(file1, file3));
    TEST(! Path::less(Path::cmp::cased)(file1, file2));
    TEST(Path::less(Path::cmp::cased)(file2, file1));
    TEST(! Path::less(Path::cmp::cased)(file1, file3));
    TEST(Path::less(Path::cmp::cased)(file3, file1));
    TEST(Path::less(Path::cmp::cased)(file2, file3));
    TEST(! Path::less(Path::cmp::cased)(file3, file2));

    TEST(Path::equal(Path::cmp::icase)(file1, file2));
    TEST(! Path::equal(Path::cmp::icase)(file1, file3));
    TEST(! Path::less(Path::cmp::icase)(file1, file2));
    TEST(! Path::less(Path::cmp::icase)(file2, file1));
    TEST(! Path::less(Path::cmp::icase)(file1, file3));
    TEST(Path::less(Path::cmp::icase)(file3, file1));
    TEST(! Path::less(Path::cmp::icase)(file2, file3));
    TEST(Path::less(Path::cmp::icase)(file3, file2));

    #if defined(__APPLE__) || defined(_WIN32)

        TEST(Path::equal()(file1, file2));
        TEST(! Path::equal()(file1, file3));
        TEST(! Path::less()(file1, file2));
        TEST(! Path::less()(file2, file1));
        TEST(! Path::less()(file1, file3));
        TEST(Path::less()(file3, file1));
        TEST(! Path::less()(file2, file3));
        TEST(Path::less()(file3, file2));

    #else

        TEST(! Path::equal()(file1, file2));
        TEST(! Path::equal()(file1, file3));
        TEST(! Path::less()(file1, file2));
        TEST(Path::less()(file2, file1));
        TEST(! Path::less()(file1, file3));
        TEST(Path::less()(file3, file1));
        TEST(Path::less()(file2, file3));
        TEST(! Path::less()(file3, file2));

    #endif

}
