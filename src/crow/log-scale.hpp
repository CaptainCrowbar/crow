#pragma once

#include "crow/format.hpp"
#include "crow/hash.hpp"
#include "crow/maths.hpp"
#include "crow/regex.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <compare>
#include <cstdlib>
#include <limits>
#include <ostream>
#include <string>
#include <string_view>

namespace Crow {

    namespace Detail {

        class LogScaleBase {
        protected:
            static std::string do_format(long double sig, long double ex, int8_t sign, FormatSpec spec);
            [[noreturn]] static void do_parse_error(std::string_view str);
            static bool do_try_parse(std::string_view str, std::string& sign, std::string& sig, std::string& ex);
        };

    }

    template <NonIntegralNumericType T>
    class LogScale:
    private Detail::LogScaleBase {

    public:

        using data_type = T;

        LogScale() noexcept {}
        LogScale(T x) noexcept;
        explicit LogScale(T ln, int sign) noexcept;

        explicit operator bool() const noexcept { return sign_ != 0; }
        explicit operator T() const noexcept { return get(); }
        LogScale operator+() const noexcept { return *this; }
        LogScale operator-() const noexcept { return make_unchecked(log_, - sign_); }
        LogScale& operator+=(LogScale y) noexcept { return *this = do_add(*this, y); }
        LogScale& operator-=(LogScale y) noexcept { return *this = do_add(*this, - y); }
        LogScale& operator*=(LogScale y) noexcept { return *this = do_multiply(*this, y); }
        LogScale& operator/=(LogScale y) noexcept { return *this = do_divide(*this, y); }

        std::strong_ordering compare(LogScale y) const noexcept;
        T get() const noexcept;
        size_t hash() const noexcept { return hash_mix(log_, sign_); }
        bool is_in_range() const noexcept { return sign_ == 0 || (log_ >= min_log() && log_ <= max_log()); }
        T log() const noexcept { return log_; }
        void parse(std::string_view str);
        bool try_parse(std::string_view str);
        int sign() const noexcept { return sign_; }
        std::string str(FormatSpec spec = {}) const;

        static LogScale min_in_range() noexcept { return make_unchecked(min_log(), 1); }
        static LogScale max_in_range() noexcept { return make_unchecked(max_log(), 1); }

        friend LogScale operator+(LogScale x, LogScale y) noexcept { return do_add(x, y); }
        friend LogScale operator-(LogScale x, LogScale y) noexcept { return do_add(x, - y); }
        friend LogScale operator*(LogScale x, LogScale y) noexcept { return do_multiply(x, y); }
        friend LogScale operator/(LogScale x, LogScale y) noexcept { return do_divide(x, y); }
        friend bool operator==(LogScale x, LogScale y) noexcept { return x.sign_ == y.sign_ && x.log_ == y.log_; }
        friend std::strong_ordering operator<=>(LogScale x, LogScale y) noexcept { return x.compare(y); }
        friend std::ostream& operator<<(std::ostream& out, LogScale x) { return out << x.str(); }

        friend LogScale abs(LogScale x) noexcept { return x.do_abs(); }
        friend LogScale exp(LogScale x) noexcept { return make_unchecked(x.get(), 1); }
        friend LogScale log(LogScale x) noexcept { return x.log_; }
        friend LogScale log2(LogScale x) noexcept { return x.log_ / ln2(); }
        friend LogScale log10(LogScale x) noexcept { return x.log_ / ln10(); }
        friend LogScale modf(LogScale x, LogScale* iptr) noexcept { return x.do_modf(iptr); }
        friend LogScale nextafter(LogScale x, LogScale y) noexcept { return x.do_next_after(y); }
        friend LogScale pow(LogScale x, LogScale y) noexcept { return x.do_power(y.get()); }
        template <NumericType U> friend LogScale pow(LogScale x, U y) noexcept { return x.do_power(y); }
        friend int sign_of(LogScale x) noexcept { return x.sign_; }

    private:

        using t_limits = std::numeric_limits<T>;

        T log_ = 0;
        int8_t sign_ = 0;

        LogScale do_abs() const noexcept;
        LogScale do_modf(LogScale* iptr) const noexcept;
        LogScale do_next_after(LogScale y) const noexcept;
        template <typename U> LogScale do_power(U y) const noexcept;
        void normalize() noexcept;

        static LogScale do_add(LogScale x, LogScale y) noexcept;
        static LogScale do_multiply(LogScale x, LogScale y) noexcept;
        static LogScale do_divide(LogScale x, LogScale y) noexcept;
        static T ln2() noexcept;
        static T ln10() noexcept;
        static LogScale make_unchecked(T ln, int sign) noexcept;
        static T min_log() noexcept;
        static T max_log() noexcept;
        static T unchecked_str_to_t(const std::string& str) noexcept;

    };

        template <NonIntegralNumericType T>
        LogScale<T>::LogScale(T x) noexcept {
            using std::abs;
            using std::log;
            if (x != T(0)) {
                log_ = log(abs(x));
                sign_ = x < T(0) ? -1 : 1;
            }
        }

        template <NonIntegralNumericType T>
        LogScale<T>::LogScale(T ln, int sign) noexcept:
        log_(ln),
        sign_(sign) {
            normalize();
        }

        template <NonIntegralNumericType T>
        std::strong_ordering LogScale<T>::compare(LogScale y) const noexcept {
            if (sign_ == 0 && y.sign_ == 0)
                return std::strong_ordering::equal;
            auto c = sign_ <=> y.sign_;
            if (c != 0)
                return c;
            int s = sign_of(log_ - y.log_);
            s *= sign_;
            return s <=> 0;
        }

        template <NonIntegralNumericType T>
        T LogScale<T>::get() const noexcept {
            using std::exp;
            return sign_ == 0 ? T(0) : T(sign_) * exp(log_);
        }

        template <NonIntegralNumericType T>
        void LogScale<T>::parse(std::string_view str) {
            if (! try_parse(str))
                do_parse_error(str);
        }

        template <NonIntegralNumericType T>
        bool LogScale<T>::try_parse(std::string_view str) {

            using std::log;

            log_ = {};
            sign_ = 0;
            std::string sign_str;
            std::string sig_str;
            std::string exp_str;

            if (! do_try_parse(str, sign_str, sig_str, exp_str))
                return false;

            T sig = unchecked_str_to_t(sig_str);

            if (sig == T(0))
                return true;

            log_ = log(sig);
            sign_ = sign_str[0] == '-' ? -1 : 1;

            if (! exp_str.empty())
                log_ += ln10() * unchecked_str_to_t(exp_str);

            return true;

        }

        template <NonIntegralNumericType T>
        std::string LogScale<T>::str(FormatSpec spec) const {

            using std::modf;
            using std::pow;

            T dec_log = {};

            if (sign_ != 0)
                dec_log = log_ / ln10();

            T dec_exp = {};
            T dec_frac = modf(dec_log, &dec_exp);

            if (dec_frac < T(0)) {
                dec_frac += T(1);
                dec_exp -= T(1);
            }

            T dec_sig = pow(T(10), dec_frac);
            auto ld_sig = static_cast<long double>(dec_sig);
            auto ld_exp = static_cast<long double>(dec_exp);

            return do_format(ld_sig, ld_exp, sign_, spec);

        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_abs() const noexcept {
            using std::abs;
            return make_unchecked(log_, abs(sign_));
        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_modf(LogScale* iptr) const noexcept {

            using std::modf;

            LogScale frac;

            if (sign_ == 0) {
                *iptr = {};
            } else if (log_ < min_log()) {
                frac = *this;
                *iptr = {};
            } else if (log_ <= max_log()) {
                T t_int = {};
                T t_frac = modf(get(), &t_int);
                frac = LogScale(t_frac);
                *iptr = LogScale(t_int);
            } else {
                *iptr = *this;
            }

            return frac;

        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_next_after(LogScale y) const noexcept {

            using std::nextafter;

            if (*this == y)
                return *this;
            else if (sign_ == 0)
                return LogScale(t_limits::lowest(), y.sign_);

            T target = sign_ == y.sign_ ? y.log_ : - t_limits::infinity();

            if (log_ == t_limits::lowest() && target < log_)
                return {};
            else
                return LogScale(nextafter(log_, target), sign_);

        }

        template <NonIntegralNumericType T>
        template <typename U>
        LogScale<T> LogScale<T>::do_power(U y) const noexcept {
            if (sign_ == 1)
                return make_unchecked(log_ * T(y), 1);
            else
                return {};
        }

        template <NonIntegralNumericType T>
        void LogScale<T>::normalize() noexcept {
            sign_ = std::clamp(sign_, int8_t(-1), int8_t(1));
            if (sign_ == 0)
                log_ = {};
        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_add(LogScale x, LogScale y) noexcept {

            using std::abs;
            using std::exp;
            using std::log;

            if (x.sign_ == 0)
                return y;

            if (y.sign_ == 0)
                return x;

            bool less = x.log_ < y.log_;
            T ratio = exp(- abs(x.log_ - y.log_));

            if (x.sign_ == y.sign_)
                ratio += T(1);
            else
                ratio = T(1) - ratio;

            T ln = std::max(x.log_, y.log_) + log(ratio);
            T sign = x.sign_ != y.sign_ && less ? y.sign_ : x.sign_;

            return make_unchecked(ln, sign);

        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_multiply(LogScale x, LogScale y) noexcept {
            return LogScale(x.log_ + y.log_, x.sign_ * y.sign_);
        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::do_divide(LogScale x, LogScale y) noexcept {
            return LogScale(x.log_ - y.log_, x.sign_ * y.sign_);
        }

        template <NonIntegralNumericType T>
        T LogScale<T>::ln2() noexcept {
            static const T value = T(0.69314'71805'59945'30941'72321'21458L);
            return value;
        }

        template <NonIntegralNumericType T>
        T LogScale<T>::ln10() noexcept {
            static const T value = T(2.30258'50929'94045'68401'79914'54684L);
            return value;
        }

        template <NonIntegralNumericType T>
        LogScale<T> LogScale<T>::make_unchecked(T ln, int sign) noexcept {
            LogScale x;
            x.log_ = ln;
            x.sign_ = int8_t(sign);
            return x;
        }

        template <NonIntegralNumericType T>
        T LogScale<T>::min_log() noexcept {

            static const T value = [] {

                using std::exp;

                static const T min_exp2 = t_limits::min_exponent * ln2();

                T a, b;

                for (a = min_exp2; exp(a) != T(0); a -= T(1)) {}
                for (b = min_exp2; exp(b) == T(0); b += T(1)) {}

                for (;;) {
                    T c = a + (b - a) / T(2);
                    if (c == a || c == b)
                        return b;
                    if (exp(c) == T(0))
                        a = c;
                    else
                        b = c;
                }

            }();

            return value;

        }

        template <NonIntegralNumericType T>
        T LogScale<T>::max_log() noexcept {

            static const T value = [] {

                using std::exp;

                static const T max_exp2 = t_limits::max_exponent * ln2();

                T a, b;

                for (a = max_exp2; exp(a) == t_limits::infinity(); a -= T(1)) {}
                for (b = max_exp2; exp(b) != t_limits::infinity(); b += T(1)) {}

                for (;;) {
                    T c = a + (b - a) / T(2);
                    if (c == a || c == b)
                        return a;
                    if (exp(c) == t_limits::infinity())
                        a = c;
                    else
                        b = c;
                }

            }();

            return value;

        }

        template <NonIntegralNumericType T>
        T LogScale<T>::unchecked_str_to_t(const std::string& str) noexcept {
            return T(std::strtold(str.data(), nullptr));
        }

}

namespace std {

    template <Crow::NonIntegralNumericType T>
    class numeric_limits<Crow::LogScale<T>>:
    public numeric_limits<T> {

    private:

        using NLT = numeric_limits<T>;
        using LS = Crow::LogScale<T>;

    public:

        static constexpr bool is_iec559               = false;
        static constexpr LS denorm_min() noexcept     { return LS(NLT::denorm_min(), 1); }
        static constexpr LS epsilon() noexcept        { return LS(NLT::epsilon(), 1); }
        static constexpr LS infinity() noexcept       { return LS(NLT::infinity(), 1); }
        static constexpr LS lowest() noexcept         { return LS(NLT::max(), -1); }
        static constexpr LS max() noexcept            { return LS(NLT::max(), 1); }
        static constexpr LS min() noexcept            { return LS(NLT::lowest(), 1); }
        static constexpr LS quiet_NaN() noexcept      { return LS(NLT::quiet_NaN(), 1); }
        static constexpr LS round_error() noexcept    { return LS(NLT::round_error(), 1); }
        static constexpr LS signaling_NaN() noexcept  { return LS(NLT::signaling_NaN(), 1); }

    };

}

CROW_STD_HASH_1(LogScale, Crow::NonIntegralNumericType)
