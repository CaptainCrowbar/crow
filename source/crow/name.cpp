#include "crow/name.hpp"
#include "crow/english.hpp"
#include "crow/format.hpp"
#include "crow/regex.hpp"
#include "crow/string.hpp"
#include "crow/unicode.hpp"
#include <stdexcept>

using namespace Crow::Literals;

namespace Crow {

    namespace {

        std::string to_case(std::string_view str, char casing, bool use_stopwords) {
            if (casing == 'l')
                return ascii_lowercase(str);
            if (casing == 'u')
                return ascii_uppercase(str);
            if (use_stopwords) {
                auto lc = ascii_lowercase(str);
                if (stopwords().contains(lc))
                    return lc;
            }
            return ascii_titlecase(str);
        }

    }

    Name::Name(std::string_view s, parse_flags pf) {

        static const auto word_template = R"(
            \\d+
            | \\p\{L&\} ({0} \\p\{Ll\})+
            | \\p\{Lu\} ({0} \\p\{Lu\})+ (?!\\p\{Ll\})
            )"_doc;

        static const auto pattern_flags = Regex::extended | Regex::no_capture;

        static const Regex word_pattern(fmt(word_template, ""), pattern_flags);
        static const Regex apos_pattern(fmt(word_template, "'?"), pattern_flags);

        auto& pattern = !! (pf & apos) ? apos_pattern : word_pattern;

        for (auto& match: pattern.grep(s))
            words_.push_back(std::string(match));

    }

    std::string Name::str(style s, format_flags ff) const {
        std::string_view format;
        switch (s) {
            case lower:        format = "l "; break;
            case kebab:        format = "l-"; break;
            case snake:        format = "l_"; break;
            case sentence:     format = "tl "; break;
            case stroustrup:   format = "tl_"; break;
            case camel:        format = "lt"; break;
            case pascal:       format = "t"; break;
            case title:        format = "t "; break;
            case title_kebab:  format = "t-"; break;
            case title_snake:  format = "t_"; break;
            case upper:        format = "u "; break;
            case upper_kebab:  format = "u-"; break;
            case upper_snake:  format = "u_"; break;
        }
        return get_str(format, ff);
    }

    std::string Name::str(std::string_view format, format_flags ff) const {
        static const Regex pattern("[ltu]{1,2}([[:punct:]]|[[:space:]])?",
            Regex::ascii | Regex::full | Regex::no_capture);
        if (! pattern(format))
            throw std::invalid_argument(fmt("Invalid name format: {0:q}", format));
        return get_str(format, ff);
    }

    std::string Name::get_str(std::string_view format, format_flags ff) const {

        char head_case = format[0];
        char tail_case = format[1];
        if (! ascii_isalpha(tail_case))
            tail_case = head_case;
        char delimiter = format.back();
        if (! (ascii_ispunct(delimiter) || ascii_isspace(delimiter)))
            delimiter = '\0';

        std::string result;
        char casing = head_case;
        bool use_stopwords = !! (ff & stop);

        for (auto& w: words_) {
            result += to_case(w, casing, use_stopwords && ! result.empty());
            if (delimiter != 0)
                result += delimiter;
            casing = tail_case;
        }

        if (delimiter != 0)
            result.pop_back();

        return result;

    }

}
