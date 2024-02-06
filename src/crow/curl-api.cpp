#include "crow/curl-api.hpp"
#include <algorithm>
#include <array>
#include <mutex>
#include <vector>

namespace Crow {

    std::string CurlError::message(int error, const std::string& function, const std::vector<std::string>& extra) {
        std::string msg = "Libcurl error";
        if (error != 0)
            msg += " " + std::to_string(error);
        msg += " in " + function;
        if (error != 0)
            msg += "\n" + std::string(curl_easy_strerror(CURLcode(error)));
        for (auto& ex: extra)
            if (! ex.empty())
                msg += "\n" + ex;
        return msg;
    }

    namespace Detail {

        CurlInit::CurlInit() {
            static std::mutex mutex;
            static bool done = false;
            auto lock = std::unique_lock(mutex);
            if (! done) {
                auto rc = curl_global_init(CURL_GLOBAL_ALL);
                if (rc != CURLE_OK)
                    throw CurlError(int(rc), "curl_global_init()");
                done = true;
            }
        }

        #define MAKE_INFO(ci) std::make_pair(ci, # ci)

        const char* get_curl_info_name(CURLINFO ci) noexcept {

            static const auto map = std::array{

                MAKE_INFO(CURLINFO_NONE),
                MAKE_INFO(CURLINFO_EFFECTIVE_URL),
                MAKE_INFO(CURLINFO_RESPONSE_CODE),
                MAKE_INFO(CURLINFO_TOTAL_TIME),
                MAKE_INFO(CURLINFO_NAMELOOKUP_TIME),
                MAKE_INFO(CURLINFO_CONNECT_TIME),
                MAKE_INFO(CURLINFO_PRETRANSFER_TIME),
                MAKE_INFO(CURLINFO_SIZE_UPLOAD),
                MAKE_INFO(CURLINFO_SIZE_UPLOAD_T),
                MAKE_INFO(CURLINFO_SIZE_DOWNLOAD),
                MAKE_INFO(CURLINFO_SIZE_DOWNLOAD_T),
                MAKE_INFO(CURLINFO_SPEED_DOWNLOAD),
                MAKE_INFO(CURLINFO_SPEED_DOWNLOAD_T),
                MAKE_INFO(CURLINFO_SPEED_UPLOAD),
                MAKE_INFO(CURLINFO_SPEED_UPLOAD_T),
                MAKE_INFO(CURLINFO_HEADER_SIZE),
                MAKE_INFO(CURLINFO_REQUEST_SIZE),
                MAKE_INFO(CURLINFO_SSL_VERIFYRESULT),
                MAKE_INFO(CURLINFO_FILETIME),
                MAKE_INFO(CURLINFO_FILETIME_T),
                MAKE_INFO(CURLINFO_CONTENT_LENGTH_DOWNLOAD),
                MAKE_INFO(CURLINFO_CONTENT_LENGTH_DOWNLOAD_T),
                MAKE_INFO(CURLINFO_CONTENT_LENGTH_UPLOAD),
                MAKE_INFO(CURLINFO_CONTENT_LENGTH_UPLOAD_T),
                MAKE_INFO(CURLINFO_STARTTRANSFER_TIME),
                MAKE_INFO(CURLINFO_CONTENT_TYPE),
                MAKE_INFO(CURLINFO_REDIRECT_TIME),
                MAKE_INFO(CURLINFO_REDIRECT_COUNT),
                MAKE_INFO(CURLINFO_PRIVATE),
                MAKE_INFO(CURLINFO_HTTP_CONNECTCODE),
                MAKE_INFO(CURLINFO_HTTPAUTH_AVAIL),
                MAKE_INFO(CURLINFO_PROXYAUTH_AVAIL),
                MAKE_INFO(CURLINFO_OS_ERRNO),
                MAKE_INFO(CURLINFO_NUM_CONNECTS),
                MAKE_INFO(CURLINFO_SSL_ENGINES),
                MAKE_INFO(CURLINFO_COOKIELIST),
                MAKE_INFO(CURLINFO_LASTSOCKET),
                MAKE_INFO(CURLINFO_FTP_ENTRY_PATH),
                MAKE_INFO(CURLINFO_REDIRECT_URL),
                MAKE_INFO(CURLINFO_PRIMARY_IP),
                MAKE_INFO(CURLINFO_APPCONNECT_TIME),
                MAKE_INFO(CURLINFO_CERTINFO),
                MAKE_INFO(CURLINFO_CONDITION_UNMET),
                MAKE_INFO(CURLINFO_RTSP_SESSION_ID),
                MAKE_INFO(CURLINFO_RTSP_CLIENT_CSEQ),
                MAKE_INFO(CURLINFO_RTSP_SERVER_CSEQ),
                MAKE_INFO(CURLINFO_RTSP_CSEQ_RECV),
                MAKE_INFO(CURLINFO_PRIMARY_PORT),
                MAKE_INFO(CURLINFO_LOCAL_IP),
                MAKE_INFO(CURLINFO_LOCAL_PORT),
                MAKE_INFO(CURLINFO_TLS_SESSION),
                MAKE_INFO(CURLINFO_ACTIVESOCKET),
                MAKE_INFO(CURLINFO_TLS_SSL_PTR),
                MAKE_INFO(CURLINFO_HTTP_VERSION),
                MAKE_INFO(CURLINFO_PROXY_SSL_VERIFYRESULT),
                MAKE_INFO(CURLINFO_PROTOCOL),
                MAKE_INFO(CURLINFO_SCHEME),
                MAKE_INFO(CURLINFO_TOTAL_TIME_T),
                MAKE_INFO(CURLINFO_NAMELOOKUP_TIME_T),
                MAKE_INFO(CURLINFO_CONNECT_TIME_T),
                MAKE_INFO(CURLINFO_PRETRANSFER_TIME_T),
                MAKE_INFO(CURLINFO_STARTTRANSFER_TIME_T),
                MAKE_INFO(CURLINFO_REDIRECT_TIME_T),
                MAKE_INFO(CURLINFO_APPCONNECT_TIME_T),
                MAKE_INFO(CURLINFO_RETRY_AFTER),
                MAKE_INFO(CURLINFO_EFFECTIVE_METHOD),
                MAKE_INFO(CURLINFO_PROXY_ERROR),
                MAKE_INFO(CURLINFO_REFERER),

            };

            static constexpr const char* null = nullptr;
            auto key = std::make_pair(ci, null);
            auto it = std::upper_bound(map.begin(), map.end(), key);

            if (it == map.begin())
                return nullptr;

            --it;

            if (it->first == ci)
                return it->second;
            else
                return nullptr;

        }

    }

}
