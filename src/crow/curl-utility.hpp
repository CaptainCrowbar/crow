// Although some of the classes in here are public, this header itself is not
// part of the public interface. The public types are documented as part of
// web-client.hpp. The implementations are in curl-api.cpp

#pragma once

#include "crow/types.hpp"
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

struct Curl_easy;

namespace Crow {

    namespace Detail {

        class CurlInit {
        public:
            CurlInit();
        };

        template <typename T>
        std::string curl_error_string(T&& t) {
            static_assert(std::is_constructible_v<std::string, T>);
            return std::string(std::forward<T>(t));
        }

        template <typename T>
        std::string curl_error_string(T* t) {
            if (t == nullptr)
                return {};
            if constexpr (std::is_convertible_v<T*, std::string>)
                return std::string(t);
            else
                return curl_error_string(*t);
        }

    }

    class CurlError:
    public std::runtime_error {
    public:
        template <typename... Args>
            CurlError(int error, const std::string& function, Args&&... extra):
            std::runtime_error(message(error, function, {Detail::curl_error_string(std::forward<Args>(extra))...})), error_(error) {}
        int error() const noexcept { return error_; }
    private:
        int error_;
        static std::string message(int error, const std::string& function, const std::vector<std::string>& extra);
    };

}
