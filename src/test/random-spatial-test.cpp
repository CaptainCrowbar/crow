#include "crow/random-spatial-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <cmath>

using namespace Crow;

void test_crow_random_vectors() {

    static constexpr int iterations = 1'000'000;
    static constexpr Double3 apex(5, 10, 15);

    Pcg64 rng(42);
    RandomVector<double, 3> rv(apex);
    Double3 low(100.0);
    Double3 high(-100.0);
    Double3 sum(0.0);
    Double3 sum2(0.0);
    Double3 v;

    for (int i = 0; i < iterations; ++i) {
        TRY(v = rv(rng));
        sum += v;
        for (int j = 0; j < 3; ++j) {
            low[j] = std::min(low[j], v[j]);
            high[j] = std::max(high[j], v[j]);
            sum2[j] += v[j] * v[j];
        }
    }

    Double3 mean = sum / iterations;
    Double3 sd;

    for (int i = 0; i < 3; ++i)
        sd[i] = std::sqrt(sum2[i] / iterations - mean[i] * mean[i]);

    TEST(low[0] > 0);
    TEST(low[1] > 0);
    TEST(low[2] > 0);
    TEST(high[0] < 5);
    TEST(high[1] < 10);
    TEST(high[2] < 15);
    TEST_NEAR(mean[0], 2.5, 0.01);
    TEST_NEAR(mean[1], 5, 0.01);
    TEST_NEAR(mean[2], 7.5, 0.01);
    TEST_NEAR(sd[0], 1.443376, 0.01);
    TEST_NEAR(sd[1], 2.886751, 0.01);
    TEST_NEAR(sd[2], 4.330127, 0.01);

    SymmetricRandomVector<double, 3> srv(apex);
    low = Double3(100);
    high = Double3(-100);
    sum = Double3::null();
    sum2 = Double3::null();

    for (int i = 0; i < iterations; ++i) {
        TRY(v = srv(rng));
        sum += v;
        for (int j = 0; j < 3; ++j) {
            low[j] = std::min(low[j], v[j]);
            high[j] = std::max(high[j], v[j]);
            sum2[j] += v[j] * v[j];
        }
    }

    mean = sum / iterations;

    for (int i = 0; i < 3; ++i)
        sd[i] = std::sqrt(sum2[i] / iterations - mean[i] * mean[i]);

    TEST(low[0] > -5);
    TEST(low[1] > -10);
    TEST(low[2] > -15);
    TEST(high[0] < 5);
    TEST(high[1] < 10);
    TEST(high[2] < 15);
    TEST_NEAR(mean[0], 0, 0.01);
    TEST_NEAR(mean[1], 0, 0.01);
    TEST_NEAR(mean[2], 0, 0.01);
    TEST_NEAR(sd[0], 2.886751, 0.01);
    TEST_NEAR(sd[1], 5.773503, 0.01);
    TEST_NEAR(sd[2], 8.660254, 0.01);

}

namespace {

    template <int N>
    void random_point_in_sphere_test() {

        static constexpr int iterations = 100'000;

        Pcg64 rng(42);
        RandomPointInSphere<double, N> gen(10);
        auto count_sides = Vector<double, N>();
        auto total = Vector<double, N>();
        Vector<double, N> point;
        double count_inner = 0;

        for (int i = 0; i < iterations; ++i) {
            TRY(point = gen(rng));
            for (int j = 0; j < N; ++j)
                if (point[j] > 0)
                    ++count_sides[j];
            if (point.r() < 5)
                ++count_inner;
            total += point;
        }

        double epsilon = 10 / std::sqrt(double(iterations));
        double expect_inner = std::pow(2.0, - double(N));

        for (auto& c: count_sides) {
            c /= iterations;
            TEST_NEAR(c, 0.5, epsilon);
        }

        count_inner /= iterations;
        total /= iterations;

        TEST_NEAR(count_inner, expect_inner, epsilon);
        TEST_NEAR(total.r(), 0, epsilon);

    }

    template <int N>
    void random_direction_test() {

        static constexpr int iterations = 100'000;

        Pcg64 rng(42);
        RandomDirection<double, N> gen;
        auto count_sides = Vector<double, N>();
        auto total = Vector<double, N>();
        Vector<double, N> point;

        for (int i = 0; i < iterations; ++i) {
            TRY(point = gen(rng));
            TEST_NEAR(point.r(), 1, 1e-6);
            for (int j = 0; j < N; ++j)
                if (point[j] > 0)
                    ++count_sides[j];
            total += point;
        }

        double epsilon = 2 / std::sqrt(double(iterations));

        for (auto& c: count_sides) {
            c /= iterations;
            TEST_NEAR(c, 0.5, epsilon);
        }

        total /= iterations;

        TEST_NEAR(total.r(), 0, 2 * epsilon); // more variance in surface points

    }

}

void test_crow_random_point_in_sphere() {

    random_point_in_sphere_test<1>();
    random_point_in_sphere_test<2>();
    random_point_in_sphere_test<3>();
    random_point_in_sphere_test<4>();
    random_point_in_sphere_test<5>();
    random_point_in_sphere_test<6>();
    random_point_in_sphere_test<7>();
    random_point_in_sphere_test<8>();
    random_point_in_sphere_test<9>();
    random_point_in_sphere_test<10>();

}

void test_crow_random_direction() {

    random_direction_test<1>();
    random_direction_test<2>();
    random_direction_test<3>();
    random_direction_test<4>();
    random_direction_test<5>();
    random_direction_test<6>();
    random_direction_test<7>();
    random_direction_test<8>();
    random_direction_test<9>();
    random_direction_test<10>();

}
