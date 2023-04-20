#pragma once

#include "crow/types.hpp"
#include <algorithm>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

#define CROW_ENUM_IMPL_(EnumType, enum_class, IntType, first_value, first_name, ...) \
    enum enum_class EnumType: IntType { \
        first_name = first_value, \
        __VA_ARGS__ \
    }; \
    enum class CROW_ ## EnumType ## _shadow_: IntType { \
        first_name = first_value, \
        __VA_ARGS__, \
        CROW_ ## EnumType ##_sentinel_ \
    }; \
    [[maybe_unused]] constexpr IntType count_enum_values(EnumType) noexcept { \
        constexpr auto f = static_cast<IntType>(first_value); \
        constexpr auto s = static_cast<IntType>(CROW_ ## EnumType ## _shadow_::CROW_ ## EnumType ##_sentinel_); \
        return s - f; \
    } \
    [[maybe_unused]] constexpr EnumType min_enum_value(EnumType) noexcept { \
        return EnumType(first_value); \
    } \
    [[maybe_unused]] constexpr EnumType max_enum_value(EnumType) noexcept { \
        constexpr auto s = static_cast<IntType>(CROW_ ## EnumType ## _shadow_::CROW_ ## EnumType ##_sentinel_); \
        return EnumType(s - 1); \
    } \
    [[maybe_unused]] inline const std::vector<std::string>& list_enum_names(EnumType) { \
        static const auto names = [] { \
            std::string all = # first_name "," # __VA_ARGS__; \
            std::vector<std::string> vec; \
            size_t i = 0, j = 0; \
            while (j < all.size()) { \
                i = all.find_first_not_of(" ,", j); \
                if (i == std::string::npos) \
                    break; \
                j = all.find_first_of(" ,", i); \
                vec.push_back(all.substr(i, j - i)); \
            } \
            return vec; \
        }(); \
        return names; \
    } \
    [[maybe_unused]] inline const std::vector<EnumType>& list_enum_values(EnumType) { \
        static const auto values = [] { \
            constexpr auto f = static_cast<IntType>(first_value); \
            constexpr auto n = count_enum_values(EnumType()); \
            std::vector<EnumType> vec(size_t(n), {}); \
            for (IntType i = 0; i < n; ++i) \
                vec[i] = EnumType(i + f); \
            return vec; \
        }(); \
        return values; \
    } \
    [[maybe_unused]] inline bool parse_enum(const std::string& str, EnumType& t) { \
        auto& names = list_enum_names(EnumType()); \
        auto it = std::find(names.begin(), names.end(), str); \
        if (it == names.end()) \
            return false; \
        t = EnumType(static_cast<IntType>(first_value) + static_cast<IntType>(it - names.begin())); \
        return true; \
    } \
    [[maybe_unused]] inline std::string to_string(EnumType t) { \
        auto& names = list_enum_names(EnumType()); \
        auto index = uintmax_t(t) - uintmax_t(first_value); \
        if (index < names.size()) \
            return names[index]; \
        else \
            return std::to_string(static_cast<IntType>(t)); \
    } \
    [[maybe_unused]] inline std::ostream& operator<<(std::ostream& out, EnumType t) { \
        return out << to_string(t); \
    }

#define CROW_ENUM(EnumType, IntType, first_value, first_name, ...) \
    CROW_ENUM_IMPL_(EnumType,, IntType, first_value, first_name, __VA_ARGS__)

#define CROW_ENUM_CLASS(EnumType, IntType, first_value, first_name, ...) \
    CROW_ENUM_IMPL_(EnumType, class, IntType, first_value, first_name, __VA_ARGS__)

#define CROW_BITMASK_OPERATORS(EnumClass) \
    [[maybe_unused]] constexpr bool operator!(EnumClass x) noexcept { return x == EnumClass(); } \
    [[maybe_unused]] constexpr EnumClass operator~(EnumClass x) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(~ U(x)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator&(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) & U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator|(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) | U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator^(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) ^ U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass& operator&=(EnumClass& x, EnumClass y) noexcept { return x = x & y; } \
    [[maybe_unused]] constexpr EnumClass& operator|=(EnumClass& x, EnumClass y) noexcept { return x = x | y; } \
    [[maybe_unused]] constexpr EnumClass& operator^=(EnumClass& x, EnumClass y) noexcept { return x = x ^ y; }
