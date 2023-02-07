#include "crow/flexible-map.hpp"
#include "crow/format.hpp"
#include "crow/iterator.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

using namespace Crow;

namespace {

    struct HashKey {
        int key;
        bool operator==(const HashKey& k) const noexcept = default;
    };

    struct LinearKey {
        int key;
        bool operator==(const LinearKey& k) const noexcept = default;
    };

}

namespace std {

    template <>
    class hash<HashKey> {
    public:
        size_t operator()(const HashKey& k) const noexcept {
            return std::hash<int>()(k.key);
        }
    };

}

void test_crow_flexible_map_order() {

    using FM = FlexibleMap<int, std::string>;

    FM map;
    FM::const_iterator i;

    TEST_EQUAL(map.impl, FlexImpl::order);
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY(map.insert({1, "aa"}));
    TEST(! map.empty());
    TEST_EQUAL(map.size(), 1u);
    TEST(! map.contains(0));
    TEST(map.contains(1));
    TEST(! map.contains(2));
    TRY(i = map.find(0));
    TEST(i == map.end());
    TRY(i = map.find(1));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 1);
    TEST_EQUAL(i->second, "aa");

    TRY(map.insert({{2, "bb"}, {3, "cc"}, {4, "dd"}}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains(0));
    TEST(map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(map.contains(4));
    TEST(! map.contains(5));
    TRY(i = map.find(0));
    TEST(i == map.end());
    TRY(i = map.find(1));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find(2));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find(3));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find(4));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 4);
    TEST_EQUAL(i->second, "dd");

    std::vector<std::pair<int, std::string>> v = {{5, "ee"}, {6, "ff"}};

    TRY(map.insert(v.begin(), v.end()));
    TEST_EQUAL(map.size(), 6u);
    TEST(! map.contains(0));
    TEST(map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));
    TRY(i = map.find(0));
    TEST(i == map.end());
    TRY(i = map.find(1));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find(2));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find(3));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find(4));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 4);
    TEST_EQUAL(i->second, "dd");
    TRY(i = map.find(5));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 5);
    TEST_EQUAL(i->second, "ee");
    TRY(i = map.find(6));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 6);
    TEST_EQUAL(i->second, "ff");

    TRY(i = map.find(1));
    TRY(map.erase(i));
    TEST_EQUAL(map.size(), 5u);
    TEST(! map.contains(0));
    TEST(! map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));

    TRY(map.erase(4));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains(0));
    TEST(! map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(! map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));

    TRY(map.erase(9));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains(0));
    TEST(! map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(! map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));

    TEST_EQUAL(map[2], "bb");
    TEST_EQUAL(map.size(), 4u);
    TEST_EQUAL(map[8], "");
    TEST_EQUAL(map.size(), 5u);
    TRY(map[2] = "BB");
    TEST_EQUAL(map.size(), 5u);
    TRY(i = map.find(2));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 2);
    TEST_EQUAL(i->second, "BB");
    TRY(map[9] = "II");
    TEST_EQUAL(map.size(), 6u);
    TRY(i = map.find(9));
    TEST(i != map.end());
    TEST_EQUAL(i->first, 9);
    TEST_EQUAL(i->second, "II");

    TRY(map.clear());
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY((map = {{1, "xx"}, {2, "yy"}, {3, "zz"}}));
    TEST_EQUAL(map.size(), 3u);
    TEST(map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST_EQUAL(map[1], "xx");
    TEST_EQUAL(map[2], "yy");
    TEST_EQUAL(map[3], "zz");

    TRY(std::copy(v.begin(), v.end(), overwrite(map)));
    TEST_EQUAL(map.size(), 2u);
    TEST(! map.contains(4));
    TEST(map.contains(5));
    TEST(map.contains(6));
    TEST(! map.contains(7));

}

void test_crow_flexible_map_hash() {

    using FM = FlexibleMap<HashKey, std::string>;

    FM map;
    FM::const_iterator i;

    TEST_EQUAL(map.impl, FlexImpl::hash);
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY(map.insert({{1}, "aa"}));
    TEST(! map.empty());
    TEST_EQUAL(map.size(), 1u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(! map.contains({2}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");

    TRY(map.insert({{{2}, "bb"}, {{3}, "cc"}, {{4}, "dd"}}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(! map.contains({5}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find({3}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find({4}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 4);
    TEST_EQUAL(i->second, "dd");

    std::vector<std::pair<HashKey, std::string>> v = {{{5}, "ee"}, {{6}, "ff"}};

    TRY(map.insert(v.begin(), v.end()));
    TEST_EQUAL(map.size(), 6u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find({3}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find({4}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 4);
    TEST_EQUAL(i->second, "dd");
    TRY(i = map.find({5}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 5);
    TEST_EQUAL(i->second, "ee");
    TRY(i = map.find({6}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 6);
    TEST_EQUAL(i->second, "ff");

    TRY(i = map.find({1}));
    TRY(map.erase(i));
    TEST_EQUAL(map.size(), 5u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TRY(map.erase({4}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TRY(map.erase({9}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TEST_EQUAL(map[{2}], "bb");
    TEST_EQUAL(map.size(), 4u);
    TEST_EQUAL(map[{8}], "");
    TEST_EQUAL(map.size(), 5u);
    TRY(map[{2}] = "BB");
    TEST_EQUAL(map.size(), 5u);
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "BB");
    TRY(map[{9}] = "II");
    TEST_EQUAL(map.size(), 6u);
    TRY(i = map.find({9}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 9);
    TEST_EQUAL(i->second, "II");

    TRY(map.clear());
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY((map = {{{1}, "xx"}, {{2}, "yy"}, {{3}, "zz"}}));
    TEST_EQUAL(map.size(), 3u);
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST_EQUAL(map[{1}], "xx");
    TEST_EQUAL(map[{2}], "yy");
    TEST_EQUAL(map[{3}], "zz");

    TRY(std::copy(v.begin(), v.end(), overwrite(map)));
    TEST_EQUAL(map.size(), 2u);
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

}

void test_crow_flexible_map_linear() {

    using FM = FlexibleMap<LinearKey, std::string>;

    FM map;
    FM::const_iterator i;

    TEST_EQUAL(map.impl, FlexImpl::linear);
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY(map.insert({{1}, "aa"}));
    TEST(! map.empty());
    TEST_EQUAL(map.size(), 1u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(! map.contains({2}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");

    TRY(map.insert({{{2}, "bb"}, {{3}, "cc"}, {{4}, "dd"}}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(! map.contains({5}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find({3}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find({4}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 4);
    TEST_EQUAL(i->second, "dd");

    std::vector<std::pair<LinearKey, std::string>> v = {{{5}, "ee"}, {{6}, "ff"}};

    TRY(map.insert(v.begin(), v.end()));
    TEST_EQUAL(map.size(), 6u);
    TEST(! map.contains({0}));
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));
    TRY(i = map.find({0}));
    TEST(i == map.end());
    TRY(i = map.find({1}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 1);
    TEST_EQUAL(i->second, "aa");
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "bb");
    TRY(i = map.find({3}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 3);
    TEST_EQUAL(i->second, "cc");
    TRY(i = map.find({4}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 4);
    TEST_EQUAL(i->second, "dd");
    TRY(i = map.find({5}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 5);
    TEST_EQUAL(i->second, "ee");
    TRY(i = map.find({6}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 6);
    TEST_EQUAL(i->second, "ff");

    TRY(i = map.find({1}));
    TRY(map.erase(i));
    TEST_EQUAL(map.size(), 5u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TRY(map.erase({4}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TRY(map.erase({9}));
    TEST_EQUAL(map.size(), 4u);
    TEST(! map.contains({0}));
    TEST(! map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

    TEST_EQUAL(map[{2}], "bb");
    TEST_EQUAL(map.size(), 4u);
    TEST_EQUAL(map[{8}], "");
    TEST_EQUAL(map.size(), 5u);
    TRY(map[{2}] = "BB");
    TEST_EQUAL(map.size(), 5u);
    TRY(i = map.find({2}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 2);
    TEST_EQUAL(i->second, "BB");
    TRY(map[{9}] = "II");
    TEST_EQUAL(map.size(), 6u);
    TRY(i = map.find({9}));
    TEST(i != map.end());
    TEST_EQUAL(i->first.key, 9);
    TEST_EQUAL(i->second, "II");

    TRY(map.clear());
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY((map = {{{1}, "xx"}, {{2}, "yy"}, {{3}, "zz"}}));
    TEST_EQUAL(map.size(), 3u);
    TEST(map.contains({1}));
    TEST(map.contains({2}));
    TEST(map.contains({3}));
    TEST_EQUAL(map[{1}], "xx");
    TEST_EQUAL(map[{2}], "yy");
    TEST_EQUAL(map[{3}], "zz");

    TRY(std::copy(v.begin(), v.end(), overwrite(map)));
    TEST_EQUAL(map.size(), 2u);
    TEST(! map.contains({4}));
    TEST(map.contains({5}));
    TEST(map.contains({6}));
    TEST(! map.contains({7}));

}
