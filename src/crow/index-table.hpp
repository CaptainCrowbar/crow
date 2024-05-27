#pragma once

#include "crow/guard.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <compare>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace Crow {

    enum class IndexMode: int {
        unique = 1,
        duplicate
    };

    template <typename T> class IndexTable;
    template <typename K, typename T, IndexMode M = IndexMode::unique> class Index;

    class IndexCollision:
    public std::runtime_error {
    public:
        IndexCollision(): std::runtime_error("Index collision") {}
    };

    namespace Detail {

        template <typename K, typename T, IndexMode M> struct IndexMapTraits;

        template <typename K, typename T>
        struct IndexMapTraits<K, T, IndexMode::unique> {

            using compare_function = std::function<bool(const K&, const K&)>;
            using map_type = std::map<K, T, compare_function>;
            using value_type = typename map_type::value_type;

            static void insert(const K& k, const T& value, map_type& map_) {
                if (! map_.insert(value_type(k, value)).second)
                    throw IndexCollision();
            }

        };

        template <typename K, typename T>
        struct IndexMapTraits<K, T, IndexMode::duplicate> {

            using compare_function = std::function<bool(const K&, const K&)>;
            using map_type = std::multimap<K, T, compare_function>;
            using value_type = typename map_type::value_type;

            static void insert(const K& k, const T& value, map_type& map_) {
                map_.insert(value_type(k, value));
            }

        };

        template <typename T>
        class IndexRefBase {
        public:

            using list_type = std::list<T>;
            using list_iterator = typename list_type::iterator;
            using value_type = T;

            virtual ~IndexRefBase() = default;
            virtual void clear() noexcept = 0;
            virtual void erase(list_iterator i) = 0;
            virtual void insert(list_iterator i) = 0;

        protected:

            IndexRefBase() = default;

        };

        template <typename T> class IndexRef;

        template <typename K, typename T, IndexMode M>
        class IndexRef<Index<K, T, M>>:
        public IndexRefBase<T> {

        public:

            using index_type = Index<K, T, M>;
            using list_type = std::list<T>;
            using list_iterator = typename list_type::iterator;
            using map_traits = typename index_type::map_traits;

            explicit IndexRef(index_type& index) noexcept: ind_(index) {}
            ~IndexRef() noexcept override { clear(); }
            IndexRef(const IndexRef&) = delete;
            IndexRef(IndexRef&&) = delete;
            IndexRef& operator=(const IndexRef&) = delete;
            IndexRef& operator=(IndexRef&&) = delete;

            void clear() noexcept override { ind_.map_.clear(); }
            void erase(list_iterator i) override { ind_.map_.erase(ind_.ext_(*i)); }
            void insert(list_iterator i) override { map_traits::insert(ind_.ext_(*i), i, ind_.map_); }

        private:

            index_type& ind_;

        };

    }

    template <typename T>
    class IndexTable {

    private:

        using list_type = std::list<T>;

    public:

        using const_iterator = typename list_type::const_iterator;
        using const_reference = const T&;
        using difference_type = ptrdiff_t;
        using iterator = typename list_type::iterator;
        using reference = T&;
        using size_type = size_t;
        using value_type = T;

        IndexTable() = default;
        template <typename InputRange> explicit IndexTable(const InputRange& src);
        template <typename Iterator> IndexTable(Iterator i1, Iterator i2);

        ~IndexTable() = default;
        IndexTable(const IndexTable&) = delete;
        IndexTable(IndexTable&&) = delete;
        IndexTable& operator=(const IndexTable&) = delete;
        IndexTable& operator=(IndexTable&&) = delete;

        iterator begin() { return list_.begin(); }
        const_iterator begin() const { return list_.begin(); }
        void clear();
        bool empty() const noexcept { return list_.empty(); }
        iterator end() { return list_.end(); }
        const_iterator end() const { return list_.end(); }
        void erase(iterator i);
        void erase(iterator i1, iterator i2);
        void insert(const T& t);
        template <typename InputRange> void insert(const InputRange& src) { for (auto& t: src) insert(t); }
        template <typename Iterator> void insert(Iterator i1, Iterator i2) { for (; i1 != i2; ++i1) insert(*i1); }
        void push_back(const T& t) { insert(t); }
        size_t size() const noexcept { return list_.size(); }

    private:

        template <typename K, typename T2, IndexMode M> friend class Index;

        using ref_base = Detail::IndexRefBase<T>;
        using ref_ptr = std::shared_ptr<ref_base>;
        using ref_map = std::unordered_map<void*, ref_ptr>;
        using ref_pair = typename ref_map::value_type;

        ref_map indices_;
        list_type list_;

    };

        template <typename T>
        template <typename InputRange>
        IndexTable<T>::IndexTable(const InputRange& src):
        indices_(), list_(begin(src), end(src)) {}

        template <typename T>
        template <typename Iterator>
        IndexTable<T>::IndexTable(Iterator i1, Iterator i2):
        indices_(), list_(i1, i2) {}

        template <typename T>
        void IndexTable<T>::clear() {
            list_.clear();
            for (auto& pair: indices_)
                pair.second->clear();
        }

        template <typename T>
        void IndexTable<T>::erase(iterator i) {
            for (auto& pair: indices_)
                pair.second->erase(i);
            list_.erase(i);
        }

        template <typename T>
        void IndexTable<T>::erase(iterator i1, iterator i2) {
            for (iterator i = i1; i != i2; ++i)
                for (auto& pair: indices_)
                    pair.second->erase(i);
            list_.erase(i1, i2);
        }

        template <typename T>
        void IndexTable<T>::insert(const T& t) {
            list_.push_back(t);
            iterator i = std::prev(end());
            auto guard = on_scope_failure([this,i] {
                list_.pop_back();
                for (auto& [k,v]: indices_)
                    v->erase(i);
            });
            for (auto& [k,v]: indices_)
                v->insert(i);
        }

    template <typename K, typename T, IndexMode M>
    class Index {

    private:

        using list_type = std::list<T>;
        using list_iterator = typename list_type::iterator;
        using map_traits = Detail::IndexMapTraits<K, list_iterator, M>;
        using map_type = typename map_traits::map_type;
        using map_const_iterator = typename map_type::const_iterator;
        using map_iterator = typename map_type::iterator;

        template <typename CT, typename MapIter> class basic_iterator:
        public BidirectionalIterator<basic_iterator<CT, MapIter>, CT> {

        public:

            basic_iterator() = default;
            CT& operator*() const noexcept { return *iter->second; }
            basic_iterator(const basic_iterator<T, map_iterator>& i) noexcept: iter(i.iter) {}
            basic_iterator& operator=(const basic_iterator<T, map_iterator>& i) noexcept { iter = i.iter; return *this; }

            basic_iterator& operator++() { ++iter; return *this; }
            basic_iterator& operator--() { --iter; return *this; }
            bool operator==(const basic_iterator& rhs) const noexcept { return iter == rhs.iter; }
            const K& key() const noexcept { return iter->first; }

        private:

            friend class Index;
            MapIter iter;
            basic_iterator(MapIter i): iter(i) {}

        };

    public:

        using compare_function = std::function<bool(const K&, const K&)>;
        using const_iterator = basic_iterator<const T, map_const_iterator>;
        using const_reference = const T&;
        using difference_type = ptrdiff_t;
        using extract_function = std::function<K(const T&)>;
        using iterator = basic_iterator<T, map_iterator>;
        using key_type = K;
        using reference = T&;
        using size_type = size_t;
        using table_type = IndexTable<T>;
        using value_type = T;

        static constexpr IndexMode mode = M;

        Index() = default;
        explicit Index(table_type& table);
        Index(table_type& table, extract_function extract): tab_(&table), ext_(extract), map_(std::less<K>()) { init(); }
        Index(table_type& table, extract_function extract, compare_function compare): tab_(&table), ext_(extract), map_(compare) { init(); }
        ~Index() noexcept { if (tab_) tab_->indices_.erase(this); }

        Index(const Index&) = delete;
        Index(Index&&) = delete;
        Index& operator=(const Index&) = delete;
        Index& operator=(Index&&) = delete;

        iterator begin() { return map_.begin(); }
        const_iterator begin() const { return map_.begin(); }
        compare_function compare() const { return map_.key_comp(); }
        size_t count(const K& k) const noexcept { return map_.count(k); }
        bool empty() const noexcept { return map_.empty(); }
        iterator end() { return map_.end(); }
        const_iterator end() const { return map_.end(); }
        Irange<iterator> equal_range(const K& k);
        Irange<const_iterator> equal_range(const K& k) const;
        void erase(iterator i);
        void erase(iterator i1, iterator i2);
        void erase(const K& k);
        extract_function extract() const { return ext_; }
        iterator find(const K& k) { return map_.find(k); }
        const_iterator find(const K& k) const { return map_.find(k); }
        iterator lower_bound(const K& k) { return map_.lower_bound(k); }
        const_iterator lower_bound(const K& k) const { return map_.lower_bound(k); }
        size_t size() const noexcept { return map_.size(); }
        table_type& table() noexcept { return *tab_; }
        const table_type& table() const noexcept { return *tab_; }
        iterator upper_bound(const K& k) { return map_.upper_bound(k); }
        const_iterator upper_bound(const K& k) const { return map_.upper_bound(k); }

    private:

        friend class IndexTable<T>;
        friend class Detail::IndexRef<Index>;

        using table_iterator = typename table_type::iterator;
        using ref_type = Detail::IndexRef<Index>;

        table_type* tab_ = nullptr;
        extract_function ext_;
        map_type map_;

        void init();

    };

        template <typename K, typename T, IndexMode M>
        Index<K, T, M>::Index(table_type& table):
        tab_(&table), ext_(), map_(std::less<K>()) {
            ext_ = [] (const T& t) { return static_cast<K>(t); };
            init();
        }

        template <typename K, typename T, IndexMode M>
        Irange<typename Index<K, T, M>::iterator> Index<K, T, M>::equal_range(const K& k) {
            auto pair = map_.equal_range(k);
            return {pair.first, pair.second};
        }

        template <typename K, typename T, IndexMode M>
        Irange<typename Index<K, T, M>::const_iterator> Index<K, T, M>::equal_range(const K& k) const {
            auto pair = map_.equal_range(k);
            return {pair.first, pair.second};
        }

        template <typename K, typename T, IndexMode M>
        void Index<K, T, M>::erase(iterator i) {
            auto m = i.iter;
            auto t = m->second;
            for(auto& pair: tab_->indices_)
                if (pair.first != this)
                    pair.second->erase(t);
            tab_->list_.erase(t);
            map_.erase(m);
        }

        template <typename K, typename T, IndexMode M>
        void Index<K, T, M>::erase(iterator i1, iterator i2) {
            auto next = i1;
            while (i1 != i2) {
                ++next;
                erase(i1);
                i1 = next;
            }
        }

        template <typename K, typename T, IndexMode M>
        void Index<K, T, M>::erase(const K& k) {
            auto m = map_.find(k);
            if (m != map_.end()) {
                auto t = m->second;
                for(auto& pair: tab_->indices_)
                    if (pair.first != this)
                        pair.second->erase(t);
                tab_->list_.erase(t);
                map_.erase(m);
            }
        }

        template <typename K, typename T, IndexMode M>
        void Index<K, T, M>::init() {
            for (auto t = tab_->begin(), e = tab_->end(); t != e; ++t)
                map_traits::insert(ext_(*t), t, map_);
            tab_->indices_[this] = std::make_shared<ref_type>(*this);
        }

}
