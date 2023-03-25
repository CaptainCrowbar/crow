#include "crow/root-finding.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace Crow;

void test_crow_root_finding_bisection() {

    auto f = [] (double x) { return x * x * x - 2 * x + 0.25; };
    auto rf = bisection<double>(f);
    double x = 0;
    double eps = 1e-10;

    TRY(x = rf->solve(0, 0, 1));
    TEST_NEAR(x, 0.126'000'192'6, eps);
    TEST_NEAR(rf->error(), 0.0, eps);
    TEST(rf->count() < 50);

    TRY(x = rf->solve(0, 1, 2));
    TEST_NEAR(x, 1.346'997'408'5, eps);
    TEST_NEAR(rf->error(), 0.0, eps);
    TEST(rf->count() < 50);

    TRY(x = rf->solve(0, -1, -2));
    TEST_NEAR(x, -1.472'997'601'1, eps);
    TEST_NEAR(rf->error(), 0.0, eps);
    TEST(rf->count() < 50);

    TEST_THROW(x = rf->solve(0, 0, -1), std::domain_error);
    TRY(rf->set_limit(10));
    TRY(rf->set_strict());
    TEST_THROW(x = rf->solve(0, 0, 1), std::range_error);

}

void test_crow_root_finding_newton_raphson() {

    double k = 0, x = 0;
    double root1 = 0.865'474'033'1;

    auto f = [&k] (double x) { return std::cos(k * x) - std::pow(k * x, 3.0); };
    auto df = [&k] (double x) { return - k * (std::sin(k * x) + 3.0 * std::pow(k * x, 2.0)); };

    auto rf = newton_raphson<double>(f, df);

    for (int i = 1; i <= 1024; i *= 2) {
        k = double(i);
        TRY(x = rf->solve(0, 1));
        TEST_NEAR(x, root1 / i, 1e-10 / i);
        TEST_NEAR(rf->error(), 0.0, 1e-10);
        TEST(rf->count() < 25);
    }

    for (int i = 2; i <= 1024; i *= 2) {
        k = 1.0 / double(i);
        TRY(x = rf->solve(0, 1));
        TEST_NEAR(x, root1 * i, 1e-10 * i);
        TEST_NEAR(rf->error(), 0.0, 1e-10);
        TEST(rf->count() < 25);
    }

}
