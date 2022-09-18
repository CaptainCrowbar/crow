#pragma once

#include "crow/types.hpp"
#include <string>
#include <utility>

namespace Crow {

    template <typename T, typename Del, T Null = T()>
    class Resource{
    public:
        static constexpr T null = Null;
        constexpr Resource() noexcept: value_(Null) {}
        constexpr explicit Resource(T t) noexcept: value_(t) {}
        ~Resource() noexcept { reset(); }
        Resource(const Resource&) = delete;
        constexpr Resource(Resource&& r) noexcept: value_(std::exchange(r.value_, Null)) {}
        Resource& operator=(const Resource&) = delete;
        Resource& operator=(Resource&& r) noexcept {
            if (&r != this) {
                reset();
                value_ = std::exchange(r.value_, Null);
            }
            return *this;
        }
        constexpr operator T() const noexcept { return value_; }
        constexpr explicit operator bool() const noexcept { return value_ != Null; }
        constexpr auto& operator*() const noexcept { return *value_; }
        constexpr auto* operator->() const noexcept { return &**this; }
        constexpr T get() const noexcept { return value_; }
        constexpr T release() noexcept { return std::exchange(value_, Null); }
        void reset() noexcept {
            if (value_ != Null) {
                Del()(value_);
                value_ = Null;
            }
        }
        friend constexpr void swap(Resource& a, Resource& b) noexcept {
            T t = a.value_;
            a.value_ = b.value_;
            b.value_ = t;
        }
    private:
        T value_;
    };

}
