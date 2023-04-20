#include "crow/format-floating.hpp"

namespace Crow {

    namespace Detail {

        std::string trim_exponent(const std::string& str, bool xsign) {
            size_t begin_cut = str.find_first_of("Ee");
            if (begin_cut == npos)
                return str;
            ++begin_cut;
            bool neg = str[begin_cut] == '-';
            if (neg)
                ++begin_cut;
            size_t end_cut = begin_cut;
            if (str[end_cut] == '+')
                ++end_cut;
            while (end_cut < str.size() && str[end_cut] == '0')
                ++end_cut;
            if (end_cut == str.size())
                --end_cut;
            auto result = str.substr(0, begin_cut);
            if (xsign && ! neg)
                result += '+';
            result += str.substr(end_cut);
            return result;
        }

        std::string trim_zeros(const std::string& str) {
            size_t dec_point = str.find('.');
            if (dec_point == npos)
                return str;
            size_t end_sig = str.find_first_not_of("0123456789", dec_point + 1);
            if (end_sig == npos)
                end_sig = str.size();
            size_t last_digit = end_sig - 1;
            while (str[last_digit] == '0')
                --last_digit;
            if (str[last_digit] != '.')
                ++last_digit;
            if (last_digit < end_sig)
                return str.substr(0, last_digit) + str.substr(end_sig);
            else
                return str;
        }

    }

}
