#include "crow/log-scale.hpp"
#include "crow/string.hpp"
#include <cstdio>
#include <stdexcept>

namespace Crow {

    namespace Detail {

        std::string LogScaleBase::do_format(long double sig, long double ex, int8_t sign, FormatSpec spec) {

            if (spec.empty())
                spec = "gz6";

            bool upcase = ascii_isupper(spec.mode());

            if (spec.find_mode("DdEe") == 0) {
                char use_mode;
                if (ex >= -3 && ex < 6)
                    use_mode = upcase ? 'D' : 'd';
                else
                    use_mode = upcase ? 'E' : 'e';
                FormatSpec use_spec(use_mode, spec.options(), spec.prec());
                return do_format(sig, ex, sign, use_spec);
            }

            std::string result;
            spec.default_prec(6);
            auto lcmode = spec.lcmode();
            bool ztrim = spec.option('z');
            int prec = std::max(spec.prec(), 1);

            if (sign == 0) {
                result += '0';
                if (! ztrim) {
                    if (prec > 1)
                        result += '.';
                    result.append(prec - 1, '0');
                }
                if (lcmode == 'e')
                    result += upcase ? "E0" : "e0";
                return result;
            }

            int rc = std::snprintf(nullptr, 0, upcase ? "%.*LG" : "%.*Lg", prec, sig);
            std::string sig_str(size_t(rc + 1), '\0');
            std::snprintf(sig_str.data(), sig_str.size(), upcase ? "%.*LG" : "%.*Lg", prec, sig);
            sig_str.pop_back();

            if (sig_str.substr(0, 2) == "10") {
                sig_str = "1";
                ++ex;
            } else {
                sig_str.erase(1, 1);
            }

            if (! ztrim)
                sig_str.resize(size_t(prec), '0');

            if (lcmode == 'd') {

                auto len = ptrdiff_t(sig_str.size());
                auto dp_pos = ptrdiff_t(ex) + 1;

                if (dp_pos <= 0) {
                    result += "0.";
                    result.append(- dp_pos, '0');
                    result += sig_str;
                } else if (dp_pos < len) {
                    result.append(sig_str, 0, dp_pos);
                    result += '.';
                    result.append(sig_str, dp_pos, npos);
                } else {
                    result += sig_str;
                    result.append(dp_pos - len, '0');
                }

            } else {

                if (sig_str.size() > 1)
                    sig_str.insert(1, 1, '.');

                result += sig_str + (upcase ? 'E' : 'e');
                rc = std::snprintf(nullptr, 0, "%.0Lf", ex);
                std::string exp_str(rc + 1, '\0');
                std::snprintf(exp_str.data(), exp_str.size(), upcase ? "%.0LF" : "%.0Lf", ex);
                exp_str.pop_back();
                result += exp_str;

            }

            if (sign == -1)
                result.insert(0, 1, '-');

            return result;

        }

        void LogScaleBase::do_parse_error(std::string_view str) {
            throw std::invalid_argument("Invalid number: " + quote(str));
        }

        bool LogScaleBase::do_try_parse(std::string_view str, std::string& sign, std::string& sig, std::string& ex) {

            static const Regex pattern(R"(
                ( [+-] )?
                ( \d+ (?: \.\d*)? | \.\d+ )
                (?: [Ee] ( [+-]? \d+ ) )?
                )", Regex::extended | Regex::full);

            auto match = pattern(str);

            if (! match)
                return false;

            sign = match[1];
            sig = match[2];
            ex = match[3];

            return true;

        }

    }

}
