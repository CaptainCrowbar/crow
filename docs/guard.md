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
    explicit BasicScopeGuard(F&& f);
    ~BasicScopeGuard() noexcept;
    void release() noexcept;
};
```

The basic scope guard object. This is normally created using one of the
functions below rather than being explicitly constructed. The callback type
`F` must be a function object that can be called with no arguments. Behaviour
is undefined if the callback is a null function object.

If the move constructor of `F` throws during the scope guard's construction,
the original function object will be called if the guard condition is `exit`
or `fail`, but not if it was `success`.

Calling `release()` cancels all destructor behaviour; the callback will never
be invoked after release.

```c++
template <ScopeState S> class BasicScopeGuard<Callback, S> {
    BasicScopeGuard() noexcept;
    explicit BasicScopeGuard(Callback f);
    ~BasicScopeGuard() noexcept;
    void operator+=(Callback f);
    void release() noexcept;
};
using ScopeExit = BasicScopeGuard<Callback, ScopeState::exit>;
using ScopeFail = BasicScopeGuard<Callback, ScopeState::fail>;
using ScopeSuccess = BasicScopeGuard<Callback, ScopeState::success>;
```

A partial specialisation of `BasicScopeGuard` for generic function objects.
Because of the function wrapper overhead, this is slightly less efficient
than the original scope guard, but it has the ability to add more actions to
the same scope guard object. The guard's list of actions will be executed in
reverse order when execution is triggered. An empty function object will be
ignored.

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
