#include "crow/superposition.hpp"
#include "crow/unit-test.hpp"
#include <cmath>
#include <stdexcept>

using namespace Crow;

void test_superposition_flat_construction() {

    FlatSuperposition<double> x;

    TEST_EQUAL(x.size(), 0u);
    TEST(x.empty());
    TEST(! x.is_unique());
    TEST_THROW(x.value(), std::invalid_argument);

    TEST_EQUAL(x.str(),      "null");
    TEST_EQUAL(x.str("f2"),  "null");

    TRY(x = 1234.5);

    TEST_EQUAL(x.size(), 1u);
    TEST(! x.empty());
    TEST(x.is_unique());
    TEST_EQUAL(x.value(), 1234.5);
    TEST_EQUAL(*x, 1234.5);
    TEST_EQUAL(x.min(), 1234.5);
    TEST_EQUAL(x.max(), 1234.5);
    TEST_EQUAL(*x.begin(), 1234.5);

    TEST_EQUAL(x.str(),      "1234.5");
    TEST_EQUAL(x.str("f2"),  "1234.50");

    TRY((x = {1, 2, 3, 4}));

    TEST_EQUAL(x.size(), 4u);
    TEST(! x.empty());
    TEST(! x.is_unique());
    TEST_THROW(x.value(), std::invalid_argument);
    TEST_EQUAL(x.min(), 1);
    TEST_EQUAL(x.max(), 4);
    TEST_EQUAL(*x.begin(), 1);

    TEST_EQUAL(x.str(),      "[1,2,3,4]");
    TEST_EQUAL(x.str("f2"),  "[1.00,2.00,3.00,4.00]");

}

void test_superposition_flat_arithmetic() {

    FlatSuperposition<double> x, y, z;

    TRY((x = {2, 3}));     TEST_EQUAL(x.str(), "[2,3]");
    TRY((y = {4, 5, 6}));  TEST_EQUAL(y.str(), "[4,5,6]");

    TRY(z = - x);    TEST_EQUAL(z.size(), 2u);  TEST_EQUAL(z.str(), "[-3,-2]");
    TRY(z = x + y);  TEST_EQUAL(z.size(), 4u);  TEST_EQUAL(z.str(), "[6,7,8,9]");
    TRY(z = x - y);  TEST_EQUAL(z.size(), 4u);  TEST_EQUAL(z.str(), "[-4,-3,-2,-1]");
    TRY(z = x * y);  TEST_EQUAL(z.size(), 5u);  TEST_EQUAL(z.str(), "[8,10,12,15,18]");
    TRY(z = x / y);  TEST_EQUAL(z.size(), 5u);  TEST_EQUAL(z.str(), "[0.333333,0.4,0.5,0.6,0.75]");

    TRY(z = x.apply([] (auto x) { return std::sqrt(x); }));
    TEST_EQUAL(z.size(), 2u);
    TEST_EQUAL(z.str(), "[1.41421,1.73205]");
    TRY(z = x.apply([] (auto x, auto y) { return std::pow(x, y); }, y));
    TEST_EQUAL(z.size(), 6u);
    TEST_EQUAL(z.str(), "[16,32,64,81,243,729]");

}

void test_superposition_flat_combination() {

    FlatSuperposition<double> x, y, z;

    TRY((x = {1, 2, 3, 4, 5}));        TEST_EQUAL(x.str(), "[1,2,3,4,5]");
    TRY((y = {3, 4, 5, 6, 7, 8, 9}));  TEST_EQUAL(y.str(), "[3,4,5,6,7,8,9]");

    TRY((z = {x, y}));                       TEST_EQUAL(z.size(), 9u);   TEST_EQUAL(z.str(), "[1,2,3,4,5,6,7,8,9]");
    TRY((z = {x, y, 10}));                   TEST_EQUAL(z.size(), 10u);  TEST_EQUAL(z.str(), "[1,2,3,4,5,6,7,8,9,10]");
    TRY(z = x.set_difference(y));            TEST_EQUAL(z.size(), 2u);   TEST_EQUAL(z.str(), "[1,2]");
    TRY(z = x.set_intersection(y));          TEST_EQUAL(z.size(), 3u);   TEST_EQUAL(z.str(), "[3,4,5]");
    TRY(z = x.set_symmetric_difference(y));  TEST_EQUAL(z.size(), 6u);   TEST_EQUAL(z.str(), "[1,2,6,7,8,9]");
    TRY(z = x.set_union(y));                 TEST_EQUAL(z.size(), 9u);   TEST_EQUAL(z.str(), "[1,2,3,4,5,6,7,8,9]");

}

void test_superposition_weighted_construction() {

    WeightedSuperposition<double> x;

    TEST_EQUAL(x.size(), 0u);
    TEST(x.empty());
    TEST(! x.is_unique());
    TEST_THROW(x.value(), std::invalid_argument);

    TEST_EQUAL(x.str(),      "null");
    TEST_EQUAL(x.str("f2"),  "null");

    TRY(x = 1234.5);

    TEST_EQUAL(x.size(), 1u);
    TEST(x.is_unique());
    TEST_EQUAL(x.value(), 1234.5);
    TEST_EQUAL(*x, 1234.5);
    TEST_EQUAL(x.min(), 1234.5);
    TEST_EQUAL(x.max(), 1234.5);
    TEST_EQUAL(x.mean(), 1234.5);
    TEST_EQUAL(x.sd(), 0);
    TEST_EQUAL(x.begin()->first, 1234.5);
    TEST_EQUAL(x.begin()->second, 1);

    TEST_EQUAL(x.str(),            "1234.5");
    TEST_EQUAL(x.str("f2"),        "1234.50");
    TEST_EQUAL(x.str("", "f4"),    "1234.5");
    TEST_EQUAL(x.str("f2", "f4"),  "1234.50");

    TRY((x = {1, 1, 2, 2, 3, 3, 4, 4}));
    // TRY((x = {{1, 1}, {2, 2}, {3, 3}, {4, 4}}));

    TEST_EQUAL(x.size(), 4u);
    TEST(! x.is_unique());
    TEST_THROW(x.value(), std::invalid_argument);
    TEST_EQUAL(x.min(), 1);
    TEST_EQUAL(x.max(), 4);
    TEST_EQUAL(x.mean(), 3);
    TEST_EQUAL(x.sd(), 1);
    TEST_EQUAL(x.begin()->first, 1);
    TEST_NEAR(x.begin()->second, 0.1, 1e-10);

    TEST_EQUAL(x.str(),            "{1:0.1,2:0.2,3:0.3,4:0.4}");
    TEST_EQUAL(x.str("f2"),        "{1.00:0.1,2.00:0.2,3.00:0.3,4.00:0.4}");
    TEST_EQUAL(x.str("", "f4"),    "{1:0.1000,2:0.2000,3:0.3000,4:0.4000}");
    TEST_EQUAL(x.str("f2", "f4"),  "{1.00:0.1000,2.00:0.2000,3.00:0.3000,4.00:0.4000}");

}

void test_superposition_weighted_arithmetic() {

    WeightedSuperposition<double> x, y, z;

    TRY((x = {2, 3, 3, 1}));        TEST_EQUAL(x.str(), "{2:0.75,3:0.25}");
    TRY((y = {4, 1, 5, 1, 6, 2}));  TEST_EQUAL(y.str(), "{4:0.25,5:0.25,6:0.5}");

    TRY(z = - x);    TEST_EQUAL(z.size(), 2u);  TEST_EQUAL(z.str(), "{-3:0.25,-2:0.75}");
    TRY(z = x + y);  TEST_EQUAL(z.size(), 4u);  TEST_EQUAL(z.str(), "{6:0.1875,7:0.25,8:0.4375,9:0.125}");
    TRY(z = x - y);  TEST_EQUAL(z.size(), 4u);  TEST_EQUAL(z.str(), "{-4:0.375,-3:0.3125,-2:0.25,-1:0.0625}");
    TRY(z = x * y);  TEST_EQUAL(z.size(), 5u);  TEST_EQUAL(z.str(), "{8:0.1875,10:0.1875,12:0.4375,15:0.0625,18:0.125}");
    TRY(z = x / y);  TEST_EQUAL(z.size(), 5u);  TEST_EQUAL(z.str(), "{0.333333:0.375,0.4:0.1875,0.5:0.3125,0.6:0.0625,0.75:0.0625}");

    TRY(z = x.apply([] (auto x) { return std::sqrt(x); }));
    TEST_EQUAL(z.size(), 2u);
    TEST_EQUAL(z.str(), "{1.41421:0.75,1.73205:0.25}");
    TRY(z = x.apply([] (auto x, auto y) { return std::pow(x, y); }, y));
    TEST_EQUAL(z.size(), 6u);
    TEST_EQUAL(z.str(), "{16:0.1875,32:0.1875,64:0.375,81:0.0625,243:0.0625,729:0.125}");

}

void test_superposition_weighted_combination() {

    WeightedSuperposition<double> x, y, z;

    TRY((x = {2, 8, 3, 1, 4, 1}));
    TEST_EQUAL(x.str(), "{2:0.8,3:0.1,4:0.1}");
    TRY((y = {4, 1, 5, 1, 6, 3}));
    TEST_EQUAL(y.str(), "{4:0.2,5:0.2,6:0.6}");
    TRY((z = {x, 1, y, 4}));
    TEST_EQUAL(z.size(), 5u);
    TEST_EQUAL(z.str(), "{2:0.16,3:0.02,4:0.18,5:0.16,6:0.48}");

}
