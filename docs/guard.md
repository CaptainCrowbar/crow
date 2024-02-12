# Scope Guards

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/guard.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class ScopeState: uint8_t {
    success  = 1,
    failure  = 2,
    exit     = success | failure,
};
```

This is used to control the behaviour of a scope guard:

* `success` = Invoke the callback on normal exit, but not when the scope is unwinding due to an exception
* `failure` = Invoke the callback when the scope is unwinding due to an exception, but not on normal exit
* `exit` = Invoke the callback unconditionally on destruction

## Basic scope guard template

```c++
template <std::invocable F, ScopeState S> class BasicScopeGuard {
    explicit BasicScopeGuard(F f);
    ~BasicScopeGuard() noexcept;
    void release() noexcept;
};
```

The basic scope guard object. This is normally created using one of the
functions below rather than being explicitly constructed. The callback type
`F` must be a function object that can be called with no arguments. Behaviour
is undefined if the callback is a null function pointer.

Any exceptions thrown by the callback will be silently ignored. Normally the
callback should be designed to never throw.

If the copy constructor of `F` throws during the scope guard's construction,
the original function object will be called if the guard condition is `exit`
or `failure`, but not if it is `success`.

Calling `release()` cancels all destructor behaviour; the callback will never
be invoked after release.

## Basic scope guard specializations

```c++
template <ScopeState S> class BasicScopeGuard<Callback, S> {
    BasicScopeGuard() noexcept;
    explicit BasicScopeGuard(Callback f);
    ~BasicScopeGuard() noexcept;
    void operator+=(Callback f);
    void release() noexcept;
};
using ScopeSuccess = BasicScopeGuard<Callback, ScopeState::success>;
using ScopeFailure = BasicScopeGuard<Callback, ScopeState::failure>;
using ScopeExit = BasicScopeGuard<Callback, ScopeState::exit>;
```

A partial specialisation of `BasicScopeGuard` for generic function objects.
Because of the function wrapper overhead, this is slightly less efficient
than the original scope guard, but it has the ability to add more actions to
the same scope guard object. The guard's list of actions will be executed in
reverse order when execution is triggered. An empty function object will be
ignored.

## Convenience functions

```c++
template <std::invocable F> [scope guard] on_scope_success(F f);
template <std::invocable F> [scope guard] on_scope_failure(F f);
template <std::invocable F> [scope guard] on_scope_exit(F f);
```

These construct scope guards that will invoke their callbacks under the
appropriate conditions.

```c++
template <[resizeable container] T> [scope guard] guard_size(T& t);
```

Creates a scope guard that will restore the previous size of a container on
failure. This does not attempt to preserve element values, only the number of
elements.

```c++
template <std::copyable T> [scope guard] guard_value(T& t);
```

Creates a scope guard that will restore the previous value of a variable on
failure.
