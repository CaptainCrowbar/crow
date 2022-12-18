#include "crow/web-client.hpp"
#include "crow/http-status.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <string>

using namespace Crow;

void test_crow_web_client_http_get() {

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebHeaders head;
    WebHeaders::const_iterator it;
    std::string body, key;

    TEST(client.native_handle() != nullptr);
    TRY(uri =Uri("https://en.wikipedia.org/wiki/Main_Page"));
    TRY(status = client.http_get(uri, head, body));
    TEST_EQUAL(status, HttpStatus::ok);
    TEST_EQUAL(int(status), 200);
    TEST_EQUAL(to_string(status), "200 OK");
    TEST(head.size() > 20);
    TEST(body.size() > 10'000);
    TEST_MATCH(body.substr(0, 100), "^<!DOCTYPE html>\n");

    key = "content-length";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^\d+$)");

    key = "content-type";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, "^text/html");

    key = "date";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

    key = "last-modified";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

}

void test_crow_web_client_http_head() {

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebHeaders head;
    WebHeaders::const_iterator it;
    std::string key;

    TRY(uri =Uri("https://en.wikipedia.org/wiki/Main_Page"));
    TRY(status = client.http_head(uri, head));
    TEST_EQUAL(status, HttpStatus::ok);
    TEST_EQUAL(int(status), 200);
    TEST_EQUAL(to_string(status), "200 OK");
    TEST(head.size() > 20);

    key = "content-length";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^\d+$)");

    key = "content-type";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, "^text/html");

    key = "date";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

    key = "last-modified";
    TEST_EQUAL(head.count(key), 1u);
    TRY(it = head.find(key));
    TEST(it != head.end());
    if (it != head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

}
