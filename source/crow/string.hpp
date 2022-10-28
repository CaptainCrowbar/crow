#pragma once

#include "crow/types.hpp"
#include <concepts>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

namespace Crow {

    // Constants

    constexpr const char* ascii_whitespace = "\t\n\f\r ";

    // Character functions

    constexpr bool ascii_isupper(char c) noexcept { return c >= 'A' && c <= 'Z'; }
    constexpr bool ascii_islower(char c) noexcept { return c >= 'a' && c <= 'z'; }
    constexpr bool ascii_isalpha(char c) noexcept { return ascii_isupper(c) || ascii_islower(c); }
    constexpr bool ascii_isdigit(char c) noexcept { return c >= '0' && c <= '9'; }
    constexpr bool ascii_isxdigit(char c) noexcept { return ascii_isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
    constexpr bool ascii_isalnum(char c) noexcept { return ascii_isdigit(c) || ascii_isalpha(c); }
    constexpr bool ascii_isgraph(char c) noexcept { return c >= 33 && c <= 126; }
    constexpr bool ascii_isprint(char c) noexcept { return c >= 32 && c <= 126; }
    constexpr bool ascii_ispunct(char c) noexcept { return ascii_isgraph(c) && ! ascii_isalnum(c); }
    constexpr bool ascii_isspace(char c) noexcept { return c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == ' '; }
    constexpr bool ascii_iscntrl(char c) noexcept { auto u = static_cast<unsigned char>(c); return u <= 31 || u == 127; }
    constexpr bool ascii_isalnum_w(char c) noexcept { return ascii_isalnum(c) || c == '_'; }
    constexpr bool ascii_isalpha_w(char c) noexcept { return ascii_isalpha(c) || c == '_'; }
    constexpr bool ascii_ispunct_w(char c) noexcept { return ascii_ispunct(c) && c != '_'; }
    constexpr char ascii_toupper(char c) noexcept { return ascii_islower(c) ? char(c - 0x20) : c; }
    constexpr char ascii_tolower(char c) noexcept { return ascii_isupper(c) ? char(c + 0x20) : c; }

    // String manipulation functions

    struct AsciiIcaseEqual {
        bool operator()(std::string_view a, std::string_view b) const noexcept;
    };

    struct AsciiIcaseLess {
        bool operator()(std::string_view a, std::string_view b) const noexcept;
    };

    std::string ascii_uppercase(std::string_view str);
    std::string ascii_lowercase(std::string_view str);
    std::string ascii_titlecase(std::string_view str);
    size_t common_prefix_length(std::string_view str1, std::string_view str2) noexcept;
    inline std::string dent(size_t level) { return std::string(4 * level, ' '); }
    std::string pad_left(std::string str, size_t len, char c = ' ');
    std::string pad_right(std::string str, size_t len, char c = ' ');
    std::pair<std::string_view, std::string_view> partition(std::string_view str, std::string_view chars = ascii_whitespace);
    std::pair<std::string_view, std::string_view> partition_at(std::string_view str, std::string_view delimiter);
    std::string quote(std::string_view str);
    std::string repeat(std::string_view str, size_t n);
    std::string replace(std::string_view str, std::string_view target, std::string_view replacement);
    std::string remove(std::string_view str, std::string_view target);
    std::vector<std::string_view> split(std::string_view str, std::string_view chars = ascii_whitespace);
    std::vector<std::string_view> split_at(std::string_view str, std::string_view delimiter);
    std::vector<std::string_view> split_lines(std::string_view str);
    std::string trim(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string trim_left(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string trim_right(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string unqualify(std::string_view str, std::string_view delimiters = ".:");
    std::string unwrap_lines(std::string_view str);
    std::string wrap_lines(std::string_view str, size_t width = 78, size_t margin = npos,
        bool checked = false);
    std::string indent_lines(std::string_view str, size_t spaces = 4);

    template <std::integral T>
    std::string hex(T t) {
        using U = std::make_unsigned_t<T>;
        static constexpr auto digits = "0123456789abcdef";
        auto u = U(t);
        std::string result(2 * sizeof(T), '0');
        for (size_t i = result.size() - 1; u != 0; --i, u >>= 4)
            result[i] = digits[u & 15];
        return result;
    }

    template <typename Range>
    std::string join(const Range& range, std::string_view delimiter = {}) {
        std::string result, str;
        for (auto& obj: range) {
            str = obj;
            result += str;
            result += delimiter;
        }
        if (! result.empty())
            result.resize(result.size() - delimiter.size());
        return result;
    }

    namespace Detail {

        std::string roman_helper(uint32_t n, bool lcase);

    }

    template <std::integral T>
    std::string roman(T t, bool lcase = false) {
        if (t < 1 || t > 1'000'000)
            throw std::invalid_argument("Invalid argument for Roman numerals: " + std::to_string(t));
        return Detail::roman_helper(uint32_t(t), lcase);
    }

    // String parsing functions

    bool to_boolean(const std::string& str);

    template <std::integral T>
    T to_integer(const std::string& str, int base = 10) {

        static const char* const error_message = std::is_signed_v<T> ? "Invalid integer: " : "Invalid unsigned integer: ";

        if (str.empty() || ascii_isspace(str[0]))
            throw std::invalid_argument(error_message + quote(str));

        if constexpr (std::is_unsigned_v<T>)
            if (str[0] == '-')
                throw std::invalid_argument(error_message + quote(str));

        if (base != 2 && base != 10 && base != 16)
            throw std::invalid_argument("Invalid base for conversion: " + std::to_string(base));

        T t;

        auto parse = [&] (auto f, auto& x) {
            char* end = nullptr;
            errno = 0;
            x = f(str.data(), &end, base);
            if (errno != 0 || end == str.data()
                    || x < std::numeric_limits<T>::min() || x > std::numeric_limits<T>::max())
                throw std::invalid_argument(error_message + quote(str));
            t = T(x);
        };

        if constexpr (sizeof(T) > sizeof(long)) {
            if constexpr (std::is_signed_v<T>) {
                long long x;
                parse(std::strtoll, x);
            } else {
                unsigned long long x;
                parse(std::strtoull, x);
            }
        } else {
            if constexpr (std::is_signed_v<T>) {
                long x;
                parse(std::strtol, x);
            } else {
                unsigned long x;
                parse(std::strtoul, x);
            }
        }

        return t;

    }

    inline auto to_short(const std::string& str, int base = 10) { return to_integer<short>(str, base); }
    inline auto to_ushort(const std::string& str, int base = 10) { return to_integer<unsigned short>(str, base); }
    inline auto to_int(const std::string& str, int base = 10) { return to_integer<int>(str, base); }
    inline auto to_uint(const std::string& str, int base = 10) { return to_integer<unsigned>(str, base); }
    inline auto to_long(const std::string& str, int base = 10) { return to_integer<long>(str, base); }
    inline auto to_ulong(const std::string& str, int base = 10) { return to_integer<unsigned long>(str, base); }
    inline auto to_llong(const std::string& str, int base = 10) { return to_integer<long long>(str, base); }
    inline auto to_ullong(const std::string& str, int base = 10) { return to_integer<unsigned long long>(str, base); }
    inline auto to_int8(const std::string& str, int base = 10) { return to_integer<int8_t>(str, base); }
    inline auto to_uint8(const std::string& str, int base = 10) { return to_integer<uint8_t>(str, base); }
    inline auto to_int16(const std::string& str, int base = 10) { return to_integer<int16_t>(str, base); }
    inline auto to_uint16(const std::string& str, int base = 10) { return to_integer<uint16_t>(str, base); }
    inline auto to_int32(const std::string& str, int base = 10) { return to_integer<int32_t>(str, base); }
    inline auto to_uint32(const std::string& str, int base = 10) { return to_integer<uint32_t>(str, base); }
    inline auto to_int64(const std::string& str, int base = 10) { return to_integer<int64_t>(str, base); }
    inline auto to_uint64(const std::string& str, int base = 10) { return to_integer<uint64_t>(str, base); }
    inline auto to_ptrdiff(const std::string& str, int base = 10) { return to_integer<ptrdiff_t>(str, base); }
    inline auto to_size(const std::string& str, int base = 10) { return to_integer<size_t>(str, base); }

    template <std::floating_point T>
    T to_floating(const std::string& str) {
        if (str.empty())
            throw std::invalid_argument("Invalid real number: " + quote(str));
        T t;
        char* end = nullptr;
        errno = 0;
        if constexpr(sizeof(T) > sizeof(double))
            t = T(std::strtold(str.data(), &end));
        else
            t = T(std::strtod(str.data(), &end));
        if (errno != 0 || end == str.data())
            throw std::invalid_argument("Invalid real number: " + quote(str));
        return t;
    }

    inline auto to_float(const std::string& str) { return to_floating<float>(str); }
    inline auto to_double(const std::string& str) { return to_floating<double>(str); }
    inline auto to_ldouble(const std::string& str) { return to_floating<long double>(str); }

    // Type functions

    namespace Detail {

        std::string type_name_helper(const std::type_info& info);

    }

    template <typename T>
    std::string type_name() {
        return Detail::type_name_helper(typeid(T));
    }

    // String literals

    namespace Literals {

        std::string operator""_doc(const char* ptr, size_t len);
        std::vector<std::string> operator""_qw(const char* ptr, size_t len);

    }

}
