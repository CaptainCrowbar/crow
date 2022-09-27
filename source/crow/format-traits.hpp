#pragma once

#include "crow/types.hpp"
#include <concepts>
#include <ostream>
#include <string>

namespace Crow::Detail {

    template <typename T>
    concept StrMethodType = requires (T t) {
        { t.str() } -> std::convertible_to<std::string>;
    };

    template <typename T>
    concept AdlToStringType = requires (T t) {
        { to_string(t) } -> std::convertible_to<std::string>;
    };

    template <typename T>
    concept StdToStringType = requires (T t) {
        { std::to_string(t) } -> std::convertible_to<std::string>;
    };

    template <typename T>
    concept OutputOperatorType = requires (T t, std::ostream& o) {
        o << t;
    };

}
