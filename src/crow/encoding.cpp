#include "crow/encoding.hpp"
#include "crow/string.hpp"
#include <array>
#include <utility>

namespace Crow {

    // Exceptions

    EncodingError::EncodingError(const std::string& encoding):
    std::runtime_error("Encoding error: invalid " + encoding) {}

    // Class Encoding

    std::string Encoding::encode(const void* in, size_t len) const {

        auto str = do_encode(in, len);

        if (line_ > 0 && line_ < npos && ! str.empty()) {

            std::string lines;
            size_t n = line_;

            for (size_t pos = 0; pos < str.size(); pos += n) {

                if (pos + line_ < str.size()) {

                    while (n > 0 && ! can_break(str, pos + n))
                        --n;

                    if (n == 0) {
                        n = line_ + 1;
                        while (n < str.size() && ! can_break(str, pos + n))
                            ++n;
                    }

                }

                lines.append(str, pos, n);
                lines += '\n';

            }

            if (lines.back() == '\n')
                lines.pop_back();

            str = std::move(lines);

        }

        return str;

    }

    std::string Encoding::decode(std::string_view in) const {

        std::string str(in.size(), '\0');
        size_t len = 0;

        for (;;) {
            len = do_decode(in, str.data(), str.size());
            if (len != npos)
                break;
            str.append(in.size(), '\0');
        }

        str.resize(len);

        return str;

    }

    // Class Hexcode

    std::string Hexcode::do_encode(const void* in, size_t len) const {

        static constexpr const char* xdigits = "0123456789abcdef";

        auto in_ptr = static_cast<const uint8_t*>(in);
        std::string str;

        for (size_t i = 0; i < len; ++i) {
            str += xdigits[in_ptr[i] / 16];
            str += xdigits[in_ptr[i] % 16];
        }

        return str;

    }

    size_t Hexcode::do_decode(std::string_view in, void* out, size_t maxlen) const {

        auto out_ptr = static_cast<uint8_t*>(out);
        size_t in_pos = 0;
        size_t out_pos = 0;

        while (in_pos < in.size()) {

            if (out_pos >= maxlen)
                return npos;

            if (in_pos == in.size() - 1)
                throw EncodingError(name());

            auto c = in[in_pos];
            auto b = uint8_t(c);

            if (b <= 127 && ! ascii_isalnum(c)) {
                ++in_pos;
                continue;
            }

            int value = 0;

            for (size_t i = 0; i < 2; ++i) {

                c = in[in_pos++];
                b = uint8_t(c);

                if (b > 126 || ! ascii_isxdigit(c))
                    throw EncodingError(name());

                if (c <= '9')
                    value = 16 * value + int(c - '0');
                else if (c <= 'F')
                    value = 16 * value + int(c - 'A' + 10);
                else
                    value = 16 * value + int(c - 'a' + 10);

            }

            out_ptr[out_pos++] = uint8_t(value);

        }

        return out_pos;

    }

    // Class Base64

    Base64::Base64(size_t line, const std::string& last2, bool padding):
    Encoding(line),
    last2_(last2),
    padding_(padding) {
        if (last2.size() != 2 || ! ascii_ispunct(last2[0]) || ! ascii_ispunct(last2[1]) || last2[0] == last2[1])
            throw std::invalid_argument("Invalid Base64 coding");
    }

    std::string Base64::do_encode(const void* in, size_t len) const {

        size_t tail_len = len % 3;
        auto in_ptr = static_cast<const uint8_t*>(in);
        auto in_end = in_ptr + len - tail_len;
        std::string str;

        auto convert_group = [&] (int n) {
            uint32_t g = 0;
            for (int i = 6 * n - 8; i >= 0; i -= 8)
                g += uint32_t(*in_ptr++) << i;
            for (int i = 6 * n - 6; i >= 0; i -= 6) {
                uint32_t x = (g >> i) & 0x3f;
                str += x < 26 ? char(x + 'A') :
                    x < 52 ? char(x - 26 + 'a') :
                    x < 62 ? char(x - 52 + '0') :
                    last2_[x - 62];
            }
        };

        while (in_ptr != in_end)
            convert_group(4);

        if (tail_len > 0) {
            convert_group(int(tail_len) + 1);
            if (padding_)
                str.append(3 - tail_len, '=');
        }

        return str;

    }

    size_t Base64::do_decode(std::string_view in, void* out, size_t maxlen) const {

        auto out_ptr = static_cast<uint8_t*>(out);
        size_t out_pos = 0;
        std::array<char, 4> group;
        size_t gpos = 0;
        bool done = false;

        auto convert_group = [&] () {

            uint32_t g = 0;
            int bits = 0;

            for (char c: group) {

                if (c == '=') {
                    done = true;
                    break;
                }

                if (done)
                    throw EncodingError(name());

                uint32_t x;

                if (c >= 'A' && c <= 'Z')
                    x = uint32_t(c - 'A');
                else if (c >= 'a' && c <= 'z')
                    x = uint32_t(c - 'a' + 26);
                else if (c >= '0' && c <= '9')
                    x = uint32_t(c - '0' + 52);
                else if (c == last2_[0])
                    x = 62;
                else if (c == last2_[1])
                    x = 63;
                else
                    throw EncodingError(name());

                g = (g << 6) + x;
                bits += 6;

            }

            for (int i = bits - 8; i >= 0; i -= 8) {
                if (out_pos >= maxlen) {
                    out_pos = npos;
                    break;
                }
                out_ptr[out_pos++] = uint8_t(g >> i);
            }

        };

        for (char c: in) {

            if (uint8_t(c) > 126)
                throw EncodingError(name());

            if (ascii_isgraph(c)) {
                group[gpos++] = c;
                if (gpos == 4) {
                    convert_group();
                    gpos = 0;
                }
            }

        }

        if (gpos == 1)
            throw EncodingError(name());

        if (gpos != 0) {
            while (gpos < 4)
                group[gpos++] = '=';
            convert_group();
        }

        return out_pos;

    }

}
