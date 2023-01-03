// This header is not part of the public interface and should not be included directly
// #include "crow/string.hpp" to access these functions

#include "crow/types.hpp"
#include <algorithm>
#include <functional>
#include <string_view>

namespace Crow {

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

}
