#pragma once

#include "crow/enum.hpp"
#include "crow/hash.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Crow {

    CROW_ENUM_CLASS(FlexImpl, int, 1,
        order,
        hash,
        linear
    )

    namespace Detail {

        template <std::equality_comparable K, FlexImpl Prefer>
        constexpr FlexImpl SelectFlexImpl =
            std::totally_ordered<K> && Hashable<K> ? Prefer :
            std::totally_ordered<K> ? FlexImpl::order :
            Hashable<K> ? FlexImpl::hash :
            FlexImpl::linear;

        template <std::equality_comparable K, std::regular T, typename Container>
        class FlexibleMapBase {
        public:
            using const_iterator = typename Container::const_iterator;
            using iterator = typename Container::iterator;
            using key_type = K;
            using mapped_type = T;
            using value_type = typename Container::value_type;
            T& operator[](const K& k) { return con_[k]; }
            iterator begin() noexcept { return con_.begin(); }
            const_iterator begin() const noexcept { return con_.begin(); }
            iterator end() noexcept { return con_.end(); }
            const_iterator end() const noexcept { return con_.end(); }
            size_t size() const noexcept { return con_.size(); }
            bool empty() const noexcept { return con_.empty(); }
            bool contains(const K& k) const { return con_.contains(k); }
            iterator find(const K& k) { return con_.find(k); }
            const_iterator find(const K& k) const { return con_.find(k); }
            std::pair<iterator, bool> insert(const value_type& value) { return con_.insert(value); }
            iterator insert(const_iterator i, const value_type& value) { return con_.insert(i, value); }
            template <InputIteratorType II> void insert(II i, II j) { return con_.insert(i, j); }
            void insert(std::initializer_list<value_type> list) { return con_.insert(list); }
            void erase(const_iterator i) { con_.erase(i); }
            bool erase(const K& k) { return con_.erase(k); }
            void clear() noexcept { con_.clear(); }
        protected:
            Container con_;
        };

        template <std::equality_comparable K, typename Container>
        class FlexibleSetBase {
        public:
            using const_iterator = typename Container::const_iterator;
            using iterator = const_iterator;
            using key_type = K;
            using value_type = K;
            const_iterator begin() const noexcept { return con_.begin(); }
            const_iterator end() const noexcept { return con_.end(); }
            size_t size() const noexcept { return con_.size(); }
            bool empty() const noexcept { return con_.empty(); }
            bool contains(const K& k) const { return con_.contains(k); }
            const_iterator find(const K& k) const { return con_.find(k); }
            std::pair<iterator, bool> insert(const K& k) { return con_.insert(k); }
            iterator insert(const_iterator i, const K& k) { return con_.insert(i, k); }
            template <InputIteratorType II> void insert(II i, II j) { return con_.insert(i, j); }
            void insert(std::initializer_list<K> list) { return con_.insert(list); }
            void erase(const_iterator i) { con_.erase(i); }
            bool erase(const K& k) { return con_.erase(k); }
            void clear() noexcept { con_.clear(); }
        protected:
            Container con_;
        };

        template <std::equality_comparable K, std::regular T, FlexImpl FI>
        class FlexibleMapImpl;

        template <std::equality_comparable K, std::regular T>
        class FlexibleMapImpl<K, T, FlexImpl::order>:
        public FlexibleMapBase<K, T, std::map<K, T>> {};

        template <std::equality_comparable K, std::regular T>
        class FlexibleMapImpl<K, T, FlexImpl::hash>:
        public FlexibleMapBase<K, T, std::unordered_map<K, T>> {};

        template <std::equality_comparable K, std::regular T>
        class FlexibleMapImpl<K, T, FlexImpl::linear>:
        public FlexibleMapBase<K, T, std::vector<std::pair<K, T>>> {
        private:
            using base = FlexibleMapBase<K, T, std::vector<std::pair<K, T>>>;
        public:
            T& operator[](const K& k) {
                auto i = find(k);
                if (i == this->end()) {
                    this->con_.push_back({k, {}});
                    i = this->end() - 1;
                }
                return i->second;
            }
            bool contains(const K& k) const { return find(k) != this->end(); }
            typename base::iterator find(const K& k) {
                return std::find_if(this->con_.begin(), this->con_.end(), [&k] (auto& v) { return v.first == k; });
            }
            typename base::const_iterator find(const K& k) const {
                return std::find_if(this->con_.begin(), this->con_.end(), [&k] (auto& v) { return v.first == k; });
            }
            std::pair<typename base::iterator, bool> insert(const typename base::value_type& value) {
                auto i = find(value.first);
                if (i != this->end())
                    return {i, false};
                this->con_.push_back(value);
                return {this->end() - 1, true};
            }
            typename base::iterator insert(typename base::const_iterator /*i*/,
                    const typename base::value_type& value) {
                return insert(value).first;
            }
            template <InputIteratorType II> void insert(II i, II j) { while (i != j) insert(*i++); }
            void insert(std::initializer_list<typename base::value_type> list) { for (auto& x: list) insert(x); }
            void erase(typename base::const_iterator i) { this->con_.erase(i); }
            bool erase(const K& k) {
                auto i = find(k);
                if (i == this->end())
                    return false;
                erase(i);
                return true;
            }
        };

        template <std::equality_comparable K, FlexImpl FI>
        class FlexibleSetImpl;

        template <std::equality_comparable K>
        class FlexibleSetImpl<K, FlexImpl::order>:
        public FlexibleSetBase<K, std::set<K>> {};

        template <std::equality_comparable K>
        class FlexibleSetImpl<K, FlexImpl::hash>:
        public FlexibleSetBase<K, std::unordered_set<K>> {};

        template <std::equality_comparable K>
        class FlexibleSetImpl<K, FlexImpl::linear>:
        public FlexibleSetBase<K, std::vector<K>> {
        private:
            using base = FlexibleSetBase<K, std::vector<K>>;
        public:
            bool contains(const K& k) const { return find(k) != this->end(); }
            typename base::iterator find(const K& k) const {
                return std::find(this->con_.begin(), this->con_.end(), k);
            }
            std::pair<typename base::iterator, bool> insert(const K& k) {
                auto i = find(k);
                if (i != this->end())
                    return {i, false};
                this->con_.push_back(k);
                return {this->end() - 1, true};
            }
            typename base::iterator insert(typename base::const_iterator /*i*/,
                    const K& k) {
                return insert(k).first;
            }
            template <InputIteratorType II> void insert(II i, II j) { while (i != j) insert(*i++); }
            void insert(std::initializer_list<K> list) { for (auto& x: list) insert(x); }
            void erase(typename base::iterator i) { this->con_.erase(i); }
            bool erase(const K& k) {
                auto i = find(k);
                if (i == this->end())
                    return false;
                erase(i);
                return true;
            }
        };

    }

    template <std::equality_comparable K, std::regular T, FlexImpl Prefer = FlexImpl::order>
    class FlexibleMap:
    public Detail::FlexibleMapImpl<K, T, Detail::SelectFlexImpl<K, Prefer>> {
    private:
        using base = Detail::FlexibleMapImpl<K, T, Detail::SelectFlexImpl<K, Prefer>>;
    public:
        static_assert(Prefer != FlexImpl::linear);
        static constexpr FlexImpl impl = Detail::SelectFlexImpl<K, Prefer>;
        FlexibleMap() = default;
        template <InputIteratorType II> FlexibleMap(II i, II j) { this->insert(i, j); }
        FlexibleMap(std::initializer_list<typename base::value_type> list) { this->insert(list); }
    };

    template <std::equality_comparable K, FlexImpl Prefer = FlexImpl::order>
    class FlexibleSet:
    public Detail::FlexibleSetImpl<K, Detail::SelectFlexImpl<K, Prefer>> {
    public:
        static_assert(Prefer != FlexImpl::linear);
        static constexpr FlexImpl impl = Detail::SelectFlexImpl<K, Prefer>;
        FlexibleSet() = default;
        template <InputIteratorType II> FlexibleSet(II i, II j) { this->insert(i, j); }
        FlexibleSet(std::initializer_list<K> list) { this->insert(list); }
    };

}
