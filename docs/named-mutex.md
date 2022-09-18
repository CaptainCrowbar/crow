# Named Mutex

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/named-mutex.hpp"
namespace Crow;
```

## Named mutex class

```c++
class NamedMutex: public Waiter;
    explicit NamedMutex::NamedMutex(const std::string& name);
    NamedMutex::~NamedMutex() noexcept;
    std::string NamedMutex::name() const;
    void NamedMutex::lock();
    bool NamedMutex::try_lock();
    template <typename R, typename P>
        bool NamedMutex::try_lock_for(duration<R, P> rel_time);
    bool NamedMutex::try_lock_until(system_clock::time_point abs_time);
    void NamedMutex::unlock() noexcept;
```

A named mutex that can be used for synchronization between processes. The
locking functions have the usual semantics required by the standard mutual
exclusion concept.

Limitations on the name are operating system specific. The name supplied to
the constructor may or may not include the standard prefix (`"/"` on Unix,
`"Local\"` on Windows; these will be automatically converted to suit the host
OS if present). The constructor will throw `std::system_error` if the name is
invalid. For maximum portability:

* Limit the name to a maximum of 30 bytes.
* Ensure the name is valid UTF-8.
* Do not use any ASCII control characters, whitespace characters, or
  punctuation marks other than `"+"`, `"-"`, `"."`, and `"_"`.
* Do not make any assumptions about whether or not mutex names are case
  sensitive.
