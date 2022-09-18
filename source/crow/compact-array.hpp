#pragma once

#include "crow/binary.hpp"
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
        class CompactArrayIterator:
        public RandomAccessIterator<CompactArrayIterator<CT>, CT> {

        public:

            CompactArrayIterator() = default;
            explicit CompactArrayIterator(CT* p) noexcept: ptr_(p) {}
            CompactArrayIterator(const CompactArrayIterator<std::remove_const_t<CT>>& i) noexcept: ptr_(i.ptr_) {}
            CompactArrayIterator& operator=(const CompactArrayIterator<std::remove_const_t<CT>>& i) noexcept
                { ptr_ = i.ptr_; return *this; }

            CT& operator*() const noexcept { return *ptr_; }
            CompactArrayIterator& operator+=(ptrdiff_t rhs) noexcept { ptr_ += rhs; return *this; }
            ptrdiff_t operator-(CompactArrayIterator rhs) const noexcept { return ptr_ - rhs.ptr_; }

        private:

            template <typename CT2> friend class CompactArrayIterator;

            CT* ptr_ = nullptr;

        };

    }

    template <typename T, size_t N>
    class CompactArray:
    public TotalOrder<CompactArray<T, N>> {

    public:

        using const_iterator = Detail::CompactArrayIterator<const T>;
        using const_reference = const T&;
        using difference_type = ptrdiff_t;
        using iterator = Detail::CompactArrayIterator<T>;
        using reference = T&;
        using size_type = size_t;
        using value_type = T;

        static constexpr size_t threshold = N;

        CompactArray() noexcept = default;
        explicit CompactArray(size_t n, const T& t = {});
        template <typename InputIterator> CompactArray(InputIterator i, InputIterator j, std::enable_if_t<is_iterator<InputIterator>>* = nullptr);
        CompactArray(std::initializer_list<T> list);
        ~CompactArray() noexcept { clear(); }
        CompactArray(const CompactArray& ca);
        CompactArray(CompactArray&& ca) noexcept;
        CompactArray& operator=(const CompactArray& ca);
        CompactArray& operator=(CompactArray&& ca) noexcept;
        CompactArray& operator=(std::initializer_list<T> list);

        T& operator[](size_t i) noexcept { return data()[i]; }
        const T& operator[](size_t i) const noexcept { return data()[i]; }

        T& at(size_t i) { check_index(i); return data()[i]; }
        const T& at(size_t i) const { check_index(i); return data()[i]; }
        iterator begin() noexcept { return iterator(data()); }
        const_iterator begin() const noexcept { return const_iterator(cdata()); }
        const_iterator cbegin() const noexcept { return const_iterator(cdata()); }
        iterator end() noexcept { return begin() + num_; }
        const_iterator end() const noexcept { return begin() + num_; }
        const_iterator cend() const noexcept { return cbegin() + num_; }
        T* data() noexcept { return reinterpret_cast<T*>(local_ ? uni_.mem : uni_.pc.ptr); }
        const T* data() const noexcept { return cdata(); }
        const T* cdata() const noexcept { return reinterpret_cast<const T*>(local_ ? uni_.mem : uni_.pc.ptr); }
        T& front() noexcept { return *data(); }
        const T& front() const noexcept { return *cdata(); }
        T& back() noexcept { return data()[num_ - 1]; }
        const T& back() const noexcept { return cdata()[num_ - 1]; }
        size_t capacity() const noexcept { return local_ ? N : uni_.pc.cap; }
        bool empty() const noexcept { return num_ == 0; }
        bool is_compact() const noexcept { return local_; }
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
        void reserve(size_t n);
        void resize(size_t n, const T& t = {});
        void shrink_to_fit();

        size_t hash() const noexcept;
        void swap(CompactArray& ca) noexcept;

        friend bool operator==(const CompactArray& a, const CompactArray& b) noexcept
            { return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }
        friend bool operator<(const CompactArray& a, const CompactArray& b) noexcept
            { return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }

    private:

        using raw_memory = typename std::aligned_union<1, T>::type;

        union {
            raw_memory mem[N];
            struct {
                raw_memory* ptr;
                size_t cap;
            } pc;
        } uni_;

        size_t num_ = 0;
        bool local_ = true;

        void check_index(size_t i) const { if (i >= num_) throw std::out_of_range("Compact array index out of bounds"); }

    };

        template <typename T, size_t N>
        CompactArray<T, N>::CompactArray(size_t n, const T& t) {
            reserve(n);
            std::uninitialized_fill(begin(), begin() + n, t);
            num_ = n;
        }

        template <typename T, size_t N>
        template <typename InputIterator>
        CompactArray<T, N>::CompactArray(InputIterator i, InputIterator j, std::enable_if_t<is_iterator<InputIterator>>*) {
            using namespace Detail;
            if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                for (; i != j; ++i)
                    push_back(*i);
            } else {
                size_t n = std::distance(i, j);
                reserve(n);
                std::uninitialized_copy(i, j, begin());
                num_ = n;
            }
        }

        template <typename T, size_t N>
        CompactArray<T, N>::CompactArray(std::initializer_list<T> list) {
            reserve(list.size());
            std::uninitialized_copy(list.begin(), list.end(), begin());
            num_ = list.size();
        }

        template <typename T, size_t N>
        CompactArray<T, N>::CompactArray(const CompactArray& ca) {
            reserve(ca.size());
            std::uninitialized_copy(ca.begin(), ca.end(), begin());
            num_ = ca.num_;
        }

        template <typename T, size_t N>
        CompactArray<T, N>::CompactArray(CompactArray&& ca) noexcept {
            using namespace Detail;
            reserve(ca.size());
            std::uninitialized_move(ca.begin(), ca.end(), begin());
            num_ = ca.num_;
            ca.clear();
        }

        template <typename T, size_t N>
        CompactArray<T, N>& CompactArray<T, N>::operator=(const CompactArray& ca) {
            CompactArray temp(ca);
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        CompactArray<T, N>& CompactArray<T, N>::operator=(CompactArray&& ca) noexcept {
            CompactArray temp(std::move(ca));
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        CompactArray<T, N>& CompactArray<T, N>::operator=(std::initializer_list<T> list) {
            CompactArray temp(list);
            swap(temp);
            return *this;
        }

        template <typename T, size_t N>
        template <typename InputIterator>
        typename CompactArray<T, N>::iterator CompactArray<T, N>::append(InputIterator i, InputIterator j) {
            using namespace Detail;
            size_t n_old = num_;
            if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                for (; i != j; ++i)
                    push_back(*i);
            } else {
                size_t n_new = std::distance(i, j);
                reserve(num_ + n_new);
                std::uninitialized_copy(i, j, begin() + n_old);
                num_ += n_new;
            }
            return begin() + n_old;
        }

        template <typename T, size_t N>
        template <typename InputRange>
        typename CompactArray<T, N>::iterator CompactArray<T, N>::append(const InputRange& r) {
            using std::begin;
            using std::end;
            return append(begin(r), end(r));
        }

        template <typename T, size_t N>
        template <typename InputRange>
        typename CompactArray<T, N>::iterator CompactArray<T, N>::append(InputRange&& r) {
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
                reserve(num_ + n_new);
                std::uninitialized_move(i, j, this->begin() + n_old);
                num_ += n_new;
            }
            return this->begin() + n_old;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::clear() noexcept {
            using namespace Detail;
            std::destroy(begin(), end());
            if (! local_)
                delete[] uni_.pc.ptr;
            num_ = 0;
            local_ = true;
        }

        template <typename T, size_t N>
        template <typename... Args>
        typename CompactArray<T, N>::iterator CompactArray<T, N>::emplace(const_iterator i, Args&&... args) {
            size_t pos = i - begin();
            reserve(num_ + 1);
            if (pos < num_) {
                new (data() + num_) T(std::move(end()[-1]));
                std::move_backward(begin() + pos, end() - 1, end());
                begin()[pos].~T();
            }
            new (data() + pos) T(args...);
            ++num_;
            return begin() + pos;
        }

        template <typename T, size_t N>
        template <typename... Args>
        void CompactArray<T, N>::emplace_back(Args&&... args) {
            reserve(num_ + 1);
            new (data() + num_) T(args...);
            ++num_;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::erase(const_iterator i) noexcept {
            auto x = i - cbegin();
            std::move(begin() + x + 1, end(), begin() + x);
            end()[-1].~T();
            --num_;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::erase(const_iterator i, const_iterator j) noexcept {
            using namespace Detail;
            size_t n_erase = j - i;
            auto x = i - cbegin();
            auto y = j - cbegin();
            std::move(begin() + y, end(), begin() + x);
            std::destroy(end() - n_erase, end());
            num_ -= n_erase;
        }

        template <typename T, size_t N>
        typename CompactArray<T, N>::iterator CompactArray<T, N>::insert(const_iterator i, const T& t) {
            size_t pos = i - begin();
            reserve(num_ + 1);
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
        typename CompactArray<T, N>::iterator CompactArray<T, N>::insert(const_iterator i, T&& t) {
            size_t pos = i - begin();
            reserve(num_ + 1);
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
        typename CompactArray<T, N>::iterator CompactArray<T, N>::insert(const_iterator i, InputIterator j, InputIterator k) {
            using namespace Detail;
            size_t n_before = i - begin(), n_after = num_ - n_before;
            if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value) {
                CompactArray temp(i, cend());
                erase(i, end());
                for (; j != k; ++j)
                    push_back(*j);
                append(std::move(temp));
            } else {
                size_t n_inserted = std::distance(j, k);
                reserve(num_ + n_inserted);
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
        void CompactArray<T, N>::pop_back() noexcept {
            end()[-1].~T();
            --num_;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::push_back(const T& t) {
            reserve(num_ + 1);
            new (data() + num_) T(t);
            ++num_;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::push_back(T&& t) {
            reserve(num_ + 1);
            new (data() + num_) T(std::move(t));
            ++num_;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::reserve(size_t n) {
            using namespace Detail;
            if (n <= num_) {
                std::destroy(begin() + n, end());
                num_ = n;
                return;
            }
            if (n <= N)
                return;
            size_t new_cap = bit_ceil(n);
            if (new_cap <= capacity())
                return;
            auto new_ptr = new raw_memory[new_cap];
            auto new_t_ptr = reinterpret_cast<T*>(new_ptr);
            std::uninitialized_move(begin(), end(), new_t_ptr);
            std::destroy(begin(), end());
            if (! local_)
                delete[] uni_.pc.ptr;
            uni_.pc.ptr = new_ptr;
            uni_.pc.cap = new_cap;
            local_ = false;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::resize(size_t n, const T& t) {
            reserve(n);
            if (n > num_) {
                std::uninitialized_fill(begin() + num_, begin() + n, t);
                num_ = n;
            }
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::shrink_to_fit() {
            using namespace Detail;
            if (local_) {
                // Already at minimum capacity, nothing to do
            } else if (num_ <= N) {
                auto old_ptr = uni_.pc.ptr;
                auto old_t_ptr = reinterpret_cast<T*>(old_ptr);
                local_ = true;
                std::uninitialized_move(old_t_ptr, old_t_ptr + num_, begin());
                std::destroy(old_t_ptr, old_t_ptr + num_);
                delete[] old_ptr;
            } else {
                auto new_ptr = new raw_memory[num_];
                auto new_t_ptr = reinterpret_cast<T*>(new_ptr);
                std::uninitialized_move(begin(), end(), new_t_ptr);
                std::destroy(begin(), end());
                delete[] uni_.pc.ptr;
                uni_.pc.ptr = new_ptr;
                uni_.pc.cap = num_;
            }
        }

        template <typename T, size_t N>
        size_t CompactArray<T, N>::hash() const noexcept {
            size_t h = 0;
            std::hash<T> ht;
            for (auto& t: *this)
                h = (h << 1) + ht(t);
            return h;
        }

        template <typename T, size_t N>
        void CompactArray<T, N>::swap(CompactArray& ca) noexcept {
            using namespace Detail;
            if (local_ && ca.local_) {
                size_t common = std::min(num_, ca.num_);
                std::swap_ranges(begin(), begin() + common, ca.begin());
                if (num_ > common) {
                    std::uninitialized_move(begin() + common, end(), ca.begin() + common);
                    std::destroy(begin() + common, end());
                } else if (ca.num_ > common) {
                    std::uninitialized_move(ca.begin() + common, ca.end(), begin() + common);
                    std::destroy(ca.begin() + common, ca.end());
                }
                std::swap(num_, ca.num_);
            } else if (local_) {
                auto p = ca.uni_.pc.ptr;
                auto c = ca.uni_.pc.cap;
                auto ca_ptr = reinterpret_cast<T*>(ca.uni_.mem);
                std::uninitialized_move(begin(), end(), ca_ptr);
                std::destroy(begin(), end());
                std::swap(num_, ca.num_);
                std::swap(local_, ca.local_);
                uni_.pc.ptr = p;
                uni_.pc.cap = c;
            } else if (ca.local_) {
                ca.swap(*this);
            } else {
                std::swap(uni_.pc.ptr, ca.uni_.pc.ptr);
                std::swap(uni_.pc.cap, ca.uni_.pc.cap);
                std::swap(num_, ca.num_);
            }
        }

        template <typename T, size_t N>
        void swap(CompactArray<T, N>& a, CompactArray<T, N>& b) noexcept {
            a.swap(b);
        }

}

namespace std {

    template <typename T, size_t N>
    struct hash<Crow::CompactArray<T, N>> {
        size_t operator()(Crow::CompactArray<T, N> ca) const noexcept {
            return ca.hash();
        }
    };

}
