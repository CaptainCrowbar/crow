#pragma once

#include "crow/http-status.hpp"
#include "crow/types.hpp"
#include "crow/uri.hpp"
#include "crow/web-client.hpp"
#include <nlohmann/json.hpp>
#include <string>

namespace Crow {

    class RestClient {

    public:

        struct result {
            HttpStatus status = HttpStatus::none;
            std::string text;
            nlohmann::json json;
            bool valid = false;
        };

        explicit RestClient(const Uri& server): server_(server) {}
        ~RestClient() = default;

        RestClient(const RestClient&) = delete;
        RestClient(RestClient&&) = default;
        RestClient& operator=(const RestClient&) = delete;
        RestClient& operator=(RestClient&&) = default;

        void basic_auth(const std::string& user, const std::string& password);
        void bearer_auth(const std::string& token);
        void no_auth();

        result query(const std::string& path = {});
        result operator()(const std::string& path = {}) { return query(path); }

    private:

        Uri server_;
        WebClient client_;
        WebClient::parameters request_;
        WebClient::parameters response_;

    };

}
