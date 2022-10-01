#pragma once

#include "crow/enum.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <concepts>
#include <list>
#include <map>
#include <unordered_map>

namespace Crow {

    CROW_ENUM_CLASS(CacheType, int, 0,
        none,
        hashed,
        ordered
    )

    template <typename T>
    constexpr CacheType cache_type =
        Hashable<T> ? CacheType::hashed :
        std::totally_ordered<T> ? CacheType::ordered :
        CacheType::none;

    namespace Detail {

        template <typename K, typename T, CacheType CT = cache_type<K>>
        struct CacheMap;

        template <typename K, typename T>
        struct CacheMap<K, T, CacheType::hashed> {
            using type = std::unordered_map<K, T>;
        };

        template <typename K, typename T>
        struct CacheMap<K, T, CacheType::ordered> {
            using type = std::map<K, T>;
        };

    }

    template <typename K, typename T>
    class LruCache {

    public:

        using key_type = K;
        using mapped_type = T;

        LruCache() = default;
        explicit LruCache(size_t cap): capacity_(cap) {}

        T operator[](const K& k);

        size_t capacity() const noexcept { return capacity_; }
        void clear() noexcept { list_.clear(); map_.clear(); }
        bool empty() const noexcept { return map_.empty(); }
        void reserve(size_t cap);
        size_t size() const noexcept { return map_.size(); }

    private:

        using list_type = std::list<K>;
        using map_type = typename Detail::CacheMap<K, T>::type;

        list_type list_; // chronological order
        map_type map_;
        size_t capacity_ = npos;

        void pop() noexcept;

    };

        template <typename K, typename T>
        T LruCache<K, T>::operator[](const K& k) {
            // TODO
            (void)k;
            return {};
        }

        template <typename K, typename T>
        void LruCache<K, T>::reserve(size_t cap) {
            capacity_ = cap;
            if (size() > capacity_) {
                auto a = list_.begin();
                auto b = a;
                for (size_t i = 0; i < capacity_; ++i, ++b)
                    map_.erase(*b);
                list_.erase(a, b);
            }
        }

        template <typename K, typename T>
        void LruCache<K, T>::pop() noexcept {
            // TODO
        }

}
