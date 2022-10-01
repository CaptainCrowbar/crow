#pragma once

#include "crow/types.hpp"
#include <concepts>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        constexpr const char* hex_digits_lc = "0123456789abcdef";
        constexpr const char* hex_digits_uc = "0123456789ABCDEF";

        template <std::integral T>
        void append_hex(T t, std::string& str) {
            using U = std::make_unsigned_t<T>;
            auto u = U(t);
            for (int i = 8 * int(sizeof(T)) - 4; i >= 0; i -= 4)
                str += hex_digits_lc[(u >> i) % 16];
        }

        template <std::integral T>
        std::string hex_data(const T* ptr, size_t len, bool space = true) {
            if (ptr == nullptr || len == 0)
                return {};
            std::string hex;
            hex.reserve((2 * sizeof(T) + 1) * len);
            for (size_t i = 0; i < len; ++i) {
                append_hex(ptr[i], hex);
                if (space)
                    hex += ' ';
            }
            if (space)
                hex.pop_back();
            return hex;
        }

        std::string hex_char32(char32_t c);
        char32_t decode_char_impl(const std::string& str, size_t& pos) noexcept;
        char32_t decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos) noexcept;
        char32_t decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos) noexcept;
        char32_t decode_char_impl(const std::u16string& str, size_t& pos) noexcept;
        char32_t decode_char_impl(const std::u32string& str, size_t& pos) noexcept;
        char32_t decode_char_impl(const std::wstring& str, size_t& pos) noexcept;
        char32_t check_decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos);
        char32_t check_decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos);
        char32_t check_decode_char_impl(const std::string& str, size_t& pos);
        char32_t check_decode_char_impl(const std::u16string& str, size_t& pos);
        char32_t check_decode_char_impl(const std::u32string& str, size_t& pos);
        char32_t check_decode_char_impl(const std::wstring& str, size_t& pos);
        bool encode_utf16_char_impl(char32_t c, std::function<void(char16_t)> out);
        bool encode_utf32_char_impl(char32_t c, std::function<void(char32_t)> out);
        bool encode_char_impl(char32_t c, std::string& str);
        bool encode_char_impl(char32_t c, std::u16string& str);
        bool encode_char_impl(char32_t c, std::u32string& str);
        bool encode_char_impl(char32_t c, std::wstring& str);

        enum class Hangul_Syllable_Type: int { NA, L, V, T, LV, LVT };

        int canonical_combining_class(char32_t c);
        char32_t canonical_composition(char32_t c1, char32_t c2);
        std::pair<char32_t, char32_t> canonical_decomposition(char32_t c);
        Hangul_Syllable_Type hangul_syllable_type(char32_t c) noexcept;
        bool is_full_composition_exclusion(char32_t c);

    }

    template <typename C>
    concept CharacterType =
        std::same_as<C, char>
        || std::same_as<C, char16_t>
        || std::same_as<C, char32_t>
        || std::same_as<C, wchar_t>;

    constexpr char32_t not_unicode = ~ char32_t(0);

    constexpr bool is_unicode(char32_t c) noexcept { return c <= 0xd7ff || (c >= 0xe000 && c <= 0x10ffff); }

    bool is_control(char32_t c);
    bool is_pattern_syntax(char32_t c);
    bool is_xid_continue(char32_t c);
    bool is_xid_start(char32_t c);

    inline bool is_xid_nonstart(char32_t c) {
        return is_xid_continue(c) && ! is_xid_start(c);
    }

    template <CharacterType C>
    char32_t decode_char(const std::basic_string<C>& str, size_t& pos) noexcept {
        return Detail::decode_char_impl(str, pos);
    }

    template <CharacterType C>
    char32_t check_decode_char(const std::basic_string<C>& str, size_t& pos) {
        return Detail::check_decode_char_impl(str, pos);
    }

    template <CharacterType C>
    bool encode_char(char32_t c, std::basic_string<C>& str) {
        return Detail::encode_char_impl(c, str);
    }

    template <CharacterType C>
    void check_encode_char(char32_t c, std::basic_string<C>& str) {
        if (! encode_char(c, str))
            throw std::invalid_argument("Invalid Unicode character: " + Detail::hex_char32(c));
    }

    template <CharacterType C>
    std::u32string decode_string(const std::basic_string<C>& str) {
        std::u32string utf32;
        for (size_t pos = 0; pos < str.size();)
            utf32 += check_decode_char(str, pos);
        return utf32;
    }

    template <CharacterType C>
    void encode_string(const std::u32string& src, std::basic_string<C>& dst) {
        std::basic_string<C> temp;
        for (auto c: src)
            check_encode_char(c, temp);
        dst = std::move(temp);
    }

    std::string to_utf8(const std::u32string& utf32);
    std::u16string to_utf16(const std::u32string& utf32);
    std::u32string to_utf32(const std::u32string& utf32);
    std::wstring to_wstring(const std::u32string& utf32);

    template <CharacterType C>
    size_t utf_length(const std::basic_string<C>& str) {
        size_t len = 0;
        for (size_t pos = 0; pos < str.size(); ++len)
            check_decode_char(str, pos);
        return len;
    }

    template <CharacterType C>
    bool is_valid_utf(const std::basic_string<C>& str) {
        for (size_t pos = 0; pos < str.size();)
            if (decode_char(str, pos) == not_unicode)
                return false;
        return true;
    }

    namespace Detail {

        class CodeSet {
        private:
            struct subrange:
            public std::pair<char32_t, char32_t> {
                subrange(char32_t c) { first = second = c; }
                subrange(char32_t a, char32_t b) { first = a; second = b; }
            };
        public:
            CodeSet(std::initializer_list<subrange> list) {
                for (auto [a,b]: list)
                    map_.insert({a,b});
            }
            bool operator()(char32_t c) const {
                auto it = map_.upper_bound(c);
                return it != map_.begin() && c <= std::prev(it)->second;
            }
        private:
            std::map<char32_t, char32_t> map_;
        };

        const CodeSet& gc_is_cc_cf_mn();  // General_Category = Cc (Control), Cf (Format), or Mn (Nonspacing_Mark)
        const CodeSet& ws_pws();          // White_Space or Pattern_White_Space property
        const CodeSet& eaw_is_f_w();      // East_Asian_Width = F (Fullwidth) or W (Wide)
        size_t character_width(char32_t c);

    }

    template <CharacterType C>
    size_t utf_width(const std::basic_string<C>& str) {
        size_t width = 0;
        for (size_t pos = 0; pos < str.size();)
            width += Detail::character_width(check_decode_char(str, pos));
        return width;
    }

    std::string to_nfc(std::string str);
    std::u32string to_nfc(std::u32string str);
    std::string to_nfd(std::string str);
    std::u32string to_nfd(std::u32string str);

}
