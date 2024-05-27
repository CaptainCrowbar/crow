#pragma once

#include "crow/enum.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <concepts>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename K> class InvalidCacheKeyType;

    }

    template <typename K, typename T>
    class LruCache {

    public:

        using key_type = K;
        using mapped_type = T;

        LruCache() = default;
        explicit LruCache(size_t cap): capacity_(cap) {}
        LruCache(const LruCache&) = delete;
        LruCache(LruCache&&) = default;
        ~LruCache() = default;
        LruCache& operator=(const LruCache&) = delete;
        LruCache& operator=(LruCache&&) = default;

        std::optional<T> get(const K& k);
        void set(const K& k, const T& t);

        template <std::invocable<K> F>
            requires (std::convertible_to<std::invoke_result_t<F, K>, T>)
            T item(const K& k, F&& f);

        size_t size() const noexcept { return map_.size(); }
        size_t capacity() const noexcept { return capacity_; }
        void clear() noexcept;
        void reserve(size_t cap) noexcept;

    private:

        friend class DebugCache;

        struct list_node {
            K key;
            T value;
        };

        using list_type = std::list<list_node>;
        using list_iterator = typename list_type::iterator;
        using map_type = std::conditional_t<Hashable<K>, std::unordered_map<K, list_iterator>,
            std::conditional_t<std::totally_ordered<K>, std::map<K, list_iterator>, Detail::InvalidCacheKeyType<K>>>;
        using map_iterator = typename map_type::iterator;

        list_type list_; // newest to oldest
        map_type map_;
        size_t capacity_ = npos;

        void check_size() noexcept;
        map_iterator move_to_front(const K& k);

    };

        template <typename K, typename T>
        std::optional<T> LruCache<K, T>::get(const K& k) {
            auto m = move_to_front(k);
            if (m == map_.end())
                return {};
            else
                return list_.front().value;
        }

        template <typename K, typename T>
        void LruCache<K, T>::set(const K& k, const T& t) {
            auto m = move_to_front(k);
            if (m == map_.end()) {
                list_.push_front({k, t});
                map_.insert({k, list_.begin()});
                check_size();
            } else {
                m->second->value = t;
            }
        }

        template <typename K, typename T>
        template <std::invocable<K> F>
        requires (std::convertible_to<std::invoke_result_t<F, K>, T>)
        T LruCache<K, T>::item(const K& k, F&& f) {
            auto m = move_to_front(k);
            if (m == map_.end()) {
                auto t = T(std::forward<F>(f)(k));
                list_.push_front({k, t});
                map_.insert({k, list_.begin()});
                check_size();
            }
            return list_.front().value;
        }

        template <typename K, typename T>
        void LruCache<K, T>::clear() noexcept {
            map_.clear();
            list_.clear();
        }

        template <typename K, typename T>
        void LruCache<K, T>::reserve(size_t cap) noexcept {
            capacity_ = cap;
            check_size();
        }

        template <typename K, typename T>
        void LruCache<K, T>::check_size() noexcept {
            if (size() > capacity_) {
                auto l = list_.end();
                for (auto n = size(); n > capacity_; --n)
                    map_.erase((--l)->key);
                list_.erase(l, list_.end());
            }
        }

        template <typename K, typename T>
        typename LruCache<K, T>::map_iterator LruCache<K, T>::move_to_front(const K& k) {
            auto m = map_.find(k);
            if (m != map_.end() && m->second != list_.begin())
                list_.splice(list_.begin(), list_, m->second, std::next(m->second));
            return m;
        }

    template <typename Arg, typename Res>
    class FunctionCache {

    public:

        using argument_type = Arg;
        using result_type = Res;

        FunctionCache() = default;

        template <std::invocable<Arg> F>
            requires (std::convertible_to<std::invoke_result_t<F, Arg>, Res>)
            explicit FunctionCache(F&& f, size_t cap = npos):
            cache_(cap), function_(std::forward<F>(f)) {}

        FunctionCache(const FunctionCache&) = delete;
        FunctionCache(FunctionCache&&) = default;
        ~FunctionCache() = default;
        FunctionCache& operator=(const FunctionCache&) = delete;
        FunctionCache& operator=(FunctionCache&&) = default;

        Res operator()(const Arg& arg) { return cache_.item(arg, function_); }

        size_t size() const noexcept { return cache_.size(); }
        size_t capacity() const noexcept { return cache_.capacity(); }
        void clear() noexcept { cache_.clear(); }
        void reserve(size_t cap) noexcept { cache_.reserve(cap); }

    private:

        LruCache<Arg, Res> cache_;
        std::function<Res(const Arg&)> function_;

    };

}
