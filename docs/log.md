# Logging

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/log.hpp"
namespace Crow;
```

## Logging

```c++
class SetLog {
    SetLog() noexcept;
    explicit SetLog(FILE* stream) noexcept;
    ~SetLog() noexcept;
};
inline void set_log(FILE* stream) noexcept;
```

Temporarily or permanently change the global output stream used for logging.
Logging goes to standard error by default. Setting the log stream to a null
pointer will disable all logging.

```c++
template <typename... Args>
    void logx(const Args&... args);
```

Write a log message. This writes all its arguments in a space delimited list,
formatted using the argument type's `str()` function if it has one, otherwise
using its output operator. The output will be prefixed with a UTC timestamp.

If the log output is going to a terminal, the text is printed in a random
colour, based on a hash of the thread ID, so that log lines from different
threads can be easily distinguished. Log lines from different threads are
printed atomically; text from different threads will not be fragmented.

```c++
#define CROW_LOG(...)
```

This does the same thing as the `logx()` function, but the macro version adds
the call site's file name, function name, and line number after the timestamp.
The file and function names are unqualified.
