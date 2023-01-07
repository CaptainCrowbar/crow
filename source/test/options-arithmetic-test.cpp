#include "crow/options.hpp"
#include "crow/random.hpp"
#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"
#include <sstream>

using namespace Crow;

void test_crow_options_arithmetic_random() {

    int16_t i = 0;
    uint16_t u = 0;
    double r = 0;

    Options opt1("Hello", "", "Goes ding when there's stuff");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(i, "integer", 'i', "Integer option", Options::random));
    TRY(opt1.add(u, "unsigned", 'u', "Unsigned option", Options::random));
    TRY(opt1.add(r, "real", 'r', "Real option", Options::random));

    {
        Options opt2 = opt1;
        TRY(opt2.auto_help());
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello\n"
            "\n"
            "Goes ding when there's stuff\n"
            "\n"
            "Options:\n"
            "    --integer, -i <int>    = Integer option\n"
            "    --unsigned, -u <uint>  = Unsigned option\n"
            "    --real, -r <real>      = Real option\n"
            "    --help, -h             = Show usage information\n"
            "    --version, -v          = Show version information\n"
            "\n"
        );
    }

    {
        i = 0;
        u = 0;
        r = 0;
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-i", "86",
            "-u", "99",
            "-r", "42.5",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("integer"));
        TEST(opt2.found("unsigned"));
        TEST(opt2.found("real"));
        TEST_EQUAL(i, 86);
        TEST_EQUAL(u, 99u);
        TEST_EQUAL(r, 42.5);
    }

    Statistics<double> istats;
    Statistics<double> ustats;
    Statistics<double> rstats;

    for (int j = 0; j < 10'000; ++j) {
        i = 0;
        u = 0;
        r = 0;
        Options opt2 = opt1;
        std::ostringstream out;
        opt2.parse({}, out);
        istats(double(i));
        ustats(double(u));
        rstats(r);
    }

    static const UniformReal<> id(0, 32767);
    static const UniformReal<> ud(0, 65535);
    static const UniformReal<> rd;

    TEST(istats.min() >= id.min());
    TEST(istats.max() <= id.max());
    TEST_NEAR(istats.mean(), id.mean(), 500);
    TEST_NEAR(istats.sd(), id.sd(), 500);

    TEST(ustats.min() >= ud.min());
    TEST(ustats.max() <= ud.max());
    TEST_NEAR(ustats.mean(), ud.mean(), 1000);
    TEST_NEAR(ustats.sd(), ud.sd(), 1000);

    TEST(rstats.min() >= rd.min());
    TEST(rstats.max() <= rd.max());
    TEST_NEAR(rstats.mean(), rd.mean(), 0.01);
    TEST_NEAR(rstats.sd(), rd.sd(), 0.01);

}

void test_crow_options_arithmetic_random_nondefault() {

    int16_t i = 99;
    uint16_t u = 999;
    double r = 10;

    Options opt1("Hello", "", "Goes ding when there's stuff");
    TRY(opt1.set_colour(false));
    TRY(opt1.add(i, "integer", 'i', "Integer option", Options::random));
    TRY(opt1.add(u, "unsigned", 'u', "Unsigned option", Options::random));
    TRY(opt1.add(r, "real", 'r', "Real option", Options::random));

    {
        Options opt2 = opt1;
        TRY(opt2.auto_help());
        std::ostringstream out;
        TEST(! opt2.parse({}, out));
        TEST_EQUAL(out.str(),
            "\n"
            "Hello\n"
            "\n"
            "Goes ding when there's stuff\n"
            "\n"
            "Options:\n"
            "    --integer, -i <int>    = Integer option\n"
            "    --unsigned, -u <uint>  = Unsigned option\n"
            "    --real, -r <real>      = Real option\n"
            "    --help, -h             = Show usage information\n"
            "    --version, -v          = Show version information\n"
            "\n"
        );
    }

    {
        i = 0;
        u = 0;
        r = 0;
        Options opt2 = opt1;
        std::ostringstream out;
        TEST(opt2.parse({
            "-i", "86",
            "-u", "99",
            "-r", "42.5",
        }, out));
        TEST_EQUAL(out.str(), "");
        TEST(opt2.found("integer"));
        TEST(opt2.found("unsigned"));
        TEST(opt2.found("real"));
        TEST_EQUAL(i, 86);
        TEST_EQUAL(u, 99u);
        TEST_EQUAL(r, 42.5);
    }

    Statistics<double> istats;
    Statistics<double> ustats;
    Statistics<double> rstats;

    for (int j = 0; j < 10'000; ++j) {
        i = 0;
        u = 0;
        r = 0;
        Options opt2 = opt1;
        std::ostringstream out;
        opt2.parse({}, out);
        istats(double(i));
        ustats(double(u));
        rstats(r);
    }

    static const UniformReal<> id(0, 99);
    static const UniformReal<> ud(0, 999);
    static const UniformReal<> rd(0, 10);

    TEST(istats.min() >= id.min());
    TEST(istats.max() <= id.max());
    TEST_NEAR(istats.mean(), id.mean(), 2);
    TEST_NEAR(istats.sd(), id.sd(), 2);

    TEST(ustats.min() >= ud.min());
    TEST(ustats.max() <= ud.max());
    TEST_NEAR(ustats.mean(), ud.mean(), 20);
    TEST_NEAR(ustats.sd(), ud.sd(), 20);

    TEST(rstats.min() >= rd.min());
    TEST(rstats.max() <= rd.max());
    TEST_NEAR(rstats.mean(), rd.mean(), 0.2);
    TEST_NEAR(rstats.sd(), rd.sd(), 0.2);

}
