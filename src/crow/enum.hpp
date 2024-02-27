#pragma once

#include <concepts>
#include <cstddef>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Crow {

    template <typename T>
    concept EnumType = std::is_enum_v<T>;

    template <typename T>
    concept UnscopedEnumType = EnumType<T> && std::convertible_to<T, std::underlying_type_t<T>>;

    template <typename T>
    concept ScopedEnumType = EnumType<T> && ! UnscopedEnumType<T>;

    namespace EnumHelper {

        inline bool char_in(char c, std::string_view ranges) noexcept {
            for (std::size_t i = 0; i < ranges.size(); i += 2)
                if (c >= ranges[i] && c <= ranges[i + 1])
                    return true;
            return false;
        }

        inline std::string_view trim(std::string_view str) noexcept {
            static constexpr std::string_view whitespace = "\t\n\r ";
            auto i = str.find_first_not_of(whitespace);
            if (i == std::string::npos)
                return {};
            auto j = str.find_last_not_of(whitespace);
            return str.substr(i, j - i + 1);
        }

        inline std::vector<std::string_view> split(std::string_view str, char delimiter) {

            std::vector<std::string_view> vec;
            std::size_t i = 0;

            while (i < str.size()) {

                auto j = str.find(delimiter, i);

                if (j == std::string::npos) {
                    auto sub = trim(str.substr(i, std::string::npos));
                    if (! sub.empty())
                        vec.push_back(sub);
                    break;
                }

                auto sub = trim(str.substr(i, j - i));
                vec.push_back(sub);
                i = j + 1;

            }

            return vec;

        }

        template <std::integral T>
        std::optional<T> parse_integer(std::string_view str) {

            // Parse a possibly signed integer literal, emulating the
            // behaviour of the C++ parser. Any alphanumeric suffix is
            // ignored. We can't use strtoll() or from_chars() here because
            // neither of them understands apostrophe grouping or the 0b
            // prefix. We don't bother with range checking here because we
            // assume the compiler already did that when it parsed the enum
            // definition.

            if (str.empty())
                return {};

            bool negative = str[0] == '-';

            if constexpr (std::unsigned_integral<T>)
                if (negative)
                    return {};

            std::size_t pos = 0;
            T base = 10;
            T digit = 0;
            T result = 0;

            if (str.size() >= 2 && (str[0] == '+' || str[0] == '-'))
                ++pos;

            if (str.size() >= 2 && str[pos] == '0') {
                switch (str[pos + 1]) {
                    case 'B': case 'b':  base = 2; pos += 2; break;
                    case 'X': case 'x':  base = 16; pos += 2; break;
                    default:             base = 8; ++pos; break;
                }
            }

            while (pos < str.size()) {
                if (char_in(str[pos], "09"))
                    digit = static_cast<T>(str[pos] - '0');
                else if (base == 16 && char_in(str[pos], "AF"))
                    digit = static_cast<T>(str[pos] - 'A' + 10);
                else if (base == 16 && char_in(str[pos], "af"))
                    digit = static_cast<T>(str[pos] - 'a' + 10);
                else if (str[pos] != '\'')
                    break;
                if (digit >= base)
                    break;
                if (str[pos] != '\'')
                    result = result * base + digit;
                ++pos;
            }

            if (pos < str.size() && char_in(str[pos], "AZaz__")) {
                ++pos;
                while (pos < str.size() && char_in(str[pos], "09AZaz__"))
                    ++pos;
            }

            if (pos < str.size())
                return {};

            if constexpr (std::signed_integral<T>)
                if (negative)
                    result = - result;

            return result;

        }

        template <std::integral T>
        std::optional<T> evaluate_token(std::string_view str, const std::unordered_map<std::string_view, T>& symbol_table) {
            auto it = symbol_table.find(str);
            if (it == symbol_table.end())
                return parse_integer<T>(str);
            else
                return it->second;
        }

        template <std::integral T>
        T evaluate(std::string_view str, const std::unordered_map<std::string_view, T>& symbol_table) {

            // Formats recognised:
            //     [+-]? <int> = integer literal
            //     <name> = name already defined
            //     <name> [+-] <int>
            //     (<int>|<name>) "<<" <int>
            //     (<int>|<name>) "|" (<int>|<name>) ...

            auto check = [str] (std::optional<T> t) {
                if (t) {
                    return *t;
                } else {
                    std::string message = "Arithmetic expression is not supported: ";
                    message += str;
                    throw std::invalid_argument(message);
                }
            };

            auto add_pos = str.find_first_of("+-", 1);

            if (add_pos != std::string::npos) {
                auto left = trim(str.substr(0, add_pos));
                auto right = trim(str.substr(add_pos + 1));
                auto x = check(evaluate_token(left, symbol_table));
                auto y = check(evaluate_token(right, symbol_table));
                if (str[add_pos] == '+')
                    x += y;
                else
                    x -= y;
                return x;
            }

            auto shift_pos = str.find("<<");

            if (shift_pos != std::string::npos) {
                auto left = trim(str.substr(0, shift_pos));
                auto right = trim(str.substr(shift_pos + 2));
                auto x = check(evaluate_token(left, symbol_table));
                auto y = check(evaluate_token(right, symbol_table));
                return x << static_cast<int>(y);
            }

            auto or_list = split(str, '|');

            if (or_list.size() > 1) {
                T sum = 0;
                for (auto& token: or_list)
                    sum |= check(evaluate_token(token, symbol_table));
                return sum;
            }

            return check(evaluate_token(str, symbol_table));

        }

        template <EnumType E>
        auto get_enum_value_name_vector(std::string_view all_names) {

            // Use long long or unsigned long long for evaluation to avoid
            // multiple redundant instantiations of the parsing functions.

            using IntType = std::underlying_type_t<E>;
            using WorkingType = std::conditional_t<std::signed_integral<IntType>,
                long long, unsigned long long>;

            auto items = split(all_names, ',');
            std::optional<WorkingType> value;
            std::unordered_map<std::string_view, WorkingType> symbol_table;
            std::vector<std::pair<E, std::string>> enum_vec;

            for (const auto& item: items) {

                auto eq_pos = item.find('=');
                std::string_view name;

                if (eq_pos != std::string::npos) {
                    name = item.substr(0, eq_pos);
                    auto value_str = trim(item.substr(eq_pos + 1));
                    value = evaluate<WorkingType>(value_str, symbol_table);
                } else {
                    name = item;
                    if (value.has_value())
                        ++*value;
                    else
                        value = 0;
                }

                name = trim(name);
                symbol_table[name] = *value;
                auto enum_value = static_cast<E>(static_cast<IntType>(*value));
                std::string name_str(name);
                enum_vec.push_back({enum_value, name_str});

            }

            return enum_vec;

        }

        template <EnumType E>
        auto get_enum_value_name_map(const std::vector<std::pair<E, std::string>>& value_name_vector) {
            std::unordered_map<E, std::string> value_name_map;
            for (const auto& [value,name]: value_name_vector)
                value_name_map.insert({value, name});
            return value_name_map;
        }

        template <EnumType E>
        auto get_enum_name_value_map(const std::vector<std::pair<E, std::string>>& value_name_vector) {
            std::unordered_map<std::string, E> name_value_map;
            for (const auto& [value,name]: value_name_vector)
                name_value_map.insert({name, value});
            return name_value_map;
        }

    }

}

#define CROW_ENUM_IMPLEMENTATION_(EnumName, Keyword, IntType, ...) \
\
    enum Keyword EnumName: IntType { __VA_ARGS__ }; \
\
    [[maybe_unused]] inline std::vector<std::pair<EnumName, std::string>> \
            enum_value_name_vector(EnumName) { \
        using namespace ::Crow::EnumHelper; \
        return get_enum_value_name_vector<EnumName>(# __VA_ARGS__); \
    } \
\
    [[maybe_unused]] inline std::unordered_map<EnumName, std::string> \
            enum_value_name_map(EnumName) { \
        using namespace ::Crow::EnumHelper; \
        return get_enum_value_name_map<EnumName>(enum_value_name_vector(EnumName())); \
    } \
\
    [[maybe_unused]] inline std::unordered_map<std::string, EnumName> \
            enum_name_value_map(EnumName) { \
        using namespace ::Crow::EnumHelper; \
        return get_enum_name_value_map<EnumName>(enum_value_name_vector(EnumName())); \
    } \
\
    [[maybe_unused]] inline std::string to_string(EnumName value) { \
        static const auto& map = enum_value_name_map(EnumName()); \
        auto iter = map.find(value); \
        if (iter == map.end()) \
            return std::to_string(static_cast<IntType>(value)); \
        else \
            return static_cast<std::string>(iter->second); \
    } \
\
    [[maybe_unused]] inline std::ostream& operator<<(std::ostream& out, EnumName x) { \
        return out << to_string(x); \
    } \
\
    [[maybe_unused]] inline bool enum_value(const std::string& name, EnumName& value) { \
        static const auto& map = enum_name_value_map(EnumName()); \
        auto iter = map.find(name); \
        if (iter == map.end()) \
            return false; \
        value = iter->second; \
        return true; \
    }

#define CROW_ENUM_UNSCOPED(EnumName, IntType, ...) \
    CROW_ENUM_IMPLEMENTATION_(EnumName,, IntType, __VA_ARGS__)

#define CROW_ENUM_SCOPED(EnumName, IntType, ...) \
    CROW_ENUM_IMPLEMENTATION_(EnumName, class, IntType, __VA_ARGS__)

#define CROW_ENUM_BITMASK_OPERATORS(EnumName) \
    [[maybe_unused]] constexpr bool operator!(EnumName x) noexcept { return ! static_cast<bool>(x); } \
    [[maybe_unused]] constexpr EnumName operator~(EnumName x) noexcept { \
        using U = std::underlying_type_t<EnumName>; \
        return static_cast<EnumName>(~ static_cast<U>(x)); \
    } \
    [[maybe_unused]] constexpr EnumName operator&(EnumName x, EnumName y) noexcept { \
        using U = std::underlying_type_t<EnumName>; \
        return static_cast<EnumName>(static_cast<U>(x) & static_cast<U>(y)); \
    } \
    [[maybe_unused]] constexpr EnumName operator|(EnumName x, EnumName y) noexcept { \
        using U = std::underlying_type_t<EnumName>; \
        return static_cast<EnumName>(static_cast<U>(x) | static_cast<U>(y)); \
    } \
    [[maybe_unused]] constexpr EnumName operator^(EnumName x, EnumName y) noexcept { \
        using U = std::underlying_type_t<EnumName>; \
        return static_cast<EnumName>(static_cast<U>(x) ^ static_cast<U>(y)); \
    } \
    [[maybe_unused]] constexpr EnumName& operator&=(EnumName& x, EnumName y) noexcept { return x = x & y; } \
    [[maybe_unused]] constexpr EnumName& operator|=(EnumName& x, EnumName y) noexcept { return x = x | y; } \
    [[maybe_unused]] constexpr EnumName& operator^=(EnumName& x, EnumName y) noexcept { return x = x ^ y; }

#define CROW_ENUM_BITMASK(EnumName, IntType, ...) \
    CROW_ENUM_IMPLEMENTATION_(EnumName, class, IntType, __VA_ARGS__) \
    CROW_ENUM_BITMASK_OPERATORS(EnumName)
