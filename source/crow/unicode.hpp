#pragma once

#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/ucd/ucd.hpp"
#include <concepts>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
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
        char32_t decode_char_impl(std::string_view str, size_t& pos) noexcept;
        char32_t decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos) noexcept;
        char32_t decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos) noexcept;
        char32_t decode_char_impl(std::u16string_view str, size_t& pos) noexcept;
        char32_t decode_char_impl(std::u32string_view str, size_t& pos) noexcept;
        char32_t decode_char_impl(std::wstring_view str, size_t& pos) noexcept;
        char32_t check_decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos);
        char32_t check_decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos);
        char32_t check_decode_char_impl(std::string_view str, size_t& pos);
        char32_t check_decode_char_impl(std::u16string_view str, size_t& pos);
        char32_t check_decode_char_impl(std::u32string_view str, size_t& pos);
        char32_t check_decode_char_impl(std::wstring_view str, size_t& pos);
        bool encode_utf16_char_impl(char32_t c, std::function<void(char16_t)> out);
        bool encode_utf32_char_impl(char32_t c, std::function<void(char32_t)> out);
        bool encode_char_impl(char32_t c, std::string& str);
        bool encode_char_impl(char32_t c, std::u16string& str);
        bool encode_char_impl(char32_t c, std::u32string& str);
        bool encode_char_impl(char32_t c, std::wstring& str);

        int canonical_combining_class(char32_t c);
        char32_t canonical_composition(char32_t c1, char32_t c2);
        std::pair<char32_t, char32_t> canonical_decomposition(char32_t c);
        East_Asian_Width east_asian_width(char32_t c);
        Hangul_Syllable_Type hangul_syllable_type(char32_t c) noexcept;
        bool is_full_composition_exclusion(char32_t c);
        constexpr bool is_regional_indicator(char32_t c) noexcept { return c >= 0x1f1e6 & c <= 0x1f1ff; }
        constexpr bool is_zero_width(GC gc) noexcept { return gc == GC::Cc || gc == GC::Cf || gc == GC::Mn || gc == GC::Sk; }

    }

    CROW_ENUM_CLASS(Usize, int, 1,
        bytes,      // Bytes
        units,      // Code units
        scalars,    // Scalar values
        graphemes,  // Grapheme clusters
        columns     // Virtual columns
    )

    template <typename C>
    concept CharacterType =
        std::same_as<C, char>
        || std::same_as<C, char16_t>
        || std::same_as<C, char32_t>
        || std::same_as<C, wchar_t>;

    constexpr char32_t not_unicode = ~ char32_t(0);

    constexpr bool is_unicode(char32_t c) noexcept { return c <= 0xd7ff || (c >= 0xe000 && c <= 0x10ffff); }

    template <CharacterType C>
    char32_t decode_char(const std::basic_string<C>& str, size_t& pos) noexcept {
        return Detail::decode_char_impl(str, pos);
    }

    template <CharacterType C>
    char32_t decode_char(std::basic_string_view<C> str, size_t& pos) noexcept {
        return Detail::decode_char_impl(str, pos);
    }

    template <CharacterType C>
    char32_t check_decode_char(const std::basic_string<C>& str, size_t& pos) {
        return Detail::check_decode_char_impl(str, pos);
    }

    template <CharacterType C>
    char32_t check_decode_char(std::basic_string_view<C> str, size_t& pos) {
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
    std::u32string decode_string(std::basic_string_view<C> str) {
        std::u32string utf32;
        for (size_t pos = 0; pos < str.size();)
            utf32 += check_decode_char(str, pos);
        return utf32;
    }

    template <CharacterType C>
    void encode_string(std::u32string_view src, std::basic_string<C>& dst) {
        std::basic_string<C> temp;
        for (auto c: src)
            check_encode_char(c, temp);
        dst = std::move(temp);
    }

    std::string to_utf8(std::u32string_view utf32);
    std::u16string to_utf16(std::u32string_view utf32);
    std::u32string to_utf32(std::u32string_view utf32);
    std::wstring to_wstring(std::u32string_view utf32);

    class UtfIterator:
    public ForwardIterator<UtfIterator, const char32_t> {
    public:
        UtfIterator() = default;
        UtfIterator(std::string_view utf8, size_t pos) noexcept: utf8_(utf8), pos_(pos), len_(0) {}
        const char32_t& operator*() const noexcept;
        UtfIterator& operator++() noexcept;
        bool operator==(const UtfIterator& rhs) const noexcept { return pos_ == rhs.pos_; }
    private:
        std::string_view utf8_;
        size_t pos_;
        mutable size_t len_;
        mutable char32_t char_;
    };

    using UtfRange = Irange<UtfIterator>;

    inline UtfIterator utf_begin(std::string_view utf8) { return {utf8, 0}; }
    inline UtfIterator utf_end(std::string_view utf8) { return {utf8, utf8.size()}; }
    inline UtfRange utf_range(std::string_view utf8) { return {utf_begin(utf8), utf_end(utf8)}; }

    GC general_category(char32_t c);
    bool is_pattern_syntax(char32_t c);
    bool is_xid_continue(char32_t c);
    bool is_xid_start(char32_t c);

    constexpr bool is_noncharacter(char32_t c) noexcept {
        return (c >= 0xfdd0 && c <= 0xfdef) || (c & 0xfffe) == 0xfffe;
    }

    template <CharacterType C>
    bool is_valid_utf(std::basic_string_view<C> str, bool hard = false) {
        if (hard) {
            for (size_t pos = 0; pos < str.size();)
                check_decode_char(str, pos);
        } else {
            for (size_t pos = 0; pos < str.size();)
                if (decode_char(str, pos) == not_unicode)
                    return false;
        }
        return true;
    }

    template <CharacterType C>
    bool is_valid_utf(const std::basic_string<C>& str, bool hard = false) {
        return is_valid_utf(std::basic_string_view<C>(str), hard);
    }

    template <CharacterType C>
    size_t utf_size(std::basic_string_view<C> str, Usize mode) {

        using namespace Detail;

        size_t n = 0;

        switch (mode) {

            case Usize::bytes: {

                n = str.size() * sizeof(C);

                break;

            }

            case Usize::units: {

                n = str.size();

                break;

            }

            case Usize::scalars: {

                for (size_t pos = 0; pos < str.size(); ++n)
                    check_decode_char(str, pos);

                break;

            }

            case Usize::graphemes: {

                char32_t c = 0;
                bool is_ri = false;
                bool was_ri = false;
                GC gc;

                for (size_t pos = 0; pos < str.size();) {
                    c = check_decode_char(str, pos);
                    is_ri = is_regional_indicator(c);
                    if (is_ri && was_ri) {
                        was_ri = false;
                    } else {
                        gc = general_category(c);
                        if (! is_zero_width(gc))
                            ++n;
                        was_ri = is_ri;
                    }
                }

                break;

            }

            case Usize::columns: {

                char32_t c = 0;
                GC gc;
                East_Asian_Width eaw;

                for (size_t pos = 0; pos < str.size();) {
                    c = check_decode_char(str, pos);
                    gc = general_category(c);
                    if (! is_zero_width(gc)) {
                        eaw = east_asian_width(c);
                        if (eaw == East_Asian_Width::Fullwidth || eaw == East_Asian_Width::Wide)
                            n += 2;
                        else
                            ++n;
                    }
                }

                break;

            }

        }

        return n;

    }

    template <CharacterType C>
    size_t utf_size(const std::basic_string<C>& str, Usize mode = Usize::columns) {
        return utf_size(std::basic_string_view<C>(str), mode);
    }

    std::string to_nfc(std::string_view str);
    std::u32string to_nfc(std::u32string_view str);
    std::string to_nfd(std::string_view str);
    std::u32string to_nfd(std::u32string_view str);

}
