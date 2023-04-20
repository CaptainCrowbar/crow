#pragma once

#include "crow/enum.hpp"
#include "crow/format-floating.hpp"
#include "crow/format-integer.hpp"
#include "crow/format-spec.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <chrono>
#include <concepts>
#include <ctime>
#include <ratio>
#include <string>
#include <type_traits>
#include <vector>

namespace Crow {

    // Supporting types

    using Nanoseconds   = std::chrono::duration<double, std::nano>;
    using Microseconds  = std::chrono::duration<double, std::micro>;
    using Milliseconds  = std::chrono::duration<double, std::milli>;
    using Seconds       = std::chrono::duration<double>;
    using Minutes       = std::chrono::duration<double, std::ratio<60>>;
    using Hours         = std::chrono::duration<double, std::ratio<3'600>>;
    using Days          = std::chrono::duration<double, std::ratio<86'400>>;
    using Years         = std::chrono::duration<double, std::ratio<31'557'600>>; // Julian year

    enum class DT: int {
        none       = 0,
        ymd_order  = 1 << 0,
        dmy_order  = 1 << 1,
        mdy_order  = 1 << 2,
        maybe_dst  = 1 << 3,
        no_dst     = 1 << 4,
        dst        = 1 << 5,
        utc        = 1 << 6,
        local      = 1 << 7,
    };

    CROW_BITMASK_OPERATORS(DT)

    // Utility functions

    std::chrono::system_clock::time_point make_date(int year, int month, int day,
        int hour = 0, int min = 0, double sec = 0, DT flags = DT::maybe_dst | DT::utc);

    // Date formatting

    class DateFormatter {
    public:
        DateFormatter() = default;
        explicit DateFormatter(const std::string& format, DT flags = DT::utc);
        std::string operator()(std::chrono::system_clock::time_point tp) const;
    private:
        std::vector<std::string> expanded_;
        DT flags_ = DT::utc;
    };

    std::string format_date(std::chrono::system_clock::time_point tp, const std::string& format, DT flags = DT::utc);
    std::string iso_date(std::chrono::system_clock::time_point tp, int prec = 0, DT flags = DT::utc);
    std::string format_time_point(std::chrono::system_clock::time_point tp, const FormatSpec& spec);

    // Time formatting

    namespace Detail {

        std::string format_time_helper(int64_t isec, double fsec, int prec);

    }

    template <typename R, typename P>
    std::string format_time(std::chrono::duration<R, P> time, FormatSpec spec) {
        using namespace std::chrono;
        if (spec.mode() == 's') {
            auto sec = duration_cast<duration<double>>(time).count();
            spec.default_prec(0);
            FormatSpec fp_spec('f', spec.options(), spec.prec());
            return format_floating_point(sec, fp_spec);
        } else {
            auto whole = duration_cast<seconds>(time);
            int64_t isec = whole.count();
            auto frac = time - duration_cast<duration<R, P>>(whole);
            double fsec = duration_cast<duration<double>>(frac).count();
            return Detail::format_time_helper(isec, fsec, spec.prec());
        }
    }

    // Date parsing

    std::chrono::system_clock::time_point parse_date(const std::string& str,
        DT flags = DT::ymd_order | DT::maybe_dst | DT::utc);

    // Time parsing

    namespace Detail {

        template <typename T> struct IsDurationType: std::false_type {};
        template <typename R, typename P> struct IsDurationType<std::chrono::duration<R, P>>: std::true_type {};
        template <typename T> concept DurationType = IsDurationType<T>::value;

        double parse_time_helper(const std::string& str);

    }

    template <typename R, typename P>
    void parse_time(const std::string& str, std::chrono::duration<R, P>& t) {
        using namespace std::chrono;
        auto s = Detail::parse_time_helper(str);
        t = duration_cast<duration<R, P>>(duration<double>(s));
    }

    template <Detail::DurationType D>
    D parse_time(const std::string& str) {
        D time;
        parse_time(str, time);
        return time;
    }

    // Time conversion functions

    namespace Detail {

        template <typename C1, typename D1, typename C2, typename D2>
        struct ConvertTimePoint {
            std::chrono::time_point<C2, D2> operator()(std::chrono::time_point<C1, D1> t) const {
                auto now1 = C1::now();
                auto now2 = C2::now();
                auto now3 = C1::now();
                now1 += (now3 - now1) / 2;
                return std::chrono::time_point_cast<D2>(now2 + (t - now1));
            }
        };

        template <typename C, typename D1, typename D2>
        struct ConvertTimePoint<C, D1, C, D2> {
            std::chrono::time_point<C, D2> operator()(std::chrono::time_point<C, D1> t) const {
                return std::chrono::time_point_cast<D2>(t);
            }
        };

        void make_timeval(int64_t us, void* tvptr) noexcept;
        void read_timeval(const void* tvptr, std::chrono::seconds& s, std::chrono::microseconds& us) noexcept;

    }

    template <typename C1, typename D1, typename C2, typename D2>
    void convert_time_point(std::chrono::time_point<C1, D1> src, std::chrono::time_point<C2, D2>& dst) {
        dst = Detail::ConvertTimePoint<C1, D1, C2, D2>()(src);
    }

    template <typename R, typename P>
    void duration_to_timespec(const std::chrono::duration<R, P>& d, std::timespec& ts) noexcept {
        using namespace std::chrono;
        static constexpr int64_t G = 1'000'000'000ll;
        int64_t ns = duration_cast<nanoseconds>(d).count();
        ts = {time_t(ns / G), long(ns % G)};
    }

    template <typename R, typename P, typename TV>
    void duration_to_timeval(const std::chrono::duration<R, P>& d, TV& tv) noexcept {
        using namespace std::chrono;
        int64_t us = duration_cast<microseconds>(d).count();
        Detail::make_timeval(us, &tv);
    }

    template <typename R, typename P>
    void timespec_to_duration(const std::timespec& ts, std::chrono::duration<R, P>& d) noexcept {
        using namespace std::chrono;
        using D = duration<R, P>;
        d = duration_cast<D>(seconds(ts.tv_sec)) + duration_cast<D>(nanoseconds(ts.tv_nsec));
    }

    template <typename R, typename P, typename TV>
    void timeval_to_duration(const TV& tv, std::chrono::duration<R, P>& d) noexcept {
        using namespace std::chrono;
        using D = duration<R, P>;
        seconds s;
        microseconds us;
        Detail::read_timeval(&tv, s, us);
        d = duration_cast<D>(s) + duration_cast<D>(us);
    }

    template <typename TV>
    void timepoint_to_timeval(const std::chrono::system_clock::time_point& tp, TV& tv) noexcept {
        using namespace std::chrono;
        duration_to_timeval(tp - system_clock::time_point(), tv);
    }

    template <typename TV>
    void timeval_to_timepoint(const TV& tv, std::chrono::system_clock::time_point& tp) noexcept {
        using namespace std::chrono;
        system_clock::duration d;
        timeval_to_duration(tv, d);
        tp = system_clock::time_point() + d;
    }

    void timepoint_to_timespec(const std::chrono::system_clock::time_point& tp, std::timespec& ts) noexcept;
    void timespec_to_timepoint(const std::timespec& ts, std::chrono::system_clock::time_point& tp) noexcept;

    #ifdef _WIN32

        namespace Detail {

            void filetime_to_timepoint_helper(const void* ftptr, std::chrono::system_clock::time_point& tp) noexcept;
            void timepoint_to_filetime_helper(const std::chrono::system_clock::time_point& tp, void* ftptr) noexcept;

        }

        template <typename FT>
        void filetime_to_timepoint(const FT& ft, std::chrono::system_clock::time_point& tp) noexcept {
            Detail::filetime_to_timepoint_helper(&ft, tp);
        }

        template <typename FT>
        void timepoint_to_filetime(const std::chrono::system_clock::time_point& tp, FT& ft) noexcept {
            Detail::timepoint_to_filetime_helper(tp, &ft);
        }

    #endif

    // Timed wait base class

    class Waiter {
    public:
        using clock = std::chrono::system_clock;
        using duration = clock::duration;
        using time_point = clock::time_point;
        virtual ~Waiter() noexcept = 0;
        virtual bool poll() { return do_wait_for({}); }
        virtual void wait() { while (! wait_for(std::chrono::seconds(1))) {} }
        virtual bool wait_until(time_point t) { return do_wait_for(t - clock::now()); }
        template <typename R, typename P> bool wait_for(std::chrono::duration<R, P> t);
    protected:
        virtual bool do_wait_for(duration t) { return wait_until(clock::now() + t); }
    };

        inline Waiter::~Waiter() noexcept {}

        template <typename R, typename P>
        bool Waiter::wait_for(std::chrono::duration<R, P> t) {
            using namespace std::chrono;
            auto dt = duration_cast<duration>(t);
            return do_wait_for(dt);
        }

}
