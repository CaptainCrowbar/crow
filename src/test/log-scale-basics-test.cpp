#include "crow/log-scale.hpp"
#include "crow/unit-test.hpp"
#include "crow/types.hpp"
#include <limits>
#include <unordered_set>

using namespace Crow;

using LS = LogScale<double>;

void test_crow_log_scale_properties() {

    TEST(NonIntegralNumericType<LS>);

}

void test_crow_log_scale_construction_from_value() {

    LS x;

    TEST_EQUAL(x.get(), 0);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 0);

    TRY(x = 1);
    TEST_EQUAL(x.get(), 1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = 42);
    TEST_NEAR(x.get(), 42, 1e-6);
    TEST_NEAR(x.log(), 3.737670, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = 1.2345e67);
    TEST_NEAR(x.get(), 1.2345e67, 1e57);
    TEST_NEAR(x.log(), 154.483867, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = 1.2345e-67);
    TEST_NEAR(x.get(), 1.2345e-67, 1e-77);
    TEST_NEAR(x.log(), -154.062535, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = -1);
    TEST_EQUAL(x.get(), -1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = -1.2345e67);
    TEST_NEAR(x.get(), -1.2345e67, 1e57);
    TEST_NEAR(x.log(), 154.483867, 1e-6);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = -1.2345e-67);
    TEST_NEAR(x.get(), -1.2345e-67, 1e-77);
    TEST_NEAR(x.log(), -154.062535, 1e-6);
    TEST_EQUAL(x.sign(), -1);

}

void test_crow_log_scale_construction_from_log_and_sign() {

    LS x;

    TRY(x = LS(0, 0));
    TEST_EQUAL(x.get(), 0);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 0);

    TRY(x = LS(42, 0));
    TEST_EQUAL(x.get(), 0);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 0);

    TRY(x = LS(0, 1));
    TEST_EQUAL(x.get(), 1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(0, 42));
    TEST_EQUAL(x.get(), 1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(0, -1));
    TEST_EQUAL(x.get(), -1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = LS(0, -42));
    TEST_EQUAL(x.get(), -1);
    TEST_EQUAL(x.log(), 0);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = LS(42, 42));
    TEST_NEAR(x.get(), 1.73927e18, 1e13);
    TEST_NEAR(x.log(), 42, 1e-10);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(-42, -42));
    TEST_NEAR(x.get(), -5.74952e-19, 1e-24);
    TEST_NEAR(x.log(), -42, 1e-10);
    TEST_EQUAL(x.sign(), -1);

}

void test_crow_log_scale_range() {

    using limits = std::numeric_limits<LS>;

    LS x, y;

    TRY(x = 0);                               TEST(x.is_in_range());
    TRY(x = 1);                               TEST(x.is_in_range());
    TRY(x = -1);                              TEST(x.is_in_range());
    TRY(x = 1.23e45);                         TEST(x.is_in_range());
    TRY(x = -1.23e45);                        TEST(x.is_in_range());
    TRY(x = 1.23e-45);                        TEST(x.is_in_range());
    TRY(x = -1.23e-45);                       TEST(x.is_in_range());
    TRY(x = LS(123456789, 1));                TEST(! x.is_in_range());
    TRY(x = LS(123456789, -1));               TEST(! x.is_in_range());
    TRY(x = LS(-123456789, 1));               TEST(! x.is_in_range());
    TRY(x = LS(-123456789, -1));              TEST(! x.is_in_range());
    TRY(x = LS::max_in_range());              TEST(x.is_in_range());
    TRY(y = nextafter(x, limits::max()));     TEST(! y.is_in_range()); TEST(y > x);
    TRY(y = nextafter(x, limits::lowest()));  TEST(y.is_in_range()); TEST(y < x);
    TRY(x = LS::min_in_range());              TEST(x.is_in_range());
    TRY(y = nextafter(x, limits::max()));     TEST(y.is_in_range()); TEST(y > x);
    TRY(y = nextafter(x, limits::lowest()));  TEST(! y.is_in_range()); TEST(y < x);

}

void test_crow_log_scale_hash() {

    std::unordered_set<LS> set;

    TRY(set.insert(LS(1)));
    TRY(set.insert(LS(23)));
    TRY(set.insert(LS(456)));
    TRY(set.insert(LS(7890)));

    TEST(! set.contains(LS(0)));
    TEST(set.contains(LS(1)));
    TEST(set.contains(LS(23)));
    TEST(set.contains(LS(456)));
    TEST(set.contains(LS(7890)));

}
