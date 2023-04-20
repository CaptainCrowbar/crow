#include "crow/bounded-array.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace Crow;
using namespace Crow::UnitTest;

void test_crow_bounded_array_capacity() {

    using IBA = BoundedArray<int, 10>;

    IBA a;

    TRY(a.resize(5, 42));
    TEST(! a.empty());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 10u);
    TEST_EQUAL(format_range(a), "[42,42,42,42,42]");

    TEST_THROW(a.resize(20), std::length_error);
    TEST(! a.empty());
    TEST_EQUAL(a.size(), 5u);
    TEST_EQUAL(a.capacity(), 10u);
    TEST_EQUAL(format_range(a), "[42,42,42,42,42]");

    TRY(a.resize(10));
    TEST(! a.empty());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 10u);
    TEST_EQUAL(format_range(a), "[42,42,42,42,42,0,0,0,0,0]");

    TEST_THROW(a.push_back(99), std::length_error);
    TEST(! a.empty());
    TEST_EQUAL(a.size(), 10u);
    TEST_EQUAL(a.capacity(), 10u);
    TEST_EQUAL(format_range(a), "[42,42,42,42,42,0,0,0,0,0]");

    TRY(a.clear());
    TEST(a.empty());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(a.capacity(), 10u);
    TEST_EQUAL(format_range(a), "[]");

}



void test_crow_bounded_array_keys() {

    using IBA = BoundedArray<int, 10>;
    using Omap = std::map<IBA, std::string>;
    using Umap = std::unordered_map<IBA, std::string>;

    IBA a;
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
