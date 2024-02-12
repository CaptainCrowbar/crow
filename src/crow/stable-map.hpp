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
        using map_iterator = typename map_type::iterator;
        using map_const_iterator = typename map_type::const_iterator;

        struct seq_entry {
            std::optional<value_type> value;
            map_iterator map_iter;
        };

        using seq_type = std::vector<seq_entry>;
        using seq_iterator = typename seq_type::iterator;
        using seq_const_iterator = typename seq_type::const_iterator;

        map_type map_;
        seq_type seq_;

        void rebuild();

    public:

        // Member types

        class const_iterator;

        class iterator:
        public ForwardIterator<iterator, value_type> {
        public:
            iterator() = default;
            value_type& operator*() noexcept { return *si_->value; }
            iterator& operator++() { while (si_ != seq_->end() && si_->value) ++si_; return *this; }
            bool operator==(const iterator& i) const noexcept { return si_ == i.si_; }
        private:
            friend class StableMap;
            friend class const_iterator;
            seq_type* seq_;
            seq_iterator si_;
            explicit iterator(seq_type& s, seq_iterator si): seq_(&s), si_(si) {}
        };

        class const_iterator {
        public:
            const_iterator() = default;
            const_iterator(const iterator& i): si_(i.si_) {}
            const value_type& operator*() noexcept { return *si_->value; }
            const_iterator& operator++() { while (si_ != seq_->end() && si_->value) ++si_; return *this; }
            bool operator==(const const_iterator& i) const noexcept { return si_ == i.si_; }
        private:
            friend class StableMap;
            const seq_type* seq_;
            seq_const_iterator si_;
            explicit const_iterator(const seq_type& s, seq_const_iterator sci): seq_(&s), si_(sci) {}
        };

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

        iterator begin() noexcept { return iterator(seq_, seq_.begin()); }
        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator cbegin() const noexcept { return const_iterator(seq_, seq_.cbegin()); }
        iterator end() noexcept { return iterator(seq_, seq_.end()); }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cend() const noexcept { return const_iterator(seq_, seq_.cend()); }

        // Capacity functions

        void clear() noexcept { seq_.clear(); map_.clear(); }
        bool empty() const noexcept { return map_.empty(); }
        size_t size() const noexcept { return map_.size(); }

        // Lookup functions

        T& operator[](const Key& k);
        bool contains(const Key& k) const { return map_.contains(k); }
        size_t count(const Key& k) const { return map_.count(k); }
        iterator find(const Key& k);
        const_iterator find(const Key& k) const;
        std::vector<iterator> find_all(const Key& k);
        std::vector<const_iterator> find_all(const Key& k) const;

        // Update functions

        std::pair<iterator, bool> insert(const value_type& v);
        std::pair<iterator, bool> insert(const Key& k, const T& t) { insert({k, t}); }
        void erase(const_iterator i) noexcept;
        size_t erase(const Key& k) noexcept;
        void swap(StableMap& m) noexcept;
        friend void swap(StableMap& a, StableMap& b) noexcept { a.swap(b); }

        // Policy functions

        Hash hash_function() const { return map_.hash_function(); }
        KeyEqual key_eq() const { return map_.key_eq(); }

    };

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>::StableMap(Hash h, KeyEqual eq):
        map_(0, h, eq),
        seq_() {}

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>::StableMap(const StableMap& m):
        map_(),
        seq_(m.seq_) {
            rebuild();
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        StableMap<Key, T, Dup, Hash, KeyEqual>&
        StableMap<Key, T, Dup, Hash, KeyEqual>::operator=(const StableMap& m) {
            auto temp(m);
            swap(temp);
            return *this;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        T& StableMap<Key, T, Dup, Hash, KeyEqual>::operator[](const Key& k) {
            auto it = find(k);
            if (it == end())
                it = insert(k, {}).first;
            return it->second;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find(const Key& k) {
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
            return iterator(seq_, seq_.begin() + index);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        typename StableMap<Key, T, Dup, Hash, KeyEqual>::const_iterator
        StableMap<Key, T, Dup, Hash, KeyEqual>::find(const Key& k) const {
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
            return const_iterator(seq_, seq_.begin() + index);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::vector<typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator>
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_all(const Key& k) {
            std::vector<iterator> results;
            auto range = map_.equal_range(k);
            for (auto mi = range.first; mi != range.second; ++mi)
                results.emplace_back(seq_, mi->second);
            return results;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::vector<typename StableMap<Key, T, Dup, Hash, KeyEqual>::const_iterator>
        StableMap<Key, T, Dup, Hash, KeyEqual>::find_all(const Key& k) const {
            std::vector<const_iterator> results;
            auto range = map_.equal_range(k);
            for (auto mi = range.first; mi != range.second; ++mi)
                results.emplace_back(seq_, mi->second);
            return results;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        std::pair<typename StableMap<Key, T, Dup, Hash, KeyEqual>::iterator, bool>
        StableMap<Key, T, Dup, Hash, KeyEqual>::insert(const value_type& v) {
            auto rc = map_.insert({v.key, size_t(0)});
            map_iterator mi;
            iterator it;
            if constexpr (Dup) {
                mi = rc;
            } else {
                mi = rc.first;
                if (! rc.second) {
                    auto si = seq_.begin() + mi->second;
                    it = iterator(seq_, si);
                    return {it, false};
                }
            }
            mi->second = seq_.size();
            auto guard = on_scope_fail([this,mi] { map_.erase(mi); });
            seq_.push_back({v, mi, false});
            return {it, true};
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::erase(const_iterator i) noexcept {
            i.si_->value.reset();
            map_.erase(i.si_->map_iter);
            if (map_.size() <= seq_.size() / 2)
                rebuild();
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        size_t StableMap<Key, T, Dup, Hash, KeyEqual>::erase(const Key& k) noexcept {
            auto range = map_.equal_range(k);
            size_t n = 0;
            for (auto mi = range.first; mi != range.second; ++mi) {
                seq_[mi->second].value.reset();
                map_.erase(mi);
                ++n;
            }
            if (map_.size() <= seq_.size() / 2)
                rebuild();
            return n;
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::swap(StableMap& m) noexcept {
            map_.swap(m.map_);
            seq_.swap(m.seq_);
        }

        template <typename Key, typename T, bool Dup, typename Hash, typename KeyEqual>
        void StableMap<Key, T, Dup, Hash, KeyEqual>::rebuild() {
            StableMap new_obj;
            seq_entry new_entry;
            map_iterator it;
            for (auto& entry: seq_) {
                if (entry.value) {
                    new_entry = {entry.value->key, new_obj.seq_.size()};
                    if constexpr (Dup)
                        it = new_obj.map_.insert(new_entry);
                    else
                        it = new_obj.map_.insert(new_entry).first;
                    new_obj.seq_.push_back({entry.value, it});
                }
            }
            swap(new_obj);
        }

}
