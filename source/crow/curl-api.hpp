// This header contains only implementation details; it is not part of the
// public interface.

#pragma once

#include "crow/curl-utility.hpp"
#include <string>
#include <type_traits>
#include <utility>

#define CURL_NO_OLDIES
#include <curl/curl.h>

namespace Crow {

    namespace Detail {

        // This is all about statically checking the argument types for
        // curl_easy_getinfo() and curl_easy_setopt(), to the extent that this is
        // possible. The selector codes in the curl headers (CURLINFO and
        // CURLoption) are defined so that a type code can be extracted by
        // masking off the low digits. Just to make it a bit more complicated,
        // CURLINFO uses a binary mask while CURLoption uses a decimal one.

        template <typename T> struct IsFunctionPointer: std::false_type {};
        template <typename T> struct IsFunctionPointer<T*>: std::is_function<T> {};

        template <typename T> struct RemoveIndirectConst { using type = T; };
        template <typename T> struct RemoveIndirectConst<const T*> { using type = T*; };

        template <CURLINFO CI, typename T, int TypeMask = CI & CURLINFO_TYPEMASK>
            struct CurlCheckInfoType: std::false_type {};
        template <CURLoption CO, typename T, typename Ctype = typename RemoveIndirectConst<T>::type, int TypeMask = CO - CO % 10'000>
            struct CheckOptionType: std::false_type {};

        // We have to use variadic macros here because the preprocessor doesn't
        // understand templates, so macro arguments can't have commas in them.

        #define CROW_CURL_CHECK_INFO_TYPE(TypeMask, ParameterType, ...) \
            template <CURLINFO CI, typename T> \
            struct CurlCheckInfoType<CI, T, TypeMask> { \
                using type = ParameterType; \
                static constexpr bool value = __VA_ARGS__; \
            };

        #define CROW_CURL_CHECK_OPTION_TYPE(TypeMask, ParameterType, ...) \
            template <CURLoption CO, typename T, typename Ctype> \
            struct CheckOptionType<CO, T, Ctype, TypeMask> { \
                using type = ParameterType; \
                static constexpr bool value = __VA_ARGS__; \
            };

        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_STRING,            T,       std::is_same_v<T, char*>)
        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_LONG,              long,    std::is_integral_v<T>)
        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_DOUBLE,            double,  std::is_floating_point_v<T>)
        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_PTR,               T,       std::is_pointer_v<T>)
        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_SOCKET,            T,       std::is_same_v<T, curl_socket_t>)
        CROW_CURL_CHECK_INFO_TYPE    (CURLINFO_OFF_T,             T,       std::is_same_v<T, curl_off_t>)
        CROW_CURL_CHECK_OPTION_TYPE  (CURLOPTTYPE_LONG,           long,    std::is_integral_v<T>)
        CROW_CURL_CHECK_OPTION_TYPE  (CURLOPTTYPE_OBJECTPOINT,    T,       std::is_pointer_v<T>)
        CROW_CURL_CHECK_OPTION_TYPE  (CURLOPTTYPE_FUNCTIONPOINT,  T,       IsFunctionPointer<T>::value)
        CROW_CURL_CHECK_OPTION_TYPE  (CURLOPTTYPE_OFF_T,          T,       std::is_same_v<T, curl_off_t>)
        CROW_CURL_CHECK_OPTION_TYPE  (CURLOPTTYPE_BLOB,           T,       std::is_same_v<Ctype, curl_blob*>)

        template <typename T> using CurlLookupFunction = const char* (*)(T);

        template <typename C, typename... Args>
        inline CURLcode check_curl_api(const C& client, CURLcode rc, const char* function, Args&&... args) {
            if (rc != CURLE_OK)
                throw CurlError(int(rc), function, web_client_error_buffer(client), std::forward<Args>(args)...);
            return rc;
        }

        template <typename C, typename T, typename... Args>
        inline CURLcode check_curl_lookup(const C& client, CURLcode rc, const char* function, T t, CurlLookupFunction<T> f, Args&&... args) {
            if (rc != CURLE_OK)
                throw CurlError(int(rc), function, web_client_error_buffer(client), f(t), std::forward<Args>(args)...);
            return rc;
        }

        const char* get_curl_info_name(CURLINFO ci) noexcept;

        inline const char* get_curl_option_name(CURLoption co) noexcept {
            auto info = curl_easy_option_by_id(co);
            return info == nullptr ? nullptr : info->name;
        }

        template <CURLINFO CI, typename C, typename T>
        void get_curl_info(const C& client, T& result) {
            static_assert(CurlCheckInfoType<CI, T>::value, "Wrong argument type for curl_easy_getinfo()");
            using U = typename CurlCheckInfoType<CI, T>::type;
            U temp_result = {};
            check_curl_lookup(client, curl_easy_getinfo(client.native_handle(), CI, &temp_result), "curl_easy_getinfo()", CI, Detail::get_curl_info_name);
            result = temp_result;
        }

        template <CURLINFO CI, typename C>
        void get_curl_info(const C& client, std::string& result) {
            static_assert(CurlCheckInfoType<CI, char*>::value, "Wrong argument type for curl_easy_getinfo()");
            char* cptr = nullptr;
            get_curl_info<CI>(client, cptr);
            result = cptr == nullptr ? "" : cptr;
        }

        template <CURLoption CO, typename C, typename T>
        void set_curl_option(const C& client, T arg) {
            static_assert(CheckOptionType<CO, T>::value, "Wrong argument type for curl_easy_setopt()");
            using U = typename CheckOptionType<CO, T>::type;
            check_curl_lookup(client, curl_easy_setopt(client.native_handle(), CO, U(arg)), "curl_easy_setopt()", CO, get_curl_option_name);
        }

        template <CURLoption CO, typename C>
        void set_curl_option(const C& client, const std::string& arg) {
            static_assert(CheckOptionType<CO, const char*>::value, "Wrong argument type for curl_easy_setopt()");
            set_curl_option<CO>(client, arg.data());
        }

    }

}
