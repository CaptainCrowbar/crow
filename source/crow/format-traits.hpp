#pragma once

#include "crow/types.hpp"
#include <string>
#include <type_traits>
#include <utility>

namespace Crow::Detail {

    template <typename T, typename = void> struct HasAdlToStringFunction: std::false_type {};
    template <typename T> struct HasAdlToStringFunction<T,
        std::void_t<decltype(std::declval<std::string&>() = to_string(std::declval<const T&>()))>>: std::true_type {};
    template <typename T, typename = void> struct HasStdToStringFunction: std::false_type {};
    template <typename T> struct HasStdToStringFunction<T,
        std::void_t<decltype(std::declval<std::string&>() = std::to_string(std::declval<const T&>()))>>: std::true_type {};

    template <typename T> constexpr bool has_str_method = HasStrMethod<T>::value;
    template <typename T> constexpr bool has_adl_to_string_function = HasAdlToStringFunction<T>::value;
    template <typename T> constexpr bool has_std_to_string_function = HasStdToStringFunction<T>::value;
    template <typename T> constexpr bool has_output_operator = HasOutputOperator<T>::value;

}
