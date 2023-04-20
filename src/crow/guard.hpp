#pragma once

#include "crow/types.hpp"
#include <concepts>
#include <exception>
#include <utility>

namespace Crow {

    enum class ScopeState: int {
        exit,    // Invoke callback unconditionally in destructor
        fail,    // Invoke callback when unwinding due to exception, but not on normal exit
        success  // Invoke callback on normal exit, but not when unwinding due to exception
    };

    template <std::invocable F, ScopeState S>
    class BasicScopeGuard {

    public:

        BasicScopeGuard(F&& f)
            try:
                callback_(std::forward<F>(f)),
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
            if constexpr (S == ScopeState::fail)
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

        void release() noexcept {
            inflight_ = -1;
        }

    private:

        F callback_;
        int inflight_ = -1;

    };

    template <std::invocable F> inline auto on_scope_exit(F&& f) { return BasicScopeGuard<F, ScopeState::exit>(std::forward<F>(f)); }
    template <std::invocable F> inline auto on_scope_fail(F&& f) { return BasicScopeGuard<F, ScopeState::fail>(std::forward<F>(f)); }
    template <std::invocable F> inline auto on_scope_success(F&& f) { return BasicScopeGuard<F, ScopeState::success>(std::forward<F>(f)); }

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
            if constexpr (S == ScopeState::fail)
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

        void release() noexcept {
            inflight_ = -1;
        }

    private:

        std::vector<Callback> callbacks_;
        int inflight_ = -1;

    };

    using ScopeExit = BasicScopeGuard<Callback, ScopeState::exit>;
    using ScopeFail = BasicScopeGuard<Callback, ScopeState::fail>;
    using ScopeSuccess = BasicScopeGuard<Callback, ScopeState::success>;

}
