#include "crow/compact-array.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <string>
#include <unordered_map>

using namespace Crow;
using namespace Crow::UnitTest;

void test_crow_compact_array_capacity() {

    using ICA = CompactArray<int, 5>;

    ICA a;
    std::string s;

    TRY(a.resize(5, 42));
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 5u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[4], 42);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42]");

    TRY(a.resize(10));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 16u);
    TEST_EQUAL(a[0], 42);
    TEST_EQUAL(a[9], 0);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,0,0,0,0,0]");

    TRY(a.shrink_to_fit());
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 10u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42,42,0,0,0,0,0]");

    TRY(a.resize(4));
    TEST(! a.empty());
    TEST(! a.is_compact());
    TEST_EQUAL(a.size(), 4u);
    TEST_EQUAL(a.capacity(), 10u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42]");

    TRY(a.shrink_to_fit());
    TEST(! a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 4u);
    TEST_EQUAL(a.capacity(), 5u);
    TRY(s = format_range(a));
    TEST_EQUAL(s, "[42,42,42,42]");

    TRY(a.clear());
    TEST(a.empty());
    TEST(a.is_compact());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 5u);

}

void test_crow_compact_array_keys() {

    using ICA = CompactArray<int, 5>;
    using Omap = std::map<ICA, std::string>;
    using Umap = std::unordered_map<ICA, std::string>;

    ICA a;
    Omap omap;
    Umap umap;
    std::string s;

    for (int i = 5; i >= 1; --i) {
        TRY(a.clear());
        for (int j = i; j <= 2 * i; ++j)
            TRY(a.push_back(j));
        TEST_EQUAL(a.size(), size_t(i + 1));
        s.assign(i, char('@' + i));
        TRY(omap[a] = s);
        TRY(umap[a] = s);
    }

    TEST_EQUAL(omap.size(), 5u);
    TEST_EQUAL(umap.size(), 5u);

    auto it = omap.begin();

    TEST_EQUAL(format_range(it->first), "[1,2]");           TEST_EQUAL(it->second, "A");      ++it;
    TEST_EQUAL(format_range(it->first), "[2,3,4]");         TEST_EQUAL(it->second, "BB");     ++it;
    TEST_EQUAL(format_range(it->first), "[3,4,5,6]");       TEST_EQUAL(it->second, "CCC");    ++it;
    TEST_EQUAL(format_range(it->first), "[4,5,6,7,8]");     TEST_EQUAL(it->second, "DDDD");   ++it;
    TEST_EQUAL(format_range(it->first), "[5,6,7,8,9,10]");  TEST_EQUAL(it->second, "EEEEE");  ++it;

    TEST(it == omap.end());

}
