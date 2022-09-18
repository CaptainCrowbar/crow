#include "crow/root-finding.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <iostream>

using namespace Crow;

namespace {

    struct F {
        double k;
        double operator()(double x) const { return std::cos(k * x) - std::pow(k * x, 3.0); };
    };

    struct DF {
        double k;
        double operator()(double x) const { return - k * (std::sin(k * x) + 3.0 * std::pow(k * x, 2.0)); };
    };

    constexpr double root1 = 0.865'474'033'1;

}

void test_crow_root_finding_newton_raphson() {

    double x = 0;

    for (int i = 1; i <= 1024; i *= 2) {
        const double k = double(i);
        auto rf = newton_raphson<double>(F{k}, DF{k});
        TRY(x = rf->solve(0, 1));
        // std::cout << "k=" << i << "\tx=" << x << "\terror=" << rf->error() << "\tcount=" << rf->count() << "\n";
        TEST_NEAR(x, root1 / i, 1e-10 / i);
        TEST_NEAR(rf->error(), 0.0, 1e-10);
        TEST(rf->count() < 25);
    }

    for (int i = 2; i <= 1024; i *= 2) {
        const double k = 1.0 / double(i);
        auto rf = newton_raphson<double>(F{k}, DF{k});
        TRY(x = rf->solve(0, 1));
        // std::cout << "k=1/" << i << "\tx=" << x << "\terror=" << rf->error() << "\tcount=" << rf->count() << "\n";
        TEST_NEAR(x, root1 * i, 1e-10 * i);
        TEST_NEAR(rf->error(), 0.0, 1e-10);
        TEST(rf->count() < 25);
    }

}
