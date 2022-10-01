#include "crow/lru-cache.hpp"
#include "crow/unit-test.hpp"
#include <compare>
#include <string>

using namespace Crow;

namespace {

    template <typename T>
    struct NoHash {
        T value;
        auto operator<=>(const NoHash& rhs) const = default;
    };

    template <typename T>
    struct NoHashOrder {
        T value;
    };

}

void test_crow_lru_cache_traits() {

    TEST_EQUAL(cache_type<int>,               CacheType::hashed);
    TEST_EQUAL(cache_type<std::string>,       CacheType::hashed);
    TEST_EQUAL(cache_type<NoHash<int>>,       CacheType::ordered);
    TEST_EQUAL(cache_type<NoHashOrder<int>>,  CacheType::none);

}

void test_crow_lru_cache_hashed() {

    LruCache<int, std::string> cache;

    // TODO

}

void test_crow_lru_cache_ordered() {

    LruCache<NoHash<int>, std::string> cache;

    // TODO

}
