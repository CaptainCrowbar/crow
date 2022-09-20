#include "crow/time.hpp"
#include "crow/unit-test.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <ratio>
#include <stdexcept>
#include <string>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef _XOPEN_SOURCE
    #include <sys/time.h>
#endif

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

using Dseconds = duration<double>;

void test_crow_time_make_date() {

    system_clock::time_point tp;
    int64_t n1 = 0, n2 = 0;

    TRY(tp = make_date(1970, 1, 1, 0, 0, 0));
    TRY(n1 = int64_t(system_clock::to_time_t(tp)));
    TEST_EQUAL(n1, 0);
    TRY(tp = make_date(2000, 1, 2, 3, 4, 5));
    TRY(n1 = int64_t(system_clock::to_time_t(tp)));
    TEST_EQUAL(n1, 946'782'245);
    TRY(tp = make_date(2000, 1, 2, 3, 4, 5, DT::local));
    TRY(n2 = int64_t(system_clock::to_time_t(tp)));
    TEST(std::abs(n2 - n1) <= 86400);

}

void test_crow_time_formatting() {

    std::string str;

    TRY(str = format_time(Dseconds(0)));              TEST_EQUAL(str, "0s");
    TRY(str = format_time(Dseconds(0), 3));           TEST_EQUAL(str, "0.000s");
    TRY(str = format_time(Dseconds(0.25), 3));        TEST_EQUAL(str, "0.250s");
    TRY(str = format_time(Dseconds(0.5), 3));         TEST_EQUAL(str, "0.500s");
    TRY(str = format_time(Dseconds(0.75), 3));        TEST_EQUAL(str, "0.750s");
    TRY(str = format_time(Dseconds(1), 3));           TEST_EQUAL(str, "1.000s");
    TRY(str = format_time(Dseconds(1.25), 3));        TEST_EQUAL(str, "1.250s");
    TRY(str = format_time(Dseconds(59.999), 3));      TEST_EQUAL(str, "59.999s");
    TRY(str = format_time(Dseconds(60), 3));          TEST_EQUAL(str, "1m00.000s");
    TRY(str = format_time(Dseconds(1234), 3));        TEST_EQUAL(str, "20m34.000s");
    TRY(str = format_time(Dseconds(12345), 3));       TEST_EQUAL(str, "3h25m45.000s");
    TRY(str = format_time(Dseconds(123456), 3));      TEST_EQUAL(str, "1d10h17m36.000s");
    TRY(str = format_time(Dseconds(1234567), 3));     TEST_EQUAL(str, "14d06h56m07.000s");
    TRY(str = format_time(Dseconds(12345678), 3));    TEST_EQUAL(str, "142d21h21m18.000s");
    TRY(str = format_time(Dseconds(123456789), 3));   TEST_EQUAL(str, "1428d21h33m09.000s");
    TRY(str = format_time(Dseconds(-0.25), 3));       TEST_EQUAL(str, "-0.250s");
    TRY(str = format_time(Dseconds(-0.5), 3));        TEST_EQUAL(str, "-0.500s");
    TRY(str = format_time(Dseconds(-0.75), 3));       TEST_EQUAL(str, "-0.750s");
    TRY(str = format_time(Dseconds(-1), 3));          TEST_EQUAL(str, "-1.000s");
    TRY(str = format_time(Dseconds(-1.25), 3));       TEST_EQUAL(str, "-1.250s");
    TRY(str = format_time(Dseconds(-59.999), 3));     TEST_EQUAL(str, "-59.999s");
    TRY(str = format_time(Dseconds(-60), 3));         TEST_EQUAL(str, "-1m00.000s");
    TRY(str = format_time(Dseconds(-1234), 3));       TEST_EQUAL(str, "-20m34.000s");
    TRY(str = format_time(Dseconds(-12345), 3));      TEST_EQUAL(str, "-3h25m45.000s");
    TRY(str = format_time(Dseconds(-123456), 3));     TEST_EQUAL(str, "-1d10h17m36.000s");
    TRY(str = format_time(Dseconds(-1234567), 3));    TEST_EQUAL(str, "-14d06h56m07.000s");
    TRY(str = format_time(Dseconds(-12345678), 3));   TEST_EQUAL(str, "-142d21h21m18.000s");
    TRY(str = format_time(Dseconds(-123456789), 3));  TEST_EQUAL(str, "-1428d21h33m09.000s");
    TRY(str = format_time(nanoseconds(1), 10));       TEST_EQUAL(str, "0.0000000010s");
    TRY(str = format_time(microseconds(1), 7));       TEST_EQUAL(str, "0.0000010s");
    TRY(str = format_time(milliseconds(1), 4));       TEST_EQUAL(str, "0.0010s");
    TRY(str = format_time(seconds(1)));               TEST_EQUAL(str, "1s");
    TRY(str = format_time(minutes(1)));               TEST_EQUAL(str, "1m00s");
    TRY(str = format_time(hours(1)));                 TEST_EQUAL(str, "1h00m00s");
    TRY(str = format_time(nanoseconds(-1), 10));      TEST_EQUAL(str, "-0.0000000010s");
    TRY(str = format_time(microseconds(-1), 7));      TEST_EQUAL(str, "-0.0000010s");
    TRY(str = format_time(milliseconds(-1), 4));      TEST_EQUAL(str, "-0.0010s");
    TRY(str = format_time(seconds(-1)));              TEST_EQUAL(str, "-1s");
    TRY(str = format_time(minutes(-1)));              TEST_EQUAL(str, "-1m00s");
    TRY(str = format_time(hours(-1)));                TEST_EQUAL(str, "-1h00m00s");

}

void test_crow_time_date_formatting() {

    system_clock::duration d;
    system_clock::time_point tp;
    std::string s;

    TRY(tp = make_date(2000, 1, 2, 3, 4, 5));
    TRY(s = iso_date(tp));
    TEST_EQUAL(s, "2000-01-02 03:04:05");
    TRY(s = iso_date(tp, 3));
    TEST_EQUAL(s, "2000-01-02 03:04:05.000");
    d = duration_cast<system_clock::duration>(Dseconds(0.12345));
    TRY(tp += d);
    TRY(s = iso_date(tp));
    TEST_EQUAL(s, "2000-01-02 03:04:05");
    TRY(s = iso_date(tp, 3));
    TEST_EQUAL(s, "2000-01-02 03:04:05.123");
    d = duration_cast<system_clock::duration>(Dseconds(0.44444));
    TRY(tp += d);
    TRY(s = iso_date(tp));
    TEST_EQUAL(s, "2000-01-02 03:04:05");
    TRY(s = iso_date(tp, 3));
    TEST_EQUAL(s, "2000-01-02 03:04:05.568");
    TRY(tp = make_date(2000, 1, 2, 3, 4, 5, DT::local));
    TRY(s = iso_date(tp, 0, DT::local));
    TEST_EQUAL(s, "2000-01-02 03:04:05");

    TRY(tp = make_date(2021, 2, 3, 4, 5, 6.7899));

    TRY(s = format_date(tp, "d/m/yyyy"));                  TEST_EQUAL(s, "3/2/2021");
    TRY(s = format_date(tp, "yyyy-mm-dd HH:MM:SS"));       TEST_EQUAL(s, "2021-02-03 04:05:06");
    TRY(s = format_date(tp, "www d mmm yy"));              TEST_EQUAL(s, "wed 3 feb 21");
    TRY(s = format_date(tp, "Www d Mmm yy"));              TEST_EQUAL(s, "Wed 3 Feb 21");
    TRY(s = format_date(tp, "WWW d MMM yy"));              TEST_EQUAL(s, "WED 3 FEB 21");
    TRY(s = format_date(tp, "H:MM:SS"));                   TEST_EQUAL(s, "4:05:06");
    TRY(s = format_date(tp, "H:MM:SS.s"));                 TEST_EQUAL(s, "4:05:06.8");
    TRY(s = format_date(tp, "H:MM:SS.ss"));                TEST_EQUAL(s, "4:05:06.79");
    TRY(s = format_date(tp, "H:MM:SS.sss"));               TEST_EQUAL(s, "4:05:06.790");
    TRY(s = format_date(tp, "H:MM:SS.ssss"));              TEST_EQUAL(s, "4:05:06.7899");
    TRY(s = format_date(tp, "H:MM:SS.sssss"));             TEST_EQUAL(s, "4:05:06.78990");
    TRY(s = format_date(tp, "H:MM:SS.ssssss"));            TEST_EQUAL(s, "4:05:06.789900");
    TRY(s = format_date(tp, "yyyy-mm-dd HH:MM:SS ZZZZ"));  TEST_EQUAL(s, "2021-02-03 04:05:06 +0000");

    TRY(tp = make_date(2021, 2, 3, 4, 5, 6.7899, DT::local));

    TRY(s = format_date(tp, "yyyy-mm-dd HH:MM:SS.sss", DT::local));   TEST_EQUAL(s, "2021-02-03 04:05:06.790");
    TRY(s = format_date(tp, "yyyy-mm-dd HH:MM:SS ZZZZ", DT::local));  TEST_MATCH(s, "^2021-02-03 04:05:06 [+-]\\d{4}$");

}

void test_crow_time_parsing() {

    Dseconds dsec = {};
    seconds sec = {};
    microseconds usec = {};

    TRY(dsec = parse_time<Dseconds>("0s"));                    TEST_EQUAL(dsec.count(), 0);
    TRY(dsec = parse_time<Dseconds>("42s"));                   TEST_EQUAL(dsec.count(), 42);
    TRY(dsec = parse_time<Dseconds>("4200s"));                 TEST_EQUAL(dsec.count(), 4200);
    TRY(dsec = parse_time<Dseconds>("1y234d5h6m7.89s"));       TEST_NEAR(dsec.count(), 51'793'567.89, 1e-6);
    TRY(dsec = parse_time<Dseconds>("-42s"));                  TEST_EQUAL(dsec.count(), -42);
    TRY(dsec = parse_time<Dseconds>("-4200s"));                TEST_EQUAL(dsec.count(), -4200);
    TRY(dsec = parse_time<Dseconds>("-1y234d5h6m7.89s"));      TEST_NEAR(dsec.count(), -51'793'567.89, 1e-6);
    TRY(sec = parse_time<seconds>("0s"));                      TEST_EQUAL(sec.count(), 0);
    TRY(sec = parse_time<seconds>("42s"));                     TEST_EQUAL(sec.count(), 42);
    TRY(sec = parse_time<seconds>("4200s"));                   TEST_EQUAL(sec.count(), 4200);
    TRY(sec = parse_time<seconds>("1y234d5h6m7.89s"));         TEST_EQUAL(sec.count(), 51'793'567);
    TRY(sec = parse_time<seconds>("-42s"));                    TEST_EQUAL(sec.count(), -42);
    TRY(sec = parse_time<seconds>("-4200s"));                  TEST_EQUAL(sec.count(), -4200);
    TRY(sec = parse_time<seconds>("-1y234d5h6m7.89s"));        TEST_EQUAL(sec.count(), -51'793'567);
    TRY(usec = parse_time<microseconds>("0s"));                TEST_EQUAL(usec.count(), 0);
    TRY(usec = parse_time<microseconds>("42s"));               TEST_EQUAL(usec.count(), 42'000'000);
    TRY(usec = parse_time<microseconds>("4200s"));             TEST_EQUAL(usec.count(), 4'200'000'000ll);
    TRY(usec = parse_time<microseconds>("1y234d5h6m7.89s"));   TEST_EQUAL(usec.count(), 51'793'567'890'000ll);
    TRY(usec = parse_time<microseconds>("-42s"));              TEST_EQUAL(usec.count(), -42'000'000);
    TRY(usec = parse_time<microseconds>("-4200s"));            TEST_EQUAL(usec.count(), -4'200'000'000ll);
    TRY(usec = parse_time<microseconds>("-1y234d5h6m7.89s"));  TEST_EQUAL(usec.count(), -51'793'567'890'000ll);
    TRY(usec = parse_time<microseconds>("42ms"));              TEST_EQUAL(usec.count(), 42'000);
    TRY(usec = parse_time<microseconds>("42us"));              TEST_EQUAL(usec.count(), 42);
    TRY(usec = parse_time<microseconds>("42ns"));              TEST_EQUAL(usec.count(), 0);
    TRY(sec = parse_time<seconds>("1y"));                      TEST_EQUAL(sec.count(), 31'557'600);
    TRY(sec = parse_time<seconds>("1ky"));                     TEST_EQUAL(sec.count(), 31'557'600'000ll);
    TRY(sec = parse_time<seconds>("1My"));                     TEST_EQUAL(sec.count(), 31'557'600'000'000ll);
    TRY(sec = parse_time<seconds>("1Gy"));                     TEST_EQUAL(sec.count(), 31'557'600'000'000'000ll);

    TEST_THROW(parse_time<seconds>(""), std::invalid_argument);
    TEST_THROW(parse_time<seconds>("12345"), std::invalid_argument);
    TEST_THROW(parse_time<seconds>("sec"), std::invalid_argument);

}

void test_crow_time_date_parsing() {

    system_clock::time_point date = {};
    std::string str;

    TRY(date = parse_date("2017-11-04"));                         TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 00:00:00.000");
    TRY(date = parse_date("2017 Nov 4"));                         TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 00:00:00.000");
    TRY(date = parse_date("2017-11-04 12"));                      TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:00:00.000");
    TRY(date = parse_date("2017-11-04 12:34"));                   TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:00.000");
    TRY(date = parse_date("2017-11-04 12:34:56"));                TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:56.000");
    TRY(date = parse_date("2017-11-04 12:34:56.789"));            TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:56.789");
    TRY(date = parse_date("2017-11-04T12:34:56"));                TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:56.000");
    TRY(date = parse_date("4 Nov 2017", DT::dmy_order));          TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 00:00:00.000");
    TRY(date = parse_date("Nov 4 2017", DT::mdy_order));          TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 00:00:00.000");
    TRY(date = parse_date("4/11/2017/12/34/56", DT::dmy_order));  TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:56.000");
    TRY(date = parse_date("11/4/2017/12/34/56", DT::mdy_order));  TRY(str = iso_date(date, 3));  TEST_EQUAL(str, "2017-11-04 12:34:56.000");

    TEST_THROW(parse_date(""), std::invalid_argument);
    TEST_THROW(parse_date("Nov 4, 2017"), std::invalid_argument);
    TEST_THROW(parse_date("Nov 2017", DT::mdy_order), std::invalid_argument);

}

void test_crow_time_duration_formatting() {

    TEST_EQUAL(format_duration(0s,          {}),     "0s");
    TEST_EQUAL(format_duration(250ms,       {}),     "0s");
    TEST_EQUAL(format_duration(1234s,       {}),     "20m34s");
    TEST_EQUAL(format_duration(12345678s,   {}),     "142d21h21m18s");
    TEST_EQUAL(format_duration(-12345678s,  {}),     "-142d21h21m18s");
    TEST_EQUAL(format_duration(0s,          "t3"),   "0.000s");
    TEST_EQUAL(format_duration(250ms,       "t3"),   "0.250s");
    TEST_EQUAL(format_duration(1234s,       "t3"),   "20m34.000s");
    TEST_EQUAL(format_duration(12345678s,   "t3"),   "142d21h21m18.000s");
    TEST_EQUAL(format_duration(-12345678s,  "t3"),   "-142d21h21m18.000s");
    TEST_EQUAL(format_duration(0s,          "s"),    "0");
    TEST_EQUAL(format_duration(250ms,       "s"),    "0");
    TEST_EQUAL(format_duration(1234s,       "s"),    "1234");
    TEST_EQUAL(format_duration(12345678s,   "s"),    "12345678");
    TEST_EQUAL(format_duration(-12345678s,  "s"),    "-12345678");
    TEST_EQUAL(format_duration(0s,          "s3"),   "0.000");
    TEST_EQUAL(format_duration(250ms,       "s3"),   "0.250");
    TEST_EQUAL(format_duration(1234s,       "s3"),   "1234.000");
    TEST_EQUAL(format_duration(12345678s,   "s3"),   "12345678.000");
    TEST_EQUAL(format_duration(-12345678s,  "s3"),   "-12345678.000");
    TEST_EQUAL(format_duration(0s,          "sa3"),  "0.000");
    TEST_EQUAL(format_duration(250ms,       "sa3"),  "0.250");
    TEST_EQUAL(format_duration(1234s,       "sa3"),  "1'234.000");
    TEST_EQUAL(format_duration(12345678s,   "sa3"),  "12'345'678.000");
    TEST_EQUAL(format_duration(-12345678s,  "sa3"),  "-12'345'678.000");

}

void test_crow_time_point_formatting() {

    system_clock::time_point tp;

    TRY(tp = make_date(2021, 2, 3, 4, 5, 6.789));

    TEST_EQUAL(format_time_point(tp, {}),     "2021-02-03 04:05:06");
    TEST_EQUAL(format_time_point(tp, "i"),    "2021-02-03 04:05:06");
    TEST_EQUAL(format_time_point(tp, "I"),    "2021-02-03T04:05:06");
    TEST_EQUAL(format_time_point(tp, "i3"),   "2021-02-03 04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "I3"),   "2021-02-03T04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "id"),   "2021-02-03");
    TEST_EQUAL(format_time_point(tp, "Id"),   "2021-02-03");
    TEST_EQUAL(format_time_point(tp, "id3"),  "2021-02-03");
    TEST_EQUAL(format_time_point(tp, "Id3"),  "2021-02-03");
    TEST_EQUAL(format_time_point(tp, "it"),   "04:05:06");
    TEST_EQUAL(format_time_point(tp, "It"),   "04:05:06");
    TEST_EQUAL(format_time_point(tp, "it3"),  "04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "It3"),  "04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "d"),    "3 Feb 2021 04:05:06");
    TEST_EQUAL(format_time_point(tp, "D"),    "Wed 3 Feb 2021 04:05:06");
    TEST_EQUAL(format_time_point(tp, "d3"),   "3 Feb 2021 04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "D3"),   "Wed 3 Feb 2021 04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "dd"),   "3 Feb 2021");
    TEST_EQUAL(format_time_point(tp, "Dd"),   "Wed 3 Feb 2021");
    TEST_EQUAL(format_time_point(tp, "dd3"),  "3 Feb 2021");
    TEST_EQUAL(format_time_point(tp, "Dd3"),  "Wed 3 Feb 2021");
    TEST_EQUAL(format_time_point(tp, "dt"),   "04:05:06");
    TEST_EQUAL(format_time_point(tp, "Dt"),   "04:05:06");
    TEST_EQUAL(format_time_point(tp, "dt3"),  "04:05:06.789");
    TEST_EQUAL(format_time_point(tp, "Dt3"),  "04:05:06.789");

}

void test_crow_time_point_conversion() {

    using Htime = high_resolution_clock::time_point;
    using Stime = system_clock::time_point;

    Htime h1, h2, h3;
    Stime s1, s2, s3;
    milliseconds ms;

    h1 = high_resolution_clock::now();
    h2 = h1 + 1min;
    s1 = system_clock::now();
    s2 = s1 + 1min;

    TRY(convert_time_point(h2, h3));  ms = duration_cast<milliseconds>(h3 - h1);  TEST_EQUAL(ms.count(), 60'000);
    TRY(convert_time_point(s2, s3));  ms = duration_cast<milliseconds>(s3 - s1);  TEST_EQUAL(ms.count(), 60'000);
    TRY(convert_time_point(s2, h3));  ms = duration_cast<milliseconds>(h3 - h1);  TEST_NEAR(ms.count(), 60'000, 50);
    TRY(convert_time_point(h2, s3));  ms = duration_cast<milliseconds>(s3 - s1);  TEST_NEAR(ms.count(), 60'000, 50);

}

void test_crow_time_system_specific_conversions() {

    using Dsec = duration<double>;
    using Days = duration<int64_t, std::ratio<86400>>;

    milliseconds ms = {};
    seconds s = {};
    Days d = {};
    Dsec fs = {};
    timespec ts = {};
    timeval tv = {};

    ts = {0, 0};                 TRY(timespec_to_duration(ts, fs));  TEST_EQUAL(fs.count(), 0);
    ts = {0, 0};                 TRY(timespec_to_duration(ts, d));   TEST_EQUAL(d.count(), 0);
    ts = {0, 0};                 TRY(timespec_to_duration(ts, ms));  TEST_EQUAL(ms.count(), 0);
    ts = {0, 0};                 TRY(timespec_to_duration(ts, s));   TEST_EQUAL(s.count(), 0);
    ts = {0, 125'000'000};       TRY(timespec_to_duration(ts, fs));  TEST_EQUAL(fs.count(), 0.125);
    ts = {0, 125'000'000};       TRY(timespec_to_duration(ts, d));   TEST_EQUAL(d.count(), 0);
    ts = {0, 125'000'000};       TRY(timespec_to_duration(ts, ms));  TEST_EQUAL(ms.count(), 125);
    ts = {0, 125'000'000};       TRY(timespec_to_duration(ts, s));   TEST_EQUAL(s.count(), 0);
    ts = {86'400, 0};            TRY(timespec_to_duration(ts, fs));  TEST_EQUAL(fs.count(), 86'400);
    ts = {86'400, 0};            TRY(timespec_to_duration(ts, d));   TEST_EQUAL(d.count(), 1);
    ts = {86'400, 0};            TRY(timespec_to_duration(ts, ms));  TEST_EQUAL(ms.count(), 86'400'000);
    ts = {86'400, 0};            TRY(timespec_to_duration(ts, s));   TEST_EQUAL(s.count(), 86'400);
    ts = {86'400, 125'000'000};  TRY(timespec_to_duration(ts, fs));  TEST_EQUAL(fs.count(), 86'400.125);
    ts = {86'400, 125'000'000};  TRY(timespec_to_duration(ts, d));   TEST_EQUAL(d.count(), 1);
    ts = {86'400, 125'000'000};  TRY(timespec_to_duration(ts, ms));  TEST_EQUAL(ms.count(), 86'400'125);
    ts = {86'400, 125'000'000};  TRY(timespec_to_duration(ts, s));   TEST_EQUAL(s.count(), 86'400);
    tv = {0, 0};                 TRY(timeval_to_duration(tv, fs));   TEST_EQUAL(fs.count(), 0);
    tv = {0, 0};                 TRY(timeval_to_duration(tv, d));    TEST_EQUAL(d.count(), 0);
    tv = {0, 0};                 TRY(timeval_to_duration(tv, ms));   TEST_EQUAL(ms.count(), 0);
    tv = {0, 0};                 TRY(timeval_to_duration(tv, s));    TEST_EQUAL(s.count(), 0);
    tv = {0, 125'000};           TRY(timeval_to_duration(tv, fs));   TEST_EQUAL(fs.count(), 0.125);
    tv = {0, 125'000};           TRY(timeval_to_duration(tv, d));    TEST_EQUAL(d.count(), 0);
    tv = {0, 125'000};           TRY(timeval_to_duration(tv, ms));   TEST_EQUAL(ms.count(), 125);
    tv = {0, 125'000};           TRY(timeval_to_duration(tv, s));    TEST_EQUAL(s.count(), 0);
    tv = {86'400, 0};            TRY(timeval_to_duration(tv, fs));   TEST_EQUAL(fs.count(), 86'400);
    tv = {86'400, 0};            TRY(timeval_to_duration(tv, d));    TEST_EQUAL(d.count(), 1);
    tv = {86'400, 0};            TRY(timeval_to_duration(tv, ms));   TEST_EQUAL(ms.count(), 86'400'000);
    tv = {86'400, 0};            TRY(timeval_to_duration(tv, s));    TEST_EQUAL(s.count(), 86'400);
    tv = {86'400, 125'000};      TRY(timeval_to_duration(tv, fs));   TEST_EQUAL(fs.count(), 86'400.125);
    tv = {86'400, 125'000};      TRY(timeval_to_duration(tv, d));    TEST_EQUAL(d.count(), 1);
    tv = {86'400, 125'000};      TRY(timeval_to_duration(tv, ms));   TEST_EQUAL(ms.count(), 86'400'125);
    tv = {86'400, 125'000};      TRY(timeval_to_duration(tv, s));    TEST_EQUAL(s.count(), 86'400);

    fs = Dsec(0);                   TRY(duration_to_timespec(fs, ts));  TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 0);
    d = Days(0);                    TRY(duration_to_timespec(d, ts));   TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 0);
    ms = milliseconds(0);           TRY(duration_to_timespec(ms, ts));  TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 0);
    s = seconds(0);                 TRY(duration_to_timespec(s, ts));   TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 0);
    fs = Dsec(0.125);               TRY(duration_to_timespec(fs, ts));  TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 125'000'000);
    ms = milliseconds(125);         TRY(duration_to_timespec(ms, ts));  TEST_EQUAL(ts.tv_sec, 0);       TEST_EQUAL(ts.tv_nsec, 125'000'000);
    fs = Dsec(86'400);              TRY(duration_to_timespec(fs, ts));  TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 0);
    d = Days(1);                    TRY(duration_to_timespec(d, ts));   TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 0);
    ms = milliseconds(86'400'000);  TRY(duration_to_timespec(ms, ts));  TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 0);
    s = seconds(86'400);            TRY(duration_to_timespec(s, ts));   TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 0);
    fs = Dsec(86'400.125);          TRY(duration_to_timespec(fs, ts));  TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 125'000'000);
    ms = milliseconds(86'400'125);  TRY(duration_to_timespec(ms, ts));  TEST_EQUAL(ts.tv_sec, 86'400);  TEST_EQUAL(ts.tv_nsec, 125'000'000);
    fs = Dsec(0);                   TRY(duration_to_timeval(fs, tv));   TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 0);
    d = Days(0);                    TRY(duration_to_timeval(d, tv));    TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 0);
    ms = milliseconds(0);           TRY(duration_to_timeval(ms, tv));   TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 0);
    s = seconds(0);                 TRY(duration_to_timeval(s, tv));    TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 0);
    fs = Dsec(0.125);               TRY(duration_to_timeval(fs, tv));   TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 125'000);
    ms = milliseconds(125);         TRY(duration_to_timeval(ms, tv));   TEST_EQUAL(tv.tv_sec, 0);       TEST_EQUAL(tv.tv_usec, 125'000);
    fs = Dsec(86'400);              TRY(duration_to_timeval(fs, tv));   TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 0);
    d = Days(1);                    TRY(duration_to_timeval(d, tv));    TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 0);
    ms = milliseconds(86'400'000);  TRY(duration_to_timeval(ms, tv));   TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 0);
    s = seconds(86'400);            TRY(duration_to_timeval(s, tv));    TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 0);
    fs = Dsec(86'400.125);          TRY(duration_to_timeval(fs, tv));   TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 125'000);
    ms = milliseconds(86'400'125);  TRY(duration_to_timeval(ms, tv));   TEST_EQUAL(tv.tv_sec, 86'400);  TEST_EQUAL(tv.tv_usec, 125'000);

    #ifdef _WIN32

        static constexpr int64_t epoch = 11'644'473'600ll;
        static constexpr int64_t freq = 10'000'000ll;

        int64_t ticks = 0;
        FILETIME ft1, ft2;
        system_clock::time_point tp1, tp2;
        system_clock::duration du;

        ticks = epoch * freq;
        ft1 = {uint32_t(ticks), uint32_t(ticks >> 32)};
        TRY(filetime_to_timepoint(ft1, tp1));
        du = tp1 - system_clock::from_time_t(0);
        TEST_EQUAL(du.count(), 0);
        TRY(timepoint_to_filetime(tp1, ft2));
        TEST_EQUAL(ft2.dwHighDateTime, ft1.dwHighDateTime);
        TEST_EQUAL(ft2.dwLowDateTime, ft1.dwLowDateTime);

        ticks += 86'400 * freq;
        ft1 = {uint32_t(ticks), uint32_t(ticks >> 32)};
        TRY(filetime_to_timepoint(ft1, tp1));
        du = tp1 - system_clock::from_time_t(0);
        TEST_EQUAL(duration_cast<milliseconds>(du).count(), 86'400'000);

        tp1 = system_clock::from_time_t(0);
        TRY(timepoint_to_filetime(tp1, ft1));
        TRY(filetime_to_timepoint(ft1, tp2));
        TEST_EQUAL(tp2.time_since_epoch().count(), tp1.time_since_epoch().count());

        tp1 = system_clock::from_time_t(1'234'567'890);
        TRY(timepoint_to_filetime(tp1, ft1));
        TRY(filetime_to_timepoint(ft1, tp2));
        TEST_EQUAL(tp2.time_since_epoch().count(), tp1.time_since_epoch().count());

    #endif

}

namespace {

    class Thing:
    public Waiter {
    protected:
        bool do_wait_for(duration t) override {
            std::this_thread::sleep_for(t);
            return false;
        }
    };

}

void test_crow_time_waiter() {

    Thing t;

    TEST(! t.wait_for(50ms));

}
