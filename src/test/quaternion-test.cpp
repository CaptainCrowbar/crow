#include "crow/quaternion.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <cstdlib>

using namespace Crow;

namespace {

    template <typename R>
    void fuzz(R& r) {
        for (auto& x: r)
            if (std::abs(x) < 1e-6)
                x = 0;
    }

}

void test_crow_quaternion() {

    Qdouble q, r, s;

    TRY(q = 0);
    TEST_EQUAL(q.str(), "H[0,0,0,0]");
    TRY(q = 42);
    TEST_EQUAL(q.str(), "H[42,0,0,0]");
    TRY((q = {10.0,20.0,30.0,40.0}));
    TEST_EQUAL(q.str(), "H[10,20,30,40]");
    TEST_EQUAL(q.str("e3"), "H[1.00e1,2.00e1,3.00e1,4.00e1]");
    TEST_EQUAL(q.str("f3"), "H[10.000,20.000,30.000,40.000]");

    TEST_EQUAL(q.a(), 10);
    TEST_EQUAL(q.b(), 20);
    TEST_EQUAL(q.c(), 30);
    TEST_EQUAL(q.d(), 40);
    TEST_EQUAL(q[0], 10);
    TEST_EQUAL(q[1], 20);
    TEST_EQUAL(q[2], 30);
    TEST_EQUAL(q[3], 40);

    TEST_EQUAL(q.scalar_part(), 10);
    TEST_EQUAL(q.vector_part().str(), "[20,30,40]");
    TEST_EQUAL(q.norm2(), 3000);
    TEST_NEAR(q.norm(), 54.772256, 1e-6);
    TEST_EQUAL(q.versor().str(), "H[0.182574,0.365148,0.547723,0.730297]");
    TEST_EQUAL(q.reciprocal().str(), "H[0.00333333,-0.00666667,-0.01,-0.0133333]");

    TRY(q *= 2);
    TEST_EQUAL(q.str(), "H[20,40,60,80]");
    TRY(q /= 4);
    TEST_EQUAL(q.str(), "H[5,10,15,20]");
    TRY((q = {10.0,20.0,30.0,40.0}));
    TRY((r = {100.0,200.0,300.0,400.0}));
    TRY(s = q + r);
    TEST_EQUAL(s.str(), "H[110,220,330,440]");
    TRY(s = q - r);
    TEST_EQUAL(s.str(), "H[-90,-180,-270,-360]");
    TRY((q = {2.0,3.0,5.0,7.0}));
    TRY((r = {11.0,13.0,17.0,19.0}));
    TRY(s = q * r);
    TEST_EQUAL(s.str(), "H[-235,35,123,101]");
    TRY((q = {10.0,20.0,30.0,40.0}));
    TRY(r = q * q.reciprocal());
    TRY(fuzz(r));
    TEST_EQUAL(r.str(), "H[1,0,0,0]");
    TRY(r = q.reciprocal() * q);
    TRY(fuzz(r));
    TEST_EQUAL(r.str(), "H[1,0,0,0]");

}
