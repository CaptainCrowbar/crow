#include "crow/web-client.hpp"
#include "crow/http-status.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <string>

using namespace Crow;

void test_crow_web_client_http_head() {

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebParameters response;
    WebHeaders::const_iterator it;
    std::string key;

    TRY(uri =Uri("https://en.wikipedia.org/wiki/Main_Page"));
    TRY(status = client.request(uri, response, WebMethod::head));
    TEST_EQUAL(status, HttpStatus::ok);
    TEST_EQUAL(int(status), 200);
    TEST_EQUAL(to_string(status), "200 OK");
    TEST(response.head.size() > 20);

    key = "content-type";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, "^text/html");

    key = "date";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

    key = "last-modified";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

}

void test_crow_web_client_http_get() {

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebParameters response;
    WebHeaders::const_iterator it;
    std::string key;

    TEST(client.native_handle() != nullptr);
    TRY(uri =Uri("https://en.wikipedia.org/wiki/Main_Page"));
    TRY(status = client.request(uri, response));
    TEST_EQUAL(status, HttpStatus::ok);
    TEST_EQUAL(int(status), 200);
    TEST_EQUAL(to_string(status), "200 OK");

    TEST(response.head.size() > 20);
    TEST(response.body.size() > 10'000);
    TEST_MATCH(response.body.substr(0, 100), "^<!DOCTYPE html>\n");

    key = "content-type";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, "^text/html");

    key = "date";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

    key = "last-modified";
    TEST(response.head.contains(key));
    TRY(it = response.head.find(key));
    TEST(it != response.head.end());
    if (it != response.head.end())
        TEST_MATCH(it->second, R"(^[A-Z][a-z]{2}, \d\d [A-Z][a-z]{2} 20\d\d \d\d:\d\d:\d\d GMT$)");

}
