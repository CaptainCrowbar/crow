#include "crow/web-client.hpp"
#include "crow/curl-api.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <cstring>
#include <memory>
#include <vector>

namespace Crow {

    namespace {

        struct SlistDeleter {
            void operator()(curl_slist* ptr) const noexcept {
                if (ptr != nullptr)
                    curl_slist_free_all(ptr);
            }
        };

        using SlistPtr = std::unique_ptr<curl_slist, SlistDeleter>;

        SlistPtr make_slist(const std::vector<std::string>& head) {
            curl_slist* cs = nullptr;
            for (auto& h: head) {
                auto next = curl_slist_append(cs, h.data());
                if (next == nullptr) {
                    curl_slist_free_all(cs);
                    throw CurlError(CURLE_OUT_OF_MEMORY, "curl_slist_append()");
                }
                cs = next;
            }
            return {cs, {}};
        }

    }

    // WebClient class

    WebClient::WebClient() {

        auto curl = curl_easy_init();

        if (curl == nullptr)
            throw CurlError(0, "curl_easy_init()");

        curl_ = curl;
        error_buffer_.resize(CURL_ERROR_SIZE, '\0');

        Detail::set_curl_option<CURLOPT_ERRORBUFFER>(*this, error_buffer_.data());
        Detail::set_curl_option<CURLOPT_NOPROGRESS>(*this, true);
        Detail::set_curl_option<CURLOPT_NOSIGNAL>(*this, true);
        Detail::set_curl_option<CURLOPT_HEADERFUNCTION>(*this, header_callback);
        Detail::set_curl_option<CURLOPT_HEADERDATA>(*this, this);
        Detail::set_curl_option<CURLOPT_WRITEFUNCTION>(*this, write_callback);
        Detail::set_curl_option<CURLOPT_WRITEDATA>(*this, this);
        Detail::set_curl_option<CURLOPT_ACCEPT_ENCODING>(*this, "");

        set_connect_timeout(default_connect_timeout);
        set_request_timeout(default_request_timeout);
        set_redirect_limit(default_redirect_limit);

    }

    WebClient& WebClient::operator=(WebClient&& c) noexcept {
        if (&c != this) {
            close();
            curl_ = std::exchange(c.curl_, nullptr);
        }
        return *this;
    }

    HttpStatus WebClient::request(const Uri& uri, parameters& response,
            method m, const parameters& params) {

        // TODO - handle DELETE, POST, PUT

        response.clear();
        response_ = &response;
        Detail::set_curl_option<CURLOPT_URL>(*this, uri.str());
        std::vector<std::string> send_headers;
        SlistPtr slist_ptr;

        if (! params.head.empty()) {
            for (auto& [key,value]: params.head)
                send_headers.push_back(key + ": " + value);
            slist_ptr = make_slist(send_headers);
            Detail::set_curl_option<CURLOPT_HTTPHEADER>(*this, slist_ptr.get());
        }

        if (m == method::head)
            Detail::set_curl_option<CURLOPT_NOBODY>(*this, true);
        else
            Detail::set_curl_option<CURLOPT_HTTPGET>(*this, true);

        Detail::check_curl_api(*this, curl_easy_perform(curl_), "curl_easy_perform()", uri.str());
        response_ = nullptr;
        int rc = 0;
        Detail::get_curl_info<CURLINFO_RESPONSE_CODE>(*this, rc);

        return HttpStatus(rc);

    }

    HttpStatus WebClient::operator()(const Uri& uri, parameters& response,
            method m, const parameters& params) {
        return request(uri, response, m, params);
    }

    void WebClient::set_redirect_limit(int n) {
        Detail::set_curl_option<CURLOPT_FOLLOWLOCATION>(*this, true);
        Detail::set_curl_option<CURLOPT_MAXREDIRS>(*this, std::max(n, 0));
    }

    void WebClient::set_user_agent(const std::string& user_agent) {
        Detail::set_curl_option<CURLOPT_USERAGENT>(*this, user_agent);
    }

    void WebClient::set_verbose(bool flag) {
        Detail::set_curl_option<CURLOPT_VERBOSE>(*this, flag);
    }

    void WebClient::close() noexcept {
        if (curl_ != nullptr)
            curl_easy_cleanup(curl_);
    }

    void WebClient::set_connect_timeout_ms(std::chrono::milliseconds ms) {
        Detail::set_curl_option<CURLOPT_CONNECTTIMEOUT_MS>(*this, ms.count());
    }

    void WebClient::set_request_timeout_ms(std::chrono::milliseconds ms) {
        Detail::set_curl_option<CURLOPT_TIMEOUT_MS>(*this, ms.count());
    }

    size_t WebClient::header_callback(char* buffer, size_t /*size*/, size_t n_items, WebClient* client_ptr) {

        auto line = trim_right(std::string(buffer, n_items));

        if (line.empty())
            return n_items;

        auto& head = client_ptr->response_->head;
        auto& prev = client_ptr->prev_header_;

        if (! head.empty() && ascii_isspace(line[0])) {
            prev->second += ' ' + trim_left(line);
        } else {
            auto [key,value] = partition(line, ":");
            prev = head.insert({trim(key), trim_left(value)});
        }

        return n_items;

    }

    size_t WebClient::write_callback(char* ptr, size_t /*size*/, size_t n_members, WebClient* client_ptr) {
        auto& body = client_ptr->response_->body;
        size_t offset = body.size();
        body.resize(offset + n_members);
        std::memcpy(body.data() + offset, ptr, n_members);
        return n_members;
    }

    // WebClient::progress class

    WebClient::progress::progress(WebClient& c, callback on_download):
    client_(c), on_download_(on_download) {
        Detail::set_curl_option<CURLOPT_XFERINFOFUNCTION>(c, progress_callback);
        Detail::set_curl_option<CURLOPT_XFERINFODATA>(c, this);
        Detail::set_curl_option<CURLOPT_NOPROGRESS>(c, false);
    }

    WebClient::progress::~progress() noexcept {
        curl_easy_setopt(client_.native_handle(), CURLOPT_NOPROGRESS, 0L);
    }

    int WebClient::progress::progress_callback(progress* ptr, int64_t dl_total, int64_t dl_now,
            int64_t /*ul_total*/, int64_t /*ul_now*/) noexcept {
        bool dl_ok = ! ptr->on_download_ || ptr->on_download_(dl_total, dl_now);
        if (dl_ok)
            return CURL_PROGRESSFUNC_CONTINUE;
        else
            return CURL_PROGRESSFUNC_CONTINUE - 1;
    }

}
