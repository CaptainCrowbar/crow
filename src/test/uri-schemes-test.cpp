#include "crow/uri.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;
using namespace Crow::Literals;

void test_crow_uri_parse_http() {

    Uri u;

    TEST_EQUAL(u.str(), "");

    TEST(! u.has_slashes());   TEST_EQUAL(u.scheme(), "");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(! u.has_host());      TEST_EQUAL(u.host(), "");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(! u.has_path());      TEST_EQUAL(u.path(), "");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TEST_EQUAL(u.path_dir(), "");
    TEST_EQUAL(u.path_leaf(), "");

    TRY(u = "https://www.example.com/"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com/");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(! u.has_path());      TEST_EQUAL(u.path(), "/");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TEST_EQUAL(u.path_dir(), "/");
    TEST_EQUAL(u.path_leaf(), "");

    TRY(u = "https://www.example.com/foo/bar"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TEST_EQUAL(u.path_dir(), "/foo/");
    TEST_EQUAL(u.path_leaf(), "bar");

    TRY(u = "https://www.example.com/foo/bar/"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar/");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TEST_EQUAL(u.path_dir(), "/foo/bar/");
    TEST_EQUAL(u.path_leaf(), "");

    TRY(u = "https://www.example.com:80/foo/bar"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com:80/foo/bar");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(u.has_port());        TEST_EQUAL(u.port(), 80);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://uuu@www.example.com/foo/bar"_uri);
    TEST_EQUAL(u.str(), "https://uuu@www.example.com/foo/bar");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(u.has_user());        TEST_EQUAL(u.user(), "uuu");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://uuu:ppp@www.example.com/foo/bar"_uri);
    TEST_EQUAL(u.str(), "https://uuu:ppp@www.example.com/foo/bar");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(u.has_user());        TEST_EQUAL(u.user(), "uuu");
    TEST(u.has_password());    TEST_EQUAL(u.password(), "ppp");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://www.example.com/foo/bar?qqq"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com/foo/bar?qqq");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(u.has_query());       TEST_EQUAL(u.query(), "qqq");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://www.example.com/foo/bar#fff"_uri);
    TEST_EQUAL(u.str(), "https://www.example.com/foo/bar#fff");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "https");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "www.example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(u.has_fragment());    TEST_EQUAL(u.fragment(), "fff");

    TRY(u = "https://uuu:ppp@www.example.com:80/foo/bar/?qqq#fff"_uri);
    TEST_EQUAL(u.str(), "https://uuu:ppp@www.example.com:80/foo/bar/?qqq#fff");

    TEST(u.has_slashes());   TEST_EQUAL(u.scheme(), "https");
    TEST(u.has_user());      TEST_EQUAL(u.user(), "uuu");
    TEST(u.has_password());  TEST_EQUAL(u.password(), "ppp");
    TEST(u.has_host());      TEST_EQUAL(u.host(), "www.example.com");
    TEST(u.has_port());      TEST_EQUAL(u.port(), 80);
    TEST(u.has_path());      TEST_EQUAL(u.path(), "/foo/bar/");
    TEST(u.has_query());     TEST_EQUAL(u.query(), "qqq");
    TEST(u.has_fragment());  TEST_EQUAL(u.fragment(), "fff");

}

void test_crow_uri_parse_file() {

    Uri u;

    TRY(u = "file:///foo/bar/hello.txt"_uri);
    TEST_EQUAL(u.str(), "file:///foo/bar/hello.txt");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "file");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(! u.has_host());      TEST_EQUAL(u.host(), "");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar/hello.txt");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "file:///C:/foo/bar/hello.txt"_uri);
    TEST_EQUAL(u.str(), "file:///C:/foo/bar/hello.txt");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "file");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(! u.has_host());      TEST_EQUAL(u.host(), "");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/C:/foo/bar/hello.txt");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

    TRY(u = "file://server/foo/bar/hello.txt"_uri);
    TEST_EQUAL(u.str(), "file://server/foo/bar/hello.txt");

    TEST(u.has_slashes());     TEST_EQUAL(u.scheme(), "file");
    TEST(! u.has_user());      TEST_EQUAL(u.user(), "");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "server");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(u.has_path());        TEST_EQUAL(u.path(), "/foo/bar/hello.txt");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

}

void test_crow_uri_parse_mailto() {

    Uri u;

    TRY(u = "mailto:joe.average@example.com"_uri);
    TEST_EQUAL(u.str(), "mailto:joe.average@example.com");

    TEST(! u.has_slashes());   TEST_EQUAL(u.scheme(), "mailto");
    TEST(u.has_user());        TEST_EQUAL(u.user(), "joe.average");
    TEST(! u.has_password());  TEST_EQUAL(u.password(), "");
    TEST(u.has_host());        TEST_EQUAL(u.host(), "example.com");
    TEST(! u.has_port());      TEST_EQUAL(u.port(), 0);
    TEST(! u.has_path());      TEST_EQUAL(u.path(), "");
    TEST(! u.has_query());     TEST_EQUAL(u.query(), "");
    TEST(! u.has_fragment());  TEST_EQUAL(u.fragment(), "");

}
