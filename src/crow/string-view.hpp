// This header is not part of the public interface and should not be included directly
// #include "crow/string.hpp" to access these functions

#include "crow/types.hpp"
#include <algorithm>
#include <functional>
#include <string_view>

namespace Crow {

    constexpr const char* ascii_whitespace = "\t\n\f\r ";

    constexpr const char* view_endptr(std::string_view view) noexcept {
        return view.data() + view.size();
    }

    constexpr bool view_is_null(std::string_view view) noexcept {
        return view.data() == nullptr;
    }

    constexpr bool view_is_substring(std::string_view view, std::string_view str) noexcept {
        using LTE = std::less_equal<const char*>;
        return LTE()(str.data(), view.data()) && LTE()(view_endptr(view), view_endptr(str));
    }

    constexpr size_t view_pos(std::string_view str, std::string_view view) noexcept {
        return size_t(view.data() - str.data());
    }

    constexpr size_t view_endpos(std::string_view str, std::string_view view) noexcept {
        return view_pos(str, view) + view.size();
    }

    constexpr std::string_view view_begin(std::string_view view) noexcept {
        if (view_is_null(view))
            return {};
        else
            return view.substr(0, 0);
    }

    constexpr std::string_view view_end(std::string_view view) noexcept {
        if (view_is_null(view))
            return {};
        else
            return view.substr(view.size(), 0);
    }

    constexpr std::string_view view_cat(std::string_view left, std::string_view right) noexcept {
        if (view_is_null(right))
            return left;
        else if (view_is_null(left))
            return right;
        else
            return std::string_view(left.data(), size_t(view_endptr(right) - left.data()));
    }

    constexpr std::string_view view_between(std::string_view left, std::string_view right) noexcept {
        auto p = view_endptr(left);
        auto q = right.data();
        return {p, std::max(p, q)};
    }

    constexpr std::string_view view_extend(std::string_view view, size_t add) noexcept {
        return std::string_view(view.data(), view.size() + add);
    }

    constexpr std::string_view view_extend(std::string_view str, std::string_view view, size_t add) noexcept {
        if (view_is_null(str))
            return {};
        size_t max_add = str.size() - view_endpos(str, view);
        return std::string_view(view.data(), view.size() + std::min(add, max_add));
    }

    constexpr std::string_view view_left_of(std::string_view str, std::string_view view) noexcept {
        if (view_is_null(str) && view_is_null(view))
            return {};
        else
            return str.substr(0, view_pos(str, view));
    }

    constexpr std::string_view view_right_of(std::string_view str, std::string_view view) noexcept {
        if (view_is_null(str) && view_is_null(view))
            return {};
        else
            return str.substr(view_endpos(str, view), npos);
    }

    constexpr std::string_view view_from_left(std::string_view str, std::string_view view) noexcept {
        if (view_is_null(str) && view_is_null(view))
            return {};
        else
            return str.substr(0, view_endpos(str, view));
    }

    constexpr std::string_view view_from_right(std::string_view str, std::string_view view) noexcept {
        if (view_is_null(str) && view_is_null(view))
            return {};
        else
            return str.substr(view_pos(str, view), npos);
    }

    constexpr bool view_trim_left(std::string_view& view, std::string_view chars = ascii_whitespace) noexcept {
        if (view.empty())
            return false;
        auto i = view.find_first_not_of(chars);
        if (i == 0)
            return false;
        else if (i == npos)
            view = view_end(view);
        else
            view = view.substr(i, npos);
        return true;
    }

    constexpr bool view_trim_right(std::string_view& view, std::string_view chars = ascii_whitespace) noexcept {
        if (view.empty())
            return false;
        auto i = view.find_last_not_of(chars) + 1;
        if (i == view.size())
            return false;
        else if (i == 0)
            view = view_begin(view);
        else
            view = view.substr(0, i);
        return true;
    }

    constexpr bool view_trim(std::string_view& view, std::string_view chars = ascii_whitespace) noexcept {
        bool l = view_trim_left(view, chars);
        bool r = view_trim_right(view, chars);
        return l || r;
    }

}
