#include "crow/string.hpp"
#include <cerrno>
#include <iterator>
#include <memory>
#include <new>

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

namespace Crow {

    namespace {

        size_t get_columns() noexcept {
            static constexpr size_t default_columns = 80;
            auto columns_env = std::getenv("COLUMNS");
            if (columns_env == nullptr || *columns_env == 0)
                return default_columns;
            errno = 0;
            size_t columns = std::strtoul(columns_env, nullptr, 10);
            if (errno == 0 && columns != 0)
                return columns;
            else
                return default_columns;
        }

    }

    // String formatting functions

    namespace Detail {

        std::string escape_helper(std::string_view str, int quote_char) {

            bool quoted = quote_char >= 0;
            auto c_quote = quoted ? char(quote_char) : '\0';
            auto u_quote = quoted ? char32_t(quote_char) : char32_t(0);
            std::string result;

            if (quoted)
                result += c_quote;

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
                    case U'\\':  result += "\\\\"; break;

                    case not_unicode:
                        for (auto p = start; p < pos; ++p) {
                            result += "\\x";
                            Detail::append_hex(char(c), result);
                        }
                        break;

                    default:
                        if (quoted && c == u_quote) {
                            result += '\\';
                            result += c_quote;
                        } else if (c <= 31 || c == 127) {
                            result += "\\x";
                            Detail::append_hex(char(c), result);
                        } else {
                            result.append(str, start, pos - start);
                        }
                        break;

                }

            }

            if (quoted)
                result += c_quote;

            return result;

        }

        std::string unescape_helper(std::string_view str, int quote_char) {

            if (str.empty())
                return {};

            bool quoted = quote_char >= 0;
            auto c_quote = quoted ? char(quote_char) : '\0';
            std::string_view content;

            if (quoted) {
                if (str.size() < 2 || str[0] != c_quote || str.back() != c_quote)
                    throw std::invalid_argument("Invalid quoted string: " + quote(str));
                content = str.substr(1, str.size() - 2);
            } else {
                content = str;
            }

            std::string result;
            std::size_t i = 0;

            while (i < content.size()) {

                auto j = content.find('\\', i);

                if (j >= content.size() - 1) {
                    result.append(content, i, npos);
                    break;
                }

                result.append(content, i, j - i);
                std::size_t skip = 2;

                switch (content[j + 1]) {

                    case 'x':
                        if (content.size() - j == 2 || ! ascii_isxdigit(content[j + 2]) || ! ascii_isxdigit(content[j + 3]))
                            throw std::invalid_argument("Invalid escape code: " + quote(str));
                        result += char(to_uint8(content.substr(j + 2, 2), 16));
                        skip = 4;
                        break;

                    case '0':  result += '\0'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    default:   result += content[j + 1]; break;

                }

                i = j + skip;

            }

            return result;

        }

        std::string roman_helper(uint32_t n, bool lcase) {
            static constexpr std::pair<uint32_t, const char*> table[] = {
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

    std::string ascii_uppercase(std::string_view str) {
        std::string result;
        result.reserve(str.size());
        std::transform(str.begin(), str.end(), std::back_inserter(result), ascii_toupper);
        return result;
    }

    std::string ascii_lowercase(std::string_view str) {
        std::string result;
        result.reserve(str.size());
        std::transform(str.begin(), str.end(), std::back_inserter(result), ascii_tolower);
        return result;
    }

    std::string ascii_titlecase(std::string_view str) {
        bool is_alpha = false;
        bool was_alpha = false;
        std::string result;
        for (char c: str) {
            if (c == '\'')
                is_alpha = was_alpha;
            else
                is_alpha = ascii_isalpha(c);
            if (! is_alpha)
                result += c;
            else if (was_alpha)
                result += ascii_tolower(c);
            else
                result += ascii_toupper(c);
            was_alpha = is_alpha;
        }
        return result;
    }

    size_t common_prefix_length(std::string_view str1, std::string_view str2) noexcept {
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

    std::pair<std::string_view, std::string_view> partition(std::string_view str, std::string_view chars) {
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

    std::pair<std::string_view, std::string_view> partition_at(std::string_view str, std::string_view delimiter) {
        if (delimiter.empty())
            return {str, {}};
        size_t i = str.find(delimiter);
        if (i == npos)
            return {str, {}};
        else
            return {str.substr(0, i), str.substr(i + delimiter.size())};
    }

    std::string repeat(std::string_view str, size_t n) {
        if (n == 0)
            return {};
        std::string result(str.size() * n, '\0');
        std::memcpy(result.data(), str.data(), str.size());
        for (size_t len = str.size(); len < result.size(); len *= 2)
            std::memcpy(result.data() + len, result.data(), std::min(len, result.size() - len));
        return result;
    }

    std::string replace(std::string_view str, std::string_view target, std::string_view replacement) {
        if (target.empty())
            return std::string(str);
        std::string result;
        size_t i = 0;
        while (i < str.size()) {
            size_t j = str.find(target, i);
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

    std::string remove(std::string_view str, std::string_view target) {
        if (target.empty())
            return std::string(str);
        std::string result;
        size_t i = 0;
        while (i < str.size()) {
            size_t j = str.find(target, i);
            if (j == npos) {
                result.append(str, i, npos);
                break;
            }
            result.append(str, i, j - i);
            i = j + target.size();
        }
        return result;
    }

    namespace {

        template <typename T>
        std::vector<T> split_impl(std::string_view str, std::string_view chars) {
            std::vector<T> vec;
            size_t i = 0;
            while (i < str.size()) {
                size_t j = str.find_first_not_of(chars, i);
                if (j == npos)
                    break;
                i = str.find_first_of(chars, j);
                vec.emplace_back(str.substr(j, i - j));
            }
            return vec;
        }

        template <typename T>
        std::vector<T> split_at_impl(std::string_view str, std::string_view delimiter) {
            if (str.empty())
                return {};
            std::vector<T> vec;
            if (delimiter.empty()) {
                vec.emplace_back(str);
                return vec;
            }
            size_t i = 0;
            for (;;) {
                size_t j = str.find(delimiter, i);
                vec.emplace_back(str.substr(i, j - i));
                if (j == npos)
                    break;
                i = j + delimiter.size();
            }
            return vec;
        }

        template <typename T>
        std::vector<T> split_lines_impl(std::string_view str, bool keep) {
            std::vector<T> vec;
            size_t i = 0;
            while (i < str.size()) {
                size_t j = str.find('\n', i);
                if (j == npos) {
                    vec.emplace_back(str.substr(i, npos));
                    break;
                }
                size_t n = keep ? 0 : j > 0 && str[j - 1] == '\r' ? 2 : 1;
                vec.emplace_back(str.substr(i, j + 1 - i - n));
                i = j + 1;
            }
            return vec;
        }

    }

    std::vector<std::string> split(std::string_view str, std::string_view chars) {
        return split_impl<std::string>(str, chars);
    }

    std::vector<std::string_view> split_v(std::string_view str, std::string_view chars) {
        return split_impl<std::string_view>(str, chars);
    }

    std::vector<std::string> split_at(std::string_view str, std::string_view delimiter) {
        return split_at_impl<std::string>(str, delimiter);
    }

    std::vector<std::string_view> split_at_v(std::string_view str, std::string_view delimiter) {
        return split_at_impl<std::string_view>(str, delimiter);
    }

    std::vector<std::string> split_lines(std::string_view str, bool keep) {
        return split_lines_impl<std::string>(str, keep);
    }

    std::vector<std::string_view> split_lines_v(std::string_view str, bool keep) {
        return split_lines_impl<std::string_view>(str, keep);
    }

    std::string trim(std::string_view str, std::string_view chars) {
        view_trim(str, chars);
        return std::string(str);
    }

    std::string_view trim_v(std::string_view str, std::string_view chars) {
        view_trim(str, chars);
        return str;
    }

    std::string trim_left(std::string_view str, std::string_view chars) {
        view_trim_left(str, chars);
        return std::string(str);
    }

    std::string_view trim_left_v(std::string_view str, std::string_view chars) {
        view_trim_left(str, chars);
        return str;
    }

    std::string trim_right(std::string_view str, std::string_view chars) {
        view_trim_right(str, chars);
        return std::string(str);
    }

    std::string_view trim_right_v(std::string_view str, std::string_view chars) {
        view_trim_right(str, chars);
        return str;
    }

    std::string unqualify(std::string_view str, std::string_view delimiters) {
        return std::string(unqualify_v(str, delimiters));
    }

    std::string_view unqualify_v(std::string_view str, std::string_view delimiters) {
        if (delimiters.empty())
            return str;
        size_t i = str.find_last_of(delimiters);
        if (i == npos)
            return str;
        else
            return str.substr(i + 1, npos);
    }

    std::string unwrap_lines(std::string_view str) {
        std::string result;
        bool was_empty = true;
        for (auto line: split_lines_v(str)) {
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

    std::string wrap_lines(std::string_view str, Usize mode, size_t width, size_t margin, bool checked) {

        if (width == npos)
            width = get_columns() - 1;

        if (checked && margin != npos && margin >= width)
            throw std::length_error("Wrap margin is too big for width");

        static const auto is_empty = [] (std::string_view s) { return s.empty(); };

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
                prefix_size = utf_size(prefix, mode);
            }

            for (; line_j != line_i; ++line_j) {

                auto words = split_v(*line_j);

                for (auto& word: words) {

                    auto word_size = utf_size(word, mode);

                    if (current_width > 0 && current_width + word_size + 1 > width) {
                        result += '\n';
                        current_width = 0;
                    }

                    if (current_width == 0) {
                        result += prefix;
                        result += word;
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

    std::string indent_lines(std::string_view str, size_t spaces) {
        std::string result;
        for (auto& line: split_lines_v(str)) {
            if (! line.empty()) {
                result.append(spaces, ' ');
                result += line;
            }
            result += '\n';
        }
        return result;
    }

    // String parsing functions

    bool to_boolean(std::string_view str) {
        auto n = str.size();
        if (n == 0)
            return false;
        switch (str[0]) {
            case '0':  if (n == 1) return false; break;
            case '1':  if (n == 1) return true; break;
            case 'F':  if (n == 1 || str == "FALSE" || str == "False") return false; break;
            case 'N':  if (n == 1 || str == "NO" || str == "No") return false; break;
            case 'O':  if (str == "OFF" || str == "Off") return false; if (str == "ON" || str == "On") return true; break;
            case 'T':  if (n == 1 || str == "TRUE" || str == "True") return true; break;
            case 'Y':  if (n == 1 || str == "YES" || str == "Yes") return true; break;
            case 'f':  if (n == 1 || str == "false") return false; break;
            case 'n':  if (n == 1 || str == "no") return false; break;
            case 'o':  if (str == "off") return false; if (str == "on") return true; break;
            case 't':  if (n == 1 || str == "true") return true; break;
            case 'y':  if (n == 1 || str == "yes") return true; break;
            default:   break;
        }
        throw std::invalid_argument("Invalid boolean value: " + quote(str));
    }

    // String query functions

    std::pair<size_t, size_t> line_and_column(std::string_view str, size_t pos, Usize mode, size_t tab_size) {

        auto text = str.substr(0, pos);
        size_t line = std::count(text.begin(), text.end(), '\n');
        size_t start = text.find_last_of('\n');

        if (start == npos)
            start = 0;
        else
            ++start;

        size_t col = 0;
        size_t i = start;
        size_t len = text.size();

        while (i < len) {
            size_t j = text.find('\t', i);
            if (j == npos)
                j = len;
            col += utf_size(text.substr(i, j - i), mode);
            if (j == len)
                break;
            if (tab_size > 0) {
                col += tab_size - col % tab_size;
                ++j;
            }
            i = text.find_first_not_of('\t', j);
            if (i == npos)
                i = len;
            col += tab_size * (i - j);
        }

        return {line, col};

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
            auto prefix = lines.back();
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

    }

}
