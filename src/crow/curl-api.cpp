#include "crow/curl-api.hpp"
#include <mutex>
#include <unordered_map>
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

        #define INFO_NAME(ci) { ci, # ci },

        const char* get_curl_info_name(CURLINFO ci) noexcept {

            static const std::unordered_map<CURLINFO, const char*> map = {

                INFO_NAME(CURLINFO_NONE)
                INFO_NAME(CURLINFO_EFFECTIVE_URL)
                INFO_NAME(CURLINFO_RESPONSE_CODE)
                INFO_NAME(CURLINFO_TOTAL_TIME)
                INFO_NAME(CURLINFO_NAMELOOKUP_TIME)
                INFO_NAME(CURLINFO_CONNECT_TIME)
                INFO_NAME(CURLINFO_PRETRANSFER_TIME)
                INFO_NAME(CURLINFO_SIZE_UPLOAD)
                INFO_NAME(CURLINFO_SIZE_UPLOAD_T)
                INFO_NAME(CURLINFO_SIZE_DOWNLOAD)
                INFO_NAME(CURLINFO_SIZE_DOWNLOAD_T)
                INFO_NAME(CURLINFO_SPEED_DOWNLOAD)
                INFO_NAME(CURLINFO_SPEED_DOWNLOAD_T)
                INFO_NAME(CURLINFO_SPEED_UPLOAD)
                INFO_NAME(CURLINFO_SPEED_UPLOAD_T)
                INFO_NAME(CURLINFO_HEADER_SIZE)
                INFO_NAME(CURLINFO_REQUEST_SIZE)
                INFO_NAME(CURLINFO_SSL_VERIFYRESULT)
                INFO_NAME(CURLINFO_FILETIME)
                INFO_NAME(CURLINFO_FILETIME_T)
                INFO_NAME(CURLINFO_CONTENT_LENGTH_DOWNLOAD)
                INFO_NAME(CURLINFO_CONTENT_LENGTH_DOWNLOAD_T)
                INFO_NAME(CURLINFO_CONTENT_LENGTH_UPLOAD)
                INFO_NAME(CURLINFO_CONTENT_LENGTH_UPLOAD_T)
                INFO_NAME(CURLINFO_STARTTRANSFER_TIME)
                INFO_NAME(CURLINFO_CONTENT_TYPE)
                INFO_NAME(CURLINFO_REDIRECT_TIME)
                INFO_NAME(CURLINFO_REDIRECT_COUNT)
                INFO_NAME(CURLINFO_PRIVATE)
                INFO_NAME(CURLINFO_HTTP_CONNECTCODE)
                INFO_NAME(CURLINFO_HTTPAUTH_AVAIL)
                INFO_NAME(CURLINFO_PROXYAUTH_AVAIL)
                INFO_NAME(CURLINFO_OS_ERRNO)
                INFO_NAME(CURLINFO_NUM_CONNECTS)
                INFO_NAME(CURLINFO_SSL_ENGINES)
                INFO_NAME(CURLINFO_COOKIELIST)
                INFO_NAME(CURLINFO_LASTSOCKET)
                INFO_NAME(CURLINFO_FTP_ENTRY_PATH)
                INFO_NAME(CURLINFO_REDIRECT_URL)
                INFO_NAME(CURLINFO_PRIMARY_IP)
                INFO_NAME(CURLINFO_APPCONNECT_TIME)
                INFO_NAME(CURLINFO_CERTINFO)
                INFO_NAME(CURLINFO_CONDITION_UNMET)
                INFO_NAME(CURLINFO_RTSP_SESSION_ID)
                INFO_NAME(CURLINFO_RTSP_CLIENT_CSEQ)
                INFO_NAME(CURLINFO_RTSP_SERVER_CSEQ)
                INFO_NAME(CURLINFO_RTSP_CSEQ_RECV)
                INFO_NAME(CURLINFO_PRIMARY_PORT)
                INFO_NAME(CURLINFO_LOCAL_IP)
                INFO_NAME(CURLINFO_LOCAL_PORT)
                INFO_NAME(CURLINFO_TLS_SESSION)
                INFO_NAME(CURLINFO_ACTIVESOCKET)
                INFO_NAME(CURLINFO_TLS_SSL_PTR)
                INFO_NAME(CURLINFO_HTTP_VERSION)
                INFO_NAME(CURLINFO_PROXY_SSL_VERIFYRESULT)
                INFO_NAME(CURLINFO_PROTOCOL)
                INFO_NAME(CURLINFO_SCHEME)
                INFO_NAME(CURLINFO_TOTAL_TIME_T)
                INFO_NAME(CURLINFO_NAMELOOKUP_TIME_T)
                INFO_NAME(CURLINFO_CONNECT_TIME_T)
                INFO_NAME(CURLINFO_PRETRANSFER_TIME_T)
                INFO_NAME(CURLINFO_STARTTRANSFER_TIME_T)
                INFO_NAME(CURLINFO_REDIRECT_TIME_T)
                INFO_NAME(CURLINFO_APPCONNECT_TIME_T)
                INFO_NAME(CURLINFO_RETRY_AFTER)
                INFO_NAME(CURLINFO_EFFECTIVE_METHOD)
                INFO_NAME(CURLINFO_PROXY_ERROR)
                INFO_NAME(CURLINFO_REFERER)

            };

            auto it = map.find(ci);
            return it == map.end() ? nullptr : it->second;

        }

    }

}
