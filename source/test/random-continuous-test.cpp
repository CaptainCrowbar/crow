#include "crow/random-continuous-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/statistics.hpp"
#include "crow/unit-test.hpp"
#include <array>
#include <cmath>

using namespace Crow;

void test_crow_random_uniform_real_distribution_properties() {

    auto ur = UniformReal<double>(10.0, 20.0);

    TEST_EQUAL(ur.min(), 10);
    TEST_EQUAL(ur.max(), 20);
    TEST_EQUAL(ur.mean(), 15);
    TEST_NEAR(ur.variance(), 8.333333, 1e-6);
    TEST_NEAR(ur.sd(), 2.886751, 1e-6);

    TEST_EQUAL(ur.pdf(9), 0);
    TEST_EQUAL(ur.pdf(11), 0.1);
    TEST_EQUAL(ur.pdf(13), 0.1);
    TEST_EQUAL(ur.pdf(15), 0.1);
    TEST_EQUAL(ur.pdf(17), 0.1);
    TEST_EQUAL(ur.pdf(19), 0.1);
    TEST_EQUAL(ur.pdf(21), 0);

    TEST_EQUAL(ur.cdf(8), 0);
    TEST_EQUAL(ur.cdf(10), 0);
    TEST_EQUAL(ur.cdf(12), 0.2);
    TEST_EQUAL(ur.cdf(14), 0.4);
    TEST_EQUAL(ur.cdf(16), 0.6);
    TEST_EQUAL(ur.cdf(18), 0.8);
    TEST_EQUAL(ur.cdf(20), 1);
    TEST_EQUAL(ur.cdf(22), 1);

    TEST_EQUAL(ur.ccdf(8), 1);
    TEST_EQUAL(ur.ccdf(10), 1);
    TEST_EQUAL(ur.ccdf(12), 0.8);
    TEST_EQUAL(ur.ccdf(14), 0.6);
    TEST_EQUAL(ur.ccdf(16), 0.4);
    TEST_EQUAL(ur.ccdf(18), 0.2);
    TEST_EQUAL(ur.ccdf(20), 0);
    TEST_EQUAL(ur.ccdf(22), 0);

    TEST_EQUAL(ur.quantile(0), 10);
    TEST_EQUAL(ur.quantile(0.2), 12);
    TEST_EQUAL(ur.quantile(0.4), 14);
    TEST_EQUAL(ur.quantile(0.6), 16);
    TEST_EQUAL(ur.quantile(0.8), 18);
    TEST_EQUAL(ur.quantile(1), 20);

    TEST_EQUAL(ur.cquantile(0), 20);
    TEST_EQUAL(ur.cquantile(0.2), 18);
    TEST_EQUAL(ur.cquantile(0.4), 16);
    TEST_EQUAL(ur.cquantile(0.6), 14);
    TEST_EQUAL(ur.cquantile(0.8), 12);
    TEST_EQUAL(ur.cquantile(1), 10);

}

void test_crow_random_uniform_real_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformReal<double> dist;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0.5, 0.001);
    TEST_NEAR(stats.sd(), 0.2887, 0.001);
    TEST_NEAR(stats.min(), 0, 0.001);
    TEST_NEAR(stats.max(), 1, 0.001);

    TRY(dist = UniformReal<double>(-100, 100));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.1);
    TEST_NEAR(stats.sd(), 57.74, 0.1);
    TEST_NEAR(stats.min(), -100, 0.1);
    TEST_NEAR(stats.max(), 100, 0.1);

}

void test_crow_random_log_uniform_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogUniform<double> dist(10, 100'000);
    Statistics<double> stats;
    std::array<int, 4> count = {{0,0,0,0}};
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
        if (x < 100)
            ++count[0];
        else if (x < 1'000)
            ++count[1];
        else if (x < 10'000)
            ++count[2];
        else
            ++count[3];
    }

    double fraction[4];
    for (int i = 0; i < 4; ++i)
        fraction[i] = count[i] / double(iterations);

    TEST_NEAR(fraction[0], 0.25, 0.001);
    TEST_NEAR(fraction[1], 0.25, 0.001);
    TEST_NEAR(fraction[2], 0.25, 0.001);
    TEST_NEAR(fraction[3], 0.25, 0.001);
    TEST_NEAR(stats.min(), 10, 0.01);
    TEST_NEAR(stats.max(), 100'000, 100);

}

void test_crow_random_normal_distribution_properties() {

    struct sample_type { double z, pdf, cdf; };

    static constexpr sample_type sample_list[] = {
        { -5.00,  0.00000'14867'20,  0.00000'02866'52 },
        { -4.75,  0.00000'50295'07,  0.00000'10170'83 },
        { -4.50,  0.00001'59837'41,  0.00000'33976'73 },
        { -4.25,  0.00004'77186'37,  0.00001'06885'26 },
        { -4.00,  0.00013'38302'26,  0.00003'16712'42 },
        { -3.75,  0.00035'25956'82,  0.00008'84172'85 },
        { -3.50,  0.00087'26826'95,  0.00023'26290'79 },
        { -3.25,  0.00202'90480'57,  0.00057'70250'42 },
        { -3.00,  0.00443'18484'12,  0.00134'98980'32 },
        { -2.75,  0.00909'35625'02,  0.00297'97632'35 },
        { -2.50,  0.01752'83004'94,  0.00620'96653'26 },
        { -2.25,  0.03173'96518'36,  0.01222'44726'55 },
        { -2.00,  0.05399'09665'13,  0.02275'01319'48 },
        { -1.75,  0.08627'73188'27,  0.04005'91568'64 },
        { -1.50,  0.12951'75956'66,  0.06680'72012'69 },
        { -1.25,  0.18264'90853'89,  0.10564'97736'67 },
        { -1.00,  0.24197'07245'19,  0.15865'52539'31 },
        { -0.75,  0.30113'74321'55,  0.22662'73523'77 },
        { -0.50,  0.35206'53267'64,  0.30853'75387'26 },
        { -0.25,  0.38666'81168'03,  0.40129'36743'17 },
        { 0.00,   0.39894'22804'01,  0.50000'00000'00 },
        { 0.25,   0.38666'81168'03,  0.59870'63256'83 },
        { 0.50,   0.35206'53267'64,  0.69146'24612'74 },
        { 0.75,   0.30113'74321'55,  0.77337'26476'23 },
        { 1.00,   0.24197'07245'19,  0.84134'47460'69 },
        { 1.25,   0.18264'90853'89,  0.89435'02263'33 },
        { 1.50,   0.12951'75956'66,  0.93319'27987'31 },
        { 1.75,   0.08627'73188'27,  0.95994'08431'36 },
        { 2.00,   0.05399'09665'13,  0.97724'98680'52 },
        { 2.25,   0.03173'96518'36,  0.98777'55273'45 },
        { 2.50,   0.01752'83004'94,  0.99379'03346'74 },
        { 2.75,   0.00909'35625'02,  0.99702'02367'65 },
        { 3.00,   0.00443'18484'12,  0.99865'01019'68 },
        { 3.25,   0.00202'90480'57,  0.99942'29749'58 },
        { 3.50,   0.00087'26826'95,  0.99976'73709'21 },
        { 3.75,   0.00035'25956'82,  0.99991'15827'15 },
        { 4.00,   0.00013'38302'26,  0.99996'83287'58 },
        { 4.25,   0.00004'77186'37,  0.99998'93114'74 },
        { 4.50,   0.00001'59837'41,  0.99999'66023'27 },
        { 4.75,   0.00000'50295'07,  0.99999'89829'17 },
        { 5.00,   0.00000'14867'20,  0.99999'97133'48 },
    };

    NormalDistribution<double> norm;

    TEST_EQUAL(norm.mean(), 0);
    TEST_EQUAL(norm.variance(), 1);
    TEST_EQUAL(norm.sd(), 1);

    double p, q, y, z;

    for (auto& sample: sample_list) {
        TRY(y = norm.pdf(sample.z));          TEST_NEAR(y, sample.pdf, 1e-10);
        TRY(p = norm.cdf(sample.z));          TEST_NEAR(p, sample.cdf, 1e-10);
        TRY(q = norm.ccdf(sample.z));         TEST_NEAR(q, 1 - sample.cdf, 1e-10);
        TRY(z = norm.quantile(sample.cdf));   TEST_NEAR(z, sample.z, 1e-6);
        TRY(z = norm.cquantile(sample.cdf));  TEST_NEAR(z, - sample.z, 1e-6);
    }

}

void test_crow_random_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    NormalDistribution<double> norm;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    TRY(norm = NormalDistribution<double>(100, 50));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 100, 0.1);
    TEST_NEAR(stats.sd(), 50, 0.1);

}

void test_crow_random_log_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogNormal<double> dist;
    Statistics<double> stats;
    double x = 0;
    double lx = 0;

    TEST_EQUAL(dist.m(), 0);
    TEST_EQUAL(dist.s(), 1);
    TEST_EQUAL(dist.median(), 1);
    TEST_NEAR(dist.s_factor(), 2.718'282, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    dist = LogNormal<double>(2, 1, LogMode::common);
    stats.clear();

    TEST_NEAR(dist.m(), 4.605'170, 1e-6);
    TEST_NEAR(dist.s(), 2.302'585, 1e-6);
    TEST_NEAR(dist.median(), 100, 1e-6);
    TEST_NEAR(dist.s_factor(), 10, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 4.605, 0.01);
    TEST_NEAR(stats.sd(), 2.303, 0.01);

}
