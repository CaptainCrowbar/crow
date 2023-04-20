#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_statistics_univariate() {

    Statistics<double> stats;

    for (int x = 0; x < 100; ++x) {
        auto y = x * x * x - x * x;
        TRY(stats(y));
    }

    TEST_EQUAL(stats.n(), 100);
    TEST_EQUAL(stats.min(), 0);
    TEST_EQUAL(stats.max(), 960'498);
    TEST_EQUAL(stats.mean(), 241'741.5);
    TEST_NEAR(stats.sd(), 277'531.552'582, 1e-6);
    TEST_NEAR(stats.pop_sd(), 276'140.408'219, 1e-6);
    TEST_NEAR(stats.skewness(), 1.068'426, 1e-6);
    TEST_NEAR(stats.kurtosis(), -0.071'029, 1e-6);

}

void test_crow_statistics_bivariate() {

    Statistics<double> stats;

    for (int x = 0; x < 100; ++x) {
        auto y = x * x * x - x * x;
        TRY(stats(x, y));
    }

    TEST_EQUAL(stats.n(), 100);

    TEST_EQUAL(stats.x_min(), 0);
    TEST_EQUAL(stats.x_max(), 99);
    TEST_EQUAL(stats.x_mean(), 49.5);
    TEST_NEAR(stats.x_sd(), 29.011'492, 1e-6);
    TEST_NEAR(stats.x_pop_sd(), 28.866'070, 1e-6);
    TEST_EQUAL(stats.x_skewness(), 0);
    TEST_NEAR(stats.x_kurtosis(), -1.200'240, 1e-6);

    TEST_EQUAL(stats.y_min(), 0);
    TEST_EQUAL(stats.y_max(), 960'498);
    TEST_EQUAL(stats.y_mean(), 241'741.5);
    TEST_NEAR(stats.y_sd(), 277'531.552'582, 1e-6);
    TEST_NEAR(stats.y_pop_sd(), 276'140.408'219, 1e-6);
    TEST_NEAR(stats.y_skewness(), 1.068'426, 1e-6);
    TEST_NEAR(stats.y_kurtosis(), -0.071'029, 1e-6);

    TEST_NEAR(stats.r(), 0.914'819, 1e-6);
    TEST_NEAR(stats.a(), 8'751.4, 1e-6);
    TEST_NEAR(stats.b(), -191'452.8, 1e-6);
    TEST_NEAR(stats.inv_a(), 9.562'973'0e-5, 1e-12);
    TEST_NEAR(stats.inv_b(), 26.382'326, 1e-6);

}

void test_crow_statistics_combination() {

    Statistics<double> stats1, stats2, stats3;

    for (int x = 0; x < 50; ++x) {
        auto y = x * x * x - x * x;
        TRY(stats1(x, y));
    }

    for (int x = 50; x < 100; ++x) {
        auto y = x * x * x - x * x;
        TRY(stats2(x, y));
    }

    TRY(stats3 = stats1 + stats2);

    TEST_EQUAL(stats3.n(), 100);

    TEST_EQUAL(stats3.x_min(), 0);
    TEST_EQUAL(stats3.x_max(), 99);
    TEST_EQUAL(stats3.x_mean(), 49.5);
    TEST_NEAR(stats3.x_sd(), 29.011'492, 1e-6);
    TEST_NEAR(stats3.x_pop_sd(), 28.866'070, 1e-6);
    TEST_EQUAL(stats3.x_skewness(), 0);
    TEST_NEAR(stats3.x_kurtosis(), -1.200'240, 1e-6);

    TEST_EQUAL(stats3.y_min(), 0);
    TEST_EQUAL(stats3.y_max(), 960'498);
    TEST_EQUAL(stats3.y_mean(), 241'741.5);
    TEST_NEAR(stats3.y_sd(), 277'531.552'582, 1e-6);
    TEST_NEAR(stats3.y_pop_sd(), 276'140.408'219, 1e-6);
    TEST_NEAR(stats3.y_skewness(), 1.068'426, 1e-6);
    TEST_NEAR(stats3.y_kurtosis(), -0.071'029, 1e-6);

    TEST_NEAR(stats3.r(), 0.914'819, 1e-6);
    TEST_NEAR(stats3.a(), 8'751.4, 1e-6);
    TEST_NEAR(stats3.b(), -191'452.8, 1e-6);
    TEST_NEAR(stats3.inv_a(), 9.562'973'0e-5, 1e-12);
    TEST_NEAR(stats3.inv_b(), 26.382'326, 1e-6);

}
