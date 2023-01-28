#include "crow/approx.hpp"
#include "crow/regex.hpp"
#include <algorithm>
#include <stdexcept>

namespace Crow {

    namespace Detail {

        char approx_options(FormatSpec& spec, size_t tsize) {

            if (tsize <= sizeof(float))
                spec.default_prec(6);
            else
                spec.default_prec(10);

            bool l = spec.option('l');
            bool r = spec.option('r');
            bool x = spec.option('x');

            if (int(l) + int(r) + int(x) > 1)
                throw std::invalid_argument("Invalid format spec: " + spec.str());

            return r ? 'r' : x ? 'x' : 'l';

        }

        void approx_extract(std::string_view str, std::string& vstr, std::string& estr) {

            static const Regex pattern(R"(
                (                                            # [1] Significand
                    [+-]?
                    (?:
                        [\d']+
                        \.? ( [\d']* )                       # [2]
                        | \. ( [\d']+ )                      # [3]
                    )
                )
                (?: \( ([\d']+) \) )?                        # [4] Error in ulps
                ( [Ee] [+-]? [\d']+ )?                       # [5] Exponent
                (?:
                    (?: ± | \+/?- )                          # Plus or minus
                    (                                        # [6] Error
                        (?: [\d']+ \.? [\d']* | \. [\d']+ )
                        (?: [Ee] [+-]? [\d']+ )?
                    )
                )?
                )", Regex::extended | Regex::full);

            auto match = pattern(str);

            if (! match || (match.matched(4) && match.matched(6)))
                throw std::invalid_argument("Invalid approximate value: " + quote(str));

            vstr = match[1];
            vstr += match[5];
            std::string_view decimal_part;

            if (match.matched(2))
                decimal_part = match[2];
            else if (match.matched(3))
                decimal_part = match[3];

            size_t decimals = std::count_if(decimal_part.begin(), decimal_part.end(), ascii_isdigit);

            if (match.matched(4)) {

                // Error in ulps

                estr = match[4];
                std::erase(estr, '\'');

                if (estr.size() > decimals) {
                    estr.insert(estr.size() - decimals, 1, '.');
                } else {
                    estr.insert(0, decimals - estr.size(), '0');
                    estr.insert(0, "0.");
                }

                estr += match[5];

            } else if (match.matched(6)) {

                // Explicit error

                estr = match[6];

            } else {

                // Error implied by last digit

                estr = "0.";
                estr.append(decimals, '0');
                estr += '5';
                estr += match[5];

            }

            std::erase(vstr, '\'');
            std::erase(estr, '\'');

        }

    }

}
