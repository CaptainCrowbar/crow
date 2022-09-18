#pragma once

#include "crow/format-floating.hpp"
#include "crow/format-numeric.hpp"
#include "crow/format-spec.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <string>
#include <type_traits>

namespace Crow {

    template <typename T>
    std::string format_integer(T t, FormatSpec spec = {}) {

        static_assert(std::is_integral_v<T>);

        if (spec.lcmode() >= 'd' && spec.lcmode() <= 'g')
            return format_floating_point(t, spec);
        else if (t == 0 && spec.option('Z'))
            return "--";
        else if (spec.lcmode() == 'r')
            return roman(t, spec.mode() == 'r');

        T base;

        switch (spec.lcmode()) {
            case 'b':  base = 2; break;
            case 'x':  base = 16; break;
            default:   base = 10; break;
        }

        bool neg = false;

        if constexpr (std::is_signed_v<T>) {
            neg = t < 0;
            if (neg)
                t = - t;
        }

        spec.default_prec(1);
        auto xdigits = spec.mode() == 'X' ? Detail::hex_digits_uc : Detail::hex_digits_lc;
        std::string result;

        for (; t != 0 || result.size() < size_t(spec.prec()); t /= base)
            result += xdigits[t % base];

        if (neg)
            result += '-';

        std::reverse(result.begin(), result.end());
        Detail::expand_formatted_number(result, spec);

        return result;
    }

}
