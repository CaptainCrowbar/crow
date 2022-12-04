#include "crow/unicode.hpp"
#include "crow/string-view.hpp"
#include <algorithm>
#include <iterator>
#include <map>

namespace Crow {

    std::string UnicodeError::message(char32_t c) {
        return "Invalid Unicode character: " + Detail::hex_char32(c);
    }

    std::string UnicodeError::message(std::string_view str, size_t pos) {
        std::string msg = "Invalid UTF-8 at byte ";
        msg += std::to_string(pos);
        msg += ": 0x";
        auto end = pos;
        Detail::decode_char_impl(str, end);
        msg += Detail::hex_data(str.data() + pos, end - pos);
        return msg;
    }

    std::string UnicodeError::message(std::u16string_view str, size_t pos) {
        std::string msg = "Invalid UTF-16 at code unit ";
        msg += std::to_string(pos);
        msg += ": ";
        msg += Detail::hex_char32(str[pos]);
        return msg;
    }

    std::string UnicodeError::message(std::u32string_view str, size_t pos) {
        std::string msg = "Invalid Unicode character at position ";
        msg += std::to_string(pos);
        msg += ": ";
        msg += Detail::hex_char32(str[pos]);
        return msg;
    }

    std::string UnicodeError::message(std::wstring_view str, size_t pos) {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            auto ptr = reinterpret_cast<const char16_t*>(str.data());
            std::u16string_view view(ptr, str.size());
            return message(view, pos);
        } else {
            auto ptr = reinterpret_cast<const char32_t*>(str.data());
            std::u32string_view view(ptr, str.size());
            return message(view, pos);
        }
    }

    namespace Detail {

        std::string hex_char32(char32_t c) {
            std::string hex = "U+";
            for (; c > 0 || hex.size() < 6; c >>= 4)
                hex += hex_digits_uc[c % 16];
            std::reverse(hex.begin() + 2, hex.end());
            return hex;
        }

        char32_t decode_char_impl(std::string_view str, size_t& pos) noexcept {

            // UTF-8 byte distribution:
            // 0x00-0x7f = Single byte character
            // 0x80-0xbf = Second or later byte of a multibyte character
            // 0xc0-0xc1 = Not allowed
            // 0xc2-0xdf = First byte of a 2-byte character
            // 0xe0-0xef = First byte of a 3-byte character
            // 0xf0-0xf4 = First byte of a 4-byte character
            // 0xf5-0xff = Not allowed

            static constexpr auto is_start = [] (uint8_t u) constexpr {
                return u <= 0x7f || (u >= 0xc2 && u <= 0xf4);
            };

            auto fail = [&str,&pos] {
                size_t max_pos = std::min(pos + 4, str.size());
                do ++pos;
                    while (pos < max_pos && ! is_start(str[pos]));
                return not_unicode;
            };

            if (pos >= str.size())
                return not_unicode;

            auto bytes = reinterpret_cast<const uint8_t*>(str.data() + pos);
            if (bytes[0] <= 0x7f)
                return char32_t(str[pos++]);
            size_t available = str.size() - pos;

            if (! is_start(bytes[0]) || available < 2 || bytes[1] < 0x80 || bytes[1] > 0xbf
                    || (bytes[0] == 0xe0 && bytes[1] < 0xa0)
                    || (bytes[0] == 0xed && bytes[1] > 0x9f)
                    || (bytes[0] == 0xf0 && bytes[1] < 0x90)
                    || (bytes[0] == 0xf4 && bytes[1] > 0x8f))
                return fail();

            if (bytes[0] <= 0xdf) {
                pos += 2;
                return (char32_t(bytes[0] & 0x1f) << 6)
                    + char32_t(bytes[1] & 0x3f);
            }

            if (bytes[0] <= 0xef) {
                if (available < 3 || bytes[2] < 0x80 || bytes[2] > 0xbf)
                    return fail();
                pos += 3;
                return (char32_t(bytes[0] & 0x0f) << 12)
                    + (char32_t(bytes[1] & 0x3f) << 6)
                    + char32_t(bytes[2] & 0x3f);
            }

            if (available < 4 || bytes[2] < 0x80 || bytes[2] > 0xbf
                    || bytes[3] < 0x80 || bytes[3] > 0xbf)
                return fail();

            pos += 4;

            return (char32_t(bytes[0] & 0x07) << 18)
                + (char32_t(bytes[1] & 0x3f) << 12)
                + (char32_t(bytes[2] & 0x3f) << 6)
                + char32_t(bytes[3] & 0x3f);

        }

        char32_t decode_utf16_char_impl(std::u16string_view str, size_t& pos) noexcept {

            // UTF-16 code unit distribution:
            // 0000-d7ff = BMP character
            // d800-dbff = High surrogate
            // dc00-dfff = Low surrogate
            // e000-ffff = BMP character

            static constexpr auto is_start = [] (char16_t c) constexpr {
                return c <= 0xdbff || c > last_surrogate;
            };

            auto ptr = str.data();
            auto len = str.size();

            auto fail = [ptr,len,&pos] {
                size_t max_pos = std::min(pos + 2, len);
                do ++pos;
                    while (pos < max_pos && ! is_start(ptr[pos]));
                return not_unicode;
            };

            if (pos >= len)
                return not_unicode;

            if (ptr[pos] < first_surrogate || ptr[pos] > last_surrogate)
                return char32_t(ptr[pos++]);

            if (len - pos < 2 || ptr[pos] >= 0xdc00
                    || ptr[pos + 1] <= 0xdbff || ptr[pos + 1] > last_surrogate) {
                return fail();
            }

            char32_t c = (char32_t(ptr[pos] & 0x3ff) << 10) + (ptr[pos + 1] & 0x3ff) + 0x10000;
            pos += 2;

            return c;

        }

        char32_t decode_utf32_char_impl(std::u32string_view str, size_t& pos) noexcept {
            if (pos >= str.size())
                return not_unicode;
            auto c = str[pos++];
            if (is_unicode(c))
                return c;
            else
                return not_unicode;
        }

        char32_t decode_char_impl(std::u16string_view str, size_t& pos) noexcept {
            return decode_utf16_char_impl(str, pos);
        }

        char32_t decode_char_impl(std::u32string_view str, size_t& pos) noexcept {
            return decode_utf32_char_impl(str, pos);
        }

        char32_t decode_char_impl(std::wstring_view str, size_t& pos) noexcept {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
                auto ptr = reinterpret_cast<const char16_t*>(str.data());
                std::u16string_view ustr(ptr, str.size());
                return decode_utf16_char_impl(ustr, pos);
            } else {
                auto ptr = reinterpret_cast<const char32_t*>(str.data());
                std::u32string_view ustr(ptr, str.size());
                return decode_utf32_char_impl(ustr, pos);
            }
        }

        char32_t check_decode_utf16_char_impl(std::u16string_view str, size_t& pos) {
            if (pos >= str.size())
                throw std::out_of_range("UTF-16 decoding position is out of range");
            auto end = pos;
            auto c = decode_utf16_char_impl(str, end);
            if (c == not_unicode)
                throw UnicodeError(str, pos);
            pos = end;
            return c;
        }

        char32_t check_decode_utf32_char_impl(std::u32string_view str, size_t& pos) {
            if (pos >= str.size())
                throw std::out_of_range("Wide character decoding position is out of range");
            auto c = str[pos];
            if (! is_unicode(c))
                throw UnicodeError(str, pos);
            ++pos;
            return c;
        }

        char32_t check_decode_char_impl(std::string_view str, size_t& pos) {
            if (pos >= str.size())
                throw std::out_of_range("UTF-8 decoding position is out of range");
            auto end = pos;
            auto c = decode_char_impl(str, end);
            if (c == not_unicode)
                throw UnicodeError(str, pos);
            pos = end;
            return c;
        }

        char32_t check_decode_char_impl(std::u16string_view str, size_t& pos) {
            return check_decode_utf16_char_impl(str, pos);
        }

        char32_t check_decode_char_impl(std::u32string_view str, size_t& pos) {
            return check_decode_utf32_char_impl(str, pos);
        }

        char32_t check_decode_char_impl(std::wstring_view str, size_t& pos) {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
                auto ptr = reinterpret_cast<const char16_t*>(str.data());
                std::u16string_view ustr(ptr, str.size());
                return check_decode_utf16_char_impl(ustr, pos);
            } else {
                auto ptr = reinterpret_cast<const char32_t*>(str.data());
                std::u32string_view ustr(ptr, str.size());
                return check_decode_utf32_char_impl(ustr, pos);
            }
        }

        bool encode_utf16_char_impl(char32_t c, std::function<void(char16_t)> out) {
            if (! is_unicode(c)) {
                return false;
            } else if (c <= 0xffff) {
                out(char16_t(c));
            } else {
                out(char16_t(c >> 10) + 0xd7c0);
                out(char16_t(c & 0x3ff) + 0xdc00);
            }
            return true;
        }

        bool encode_utf32_char_impl(char32_t c, std::function<void(char32_t)> out) {
            if (! is_unicode(c))
                return false;
            out(c);
            return true;
        }

        bool encode_char_impl(char32_t c, std::string& str) {
            if (! is_unicode(c))
                return false;
            if (c <= 0x7f) {
                str += char(c);
            } else if (c <= 0x7ff) {
                str += char(0xc0 | uint8_t(c >> 6));
                str += char(0x80 | uint8_t(c & 0x3f));
            } else if (c <= 0xffff) {
                str += char(0xe0 | uint8_t(c >> 12));
                str += char(0x80 | uint8_t((c >> 6) & 0x3f));
                str += char(0x80 | uint8_t(c & 0x3f));
            } else {
                str += char(0xf0 | uint8_t(c >> 18));
                str += char(0x80 | uint8_t((c >> 12) & 0x3f));
                str += char(0x80 | uint8_t((c >> 6) & 0x3f));
                str += char(0x80 | uint8_t(c & 0x3f));
            }
            return true;
        }

        bool encode_char_impl(char32_t c, std::u16string& str) {
            return encode_utf16_char_impl(c, [&str] (char16_t c) { str += c; });
        }

        bool encode_char_impl(char32_t c, std::u32string& str) {
            return encode_utf32_char_impl(c, [&str] (char32_t c) { str += c; });
        }

        bool encode_char_impl(char32_t c, std::wstring& str) {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t))
                return encode_utf16_char_impl(c, [&str] (char16_t c) { str += wchar_t(c); });
            else
                return encode_utf32_char_impl(c, [&str] (char32_t c) { str += wchar_t(c); });
        }

    }

    std::string to_utf8(std::u32string_view utf32) {
        std::string out;
        encode_string(utf32, out);
        return out;
    }

    std::u16string to_utf16(std::u32string_view utf32) {
        std::u16string out;
        encode_string(utf32, out);
        return out;
    }

    std::u32string to_utf32(std::u32string_view utf32) {
        std::u32string out;
        encode_string(utf32, out);
        return out;
    }

    std::wstring to_wstring(std::u32string_view utf32) {
        std::wstring out;
        encode_string(utf32, out);
        return out;
    }

    UtfIterator::UtfIterator(std::string_view utf8, size_t pos, bool checked):
    utf8_(utf8), pos_(pos), next_(pos), checked_(checked) {
        ++*this;
    }

    UtfIterator& UtfIterator::operator++() {
        pos_ = next_;
        if (pos_ < utf8_.size()) {
            char_ = decode_char(utf8_, next_);
            if (char_ == not_unicode) {
                if (checked_)
                    throw UnicodeError(utf8_, pos_);
                char_ = replacement_char;
            }
        }
        return *this;
    }

    GraphemeIterator::GraphemeIterator(std::string_view utf8, size_t pos, bool checked):
    source_(utf8), current_(utf8.substr(pos, 0)), checked_(checked) {
        using namespace Detail;
        if (pos >= utf8.size())
            return;
        auto q = pos;
        auto c = decode_char(source_, q);
        peek_ = source_.substr(pos, q - pos);
        uchars_ += c;
        gcs_.push_back(general_category(c));
        gcbs_.push_back(grapheme_cluster_break(c));
        eps_.push_back(is_extended_pictographic(c));
        ++*this;
    }

    namespace {

        using GCB = Detail::Grapheme_Cluster_Break;

        bool continue_grapheme(char32_t /*first*/, char32_t last, char32_t next,
                GCB /*first_gcb*/, GCB last_gcb, GCB next_gcb,
                bool first_ep, bool /*last_ep*/, bool next_ep,
                size_t prefix_length) noexcept {

            // https://www.unicode.org/reports/tr29/tr29-41.html#Grapheme_Cluster_Boundaries

            //  Break at the start and end of text, unless the text is empty.
            //      GB1.    sot <break> Any
            //      GB2.    Any <break> eot
            //  Do not break between a CR and LF. Otherwise, break before and after controls.
            //      GB3.    CR <join> LF
            //      GB4.    (Control | CR | LF) <break>
            //      GB5.    <break> (Control | CR | LF)
            //  Do not break Hangul syllable sequences.
            //      GB6.    L <join> (L | V | LV | LVT)
            //      GB7.    (LV | V) <join> (V | T)
            //      GB8.    (LVT | T) <join> T
            //  Do not break before extending characters or ZWJ.
            //      GB9.    <join> (Extend | ZWJ)
            //  Do not break before SpacingMarks, or after Prepend characters.
            //      GB9a.   <join> SpacingMark
            //      GB9b.   Prepend <join>
            //  Do not break within emoji modifier sequences or emoji zwj sequences.
            //      GB11.   \p{Extended_Pictographic} Extend* ZWJ <join> \p{Extended_Pictographic}
            //  Do not break within emoji flag sequences.
            //  That is, do not break between regional indicator (RI) symbols if there is an odd number of RI characters before the break point.
            //      GB12.   sot (RI RI)* RI <join> RI
            //      GB13.   [^RI] (RI RI)* RI <join> RI
            //  Otherwise, break everywhere.
            //      GB999.  Any <break> Any

            using namespace Detail;

            if (last == U'\r' && next == U'\n')
                return true;
            else if (last_gcb == GCB::Control || next_gcb == GCB::Control)
                return true;
            else if (last_gcb == GCB::L && (next_gcb == GCB::L || next_gcb == GCB::V || next_gcb == GCB::LV || next_gcb == GCB::LVT))
                return true;
            else if ((last_gcb == GCB::LV || last_gcb == GCB::V) && (next_gcb == GCB::V || next_gcb == GCB::T))
                return true;
            else if ((last_gcb == GCB::LVT || last_gcb == GCB::T) && next_gcb == GCB::T)
                return true;
            else if (next_gcb == GCB::Extend || next_gcb == GCB::ZWJ)
                return true;
            else if (last_gcb == GCB::Prepend)
                return true;
            else if (first_ep && last_gcb == GCB::ZWJ && next_ep)
                return true;
            else if (prefix_length == 1 && last_gcb == GCB::Regional_Indicator && next_gcb == GCB::Regional_Indicator)
                return true;
            else
                return false;

        }

    }

    GraphemeIterator& GraphemeIterator::operator++() {

        using namespace Detail;

        // Discard current grapheme
        current_ = view_begin(peek_);
        uchars_.erase(uchars_.begin(), uchars_.end() - 1);
        gcs_.erase(gcs_.begin(), gcs_.end() - 1);
        gcbs_.erase(gcbs_.begin(), gcbs_.end() - 1);
        eps_.erase(eps_.begin(), eps_.end() - 1);
        if (peek_.empty())
            return *this;

        for (;;) {

            // Check peek for valid Unicode
            auto peek_pos = view_pos(source_, peek_);
            if (uchars_[0] == not_unicode) {
                if (checked_)
                    throw UnicodeError(source_, peek_pos);
                uchars_[0] = replacement_char;
            }

            // Append peek to current
            current_ = view_cat(current_, peek_);
            peek_pos += peek_.size();
            if (peek_pos == source_.size()) {
                peek_ = view_end(source_);
                break;
            }

            // Get next peek
            auto q = peek_pos;
            auto c = decode_char(source_, q);
            peek_ = source_.substr(peek_pos, q - peek_pos);
            uchars_ += c;
            gcs_.push_back(general_category(c));
            gcbs_.push_back(grapheme_cluster_break(c));
            eps_.push_back(is_extended_pictographic(c));

            // Check peek for grapheme continuation
            size_t n_prefix = uchars_.size() - 1;
            if (! continue_grapheme(uchars_[0], uchars_[n_prefix - 1], uchars_.back(),
                    gcbs_[0], gcbs_[n_prefix - 1], gcbs_.back(),
                    eps_[0], eps_[n_prefix - 1], eps_.back(),
                    n_prefix))
                break;

        }

        return *this;

    }

    namespace {

        template <typename T>
        T find_in_ucd_table(char32_t c, const Detail::UcdPropertyTable<T>& table) noexcept {
            auto pair = std::make_pair(c, T());
            auto it = std::lower_bound(table.begin(), table.end(), pair);
            if (it == table.end() || it->first > c)
                --it;
            return it->second;
        }

    }

    namespace Detail {

        int canonical_combining_class(char32_t c) {
            return find_in_ucd_table(c, canonical_combining_class_table());
        }

        East_Asian_Width east_asian_width(char32_t c) {
            return find_in_ucd_table(c, east_asian_width_table());
        }

        Grapheme_Cluster_Break grapheme_cluster_break(char32_t c) {
            return find_in_ucd_table(c, grapheme_cluster_break_table());
        }

        bool is_full_composition_exclusion(char32_t c) {
            return find_in_ucd_table(c, full_composition_exclusion_table());
        }

        bool is_extended_pictographic(char32_t c) {
            return find_in_ucd_table(c, extended_pictographic_table());
        }

        namespace {

            constexpr uint32_t sbase = 0xac00;
            constexpr uint32_t lbase = 0x1100;
            constexpr uint32_t vbase = 0x1161;
            constexpr uint32_t tbase = 0x11a7;
            constexpr uint32_t tcount = 28;
            constexpr uint32_t ncount = 588;

            std::pair<char32_t, char32_t> hangul_decomposition(char32_t c) {
                std::pair<char32_t, char32_t> result = {0, 0};
                auto hst = hangul_syllable_type(c);
                if (hst != Hangul_Syllable_Type::LV && hst != Hangul_Syllable_Type::LVT)
                    return result;
                uint32_t sindex = c - sbase;
                if (hst == Hangul_Syllable_Type::LV) {
                    uint32_t lindex = sindex / ncount;
                    uint32_t vindex = (sindex % ncount) / tcount;
                    result.first = lbase + lindex;
                    result.second = vbase + vindex;
                } else {
                    uint32_t lvindex = (sindex / tcount) * tcount;
                    uint32_t tindex = sindex % tcount;
                    result.first = sbase + lvindex;
                    result.second = tbase + tindex;
                }
                return result;
            }

            char32_t hangul_composition(char32_t c1, char32_t c2) noexcept {
                auto hst1 = hangul_syllable_type(c1);
                auto hst2 = hangul_syllable_type(c2);
                if (hst1 == Hangul_Syllable_Type::L && hst2 == Hangul_Syllable_Type::V) {
                    uint32_t lindex = c1 - lbase;
                    uint32_t vindex = c2 - vbase;
                    uint32_t lvindex = lindex * ncount + vindex * tcount;
                    return sbase + lvindex;
                } else if (hst1 == Hangul_Syllable_Type::LV && hst2 == Hangul_Syllable_Type::T) {
                    auto lv = hangul_decomposition(c1);
                    uint32_t lindex = lv.first - lbase;
                    uint32_t vindex = lv.second - vbase;
                    uint32_t tindex = c2 - tbase;
                    uint32_t lvindex = lindex * ncount + vindex * tcount;
                    return sbase + lvindex + tindex;
                } else {
                    return 0;
                }
            }

        }

        char32_t canonical_composition(char32_t c1, char32_t c2) {
            if (c1 == 0 || c2 == 0)
                return 0;
            char32_t hc = hangul_composition(c1, c2);
            if (hc != 0)
                return hc;
            static const auto map = [] {
                std::map<std::pair<char32_t, char32_t>, char32_t> dcmap;
                for (auto& [k,v]: canonical_decomposition_mapping_table())
                    if (v.second != 0 && ! is_full_composition_exclusion(k))
                        dcmap.insert({v,k});
                return dcmap;
            }();
            auto it = map.find(std::make_pair(c1, c2));
            if (it == map.end())
                return 0;
            else
                return it->second;
        }

        std::pair<char32_t, char32_t> canonical_decomposition(char32_t c) {
            auto pair = hangul_decomposition(c);
            if (pair.first != 0)
                return pair;
            auto it = canonical_decomposition_mapping_table().find(c);
            if (it == canonical_decomposition_mapping_table().end())
                return {0,0};
            else
                return it->second;
        }

        Hangul_Syllable_Type hangul_syllable_type(char32_t c) noexcept {
            if ((c >= 0x1100 && c <= 0x115f) || (c >= 0xa960 && c <= 0xa97c)) {
                return Hangul_Syllable_Type::L;
            } else if ((c >= 0x1160 && c <= 0x11a7) || (c >= 0xd7b0 && c <= 0xd7c6)) {
                return Hangul_Syllable_Type::V;
            } else if ((c >= 0x11a8 && c <= 0x11ff) || (c >= 0xd7cb && c <= 0xd7fb)) {
                return Hangul_Syllable_Type::T;
            } else if (c >= 0xac00 && c <= 0xd7a3) {
                if ((c - 0xac00) % 28 == 0)
                    return Hangul_Syllable_Type::LV;
                else
                    return Hangul_Syllable_Type::LVT;
            } else {
                return Hangul_Syllable_Type::NA;
            }
        }

    }

    GC general_category(char32_t c) {
        return find_in_ucd_table(c, Detail::general_category_table());
    }

    bool is_pattern_syntax(char32_t c) {
        return find_in_ucd_table(c, Detail::pattern_syntax_table());
    }

    bool is_xid_start(char32_t c) {
        return find_in_ucd_table(c, Detail::xid_start_table());
    }

    bool is_xid_continue(char32_t c) {
        return find_in_ucd_table(c, Detail::xid_continue_table());
    }

    namespace {

        size_t character_width(char32_t c, GC gc) {
            using namespace Detail;
            if (gc == GC::Cc || gc == GC::Cf || gc == GC::Mn || gc == GC::Sk)
                return 0;
            auto eaw = east_asian_width(c);
            if (eaw == East_Asian_Width::F || eaw == East_Asian_Width::W)
                return 2;
            else
                return 1;
        }

    }

    size_t utf_size(std::string_view str, Usize mode) {

        switch (mode) {

            case Usize::units: {
                return str.size();
            }

            case Usize::scalars: {
                size_t n = 0;
                size_t pos = 0;
                while (pos < str.size()) {
                    check_decode_char(str, pos);
                    ++n;
                }
                return n;
            }

            case Usize::graphemes: {
                auto range = graphemes(str, true);
                return size_t(std::distance(range.begin(), range.end()));
            }

            case Usize::columns: {
                size_t n = 0;
                auto range = graphemes(str, true);
                for (auto g = range.begin(); g != range.end(); ++g) {
                    if (g.uchars_.size() >= 2 && g.gcbs_[0] == GCB::Regional_Indicator && g.gcbs_[1] == GCB::Regional_Indicator)
                        n += 2;
                    else
                        n += character_width(g.uchars_[0], g.gcs_[0]);
                }
                return n;
            }

        }

    }

}
