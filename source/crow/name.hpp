#pragma once

#include "crow/enum.hpp"
#include "crow/types.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace Crow {

    class Name {

    public:

        enum class style {
            lower,        // "l "   = lower case with spaces
            kebab,        // "l-"   = lower-case-with-hyphens
            snake,        // "l_"   = lower_case_with_underscores
            sentence,     // "tl "  = Sentence case with spaces
            stroustrup,   // "tl_"  = Sentence_case_with_underscores
            camel,        // "lt"   = camelCaseWithNoDelimiters
            pascal,       // "lt"   = TitleCaseWithNoDelimiters
            title,        // "t "   = Title Case With Spaces
            title_kebab,  // "t-"   = Title-Case-With-Hyphens
            title_snake,  // "t_"   = Title_Case_With_Underscores
            upper,        // "u "   = UPPER CASE WITH SPACES
            upper_kebab,  // "u-"   = UPPER-CASE-WITH-HYPHENS
            upper_snake,  // "u_"   = UPPER_CASE_WITH_UNDERSCORES
        };

        enum class parse_flags {
            no_pf  = 0,
            apos   = 1,
        };

        enum class format_flags {
            no_ff  = 0,
            stop   = 2,
        };

        using enum style;
        using enum parse_flags;
        using enum format_flags;

        using iterator = std::vector<std::string>::const_iterator;

        Name() = default;
        Name(std::string_view s, parse_flags pf = no_pf);
        Name(const std::string& s, parse_flags pf = no_pf): Name(std::string_view(s), pf) {}
        Name(const char* s, parse_flags pf = no_pf): Name(std::string_view(s), pf) {}

        iterator begin() const noexcept { return words_.begin(); }
        iterator end() const noexcept { return words_.end(); }
        bool empty() const noexcept { return words_.empty(); }
        size_t size() const noexcept { return words_.size(); }

        std::string initials() const;
        std::string str(style s, format_flags ff = no_ff) const;
        std::string str(std::string_view format, format_flags ff = no_ff) const;

    private:

        std::vector<std::string> words_;

        std::string get_str(std::string_view format, format_flags ff) const;

    };

    CROW_BITMASK_OPERATORS(Name::parse_flags)
    CROW_BITMASK_OPERATORS(Name::format_flags)

}
