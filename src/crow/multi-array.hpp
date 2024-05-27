#pragma once

#include "crow/iterator.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <numeric>
#include <type_traits>
#include <vector>

namespace Crow {

    template <std::copyable T, int N, ContiguousContainerType C = std::vector<T>>
    requires (std::same_as<T, typename C::value_type> && N >= 1)
    class MultiArray {

    private:

        template <typename CMA, typename CT>
        class basic_iterator:
        public BidirectionalIterator<basic_iterator<CMA, CT>, CT> {

        public:

            basic_iterator() = default;
            basic_iterator(const basic_iterator<std::remove_const_t<CMA>, std::remove_const_t<CT>>& i):
                owner_(i.owner_), data_(i.data_), index_(i.index_) {}

            CT& operator*() const noexcept { return data_[index_]; }
            basic_iterator& operator++() noexcept { ++index_; return *this; }
            basic_iterator& operator--() noexcept { --index_; return *this; }
            bool operator==(const basic_iterator& i) const noexcept { return index_ == i.index_; }

            basic_iterator& move(int axis, int distance = 1) noexcept {
                index_ += owner_->factors_[axis] * distance;
                return *this;
            }

            Vector<int, N> pos() const noexcept { return owner_->index_to_position(index_); }

        private:

            friend class MultiArray;

            CMA* owner_;
            CT* data_;
            int index_;

            basic_iterator(CMA& owner, int index) noexcept:
                owner_(&owner), data_(owner.store_.data()), index_(index) {}

        };

        template <typename... Args>
            using int_arg = std::enable_if_t<sizeof...(Args) + 1 == N, int>;

    public:

        using iterator = basic_iterator<MultiArray, T>;
        using const_iterator = basic_iterator<const MultiArray, const T>;
        using position = Vector<int, N>;
        using value_type = T;

        static constexpr int dim = N;

        MultiArray() = default;
        explicit MultiArray(const position& shape) { do_reset(shape); }
        explicit MultiArray(const position& shape, const T& t) { do_reset(shape, t); }
        template <typename... Args> explicit MultiArray(int_arg<Args...> x, Args... args):
            MultiArray(position(x, args...)) {}
        MultiArray(const MultiArray& a) = default;
        MultiArray(MultiArray&& a) = default;
        ~MultiArray() = default;
        MultiArray& operator=(const MultiArray& a) = default;
        MultiArray& operator=(MultiArray&& a) = default;

        T& operator[](const position& p) noexcept { return ref(p); }
        const T& operator[](const position& p) const noexcept { return get(p); }
        template <typename... Args> T& operator()(int_arg<Args...> x, Args... args) noexcept
            { return ref(x, args...); }
        template <typename... Args> const T& operator()(int_arg<Args...> x, Args... args) const noexcept
            { return get(x, args...); }

        iterator begin() noexcept { return iterator(*this, 0); }
        const_iterator begin() const noexcept { return const_iterator(*this, 0); }
        iterator end() noexcept { return iterator(*this, int(size())); }
        const_iterator end() const noexcept { return const_iterator(*this, int(size())); }
        T* data() noexcept { return store_.data(); }
        const T* data() const noexcept { return store_.data(); }

        iterator locate(const position& p) noexcept { return iterator(*this, position_to_index(p)); }
        const_iterator locate(const position& p) const noexcept { return const_iterator(*this, position_to_index(p)); }
        template <typename... Args> iterator locate(int_arg<Args...> x, Args... args) noexcept
            { return iterator(*this, position_to_index({x, args...})); }
        template <typename... Args> const_iterator locate(int_arg<Args...> x, Args... args) const noexcept
            { return const_iterator(*this, position_to_index({x, args...})); }
        T& ref(const position& p) noexcept { return store_[position_to_index(p)]; }
        template <typename... Args> T& ref(int_arg<Args...> x, Args... args) noexcept
            { return store_[position_to_index({x, args...})]; }
        const T& get(const position& p) const noexcept { return store_[position_to_index(p)]; }
        template <typename... Args> const T& get(int_arg<Args...> x, Args... args) const noexcept
            { return store_[position_to_index({x, args...})]; }

        bool contains(const position& p) const noexcept;
        template <typename... Args> bool contains(int_arg<Args...> x, Args... args) const noexcept
            { return contains({x, args...}); }
        bool empty() const noexcept { return size() == 0; }
        position shape() const noexcept { return shape_; }
        size_t size() const noexcept { return size_t(factors_[N]); }

        void clear() noexcept { shape_ = position(0); store_.clear(); }
        void fill(const T& t) { std::fill(begin(), end(), t); }

        void reset(const position& shape) { do_reset(shape); }
        void reset(const position& shape, const T& t) { do_reset(shape, t); }
        template <typename... Args> void reset(int_arg<Args...> x, Args... args)
            { do_reset(position(x, args...)); }

        void swap(MultiArray& a) noexcept;
        friend void swap(MultiArray& a, MultiArray& b) noexcept { a.swap(b); }

        friend bool operator==(const MultiArray& a, const MultiArray& b) noexcept
            { return a.shape_ == b.shape_ && std::equal(a.begin(), a.end(), b.begin(), b.end()); }

    private:

        C store_;
        position shape_{0};
        Vector<int, N + 1> factors_;

        void do_reset(const position& shape, const T& t = {});
        position index_to_position(int i) const noexcept;
        int position_to_index(const position& p) const noexcept
            { return std::inner_product(p.begin(), p.end(), factors_.begin(), 0); }

    };

        template <std::copyable T, int N, ContiguousContainerType C>
        requires (std::same_as<T, typename C::value_type> && N >= 1)
        bool MultiArray<T, N, C>::contains(const position& p) const noexcept {
            for (int i = 0; i < N; ++i)
                if (p[i] < 0 || p[i] >= shape_[i])
                    return false;
            return true;
        }

        template <std::copyable T, int N, ContiguousContainerType C>
        requires (std::same_as<T, typename C::value_type> && N >= 1)
        void MultiArray<T, N, C>::swap(MultiArray& a) noexcept {
            std::swap(store_, a.store_);
            std::swap(shape_, a.shape_);
            std::swap(factors_, a.factors_);
        }

        template <std::copyable T, int N, ContiguousContainerType C>
        requires (std::same_as<T, typename C::value_type> && N >= 1)
        void MultiArray<T, N, C>::do_reset(const position& shape, const T& t) {
            size_t old_size = factors_[N];
            shape_ = shape;
            factors_[0] = 1;
            for (int i = 0; i < N; ++i)
                factors_[i + 1] = factors_[i] * shape_[i];
            size_t new_size = factors_[N];
            store_.resize(size_t(new_size), t);
            std::fill(store_.begin(), store_.begin() + std::min(old_size, new_size), t);
        }

        template <std::copyable T, int N, ContiguousContainerType C>
        requires (std::same_as<T, typename C::value_type> && N >= 1)
        typename MultiArray<T, N, C>::position MultiArray<T, N, C>::index_to_position(int i) const noexcept {
            position p(0);
            if (! empty()) {
                for (int j = N - 1; j >= 0; --j) {
                    p[j] = i / factors_[j];
                    i %= factors_[j];
                }
            }
            return p;
        }

}
