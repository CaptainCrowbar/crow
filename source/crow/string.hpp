#pragma once

#include "crow/enum.hpp"
#include "crow/string-view.hpp"
#include "crow/types.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#if __apple_build_version__ / 10000 == 1400

    // Missing comparison operators in Xcode 14

    namespace std {

        template <typename C, typename T>
        constexpr std::strong_ordering operator<=>
        (std::basic_string_view<C, T> s, std::basic_string_view<C, T> t) noexcept {
            size_t n = std::min(s.size(), t.size());
            int c = std::memcmp(s.data(), t.data(), n);
            return c == 0 ? s.size() <=> t.size() : Crow::to_order(c);
        }

        template <typename C, typename T, typename A>
        constexpr std::strong_ordering operator<=>
        (const std::basic_string<C, T, A>& s, const std::basic_string<C, T, A>& t) noexcept {
            std::basic_string_view<C, T> v(s);
            std::basic_string_view<C, T> w(t);
            return v <=> w;
        }

        template <typename C, typename T, typename A>
        constexpr std::strong_ordering operator<=>
        (const std::basic_string<C, T, A>& s, const std::basic_string_view<C, T>& t) noexcept {
            std::basic_string_view<C, T> v(s);
            return v <=> t;
        }

        template <typename C, typename T, typename A>
        constexpr std::strong_ordering operator<=>
        (const std::basic_string_view<C, T>& s, const std::basic_string<C, T, A>& t) noexcept {
            std::basic_string_view<C, T> w(t);
            return s <=> w;
        }

    }

#endif

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
    constexpr bool ascii_isgraph(char c) noexcept { return c >= 0x21 && c <= 0x7e; }
    constexpr bool ascii_isprint(char c) noexcept { return c >= 0x20 && c <= 0x7e; }
    constexpr bool ascii_ispunct(char c) noexcept { return ascii_isgraph(c) && ! ascii_isalnum(c); }
    constexpr bool ascii_isspace(char c) noexcept { return c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == ' '; }
    constexpr bool ascii_iscntrl(char c) noexcept { auto u = static_cast<unsigned char>(c); return u <= 0x1f || u == 0x7f; }
    constexpr bool ascii_isalnum_w(char c) noexcept { return ascii_isalnum(c) || c == '_'; }
    constexpr bool ascii_isalpha_w(char c) noexcept { return ascii_isalpha(c) || c == '_'; }
    constexpr bool ascii_ispunct_w(char c) noexcept { return ascii_ispunct(c) && c != '_'; }
    constexpr char ascii_toupper(char c) noexcept { return ascii_islower(c) ? char(c - 0x20) : c; }
    constexpr char ascii_tolower(char c) noexcept { return ascii_isupper(c) ? char(c + 0x20) : c; }

    // String formatting functions

    namespace Detail {

        std::string roman_helper(uint32_t n, bool lcase);

    }

    template <std::integral T>
    std::string bin(T t, size_t digits = 8 * sizeof(T)) {
        using U = std::make_unsigned_t<T>;
        auto u = U(t);
        std::string result;
        for (; u != 0 || result.size() < digits; u /= 2)
            result.push_back(char('0' + u % 2));
        std::reverse(result.begin(), result.end());
        return result;
    }

    template <std::integral T>
    std::string dec(T t, size_t digits = 1) {
        using U = std::make_unsigned_t<T>;
        U u;
        if constexpr (std::is_signed_v<T>) {
            if (t < 0)
                u = U(- t);
            else
                u = U(t);
        } else {
            u = U(t);
        }
        std::string result;
        for (; u != 0 || result.size() < digits; u /= 10)
            result.push_back(char('0' + u % 10));
        if constexpr (std::is_signed_v<T>)
            if (t < 0)
                result += '-';
        std::reverse(result.begin(), result.end());
        return result;
    }

    template <std::integral T>
    std::string hex(T t, size_t digits = 2 * sizeof(T)) {
        using U = std::make_unsigned_t<T>;
        static constexpr auto xdigits = "0123456789abcdef";
        auto u = U(t);
        std::string result;
        for (; u != 0 || result.size() < digits; u /= 16)
            result.push_back(xdigits[u % 16]);
        std::reverse(result.begin(), result.end());
        return result;
    }

    template <std::integral T>
    std::string roman(T t, bool lcase = false) {
        if (t < 1 || t > 1'000'000)
            throw std::invalid_argument("Invalid argument for Roman numerals: " + std::to_string(t));
        return Detail::roman_helper(uint32_t(t), lcase);
    }

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
    std::vector<std::string> splits(std::string_view str, std::string_view chars = ascii_whitespace);
    std::vector<std::string_view> splitv(std::string_view str, std::string_view chars = ascii_whitespace);
    std::vector<std::string> splits_at(std::string_view str, std::string_view delimiter);
    std::vector<std::string_view> splitv_at(std::string_view str, std::string_view delimiter);
    std::vector<std::string> splits_lines(std::string_view str, bool keep = false);
    std::vector<std::string_view> splitv_lines(std::string_view str, bool keep = false);
    std::string trim(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string trim_left(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string trim_right(std::string_view str, std::string_view chars = ascii_whitespace);
    std::string unqualify(std::string_view str, std::string_view delimiters = ".:");
    std::string unwrap_lines(std::string_view str);
    std::string wrap_lines(std::string_view str, Usize mode = Usize::columns,
        size_t width = npos, size_t margin = npos, bool checked = false);
    std::string indent_lines(std::string_view str, size_t spaces = 4);

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

    // String parsing functions

    bool to_boolean(const std::string& str);
    inline bool to_boolean(std::string_view str) { return to_boolean(std::string(str)); }
    inline bool to_boolean(const char* str) { return str != nullptr && to_boolean(std::string(str)); }

    template <std::integral T>
    T to_integer(const char* str, int base = 10) {

        static const char* const error_message = std::is_signed_v<T> ? "Invalid integer: " : "Invalid unsigned integer: ";

        if (str == nullptr || *str == '\0' || ascii_isspace(*str))
            throw std::invalid_argument(error_message + quote(str));

        if constexpr (std::is_unsigned_v<T>)
            if (*str == '-')
                throw std::invalid_argument(error_message + quote(str));

        if (base != 2 && base != 10 && base != 16)
            throw std::invalid_argument("Invalid base for conversion: " + std::to_string(base));

        T t;

        auto parse = [&] (auto f, auto& x) {
            char* end = nullptr;
            errno = 0;
            x = f(str, &end, base);
            if (errno != 0 || end == str
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

    template <std::floating_point T>
    T to_floating(const char* str) {
        if (str == nullptr || *str == '\0' || ascii_isspace(*str))
            throw std::invalid_argument("Invalid real number: " + quote(str));
        T t;
        char* end = nullptr;
        errno = 0;
        if constexpr(sizeof(T) > sizeof(double))
            t = T(std::strtold(str, &end));
        else
            t = T(std::strtod(str, &end));
        if (errno != 0 || end == str)
            throw std::invalid_argument("Invalid real number: " + quote(str));
        return t;
    }

    template <std::integral T> T to_integer(const std::string& str, int base = 10) { return to_integer<T>(str.data(), base); }
    template <std::integral T> T to_integer(std::string_view str, int base = 10) { return to_integer<T>(std::string(str), base); }
    template <std::floating_point T> T to_floating(const std::string& str) { return to_floating<T>(str.data()); }
    template <std::floating_point T> T to_floating(std::string_view str) { return to_floating<T>(std::string(str)); }

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
    inline auto to_float(const std::string& str) { return to_floating<float>(str); }
    inline auto to_double(const std::string& str) { return to_floating<double>(str); }
    inline auto to_ldouble(const std::string& str) { return to_floating<long double>(str); }

    inline auto to_short(std::string_view str, int base = 10) { return to_integer<short>(str, base); }
    inline auto to_ushort(std::string_view str, int base = 10) { return to_integer<unsigned short>(str, base); }
    inline auto to_int(std::string_view str, int base = 10) { return to_integer<int>(str, base); }
    inline auto to_uint(std::string_view str, int base = 10) { return to_integer<unsigned>(str, base); }
    inline auto to_long(std::string_view str, int base = 10) { return to_integer<long>(str, base); }
    inline auto to_ulong(std::string_view str, int base = 10) { return to_integer<unsigned long>(str, base); }
    inline auto to_llong(std::string_view str, int base = 10) { return to_integer<long long>(str, base); }
    inline auto to_ullong(std::string_view str, int base = 10) { return to_integer<unsigned long long>(str, base); }
    inline auto to_int8(std::string_view str, int base = 10) { return to_integer<int8_t>(str, base); }
    inline auto to_uint8(std::string_view str, int base = 10) { return to_integer<uint8_t>(str, base); }
    inline auto to_int16(std::string_view str, int base = 10) { return to_integer<int16_t>(str, base); }
    inline auto to_uint16(std::string_view str, int base = 10) { return to_integer<uint16_t>(str, base); }
    inline auto to_int32(std::string_view str, int base = 10) { return to_integer<int32_t>(str, base); }
    inline auto to_uint32(std::string_view str, int base = 10) { return to_integer<uint32_t>(str, base); }
    inline auto to_int64(std::string_view str, int base = 10) { return to_integer<int64_t>(str, base); }
    inline auto to_uint64(std::string_view str, int base = 10) { return to_integer<uint64_t>(str, base); }
    inline auto to_ptrdiff(std::string_view str, int base = 10) { return to_integer<ptrdiff_t>(str, base); }
    inline auto to_size(std::string_view str, int base = 10) { return to_integer<size_t>(str, base); }
    inline auto to_float(std::string_view str) { return to_floating<float>(str); }
    inline auto to_double(std::string_view str) { return to_floating<double>(str); }
    inline auto to_ldouble(std::string_view str) { return to_floating<long double>(str); }

    inline auto to_short(const char* str, int base = 10) { return to_integer<short>(str, base); }
    inline auto to_ushort(const char* str, int base = 10) { return to_integer<unsigned short>(str, base); }
    inline auto to_int(const char* str, int base = 10) { return to_integer<int>(str, base); }
    inline auto to_uint(const char* str, int base = 10) { return to_integer<unsigned>(str, base); }
    inline auto to_long(const char* str, int base = 10) { return to_integer<long>(str, base); }
    inline auto to_ulong(const char* str, int base = 10) { return to_integer<unsigned long>(str, base); }
    inline auto to_llong(const char* str, int base = 10) { return to_integer<long long>(str, base); }
    inline auto to_ullong(const char* str, int base = 10) { return to_integer<unsigned long long>(str, base); }
    inline auto to_int8(const char* str, int base = 10) { return to_integer<int8_t>(str, base); }
    inline auto to_uint8(const char* str, int base = 10) { return to_integer<uint8_t>(str, base); }
    inline auto to_int16(const char* str, int base = 10) { return to_integer<int16_t>(str, base); }
    inline auto to_uint16(const char* str, int base = 10) { return to_integer<uint16_t>(str, base); }
    inline auto to_int32(const char* str, int base = 10) { return to_integer<int32_t>(str, base); }
    inline auto to_uint32(const char* str, int base = 10) { return to_integer<uint32_t>(str, base); }
    inline auto to_int64(const char* str, int base = 10) { return to_integer<int64_t>(str, base); }
    inline auto to_uint64(const char* str, int base = 10) { return to_integer<uint64_t>(str, base); }
    inline auto to_ptrdiff(const char* str, int base = 10) { return to_integer<ptrdiff_t>(str, base); }
    inline auto to_size(const char* str, int base = 10) { return to_integer<size_t>(str, base); }
    inline auto to_float(const char* str) { return to_floating<float>(str); }
    inline auto to_double(const char* str) { return to_floating<double>(str); }
    inline auto to_ldouble(const char* str) { return to_floating<long double>(str); }

    // String query functions

    std::pair<size_t, size_t> line_and_column(std::string_view str, size_t pos, Usize mode, size_t tab_size = 4);

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

        inline std::vector<std::string> operator""_qw(const char* ptr, size_t len) {
            return splits(std::string_view(ptr, len));
        }

    }

}
