#pragma once

#include "crow/guard.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <functional>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Crow {

    template <typename Key, typename T, bool Dup = false,
        typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class StableMap {

    public:

        // Member types

        using key_type         = Key;
        using mapped_type      = T;
        using value_type       = std::pair<const Key, T>;
        using hasher           = Hash;
        using key_equal        = KeyEqual;
        using reference        = value_type&;
        using const_reference  = const value_type&;
        using pointer          = value_type*;
        using const_pointer    = const value_type*;
        using size_type        = size_t;
        using difference_type  = ptrdiff_t;

    private:

        using map_type = std::conditional_t<Dup,
            std::unordered_multimap<Key, size_t, Hash, KeyEqual>,
            std::unordered_map<Key, size_t, Hash, KeyEqual>>;
        using map_value = typename map_type::value_type;
        using map_iterator = typename map_type::iterator;
        using map_const_iterator = typename map_type::const_iterator;

        struct seq_entry {
            std::optional<value_type> value; // Empty optional acts as tombstone
            map_iterator map_iter;
        };

        using seq_type = std::vector<seq_entry>;
        using seq_iterator = typename seq_type::iterator;
        using seq_const_iterator = typename seq_type::const_iterator;

        class dummy_type;

        template <bool IsConst>
        class basic_iterator:
        public ForwardIterator<basic_iterator<IsConst>, std::conditional_t<IsConst, const value_type, value_type>> {
        private:
            using maybe_const_value = std::conditional_t<IsConst, const value_type, value_type>;
            using maybe_const_seq = std::conditional_t<IsConst, const seq_type, seq_type>;
            using mutable_iterator = std::conditional_t<IsConst, basic_iterator<false>, dummy_type>;
        public:
            basic_iterator() = default;
            basic_iterator(const mutable_iterator& i): seq_(i.seq_), index_(i.index_) { next(); }
            maybe_const_value& operator*() const noexcept { return *(*seq_)[index_].value; }
            basic_iterator& operator++() { ++index_; next(); return *this; }
            bool operator==(const basic_iterator& i) const noexcept { return index_ == i.index_; }
        private:
            friend class StableMap;
            template <bool> friend class basic_iterator;
            maybe_const_seq* seq_;
            size_t index_;
            explicit basic_iterator(maybe_const_seq& s, size_t i): seq_(&s), index_(i) { next(); }
            void next() { while (index_ < seq_->size() && ! (*seq_)[index_].value) ++index_; }
        };

    public:

        // Member types

        using iterator = basic_iterator<false>;
        using const_iterator = basic_iterator<true>;

        // Constants

        static constexpr bool duplicates = Dup;

        // Life cycle functions

        StableMap() = default;
        explicit StableMap(Hash h, KeyEqual eq = {});
        StableMap(const StableMap& m);
        StableMap(StableMap&& m) noexcept = default;
        ~StableMap() noexcept = default;
        StableMap& operator=(const StableMap& m);
        StableMap& operator=(StableMap&& m) noexcept = default;

        // Iterator functions

        iterator begin() noexcept { return iterator(seq_, 0); }
        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator cbegin() const noexcept { return const_iterator(seq_, 0); }
        iterator end() noexcept { return iterator(seq_, seq_.size()); }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cend() const noexcept { return const_iterator(seq_, seq_.size()); }

        // Capacity functions

        void clear() noexcept { seq_.clear(); map_.clear(); }
        bool empty() const noexcept { return map_.empty(); }
        void rehash();
        size_t size() const noexcept { return map_.size(); }

        // Lookup functions

        template <bool OK = ! Dup, std::enable_if_t<OK, int> = 0> T& operator[](const Key& k);
        bool contains(const Key& k) const { return map_.contains(k); }
        size_t count(const Key& k) const { return map_.count(k); }
        iterator find(const Key& k);
        const_iterator find(const Key& k) const;
        iterator find_last(const Key& k);
        const_iterator find_last(const Key& k) const;
        std::vector<iterator> find_all(const Key& k);
        std::vector<const_iterator> find_all(const Key& k) const;

        // Update functions

        std::pair<iterator, bool> insert(const value_type& v) { return insert_or_update(v, false); }
        std::pair<iterator, bool> insert(const Key& k, const T& t) { return insert_or_update({k, t}, false); }
        template <bool OK = ! Dup, std::enable_if_t<OK, int> = 0>
            std::pair<iterator, bool> update(const value_type& v) { return insert_or_update(v, true); }
        template <bool OK = ! Dup, std::enable_if_t<OK, int> = 0>
            std::pair<iterator, bool> update(const Key& k, const T& t) { return insert_or_update({k, t}, true); }
        void erase(const_iterator i) noexcept;
        size_t erase(const Key& k) noexcept;
        void swap(StableMap& m) noexcept;
        friend void swap(StableMap& a, StableMap& b) noexcept { a.swap(b); }

        // Policy functions

        Hash hash_function() const { return map_.hash_function(); }
        KeyEqual key_eq() const { return map_.key_eq(); }

    private:

        map_type map_;
        seq_type seq_;

        std::pair<iterator, bool> insert_or_update(const value_type& v, bool update);
        void rebuild_vector();

    };

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>::StableMap(Hash h, KeyEqual eq):
        map_(0, h, eq),
        seq_() {}

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>::StableMap(const StableMap& m):
        map_(),
        seq_(m.seq_) {
            rebuild_vector();
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>&
        StableMap<Key, T, Dup, Hash, KeyEqual>::operator=(const StableMap& m) {
            auto temp(m);
            swap(temp);
            return *this;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::rehash() {
            map_.rehash(0);
            rebuild_vector();
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        template <bool OK, std::enable_if_t<OK, int>>
        T& StableMap<Key, T, Dup, Hash, KeyEqual>::operator[](const Key& k) {
            auto it = find(k);
            if (it == end())
                it = insert(k, {}).first;
            return it->second;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find(const Key& k) {
            map_iterator mi;
            if constexpr (Dup) {
                auto range = map_.equal_range(k);
                if (range.first == range.second)
                    return end();
                mi = range.first;
            } else {
                mi = map_.find(k);
                if (mi == map_.end())
                    return end();
            }
            return iterator(seq_, mi->second);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::const_iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find(const Key& k) const {
            map_const_iterator mi;
            if constexpr (Dup) {
                auto range = map_.equal_range(k);
                if (range.first == range.second)
                    return end();
                mi = range.first;
            } else {
                mi = map_.find(k);
                if (mi == map_.end())
                    return end();
            }
            return const_iterator(seq_, mi->second);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_last(const Key& k) {
            size_t index = 0;
            if constexpr (Dup) {
                auto range = map_.equal_range(k);
                if (range.first == range.second)
                    return end();
                for (auto mi = range.first; mi != range.second; ++mi)
                    index = std::max(index, mi->second);
            } else {
                auto mi = map_.find(k);
                if (mi == map_.end())
                    return end();
                index = mi->second;
            }
            return iterator(seq_, index);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::const_iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_last(const Key& k) const {
            size_t index = 0;
            if constexpr (Dup) {
                auto range = map_.equal_range(k);
                if (range.first == range.second)
                    return end();
                for (auto mi = range.first; mi != range.second; ++mi)
                    index = std::max(index, mi->second);
            } else {
                auto mi = map_.find(k);
                if (mi == map_.end())
                    return end();
                index = mi->second;
            }
            return const_iterator(seq_, index);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::vector<typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator>
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_all(const Key& k) {
            std::vector<iterator> results;
            auto range = map_.equal_range(k);
            for (auto mi = range.first; mi != range.second; ++mi)
                results.push_back(iterator(seq_, mi->second));
            return results;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::vector<typename StableMap<Key, T, Dup, Hash, KeyEqual>::const_iterator>
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_all(const Key& k) const {
            std::vector<const_iterator> results;
            auto range = map_.equal_range(k);
            for (auto mi = range.first; mi != range.second; ++mi)
                results.push_back(const_iterator(seq_, mi->second));
            return results;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::erase(const_iterator i) noexcept {
            seq_[i.index_].value.reset();
            map_.erase(seq_[i.index_].map_iter);
            if (map_.size() <= seq_.size() / 2)
                rebuild_vector();
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        size_t StableMap<Key, T, Dup, Hash, KeyEqual>::erase(const Key& k) noexcept {
            auto range = map_.equal_range(k);
            std::vector<map_iterator> trash;
            size_t n = 0;
            for (auto mi = range.first; mi != range.second; ++mi) {
                seq_[mi->second].value.reset();
                trash.push_back(mi);
                ++n;
            }
            for (auto mi: trash)
                map_.erase(mi);
            if (map_.size() <= seq_.size() / 2)
                rebuild_vector();
            return n;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::swap(StableMap& m) noexcept {
            map_.swap(m.map_);
            seq_.swap(m.seq_);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::pair<typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator, bool>
        StableMap<Key, T, Dup, Hash, KeyEqual>::insert_or_update(const value_type& v, bool update) {
            auto index = seq_.size();
            auto pre_buckets = map_.bucket_count();
            auto rc = map_.insert({v.first, index});
            auto post_buckets = map_.bucket_count();
            map_iterator mi;
            bool inserted;
            if constexpr (Dup) {
                mi = rc;
                inserted = true;
            } else {
                mi = rc.first;
                inserted = rc.second;
            }
            if (inserted) {
                auto map_guard = on_scope_failure([this,mi] { map_.erase(mi); });
                seq_.push_back({v, mi});
                if (post_buckets > pre_buckets) {
                    auto seq_guard = on_scope_failure([this] { seq_.pop_back(); });
                    rehash();
                }
            } else {
                index = mi->second;
                if (update) {
                    auto map_guard = on_scope_failure([this,mi] { map_.erase(mi); });
                    seq_[index].value->second = v.second;
                }
            }
            auto it = iterator(seq_, index);
            return {it, inserted};
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::rebuild_vector() {
            StableMap new_obj;
            map_iterator it;
            for (auto& entry: seq_) {
                if (entry.value) {
                    map_value new_value{entry.value->first, new_obj.seq_.size()};
                    if constexpr (Dup)
                        it = new_obj.map_.insert(std::move(new_value));
                    else
                        it = new_obj.map_.insert(std::move(new_value)).first;
                    new_obj.seq_.push_back({entry.value, it});
                }
            }
            swap(new_obj);
        }

}
