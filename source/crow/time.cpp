#include "crow/time.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <unordered_set>

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef _XOPEN_SOURCE
    #include <sys/time.h>
#endif

using namespace std::chrono;

namespace Crow {

    // Utility functions

    system_clock::time_point make_date(int year, int month, int day, int hour, int min, double sec, DT flags) {

        DT zone = flags & (DT::utc | DT::local);
        if (zone != DT::none && zone != DT::utc && zone != DT::local)
            throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));

        DT dst = {};
        if (zone == DT::local) {
            dst = flags & (DT::maybe_dst | DT::no_dst | DT::dst);
            if (dst != DT::none && dst != DT::maybe_dst && dst != DT::no_dst && dst != DT::dst)
                throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));
        }

        double isec = 0, fsec = modf(sec, &isec);
        if (fsec < 0) {
            isec -= 1;
            fsec += 1;
        }

        std::tm tm;
        std::memset(&tm, 0, sizeof(tm));
        tm.tm_sec = int(isec);
        tm.tm_min = min;
        tm.tm_hour = hour;
        tm.tm_mday = day;
        tm.tm_mon = month - 1;
        tm.tm_year = year - 1900;

        switch (dst) {
            case DT::no_dst:   tm.tm_isdst = 0; break;
            case DT::dst:  tm.tm_isdst = 1; break;
            default:        tm.tm_isdst = -1; break;
        }

        time_t t;

        if (zone == DT::local)
            t = std::mktime(&tm);
        else
            #ifdef _XOPEN_SOURCE
                t = timegm(&tm);
            #else
                t = _mkgmtime(&tm);
            #endif

        system_clock::time_point::rep extra(int64_t(fsec * system_clock::time_point::duration(seconds(1)).count()));

        return system_clock::from_time_t(t) + system_clock::time_point::duration(extra);

    }

    // Date formatting

    DateFormatter::DateFormatter(const std::string& format, DT flags):
    expanded_(), flags_(flags) {

        static const std::unordered_set<std::string> valid_codes = {
            "yy", "yyyy", "m", "mm", "mmm", "Mmm", "MMM", "d", "dd", "www", "Www", "WWW",
            "H", "HH", "M", "MM", "S", "SS", "ZZZZ"
        };

        for (auto i = format.begin(), end = format.end(); i != end;) {

            auto j = std::find_if(i, format.end(), ascii_isalnum);

            if (j != i)
                expanded_.emplace_back(i, j);

            char initial = ascii_toupper(*j);
            i = std::find_if(j, end, [initial] (char c) { return ascii_toupper(c) != initial; });
            std::string code(j, i);

            if (code[0] != 's' && valid_codes.count(code) == 0)
                throw std::invalid_argument("Invalid date format: " + quote(format));

            expanded_.push_back(code);

        }

        DT zone = flags & (DT::utc | DT::local);

        if (zone != DT::none && zone != DT::utc && zone != DT::local)
            throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));

    }

    std::string DateFormatter::operator()(system_clock::time_point tp) const {

        static constexpr const char* month_name[] = {
            "jan", "feb", "mar", "apr", "may", "jun",
            "jul", "aug", "sep", "oct", "nov", "dec",
        };

        static constexpr const char* weekday_name[] = {
            "sun", "mon", "tue", "wed", "thu", "fri", "sat",
        };

        static const auto fraction_str = [] (double x, size_t len) {
            x *= std::pow(10.0, double(len));
            auto str = std::to_string(std::llround(x));
            return pad_left(str, len, '0');
        };

        static const auto zone_offset = [] (std::tm& tm) {
            std::string buf(10, '\0');
            size_t n = std::strftime(buf.data(), buf.size(), "%z", &tm);
            buf.resize(n);
            return buf;
        };

        std::time_t tt = system_clock::to_time_t(tp);
        auto diff = tp - system_clock::from_time_t(tt);

        if (diff.count() < 0) {
            --tt;
            diff += 1s;
        }

        double fraction = duration_cast<duration<double>>(diff).count();
        std::tm tm;

        if (!! (flags_ & DT::local))
            tm = *std::localtime(&tt);
        else
            tm = *std::gmtime(&tt);

        std::string result;

        for (auto& code: expanded_) {

            if (! ascii_isalnum(code[0]))  result += code;
            else if (code == "yyyy")       result += std::to_string(tm.tm_year + 1900);
            else if (code == "yy")         result += format_integer(tm.tm_year % 100, "n2");
            else if (code == "m")          result += std::to_string(tm.tm_mon + 1);
            else if (code == "mm")         result += format_integer(tm.tm_mon + 1, "n2");
            else if (code == "mmm")        result += month_name[tm.tm_mon];
            else if (code == "Mmm")        result += ascii_titlecase(month_name[tm.tm_mon]);
            else if (code == "MMM")        result += ascii_uppercase(month_name[tm.tm_mon]);
            else if (code == "d")          result += std::to_string(tm.tm_mday);
            else if (code == "dd")         result += format_integer(tm.tm_mday, "n2");
            else if (code == "www")        result += weekday_name[tm.tm_wday];
            else if (code == "Www")        result += ascii_titlecase(weekday_name[tm.tm_wday]);
            else if (code == "WWW")        result += ascii_uppercase(weekday_name[tm.tm_wday]);
            else if (code == "H")          result += std::to_string(tm.tm_hour);
            else if (code == "HH")         result += format_integer(tm.tm_hour, "n2");
            else if (code == "MM")         result += format_integer(tm.tm_min, "n2");
            else if (code == "SS")         result += format_integer(tm.tm_sec, "n2");
            else if (code[0] == 's')       result += fraction_str(fraction, code.size());
            else if (code == "ZZZZ")       result += !! (flags_ & DT::local) ? zone_offset(tm) : "+0000";
        }

        return result;

    }

    std::string format_date(system_clock::time_point tp, const std::string& format, DT flags) {
        return DateFormatter(format, flags)(tp);
    }

    std::string iso_date(system_clock::time_point tp, int prec, DT flags) {

        DT zone = flags & (DT::utc | DT::local);
        if (zone != DT::none && zone != DT::utc && zone != DT::local)
            throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));

        auto t = system_clock::to_time_t(tp);
        std::tm tm = zone == DT::local ? *localtime(&t) : *gmtime(&t);
        std::string result(30, '\0');
        auto rc = strftime(&result[0], result.size(), "%Y-%m-%d %H:%M:%S", &tm);
        result.resize(rc);

        if (prec > 0) {
            double sec = duration_cast<duration<double>>(tp.time_since_epoch()).count();
            double isec = 0;
            double fsec = std::modf(sec, &isec);
            std::string buf(prec + 3, '\0');
            snprintf(&buf[0], buf.size(), "%.*f", prec, fsec);
            result += buf.data() + 1;
        }

        return result;

    }

    std::string format_time_point(system_clock::time_point tp, const FormatSpec& spec) {

        DT zone = spec.option('l') ? DT::local : DT::utc;
        bool date_only = spec.option('d') && ! spec.option('t');
        bool time_only = ! spec.option('d') && spec.option('t');
        std::string result;

        if (time_only) {

            result = iso_date(tp, spec.prec(), zone);
            auto sp = result.find(' ');
            result.erase(0, sp + 1);

        } else if (spec.lcmode() == 'd') {

            std::string format = "d Mmm yyyy";

            if (spec.mode() == 'D')
                format.insert(0, "Www ");

            if (! date_only) {
                format += " HH:MM:SS";
                if (spec.prec() > 0) {
                    format += '.';
                    format.append(spec.prec(), 's');
                }
            }

            result = format_date(tp, format, zone);

        } else {

            result = iso_date(tp, spec.prec(), zone);
            size_t sp = 0;

            if (spec.mode() == 'I' || date_only)
                sp = result.find(' ');

            if (date_only)
                result.erase(sp, npos);
            else if (spec.mode() == 'I')
                result[sp] = 'T';

        }

        return result;

    }

    // Time formatting

    namespace Detail {

        std::string format_time_helper(int64_t isec, double fsec, int prec) {

            std::string result;

            if (isec < 0 || fsec < 0)
                result += '-';

            isec = std::abs(isec);
            fsec = std::abs(fsec);
            int64_t d = isec / 86400;
            isec -= 86400 * d;

            if (d != 0)
                result += std::to_string(d) + 'd';

            int64_t h = isec / 3600;
            isec -= 3600 * h;

            if (d != 0 || h != 0)
                result += format_integer(h, d == 0 ? "n1" : "n2") + 'h';

            int64_t m = isec / 60;

            if (d != 0 || h != 0 || m != 0)
                result += format_integer(m, d == 0 && h == 0 ? "n1" : "n2") + 'm';

            isec -= 60 * m;
            result += format_integer(isec, d == 0 && h == 0 && m == 0 ? "n1" : "n2");

            if (prec > 0) {
                std::string buf(prec + 3, '\0');
                snprintf(&buf[0], buf.size(), "%.*f", prec, fsec);
                result += buf.data() + 1;
            }

            result += 's';

            return result;

        }

    }

    // Date parsing

    namespace {

        void date_skip_punct(const char*& ptr, const char* end) {
            while (ptr != end && (ascii_ispunct(*ptr) || ascii_isspace(*ptr)))
                ++ptr;
        };

        bool date_read_number(const char*& ptr, const char* end, int& result) {
            date_skip_punct(ptr, end);
            auto next = std::find_if_not(ptr, end, ascii_isdigit);
            if (next == ptr)
                return false;
            std::string sub(ptr, next);
            char* dummy = nullptr;
            result = int(std::strtoul(sub.data(), &dummy, 10));
            ptr = next;
            return true;
        };

        bool date_read_number(const char*& ptr, const char* end, double& result) {
            date_skip_punct(ptr, end);
            if (ptr == end || ! ascii_isdigit(*ptr))
                return false;
            auto next = std::find_if_not(ptr, end, ascii_isdigit);
            if (next != end && *next == '.')
                next = std::find_if_not(next + 1, end, ascii_isdigit);
            std::string sub(ptr, next);
            char* dummy = nullptr;
            result = std::strtod(sub.data(), &dummy);
            ptr = next;
            return true;
        };

        bool date_read_month(const char*& ptr, const char* end, int& result) {

            date_skip_punct(ptr, end);

            if (ptr == end)
                return false;
            else if (ascii_isdigit(*ptr))
                return date_read_number(ptr, end, result);
            else if (end - ptr < 3 || ! ascii_isalpha(*ptr))
                return false;

            std::string mon(ptr, 3);
            mon = ascii_lowercase(mon);

            if (mon == "jan")       result = 1;
            else if (mon == "feb")  result = 2;
            else if (mon == "mar")  result = 3;
            else if (mon == "apr")  result = 4;
            else if (mon == "may")  result = 5;
            else if (mon == "jun")  result = 6;
            else if (mon == "jul")  result = 7;
            else if (mon == "aug")  result = 8;
            else if (mon == "sep")  result = 9;
            else if (mon == "oct")  result = 10;
            else if (mon == "nov")  result = 11;
            else if (mon == "dec")  result = 12;
            else                    return false;

            ptr = std::find_if_not(ptr + 3, end, ascii_isalpha);

            return true;

        };

    }

    system_clock::time_point parse_date(const std::string& str, DT flags) {

        DT order = flags & (DT::ymd_order | DT::dmy_order | DT::mdy_order);
        if (order != DT::none && order != DT::ymd_order && order != DT::dmy_order && order != DT::mdy_order)
            throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));

        DT zone = flags & (DT::utc | DT::local);
        if (zone != DT::none && zone != DT::utc && zone != DT::local)
            throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));

        DT dst = DT::none;
        if (zone == DT::local) {
            dst = flags & (DT::maybe_dst | DT::no_dst | DT::dst);
            if (dst != DT::none && dst != DT::maybe_dst && dst != DT::no_dst && dst != DT::dst)
                throw std::invalid_argument("Invalid date flags: 0x" + format_integer(int(flags), "x"));
        }

        int year = 0, month = 0, day = 0, hour = 0, min = 0;
        double sec = 0;
        auto ptr = str.data();
        auto end = ptr + str.size();
        bool ok = true;

        switch (order) {
            case DT::dmy_order:
                ok = date_read_number(ptr, end, day) && date_read_month(ptr, end, month) && date_read_number(ptr, end, year);
                break;
            case DT::mdy_order:
                ok = date_read_month(ptr, end, month) && date_read_number(ptr, end, day) && date_read_number(ptr, end, year);
                break;
            default:
                ok = date_read_number(ptr, end, year) && date_read_month(ptr, end, month) && date_read_number(ptr, end, day);
                break;
        }

        if (! ok)
            throw std::invalid_argument("Invalid date: " + quote(str));

        date_skip_punct(ptr, end);

        if (ptr != end && (*ptr == 'T' || *ptr == 't'))
            ++ptr;

        date_read_number(ptr, end, hour);
        date_read_number(ptr, end, min);
        date_read_number(ptr, end, sec);

        return make_date(year, month, day, hour, min, sec, dst | zone);

    }

    // Time parsing

    namespace Detail {

        double parse_time_helper(const std::string& str) {

            static constexpr double jyear = 86400 * 365.25;

            std::string nstr = replace(str, " ", "");
            auto ptr = nstr.data();
            auto end = ptr + nstr.size();
            char sign = '+';

            if (ptr != end && (*ptr == '+' || *ptr == '-'))
                sign = *ptr++;
            if (ptr == end)
                throw std::invalid_argument("Invalid time: " + quote(str));

            double count = 0, seconds = 0;
            char* next = nullptr;
            std::string unit;

            while (ptr != end) {

                if (! ascii_isdigit(*ptr))
                    throw std::invalid_argument("Invalid time: " + quote(str));

                count = std::strtod(ptr, &next);
                unit.clear();

                while (next != end && ascii_isalpha(*next))
                    unit += *next++;

                auto prefix = unit.substr(0, 2);

                if (prefix == "Yy")       count *= jyear * 1e24;
                else if (prefix == "Zy")  count *= jyear * 1e21;
                else if (prefix == "Ey")  count *= jyear * 1e18;
                else if (prefix == "Py")  count *= jyear * 1e15;
                else if (prefix == "Ty")  count *= jyear * 1e12;
                else if (prefix == "Gy")  count *= jyear * 1e9;
                else if (prefix == "My")  count *= jyear * 1e6;
                else if (prefix == "ky")  count *= jyear * 1e3;
                else if (prefix == "ms")  count *= 1e-3;
                else if (prefix == "us")  count *= 1e-6;
                else if (prefix == "µs")  count *= 1e-6;
                else if (prefix == "ns")  count *= 1e-9;
                else if (prefix == "ps")  count *= 1e-12;
                else if (prefix == "fs")  count *= 1e-15;
                else if (prefix == "as")  count *= 1e-18;
                else if (prefix == "zs")  count *= 1e-21;
                else if (prefix == "ys")  count *= 1e-24;
                else if (unit[0] == 'y')  count *= jyear;
                else if (unit[0] == 'd')  count *= 86400;
                else if (unit[0] == 'h')  count *= 3600;
                else if (unit[0] == 'm')  count *= 60;
                else if (unit[0] == 's')  {}
                else                      throw std::invalid_argument("Invalid time: " + quote(str));

                seconds += count;
                ptr = next;

            }

            if (sign == '-')
                seconds = - seconds;

            return seconds;

        }

    }

    // Time conversion functions

    void timepoint_to_timespec(const std::chrono::system_clock::time_point& tp, std::timespec& ts) noexcept {
        using namespace std::chrono;
        duration_to_timespec(tp - system_clock::time_point(), ts);
    }

    void timespec_to_timepoint(const std::timespec& ts, std::chrono::system_clock::time_point& tp) noexcept {
        using namespace std::chrono;
        system_clock::duration d;
        timespec_to_duration(ts, d);
        tp = system_clock::time_point() + d;
    }

    namespace Detail {

        #ifdef _XOPEN_SOURCE
            using TvSec = time_t;
            using TvUsec = suseconds_t;
        #else
            using TvSec = long;
            using TvUsec = long;
        #endif

        void make_timeval(int64_t us, void* tvptr) noexcept {
            static constexpr int64_t M = 1'000'000;
            auto& tv = *static_cast<timeval*>(tvptr);
            tv = {TvSec(us / M), TvUsec(us % M)};
        }

        void read_timeval(const void* tvptr, seconds& s, microseconds& us) noexcept {
            auto& tv = *static_cast<const timeval*>(tvptr);
            s = seconds(tv.tv_sec);
            us = microseconds(tv.tv_usec);
        }

        #ifdef _WIN32

            void filetime_to_timepoint_helper(const void* ftptr, system_clock::time_point& tp) noexcept {
                static constexpr int64_t filetime_freq = 10'000'000;        // FILETIME ticks (100 ns) per second
                static constexpr int64_t windows_epoch = 11'644'473'600ll;  // Windows epoch (1601) to Unix epoch (1970) in seconds
                auto& ft = *static_cast<const FILETIME*>(ftptr);
                int64_t ticks = (int64_t(ft.dwHighDateTime) << 32) + int64_t(ft.dwLowDateTime);
                int64_t sec = ticks / filetime_freq - windows_epoch;
                int64_t nsec = 100ll * (ticks % filetime_freq);
                tp = system_clock::from_time_t(time_t(sec)) + duration_cast<system_clock::duration>(nanoseconds(nsec));
            }

            void timepoint_to_filetime_helper(const system_clock::time_point& tp, void* ftptr) noexcept {
                static constexpr uint64_t filetime_freq = 10'000'000;        // FILETIME ticks (100 ns) per second
                static constexpr uint64_t windows_epoch = 11'644'473'600ll;  // Windows epoch (1601) to Unix epoch (1970) in seconds
                auto& ft = *static_cast<FILETIME*>(ftptr);
                auto unix_time = tp - system_clock::from_time_t(0);
                uint64_t nsec = duration_cast<nanoseconds>(unix_time).count();
                uint64_t ticks = nsec / 100ll + filetime_freq * windows_epoch;
                ft = {uint32_t(ticks & 0xfffffffful), uint32_t(ticks >> 32)};
            }

        #endif

    }

}
