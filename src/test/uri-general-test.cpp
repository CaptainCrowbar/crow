#include "crow/uri.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_uri_assembly() {

    Uri u;

    TRY(u = Uri("http", "", "", "www.example.com"));                                     TEST_EQUAL(u.str(), "http://www.example.com");
    TRY(u = Uri("http", "user", "password", "www.example.com"));                         TEST_EQUAL(u.str(), "http://user:password@www.example.com");
    TRY(u = Uri("http", "", "", "www.example.com", 8080));                               TEST_EQUAL(u.str(), "http://www.example.com:8080");
    TRY(u = Uri("http", "", "", "www.example.com", 0, "foo/bar"));                       TEST_EQUAL(u.str(), "http://www.example.com/foo/bar");
    TRY(u = Uri("http", "", "", "www.example.com", 0, "foo/bar", "query", "fragment"));  TEST_EQUAL(u.str(), "http://www.example.com/foo/bar?query#fragment");

}

void test_crow_uri_modifiers() {

    Uri u;

    TRY(u.set_scheme("file"));  TEST_EQUAL(u.str(), "file://");  TEST_EQUAL(u.scheme(), "file");  TEST_EQUAL(u.host(), "");  TEST_EQUAL(u.path(), "");

    TRY(u = "http://www.example.com"_uri);

    TRY(u.set_scheme("https"));    TEST_EQUAL(u.str(), "https://www.example.com");  TEST_EQUAL(u.scheme(), "https");   TEST(u.has_slashes());
    TRY(u.set_scheme("mailto:"));  TEST_EQUAL(u.str(), "mailto:www.example.com");   TEST_EQUAL(u.scheme(), "mailto");  TEST(! u.has_slashes());
    TRY(u.set_scheme("ftp://"));   TEST_EQUAL(u.str(), "ftp://www.example.com");    TEST_EQUAL(u.scheme(), "ftp");     TEST(u.has_slashes());

    TRY(u = "https://www.example.com/"_uri);

    TRY(u.set_scheme("http"));     TEST_EQUAL(u.str(), "http://www.example.com/");                     TEST_EQUAL(u.scheme(), "http");           TEST_EQUAL(u.host(), "www.example.com");  TEST_EQUAL(u.path(), "/");
    TRY(u.set_user("uvwxyz"));     TEST_EQUAL(u.str(), "http://uvwxyz@www.example.com/");              TEST_EQUAL(u.user(), "uvwxyz");           TEST_EQUAL(u.host(), "www.example.com");  TEST_EQUAL(u.path(), "/");
    TRY(u.set_password("pqrst"));  TEST_EQUAL(u.str(), "http://uvwxyz:pqrst@www.example.com/");        TEST_EQUAL(u.password(), "pqrst");        TEST_EQUAL(u.host(), "www.example.com");  TEST_EQUAL(u.path(), "/");
    TRY(u.set_port(12345));        TEST_EQUAL(u.str(), "http://uvwxyz:pqrst@www.example.com:12345/");  TEST_EQUAL(u.host(), "www.example.com");  TEST_EQUAL(u.port(), 12345);              TEST_EQUAL(u.path(), "/");
    TRY(u.set_host("localhost"));  TEST_EQUAL(u.str(), "http://uvwxyz:pqrst@localhost:12345/");        TEST_EQUAL(u.host(), "localhost");        TEST_EQUAL(u.port(), 12345);              TEST_EQUAL(u.path(), "/");
    TRY(u.clear_password());       TEST_EQUAL(u.str(), "http://uvwxyz@localhost:12345/");              TEST_EQUAL(u.password(), "");             TEST_EQUAL(u.host(), "localhost");        TEST_EQUAL(u.path(), "/");
    TRY(u.clear_user());           TEST_EQUAL(u.str(), "http://localhost:12345/");                     TEST_EQUAL(u.user(), "");                 TEST_EQUAL(u.host(), "localhost");        TEST_EQUAL(u.path(), "/");
    TRY(u.clear_port());           TEST_EQUAL(u.str(), "http://localhost/");                           TEST_EQUAL(u.port(), 0);                  TEST_EQUAL(u.host(), "localhost");        TEST_EQUAL(u.path(), "/");

    TRY(u = "https://www.example.com"_uri);

    TRY(u.set_path("foo"));          TEST_EQUAL(u.str(), "https://www.example.com/foo");         TEST_EQUAL(u.path(), "/foo");
    TRY(u.set_path("foo/bar"));      TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");     TEST_EQUAL(u.path(), "/foo/bar");
    TRY(u.set_path("foo/bar/"));     TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/");    TEST_EQUAL(u.path(), "/foo/bar/");
    TRY(u.set_path(""));             TEST_EQUAL(u.str(), "https://www.example.com");             TEST_EQUAL(u.path(), "");
    TRY(u.set_path("/"));            TEST_EQUAL(u.str(), "https://www.example.com/");            TEST_EQUAL(u.path(), "/");
    TRY(u.set_path("/foo"));         TEST_EQUAL(u.str(), "https://www.example.com/foo");         TEST_EQUAL(u.path(), "/foo");
    TRY(u.set_path("/foo/bar"));     TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");     TEST_EQUAL(u.path(), "/foo/bar");
    TRY(u.set_path("/foo/bar/"));    TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/");    TEST_EQUAL(u.path(), "/foo/bar/");
    TRY(u.clear_path());             TEST_EQUAL(u.str(), "https://www.example.com");             TEST_EQUAL(u.path(), "");

    TRY(u = "https://www.example.com"_uri);

    TRY(u.set_fragment("world"));  TEST_EQUAL(u.str(), "https://www.example.com#world");        TEST_EQUAL(u.query(), "");       TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_query("hello"));     TEST_EQUAL(u.str(), "https://www.example.com?hello#world");  TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.clear_fragment());       TEST_EQUAL(u.str(), "https://www.example.com?hello");        TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "");
    TRY(u.clear_query());          TEST_EQUAL(u.str(), "https://www.example.com");              TEST_EQUAL(u.query(), "");       TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://www.example.com/foo/bar"_uri);

    TRY(u.set_fragment("world"));  TEST_EQUAL(u.str(), "https://www.example.com/foo/bar#world");        TEST_EQUAL(u.query(), "");       TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_query("hello"));     TEST_EQUAL(u.str(), "https://www.example.com/foo/bar?hello#world");  TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.clear_fragment());       TEST_EQUAL(u.str(), "https://www.example.com/foo/bar?hello");        TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "");
    TRY(u.clear_query());          TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");              TEST_EQUAL(u.query(), "");       TEST_EQUAL(u.fragment(), "");

    TRY(u = "https://www.example.com/xyz?hello#world"_uri);

    TRY(u.set_path(""));           TEST_EQUAL(u.str(), "https://www.example.com?hello#world");           TEST_EQUAL(u.path(), "");           TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_path("/"));          TEST_EQUAL(u.str(), "https://www.example.com/?hello#world");          TEST_EQUAL(u.path(), "/");          TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_path("/foo"));       TEST_EQUAL(u.str(), "https://www.example.com/foo?hello#world");       TEST_EQUAL(u.path(), "/foo");       TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_path("/foo/bar"));   TEST_EQUAL(u.str(), "https://www.example.com/foo/bar?hello#world");   TEST_EQUAL(u.path(), "/foo/bar");   TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.set_path("/foo/bar/"));  TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/?hello#world");  TEST_EQUAL(u.path(), "/foo/bar/");  TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");
    TRY(u.clear_path());           TEST_EQUAL(u.str(), "https://www.example.com?hello#world");           TEST_EQUAL(u.path(), "");           TEST_EQUAL(u.query(), "hello");  TEST_EQUAL(u.fragment(), "world");

    TRY(u = "https://www.example.com"_uri);          TRY(u /= "");             TEST_EQUAL(u.str(), "https://www.example.com");
    TRY(u = "https://www.example.com"_uri);          TRY(u /= "foo");          TEST_EQUAL(u.str(), "https://www.example.com/foo");
    TRY(u = "https://www.example.com"_uri);          TRY(u /= "/foo");         TEST_EQUAL(u.str(), "https://www.example.com/foo");
    TRY(u = "https://www.example.com"_uri);          TRY(u /= "foo/bar");      TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");
    TRY(u = "https://www.example.com/foo/bar"_uri);  TRY(u /= "");             TEST_EQUAL(u.str(), "https://www.example.com/foo/bar");
    TRY(u = "https://www.example.com/foo/bar"_uri);  TRY(u /= "hello");        TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/hello");
    TRY(u = "https://www.example.com/foo/bar"_uri);  TRY(u /= "hello/world");  TEST_EQUAL(u.str(), "https://www.example.com/foo/bar/hello/world");
    TRY(u = "https://www.example.com/foo/bar"_uri);  TRY(u /= "/hello");       TEST_EQUAL(u.str(), "https://www.example.com/hello");

}

void test_crow_uri_navigation() {

    Uri u1, u2;

    TRY(u1 = {});                                    TEST(! u1.is_root());
    TRY(u1 = "https://www.example.com"_uri);         TEST(u1.is_root());
    TRY(u1 = "https://www.example.com/"_uri);        TEST(u1.is_root());
    TRY(u1 = "https://www.example.com/foo"_uri);     TEST(! u1.is_root());
    TRY(u1 = "https://www.example.com/?hello"_uri);  TEST(! u1.is_root());
    TRY(u1 = "https://www.example.com/#world"_uri);  TEST(! u1.is_root());

    TRY(u1 = "https://www.example.com/"_uri);

    TRY(u2 = u1.doc());   TEST_EQUAL(u2.str(), "https://www.example.com/");
    TRY(u2 = u1.base());  TEST_EQUAL(u2.str(), "https://www.example.com/");
    TRY(u2 = u1.root());  TEST_EQUAL(u2.str(), "https://www.example.com/");

    TRY(u1 = "https://www.example.com/foo/bar"_uri);

    TRY(u2 = u1.doc());   TEST_EQUAL(u2.str(), "https://www.example.com/foo/bar");
    TRY(u2 = u1.base());  TEST_EQUAL(u2.str(), "https://www.example.com/foo/bar");
    TRY(u2 = u1.root());  TEST_EQUAL(u2.str(), "https://www.example.com/");

    TRY(u1 = "https://www.example.com/foo/bar?hello#world"_uri);

    TRY(u2 = u1.doc());   TEST_EQUAL(u2.str(), "https://www.example.com/foo/bar?hello");
    TRY(u2 = u1.base());  TEST_EQUAL(u2.str(), "https://www.example.com/foo/bar");
    TRY(u2 = u1.root());  TEST_EQUAL(u2.str(), "https://www.example.com/");

    TRY(u2 = "https://www.example.com/foo/bar/zap?hello#world"_uri);

    TRY(u2 = u2.parent());  TEST_EQUAL(u2.str(), "https://www.example.com/foo/bar/");
    TRY(u2 = u2.parent());  TEST_EQUAL(u2.str(), "https://www.example.com/foo/");
    TRY(u2 = u2.parent());  TEST_EQUAL(u2.str(), "https://www.example.com/");
    TRY(u2 = u2.parent());  TEST_EQUAL(u2.str(), "https://www.example.com/");

}

void test_crow_uri_encoding() {

    std::string s1, s2, s3;

    s1 = "";
    TRY(s2 = Uri::encode(s1));
    TEST_EQUAL(s2, "");
    TRY(s3 = Uri::decode(s2));
    TEST_EQUAL(s3, s1);

    s1 = "Hello world (%)";
    TRY(s2 = Uri::encode(s1));
    TEST_EQUAL(s2, "Hello%20world%20%28%25%29");
    TRY(s3 = Uri::decode(s2));
    TEST_EQUAL(s3, s1);

}

void test_crow_uri_query() {

    std::string query, s;
    std::vector<std::pair<std::string, std::string>> table;

    TRY(query = Uri::make_query(table));
    TEST_EQUAL(query, "");

    TRY(table = Uri::parse_query(""));
    TEST(table.empty());

    table = {
        { "abc", "123" },
        { "def", "456" },
        { "ghi", "789" },
        { "jkl", "" },
    };

    TRY(query = Uri::make_query(table));                       TEST_EQUAL(query, "abc=123&def=456&ghi=789&jkl=");
    TRY(query = Uri::make_query(table, ';'));                  TEST_EQUAL(query, "abc=123;def=456;ghi=789;jkl=");
    TRY(query = Uri::make_query(table, ';', Uri::lone_keys));  TEST_EQUAL(query, "abc=123;def=456;ghi=789;jkl");

    query = "abc=123&def=456&ghi=789&jkl=";
    TRY(table = Uri::parse_query(query));
    TEST_EQUAL(table.size(), 4u);
    TEST_EQUAL(format_map(table), "{abc:123,def:456,ghi:789,jkl:}");

    query = "abc=123;def=456;ghi=789;jkl";
    TRY(table = Uri::parse_query(query));
    TEST_EQUAL(table.size(), 4u);
    TEST_EQUAL(format_map(table), "{abc:123,def:456,ghi:789,jkl:}");

}
