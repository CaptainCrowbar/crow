# Thread

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/thread.hpp"
namespace Crow;
```

## Thread class

```c++
class Thread: public std::thread {
    Thread::Thread() noexcept;
    template <typename F, typename... Args> explicit Thread::Thread(F&& f, Args&&... args);
    Thread::Thread(Thread&& t) noexcept;
    Thread::~Thread() noexcept;
    Thread& Thread::operator=(Thread&& t) noexcept;
    void Thread::detach();
    std::thread::id Thread::get_id() const noexcept;
    void Thread::join();
    bool Thread::joinable() const noexcept;
    native_handle_type Thread::native_handle() noexcept;
    void Thread::swap(Thread& t) noexcept;
};
```

A simple wrapper for `std::thread`. This differs from `std::thread` only in
that it will automatically join on destruction, or when used on the LHS of an
assignment.

(This doesn't really belong in a "template library" but it doesn't need a
library of its own and I didn't have anywhere else to put it.)
