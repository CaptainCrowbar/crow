#include "crow/spatial-index.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using namespace Crow;

namespace {

    template <RegularHashable T, ArithmeticType S, int D>
    std::string dump(const SpatialIndex<T, S, D>& space) {
        std::vector<std::string> ss;
        for (auto& [k,v]: space)
            ss.push_back(fmt("{0:*Z} {1}\n", k, v));
        std::sort(ss.begin(), ss.end());
        return join(ss);
    }

    using Index = SpatialIndex<std::string, double, 2>;

}

void test_crow_spatial_index_construction() {

    Index space(10);
    Index::iterator i;
    bool b = false;

    TEST_EQUAL(space.scale(), 10);
    TEST(space.empty());
    TEST_EQUAL(space.size(), 0u);
    TEST(! space.contains("alpha"));
    TEST_EQUAL(dump(space), "");

    TRY(std::tie(i, b) = space.insert("alpha", {5, 10}));
    TEST(b);
    TEST(! space.empty());
    TEST_EQUAL(space.size(), 1u);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "alpha");
    TEST_EQUAL(i->second, Double2(5, 10));
    TEST(space.contains("alpha"));
    TEST_EQUAL(dump(space), "alpha [5,10]\n");

    TRY(space.clear());
    TEST(space.empty());
    TEST_EQUAL(space.size(), 0u);
    TEST(! space.contains("alpha"));
    TEST_EQUAL(dump(space), "");

}

void test_crow_spatial_index_insert() {

    Index space(10);
    Index::iterator i;
    bool b = false;

    TRY(std::tie(i, b) = space.insert("alpha",    {5, 0}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "alpha");
    TRY(std::tie(i, b) = space.insert("bravo",    {5, 5}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "bravo");
    TRY(std::tie(i, b) = space.insert("charlie",  {5, 10}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "charlie");
    TRY(std::tie(i, b) = space.insert("delta",    {5, 15}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "delta");
    TRY(std::tie(i, b) = space.insert("echo",     {5, 20}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "echo");

    TEST_EQUAL(space.size(), 5u);
    TEST(space.contains("alpha"));
    TEST(space.contains("bravo"));
    TEST(space.contains("charlie"));
    TEST(space.contains("delta"));
    TEST(space.contains("echo"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
        "echo [5,20]\n"
    );

    TEST_THROW(space.insert("zulu", {1e50, 0}), std::out_of_range);

    TRY(std::tie(i, b) = space.insert("alpha", {5, 0}));
    TEST(! b);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "alpha");
    TEST_EQUAL(i->second, Double2(5, 0));
    TEST_EQUAL(space.size(), 5u);

    // Update with unchanged position

    TRY(std::tie(i, b) = space.update("alpha", {5, 0}));
    TEST(! b);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "alpha");
    TEST_EQUAL(i->second, Double2(5, 0));
    TEST_EQUAL(space.size(), 5u);

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
        "echo [5,20]\n"
    );

    // Update within the same cell

    TRY(std::tie(i, b) = space.update("alpha", {6, 1}));
    TEST(! b);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "alpha");
    TEST_EQUAL(i->second, Double2(6, 1));
    TEST_EQUAL(space.size(), 5u);

    TEST_EQUAL(dump(space),
        "alpha [6,1]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
        "echo [5,20]\n"
    );

    // Move to a new cell

    TRY(std::tie(i, b) = space.update("alpha", {5, 30}));
    TEST(! b);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "alpha");
    TEST_EQUAL(i->second, Double2(5, 30));
    TEST_EQUAL(space.size(), 5u);

    TEST_EQUAL(dump(space),
        "alpha [5,30]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
        "echo [5,20]\n"
    );

    // New object

    TRY(std::tie(i, b) = space.update("foxtrot", {5, 25}));
    TEST(b);
    REQUIRE(i != space.end());
    TEST_EQUAL(i->first, "foxtrot");
    TEST_EQUAL(i->second, Double2(5, 25));
    TEST_EQUAL(space.size(), 6u);

    TEST_EQUAL(dump(space),
        "alpha [5,30]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
        "echo [5,20]\n"
        "foxtrot [5,25]\n"
    );

}

void test_crow_spatial_index_find() {

    Index space(10);
    Index::iterator i;
    bool b = false;

    TRY(std::tie(i, b) = space.insert("alpha",    {5, 0}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "alpha");
    TRY(std::tie(i, b) = space.insert("bravo",    {5, 5}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "bravo");
    TRY(std::tie(i, b) = space.insert("charlie",  {5, 10}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "charlie");
    TRY(std::tie(i, b) = space.insert("delta",    {5, 15}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "delta");

    TEST_EQUAL(space.size(), 4u);
    TEST(space.contains("alpha"));
    TEST(space.contains("bravo"));
    TEST(space.contains("charlie"));
    TEST(space.contains("delta"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
    );

    TRY(i = space.find("alpha"));    REQUIRE(i != space.end());  TEST_EQUAL(i->first, "alpha");    TEST_EQUAL(i->second, Double2(5, 0));
    TRY(i = space.find("bravo"));    REQUIRE(i != space.end());  TEST_EQUAL(i->first, "bravo");    TEST_EQUAL(i->second, Double2(5, 5));
    TRY(i = space.find("charlie"));  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "charlie");  TEST_EQUAL(i->second, Double2(5, 10));
    TRY(i = space.find("delta"));    REQUIRE(i != space.end());  TEST_EQUAL(i->first, "delta");    TEST_EQUAL(i->second, Double2(5, 15));

    TRY(i = space.find(""));      TEST(i == space.end());
    TRY(i = space.find("echo"));  TEST(i == space.end());

}

void test_crow_spatial_index_erase() {

    Index space(10);
    Index::iterator i;
    bool b = false;

    TRY(std::tie(i, b) = space.insert("alpha",    {5, 0}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "alpha");
    TRY(std::tie(i, b) = space.insert("bravo",    {5, 5}));   TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "bravo");
    TRY(std::tie(i, b) = space.insert("charlie",  {5, 10}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "charlie");
    TRY(std::tie(i, b) = space.insert("delta",    {5, 15}));  TEST(b);  REQUIRE(i != space.end());  TEST_EQUAL(i->first, "delta");

    TEST_EQUAL(space.size(), 4u);
    TEST(space.contains("alpha"));
    TEST(space.contains("bravo"));
    TEST(space.contains("charlie"));
    TEST(space.contains("delta"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
    );

    TEST(! space.erase("echo"));
    TEST_EQUAL(space.size(), 4u);
    TEST(space.contains("alpha"));
    TEST(space.contains("bravo"));
    TEST(space.contains("charlie"));
    TEST(space.contains("delta"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "charlie [5,10]\n"
        "delta [5,15]\n"
    );

    TEST(space.erase("charlie"));
    TEST_EQUAL(space.size(), 3u);
    TEST(space.contains("alpha"));
    TEST(space.contains("bravo"));
    TEST(! space.contains("charlie"));
    TEST(space.contains("delta"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "bravo [5,5]\n"
        "delta [5,15]\n"
    );

    TRY(i = space.find("bravo"));
    TRY(space.erase(i));
    TEST_EQUAL(space.size(), 2u);
    TEST(space.contains("alpha"));
    TEST(! space.contains("bravo"));
    TEST(! space.contains("charlie"));
    TEST(space.contains("delta"));

    TEST_EQUAL(dump(space),
        "alpha [5,0]\n"
        "delta [5,15]\n"
    );

}

void test_crow_spatial_index_search() {

    Index space(10);
    Index::iterator it;
    std::vector<Index::iterator> its;
    Double2 pos(10, 10);
    Double2 outside(1e20, 0);

    auto hits = [&its] () {
        std::string s;
        std::transform(its.begin(), its.end(), append(s), [] (auto& it) { return it->first[0]; });
        std::sort(s.begin(), s.end());
        return s;
    };

    // Object                    Position     From alpha  From (10,10)
    TRY(space.insert("alpha",    {5, 0}));    // 0        11.18
    TRY(space.insert("bravo",    {5, 5}));    // 5        7.07
    TRY(space.insert("charlie",  {5, 10}));   // 10       5
    TRY(space.insert("delta",    {5, 15}));   // 15       7.07
    TRY(space.insert("echo",     {5, 20}));   // 20       11.18
    TRY(space.insert("foxtrot",  {15, 0}));   // 10       11.18
    TRY(space.insert("golf",     {15, 10}));  // 14.14    5
    TRY(space.insert("hotel",    {15, 20}));  // 22.36    11.18
    TRY(space.insert("india",    {-10, 0}));  // 15       22.36
    TRY(space.insert("juliet",   {20, 20}));  // 25       14.14

    TRY(it = space.find("alpha"));
    REQUIRE(it != space.end());

    TRY(its = space.search(pos, 1));         TEST_EQUAL(its.size(), 0u);   TEST_EQUAL(hits(), "");
    TRY(its = space.search(pos, 6));         TEST_EQUAL(its.size(), 2u);   TEST_EQUAL(hits(), "cg");
    TRY(its = space.search(pos, 11));        TEST_EQUAL(its.size(), 4u);   TEST_EQUAL(hits(), "bcdg");
    TRY(its = space.search(pos, 16));        TEST_EQUAL(its.size(), 9u);   TEST_EQUAL(hits(), "abcdefghj");
    TRY(its = space.search("alpha", 1));     TEST_EQUAL(its.size(), 1u);   TEST_EQUAL(hits(), "a");
    TRY(its = space.search("alpha", 6));     TEST_EQUAL(its.size(), 2u);   TEST_EQUAL(hits(), "ab");
    TRY(its = space.search("alpha", 11));    TEST_EQUAL(its.size(), 4u);   TEST_EQUAL(hits(), "abcf");
    TRY(its = space.search("alpha", 16));    TEST_EQUAL(its.size(), 7u);   TEST_EQUAL(hits(), "abcdfgi");
    TRY(its = space.search("zulu", 1));      TEST_EQUAL(its.size(), 0u);   TEST_EQUAL(hits(), "");
    TRY(its = space.search(it, 1));          TEST_EQUAL(its.size(), 1u);   TEST_EQUAL(hits(), "a");
    TRY(its = space.search(it, 6));          TEST_EQUAL(its.size(), 2u);   TEST_EQUAL(hits(), "ab");
    TRY(its = space.search(it, 11));         TEST_EQUAL(its.size(), 4u);   TEST_EQUAL(hits(), "abcf");
    TRY(its = space.search(it, 16));         TEST_EQUAL(its.size(), 7u);   TEST_EQUAL(hits(), "abcdfgi");
    TRY(its = space.search(outside, 1e10));  TEST_EQUAL(its.size(), 0u);   TEST_EQUAL(hits(), "");
    TRY(its = space.search(outside, 1e30));  TEST_EQUAL(its.size(), 10u);  TEST_EQUAL(hits(), "abcdefghij");

}
