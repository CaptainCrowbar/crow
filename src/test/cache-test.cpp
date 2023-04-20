#include "crow/cache.hpp"
#include "crow/format.hpp"
#include "crow/hash.hpp"
#include "crow/unit-test.hpp"
#include <compare>
#include <optional>
#include <string>

using namespace Crow;

namespace Crow {

    class DebugCache {
    public:
        template <typename K, typename T>
        std::string operator()(const LruCache<K, T>& c) const {
            std::string s = fmt("LruCache\n    list [{0}]\n", c.list_.size());
            for (auto& [k,v]: c.list_) {
                s += fmt("        {0}: {1}\n", k, v);
            }
            s += fmt("    map [{0}]\n", c.map_.size());
            if constexpr (Hashable<K>) {
                for (auto& [k,v]: c.list_) {
                    auto m = c.map_.find(k);
                    if (m == c.map_.end())
                        s += fmt("        {0}: not found\n", k);
                    else
                        s += fmt("        {0}: {1}, {2}\n", k, m->second->key, m->second->value);
                }
            } else {
                for (auto& [k,v]: c.map_)
                    s += fmt("        {0}: {1}, {2}\n", k, v->key, v->value);
            }
            return s;
        }
    };

    const DebugCache dump;

    int calls = 0;

    std::string alpha(int n) {
        ++calls;
        return std::string(size_t(n), char('a' + n - 1));
    }

    struct Alpha {
        template <typename T> std::string operator()(T t) const { return alpha(int(t)); }
    };

}

namespace {

    template <typename T>
    class NoHash {
    public:
        NoHash(T t = {}): value_(t) {}
        explicit operator T() const { return value_; }
        auto operator<=>(const NoHash& rhs) const = default;
        auto str() const { return format_object(value_); }
        auto alpha() const { return alpha(value_); }
    private:
        T value_;
    };

}

void test_crow_cache_hashed() {

    LruCache<int, std::string> cache;
    std::optional<std::string> optstr;

    TRY(cache.reserve(3));
    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [0]\n"
        "    map [0]\n"
    );

    TRY(cache.set(1, alpha(1)));
    TEST_EQUAL(cache.size(), 1u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        1: a\n"
        "    map [1]\n"
        "        1: 1, a\n"
    );

    TRY(cache.set(2, alpha(2)));
    TRY(cache.set(3, alpha(3)));
    TRY(cache.set(4, alpha(4)));
    TEST_EQUAL(cache.size(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "        2: bb\n"
        "    map [3]\n"
        "        4: 4, dddd\n"
        "        3: 3, ccc\n"
        "        2: 2, bb\n"
    );

    TRY(optstr = cache.get(1));
    TEST(! optstr.has_value());
    TRY(optstr = cache.get(2));
    TEST(optstr.has_value());
    TEST_EQUAL(optstr.value(), "bb");
    TEST_EQUAL(cache.size(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        2: bb\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "    map [3]\n"
        "        2: 2, bb\n"
        "        4: 4, dddd\n"
        "        3: 3, ccc\n"
    );

    TRY(cache.reserve(1));
    TEST_EQUAL(cache.size(), 1u);
    TEST_EQUAL(cache.capacity(), 1u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        2: bb\n"
        "    map [1]\n"
        "        2: 2, bb\n"
    );

    TRY(cache.reserve(5));
    TEST_EQUAL(cache.size(), 1u);
    TEST_EQUAL(cache.capacity(), 5u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        2: bb\n"
        "    map [1]\n"
        "        2: 2, bb\n"
    );

    calls = 0;
    TRY(cache.item(2, alpha));
    TRY(cache.item(3, alpha));
    TRY(cache.item(4, alpha));
    TEST_EQUAL(cache.size(), 3u);
    TEST_EQUAL(calls, 2);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "        2: bb\n"
        "    map [3]\n"
        "        4: 4, dddd\n"
        "        3: 3, ccc\n"
        "        2: 2, bb\n"
    );

    TRY(cache.clear());
    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 5u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [0]\n"
        "    map [0]\n"
    );

}

void test_crow_cache_ordered() {

    LruCache<NoHash<int>, std::string> cache;
    std::optional<std::string> optstr;

    TRY(cache.reserve(3));
    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [0]\n"
        "    map [0]\n"
    );

    TRY(cache.set(1, alpha(1)));
    TEST_EQUAL(cache.size(), 1u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        1: a\n"
        "    map [1]\n"
        "        1: 1, a\n"
    );

    TRY(cache.set(2, alpha(2)));
    TRY(cache.set(3, alpha(3)));
    TRY(cache.set(4, alpha(4)));
    TEST_EQUAL(cache.size(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "        2: bb\n"
        "    map [3]\n"
        "        2: 2, bb\n"
        "        3: 3, ccc\n"
        "        4: 4, dddd\n"
    );

    TRY(optstr = cache.get(1));
    TEST(! optstr.has_value());
    TRY(optstr = cache.get(2));
    TEST(optstr.has_value());
    TEST_EQUAL(optstr.value(), "bb");
    TEST_EQUAL(cache.size(), 3u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        2: bb\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "    map [3]\n"
        "        2: 2, bb\n"
        "        3: 3, ccc\n"
        "        4: 4, dddd\n"
    );

    TRY(cache.reserve(1));
    TEST_EQUAL(cache.size(), 1u);
    TEST_EQUAL(cache.capacity(), 1u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        2: bb\n"
        "    map [1]\n"
        "        2: 2, bb\n"
    );

    TRY(cache.reserve(5));
    TEST_EQUAL(cache.size(), 1u);
    TEST_EQUAL(cache.capacity(), 5u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [1]\n"
        "        2: bb\n"
        "    map [1]\n"
        "        2: 2, bb\n"
    );

    calls = 0;
    TRY(cache.item(2, Alpha()));
    TRY(cache.item(3, Alpha()));
    TRY(cache.item(4, Alpha()));
    TEST_EQUAL(cache.size(), 3u);
    TEST_EQUAL(calls, 2);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [3]\n"
        "        4: dddd\n"
        "        3: ccc\n"
        "        2: bb\n"
        "    map [3]\n"
        "        2: 2, bb\n"
        "        3: 3, ccc\n"
        "        4: 4, dddd\n"
    );

    TRY(cache.clear());
    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 5u);

    TEST_EQUAL(dump(cache),
        "LruCache\n"
        "    list [0]\n"
        "    map [0]\n"
    );

}

void test_crow_cache_function() {

    calls = 0;

    FunctionCache<int, std::string> cache(alpha, 3);

    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 3u);
    TEST_EQUAL(calls, 0);

    TEST_EQUAL(cache(1), "a");
    TEST_EQUAL(cache.size(), 1u);
    TEST_EQUAL(calls, 1);

    TEST_EQUAL(cache(2), "bb");
    TEST_EQUAL(cache(3), "ccc");
    TEST_EQUAL(cache(4), "dddd");
    TEST_EQUAL(cache.size(), 3u);
    TEST_EQUAL(calls, 4);

    TEST_EQUAL(cache(2), "bb");
    TEST_EQUAL(cache(3), "ccc");
    TEST_EQUAL(cache(4), "dddd");
    TEST_EQUAL(cache.size(), 3u);
    TEST_EQUAL(calls, 4);

    TRY(cache.clear());
    TEST_EQUAL(cache.size(), 0u);
    TEST_EQUAL(cache.capacity(), 3u);
    TEST_EQUAL(calls, 4);

}
