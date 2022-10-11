# Standard I/O

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/stdio.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class IoMode {
    read,
    write,
    append,
    create_always,
    open_always,
    open_existing
};
```

| Mode             | Description                                                                              |
| ----             | -----------                                                                              |
| `read`           | Open for read only; fail if the file does not exist                                      |
| `write`          | Open for write only; always create a new file, replacing it if it already exists         |
| `append`         | Open for append; open the file if it already exists, otherwise create a new one          |
| `create_always`  | Open for read and write; always create a new file, replacing it if it already exists     |
| `open_always`    | Open for read and write; open the file if it already exists, otherwise create a new one  |
| `open_existing`  | Open for read and write; open the file if it already exists, otherwise fail              |

Flags for some of the commonly used file opening modes.

```c++
class IoError: public std::system_error;
```

This is thrown to report I/O errors.

```c++
class LineIterator;
```

An iterator over the lines in a text file (see `IoBase::read_line()`).

## I/O abstract base class

```c++
class IoBase;
```

This is an abstract base class for classes that implement file I/O. Unless
documented otherwise, any of the member functions of `IoBase` or any derived
class can throw `IoError` if an I/O error is encountered.

In the documentation for derived classes, functions inherited or overridden
from the base class are not listed if their behaviour has not significantly
changed from that documented here.

```c++
static constexpr size_t IoBase::default_length = 65'536;
```

Default maximum length for reads.

```c++
virtual IoBase::~IoBase() noexcept;
```

Virtual destructor.

```c++
virtual void IoBase::close() = 0;
```

Close the file. This is only needed if you want to check for an error
afterwards; any open file is closed by the destructor.

```c++
virtual void IoBase::flush() = 0;
```

Flush the stream. The exact behaviour depends on the derived class. Flushing
an input stream usually does nothing.

```c++
virtual int IoBase::getc();
virtual size_t IoBase::read(void* ptr, size_t maxlen) = 0;
virtual std::string IoBase::read_line();
std::string IoBase::read_all();
size_t IoBase::read_some(std::string& buf, size_t maxlen = default_length);
std::string IoBase::reads(size_t maxlen = default_length);
```

Input functions. The `getc()` function reads one byte, returning the byte
value or `EOF`.

The `read()` function reads up to `maxlen` bytes into the destination buffer,
returning the number of bytes actually read. The `read_line()` function reads
up to the next line feed (or EOF), returning the line as a string
(including the terminating line feed, if present).

The `read_all()` function reads all available data until it reaches EOF or an
error occurs. The `read_some()` and `reads()` functions call `read()`, writing
the incoming data into a string instead of a user supplied buffer.

All of the `read*()` functions indicate end of file by returning an empty
string or a zero byte count.

```c++
virtual bool IoBase::is_open() const = 0;
```

True if the file is open.

```c++
virtual void IoBase::putc(char c);
virtual size_t IoBase::write(const void* ptr, size_t len) = 0;
void IoBase::write_chars(size_t n, char c);
void IoBase::write_line(const std::string& str = {});
size_t IoBase::writes(const std::string& str);
```

Output functions. The `putc()` function writes one byte.

The `write()` function attempts to write the given block of bytes, returning
the number successfully written (on most I/O streams this will be less than
`len` only if something goes wrong, but this is not guaranteed).

The `write_chars()` function writes `n` identical bytes equal to `c`.

The `writes()` function calls `write()` with the string contents, repeating
the call if an incomplete write occurs but is not reported as an error;
`write_line()` does the same thing, but will add a line feed if the string did
not already end with one.

```c++
virtual void IoBase::seek(ptrdiff_t offset, int which = SEEK_CUR) = 0;
virtual ptrdiff_t IoBase::tell() = 0;
```

Set or query the current position in the file. The `which` argument takes the
standard values `SEEK_CUR`, `SEEK_SET`, or `SEEK_END`. These may not work on
all streams.

```c++
template <typename... Args>
    void IoBase::format(const std::string& pattern, Args&&... args);
template <typename... Args> void IoBase::print(Args&&... args);
```

Formatted output functions. These call `format()` or `prints()` and write the
resulting string to the output stream.

```c++
Irange<LineIterator> IoBase::lines();
```

Return iterators over the lines in a file. These are obtained by calling
`read_line()`.

## C standard I/O

```c++
class Cstdio: public IoBase;
```

This class implements standard buffered I/O, using the `FILE*` based system
API in `<cstdio>` or `<stdio.h>`

```c++
using Cstdio::handle_type = FILE*;
```

The underlying native file handle type.

```c++
Cstdio::Cstdio() noexcept;
```

Sets the file handle to null.

```c++
explicit Cstdio::Cstdio(FILE* f) noexcept;
```

Wraps a `Cstdio` object around an existing file pointer. This may be one of
the standard I/O streams.

```c++
explicit Cstdio::Cstdio(const Path& f, IoMode m = IoMode::read);
```

Opens the file using one of the standard `IoMode` values, defaulting to read
only.

```c++
Cstdio::Cstdio(const Path& f, const std::string& iomode);
```

Opens the file using the same kind of mode string as `fopen()`.

```c++
Cstdio::~Cstdio() noexcept;
```

The destructor closes the file, if it was still open and was not one of the
standard IO streams.

```c++
Cstdio::Cstdio(Cstdio&& obj);
Cstdio& Cstdio::operator=(Cstdio&& obj);
```

Other life cycle functions. This class is movable but not copyable.

```c++
int Cstdio::fd() const;
```

Returns the underlying file descriptor.

```c++
FILE* Cstdio::get() const noexcept;
FILE* Cstdio::release() noexcept;
```

Return the native file handle. The `release()` function sets the internal
handle to null and abandons ownership of the stream.

```c++
void Cstdio::ungetc(char c);
```

Calls the native `ungetc()` function to push one byte back into the stream
buffer.

```c++
static Cstdio Cstdio::null();
static Cstdio Cstdio::std_input();
static Cstdio Cstdio::std_output();
static Cstdio Cstdio::std_error();
```

Standard streams.

## Unix file descriptor I/O

```c++
class Fdio: public IoBase;
```

This class implements standard Posix unbuffered I/O, using the file descriptor
based system API.

```c++
using Fdio::handle_type = int;
```

The underlying native file handle type.

```c++
Fdio::Fdio() noexcept;
```

Sets the file handle to -1.

```c++
explicit Fdio::Fdio(int f) noexcept;
```

Wraps an `Fdio` object around an existing file descriptor. This may be one of
the standard I/O streams.

```c++
explicit Fdio::Fdio(const Path& f, IoMode m = IoMode::read);
```

Opens the file using one of the standard `IoMode` values, defaulting to read
only.

```c++
Fdio::Fdio(const Path& f, int iomode, int perm = 0666);
```

Opens the file using the same flags used in the Posix `open()` function.

| Flag                | Description                                    | Posix    | Linux    | Mac      | Windows  |
| ----                | -----------                                    | -----    | -----    | ---      | -------  |
| Access mode flags   |                                                |          |          |          |          |
| `O_EXEC`            | Open for execute only                          | Yes      | &mdash;  | &mdash;  | &mdash;  |
| `O_RDONLY`          | Open for reading only                          | Yes      | Yes      | Yes      | Yes      |
| `O_RDWR`            | Open for reading and writing                   | Yes      | Yes      | Yes      | Yes      |
| `O_SEARCH`          | Open directory for search only                 | Yes      | &mdash;  | &mdash;  | &mdash;  |
| `O_WRONLY`          | Open for writing only                          | Yes      | Yes      | Yes      | Yes      |
| Other Posix flags   |                                                |          |          |          |          |
| `O_APPEND`          | Set append mode                                | Yes      | Yes      | Yes      | Yes      |
| `O_CLOEXEC`         | Implicitly set `FD_CLOEXEC`                    | Yes      | Yes      | Yes      | &mdash;  |
| `O_CREAT`           | Create if file does not exist                  | Yes      | Yes      | Yes      | Yes      |
| `O_DIRECTORY`       | Must be a directory                            | Yes      | Yes      | Yes      | &mdash;  |
| `O_DSYNC`           | Synchronize data                               | Yes      | Yes      | &mdash;  | &mdash;  |
| `O_EXCL`            | Error if file already exists                   | Yes      | Yes      | Yes      | Yes      |
| `O_NOCTTY`          | Don't assign controlling terminal              | Yes      | Yes      | Yes      | &mdash;  |
| `O_NOFOLLOW`        | Don't follow symlinks                          | Yes      | Yes      | Yes      | &mdash;  |
| `O_NONBLOCK`        | No delay                                       | Yes      | Yes      | Yes      | &mdash;  |
| `O_RSYNC`           | Synchronize read operations                    | Yes      | Yes      | &mdash;  | &mdash;  |
| `O_SYNC`            | Synchronize write operations                   | Yes      | Yes      | &mdash;  | &mdash;  |
| `O_TRUNC`           | Truncate to zero length                        | Yes      | Yes      | Yes      | Yes      |
| `O_TTY_INIT`        | Set `termios` structure if this is a terminal  | Yes      | &mdash;  | &mdash;  | &mdash;  |
| Non-standard flags  |                                                |          |          |          |          |
| `O_ASYNC`           | Signal process group when data ready           | &mdash;  | Yes      | Yes      | &mdash;  |
| `O_BINARY`          | Open in binary mode                            | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_DIRECT`          | Minimize caching                               | &mdash;  | Yes      | &mdash;  | &mdash;  |
| `O_EVTONLY`         | Descriptor for event notification only         | &mdash;  | &mdash;  | Yes      | &mdash;  |
| `O_EXLOCK`          | Open with exclusive file lock                  | &mdash;  | Yes      | Yes      | &mdash;  |
| `O_FSYNC`           | Synchronous writes                             | &mdash;  | Yes      | Yes      | &mdash;  |
| `O_LARGEFILE`       | Allow files too large for a 32-bit offset      | &mdash;  | Yes      | &mdash;  | &mdash;  |
| `O_NDELAY`          | Synonym for `O_NONBLOCK`                       | &mdash;  | Yes      | Yes      | &mdash;  |
| `O_NOATIME`         | Don't update last access time                  | &mdash;  | Yes      | &mdash;  | &mdash;  |
| `O_NOINHERIT`       | Prevent creation of a shared descriptor        | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_PATH`            | Path-only handle, do not open file             | &mdash;  | Yes      | &mdash;  | &mdash;  |
| `O_RANDOM`          | Optimize for random access                     | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_SEQUENTIAL`      | Optimize for sequential access                 | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_SHLOCK`          | Open with shared file lock                     | &mdash;  | Yes      | Yes      | &mdash;  |
| `O_SHORT_LIVED`     | If possible do not flush to disk               | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_SYMLINK`         | Allow opening a symlink                        | &mdash;  | &mdash;  | Yes      | &mdash;  |
| `O_TEMPORARY`       | Delete file when last descriptor is closed     | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_TMPFILE`         | Delete file when last descriptor is closed     | &mdash;  | Yes      | &mdash;  | &mdash;  |
| `O_TEXT`            | Open in text mode                              | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_U8TEXT`          | Open in UTF-8 text mode                        | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_U16TEXT`         | Open in UTF-16 text mode                       | &mdash;  | &mdash;  | &mdash;  | Yes      |
| `O_WTEXT`           | Open in UTF-16 text mode, check for BOM        | &mdash;  | &mdash;  | &mdash;  | Yes      |

```c++
Fdio::~Fdio() noexcept;
```

The destructor closes the file, if it was still open and was not one of the
standard IO streams.

```c++
Fdio::Fdio(Fdio&& obj);
Fdio& Fdio::operator=(Fdio&& obj);
```

Other life cycle functions. This class is movable but not copyable.

```c++
Fdio Fdio::dup();
Fdio Fdio::dup(int f);
```

These call the native `dup()` or `dup2()` functions to duplicate a file
descriptor.

```c++
int Fdio::get() const noexcept;
int Fdio::release() noexcept;
```

Return the native file handle. The `release()` function sets the internal
handle to -1 and abandons ownership of the stream.

```c++
static Fdio Fdio::null();
static Fdio Fdio::std_input();
static Fdio Fdio::std_output();
static Fdio Fdio::std_error();
```

Standard streams.

```c++
static std::pair<Fdio, Fdio> Fdio::pipe(size_t winmem = default_length);
```

Opens a pipe (by calling the native `pipe()` function or the equivalent),
returning the read and write ends of the pipe. On Windows a size limit is
required when creating a pipe; this argument is ignored on Unix.

## Windows file handle I/O

```c++
class Winio: public IoBase;
```

This class implements I/O using the Win32 system API.

```c++
using Winio::handle_type = HANDLE;
```

The underlying native file handle type.

```c++
Winio::Winio() noexcept;
```

Sets the file handle to null.

```c++
explicit Winio::Winio(HANDLE f) noexcept;
```

Wraps a `Winio` object around an existing file handle. This may be one of the
standard I/O streams.

```c++
explicit Winio::Winio(const Path& f, IoMode m = IoMode::read);
```

Opens the file using one of the standard `IoMode` values, defaulting to read
only.

```c++
Winio::Winio(const Path& f, DWORD desired_access, DWORD share_mode,
    LPSECURITY_ATTRIBUTES security_attributes, DWORD creation_disposition,
    DWORD flags_and_attributes = 0, HANDLE template_file = nullptr);
```

Opens the file using the same arguments used in `CreateFile()`.

```c++
Winio::~Winio() noexcept;
```

The destructor closes the file, if it was still open and was not one of the
standard IO streams.

```c++
Winio::Winio(Winio&& obj);
Winio& Winio::operator=(Winio&& obj);
```

Other life cycle functions. This class is movable but not copyable.

```c++
HANDLE Winio::get() const noexcept;
HANDLE Winio::release() noexcept;
```

Return the native file handle. The `release()` function sets the internal
handle to null and abandons ownership of the stream.

```c++
static Winio Winio::null();
static Winio Winio::std_input();
static Winio Winio::std_output();
static Winio Winio::std_error();
```

Standard streams.

## Temporary file

```c++
class TempFile: public Cstdio;
```

A `TempFile` object holds a handle to a file opened for read and write. The
file is created when the `TempFile` is constructed, and closed when it is
destroyed (or moved into). It is implementation defined whether or not an
actual file exists in the file system corresponding to the `TempFile` object.
If such a file exists, it may be left behind if the process terminates
without properly destroying the `TempFile` object.

```c++
TempFile::TempFile();
```

The default constructor creates a file in the system's standard location by
calling `std::tmpfile()`.

```c++
TempFile::TempFile(const Path& dir, const std::string& prefix);
```

Creates a file in the specified directory, with a leaf name starting with the
specified prefix (which may be empty).

```c++
TempFile::~TempFile() noexcept;
TempFile::TempFile(TempFile&& obj);
TempFile& TempFile::operator=(TempFile&& obj);
```

Other life cycle functions. This class is movable but not copyable.

```c++
Path TempFile::where() const;
```

Returns the actual location of the file. This may be empty if the object was
default constructed.
