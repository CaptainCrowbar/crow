# Process Control

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/process.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Shell commands

```c++
std::string shell(const std::string& cmd);
```

Runs a shell command synchronously (via `popen()` or the equivalent),
capturing standard output. No error indication is returned; standard error
still goes to its usual destination, unless explicitly redirected in the
command line. This function may throw `system_error` if anything goes wrong.

## Spawned process channels

These classes call `popen()` to create a child process on construction, with
its standard output connected to the channel. Closing or destroying the
channel calls `pclose()`. They may throw `system_error` if anything goes
wrong.

### Class StreamProcess

```c++
class StreamProcess: public StreamChannel;
    explicit StreamProcess::StreamProcess(const std::string& cmd);
    virtual StreamProcess::~StreamProcess() noexcept;
    int StreamProcess::status() const noexcept;
```

This runs a process whose output is treated as an undifferentiated stream of
bytes. The `status()` function returns the result of `pclose()`, or -1 if the
process has not been closed yet.

### Class TextProcess

```c++
class TextProcess: public MessageChannel<std::string>;
    explicit TextProcess::TextProcess(const std::string& cmd);
    virtual TextProcess::~TextProcess() noexcept;
    std::string TextProcess::read_all();
    int TextProcess::status() const noexcept;
```

This runs a process whose output is treated as a text stream. Each call to
`read()` yields one line of text (if any is available), with the terminating
LF or CRLF stripped.
