#pragma once

#include "crow/types.hpp"
#include <utility>
#include <vector>

namespace Crow {

    template <typename T>
    class Stack {

    public:

        using const_iterator = typename std::vector<T>::const_iterator;
        using iterator = typename std::vector<T>::iterator;
        using value_type = T;

        Stack() = default;
        ~Stack() noexcept { clear(); }
        Stack(const Stack&) = delete;
        Stack(Stack&& s) = default;
        Stack& operator=(const Stack&) = delete;
        Stack& operator=(Stack&& s) { if (&s != this) { clear(); vec_ = std::move(s.vec_); } return *this; }

        iterator begin() noexcept { return vec_.begin(); }
        const_iterator begin() const noexcept { return vec_.begin(); }
        iterator end() noexcept { return vec_.end(); }
        const_iterator end() const noexcept { return vec_.end(); }

        void clear() noexcept { while (! vec_.empty()) vec_.pop_back(); }
        template <typename... Args> void emplace(Args&&... args) { vec_.emplace_back(std::forward<Args>(args)...); }
        bool empty() const noexcept { return vec_.empty(); }
        void pop() noexcept { vec_.pop_back(); }
        void push(const T& t) { vec_.push_back(t); }
        void push(T&& t) { vec_.push_back(std::move(t)); }
        size_t size() const noexcept { return vec_.size(); }
        T& top() noexcept { return vec_.back(); }
        const T& top() const noexcept { return vec_.back(); }

    private:

        std::vector<T> vec_;

    };

}
