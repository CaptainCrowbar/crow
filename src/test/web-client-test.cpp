#include "crow/web-client.hpp"
#include "crow/http-status.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <nlohmann/json.hpp>
#include <string>

using namespace Crow;

using nlohmann::json;

void test_crow_web_client_http_head() {

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebClient::parameters response;
    WebClient::headers::const_iterator it;
    std::string key;

    TRY(uri = Uri("https://en.wikipedia.org/wiki/Main_Page"));
    TRY(status = client.request(uri, response, WebClient::method::head));
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
    WebClient::parameters response;
    WebClient::headers::const_iterator it;
    std::string key;

    TEST(client.native_handle() != nullptr);
    TRY(uri = Uri("https://en.wikipedia.org/wiki/Main_Page"));
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

void test_crow_web_client_rest_api() {

    const Uri server("https://api.guildwars2.com/v2/");

    HttpStatus status = {};
    Uri uri;
    WebClient client;
    WebClient::parameters response;
    nlohmann::json json;
    std::string value;

    TRY(uri = server / "mounts/types/skyscale");
    TRY(status = client.request(uri, response));
    TEST_EQUAL(status, HttpStatus::ok);

    TRY(json = nlohmann::json::parse(response.body));
    TEST(! json.empty());
    TEST(! json.is_discarded());

    TRY(value = json["id"]);    TEST_EQUAL(value, "skyscale");
    TRY(value = json["name"]);  TEST_EQUAL(value, "Skyscale");

}
