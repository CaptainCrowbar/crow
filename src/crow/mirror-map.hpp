#pragma once

#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <set>
#include <tuple>
#include <utility>

namespace Crow {

    template <typename K1, typename K2,
        std::strict_weak_order<K1, K1> C1 = std::less<K1>,
        std::strict_weak_order<K2, K2> C2 = std::less<K2>>
    class MirrorMap {

    public:

        using left_key = K1;
        using right_key = K2;
        using left_compare = C1;
        using right_compare = C2;
        using value_type = std::pair<K1, K2>;

    private:

        struct left_order {
            using is_transparent = void;
            C1 cmp1;
            C2 cmp2;
            bool operator()(const value_type& lhs, const value_type& rhs) const;
            bool operator()(const value_type& lhs, const K1& rhs) const { return cmp1(lhs.first, rhs); }
            bool operator()(const K1& lhs, const value_type& rhs) const { return cmp1(lhs, rhs.first); }
        };

        using left_set_type = std::set<value_type, left_order>;
        using left_position = typename left_set_type::const_iterator;

        struct right_order {
            using is_transparent = void;
            C1 cmp1;
            C2 cmp2;
            bool operator()(const value_type& lhs, const value_type& rhs) const;
            bool operator()(const left_position& lhs, const left_position& rhs) const { return (*this)(*lhs, *rhs); }
            bool operator()(const left_position& lhs, const value_type& rhs) const { return (*this)(*lhs, rhs); }
            bool operator()(const value_type& lhs, const left_position& rhs) const { return (*this)(lhs, *rhs); }
            bool operator()(const left_position& lhs, const K2& rhs) const { return cmp2(lhs->second, rhs); }
            bool operator()(const K2& lhs, const left_position& rhs) const { return cmp2(lhs, rhs->second); }
        };

        using right_set_type = std::set<left_position, right_order>;
        using right_position = typename right_set_type::const_iterator;

    public:

        class left_iterator:
        public BidirectionalIterator<left_iterator, const value_type> {
        public:
            const value_type& operator*() const noexcept { return *iter; }
            left_iterator& operator++() { ++iter; return *this; }
            left_iterator& operator--() { --iter; return *this; }
            bool operator==(const left_iterator& rhs) const noexcept { return iter == rhs.iter; }
        private:
            friend class MirrorMap;
            left_position iter;
        };

        class right_iterator:
        public BidirectionalIterator<right_iterator, const value_type> {
        public:
            const value_type& operator*() const noexcept { return **iter; }
            right_iterator& operator++() { ++iter; return *this; }
            right_iterator& operator--() { --iter; return *this; }
            bool operator==(const right_iterator& rhs) const noexcept { return iter == rhs.iter; }
        private:
            friend class MirrorMap;
            right_position iter;
        };

        struct insert_result {
            left_iterator left;
            right_iterator right;
            bool inserted = false;
        };

        using left_range = Irange<left_iterator>;
        using right_range = Irange<right_iterator>;

        MirrorMap() = default;
        explicit MirrorMap(C1 c1, C2 c2 = {});
        MirrorMap(std::initializer_list<value_type> list);
        template <typename Iterator> MirrorMap(Iterator i, Iterator j, C1 c1 = {}, C2 c2 = {});

        void clear() noexcept { right_set_.clear(); left_set_.clear(); }
        size_t count_left(const K1& key) const { return left_set_.count(key); }
        size_t count_right(const K2& key) const { return right_set_.count(key); }
        bool empty() const noexcept { return left_set_.empty(); }
        left_range equal_left(const K1& key) const;
        right_range equal_right(const K2& key) const;
        void erase(left_iterator i) noexcept;
        void erase(right_iterator i) noexcept;
        bool erase(const value_type& pair) noexcept;
        size_t erase_left(const K1& key) noexcept;
        size_t erase_right(const K2& key) noexcept;
        left_iterator find_left(const K1& key) const;
        left_iterator find_left(const value_type& pair) const;
        right_iterator find_right(const K2& key) const;
        right_iterator find_right(const value_type& pair) const;
        insert_result insert(const value_type& pair);
        template <typename Iterator> void insert(Iterator i, Iterator j);
        left_range left() const { return {begin_left(), end_left()}; }
        left_iterator begin_left() const;
        left_iterator end_left() const;
        right_range right() const { return {begin_right(), end_right()}; }
        right_iterator begin_right() const;
        right_iterator end_right() const;
        C1 left_comp() const { return left_set_.key_comp().cmp1; }
        C2 right_comp() const { return left_set_.key_comp().cmp2; }
        left_iterator mirror(right_iterator i) const;
        right_iterator mirror(left_iterator i) const;
        size_t size() const noexcept { return left_set_.size(); }
        void swap(MirrorMap& mm) noexcept;

        friend void swap(MirrorMap& mm1, MirrorMap& mm2) noexcept { mm1.swap(mm2); }

    private:

        left_set_type left_set_;
        right_set_type right_set_;

        void build_right_set();

    };

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        bool MirrorMap<K1, K2, C1, C2>::left_order::operator()(const value_type& lhs, const value_type& rhs) const {
            if (cmp1(lhs.first, rhs.first))
                return true;
            else if (cmp1(rhs.first, lhs.first))
                return false;
            else
                return cmp2(lhs.second, rhs.second);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        bool MirrorMap<K1, K2, C1, C2>::right_order::operator()(const value_type& lhs, const value_type& rhs) const {
            if (cmp2(lhs.second, rhs.second))
                return true;
            else if (cmp2(rhs.second, lhs.second))
                return false;
            else
                return cmp1(lhs.first, rhs.first);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        MirrorMap<K1, K2, C1, C2>::MirrorMap(C1 c1, C2 c2):
        left_set_(left_order{c1, c2}),
        right_set_(right_order{c1, c2}) {}

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        MirrorMap<K1, K2, C1, C2>::MirrorMap(std::initializer_list<value_type> list):
        left_set_{list},
        right_set_() {
            build_right_set();
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        template <typename Iterator>
        MirrorMap<K1, K2, C1, C2>::MirrorMap(Iterator i, Iterator j, C1 c1, C2 c2):
        left_set_(i, j, left_order{c1, c2}), right_set_(right_order{c1, c2}) {
            build_right_set();
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_range MirrorMap<K1, K2, C1, C2>::equal_left(const K1& key) const {
            auto eqr = std::equal_range(left_set_.begin(), left_set_.end(), key, left_set_.key_comp());
            left_range range;
            range.first.iter = eqr.first;
            range.second.iter = eqr.second;
            return range;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_range MirrorMap<K1, K2, C1, C2>::equal_right(const K2& key) const {
            auto eqr = std::equal_range(right_set_.begin(), right_set_.end(), key, right_set_.key_comp());
            right_range range;
            range.first.iter = eqr.first;
            range.second.iter = eqr.second;
            return range;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        void MirrorMap<K1, K2, C1, C2>::erase(left_iterator i) noexcept {
            right_set_.erase(i.iter);
            left_set_.erase(i.iter);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        void MirrorMap<K1, K2, C1, C2>::erase(right_iterator i) noexcept {
            auto j = *i.iter;
            right_set_.erase(i.iter);
            left_set_.erase(j);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        bool MirrorMap<K1, K2, C1, C2>::erase(const value_type& pair) noexcept {
            auto i = left_set_.find(pair);
            if (i == left_set_.end())
                return false;
            right_set_.erase(i);
            left_set_.erase(i);
            return true;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        size_t MirrorMap<K1, K2, C1, C2>::erase_left(const K1& key) noexcept {
            auto eqr = std::equal_range(left_set_.begin(), left_set_.end(), key, left_set_.key_comp());
            size_t n = 0;
            for (auto i = eqr.first; i != eqr.second; ++i, ++n)
                right_set_.erase(i);
            left_set_.erase(eqr.first, eqr.second);
            return n;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        size_t MirrorMap<K1, K2, C1, C2>::erase_right(const K2& key) noexcept {
            auto eqr = std::equal_range(right_set_.begin(), right_set_.end(), key, right_set_.key_comp());
            size_t n = 0;
            for (auto i = eqr.first; i != eqr.second; ++i, ++n)
                left_set_.erase(*i);
            right_set_.erase(eqr.first, eqr.second);
            return n;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_iterator MirrorMap<K1, K2, C1, C2>::find_left(const K1& key) const {
            left_iterator i;
            i.iter = left_set_.find(key);
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_iterator MirrorMap<K1, K2, C1, C2>::find_left(const value_type& pair) const {
            left_iterator i;
            i.iter = left_set_.find(pair);
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_iterator MirrorMap<K1, K2, C1, C2>::find_right(const K2& key) const {
            right_iterator i;
            i.iter = right_set_.find(key);
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_iterator MirrorMap<K1, K2, C1, C2>::find_right(const value_type& pair) const {
            right_iterator i;
            i.iter = right_set_.find(pair);
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::insert_result MirrorMap<K1, K2, C1, C2>::insert(const value_type& pair) {
            insert_result res;
            std::tie(res.left.iter, res.inserted) = left_set_.insert(pair);
            res.right.iter = right_set_.insert(res.left.iter).first;
            return res;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        template <typename Iterator>
        void MirrorMap<K1, K2, C1, C2>::insert(Iterator i, Iterator j) {
            for (; i != j; ++i)
                insert(*i);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_iterator MirrorMap<K1, K2, C1, C2>::begin_left() const {
            left_iterator i;
            i.iter = left_set_.begin();
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_iterator MirrorMap<K1, K2, C1, C2>::end_left() const {
            left_iterator i;
            i.iter = left_set_.end();
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_iterator MirrorMap<K1, K2, C1, C2>::begin_right() const {
            right_iterator i;
            i.iter = right_set_.begin();
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_iterator MirrorMap<K1, K2, C1, C2>::end_right() const {
            right_iterator i;
            i.iter = right_set_.end();
            return i;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::left_iterator MirrorMap<K1, K2, C1, C2>::mirror(right_iterator i) const {
            left_iterator j;
            j.iter = i.iter == right_set_.end() ? left_set_.end() : *i.iter;
            return j;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        typename MirrorMap<K1, K2, C1, C2>::right_iterator MirrorMap<K1, K2, C1, C2>::mirror(left_iterator i) const {
            right_iterator j;
            j.iter = i.iter == left_set_.end() ? right_set_.end() : right_set_.find(i.iter);
            return j;
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        void MirrorMap<K1, K2, C1, C2>::swap(MirrorMap& mm) noexcept {
            left_set_.swap(mm.left_set_);
            right_set_.swap(mm.right_set_);
        }

        template <typename K1, typename K2, std::strict_weak_order<K1, K1> C1, std::strict_weak_order<K2, K2> C2>
        void MirrorMap<K1, K2, C1, C2>::build_right_set() {
            for (auto i = left_set_.begin(); i != left_set_.end(); ++i)
                right_set_.insert(i);
        }

}
