#pragma once

#include "crow/types.hpp"
#include <concepts>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename T>
        concept NonVoidPointer = std::is_pointer_v<T>
            && ! std::is_same_v<std::decay_t<std::remove_pointer_t<T>>, void>;

    }

    template <typename T, typename U>
    concept ResourceDeleter = std::is_default_constructible_v<T> && std::invocable<T, U>;

    template <std::regular T, ResourceDeleter<T> Del, T Null = T()>
    class Resource {

    public:

        using value_type = T;
        using deleter_type = Del;

        static constexpr T null = Null;

        constexpr Resource() noexcept: handle_(Null) {}
        constexpr explicit Resource(T t) noexcept: handle_(t) {}
        ~Resource() noexcept { reset(); }
        Resource(const Resource&) = delete;
        constexpr Resource(Resource&& r) noexcept: handle_(std::exchange(r.handle_, Null)) {}
        Resource& operator=(const Resource&) = delete;

        Resource& operator=(Resource&& r) noexcept {
            if (&r != this) {
                reset();
                handle_ = std::exchange(r.handle_, Null);
            }
            return *this;
        }

        Resource& operator=(T t) noexcept {
            reset(t);
            return *this;
        }

        constexpr explicit operator T() const noexcept { return handle_; }
        constexpr explicit operator bool() const noexcept { return handle_ != Null; }

        template <typename U = T>
            constexpr std::enable_if_t<Detail::NonVoidPointer<U>, std::remove_pointer_t<U>&>
            operator*() const noexcept { return *handle_; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::NonVoidPointer<U>, std::remove_pointer_t<U>*>
            operator->() const noexcept { return &**this; }

        constexpr T get() const noexcept { return handle_; }
        constexpr T release() noexcept { return std::exchange(handle_, Null); }

        void reset() noexcept {
            if (handle_ != Null) {
                Del()(handle_);
                handle_ = Null;
            }
        }

        void reset(T t) noexcept {
            reset();
            handle_ = t;
        }

        constexpr void swap(Resource& r) noexcept {
            auto h = handle_;
            handle_ = r.handle_;
            r.handle_ = h;
        }

        friend constexpr void swap(Resource& a, Resource& b) noexcept { a.swap(b); }

    private:

        T handle_;

    };

}
