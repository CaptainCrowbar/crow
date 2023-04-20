#include "crow/compact-array.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::UnitTest;

void test_crow_compact_array_construction() {

    using ICA = CompactArray<int, 5>;

    ICA a;
    std::string s;

    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_THROW(a.at(0), std::out_of_range);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[]");

    ICA b(5, 42);

    TEST(! b.empty());
    TEST(b.is_compact());
    TEST_EQUAL(b.size(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(b[0], 42);
    TEST_EQUAL(b[4], 42);
    TEST_EQUAL(b.at(0), 42);
    TEST_EQUAL(b.at(4), 42);
    TEST_THROW(b.at(5), std::out_of_range);
    TRY(s = format_range(b));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a = b);
    TEST(! a.empty());
    TEST(! b.empty());
    TEST(a.is_compact());
    TEST(b.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(b.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a = std::move(b));
    TEST(! a.empty());
    TEST(b.empty());
    TEST(a.is_compact());
    TEST(b.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(b.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(b.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    ICA c(10, 42);

    TEST(! c.empty());
    TEST(! c.is_compact());
    TEST_EQUAL(c.size(), 10u);
    TEST_EQUAL(c.capacity(), 16u);
    TEST_EQUAL(c[0], 42);
    TEST_EQUAL(c[9], 42);
    TRY(s = format_range(c));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY(a = c);
    TEST(! a.empty());
    TEST(! c.empty());
    TEST(! a.is_compact());
    TEST(! c.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(c.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(c.capacity(), 16u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY(a = std::move(c));
    TEST(! a.empty());
    TEST(c.empty());
    TEST(! a.is_compact());
    TEST(c.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(c.size(), 0u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(c.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,42,42,42,42,42]");

    TRY((a = ICA{1,2,3,4,5}));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");
    TEST_EQUAL(a.front(), 1);
    TEST_EQUAL(a.back(), 5);

    TRY((a = ICA{1,2,3,4,5,6,7,8,9,10}));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    TRY((a = {1,2,3,4,5}));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");

    TRY((a = {1,2,3,4,5,6,7,8,9,10}));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    std::vector<int> v = {1,2,3,4,5};
    TRY(a = ICA(v.begin(), v.end()));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[4], 5);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5]");

    v = {1,2,3,4,5,6,7,8,9,10};
    TRY(a = ICA(v.begin(), v.end()));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 1);
    TEST_EQUAL(a[9], 10);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[1,2,3,4,5,6,7,8,9,10]");

    TRY(a.clear());
    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);

}
