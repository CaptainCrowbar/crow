#include "crow/format-type.hpp"
#include <algorithm>

namespace Crow {

    // Functions

    std::string format_boolean(bool b, const FormatSpec& spec) {
        switch (spec.mode()) {
            case 'b': case 0:  return b ? "true" : "false";
            case 'B':          return b ? "True" : "False";
            case 'y':          return b ? "yes" : "no";
            case 'Y':          return b ? "Yes" : "No";
            default:           return format_integer(int(b), spec);
        }
    }

    // Class Formatter

    Formatter::Formatter(const std::string& pattern) {

        is_valid_utf(pattern, true);

        for (size_t current_pos = 0; current_pos < pattern.size();) {

            auto field_pos = pattern.find_first_of("{\\", current_pos);

            if (field_pos > current_pos)
                fields_.push_back({{}, pattern.substr(current_pos, field_pos - current_pos), -1});
            if (field_pos == npos)
                break;

            if (pattern[field_pos] == '{') {

                size_t end_field = pattern.find('}', field_pos + 1);
                if (end_field == npos)
                    throw std::invalid_argument("Invalid format pattern: " + quote(pattern));

                size_t end_index = std::find_if_not(pattern.begin() + field_pos + 1, pattern.begin() + end_field, ascii_isdigit)
                    - pattern.begin();
                if (end_index == field_pos + 1 || ! (end_index == end_field || pattern[end_index] == ':'))
                    throw std::invalid_argument("Invalid format pattern: " + quote(pattern));

                int index = to_int(pattern.substr(field_pos + 1, end_index - field_pos - 1));
                fields_.push_back({{}, {}, index});
                required_args_ = std::max(required_args_, size_t(index + 1));

                if (pattern[end_index] == ':')
                    fields_.back().spec = pattern.substr(end_index + 1, end_field - end_index - 1);

                current_pos = end_field + 1;

            } else {

                if (pattern.size() - field_pos == 1)
                    throw std::invalid_argument("Invalid format pattern: " + quote(pattern));

                size_t end_escape = field_pos + 1;
                decode_char(pattern, end_escape);
                fields_.push_back({{}, pattern.substr(field_pos + 1, end_escape - field_pos - 1), -1});
                current_pos = end_escape;

            }

        }

    }

}
