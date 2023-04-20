#include "crow/uuid.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace Crow {

    // Class Uuid

    Uuid::Uuid(const std::string& str) {
        auto s = str.begin();
        auto s_end = str.end();
        auto t = begin();
        auto t_end = end();
        while (s != s_end && t != t_end) {
            s = std::find_if(s, s_end, ascii_isalnum);
            if (s == s_end)
                break;
            if (s[0] == '0' && (s[1] == 'X' || s[1] == 'x')) {
                s += 2;
                continue;
            }
            if (s == s_end || ! ascii_isxdigit(s[0]) || ! ascii_isxdigit(s[1]))
                break;
            int x = 0;
            for (int i = 0; i < 2; ++i) {
                x *= 16;
                if (s[i] <= '9')
                    x += s[i] - '0';
                else if (s[i] <= 'Z')
                    x += s[i] - 'A' + 10;
                else
                    x += s[i] - 'a' + 10;
            }
            *t++ = uint8_t(x);
            s += 2;
        }
        if (t == t_end)
            s = std::find_if(s, s_end, ascii_isalnum);
        if (s != s_end || t != t_end)
            throw std::invalid_argument("Invalid UUID: " + str);
    }

    std::string Uuid::str() const {
        std::string str;
        str.reserve(36);
        int i = 0;
        for (; i < 4; ++i)
            add_byte(bytes_[i], str);
        str += '-';
        for (; i < 6; ++i)
            add_byte(bytes_[i], str);
        str += '-';
        for (; i < 8; ++i)
            add_byte(bytes_[i], str);
        str += '-';
        for (; i < 10; ++i)
            add_byte(bytes_[i], str);
        str += '-';
        for (; i < 16; ++i)
            add_byte(bytes_[i], str);
        return str;
    }

    void Uuid::add_byte(uint8_t b, std::string& str) {
        static constexpr const char* xdigits = "0123456789abcdef";
        str += xdigits[b / 16];
        str += xdigits[b % 16];
    }

}
