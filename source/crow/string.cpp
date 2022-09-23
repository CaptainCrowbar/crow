#include "crow/string.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <cstring>
#include <memory>
#include <new>

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

namespace Crow {

    // String manipulation functions

    bool AsciiIcaseEqual::operator()(std::string_view a, std::string_view b) const noexcept {
        if (a.size() != b.size())
            return false;
        auto its = std::mismatch(a.begin(), a.end(), b.begin(),
            [] (char x, char y) { return ascii_toupper(x) == ascii_toupper(y); });
        return its.first == a.end();
    }

    bool AsciiIcaseLess::operator()(std::string_view a, std::string_view b) const noexcept {
        auto its = std::mismatch(a.begin(), a.end(), b.begin(), b.end(),
            [] (char x, char y) { return ascii_toupper(x) == ascii_toupper(y); });
        if (its.second == b.end())
            return false;
        else if (its.first == a.end())
            return true;
        else
            return ascii_toupper(*its.first) < ascii_toupper(*its.second);
    }

    std::string ascii_uppercase(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ascii_toupper);
        return str;
    }

    std::string ascii_lowercase(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ascii_tolower);
        return str;
    }

    std::string ascii_titlecase(std::string str) {
        bool is_alpha = false;
        bool was_alpha = false;
        for (char& c: str) {
            if (c == '\'')
                is_alpha = was_alpha;
            else
                is_alpha = ascii_isalpha(c);
            if (is_alpha) {
                if (was_alpha)
                    c = ascii_tolower(c);
                else
                    c = ascii_toupper(c);
            }
            was_alpha = is_alpha;
        }
        return str;
    }

    size_t common_prefix_length(const std::string& str1, const std::string& str2) noexcept {
        size_t len = std::min(str1.size(), str2.size());
        auto it = std::mismatch(str1.begin(), str1.begin() + len, str2.begin()).first;
        return it - str1.begin();
    }

    std::string pad_left(std::string str, size_t len, char c) {
        if (str.size() < len)
            str.insert(0, len - str.size(), c);
        return str;
    }

    std::string pad_right(std::string str, size_t len, char c) {
        if (str.size() < len)
            str.append(len - str.size(), c);
        return str;
    }

    std::pair<std::string, std::string> partition(const std::string& str, const std::string& chars) {
        if (chars.empty())
            return {str, {}};
        size_t i = str.find_first_of(chars);
        if (i == npos)
            return {str, {}};
        size_t j = str.find_first_not_of(chars, i);
        if (j == npos)
            return {str.substr(0, i), {}};
        else
            return {str.substr(0, i), str.substr(j)};
    }

    std::pair<std::string, std::string> partition_at(const std::string& str, const std::string& delimiter) {
        if (delimiter.empty())
            return {str, {}};
        size_t i = str.find(delimiter);
        if (i == npos)
            return {str, {}};
        else
            return {str.substr(0, i), str.substr(i + delimiter.size())};
    }

    std::string quote(const std::string& str) {
        std::string result = "\"";
        size_t pos = 0;
        while (pos < str.size()) {
            auto start = pos;
            auto c = decode_char(str, pos);
            switch (c) {
                case 0:      result += "\\0"; break;
                case U'\t':  result += "\\t"; break;
                case U'\n':  result += "\\n"; break;
                case U'\f':  result += "\\f"; break;
                case U'\r':  result += "\\r"; break;
                case U'\"':  result += "\\\""; break;
                case U'\\':  result += "\\\\"; break;
                case not_unicode:
                    for (auto p = start; p < pos; ++p) {
                        result += "\\x";
                        Detail::append_hex(char(c), result);
                    }
                    break;
                default:
                    if (c <= 31 || c == 127) {
                        result += "\\x";
                        Detail::append_hex(char(c), result);
                    } else {
                        result.append(str, start, pos - start);
                    }
                    break;
            }
        }
        result += '\"';
        return result;
    }

    std::string repeat(const std::string& str, size_t n) {
        if (n == 0)
            return {};
        std::string result(str.size() * n, '\0');
        std::memcpy(result.data(), str.data(), str.size());
        for (size_t len = str.size(); len < result.size(); len *= 2)
            std::memcpy(result.data() + len, result.data(), std::min(len, result.size() - len));
        return result;
    }

    std::string replace(const std::string& str, const std::string& target, const std::string& replacement) {
        if (target.empty())
            return str;
        std::string result;
        size_t i = 0, j = 0;
        while (i < str.size()) {
            j = str.find(target, i);
            if (j == npos) {
                result.append(str, i, npos);
                break;
            }
            result.append(str, i, j - i);
            result += replacement;
            i = j + target.size();
        }
        return result;
    }

    std::string remove(const std::string& str, const std::string& target) {
        if (target.empty())
            return str;
        std::string result;
        size_t i = 0, j = 0;
        while (i < str.size()) {
            j = str.find(target, i);
            if (j == npos) {
                result.append(str, i, npos);
                break;
            }
            result.append(str, i, j - i);
            i = j + target.size();
        }
        return result;
    }

    std::vector<std::string> split(const std::string& str, const std::string& chars) {
        std::vector<std::string> vec;
        size_t i = 0, j = 0;
        while (j < str.size()) {
            i = str.find_first_not_of(chars, j);
            if (i == npos)
                break;
            j = str.find_first_of(chars, i);
            vec.push_back(str.substr(i, j - i));
        }
        return vec;
    }

    std::vector<std::string> split_at(const std::string& str, const std::string& delimiter) {
        if (str.empty())
            return {};
        if (delimiter.empty())
            return {str};
        std::vector<std::string> vec;
        size_t i = 0, j = 0;
        for (;;) {
            j = str.find(delimiter, i);
            vec.push_back(str.substr(i, j - i));
            if (j == npos)
                break;
            i = j + delimiter.size();
        }
        return vec;
    }

    std::vector<std::string> split_lines(const std::string& str) {
        auto lines = split_at(str, "\n");
        if (! str.empty() && str.back() == '\n')
            lines.pop_back();
        for (auto& line: lines)
            if (! line.empty() && line.back() == '\r')
                line.pop_back();
        return lines;
    }

    std::string trim(const std::string& str, const std::string& chars) {
        size_t i = str.find_first_not_of(chars);
        if (i == npos)
            return {};
        size_t j = str.find_last_not_of(chars);
        return str.substr(i, j - i + 1);
    }

    std::string trim_left(const std::string& str, const std::string& chars) {
        size_t i = str.find_first_not_of(chars);
        if (i == npos)
            return {};
        else
            return str.substr(i);
    }

    std::string trim_right(const std::string& str, const std::string& chars) {
        size_t i = str.find_last_not_of(chars);
        if (i == npos)
            return {};
        else
            return str.substr(0, i + 1);
    }

    std::string unqualify(const std::string& str, const std::string& delimiters) {
        size_t i = str.find_last_of(delimiters);
        if (i == npos)
            return str;
        else
            return str.substr(i + 1);
    }

    std::string unwrap_lines(const std::string& str) {
        std::string result;
        bool was_empty = true;
        for (auto line: split_lines(str)) {
            if (line.empty()) {
                result.append(size_t(! was_empty) + 1, '\n');
            } else if (was_empty) {
                result += trim_right(line);
            } else {
                result += ' ';
                result += trim(line);
            }
            was_empty = line.empty();
        }
        if (! was_empty)
            result += '\n';
        return result;
    }

    std::string wrap_lines(const std::string& str, size_t width, size_t margin, bool checked) {

        if (checked && margin != npos && margin >= width)
            throw std::length_error("Wrap margin is too big for width");

        static const auto is_empty = [] (const std::string& s) { return s.empty(); };

        auto lines = split_lines(str);

        for (auto& line: lines)
            line = trim_right(line);

        auto line_i = lines.begin();
        size_t prefix_size = 0;
        std::string prefix, result;

        if (margin != npos) {
            prefix_size = margin;
            if (checked && margin >= width)
                throw std::length_error("Margin of paragraph is too big for wrap width");
            prefix.assign(prefix_size, ' ');
        }

        while (line_i != lines.end()) {

            auto line_j = std::find_if_not(line_i, lines.end(), is_empty);
            result.append(line_j - line_i, '\n');

            if (line_j == lines.end())
                break;

            line_i = std::find_if(line_j, lines.end(), is_empty);
            size_t current_width = 0;

            if (margin == npos) {
                prefix = line_j->substr(0, line_j->find_first_not_of("\t "));
                prefix = replace(prefix, "\t", "    ");
                prefix_size = utf_width(prefix);
            }

            for (; line_j != line_i; ++line_j) {

                auto words = split(*line_j);

                for (auto& word: words) {

                    auto word_size = utf_width(word);

                    if (current_width > 0 && current_width + word_size + 1 > width) {
                        result += '\n';
                        current_width = 0;
                    }

                    if (current_width == 0) {
                        result += prefix + word;
                        current_width += prefix_size + word_size;
                        if (checked && current_width > width)
                            throw std::length_error("Words will not fit in wrap width");
                    } else {
                        result += ' ';
                        result += word;
                        current_width += word_size + 1;
                    }

                }

            }

            if (current_width > 0)
                result += '\n';

        }

        return result;

    }

    std::string indent_lines(const std::string& str, size_t spaces) {
        std::string result;
        for (auto& line: split_lines(str)) {
            if (! line.empty()) {
                result.append(spaces, ' ');
                result += line;
            }
            result += '\n';
        }
        return result;
    }

    namespace Detail {

        std::string roman_helper(uint64_t n, bool lcase) {
            static constexpr std::pair<uint64_t, const char*> table[] = {
                { 900, "CM" }, { 500, "D" }, { 400, "CD" }, { 100, "C" },
                { 90, "XC" }, { 50, "L" }, { 40, "XL" }, { 10, "X" },
                { 9, "IX" }, { 5, "V" }, { 4, "IV" }, { 1, "I" },
            };
            std::string rom(size_t(n / 1000), 'M');
            n %= 1000;
            for (auto [div,str]: table) {
                for (auto q = n / div; q > 0; --q)
                    rom += str;
                n %= div;
            }
            if (lcase)
                rom = ascii_lowercase(rom);
            return rom;
        }

    }

    // String parsing functions

    bool to_boolean(const std::string& str) {
        if (str.empty())
            return false;
        auto lcstr = ascii_lowercase(str.substr(0, 6));
        if (lcstr == "1" || lcstr == "t" || lcstr == "y" || lcstr == "true" || lcstr == "yes" || lcstr == "on")
            return true;
        else if (lcstr == "0" || lcstr == "f" || lcstr == "n" || lcstr == "false" || lcstr == "no" || lcstr == "off")
            return false;
        else
            throw std::invalid_argument("Invalid boolean value: " + quote(str));
    }

    // Type functions

    namespace Detail {

        std::string type_name_helper(const std::type_info& info) {
            std::string name = info.name();
            #ifdef __GNUC__
                auto mangled = name;
                std::shared_ptr<char> demangled;
                int status = 0;
                for (;;) {
                    if (mangled.empty())
                        break;
                    demangled.reset(abi::__cxa_demangle(mangled.data(), nullptr, nullptr, &status), std::free);
                    if (status == -1)
                        throw std::bad_alloc();
                    if (status == 0 && demangled)
                        return demangled.get();
                    if (mangled[0] != '_')
                        break;
                    mangled.erase(0, 1);
                }
            #endif
            return name;
        }

    }

    // String literals

    namespace Literals {

        std::string operator""_doc(const char* ptr, size_t len) {
            std::string str(ptr, len);
            auto lines = split_at(str, "\n");
            if (lines.empty())
                return {};
            std::string prefix = lines.back();
            for (auto& line: lines)
                line = trim_right(line);
            while (! lines.empty() && lines[0].empty())
                lines.erase(lines.begin());
            while (! lines.empty() && lines.back().empty())
                lines.pop_back();
            if (lines.empty())
                return {};
            for (auto& line: lines)
                if (! line.empty())
                    prefix.resize(common_prefix_length(prefix, line));
            size_t cut = std::min(prefix.find_first_not_of("\t "), prefix.size());
            std::string doc;
            for (auto& line: lines) {
                if (! line.empty())
                    doc.append(line, cut, npos);
                doc += '\n';
            }
            return doc;
        }

        std::vector<std::string> operator""_qw(const char* ptr, size_t len) {
            return split(std::string(ptr, len));
        }

    }

}
