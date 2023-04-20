#include "crow/probability.hpp"

namespace Crow {

    namespace Detail {

        std::string format_probability_helper(FormatSpec spec, int band,
                std::function<std::string(FormatSpec)> g_format,
                std::function<std::string(FormatSpec)> d_format) {

            static const FormatSpec default_format("pz6");

            if (spec.empty()) {
                spec = default_format;
            } else {
                spec.default_mode('p');
                spec.default_prec(6);
            }

            if (spec.lcmode() != 'p')
                return g_format(spec);

            bool pc = spec.mode() == 'P';

            if (band == 0)
                return "0";
            else if (band == 4)
                return pc ? "100" : "1";

            auto s = d_format(spec);

            if (band == 3) {

                int a = 2 * '0' + 10;
                int b = a - 1;

                for (auto i = int(s.find_last_not_of("0.")); i >= 0; --i, a = b)
                    if (ascii_isdigit(s[i]))
                        s[i] = char(a - s[i]);

                if (! pc)
                    s[0] = '0';
                else if (s.size() == 1 || s[1] == '.')
                    s.insert(0, 1, '9');

            }

            if (spec.option('z') && s.find('.') != npos) {
                s = trim_right(s, "0");
                s = trim_right(s, ".");
            }

            return s;

        }

    }

}
