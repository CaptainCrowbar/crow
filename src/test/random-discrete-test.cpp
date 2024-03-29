#include "crow/random-discrete-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <map>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_random_bernoulli_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    BernoulliDistribution dist;
    bool b = false;
    int n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    double p = double(n) / iterations;
    TEST_NEAR(p, 0.5, 0.001);

    TRY(dist = BernoulliDistribution(Rational(1, 10)));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.1, 0.001);

    TRY(dist = BernoulliDistribution(0.9));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.9, 0.001);

}

void test_crow_random_uniform_integer_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformInteger<int> dist1(4);
    std::map<int, int> census;
    int x = 0;

    TEST_EQUAL(dist1.pdf(-1), 0);    TEST_EQUAL(dist1.cdf(-1), 0);    TEST_EQUAL(dist1.ccdf(-1), 1);
    TEST_EQUAL(dist1.pdf(0), 0.25);  TEST_EQUAL(dist1.cdf(0), 0.25);  TEST_EQUAL(dist1.ccdf(0), 1);
    TEST_EQUAL(dist1.pdf(1), 0.25);  TEST_EQUAL(dist1.cdf(1), 0.5);   TEST_EQUAL(dist1.ccdf(1), 0.75);
    TEST_EQUAL(dist1.pdf(2), 0.25);  TEST_EQUAL(dist1.cdf(2), 0.75);  TEST_EQUAL(dist1.ccdf(2), 0.5);
    TEST_EQUAL(dist1.pdf(3), 0.25);  TEST_EQUAL(dist1.cdf(3), 1);     TEST_EQUAL(dist1.ccdf(3), 0.25);
    TEST_EQUAL(dist1.pdf(4), 0);     TEST_EQUAL(dist1.cdf(4), 1);     TEST_EQUAL(dist1.ccdf(4), 0);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist1(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 4u);

    TEST_NEAR(census[0] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[1] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.25, 0.002);

    UniformInteger dist2(1, 4);
    census.clear();

    TEST_EQUAL(dist2.pdf(0), 0);     TEST_EQUAL(dist2.cdf(0), 0);     TEST_EQUAL(dist2.ccdf(0), 1);
    TEST_EQUAL(dist2.pdf(1), 0.25);  TEST_EQUAL(dist2.cdf(1), 0.25);  TEST_EQUAL(dist2.ccdf(1), 1);
    TEST_EQUAL(dist2.pdf(2), 0.25);  TEST_EQUAL(dist2.cdf(2), 0.5);   TEST_EQUAL(dist2.ccdf(2), 0.75);
    TEST_EQUAL(dist2.pdf(3), 0.25);  TEST_EQUAL(dist2.cdf(3), 0.75);  TEST_EQUAL(dist2.ccdf(3), 0.5);
    TEST_EQUAL(dist2.pdf(4), 0.25);  TEST_EQUAL(dist2.cdf(4), 1);     TEST_EQUAL(dist2.ccdf(4), 0.25);
    TEST_EQUAL(dist2.pdf(5), 0);     TEST_EQUAL(dist2.cdf(5), 1);     TEST_EQUAL(dist2.ccdf(5), 0);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist2(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 4u);

    TEST_NEAR(census[1] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.25, 0.002);
    TEST_NEAR(census[4] / double(iterations), 0.25, 0.002);

    UniformInteger dist3(0_u64, ~ 0_u64);
    uint64_t u = 0;
    double w = 0;
    double sum = 0;
    double sum2 = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(u = dist3(rng));
        w = double(u);
        sum += w;
        sum2 += w * w;
    }

    double m = sum / iterations;
    double s = std::sqrt(sum2 / iterations - m * m);

    TEST_NEAR(m, 9.22e18, 1e16);
    TEST_NEAR(s, 5.33e18, 1e16);

}

void test_crow_random_poisson_distribution() {

    static constexpr int iterations = 1'000'000;

    Statistics stats;
    Pcg64 rng(42);
    int x = 0;

    PoissonDistribution dist1(4);
    TEST_EQUAL(dist1.mean(), 4);
    TEST_EQUAL(dist1.variance(), 4);
    TEST_EQUAL(dist1.sd(), 2);

    x = 0;   TEST_NEAR(dist1.pdf(x), 0.018316, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.018316, 1e-6);  TEST_EQUAL(dist1.ccdf(x), 1);
    x = 1;   TEST_NEAR(dist1.pdf(x), 0.073263, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.091578, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.981684, 1e-6);
    x = 2;   TEST_NEAR(dist1.pdf(x), 0.146525, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.238103, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.908422, 1e-6);
    x = 3;   TEST_NEAR(dist1.pdf(x), 0.195367, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.433470, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.761897, 1e-6);
    x = 4;   TEST_NEAR(dist1.pdf(x), 0.195367, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.628837, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.566530, 1e-6);
    x = 5;   TEST_NEAR(dist1.pdf(x), 0.156293, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.785130, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.371163, 1e-6);
    x = 6;   TEST_NEAR(dist1.pdf(x), 0.104196, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.889326, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.214870, 1e-6);
    x = 7;   TEST_NEAR(dist1.pdf(x), 0.059540, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.948866, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.110674, 1e-6);
    x = 8;   TEST_NEAR(dist1.pdf(x), 0.029770, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.978637, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.051134, 1e-6);
    x = 9;   TEST_NEAR(dist1.pdf(x), 0.013231, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.991868, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.021363, 1e-6);
    x = 10;  TEST_NEAR(dist1.pdf(x), 0.005292, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.997160, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.008132, 1e-6);
    x = 11;  TEST_NEAR(dist1.pdf(x), 0.001925, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.999085, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.002840, 1e-6);
    x = 12;  TEST_NEAR(dist1.pdf(x), 0.000642, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.999726, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.000915, 1e-6);
    x = 13;  TEST_NEAR(dist1.pdf(x), 0.000197, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.999924, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.000274, 1e-6);
    x = 14;  TEST_NEAR(dist1.pdf(x), 0.000056, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.999980, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.000076, 1e-6);
    x = 15;  TEST_NEAR(dist1.pdf(x), 0.000015, 1e-6);  TEST_NEAR(dist1.cdf(x), 0.999995, 1e-6);  TEST_NEAR(dist1.ccdf(x), 0.000020, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist1(rng));
        stats(double(x));
    }

    TEST_NEAR(stats.mean(), dist1.mean(), 0.002);
    TEST_NEAR(stats.sd(), dist1.sd(), 0.002);

    PoissonDistribution dist2(100);
    TEST_EQUAL(dist2.mean(), 100);
    TEST_EQUAL(dist2.variance(), 100);
    TEST_EQUAL(dist2.sd(), 10);

    x = 0;    TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_EQUAL(dist2.ccdf(x), 1);
    x = 5;    TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 10;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 15;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 20;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 25;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 30;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 35;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 40;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 45;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 50;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 55;   TEST_NEAR(dist2.pdf(x), 0.000000, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000001, 1e-6);  TEST_NEAR(dist2.ccdf(x), 1.000000, 1e-6);
    x = 60;   TEST_NEAR(dist2.pdf(x), 0.000004, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000011, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.999994, 1e-6);
    x = 65;   TEST_NEAR(dist2.pdf(x), 0.000045, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000123, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.999922, 1e-6);
    x = 70;   TEST_NEAR(dist2.pdf(x), 0.000311, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.000971, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.999339, 1e-6);
    x = 75;   TEST_NEAR(dist2.pdf(x), 0.001499, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.005473, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.996027, 1e-6);
    x = 80;   TEST_NEAR(dist2.pdf(x), 0.005198, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.022649, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.982549, 1e-6);
    x = 85;   TEST_NEAR(dist2.pdf(x), 0.013205, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.070750, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.942455, 1e-6);
    x = 90;   TEST_NEAR(dist2.pdf(x), 0.025039, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.171385, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.853654, 1e-6);
    x = 95;   TEST_NEAR(dist2.pdf(x), 0.036012, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.331192, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.704821, 1e-6);
    x = 100;  TEST_NEAR(dist2.pdf(x), 0.039861, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.526562, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.513299, 1e-6);
    x = 105;  TEST_NEAR(dist2.pdf(x), 0.034401, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.712808, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.321593, 1e-6);
    x = 110;  TEST_NEAR(dist2.pdf(x), 0.023423, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.852863, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.170560, 1e-6);
    x = 115;  TEST_NEAR(dist2.pdf(x), 0.012718, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.936821, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.075897, 1e-6);
    x = 120;  TEST_NEAR(dist2.pdf(x), 0.005561, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.977331, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.028230, 1e-6);
    x = 125;  TEST_NEAR(dist2.pdf(x), 0.001976, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.993202, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.008774, 1e-6);
    x = 130;  TEST_NEAR(dist2.pdf(x), 0.000575, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.998293, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.002282, 1e-6);
    x = 135;  TEST_NEAR(dist2.pdf(x), 0.000138, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.999640, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.000498, 1e-6);
    x = 140;  TEST_NEAR(dist2.pdf(x), 0.000028, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.999936, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.000092, 1e-6);
    x = 145;  TEST_NEAR(dist2.pdf(x), 0.000005, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.999990, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.000014, 1e-6);
    x = 150;  TEST_NEAR(dist2.pdf(x), 0.000001, 1e-6);  TEST_NEAR(dist2.cdf(x), 0.999999, 1e-6);  TEST_NEAR(dist2.ccdf(x), 0.000002, 1e-6);

    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist2(rng));
        stats(double(x));
    }

    TEST_NEAR(stats.mean(), dist2.mean(), 0.1);
    TEST_NEAR(stats.sd(), dist2.sd(), 0.1);

}
