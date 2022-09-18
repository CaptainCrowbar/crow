#pragma once

#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <numeric>
#include <type_traits>

namespace Crow {

    template <typename T, int N>
    class MultiArray {

    private:

        static_assert(N >= 1);

        template <typename CMA, typename CT>
        class basic_iterator {
        public:

            using difference_type = int;
            using iterator_category = std::forward_iterator_tag;
            using pointer = CT*;
            using reference = CT&;
            using value_type = T;

            basic_iterator() = default;
            basic_iterator(const basic_iterator<std::remove_const_t<CMA>, std::remove_const_t<CT>>& i):
                owner_(i.owner_), data_(i.data_), index_(i.index_) {}

            CT& operator*() const noexcept { return data_[index_]; }
            CT* operator->() const noexcept { return &**this; }

            basic_iterator& operator++() noexcept { ++index_; return *this; }
            basic_iterator operator++(int) noexcept { auto i = *this; ++*this; return i; }

            bool operator==(const basic_iterator& i) const noexcept { return index_ == i.index_; }
            bool operator!=(const basic_iterator& i) const noexcept { return ! (*this == i); }

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
                owner_(&owner), data_(owner.data_.get()), index_(index) {}

        };

        template <typename... Args>
            using int_args = std::enable_if_t<sizeof...(Args) + 1 == N, int>;

    public:

        using iterator = basic_iterator<MultiArray, T>;
        using const_iterator = basic_iterator<const MultiArray, const T>;
        using position = Vector<int, N>;
        using value_type = T;

        static constexpr int dim = N;

        MultiArray() = default;
        explicit MultiArray(const position& shape);
        explicit MultiArray(const position& shape, const T& t);
        template <typename... Args>
            explicit MultiArray(int_args<Args...> x, Args... args):
            MultiArray(position(x, args...)) {}
        MultiArray(const MultiArray& a);
        MultiArray(MultiArray&& a) = default;
        ~MultiArray() = default;
        MultiArray& operator=(const MultiArray& a) { auto b(a); swap(b); return *this; }
        MultiArray& operator=(MultiArray&& a) = default;

        T& operator[](const position& p) noexcept { return ref(p); }
        const T& operator[](const position& p) const noexcept { return get(p); }
        template <typename... Args>
            T& operator()(int_args<Args...> x, Args... args) noexcept
            { return ref(x, args...); }
        template <typename... Args>
            const T& operator()(int_args<Args...> x, Args... args) const noexcept
            { return get(x, args...); }

        iterator begin() noexcept { return iterator(*this, 0); }
        const_iterator begin() const noexcept { return const_iterator(*this, 0); }
        iterator end() noexcept { return iterator(*this, int(size())); }
        const_iterator end() const noexcept { return const_iterator(*this, int(size())); }
        T* data() noexcept { return data_.ref(); }
        const T* data() const noexcept { return data_.get(); }

        iterator locate(const position& p) noexcept { return iterator(*this, position_to_index(p)); }
        const_iterator locate(const position& p) const noexcept { return const_iterator(*this, position_to_index(p)); }
        template <typename... Args>
            iterator locate(int_args<Args...> x, Args... args) noexcept
            { return iterator(*this, position_to_index({x, args...})); }
        template <typename... Args>
            const_iterator locate(int_args<Args...> x, Args... args) const noexcept
            { return const_iterator(*this, position_to_index({x, args...})); }
        T& ref(const position& p) noexcept { return data_[position_to_index(p)]; }
        template <typename... Args>
            T& ref(int_args<Args...> x, Args... args) noexcept
            { return data_[position_to_index({x, args...})]; }
        const T& get(const position& p) const noexcept { return data_[position_to_index(p)]; }
        template <typename... Args>
            const T& get(int_args<Args...> x, Args... args) const noexcept
            { return data_[position_to_index({x, args...})]; }

        bool contains(const position& p) const noexcept;
        template <typename... Args>
            bool contains(int_args<Args...> x, Args... args) const noexcept
            { return contains({x, args...}); }
        bool empty() const noexcept { return size() == 0; }
        position shape() const noexcept { return shape_; }
        size_t size() const noexcept { return size_t(factors_[N]); }

        void clear() noexcept { shape_ = position(0); data_.reset(); }
        void fill(const T& t) { std::fill(begin(), end(), t); }
        void reset(const position& shape) { MultiArray a(shape); swap(a); }
        void reset(const position& shape, const T& t) { MultiArray a(shape, t); swap(a); }
        template <typename... Args>
            void reset(int_args<Args...> x, Args... args)
            { MultiArray a(x, args...); swap(a); }

        void swap(MultiArray& a) noexcept;
        friend void swap(MultiArray& a, MultiArray& b) noexcept { a.swap(b); }

        friend bool operator==(const MultiArray& a, const MultiArray& b) noexcept
            { return a.shape_ == b.shape_ && std::equal(a.begin(), a.end(), b.begin(), b.end()); }
        friend bool operator!=(const MultiArray& a, const MultiArray& b) noexcept { return ! (a == b); }

    private:

        position shape_{0};
        Vector<int, N + 1> factors_;
        std::unique_ptr<T[]> data_;

        position index_to_position(int i) const noexcept;
        int position_to_index(const position& p) const noexcept;
        void set_factors() noexcept;

    };

        template <typename T, int N>
        MultiArray<T, N>::MultiArray(const position& shape):
        shape_(shape), factors_(), data_() {
            set_factors();
            data_.reset(new T[size()]);
        }

        template <typename T, int N>
        MultiArray<T, N>::MultiArray(const position& shape, const T& t):
        shape_(shape), factors_(), data_() {
            set_factors();
            data_.reset(new T[size()]);
            fill(t);
        }

        template <typename T, int N>
        MultiArray<T, N>::MultiArray(const MultiArray& a):
        shape_(a.shape_), factors_(a.factors_), data_(new T[size()]) {
            std::copy(a.begin(), a.end(), begin());
        }

        template <typename T, int N>
        bool MultiArray<T, N>::contains(const position& p) const noexcept {
            for (int i = 0; i < N; ++i)
                if (p[i] < 0 || p[i] >= shape_[i])
                    return false;
            return true;
        }

        template <typename T, int N>
        void MultiArray<T, N>::swap(MultiArray& a) noexcept {
            std::swap(shape_, a.shape_);
            std::swap(factors_, a.factors_);
            std::swap(data_, a.data_);
        }

        template <typename T, int N>
        typename MultiArray<T, N>::position MultiArray<T, N>::index_to_position(int i) const noexcept {
            position p(0);
            if (! empty()) {
                for (int j = N - 1; j >= 0; --j) {
                    p[j] = i / factors_[j];
                    i %= factors_[j];
                }
            }
            return p;
        }

        template <typename T, int N>
        int MultiArray<T, N>::position_to_index(const position& p) const noexcept {
            return std::inner_product(p.begin(), p.end(), factors_.begin(), 0);
        }

        template <typename T, int N>
        void MultiArray<T, N>::set_factors() noexcept {
            factors_[0] = 1;
            for (int i = 0; i < N; ++i)
                factors_[i + 1] = factors_[i] * shape_[i];
        }

}
