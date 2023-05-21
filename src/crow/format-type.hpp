#pragma once

#include "crow/format-floating.hpp"
#include "crow/format-integer.hpp"
#include "crow/format-range.hpp"
#include "crow/format-spec.hpp"
#include "crow/format-traits.hpp"
#include "crow/string.hpp"
#include "crow/time.hpp"
#include "crow/types.hpp"
#include "crow/unicode.hpp"
#include <chrono>
#include <compare>
#include <concepts>
#include <cstddef>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace Crow {

    std::string format_boolean(bool b, const FormatSpec& spec);
    std::string format_ordering(std::partial_ordering ord);
    std::string format_ordering(std::strong_ordering ord);
    std::string format_ordering(std::weak_ordering ord);
    std::string format_type_name(const std::string& name, const FormatSpec& spec);

    template <std::integral T>
    std::string format_number(T t, const FormatSpec& spec = {}) {
        return format_integer(t, spec);
    }

    template <std::floating_point T>
    std::string format_number(T t, const FormatSpec& spec = {}) {
        return format_floating_point(t, spec);
    }

    template <std::floating_point T>
    std::string format_number(std::complex<T> t, const FormatSpec& spec = {}) {
        return format_complex(t, spec);
    }

    template <typename T>
    std::string format_pointer(T* ptr, const FormatSpec& spec) {
        if (ptr != nullptr)
            return "<0x" + format_integer(uintptr_t(ptr), "x") + ">";
        else if (spec.option('Z'))
            return "--";
        else
            return "<null>";
    }

    template <typename T>
    std::string format_via_stream(const T& t) {
        std::ostringstream out;
        out << t;
        return out.str();
    }

    template <typename T>
    concept FixedFormatType =
        SameBasicType<T, std::nullptr_t>
        || SameBasicType<T, bool>
        || SameBasicType<T, char>
        || SameBasicType<T, char16_t>
        || SameBasicType<T, char32_t>
        || SameBasicType<T, wchar_t>
        || SameBasicType<T, std::chrono::system_clock::time_point>
        || std::convertible_to<T, const char*>
        || std::convertible_to<T, const char16_t*>
        || std::convertible_to<T, const char32_t*>
        || std::convertible_to<T, const wchar_t*>
        || std::convertible_to<T, std::string>
        || std::convertible_to<T, std::u16string>
        || std::convertible_to<T, std::u32string>
        || std::convertible_to<T, std::wstring>
        || std::convertible_to<T, std::string_view>
        || std::convertible_to<T, std::u16string_view>
        || std::convertible_to<T, std::u32string_view>
        || std::convertible_to<T, std::wstring_view>
        || std::constructible_from<std::string, std::decay_t<T>>
        || std::derived_from<std::decay_t<T>, std::exception>
        || std::derived_from<std::decay_t<T>, Formatted>
        || std::is_pointer_v<std::decay_t<T>>
        || ArithmeticType<std::decay_t<T>>
        || RangeType<std::decay_t<T>>
        || Detail::DurationType<std::decay_t<T>>
        || Detail::StrMethodType<std::decay_t<T>>
        || Detail::ExtendedStrMethodType<std::decay_t<T>>
        || Detail::AdlToStringType<std::decay_t<T>>
        || Detail::StdToStringType<std::decay_t<T>>
        || Detail::OutputOperatorType<std::decay_t<T>>
        || Detail::StdOrderingType<std::decay_t<T>>;

    template <typename T>
    concept VariableFormatType =
        SameBasicType<T, bool>
        || SameBasicType<T, char>
        || SameBasicType<T, char16_t>
        || SameBasicType<T, char32_t>
        || SameBasicType<T, wchar_t>
        || SameBasicType<T, std::chrono::system_clock::time_point>
        || std::convertible_to<T, const char*>
        || std::convertible_to<T, const char16_t*>
        || std::convertible_to<T, const char32_t*>
        || std::convertible_to<T, const wchar_t*>
        || std::convertible_to<T, std::string>
        || std::convertible_to<T, std::u16string>
        || std::convertible_to<T, std::u32string>
        || std::convertible_to<T, std::wstring>
        || std::convertible_to<T, std::string_view>
        || std::convertible_to<T, std::u16string_view>
        || std::convertible_to<T, std::u32string_view>
        || std::convertible_to<T, std::wstring_view>
        || std::derived_from<std::decay_t<T>, Formatted>
        || ArithmeticType<std::decay_t<T>>
        || RangeType<std::decay_t<T>>
        || Detail::DurationType<std::decay_t<T>>
        || Detail::ExtendedStrMethodType<std::decay_t<T>>
        || Detail::IsOptionalType<std::decay_t<T>>::value;

    template <typename T>
    struct FormatType {
        std::string operator()(const T& t, const FormatSpec& spec) const {
            using U = std::decay_t<T>;
            if (spec.mode() == 'T')
                return format_type_name(type_name<U>(), spec);
            if constexpr (std::same_as<U, std::nullptr_t>)
                return spec.option('Z') ? "--" : "<null>";
            else if constexpr (Detail::StdOrderingType<U>)
                return format_ordering(t);
            else if constexpr (Detail::IsOptionalType<std::decay_t<T>>::value)
                return t.has_value() ? FormatType<typename T::value_type>()(*t, spec) : spec.option('Z') ? "--" : "<null>";
            else if constexpr (std::same_as<U, bool>)
                return format_boolean(t, spec);
            else if constexpr (std::same_as<U, char>)
                return t == 0 && spec.option('Z') ? "--" : format_string(std::string{t}, spec);
            else if constexpr (std::same_as<U, char16_t>)
                return t == 0 && spec.option('Z') ? "--" : format_string(to_utf8(decode_string(std::u16string{t})), spec);
            else if constexpr (std::same_as<U, char32_t>)
                return t == 0 && spec.option('Z') ? "--" : format_string(to_utf8(std::u32string{t}), spec);
            else if constexpr (std::same_as<U, wchar_t>)
                return t == 0 && spec.option('Z') ? "--" : format_string(to_utf8(decode_string(std::wstring{t})), spec);
            else if constexpr (std::is_integral_v<U>)
                return format_integer(t, spec);
            else if constexpr (std::is_floating_point_v<U>)
                return format_floating_point(t, spec);
            else if constexpr (Detail::DurationType<U>)
                return format_time(t, spec);
            else if constexpr (std::same_as<U, std::chrono::system_clock::time_point>)
                return format_time_point(t, spec);
            else if constexpr (std::is_base_of_v<Formatted, U>)
                return t.str(spec);
            else if constexpr (Detail::ExtendedStrMethodType<U>)
                return t.str(spec);
            else if constexpr (Detail::StrMethodType<U>)
                return t.str();
            else if constexpr (Detail::AdlToStringType<U>)
                return to_string(t);
            else if constexpr (Detail::StdToStringType<U>)
                return std::to_string(t);
            else if constexpr (std::is_convertible_v<U, const char*>)
                return format_string(static_cast<const char*>(t), spec);
            else if constexpr (std::is_convertible_v<U, std::string>)
                return format_string(std::string(t), spec);
            else if constexpr (std::is_convertible_v<U, std::string_view>)
                return format_string(std::string_view(t), spec);
            else if constexpr (std::is_convertible_v<U, const char16_t*>)
                return format_string(static_cast<const char16_t*>(t), spec);
            else if constexpr (std::is_convertible_v<U, std::u16string>)
                return format_string(std::u16string(t), spec);
            else if constexpr (std::is_convertible_v<U, std::u16string_view>)
                return format_string(std::u16string_view(t), spec);
            else if constexpr (std::is_convertible_v<U, const char32_t*>)
                return format_string(static_cast<const char32_t*>(t), spec);
            else if constexpr (std::is_convertible_v<U, std::u32string>)
                return format_string(std::u32string(t), spec);
            else if constexpr (std::is_convertible_v<U, std::u32string_view>)
                return format_string(std::u32string_view(t), spec);
            else if constexpr (std::is_convertible_v<U, const wchar_t*>)
                return format_string(static_cast<const wchar_t*>(t), spec);
            else if constexpr (std::is_convertible_v<U, std::wstring>)
                return format_string(std::wstring(t), spec);
            else if constexpr (std::is_convertible_v<U, std::wstring_view>)
                return format_string(std::wstring_view(t), spec);
            else if constexpr (std::is_constructible_v<std::string, U>)
                return std::string(t);
            else if constexpr (std::is_base_of_v<std::exception, U>)
                return t.what();
            else if constexpr (MaplikeRangeType<U>)
                return format_map(t, spec);
            else if constexpr (RangeType<U>)
                return format_range(t, spec);
            else if constexpr (std::is_pointer_v<U>)
                return format_pointer(t, spec);
            else if constexpr (Detail::OutputOperatorType<U>)
                return format_via_stream(t);
            else
                return type_name<U>() + ":" + format_pointer(&t, spec);
        }
    };

    class FormatObject {
    public:
        FormatObject() = default;
        explicit FormatObject(const FormatSpec& spec): spec_(spec) {}
        template <typename T> std::string operator()(const T& t) const { return FormatType<T>()(t, spec_); }
        FormatSpec spec() const { return spec_; }
    private:
        FormatSpec spec_;
    };

    template <typename T>
    std::string format_object(const T& t, const FormatSpec& spec = {}) {
        return FormatType<T>()(t, spec);
    }

    class Formatter {
    public:
        Formatter() = default;
        explicit Formatter(const std::string& pattern);
        template <typename... Args> std::string operator()(const Args&... args) const;
    private:
        struct field_type {
            FormatSpec spec;
            std::string text;
            int index;
        };
        std::vector<field_type> fields_;
        size_t required_args_ = 0;
        static void handle_arg(std::string& /*result*/, const field_type& /*field*/, int /*index*/) {}
        template <typename T, typename... Args>
            static void handle_arg(std::string& result, const field_type& field, int index, const T& t, const Args&... args);
    };

        template <typename... Args>
        std::string Formatter::operator()(const Args&... args) const {
            if (sizeof...(args) < required_args_)
                throw std::out_of_range("Not enough format arguments: "+ std::to_string(sizeof...(args)) + " supplied, "
                    + std::to_string(required_args_) + " required");
            std::string result;
            for (auto& field: fields_) {
                if (field.index < 0)
                    result += field.text;
                else
                    handle_arg(result, field, 0, args...);
            }
            return result;
        }

        template <typename T, typename... Args>
        void Formatter::handle_arg(std::string& result, const field_type& field, int index, const T& t, const Args&... args) {
            if (index == field.index)
                result += format_object(t, field.spec);
            else
                handle_arg(result, field, index + 1, args...);
        }

    template <typename... Args>
    std::string fmt(const std::string& pattern, const Args&... args) {
        return Formatter(pattern)(args...);
    }

    namespace Literals {

        inline Formatter operator""_fmt(const char* ptr, size_t len) {
            return Formatter(std::string(ptr, len));
        }

    }

}
