#include "crow/format-range.hpp"
#include "crow/string.hpp"

namespace Crow {

    // String formatting

    std::string format_string(const std::string& str, const FormatSpec& spec) {

        if (str.empty() && spec.option('Z'))
            return "--";

        std::string short_str;
        auto sptr = &str;
        bool truncate = spec.find_mode("QqsXx") != 0 && spec.has_prec() && size_t(spec.prec()) < str.size();

        if (spec.lcmode() == 'x') {

            if (truncate) {
                short_str = str.substr(0, spec.prec());
                sptr = &short_str;
            }

            bool space = ! spec.option('z');
            std::string result = Detail::hex_data(sptr->data(), sptr->size(), space);

            if (spec.mode() == 'X')
                result = ascii_uppercase(result);
            if (truncate)
                result += "...";

            return result;

        } else {

            if (truncate) {
                size_t cut = 0;
                for (int i = 0; i < spec.prec() && cut < str.size(); ++i)
                    decode_char(str, cut);
                if (cut < str.size()) {
                    short_str.assign(str, 0, cut);
                    short_str += "...";
                    sptr = &short_str;
                }
            }

            if (spec.mode() == 'q')
                return quote(*sptr);
            else
                return *sptr;

        }

    }

    std::string format_string(const std::string_view& str, const FormatSpec& spec) {
        return format_string(std::string(str), spec);
    }

    std::string format_string(const char* str, const FormatSpec& spec) {
        if (str != nullptr)
            return format_string(std::string(str), spec);
        else if (spec.option('Z'))
            return "--";
        else
            return "<null>";
    }

    std::string format_string(const std::u16string& str, const FormatSpec& spec) {
        return format_string(to_utf8(decode_string(str)), spec);
    }

    std::string format_string(const std::u16string_view& str, const FormatSpec& spec) {
        return format_string(std::u16string(str), spec);
    }

    std::string format_string(const char16_t* str, const FormatSpec& spec) {
        if (str != nullptr)
            return format_string(std::u16string(str), spec);
        else if (spec.option('Z'))
            return "--";
        else
            return "<null>";
    }

    std::string format_string(const std::u32string& str, const FormatSpec& spec) {
        return format_string(to_utf8(str), spec);
    }

    std::string format_string(const std::u32string_view& str, const FormatSpec& spec) {
        return format_string(std::u32string(str), spec);
    }

    std::string format_string(const char32_t* str, const FormatSpec& spec) {
        if (str != nullptr)
            return format_string(std::u32string(str), spec);
        else if (spec.option('Z'))
            return "--";
        else
            return "<null>";
    }

    std::string format_string(const std::wstring& str, const FormatSpec& spec) {
        return format_string(to_utf8(decode_string(str)), spec);
    }

    std::string format_string(const std::wstring_view& str, const FormatSpec& spec) {
        return format_string(std::wstring(str), spec);
    }

    std::string format_string(const wchar_t* str, const FormatSpec& spec) {
        if (str != nullptr)
            return format_string(std::wstring(str), spec);
        else if (spec.option('Z'))
            return "--";
        else
            return "<null>";
    }

}
