#include "crow/guard.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_scope_guard() {

    int n = 0;
    {
        n = 1;
        auto guard = on_scope_exit([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 2);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_exit([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 2);

    n = 0;
    {
        n = 1;
        auto guard = on_scope_success([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 2);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_success([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 1);

    n = 0;
    {
        n = 1;
        auto guard = on_scope_fail([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 1);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_fail([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(n, 2);

}

void test_crow_scope_guard_multiple_action() {

    std::string s;
    {
        s = "a";
        ScopeExit guard([&] { s += "b"; });
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
    }
    TEST_EQUAL(s, "acb");

    s.clear();
    try {
        s = "a";
        ScopeExit guard([&] { s += "b"; });
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(s, "acb");

    s.clear();
    {
        s = "a";
        ScopeSuccess guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
    }
    TEST_EQUAL(s, "acb");

    s.clear();
    try {
        s = "a";
        ScopeSuccess guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(s, "a");

    s.clear();
    {
        s = "a";
        ScopeFail guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
    }
    TEST_EQUAL(s, "a");

    s.clear();
    try {
        s = "a";
        ScopeFail guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
        throw std::runtime_error("fail");
    }
    catch (...) {}
    TEST_EQUAL(s, "acb");

}
