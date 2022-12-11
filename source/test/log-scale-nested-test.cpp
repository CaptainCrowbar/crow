#include "crow/log-scale.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

using LS = LogScale<double>;
using LLS = LogScale<LS>;

void test_crow_log_scale_nested_construction() {

    LLS x;

    TEST_EQUAL(x.get().get(), 0);
    TEST_EQUAL(x.log().get(), 0);
    TEST_EQUAL(x.sign(), 0);

    TRY(x = LS(1));
    TEST_EQUAL(x.get().get(), 1);
    TEST_EQUAL(x.log().get(), 0);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(42));
    TEST_NEAR(x.get().get(), 42, 1e-6);
    TEST_NEAR(x.log().get(), 3.737670, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(1.2345e67));
    TEST_NEAR(x.get().get(), 1.2345e67, 1e57);
    TEST_NEAR(x.log().get(), 154.483867, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(1.2345e-67));
    TEST_NEAR(x.get().get(), 1.2345e-67, 1e-77);
    TEST_NEAR(x.log().get(), -154.062535, 1e-6);
    TEST_EQUAL(x.sign(), 1);

    TRY(x = LS(-1));
    TEST_EQUAL(x.get().get(), -1);
    TEST_EQUAL(x.log().get(), 0);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = LS(-1.2345e67));
    TEST_NEAR(x.get().get(), -1.2345e67, 1e57);
    TEST_NEAR(x.log().get(), 154.483867, 1e-6);
    TEST_EQUAL(x.sign(), -1);

    TRY(x = LS(-1.2345e-67));
    TEST_NEAR(x.get().get(), -1.2345e-67, 1e-77);
    TEST_NEAR(x.log().get(), -154.062535, 1e-6);
    TEST_EQUAL(x.sign(), -1);

}
