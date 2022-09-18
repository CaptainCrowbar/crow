#pragma once

#include "crow/types.hpp"
#include <exception>
#include <functional>
#include <utility>

namespace Crow {

    enum class ScopeState: int {
        exit,    // Invoke callback unconditionally in destructor
        fail,    // Invoke callback when unwinding due to exception, but not on normal exit
        success  // Invoke callback on normal exit, but not when unwinding due to exception
    };

    template <typename F, ScopeState S>
    class BasicScopeGuard {
    public:
        BasicScopeGuard() = default;
        BasicScopeGuard(F&& f) try:
            callback_(std::forward<F>(f)),
            inflight_(std::uncaught_exceptions()) {}
            catch (...) {
                if constexpr (S != ScopeState::success) {
                    try { f(); }
                    catch (...) {}
                }
                throw;
            }
        BasicScopeGuard(const BasicScopeGuard&) = delete;
        BasicScopeGuard(BasicScopeGuard&& g) noexcept:
            callback_(std::move(g.callback_)),
            inflight_(std::exchange(g.inflight_, -1)) {}
        ~BasicScopeGuard() noexcept { close(); }
        BasicScopeGuard& operator=(const BasicScopeGuard&) = delete;
        BasicScopeGuard& operator=(F&& f) {
            close();
            callback_ = std::forward<F>(f);
            inflight_ = std::uncaught_exceptions();
            return *this;
        }
        BasicScopeGuard& operator=(BasicScopeGuard&& g) noexcept {
            if (&g != this) {
                close();
                callback_ = std::move(g.callback_);
                inflight_ = std::exchange(g.inflight_, -1);
            }
            return *this;
        }
        void release() noexcept { inflight_ = -1; }
    private:
        F callback_;
        int inflight_ = -1;
        void close() noexcept {
            if (inflight_ >= 0) {
                bool call = true;
                if constexpr (S == ScopeState::fail)
                    call = std::uncaught_exceptions() > inflight_;
                else if constexpr (S == ScopeState::success)
                    call = std::uncaught_exceptions() <= inflight_;
                if (call)
                    try { callback_(); } catch (...) {}
                inflight_ = -1;
            }
        }
    };

    template <typename F> inline auto on_scope_exit(F&& f) { return BasicScopeGuard<F, ScopeState::exit>(std::forward<F>(f)); }
    template <typename F> inline auto on_scope_fail(F&& f) { return BasicScopeGuard<F, ScopeState::fail>(std::forward<F>(f)); }
    template <typename F> inline auto on_scope_success(F&& f) { return BasicScopeGuard<F, ScopeState::success>(std::forward<F>(f)); }

}
