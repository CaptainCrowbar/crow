#include "crow/rest-client.hpp"
#include "crow/encoding.hpp"

namespace Crow {

    void RestClient::basic_auth(const std::string& user, const std::string& password) {
        auto text = user + ':' + password;
        auto code = Base64().encode(text);
        auto auth = "Basic " + code;
        request_.head.insert({"Authorization", auth});
    }

    void RestClient::bearer_auth(const std::string& token) {
        auto auth = "Bearer " + token;
        request_.head.insert({"Authorization", auth});
    }

    void RestClient::no_auth() {
        request_.head.erase("Authorization");
    }

    RestClient::result RestClient::query(const std::string& path) {
        auto uri = server_ / path;
        result res;
        res.status = client_.request(uri, response_, WebClient::method::get, request_);
        if (res.status == HttpStatus::ok) {
            res.text = response_.body;
            auto json = nlohmann::json::parse(res.text, nullptr, false);
            res.valid = ! json.is_discarded();
            if (res.valid)
                res.json = std::move(json);
        }
        return res;
    }

}
