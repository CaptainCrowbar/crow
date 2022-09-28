#include "crow/web-client.hpp"
#include "crow/curl-api.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>

namespace Crow {

    namespace Detail {

        const std::string* web_client_error_buffer(const WebClient& c) noexcept {
            if (c.error_buffer_.empty())
                return nullptr;
            else
                return &c.error_buffer_;
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

    void WebClient::set_redirect_limit(int n) {
        Detail::set_curl_option<CURLOPT_FOLLOWLOCATION>(*this, true);
        Detail::set_curl_option<CURLOPT_MAXREDIRS>(*this, std::max(n, 0));
    }

    void WebClient::set_user_agent(const std::string& user_agent) {
        Detail::set_curl_option<CURLOPT_USERAGENT>(*this, user_agent);
    }

    void WebClient::close() noexcept {
        if (curl_ != nullptr)
            curl_easy_cleanup(curl_);
    }

    HttpStatus WebClient::perform_get(const Uri& uri, WebHeaders* head, std::string* body) {
        head_ = head;
        body_ = body;
        if (head != nullptr)
            head->clear();
        if (body != nullptr)
            body->clear();
        Detail::set_curl_option<CURLOPT_URL>(*this, uri.str());
        if (body == nullptr)
            Detail::set_curl_option<CURLOPT_NOBODY>(*this, true);
        else
            Detail::set_curl_option<CURLOPT_HTTPGET>(*this, true);
        Detail::check_curl_api(*this, curl_easy_perform(curl_), "curl_easy_perform()", uri.str());
        body_ = nullptr;
        head_ = nullptr;
        int rc = 0;
        Detail::get_curl_info<CURLINFO_RESPONSE_CODE>(*this, rc);
        return HttpStatus(rc);
    }

    void WebClient::set_connect_timeout_ms(std::chrono::milliseconds ms) {
        Detail::set_curl_option<CURLOPT_CONNECTTIMEOUT_MS>(*this, ms.count());
    }

    void WebClient::set_request_timeout_ms(std::chrono::milliseconds ms) {
        Detail::set_curl_option<CURLOPT_TIMEOUT_MS>(*this, ms.count());
    }

    size_t WebClient::header_callback(char* buffer, size_t /*size*/, size_t n_items, WebClient* client_ptr) {
        if (n_items == 0 || client_ptr->head_ == nullptr)
            return n_items;
        std::string line(buffer, n_items);
        if (line == "\r\n")
            return n_items;
        auto& headers = *client_ptr->head_;
        auto& prev = client_ptr->prev_header_;
        if (! headers.empty() && ascii_isspace(line[0])) {
            prev->second += " " + trim(line);
            return n_items;
        }
        auto [key,value] = partition(line, ":");
        prev = headers.insert({trim(key), trim(value)});
        return n_items;
    }

    size_t WebClient::write_callback(char* ptr, size_t /*size*/, size_t n_members, WebClient* client_ptr) {
        if (client_ptr->body_ == nullptr)
            return n_members;
        auto& to = *client_ptr->body_;
        size_t offset = to.size();
        to.resize(offset + n_members);
        std::memcpy(to.data() + offset, ptr, n_members);
        return n_members;
    }

    // WebProgress class

    WebProgress::WebProgress(WebClient& c, callback on_download):
    client_(c), on_download_(on_download) {
        Detail::set_curl_option<CURLOPT_XFERINFOFUNCTION>(c, progress_callback);
        Detail::set_curl_option<CURLOPT_XFERINFODATA>(c, this);
        Detail::set_curl_option<CURLOPT_NOPROGRESS>(c, false);
    }

    WebProgress::~WebProgress() noexcept {
        curl_easy_setopt(client_.native_handle(), CURLOPT_NOPROGRESS, 0L);
    }

    int WebProgress::progress_callback(WebProgress* ptr, int64_t dl_total, int64_t dl_now,
            int64_t /*ul_total*/, int64_t /*ul_now*/) noexcept {
        bool dl_ok = ! ptr->on_download_ || ptr->on_download_(dl_total, dl_now);
        if (dl_ok)
            return CURL_PROGRESSFUNC_CONTINUE;
        else
            return CURL_PROGRESSFUNC_CONTINUE - 1;
    }

}
