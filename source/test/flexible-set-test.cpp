#include "crow/flexible-map.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <functional>
#include <string>
#include <vector>

using namespace Crow;

namespace {

    struct HashKey {
        int key;
        bool operator==(const HashKey& k) const noexcept { return key == k.key; }
    };

    struct LinearKey {
        int key;
        bool operator==(const LinearKey& k) const noexcept { return key == k.key; }
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

void test_crow_flexible_set_order() {

    using FS = FlexibleSet<int>;

    FS set;
    FS::iterator i;

    TEST_EQUAL(set.impl, FlexImpl::order);
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY(set.insert(1));
    TEST(! set.empty());
    TEST_EQUAL(set.size(), 1u);
    TEST(! set.contains(0));
    TEST(set.contains(1));
    TEST(! set.contains(2));
    TRY(i = set.find(0));
    TEST(i == set.end());
    TRY(i = set.find(1));
    TEST(i != set.end());
    TEST_EQUAL(*i, 1);

    TRY(set.insert({2, 3, 4}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains(0));
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set.contains(4));
    TEST(! set.contains(5));
    TRY(i = set.find(0));
    TEST(i == set.end());
    TRY(i = set.find(1));
    TEST(i != set.end());
    TEST_EQUAL(*i, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(*i, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(*i, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(*i, 4);

    std::vector<int> v = {5, 6};

    TRY(set.insert(v.begin(), v.end()));
    TEST_EQUAL(set.size(), 6u);
    TEST(! set.contains(0));
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));
    TRY(i = set.find(0));
    TEST(i == set.end());
    TRY(i = set.find(1));
    TEST(i != set.end());
    TEST_EQUAL(*i, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(*i, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(*i, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(*i, 4);
    TRY(i = set.find(5));
    TEST(i != set.end());
    TEST_EQUAL(*i, 5);
    TRY(i = set.find(6));
    TEST(i != set.end());
    TEST_EQUAL(*i, 6);

    TRY(i = set.find(1));
    TRY(set.erase(i));
    TEST_EQUAL(set.size(), 5u);
    TEST(! set.contains(0));
    TEST(! set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY(set.erase(4));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains(0));
    TEST(! set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(! set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY(set.erase(9));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains(0));
    TEST(! set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(! set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY(set.clear());
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY((set = {1, 2, 3}));
    TEST_EQUAL(set.size(), 3u);
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));

}

void test_crow_flexible_set_hash() {

    using FS = FlexibleSet<HashKey>;

    FS set;
    FS::iterator i;

    TEST_EQUAL(set.impl, FlexImpl::hash);
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY(set.insert({1}));
    TEST(! set.empty());
    TEST_EQUAL(set.size(), 1u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(! set.contains({2}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);

    TRY(set.insert({{2}, {3}, {4}}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(! set.contains({5}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find({2}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find({3}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find({4}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);

    std::vector<HashKey> v = {{5}, {6}};

    TRY(set.insert(v.begin(), v.end()));
    TEST_EQUAL(set.size(), 6u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find({2}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find({3}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find({4}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);
    TRY(i = set.find({5}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 5);
    TRY(i = set.find({6}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 6);

    TRY(i = set.find({1}));
    TRY(set.erase(i));
    TEST_EQUAL(set.size(), 5u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.erase({4}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(! set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.erase({9}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(! set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.clear());
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY((set = {{1}, {2}, {3}}));
    TEST_EQUAL(set.size(), 3u);
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));

}

void test_crow_flexible_set_linear() {

    using FS = FlexibleSet<LinearKey>;

    FS set;
    FS::iterator i;

    TEST_EQUAL(set.impl, FlexImpl::linear);
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY(set.insert({1}));
    TEST(! set.empty());
    TEST_EQUAL(set.size(), 1u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(! set.contains({2}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);

    TRY(set.insert({{2}, {3}, {4}}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(! set.contains({5}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find({2}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find({3}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find({4}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);

    std::vector<LinearKey> v = {{5}, {6}};

    TRY(set.insert(v.begin(), v.end()));
    TEST_EQUAL(set.size(), 6u);
    TEST(! set.contains({0}));
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));
    TRY(i = set.find({0}));
    TEST(i == set.end());
    TRY(i = set.find({1}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find({2}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find({3}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find({4}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);
    TRY(i = set.find({5}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 5);
    TRY(i = set.find({6}));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 6);

    TRY(i = set.find({1}));
    TRY(set.erase(i));
    TEST_EQUAL(set.size(), 5u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.erase({4}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(! set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.erase({9}));
    TEST_EQUAL(set.size(), 4u);
    TEST(! set.contains({0}));
    TEST(! set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));
    TEST(! set.contains({4}));
    TEST(set.contains({5}));
    TEST(set.contains({6}));
    TEST(! set.contains({7}));

    TRY(set.clear());
    TEST(set.empty());
    TEST_EQUAL(set.size(), 0u);

    TRY((set = {{1}, {2}, {3}}));
    TEST_EQUAL(set.size(), 3u);
    TEST(set.contains({1}));
    TEST(set.contains({2}));
    TEST(set.contains({3}));

}
