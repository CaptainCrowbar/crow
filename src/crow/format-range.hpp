#pragma once

#include "crow/format-spec.hpp"
#include "crow/types.hpp"
#include <string>
#include <string_view>

namespace Crow {

    // Range formatting

    template <typename Range>
    std::string format_range(const Range& r, const FormatSpec& spec = {}) {
        bool bracket = ! spec.option('N');
        std::string s;
        if (bracket)
            s += '[';
        for (auto& x: r)
            s += format_object(x, spec) + ',';
        if (s.size() > 1)
            s.pop_back();
        if (bracket)
            s += ']';
        return s;
    }

    template <typename Range>
    std::string format_map(const Range& r, const FormatSpec& spec1, const FormatSpec& spec2) {
        bool bracket = ! spec1.option('N');
        std::string s;
        if (bracket)
            s += '{';
        for (auto& [k,v]: r)
            s += format_object(k, spec1) + ':' + format_object(v, spec2) + ',';
        if (s.size() > 1)
            s.pop_back();
        if (bracket)
            s += '}';
        return s;
    }

    template <typename Range>
    std::string format_map(const Range& r, const FormatSpec& spec = {}) {
        return format_map(r, spec, spec);
    }

    // String formatting

    std::string format_string(const std::string& str, const FormatSpec& spec = {});
    std::string format_string(const std::string_view& str, const FormatSpec& spec = {});
    std::string format_string(const char* str, const FormatSpec& spec = {});
    std::string format_string(const std::u16string& str, const FormatSpec& spec = {});
    std::string format_string(const std::u16string_view& str, const FormatSpec& spec = {});
    std::string format_string(const char16_t* str, const FormatSpec& spec = {});
    std::string format_string(const std::u32string& str, const FormatSpec& spec = {});
    std::string format_string(const std::u32string_view& str, const FormatSpec& spec = {});
    std::string format_string(const char32_t* str, const FormatSpec& spec = {});
    std::string format_string(const std::wstring& str, const FormatSpec& spec = {});
    std::string format_string(const std::wstring_view& str, const FormatSpec& spec = {});
    std::string format_string(const wchar_t* str, const FormatSpec& spec = {});

}
