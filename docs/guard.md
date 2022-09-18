# Scope Guards

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/guard.hpp"
namespace Crow;
```

## Scope guards

```c++
enum class ScopeState: int {
    exit,
    fail,
    success
};
```

This is used to control the behaviour of a scope guard:

* `exit` = Invoke the callback unconditionally on destruction
* `fail` = Invoke the callback when the scope is unwinding due to an exception, but not on normal exit
* `success` = Invoke the callback on normal exit, but not when the scope is unwinding due to an exception

```c++
template <typename F, ScopeState S> class BasicScopeGuard {
    BasicScopeGuard() noexcept;
    BasicScopeGuard(F&& f);
    BasicScopeGuard(BasicScopeGuard&& g) noexcept;
    ~BasicScopeGuard() noexcept;
    BasicScopeGuard& operator=(F&& f);
    BasicScopeGuard& operator=(BasicScopeGuard&& g) noexcept;
    void release() noexcept;
};
```

The scope guard object. This is normally created using one of the functions
below rather than being explicitly constructed. The callback type `F` must be
a function object that can be called with no arguments. Behaviour is
undefined if the callback is a null function object.

If the move constructor of `F` throws during the scope guard's construction,
the original function object will be called if the guard condition is `exit`
or `fail`, but not if it was `success`. Behaviour is undefined if moving `F`
throws when the scope guard is moved.

Calling `release()` cancels all destructor behaviour; the callback will never
be invoked after release.

```c++
template <typename F>
    BasicScopeGuard<F, ScopeState::exit> on_scope_exit(F&& f);
template <typename F>
    BasicScopeGuard<F, ScopeState::fail> on_scope_fail(F&& f);
template <typename F>
    BasicScopeGuard<F, ScopeState::success> on_scope_success(F&& f);
```

These construct scope guards that will invoke their callbacks under the
appropriate conditions.
