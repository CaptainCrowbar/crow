#pragma once

#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename CT>
        class BoundedArrayIterator:
        public RandomAccessIterator<BoundedArrayIterator<CT>, CT> {

        public:

            BoundedArrayIterator() = default;
            explicit BoundedArrayIterator(CT* p) noexcept: ptr_(p) {}
            BoundedArrayIterator(const BoundedArrayIterator<std::remove_const_t<CT>>& i) noexcept: ptr_(i.ptr_) {}
            BoundedArrayIterator& operator=(const BoundedArrayIterator<std::remove_const_t<CT>>& i) noexcept
                { ptr_ = i.ptr_; return *this; }

            CT& operator*() const noexcept { return *ptr_; }
            BoundedArrayIterator& operator+=(ptrdiff_t rhs) noexcept { ptr_ += rhs; return *this; }
            ptrdiff_t operator-(BoundedArrayIterator rhs) const noexcept { return ptr_ - rhs.ptr_; }

        private:

            template <typename CT2> friend class BoundedArrayIterator;

            CT* ptr_ = nullptr;

        };

    }

    template <typename T, size_t N>
    class BoundedArray:
    public TotalOrder<BoundedArray<T, N>> {

    public:

        using const_iterator = Detail::BoundedArrayIterator<const T>;
        using const_reference = const T&;
        using difference_type = ptrdiff_t;
        using iterator = Detail::BoundedArrayIterator<T>;
        using reference = T&;
        using size_type = size_t;
        using value_type = T;

        static constexpr size_t bound = N;

        BoundedArray() noexcept = default;
        explicit BoundedArray(size_t n, const T& t = {});
        template <typename InputIterator> BoundedArray(InputIterator i, InputIterator j, std::enable_if_t<is_iterator<InputIterator>>* = nullptr);
        BoundedArray(std::initializer_list<T> list);
        ~BoundedArray() noexcept { clear(); }
        BoundedArray(const BoundedArray& ba);
        BoundedArray(BoundedArray&& ba) noexcept;
        BoundedArray& operator=(const BoundedArray& ba);
        BoundedArray& operator=(BoundedArray&& ba) noexcept;
        BoundedArray& operator=(std::initializer_list<T> list);

        T& operator[](size_t i) noexcept { return data()[i]; }
        const T& operator[](size_t i) const noexcept { return data()[i]; }

        T& at(size_t i) { check_index(i); return data()[i]; }
        const T& at(size_t i) const { check_index(i); return data()[i]; }
        iterator begin() noexcept { return iterator(data()); }
        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator cbegin() const noexcept { return const_iterator(cdata()); }
        iterator end() noexcept { return begin() + num_; }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cend() const noexcept { return cbegin() + num_; }
        T* data() noexcept { return reinterpret_cast<T*>(mem_); }
        const T* data() const noexcept { return cdata(); }
        const T* cdata() const noexcept { return reinterpret_cast<const T*>(mem_); }
        T& front() noexcept { return *data(); }
        const T& front() const noexcept { return *cdata(); }
        T& back() noexcept { return data()[num_ - 1]; }
        const T& back() const noexcept { return cdata()[num_ - 1]; }
        size_t capacity() const noexcept { return N; }
        bool empty() const noexcept { return num_ == 0; }
        size_t size() const noexcept { return num_; }

        template <typename InputIterator> iterator append(InputIterator i, InputIterator j);
        template <typename InputRange> iterator append(const InputRange& r);
        template <typename InputRange> iterator append(InputRange&& r);
        void clear() noexcept;
        template <typename... Args> iterator emplace(const_iterator i, Args&&... args);
        template <typename... Args> void emplace_back(Args&&... args);
        void erase(const_iterator i) noexcept;
        void erase(const_iterator i, const_iterator j) noexcept;
        iterator insert(const_iterator i, const T& t);
        iterator insert(const_iterator i, T&& t);
        template <typename InputIterator> iterator insert(const_iterator i, InputIterator j, InputIterator k);
        void pop_back() noexcept;
        void push_back(const T& t);
        void push_back(T&& t);
        void resize(size_t n, const T& t = {});

        size_t hash() const noexcept;
        void swap(BoundedArray& ba) noexcept;

        friend bool operator==(const BoundedArray& a, const BoundedArray& b) noexcept
            { return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }
        friend bool operator<(const BoundedArray& a, const BoundedArray& b) noexcept
            { return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }

    private:

        using raw_memory = typename std::aligned_union<1, T>::type;

        raw_memory mem_[N];
        size_t num_ = 0;

        void check_index(size_t i) const { if (i >= num_) throw std::out_of_range("Bounded array index out of bounds"); }
        void check_length(size_t n) const { if (n > N) throw std::length_error("Bounded array length exceeds bound"); }

    };

        template <typename T, size_t N>
        BoundedArray<T, N>::BoundedArray(size_t n, const T& t):
        mem_(), num_(n) {
            check_length(n);
            std::uninitialized_fill(begin(), begin() + n, t);
        }

        template <typename T, size_t N>
        template <typename InputIterator>
        BoundedArray<T, N>::BoundedArray(InputIterator i, InputIterator j, std::enable_if_t<is_iterator<InputIterator>>*) {
            using namespace Detail;
            if constexpr (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                for (; i != j; ++i)
                    push_back(*i);
            } else {
                size_t n = std::distance(i, j);
                check_length(n);
                num_ = n;
                std::uninitialized_copy(i, j, begin());
            }
        }

        template <typename T, size_t N>
        BoundedArray<T, N>::BoundedArray(std::initializer_list<T> list):
        mem_(), num_(list.size()) {
            check_length(num_);
            std::uninitialized_copy(list.begin(), list.end(), begin());
        }

        template <typename T, size_t N>
        BoundedArray<T, N>::BoundedArray(const BoundedArray& ba):
        mem_(), num_(ba.num_) {
            std::uninitialized_copy(ba.begin(), ba.end(), begin());
        }

        template <typename T, size_t N>
        BoundedArray<T, N>::BoundedArray(BoundedArray&& ba) noexcept:
        mem_(), num_(ba.num_) {
            using namespace Detail;
            std::uninitialized_move(ba.begin(), ba.end(), begin());
        }

        template <typename T, size_t N>
        BoundedArray<T, N>& BoundedArray<T, N>::operator=(const BoundedArray& ba) {
            BoundedArray temp(ba);
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        BoundedArray<T, N>& BoundedArray<T, N>::operator=(BoundedArray&& ba) noexcept {
            BoundedArray temp(std::move(ba));
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        BoundedArray<T, N>& BoundedArray<T, N>::operator=(std::initializer_list<T> list) {
            BoundedArray temp(list);
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        template <typename InputIterator>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::append(InputIterator i, InputIterator j) {
            using namespace Detail;
            size_t n_old = num_;
            if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                for (; i != j; ++i)
                    push_back(*i);
            } else {
                size_t n_new = std::distance(i, j);
                check_length(num_ + n_new);
                std::uninitialized_copy(i, j, begin() + n_old);
                num_ += n_new;
            }
            return begin() + n_old;
        }

        template <typename T, size_t N>
        template <typename InputRange>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::append(const InputRange& r) {
            using std::begin;
            using std::end;
            return append(begin(r), end(r));
        }

        template <typename T, size_t N>
        template <typename InputRange>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::append(InputRange&& r) {
            using namespace Detail;
            using std::begin;
            using std::end;
            auto i = begin(r), j = end(r);
            size_t n_old = num_;
            if (std::is_same<typename std::iterator_traits<decltype(i)>::iterator_category, std::input_iterator_tag>::value) {
                for (; i != j; ++i)
                    push_back(*i);
            } else {
                size_t n_new = std::distance(i, j);
                check_length(num_ + n_new);
                std::uninitialized_move(i, j, this->begin() + n_old);
                num_ += n_new;
            }
            return this->begin() + n_old;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::clear() noexcept {
            using namespace Detail;
            std::destroy(begin(), end());
            num_ = 0;
        }

        template <typename T, size_t N>
        template <typename... Args>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::emplace(const_iterator i, Args&&... args) {
            check_length(num_ + 1);
            size_t pos = i - begin();
            if (pos < num_) {
                new (data() + num_) T(std::move(end()[-1]));
                std::move_backward(begin() + pos, end() - 1, end());
                begin()[pos].~T();
            }
            new (data() + pos) T(std::forward<Args>(args)...);
            ++num_;
            return begin() + pos;
        }

        template <typename T, size_t N>
        template <typename... Args>
        void BoundedArray<T, N>::emplace_back(Args&&... args) {
            check_length(num_ + 1);
            new (data() + num_) T(std::forward<Args>(args)...);
            ++num_;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::erase(const_iterator i) noexcept {
            auto x = i - cbegin();
            std::move(begin() + x + 1, end(), begin() + x);
            end()[-1].~T();
            --num_;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::erase(const_iterator i, const_iterator j) noexcept {
            using namespace Detail;
            size_t n_erase = j - i;
            auto x = i - cbegin();
            auto y = j - cbegin();
            std::move(begin() + y, end(), begin() + x);
            std::destroy(end() - n_erase, end());
            num_ -= n_erase;
        }

        template <typename T, size_t N>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::insert(const_iterator i, const T& t) {
            check_length(num_ + 1);
            size_t pos = i - begin();
            if (pos < num_) {
                new (data() + num_) T(std::move(end()[-1]));
                std::move_backward(begin() + pos, end() - 1, end());
                begin()[pos] = t;
            } else {
                new (data() + pos) T(t);
            }
            ++num_;
            return begin() + pos;
        }

        template <typename T, size_t N>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::insert(const_iterator i, T&& t) {
            check_length(num_ + 1);
            size_t pos = i - begin();
            if (pos < num_) {
                new (data() + num_) T(std::move(end()[-1]));
                std::move_backward(begin() + pos, end() - 1, end());
                begin()[pos] = std::move(t);
            } else {
                new (data() + pos) T(std::move(t));
            }
            ++num_;
            return begin() + pos;
        }

        template <typename T, size_t N>
        template <typename InputIterator>
        typename BoundedArray<T, N>::iterator BoundedArray<T, N>::insert(const_iterator i, InputIterator j, InputIterator k) {
            using namespace Detail;
            size_t n_before = i - begin(), n_after = num_ - n_before;
            if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                BoundedArray temp(i, cend());
                erase(i, end());
                for (; j != k; ++j)
                    push_back(*j);
                append(std::move(temp));
            } else {
                size_t n_inserted = std::distance(j, k);
                check_length(num_ + n_inserted);
                auto insert_at = begin() + n_before;
                if (n_inserted < n_after) {
                    std::uninitialized_move(end() - n_inserted, end(), end());
                    std::move_backward(insert_at, end() - n_inserted, end());
                    std::copy(j, k, insert_at);
                } else {
                    std::uninitialized_move(insert_at, end(), end() + n_inserted - n_after);
                    auto mid = j;
                    std::advance(mid, n_after);
                    std::copy(j, mid, insert_at);
                    std::uninitialized_copy(mid, k, end());
                }
                num_ += n_inserted;
            }
            return begin() + n_before;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::pop_back() noexcept {
            end()[-1].~T();
            --num_;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::push_back(const T& t) {
            check_length(num_ + 1);
            new (data() + num_) T(t);
            ++num_;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::push_back(T&& t) {
            check_length(num_ + 1);
            new (data() + num_) T(std::move(t));
            ++num_;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::resize(size_t n, const T& t) {
            check_length(n);
            if (n < num_)
                std::destroy(begin() + n, end());
            else if (n > num_)
                std::uninitialized_fill(begin() + num_, begin() + n, t);
            num_ = n;
        }

        template <typename T, size_t N>
        size_t BoundedArray<T, N>::hash() const noexcept {
            size_t h = 0;
            std::hash<T> ht;
            for (auto& t: *this)
                h = (h << 1) + ht(t);
            return h;
        }

        template <typename T, size_t N>
        void BoundedArray<T, N>::swap(BoundedArray& ba) noexcept {
            using namespace Detail;
            size_t common = std::min(num_, ba.num_);
            std::swap_ranges(begin(), begin() + common, ba.begin());
            if (num_ > common) {
                std::uninitialized_move(begin() + common, end(), ba.begin() + common);
                std::destroy(begin() + common, end());
            } else if (ba.num_ > common) {
                std::uninitialized_move(ba.begin() + common, ba.end(), begin() + common);
                std::destroy(ba.begin() + common, ba.end());
            }
            std::swap(num_, ba.num_);
        }

        template <typename T, size_t N>
        void swap(BoundedArray<T, N>& a, BoundedArray<T, N>& b) noexcept {
            a.swap(b);
        }

}

namespace std {

    template <typename T, size_t N>
    struct hash<Crow::BoundedArray<T, N>> {
        size_t operator()(Crow::BoundedArray<T, N> ba) const noexcept {
            return ba.hash();
        }
    };

}
