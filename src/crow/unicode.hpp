#pragma once

#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/ucd/ucd.hpp"
#include <compare>
#include <concepts>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

static_assert(sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t));

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
        char32_t decode_utf16_char_impl(std::u16string_view str, size_t& pos) noexcept;
        char32_t decode_utf32_char_impl(std::u32string_view str, size_t& pos) noexcept;
        char32_t decode_char_impl(std::u16string_view str, size_t& pos) noexcept;
        char32_t decode_char_impl(std::u32string_view str, size_t& pos) noexcept;
        char32_t decode_char_impl(std::wstring_view str, size_t& pos) noexcept;
        char32_t check_decode_utf16_char_impl(std::u16string_view str, size_t& pos);
        char32_t check_decode_utf32_char_impl(std::u32string_view str, size_t& pos);
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
        Grapheme_Cluster_Break grapheme_cluster_break(char32_t c);
        Hangul_Syllable_Type hangul_syllable_type(char32_t c) noexcept;
        bool is_full_composition_exclusion(char32_t c);
        bool is_extended_pictographic(char32_t c);

    }

    CROW_ENUM_SCOPED(Usize, int,
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

    class UnicodeError:
    public std::runtime_error {
    public:
        explicit UnicodeError(char32_t c): std::runtime_error(message(c)), pos_(0) {}
        UnicodeError(std::string_view str, size_t pos): std::runtime_error(message(str, pos)), pos_(pos) {}
        UnicodeError(std::u16string_view str, size_t pos): std::runtime_error(message(str, pos)), pos_(pos) {}
        UnicodeError(std::u32string_view str, size_t pos): std::runtime_error(message(str, pos)), pos_(pos) {}
        UnicodeError(std::wstring_view str, size_t pos): std::runtime_error(message(str, pos)), pos_(pos) {}
        size_t pos() const noexcept { return pos_; }
    private:
        size_t pos_ = 0;
        static std::string message(char32_t c);
        static std::string message(std::string_view str, size_t pos);
        static std::string message(std::u16string_view str, size_t pos);
        static std::string message(std::u32string_view str, size_t pos);
        static std::string message(std::wstring_view str, size_t pos);
    };

    constexpr char32_t first_surrogate        = 0xd800;         // First UTF-16 surrogate code
    constexpr char32_t last_surrogate         = 0xdfff;         // Last UTF-16 surrogate code
    constexpr char32_t byte_order_mark        = 0xfeff;         // Byte order mark
    constexpr char32_t replacement_character  = 0xfffd;         // Unicode replacement character
    constexpr char32_t max_unicode            = 0x10ffff;       // Highest possible Unicode code point
    constexpr char32_t not_unicode            = ~ char32_t(0);  // Returned as an error code

    constexpr bool is_unicode(char32_t c) noexcept {
        return c < first_surrogate || (c > last_surrogate && c <= max_unicode);
    }

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
            throw UnicodeError(c);
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
        explicit UtfIterator(std::string_view utf8, size_t pos, bool checked = false);
        const char32_t& operator*() const { return char_; }
        UtfIterator& operator++();
        bool operator==(const UtfIterator& rhs) const noexcept { return pos_ == rhs.pos_; }
        std::string_view view() const noexcept { return utf8_.substr(pos_, next_ - pos_); }
    private:
        std::string_view utf8_;
        size_t pos_ = 0;
        size_t next_ = 0;
        char32_t char_ = 0;
        bool checked_ = false;
    };

    using UtfRange = Irange<UtfIterator>;

    inline UtfIterator utf_begin(std::string_view utf8, bool checked = false) noexcept {
        return UtfIterator(utf8, 0, checked);
    }

    inline UtfIterator utf_end(std::string_view utf8, bool checked = false) noexcept {
        return UtfIterator(utf8, utf8.size(), checked);
    }

    inline UtfRange utf_range(std::string_view utf8, bool checked = false) noexcept {
        return {utf_begin(utf8, checked), utf_end(utf8, checked)};
    }

    size_t utf_size(std::string_view str, Usize mode);

    class GraphemeIterator:
    public ForwardIterator<GraphemeIterator, const std::string_view> {
    public:
        GraphemeIterator() = default;
        explicit GraphemeIterator(std::string_view utf8, size_t pos, bool checked = false);
        const std::string_view& operator*() const { return current_; }
        GraphemeIterator& operator++();
        bool operator==(const GraphemeIterator& rhs) const noexcept { return current_.data() == rhs.current_.data(); }
    private:
        friend size_t utf_size(std::string_view str, Usize mode);
        using GCB = Detail::Grapheme_Cluster_Break;
        std::string_view source_;   // Full subject string
        std::string_view current_;  // Encoded grapheme cluster
        std::string_view peek_;     // Next encoded scalar
        std::u32string uchars_;     // Current+peek as UTF-32
        std::vector<GC> gcs_;       // Current+peek general category
        std::vector<GCB> gcbs_;     // Current+peek grapheme cluster break
        std::vector<bool> eps_;     // Current+peek extended pictographic
        bool checked_ = false;
    };

    using GraphemeRange = Irange<GraphemeIterator>;

    inline GraphemeIterator graphemes_begin(std::string_view utf8, bool checked = false) {
        return GraphemeIterator(utf8, 0, checked);
    }

    inline GraphemeIterator graphemes_end(std::string_view utf8, bool checked = false) {
        return GraphemeIterator(utf8, utf8.size(), checked);
    }

    inline GraphemeRange graphemes(std::string_view utf8, bool checked = false) {
        return {graphemes_begin(utf8, checked), graphemes_end(utf8, checked)};
    }

    GC general_category(char32_t c);
    char primary_category(char32_t c);
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

    std::string to_nfc(std::string_view str);
    std::u32string to_nfc(std::u32string_view str);
    std::string to_nfd(std::string_view str);
    std::u32string to_nfd(std::u32string_view str);

}
