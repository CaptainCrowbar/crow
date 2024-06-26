#include "crow/guard.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;

using Crow::UnitTest::format_range;

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
        throw std::runtime_error("failed");
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
        throw std::runtime_error("failed");
    }
    catch (...) {}
    TEST_EQUAL(n, 1);

    n = 0;
    {
        n = 1;
        auto guard = on_scope_failure([&] { n = 2; });
        TEST_EQUAL(n, 1);
    }
    TEST_EQUAL(n, 1);

    n = 0;
    try {
        n = 1;
        auto guard = on_scope_failure([&] { n = 2; });
        TEST_EQUAL(n, 1);
        throw std::runtime_error("failed");
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
        throw std::runtime_error("failed");
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
        throw std::runtime_error("failed");
    }
    catch (...) {}
    TEST_EQUAL(s, "a");

    s.clear();
    {
        s = "a";
        ScopeFailure guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
    }
    TEST_EQUAL(s, "a");

    s.clear();
    try {
        s = "a";
        ScopeFailure guard;
        guard += [&] { s += "b"; };
        guard += [&] { s += "c"; };
        TEST_EQUAL(s, "a");
        throw std::runtime_error("failed");
    }
    catch (...) {}
    TEST_EQUAL(s, "acb");

}

void test_crow_scope_guard_saved_container_size() {

    std::vector<int> v = {1, 2, 3};
    {
        auto guard = guard_size(v);
        v.push_back(4);
        v.push_back(5);
    }
    TEST_EQUAL(v.size(), 5u);
    TEST_EQUAL(format_range(v), "[1,2,3,4,5]");

    v = {1, 2, 3};
    try {
        auto guard = guard_size(v);
        v.push_back(4);
        v.push_back(5);
        throw std::runtime_error("failed");
    }
    catch (...) {}
    TEST_EQUAL(v.size(), 3u);
    TEST_EQUAL(format_range(v), "[1,2,3]");

}

void test_crow_scope_guard_saved_value() {

    int n = 1;
    {
        auto guard = guard_value(n);
        n = 2;
        TEST_EQUAL(n, 2);
    }
    TEST_EQUAL(n, 2);

    n = 1;
    try {
        auto guard = guard_value(n);
        n = 2;
        TEST_EQUAL(n, 2);
        throw std::runtime_error("failed");
    }
    catch (...) {}
    TEST_EQUAL(n, 1);

}
