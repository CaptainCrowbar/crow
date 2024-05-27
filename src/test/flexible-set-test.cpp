#include "crow/flexible-map.hpp"
#include "crow/format.hpp"
#include "crow/iterator.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <compare>
#include <functional>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace Crow;

namespace {

    struct HashKey {
        int key;
        HashKey(int k): key(k) {}
        bool operator==(const HashKey& k) const noexcept { return key == k.key; }
    };

    struct LinearKey {
        int key;
        LinearKey(int k): key(k) {}
        bool operator==(const LinearKey& k) const noexcept { return key == k.key; }
    };

    std::ostream& operator<<(std::ostream& out, const HashKey& k) { return out << k.key; }
    std::ostream& operator<<(std::ostream& out, const LinearKey& k) { return out << k.key; }

    struct CompareKeys {
        template <typename K> bool operator()(K a, K b) const { return a.key < b.key; }
    };

    template <Setlike S>
    std::string format_set(const S& s) {
        if constexpr (Detail::SetlikeMode<S>::value == FlexImpl::ordered) {
            return format_range(s);
        } else {
            auto v = std::vector(s.begin(), s.end());
            std::sort(v.begin(), v.end(), [] (auto a, auto b) { return a.key < b.key; });
            return format_range(v);
        }
    }

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

void test_crow_flexible_set_ordered() {

    using FS = FlexibleSet<int>;

    FS set, set2;
    FS::iterator i;
    std::string str;

    TEST_EQUAL(set.impl, FlexImpl::ordered);
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

    TRY(std::copy(v.begin(), v.end(), overwrite(set)));
    TEST_EQUAL(set.size(), 2u);
    TEST(! set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY((set2 = {1, 2, 3}));
    TRY(set.swap(set2));
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set2.contains(5));
    TEST(set2.contains(6));

    TRY((set = {1, 2, 3, 4, 5}));
    TRY(str = format_set(set));
    TEST_EQUAL(str, "[1,2,3,4,5]");

}

void test_crow_flexible_set_hash() {

    using FS = FlexibleSet<HashKey>;

    FS set, set2;
    FS::iterator i;
    std::string str;

    TEST_EQUAL(set.impl, FlexImpl::hash);
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
    TEST_EQUAL(i->key, 1);

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
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);

    std::vector<HashKey> v = {5, 6};

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
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);
    TRY(i = set.find(5));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 5);
    TRY(i = set.find(6));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 6);

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

    TRY(std::copy(v.begin(), v.end(), overwrite(set)));
    TEST_EQUAL(set.size(), 2u);
    TEST(! set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY((set2 = {1, 2, 3}));
    TRY(set.swap(set2));
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set2.contains(5));
    TEST(set2.contains(6));

    TRY((set = {1, 2, 3, 4, 5}));
    TRY(str = format_set(set));
    TEST_EQUAL(str, "[1,2,3,4,5]");

}

void test_crow_flexible_set_linear() {

    using FS = FlexibleSet<LinearKey>;

    FS set, set2;
    FS::iterator i;
    std::string str;

    TEST_EQUAL(set.impl, FlexImpl::linear);
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
    TEST_EQUAL(i->key, 1);

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
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);

    std::vector<LinearKey> v = {5, 6};

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
    TEST_EQUAL(i->key, 1);
    TRY(i = set.find(2));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 2);
    TRY(i = set.find(3));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 3);
    TRY(i = set.find(4));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 4);
    TRY(i = set.find(5));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 5);
    TRY(i = set.find(6));
    TEST(i != set.end());
    TEST_EQUAL(i->key, 6);

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

    TRY(std::copy(v.begin(), v.end(), overwrite(set)));
    TEST_EQUAL(set.size(), 2u);
    TEST(! set.contains(4));
    TEST(set.contains(5));
    TEST(set.contains(6));
    TEST(! set.contains(7));

    TRY((set2 = {1, 2, 3}));
    TRY(set.swap(set2));
    TEST(set.contains(1));
    TEST(set.contains(2));
    TEST(set.contains(3));
    TEST(set2.contains(5));
    TEST(set2.contains(6));

    TRY((set = {1, 2, 3, 4, 5}));
    TRY(str = format_set(set));
    TEST_EQUAL(str, "[1,2,3,4,5]");

}

void test_crow_flexible_set_concepts() {

    TEST_EQUAL((Detail::SetlikeMode<std::vector<int>>::value),                  FlexImpl::none);
    TEST_EQUAL((Detail::SetlikeMode<std::set<int>>::value),                     FlexImpl::ordered);
    TEST_EQUAL((Detail::SetlikeMode<std::map<int, int>>::value),                FlexImpl::none);
    TEST_EQUAL((Detail::SetlikeMode<std::unordered_set<int>>::value),           FlexImpl::hash);
    TEST_EQUAL((Detail::SetlikeMode<std::unordered_map<int, int>>::value),      FlexImpl::none);
    TEST_EQUAL((Detail::SetlikeMode<FlexibleSet<int>>::value),                  FlexImpl::ordered);
    TEST_EQUAL((Detail::SetlikeMode<FlexibleSet<int, FlexImpl::hash>>::value),  FlexImpl::hash);
    TEST_EQUAL((Detail::SetlikeMode<FlexibleSet<HashKey>>::value),              FlexImpl::hash);
    TEST_EQUAL((Detail::SetlikeMode<FlexibleSet<LinearKey>>::value),            FlexImpl::linear);

    TEST((! Setlike<std::vector<int>>));
    TEST((Setlike<std::set<int>>));
    TEST((! Setlike<std::map<int, int>>));
    TEST((Setlike<std::unordered_set<int>>));
    TEST((! Setlike<std::unordered_map<int, int>>));
    TEST((Setlike<FlexibleSet<int>>));
    TEST((Setlike<FlexibleSet<int, FlexImpl::hash>>));
    TEST((Setlike<FlexibleSet<HashKey>>));
    TEST((Setlike<FlexibleSet<LinearKey>>));

}

void test_crow_flexible_set_union() {

    using HS = FlexibleSet<HashKey>;
    using LS = FlexibleSet<LinearKey>;
    using OS = FlexibleSet<int>;

    HS h1 = {1, 2, 3, 4}, h2 = {3, 4, 5}, h3;
    LS l1 = {1, 2, 3, 4}, l2 = {3, 4, 5}, l3;
    OS o1 = {1, 2, 3, 4}, o2 = {3, 4, 5}, o3;
    std::string s, expect = "[1,2,3,4,5]";

    TRY(set_union(h1, h2, h3));   TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(set_union(l1, l2, l3));   TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(set_union(o1, o2, o3));   TRY(s = format_set(o3));  TEST_EQUAL(s, expect);
    TRY(h3 = set_union(h1, h2));  TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(l3 = set_union(l1, l2));  TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(o3 = set_union(o1, o2));  TRY(s = format_set(o3));  TEST_EQUAL(s, expect);

}

void test_crow_flexible_set_intersection() {

    using HS = FlexibleSet<HashKey>;
    using LS = FlexibleSet<LinearKey>;
    using OS = FlexibleSet<int>;

    HS h1 = {1, 2, 3, 4}, h2 = {3, 4, 5}, h3;
    LS l1 = {1, 2, 3, 4}, l2 = {3, 4, 5}, l3;
    OS o1 = {1, 2, 3, 4}, o2 = {3, 4, 5}, o3;
    std::string s, expect = "[3,4]";

    TRY(set_intersection(h1, h2, h3));   TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(set_intersection(l1, l2, l3));   TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(set_intersection(o1, o2, o3));   TRY(s = format_set(o3));  TEST_EQUAL(s, expect);
    TRY(h3 = set_intersection(h1, h2));  TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(l3 = set_intersection(l1, l2));  TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(o3 = set_intersection(o1, o2));  TRY(s = format_set(o3));  TEST_EQUAL(s, expect);

}

void test_crow_flexible_set_difference() {

    using HS = FlexibleSet<HashKey>;
    using LS = FlexibleSet<LinearKey>;
    using OS = FlexibleSet<int>;

    HS h1 = {1, 2, 3, 4}, h2 = {3, 4, 5}, h3;
    LS l1 = {1, 2, 3, 4}, l2 = {3, 4, 5}, l3;
    OS o1 = {1, 2, 3, 4}, o2 = {3, 4, 5}, o3;
    std::string s, expect = "[1,2]";

    TRY(set_difference(h1, h2, h3));   TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(set_difference(l1, l2, l3));   TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(set_difference(o1, o2, o3));   TRY(s = format_set(o3));  TEST_EQUAL(s, expect);
    TRY(h3 = set_difference(h1, h2));  TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(l3 = set_difference(l1, l2));  TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(o3 = set_difference(o1, o2));  TRY(s = format_set(o3));  TEST_EQUAL(s, expect);

}

void test_crow_flexible_set_symmetric_difference() {

    using HS = FlexibleSet<HashKey>;
    using LS = FlexibleSet<LinearKey>;
    using OS = FlexibleSet<int>;

    HS h1 = {1, 2, 3, 4}, h2 = {3, 4, 5}, h3;
    LS l1 = {1, 2, 3, 4}, l2 = {3, 4, 5}, l3;
    OS o1 = {1, 2, 3, 4}, o2 = {3, 4, 5}, o3;
    std::string s, expect = "[1,2,5]";

    TRY(set_symmetric_difference(h1, h2, h3));   TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(set_symmetric_difference(l1, l2, l3));   TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(set_symmetric_difference(o1, o2, o3));   TRY(s = format_set(o3));  TEST_EQUAL(s, expect);
    TRY(h3 = set_symmetric_difference(h1, h2));  TRY(s = format_set(h3));  TEST_EQUAL(s, expect);
    TRY(l3 = set_symmetric_difference(l1, l2));  TRY(s = format_set(l3));  TEST_EQUAL(s, expect);
    TRY(o3 = set_symmetric_difference(o1, o2));  TRY(s = format_set(o3));  TEST_EQUAL(s, expect);

}
