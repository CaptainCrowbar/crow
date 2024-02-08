#include "crow/rest-client.hpp"
#include "crow/http-status.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <nlohmann/json.hpp>
#include <string>

using namespace Crow;

using nlohmann::json;

namespace {

    const Uri server("https://api.guildwars2.com/v2/");

}

void test_crow_rest_client() {

    RestClient client(server);
    RestClient::result result;
    std::string s;

    TRY(result = client("mounts/types/skyscale"));

    TEST_EQUAL(result.status, HttpStatus::ok);
    TEST(result.valid);
    TEST(! result.text.empty());
    TEST(! result.json.empty());

    TRY(s = result.json["id"]);    TEST_EQUAL(s, "skyscale");
    TRY(s = result.json["name"]);  TEST_EQUAL(s, "Skyscale");

}
