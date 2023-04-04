#include "crow/resource.hpp"
#include "crow/unit-test.hpp"
#include <utility>

using namespace Crow;

namespace {

    using C = Crow::UnitTest::Counted<int>;

    struct Release {
        void operator()(C* p) noexcept {
            delete p;
        }
    };

}

void test_crow_resource_handle() {

    Resource<C*, Release> a, b;

    TEST_EQUAL(C::count(), 0);
    TEST(! a);

    TRY(a = new C(1));
    REQUIRE(a);
    TEST_EQUAL(*a, 1);
    TEST_EQUAL(C::count(), 1);

    TRY(b = new C(2));
    REQUIRE(b);
    TEST_EQUAL(*b, 2);
    TEST_EQUAL(C::count(), 2);

    TRY(swap(a, b));
    REQUIRE(a);
    REQUIRE(b);
    TEST_EQUAL(*a, 2);
    TEST_EQUAL(*b, 1);
    TEST_EQUAL(C::count(), 2);

    TRY(a = std::move(b));
    REQUIRE(a);
    TEST(! b);
    TEST_EQUAL(*a, 1);
    TEST_EQUAL(C::count(), 1);

    TRY(a.reset());
    TEST(! a);
    TEST(! b);
    TEST_EQUAL(C::count(), 0);

}
