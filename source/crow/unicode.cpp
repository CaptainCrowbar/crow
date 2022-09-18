#include "crow/unicode.hpp"
#include <algorithm>

namespace Crow {

    namespace Detail {

        std::string hex_char32(char32_t c) {
            std::string hex = "U+";
            for (; c > 0 || hex.size() < 6; c >>= 4)
                hex += hex_digits_uc[c % 16];
            std::reverse(hex.begin() + 2, hex.end());
            return hex;
        }

        char32_t decode_char_impl(const std::string& str, size_t& pos) noexcept {

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

        char32_t decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos) noexcept {

            // UTF-16 code unit distribution:
            // 0000-d7ff = BMP character
            // d800-dbff = High surrogate
            // dc00-dfff = Low surrogate
            // e000-ffff = BMP character

            static constexpr auto is_start = [] (char16_t c) constexpr {
                return c <= 0xdbff || c >= 0xe000;
            };

            auto fail = [ptr,len,&pos] {
                size_t max_pos = std::min(pos + 2, len);
                do ++pos;
                    while (pos < max_pos && ! is_start(ptr[pos]));
                return not_unicode;
            };

            if (pos >= len)
                return not_unicode;

            if (ptr[pos] <= 0xd7ff || ptr[pos] >= 0xe000)
                return char32_t(ptr[pos++]);

            if (len - pos < 2 || ptr[pos] >= 0xdc00
                    || ptr[pos + 1] <= 0xdbff || ptr[pos + 1] >= 0xe000) {
                return fail();
            }

            char32_t c = (char32_t(ptr[pos] & 0x3ff) << 10) + (ptr[pos + 1] & 0x3ff) + 0x10000;
            pos += 2;

            return c;

        }

        char32_t decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos) noexcept {
            if (pos >= len)
                return not_unicode;
            auto c = ptr[pos++];
            if (is_unicode(c))
                return c;
            else
                return not_unicode;
        }

        char32_t decode_char_impl(const std::u16string& str, size_t& pos) noexcept {
            return Detail::decode_utf16_char_impl(str.data(), str.size(), pos);
        }

        char32_t decode_char_impl(const std::u32string& str, size_t& pos) noexcept {
            return Detail::decode_utf32_char_impl(str.data(), str.size(), pos);
        }

        char32_t decode_char_impl(const std::wstring& str, size_t& pos) noexcept {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
                auto ptr = reinterpret_cast<const char16_t*>(str.data());
                return Detail::decode_utf16_char_impl(ptr, str.size(), pos);
            } else {
                auto ptr = reinterpret_cast<const char32_t*>(str.data());
                return Detail::decode_utf32_char_impl(ptr, str.size(), pos);
            }
        }

        char32_t check_decode_utf16_char_impl(const char16_t* ptr, size_t len, size_t& pos) {
            if (pos >= len)
                throw std::out_of_range("UTF-16 decoding position is out of range");
            auto start = pos;
            auto c = decode_utf16_char_impl(ptr, len, pos);
            if (c == not_unicode) {
                size_t n = pos - start;
                pos = start;
                auto hex = Detail::hex_data(ptr + start, n);
                throw std::invalid_argument("Invalid UTF-16: " + hex);
            }
            return c;
        }

        char32_t check_decode_utf32_char_impl(const char32_t* ptr, size_t len, size_t& pos) {
            if (pos >= len)
                throw std::out_of_range("Wide character decoding position is out of range");
            auto c = ptr[pos++];
            if (! is_unicode(c))
                throw std::invalid_argument("Invalid Unicode character: " + Detail::hex_char32(c));
            return c;
        }

        char32_t check_decode_char_impl(const std::string& str, size_t& pos) {
            if (pos >= str.size())
                throw std::out_of_range("UTF-8 decoding position is out of range");
            auto start = pos;
            auto c = decode_char_impl(str, pos);
            if (c == not_unicode) {
                size_t len = pos - start;
                pos = start;
                auto bytes = Detail::hex_data(str.data() + start, len);
                throw std::invalid_argument("Invalid UTF-8: " + bytes);
            }
            return c;
        }

        char32_t check_decode_char_impl(const std::u16string& str, size_t& pos) {
            return Detail::check_decode_utf16_char_impl(str.data(), str.size(), pos);
        }

        char32_t check_decode_char_impl(const std::u32string& str, size_t& pos) {
            return Detail::check_decode_utf32_char_impl(str.data(), str.size(), pos);
        }

        char32_t check_decode_char_impl(const std::wstring& str, size_t& pos) {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
                auto ptr = reinterpret_cast<const char16_t*>(str.data());
                return Detail::check_decode_utf16_char_impl(ptr, str.size(), pos);
            } else {
                auto ptr = reinterpret_cast<const char32_t*>(str.data());
                return Detail::check_decode_utf32_char_impl(ptr, str.size(), pos);
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
            return Detail::encode_utf16_char_impl(c, [&str] (char16_t c) { str += c; });
        }

        bool encode_char_impl(char32_t c, std::u32string& str) {
            return Detail::encode_utf32_char_impl(c, [&str] (char32_t c) { str += c; });
        }

        bool encode_char_impl(char32_t c, std::wstring& str) {
            if constexpr (sizeof(wchar_t) == sizeof(char16_t))
                return Detail::encode_utf16_char_impl(c, [&str] (char16_t c) { str += wchar_t(c); });
            else
                return Detail::encode_utf32_char_impl(c, [&str] (char32_t c) { str += wchar_t(c); });
        }

    }

    std::string to_utf8(const std::u32string& utf32) {
        std::string out;
        encode_string(utf32, out);
        return out;
    }

    std::u16string to_utf16(const std::u32string& utf32) {
        std::u16string out;
        encode_string(utf32, out);
        return out;
    }

    std::u32string to_utf32(const std::u32string& utf32) {
        std::u32string out;
        encode_string(utf32, out);
        return out;
    }

    std::wstring to_wstring(const std::u32string& utf32) {
        std::wstring out;
        encode_string(utf32, out);
        return out;
    }

    namespace Detail {

        const CodeSet& gc_is_cc_cf_mn() {

            static const CodeSet set = {

                // Characters with General_Category = Cc (Control), Cf (Format), or Mn (Nonspacing_Mark)

                {0x0000,0x001f},    {0x007f,0x009f},    {0x00ad},           {0x0300,0x036f},    {0x0483,0x0487},
                {0x0591,0x05bd},    {0x05bf},           {0x05c1,0x05c2},    {0x05c4,0x05c5},    {0x05c7},
                {0x0600,0x0605},    {0x0610,0x061a},    {0x061c},           {0x064b,0x065f},    {0x0670},
                {0x06d6,0x06dd},    {0x06df,0x06e4},    {0x06e7,0x06e8},    {0x06ea,0x06ed},    {0x070f},
                {0x0711},           {0x0730,0x074a},    {0x07a6,0x07b0},    {0x07eb,0x07f3},    {0x07fd},
                {0x0816,0x0819},    {0x081b,0x0823},    {0x0825,0x0827},    {0x0829,0x082d},    {0x0859,0x085b},
                {0x0890,0x0891},    {0x0898,0x089f},    {0x08ca,0x0902},    {0x093a},           {0x093c},
                {0x0941,0x0948},    {0x094d},           {0x0951,0x0957},    {0x0962,0x0963},    {0x0981},
                {0x09bc},           {0x09c1,0x09c4},    {0x09cd},           {0x09e2,0x09e3},    {0x09fe},
                {0x0a01,0x0a02},    {0x0a3c},           {0x0a41,0x0a42},    {0x0a47,0x0a48},    {0x0a4b,0x0a4d},
                {0x0a51},           {0x0a70,0x0a71},    {0x0a75},           {0x0a81,0x0a82},    {0x0abc},
                {0x0ac1,0x0ac5},    {0x0ac7,0x0ac8},    {0x0acd},           {0x0ae2,0x0ae3},    {0x0afa,0x0aff},
                {0x0b01},           {0x0b3c},           {0x0b3f},           {0x0b41,0x0b44},    {0x0b4d},
                {0x0b55,0x0b56},    {0x0b62,0x0b63},    {0x0b82},           {0x0bc0},           {0x0bcd},
                {0x0c00},           {0x0c04},           {0x0c3c},           {0x0c3e,0x0c40},    {0x0c46,0x0c48},
                {0x0c4a,0x0c4d},    {0x0c55,0x0c56},    {0x0c62,0x0c63},    {0x0c81},           {0x0cbc},
                {0x0cbf},           {0x0cc6},           {0x0ccc,0x0ccd},    {0x0ce2,0x0ce3},    {0x0d00,0x0d01},
                {0x0d3b,0x0d3c},    {0x0d41,0x0d44},    {0x0d4d},           {0x0d62,0x0d63},    {0x0d81},
                {0x0dca},           {0x0dd2,0x0dd4},    {0x0dd6},           {0x0e31},           {0x0e34,0x0e3a},
                {0x0e47,0x0e4e},    {0x0eb1},           {0x0eb4,0x0ebc},    {0x0ec8,0x0ecd},    {0x0f18,0x0f19},
                {0x0f35},           {0x0f37},           {0x0f39},           {0x0f71,0x0f7e},    {0x0f80,0x0f84},
                {0x0f86,0x0f87},    {0x0f8d,0x0f97},    {0x0f99,0x0fbc},    {0x0fc6},           {0x102d,0x1030},
                {0x1032,0x1037},    {0x1039,0x103a},    {0x103d,0x103e},    {0x1058,0x1059},    {0x105e,0x1060},
                {0x1071,0x1074},    {0x1082},           {0x1085,0x1086},    {0x108d},           {0x109d},
                {0x135d,0x135f},    {0x1712,0x1714},    {0x1732,0x1733},    {0x1752,0x1753},    {0x1772,0x1773},
                {0x17b4,0x17b5},    {0x17b7,0x17bd},    {0x17c6},           {0x17c9,0x17d3},    {0x17dd},
                {0x180b,0x180f},    {0x1885,0x1886},    {0x18a9},           {0x1920,0x1922},    {0x1927,0x1928},
                {0x1932},           {0x1939,0x193b},    {0x1a17,0x1a18},    {0x1a1b},           {0x1a56},
                {0x1a58,0x1a5e},    {0x1a60},           {0x1a62},           {0x1a65,0x1a6c},    {0x1a73,0x1a7c},
                {0x1a7f},           {0x1ab0,0x1abd},    {0x1abf,0x1ace},    {0x1b00,0x1b03},    {0x1b34},
                {0x1b36,0x1b3a},    {0x1b3c},           {0x1b42},           {0x1b6b,0x1b73},    {0x1b80,0x1b81},
                {0x1ba2,0x1ba5},    {0x1ba8,0x1ba9},    {0x1bab,0x1bad},    {0x1be6},           {0x1be8,0x1be9},
                {0x1bed},           {0x1bef,0x1bf1},    {0x1c2c,0x1c33},    {0x1c36,0x1c37},    {0x1cd0,0x1cd2},
                {0x1cd4,0x1ce0},    {0x1ce2,0x1ce8},    {0x1ced},           {0x1cf4},           {0x1cf8,0x1cf9},
                {0x1dc0,0x1dff},    {0x200b,0x200f},    {0x202a,0x202e},    {0x2060,0x2064},    {0x2066,0x206f},
                {0x20d0,0x20dc},    {0x20e1},           {0x20e5,0x20f0},    {0x2cef,0x2cf1},    {0x2d7f},
                {0x2de0,0x2dff},    {0x302a,0x302d},    {0x3099,0x309a},    {0xa66f},           {0xa674,0xa67d},
                {0xa69e,0xa69f},    {0xa6f0,0xa6f1},    {0xa802},           {0xa806},           {0xa80b},
                {0xa825,0xa826},    {0xa82c},           {0xa8c4,0xa8c5},    {0xa8e0,0xa8f1},    {0xa8ff},
                {0xa926,0xa92d},    {0xa947,0xa951},    {0xa980,0xa982},    {0xa9b3},           {0xa9b6,0xa9b9},
                {0xa9bc,0xa9bd},    {0xa9e5},           {0xaa29,0xaa2e},    {0xaa31,0xaa32},    {0xaa35,0xaa36},
                {0xaa43},           {0xaa4c},           {0xaa7c},           {0xaab0},           {0xaab2,0xaab4},
                {0xaab7,0xaab8},    {0xaabe,0xaabf},    {0xaac1},           {0xaaec,0xaaed},    {0xaaf6},
                {0xabe5},           {0xabe8},           {0xabed},           {0xfb1e},           {0xfe00,0xfe0f},
                {0xfe20,0xfe2f},    {0xfeff},           {0xfff9,0xfffb},    {0x101fd},          {0x102e0},
                {0x10376,0x1037a},  {0x10a01,0x10a03},  {0x10a05,0x10a06},  {0x10a0c,0x10a0f},  {0x10a38,0x10a3a},
                {0x10a3f},          {0x10ae5,0x10ae6},  {0x10d24,0x10d27},  {0x10eab,0x10eac},  {0x10f46,0x10f50},
                {0x10f82,0x10f85},  {0x11001},          {0x11038,0x11046},  {0x11070},          {0x11073,0x11074},
                {0x1107f,0x11081},  {0x110b3,0x110b6},  {0x110b9,0x110ba},  {0x110bd},          {0x110c2},
                {0x110cd},          {0x11100,0x11102},  {0x11127,0x1112b},  {0x1112d,0x11134},  {0x11173},
                {0x11180,0x11181},  {0x111b6,0x111be},  {0x111c9,0x111cc},  {0x111cf},          {0x1122f,0x11231},
                {0x11234},          {0x11236,0x11237},  {0x1123e},          {0x112df},          {0x112e3,0x112ea},
                {0x11300,0x11301},  {0x1133b,0x1133c},  {0x11340},          {0x11366,0x1136c},  {0x11370,0x11374},
                {0x11438,0x1143f},  {0x11442,0x11444},  {0x11446},          {0x1145e},          {0x114b3,0x114b8},
                {0x114ba},          {0x114bf,0x114c0},  {0x114c2,0x114c3},  {0x115b2,0x115b5},  {0x115bc,0x115bd},
                {0x115bf,0x115c0},  {0x115dc,0x115dd},  {0x11633,0x1163a},  {0x1163d},          {0x1163f,0x11640},
                {0x116ab},          {0x116ad},          {0x116b0,0x116b5},  {0x116b7},          {0x1171d,0x1171f},
                {0x11722,0x11725},  {0x11727,0x1172b},  {0x1182f,0x11837},  {0x11839,0x1183a},  {0x1193b,0x1193c},
                {0x1193e},          {0x11943},          {0x119d4,0x119d7},  {0x119da,0x119db},  {0x119e0},
                {0x11a01,0x11a0a},  {0x11a33,0x11a38},  {0x11a3b,0x11a3e},  {0x11a47},          {0x11a51,0x11a56},
                {0x11a59,0x11a5b},  {0x11a8a,0x11a96},  {0x11a98,0x11a99},  {0x11c30,0x11c36},  {0x11c38,0x11c3d},
                {0x11c3f},          {0x11c92,0x11ca7},  {0x11caa,0x11cb0},  {0x11cb2,0x11cb3},  {0x11cb5,0x11cb6},
                {0x11d31,0x11d36},  {0x11d3a},          {0x11d3c,0x11d3d},  {0x11d3f,0x11d45},  {0x11d47},
                {0x11d90,0x11d91},  {0x11d95},          {0x11d97},          {0x11ef3,0x11ef4},  {0x13430,0x13438},
                {0x16af0,0x16af4},  {0x16b30,0x16b36},  {0x16f4f},          {0x16f8f,0x16f92},  {0x16fe4},
                {0x1bc9d,0x1bc9e},  {0x1bca0,0x1bca3},  {0x1cf00,0x1cf2d},  {0x1cf30,0x1cf46},  {0x1d167,0x1d169},
                {0x1d173,0x1d182},  {0x1d185,0x1d18b},  {0x1d1aa,0x1d1ad},  {0x1d242,0x1d244},  {0x1da00,0x1da36},
                {0x1da3b,0x1da6c},  {0x1da75},          {0x1da84},          {0x1da9b,0x1da9f},  {0x1daa1,0x1daaf},
                {0x1e000,0x1e006},  {0x1e008,0x1e018},  {0x1e01b,0x1e021},  {0x1e023,0x1e024},  {0x1e026,0x1e02a},
                {0x1e130,0x1e136},  {0x1e2ae},          {0x1e2ec,0x1e2ef},  {0x1e8d0,0x1e8d6},  {0x1e944,0x1e94a},
                {0xe0001},          {0xe0020,0xe007f},  {0xe0100,0xe01ef},

            };

            return set;

        }

        const CodeSet& ws_pws() {

            static const CodeSet set = {

                // Characters with White_Space or Pattern_White_Space property

                {0x0009,0x000d},  {0x0020},         {0x0085},         {0x00a0},  {0x1680},
                {0x2000,0x200a},  {0x200e,0x200f},  {0x2028,0x2029},  {0x202f},  {0x205f},
                {0x3000},

            };

            return set;

        }

        const CodeSet& eaw_is_f_w() {

            static const CodeSet set = {

                // Characters with East_Asian_Width = F (Fullwidth) or W (Wide)

                {0x1100,0x115f},    {0x231a,0x231b},    {0x2329,0x232a},    {0x23e9,0x23ec},    {0x23f0},
                {0x23f3},           {0x25fd,0x25fe},    {0x2614,0x2615},    {0x2648,0x2653},    {0x267f},
                {0x2693},           {0x26a1},           {0x26aa,0x26ab},    {0x26bd,0x26be},    {0x26c4,0x26c5},
                {0x26ce},           {0x26d4},           {0x26ea},           {0x26f2,0x26f3},    {0x26f5},
                {0x26fa},           {0x26fd},           {0x2705},           {0x270a,0x270b},    {0x2728},
                {0x274c},           {0x274e},           {0x2753,0x2755},    {0x2757},           {0x2795,0x2797},
                {0x27b0},           {0x27bf},           {0x2b1b,0x2b1c},    {0x2b50},           {0x2b55},
                {0x2e80,0x2e99},    {0x2e9b,0x2ef3},    {0x2f00,0x2fd5},    {0x2ff0,0x2ffb},    {0x3000,0x303e},
                {0x3041,0x3096},    {0x3099,0x30ff},    {0x3105,0x312f},    {0x3131,0x318e},    {0x3190,0x31e3},
                {0x31f0,0x321e},    {0x3220,0x3247},    {0x3250,0x4dbf},    {0x4e00,0xa48c},    {0xa490,0xa4c6},
                {0xa960,0xa97c},    {0xac00,0xd7a3},    {0xf900,0xfaff},    {0xfe10,0xfe19},    {0xfe30,0xfe52},
                {0xfe54,0xfe66},    {0xfe68,0xfe6b},    {0xff01,0xff60},    {0xffe0,0xffe6},    {0x16fe0,0x16fe4},
                {0x16ff0,0x16ff1},  {0x17000,0x187f7},  {0x18800,0x18cd5},  {0x18d00,0x18d08},  {0x1aff0,0x1aff3},
                {0x1aff5,0x1affb},  {0x1affd,0x1affe},  {0x1b000,0x1b122},  {0x1b150,0x1b152},  {0x1b164,0x1b167},
                {0x1b170,0x1b2fb},  {0x1f004},          {0x1f0cf},          {0x1f18e},          {0x1f191,0x1f19a},
                {0x1f200,0x1f202},  {0x1f210,0x1f23b},  {0x1f240,0x1f248},  {0x1f250,0x1f251},  {0x1f260,0x1f265},
                {0x1f300,0x1f320},  {0x1f32d,0x1f335},  {0x1f337,0x1f37c},  {0x1f37e,0x1f393},  {0x1f3a0,0x1f3ca},
                {0x1f3cf,0x1f3d3},  {0x1f3e0,0x1f3f0},  {0x1f3f4},          {0x1f3f8,0x1f43e},  {0x1f440},
                {0x1f442,0x1f4fc},  {0x1f4ff,0x1f53d},  {0x1f54b,0x1f54e},  {0x1f550,0x1f567},  {0x1f57a},
                {0x1f595,0x1f596},  {0x1f5a4},          {0x1f5fb,0x1f64f},  {0x1f680,0x1f6c5},  {0x1f6cc},
                {0x1f6d0,0x1f6d2},  {0x1f6d5,0x1f6d7},  {0x1f6dd,0x1f6df},  {0x1f6eb,0x1f6ec},  {0x1f6f4,0x1f6fc},
                {0x1f7e0,0x1f7eb},  {0x1f7f0},          {0x1f90c,0x1f93a},  {0x1f93c,0x1f945},  {0x1f947,0x1f9ff},
                {0x1fa70,0x1fa74},  {0x1fa78,0x1fa7c},  {0x1fa80,0x1fa86},  {0x1fa90,0x1faac},  {0x1fab0,0x1faba},
                {0x1fac0,0x1fac5},  {0x1fad0,0x1fad9},  {0x1fae0,0x1fae7},  {0x1faf0,0x1faf6},  {0x20000,0x2fffd},
                {0x30000,0x3fffd},

            };

            return set;

        }

        size_t character_width(char32_t c) {
            if (gc_is_cc_cf_mn()(c) && ! ws_pws()(c))
                return 0;
            else if (eaw_is_f_w()(c))
                return 2;
            else
                return 1;
        }

    }

}
