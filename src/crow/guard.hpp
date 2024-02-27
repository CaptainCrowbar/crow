#pragma once

#include "crow/enum.hpp"
#include "crow/types.hpp"
#include <concepts>
#include <exception>

namespace Crow {

    CROW_ENUM_BITMASK(ScopeState, uint8_t,
        success  = 1,  // Invoke callback on normal exit, but not when unwinding due to exception
        failure  = 2,  // Invoke callback when unwinding due to exception, but not on normal exit
        exit     = 3,  // Invoke callback unconditionally in destructor
    )

    template <std::invocable F, ScopeState S>
    class BasicScopeGuard {

    public:

        BasicScopeGuard(F f)
            try:
                callback_(f),
                inflight_(std::uncaught_exceptions()) {}
            catch (...) {
                if constexpr (S != ScopeState::success) {
                    try { f(); }
                    catch (...) {}
                }
                throw;
            }

        ~BasicScopeGuard() noexcept {
            if (inflight_ < 0)
                return;
            if constexpr (S == ScopeState::failure)
                if (std::uncaught_exceptions() <= inflight_)
                    return;
            if constexpr (S == ScopeState::success)
                if (std::uncaught_exceptions() > inflight_)
                    return;
            try { callback_(); }
            catch (...) {}
        }

        BasicScopeGuard() = delete;
        BasicScopeGuard(const BasicScopeGuard&) = delete;
        BasicScopeGuard(BasicScopeGuard&&) = delete;
        BasicScopeGuard& operator=(const BasicScopeGuard&) = delete;
        BasicScopeGuard& operator=(BasicScopeGuard&&) = delete;

        void release() noexcept { inflight_ = -1; }

    private:

        F callback_;
        int inflight_ = -1;

    };

    template <ScopeState S>
    class BasicScopeGuard<Callback, S> {

    public:

        BasicScopeGuard() noexcept:
        inflight_(std::uncaught_exceptions()) {}

        BasicScopeGuard(Callback f):
        inflight_(std::uncaught_exceptions()) {
            *this += f;
        }

        ~BasicScopeGuard() noexcept {
            if (inflight_ < 0)
                return;
            if constexpr (S == ScopeState::failure)
                if (std::uncaught_exceptions() <= inflight_)
                    return;
            if constexpr (S == ScopeState::success)
                if (std::uncaught_exceptions() > inflight_)
                    return;
            for (int i = int(callbacks_.size()) - 1; i >= 0; --i) {
                try { callbacks_[i](); }
                catch (...) {}
            }
        }

        BasicScopeGuard(const BasicScopeGuard&) = delete;
        BasicScopeGuard(BasicScopeGuard&&) = delete;
        BasicScopeGuard& operator=(const BasicScopeGuard&) = delete;
        BasicScopeGuard& operator=(BasicScopeGuard&&) = delete;

        void operator+=(Callback f) {
            if (! f)
                return;
            try {
                callbacks_.push_back(f);
            }
            catch (...) {
                if constexpr (S != ScopeState::success) {
                    try { f(); }
                    catch (...) {}
                }
                throw;
            }
        }

        void release() noexcept { inflight_ = -1; }

    private:

        std::vector<Callback> callbacks_;
        int inflight_ = -1;

    };

    using ScopeSuccess = BasicScopeGuard<Callback, ScopeState::success>;
    using ScopeFailure = BasicScopeGuard<Callback, ScopeState::failure>;
    using ScopeExit = BasicScopeGuard<Callback, ScopeState::exit>;

    template <std::invocable F>
    inline auto on_scope_success(F f) {
        return BasicScopeGuard<F, ScopeState::success>(f);
    }

    template <std::invocable F>
    inline auto on_scope_failure(F f) {
        return BasicScopeGuard<F, ScopeState::failure>(f);
    }

    template <std::invocable F>
    inline auto on_scope_exit(F f) {
        return BasicScopeGuard<F, ScopeState::exit>(f);
    }

    template <typename T>
    requires requires (T& t, size_t n) {
        t.resize(n);
        { t.size() } -> std::convertible_to<size_t>;
    }
    inline auto guard_size(T& t) {
        return on_scope_failure([&t,n=t.size()] { t.resize(n); });
    }

    template <std::copyable T>
    inline auto guard_value(T& t) {
        return on_scope_failure([&t,save=t] { t = save; });
    }

}
