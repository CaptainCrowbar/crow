#include "crow/format-numeric.hpp"
#include <algorithm>

namespace Crow::Detail {

    void expand_formatted_number(std::string& numstr, const FormatSpec& spec) {

        // Implements the formatting options common to all arithmetic types

        if (spec.empty())
            return;
        if (spec.option('s') && numstr[0] != '-')
            numstr.insert(0, 1, '+');

        bool comma_point = spec.option('e');
        char group_code = spec.find_option("ACUWacuw");

        if (! comma_point && group_code == 0)
            return;

        auto is_digit = spec.lcmode() == 'x' ? ascii_isxdigit : ascii_isdigit;
        size_t prefix_starts = size_t(! ascii_isdigit(numstr[0]));
        size_t prefix_ends = size_t(std::find_if_not(numstr.begin() + prefix_starts, numstr.end(), is_digit) - numstr.begin());
        bool has_point = numstr[prefix_ends] == '.';
        int prefix_digits = int(prefix_ends - prefix_starts);
        size_t suffix_ends = npos;
        int suffix_digits = 0;
        if (has_point) {
            suffix_ends = size_t(std::find_if_not(numstr.begin() + prefix_ends + 1, numstr.end(), is_digit) - numstr.begin());
            suffix_digits = int(suffix_ends - prefix_ends - 1);
        }

        if (has_point && comma_point)
            numstr[prefix_ends] = ',';

        char delimiter = 0;

        switch (group_code) {
            case 'A': case 'a':  delimiter = '\''; break;
            case 'C': case 'c':  delimiter = ','; break;
            case 'U': case 'u':  delimiter = '_'; break;
            case 'W': case 'w':  delimiter = ' '; break;
            default:             break;
        }

        if (delimiter != 0) {

            int group_size = 0;
            if (spec.find_mode("BXbx") != 0)
                group_size = 4;
            else if (spec.find_option("ACUW"))
                group_size = 5;
            else
                group_size = 3;

            int min_grouped = group_size + int(spec.option('E')) + 1;

            if (has_point && suffix_digits >= min_grouped) {
                int num_delimiters = (suffix_digits - 1) / group_size;
                for (int i = 0, pos = int(prefix_ends) + group_size + 1; i < num_delimiters; ++i, pos += group_size + 1)
                    numstr.insert(pos, 1, delimiter);
            }

            if (prefix_digits >= min_grouped)
                for (int pos = int(prefix_ends) - group_size; pos > int(prefix_starts); pos -= group_size)
                    numstr.insert(pos, 1, delimiter);

        }

    }

}
