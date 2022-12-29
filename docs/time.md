# Time and Date Functions

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/time.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
using Nanoseconds = std::chrono::duration<double, std::nano>;
using Microseconds = std::chrono::duration<double, std::micro>;
using Milliseconds = std::chrono::duration<double, std::milli>;
using Seconds = std::chrono::duration<double>;
using Minutes = std::chrono::duration<double, std::ratio<60>>;
using Hours = std::chrono::duration<double, std::ratio<3'600>>;
using Days = std::chrono::duration<double, std::ratio<86'400>>;
using Years = std::chrono::duration<double, std::ratio<31'557'600>>;
```

Floating point duration types. The `Years` type represents Julian years
(365.25 days).

```c++
enum class DT: int {
    none = 0,
    ymd_order,
    dmy_order,
    mdy_order,
    maybe_dst,
    no_dst,
    dst,
    utc,
    local,
};
```

Bitmask flags used to control the behaviour of date and time conversion
functions. These fall into three groups; functions that accept flags will
throw `std::invalid_argument` if more than one flag from the same group is
present. In all cases, the first value in each group is the default.

* `ymd_order, dmy_order, mdy_order` -- Indicate which order to expect the
  components to be in when parsing a date.
* `maybe_dst, no_dst, dst` -- Indicate whether DST is in effect when
  converting a broken down local time to a time point.
* `utc, local` -- Indicate whether a date and time is expressed in UTC or the
  local time zone.

## Utility functions

```c++
std::chrono::system_clock::time_point
    make_date(int year, int month, int day,
        int hour = 0, int min = 0, double sec = 0,
        DT flags = DT::maybe_dst | DT::utc);
```

Calculates a time point from a date and time. The `flags` argument must be a
bitwise combination of optional `DST` and `TimeZone` values (defaulting to
`DT::maybe_dst` and `DT::utc` respectively). The DST flag is ignored if
the zone is UTC. This will throw `std::invalid_argument` if the flags are
invalid.

The `year` argument is the full year. Normally, `month` is expected to be
1-12, `day` to be 1-31 (or the length of the month), `hour` to be 0-23, and
`min` and `sec` to be 0-59. Arguments out of their normal range will be
handled using the `std::mktime()` rules.

## Date formatting

```c++
class DateFormatter {
    DateFormatter();
    explicit DateFormatter(const std::string& format,
        DT flags = DT::utc);
    DateFormatter(const DateFormatter& df);
    DateFormatter(DateFormatter&& df) noexcept;
    ~DateFormatter() noexcept;
    DateFormatter& operator=(const DateFormatter& df);
    DateFormatter& operator=(DateFormatter&& df) noexcept;
    std::string operator()(std::chrono::system_clock::time_point tp,
        DT flags = 0) const;
};
```

Formatter for dates. This converts a time point to a broken down date. The
format string may contain any of the following codes:

| Code           | Description                           |
| ----           | -----------                           |
| `yyyy`         | Full year                             |
| `yy`           | Last 2 digits of the year             |
| `m`            | 1-2 digit month number (1-12)         |
| `mm`           | 2 digit month number (01-12)          |
| `mmm,Mmm,MMM`  | Month abbreviation (not localised)    |
| `d`            | 1-2 digit day of month (1-31)         |
| `dd`           | 2 digit day of month (01-31)          |
| `www,Www,WWW`  | Weekday abbreviation (not localised)  |
| `H`            | 1-2 digit hour number (0-23)          |
| `HH`           | 2 digit hour number (00-23)           |
| `MM`           | 2 digit minute number (00-59)         |
| `SS`           | 2 digit second number (00-59)         |
| `sss...`       | Fraction of a second                  |
| `ZZZZ`         | Time zone offset                      |

The constructor will throw `std::invalid_argument` if the format string is
invalid(it contains an unrecognised alphabetic code), or the flags are
invalid.

```c++
std::string format_date(std::chrono::system_clock::time_point tp,
    const std::string& format, DT flags = DT::utc);
```

This simply calls `DateFormatter(format, flags)(tp)`.

```c++
std::string iso_date(std::chrono::system_clock::time_point tp, int prec = 0,
    DT flags = DT::utc);
```

Formats a date and time in ISO 8601 format (e.g. `"2021-02-03 04:05:06.789"`).
If `prec` is positive, it indicates how many decimal places of seconds to
display. This will throw `std::invalid_argument` if the flags are invalid.

```c++
inline std::string format_time_point(std::chrono::system_clock::time_point tp,
    const FormatSpec& spec);
```

Formats a date and time according to the [Formatting](format.html) rules:

* Mode:
    * `d` = Day, month, year, time (e.g. `"3 Feb 2021 04:05:06"`)
    * `D` = Weekday, day, month, year, time (e.g. `"Wed 3 Feb 2021 04:05:06"`)
    * `i` = ISO 8601 with space delimiter (e.g. `"2021-02-03 04:05:06"`) (default)
    * `I` = ISO 8601 with T delimiter (e.g. `"2021-02-03T04:05:06"`)
* Options:
    * `d` = Show only the date
    * `l` = Use the local time zone (default is UTC)
    * `t` = Show only the time of day
* Precision:
    * If a precision is supplied, seconds are shown to this many decimal places

## Time formatting

```c++
template <typename R, typename P>
    std::string format_time(std::chrono::duration<R, P> time,
        FormatSpec spec);
```

Formats a time interval according to the [Formatting](format.html) rules:

* Mode:
    * `s` = Format as seconds (the common numerical options also apply here)
    * `t` = Expanded time format in days, hours, minutes, and seconds (default), e.g. `"12h34m56s"`
* Precision:
    * If a precision is supplied, the value is shown to this many decimal places

## Date parsing

```c++
std::chrono::system_clock::time_point parse_date(const std::string& str,
    DT flags = DT::ymd_order | DT::maybe_dst | DT::utc);
```

Parses a date consisting of 3-6 numbers. The `flags` argument indicates which
order to expect the year, month, and day to be in, whether DST is in effect,
and whether to interpret the date as UTC or local time. This will throw
`std::invalid_argument` if the date or flags are invalid.

## Time parsing

```c++
template <typename R, typename P>
    void parse_time(const std::string& str, std::chrono::duration<R, P>& t);
template <typename D> D parse_time(const std::string& str);
```

Parses a time in the same representation generated by `format_time()`. This
will throw `std::invalid_argument` if the time is invalid.

## Time conversion functions

```c++
template <typename C1, typename D1, typename C2, typename D2>
    void convert_time_point(time_point<C1, D1> src, time_point<C2, D2>& dst);
```

Converts a time point from one representation to another, possibly on a
different clock. This will call `time_point_cast()` if possible; otherwise,
it will check the current times on both clocks and use that to convert from
one clock to the other.

```c++
// Unix
void timepoint_to_timespec(const system_clock::time_point& tp,
    timespec& ts) noexcept;
void timepoint_to_timeval(const system_clock::time_point& tp,
    timeval& tv) noexcept;
void timespec_to_timepoint(const timespec& ts,
    system_clock::time_point& tp) noexcept;
void timeval_to_timepoint(const timeval& tv,
    system_clock::time_point& tp) noexcept;
template <typename R, typename P>
    void duration_to_timespec(const duration<R, P>& d, timespec& ts) noexcept;
template <typename R, typename P>
    void duration_to_timeval(const duration<R, P>& d, timeval& tv) noexcept;
template <typename R, typename P>
    void timespec_to_duration(const timespec& ts, duration<R, P>& d) noexcept;
template <typename R, typename P>
    void timeval_to_duration(const timeval& tv, duration<R, P>& d) noexcept;

// Windows
void filetime_to_timepoint(const FILETIME& ft,
    system_clock::time_point& tp) noexcept;
void timepoint_to_filetime(const system_clock::time_point& tp,
    FILETIME& ft) noexcept;
```

Conversion functions between C++ chrono types and system API types. These
return their result through a reference argument to avoid having to include
system headers in this header. Behaviour is undefined if the value being
represented is out of range for either the source or destination type.

The Windows functions are only defined on Windows builds; the Unix functions
are always defined, since the relevant time structures are also defined in
the Windows API.

For reference, the system types are:

```c++
// Unix
#include <time.h>
    struct timespec {
        time_t tv_sec;  // seconds
        long tv_nsec;   // nanoseconds
    };
#include <sys/time.h>
    struct timeval {
        time_t tv_sec;        // seconds
        suseconds_t tv_usec;  // microseconds
    };

// Windows
#include <windows.h>
    struct FILETIME {
        DWORD dwLowDateTime;   // low 32 bits
        DWORD dwHighDateTime;  // high 32 bits
    };
```

## Timed wait base class

```c++
class Waiter {
public:
    using Waiter::clock = std::chrono::system_clock;
    using Waiter::duration = clock::duration;
    using Waiter::time_point = clock::time_point;
    virtual Waiter::~Waiter() noexcept = 0;
    virtual bool Waiter::poll();
    virtual void Waiter::wait();
    virtual bool Waiter::wait_until(time_point t);
    template <typename R, typename P>
        bool Waiter::wait_for(std::chrono::duration<R, P> t);
protected:
    virtual bool Waiter::do_wait_for(duration t);
};
```

A general purpose base class for waitable objects. The `poll()` function
returns true if whatever condition was being waited on is ready, false if it
is not. The `wait_for()` and `wait_until()` functions return true on success,
false on timeout. The `wait()` function will wait indefinitely.

A derived class must implement at least one of `do_wait_for()` or
`wait_until()`, whichever is more convenient for the particular class; the
default implementations call each other. The default implementations of
`poll()` and `wait()` call `do_wait_for()`; inheritors may optionally also
implement either or both of `poll()` or `wait()`, if a more efficient
implementation is possible.
