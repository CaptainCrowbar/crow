#pragma once

#include "crow/format-numeric.hpp"
#include "crow/format-spec.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace Crow {

    namespace Detail {

        std::string trim_exponent(const std::string& str, bool xsign);
        std::string trim_zeros(const std::string& str);

        template <typename T, typename U>
        std::string native_format(const char* pattern, U u, int prec) {
            std::string result(15, '\0');
            int rc = 0;
            for (;;) {
                rc = std::snprintf(result.data(), result.size(), pattern, prec, T(u));
                if (rc < int(result.size()))
                    break;
                result.resize(rc + 1);
            }
            result.resize(rc);
            return result;
        }

        template <typename T>
        std::string format_float_d(T t, const FormatSpec& spec) {
            int prec = std::max(spec.prec() - 1, 0);
            std::string native;
            if constexpr (sizeof(T) <= sizeof(double))
                native = native_format<double>("%.*e", t, prec);
            else
                native = native_format<long double>("%.*Le", t, prec);
            size_t sig_pos = size_t(native[0] == '-');
            size_t exp_pos = native.find_first_of("Ee", sig_pos);
            auto result = native.substr(sig_pos, exp_pos - sig_pos);
            if (result[1] == '.')
                result.erase(1, 1);
            int exp = to_int(native.substr(exp_pos + 1));
            if (exp < 0) {
                result.insert(0, "0.");
                result.insert(2, - exp - 1, '0');
            } else if (exp < prec) {
                result.insert(exp + 1, 1, '.');
            } else if (exp > prec - 1) {
                result.append(exp - prec, '0');
            }
            if (sig_pos > 0)
                result.insert(0, 1, '-');
            return result;
        }

        template <typename T>
        std::string format_float_e(T t, const FormatSpec& spec) {
            bool cap = ascii_isupper(spec.mode());
            int prec = std::max(spec.prec() - 1, 0);
            std::string native;
            if constexpr (sizeof(T) <= sizeof(double))
                native = native_format<double>(cap ? "%.*E" : "%.*e", t, prec);
            else
                native = native_format<long double>(cap ? "%.*LE" : "%.*Le", t, prec);
            return trim_exponent(native, spec.option('S'));
        }

        template <typename T>
        std::string format_float_f(T t, const FormatSpec& spec) {
            std::string result;
            if constexpr (sizeof(T) <= sizeof(double))
                result = native_format<double>("%.*f", t, spec.prec());
            else
                result = native_format<long double>("%.*Lf", t, spec.prec());
            if (result[0] == '-' && result.find_first_not_of("0.-") == npos)
                result.erase(0, 1);
            return result;
        }

        template <typename T>
        std::string format_float_g(T t, const FormatSpec& spec) {
            auto y = std::abs(t);
            if (y == 0 || (y >= 1e-3 && y < 1e6))
                return format_float_d(t, spec);
            else
                return format_float_e(t, spec);
        }

        template <typename T>
        std::string format_float_p(T t, const FormatSpec& spec) {
            bool pc = spec.mode() == 'P';
            if (t < 0 || t > 1)
                throw std::domain_error("Probability is out of range: expected 0-1, found " + std::to_string(t));
            else if (t == 0)
                return "0";
            else if (t == 1)
                return pc ? "100" : "1";
            T k = T(pc ? 100 : 1);
            if (t < T(0.9))
                return format_float_d(k * t, spec);
            auto result = format_float_d(k * (1 - t), spec);
            int a = 2 * '0' + 10;
            int b = a - 1;
            for (auto i = int(result.find_last_not_of('0')); i >= 0; --i, a = b)
                if (ascii_isdigit(result[i]))
                    result[i] = char(a - result[i]);
            if (pc)
                result.insert(0, 1, '9');
            else
                result[0] = '0';
            return result;
        }

    }

    template <typename T>
    std::string format_floating_point(T t, FormatSpec spec = {}) {

        if constexpr(std::is_integral_v<T>) {

            using floating_type = std::conditional_t<(sizeof(T) >= sizeof(double)), long double, double>;

            return format_floating_point(floating_type(t), spec);

        } else {

            static_assert(std::is_floating_point_v<T>);

            static const FormatSpec default_spec("gz6");

            if (t == 0 && spec.option('Z'))
                return "--";
            else if (spec.empty())
                spec = default_spec;
            spec.default_prec(6);
            std::string result;

            switch (spec.lcmode()) {
                case 'd':  result = Detail::format_float_d(t, spec); break;
                case 'e':  result = Detail::format_float_e(t, spec); break;
                case 'f':  result = Detail::format_float_f(t, spec); break;
                case 'p':  result = Detail::format_float_p(t, spec); break;
                default:   result = Detail::format_float_g(t, spec); break;
            }

            if (spec.option('z'))
                result = Detail::trim_zeros(result);
            Detail::expand_formatted_number(result, spec);

            return result;

        }

    }

}
