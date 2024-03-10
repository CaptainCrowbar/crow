#pragma once

#include "crow/curl-utility.hpp"
#include "crow/http.hpp"
#include "crow/stable-map.hpp"
#include "crow/types.hpp"
#include "crow/uri.hpp"
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <utility>

namespace Crow {

    class WebClient:
    private Detail::CurlInit {

    public:

        using headers = StableMap<std::string, std::string>;

        enum class method: int {
            del,
            get,
            head,
            post,
            put,
        };

        struct parameters {
            headers head;
            std::string body;
            parameters() = default;
            parameters(const headers& h): head(h), body() {}
            parameters(const std::string& b): head(), body(b) {}
            parameters(const headers& h, const std::string& b): head(h), body(b) {}
            void clear() noexcept { head.clear(); body.clear(); }
        };

        class progress {
        public:
            using callback = std::function<bool(int64_t dltotal, int64_t dlnow)>;
            progress(WebClient& c, callback on_download);
            ~progress() noexcept;
            progress(const progress&) = delete;
            progress(progress&&) = delete;
            progress& operator=(const progress&) = delete;
            progress& operator=(progress&&) = delete;
        private:
            WebClient& client_;
            callback on_download_;
            static int progress_callback(progress* ptr, int64_t dl_total, int64_t dl_now,
                int64_t ul_total, int64_t ul_now) noexcept;
        };

        static constexpr auto default_connect_timeout = std::chrono::seconds(15);
        static constexpr auto default_request_timeout = std::chrono::seconds(60);
        static constexpr int default_redirect_limit = 10;

        WebClient();
        ~WebClient() noexcept { close(); }
        WebClient(const WebClient& c) = delete;
        WebClient(WebClient&& c) noexcept: curl_(std::exchange(c.curl_, nullptr)) {}
        WebClient& operator=(const WebClient& c) = delete;
        WebClient& operator=(WebClient&& c) noexcept;

        HttpStatus request(const Uri& uri, parameters& response,
            method m = method::get, const parameters& params = {});
        HttpStatus operator()(const Uri& uri, parameters& response,
            method m = method::get, const parameters& params = {});

        template <typename R, typename P> void set_connect_timeout(std::chrono::duration<R, P> t);
        template <typename R, typename P> void set_request_timeout(std::chrono::duration<R, P> t);
        void set_redirect_limit(int n);
        void set_user_agent(const std::string& user_agent);
        void set_verbose(bool flag);

        const std::string& native_error() const noexcept { return error_buffer_; }
        Curl_easy* native_handle() const noexcept { return curl_; }

    private:

        Curl_easy* curl_ = nullptr;
        parameters* response_ = nullptr;
        std::string error_buffer_;
        headers::iterator prev_header_;

        void close() noexcept;
        void set_connect_timeout_ms(std::chrono::milliseconds ms);
        void set_request_timeout_ms(std::chrono::milliseconds ms);

        static size_t header_callback(char* buffer, size_t size, size_t n_items, WebClient* client_ptr);
        static size_t write_callback(char* ptr, size_t size, size_t n_members, WebClient* client_ptr);

    };

        template <typename R, typename P>
        void WebClient::set_request_timeout(std::chrono::duration<R, P> t) {
            using namespace std::chrono;
            set_request_timeout_ms(duration_cast<milliseconds>(t));
        }

        template <typename R, typename P>
        void WebClient::set_connect_timeout(std::chrono::duration<R, P> t) {
            using namespace std::chrono;
            set_connect_timeout_ms(duration_cast<milliseconds>(t));
        }

}
