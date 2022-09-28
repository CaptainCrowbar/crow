# Sqlite Wrapper

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/sqlite.hpp"
namespace Crow::Sqlite;
```

## Contents

* TOC
{:toc}

## Version information

```c++
std::array<int, 3> compile_version() noexcept;
std::array<int, 3> runtime_version() noexcept;
```

Return the version numbers of the Sqlite library.

## Exceptions

```c++
class Exception: public std::runtime_error;
class InvalidArgument: public Exception;
class InvalidOperation: public Exception;
class SqliteError: public Exception {
    int error() const noexcept;
};
```

Exceptions thrown by functions in this module:

* `InvalidArgument` is thrown to report that a function was called with invalid arguments.
* `InvalidOperation` is thrown to report that a SQL operation was invoked when
  it should not be possible (e.g. double commit or rollback).
* `SqliteError` is thrown to propagate an error reported by a Sqlite library call.

## Database connection class

```c++
class Connect;
```

Represents an open Sqlite database.

| Flag                   | Meaning                                                |
| ----                   | -------                                                |
|                        | **Open mode flags**                                    |
| `Connect::read`        | Open in read only mode; fail if it does not exist      |
| `Connect::write`       | Open in read/write mode; fail if it does not exist     |
| `Connect::create`      | Open in read/write mode; create the file if necessary  |
| `Connect::memory`      | Create anonymous temporary database in memory          |
| `Connect::tempfile`    | Create anonymous temporary database on disk            |
|                        | **Other connection flags**                             |
| `Connect::nofollow`    | File may not be a symlink                              |
| `Connect::nomutex`     | No mutex on connection; UB if shared between threads   |
| `Connect::uri`         | File name may be a URI                                 |
|                        | **Query usage hints**                                  |
| `Connect::persistent`  | Query is likely to be frequently used                  |

Bitmask flags used when a connection or query is constructed. At most one of
the open mode flags may be passed to the `Connection` constructor; `read` is
the default. The other connection flags may be added to these; the `nofollow`
and `uri` flags may not be combined with `memory` or `tempfile`.

The `persistent` flag may be used when preparing a query using
`Connect::query()`. It does not change the semantics of the query but acts as
an optimization hint to the Sqlite engine, indicating that the query is likely
to be frequently used.

```c++
Connect::Connect();
```

A default constructed object does not open a database; any attempt to run
queries on it will fail.

```c++
explicit Connect::Connect(int flags);
explicit Connect::Connect(const std::string& file, int flags = read);
```

The constructors take an optional file name and flags. These will throw
`InvalidArgument` if an invalid combination of flags is used, if a non-empty
file name is supplied with the `memory` or `tempfile` flags, or if an empty
file name is supplied with any other mode. They will throw `SqliteError` if
Sqlite reports an error.

```c++
Connect::Connect(Connect&& c) noexcept;
Connect::~Connect() noexcept;
Connect& Connect::operator=(Connect&& c) noexcept;
```

Other life cycle operations. `Connect` is movable but not not copyable.

```c++
Query Connect::query(const std::string& sql, int flags = 0);
```

Prepare a query from a SQL string. Optionally the `persistent` flag can be
supplied as a hint that the query will be frequently used.

This will throw `InvalidArgument` if any flag other than `persistent` is used,
or `SqliteError` if Sqlite reports an error.

```c++
Result Connect::run(const std::string& sql);
Result Connect::operator()(const std::string& sql);
```

Run a parameterless query and return the result. This may throw `SqliteError`.

```c++
void Connect::set_pragma(const std::string& name, const std::string& value);
```

Sets a Sqlite pragma. This may throw `SqliteError`.

```c++
template <typename R, typename P>
    void Connect::set_timeout(std::chrono::duration<R, P> t);
```

Sets the timeout for database operations. This may throw `SqliteError`.

```c++ sqlite3* explicit Connect::native_handle() const noexcept; The
conversion operator returns the first column of the first row, and is
intended for simple scalar-valued queries.

```

Returns the native Sqlite connection handle.

## Database query class

```c++
class Query;
```

Represents a prepared query.

```c++
Query::Query();
Query::Query(Query&& q) noexcept;
Query::~Query() noexcept;
Query& Query::operator=(Query&& q) noexcept;
```

Life cycle operations. `Query` is movable but not copyable. Normally the user
will not explicitly construct a `Query` object, obtaining one by calling
`Connect::query()` instead.

```c++
template <typename... Args> void Query::bind(const Args&... args);
```

Bind values to all of the queries parameters. The arguments may be `nullptr`,
`bool`, `char`, `std::string`, any arithmetic type, or any type implicitly or
explicitly convertible to `std::string`. This will throw `InvalidArgument` if
the wrong number of arguments are supplied, and may also throw `SqliteError`.

```c++
template <typename T> void Query::set(int index, const T& value);
template <typename T>
    void Query::set(const std::string& name, const T& value);
```

Bind a value to a specific parameter, identified by number or name. Note that
Sqlite uses 1-based indexing for query parameters. These will throw
`InvalidArgument` if the index is out of bounds or the named parameter does
not exist, and may also throw `SqliteError`.

```c++
void Query::clear();
```

Unbind all parameters. This may throw `SqliteError`.

```c++
Result Query::run();
Result Query::operator()();
```

Run the query, returning a `Result`. These will throw `InvalidArgument` if not
all parameters have been bound, and may also throw `SqliteError`.

```c++
template <typename... Args>
    Result Query::run(const Args&... args);
template <typename... Args>
    Result Query::operator()(const Args&... args);
```

Bind all parameters and run the query. These will throw `InvalidArgument` if
the wrong number of arguments are supplied, and may also throw `SqliteError`.

```c++
int get_index(const std::string& name) const noexcept;
std::string Query::get_name(int index) const;
```

Retrieve the index of a named parameter, or the name of a parameter identified
by index. These will return zero or an empty string if the requested parameter
does not exist.

```c++
int Query::parameters() const noexcept;
```

Returns the number of parameters.

```c++
sqlite3_stmt* Query::native_handle() const noexcept;
```

Returns the native Sqlite handle for the prepared query.

## Query result class

```c++
class Result;
```

Represents the result of running a query. This can be treated as a single pass
container of `Row` objects; a scalar result can also be queried directly.

```c++
class Result::iterator;
```

An input iterator over the sequence of `Row` objects representing the query
result. The increment operator may throw `SqliteError`.

```c++
Result::Result() noexcept;
Result::Result(Result&& res) noexcept;
Result::~Result() noexcept;
Result& Result::operator=(Result&& res) noexcept;
```

Life cycle operations. `Result` is movable but not copyable. Normally the user
will not explicitly construct a `Result` object, obtaining one from a `Query`
instead.

```c++
Result::iterator Result::begin();
Result::iterator Result::end();
```

Iterators over the row sequence. The `begin()` function may throw
`SqliteError`.

```c++
int Result::columns() const noexcept;
```

Returns the number of columns in the result.

```c++
bool Result::empty() const noexcept;
```

True if the result is empty.

```c++
template <typename T> T Result::get(int col = 0) const;
template <typename T> void Result::get(int col, T& t) const;
template <typename T> explicit Result::operator T() const;
```

These can be used to retrieve columns from a single-row result. The column
type `T` may be `bool`, `std::string`, any arithmetic type, or any type
implicitly or explicitly constructible from `std::string`. The conversion
operator returns the first column of the first row, and is intended for
simple scalar-valued queries. These may throw `SqliteError`.

```c++
template <typename... Args> void Result::read(Args&... args) const;
```

Reads the columns of a single-row result. This will throw `InvalidArgument` if
the number of arguments does not match the number of columns in the result,
and may also throw `SqliteError`.

## Query result row class

```c++
class Row;
```

Represents one row of a query result. This class is not directly accessible to
the user; a reference to it can only be obtained by dereferencing a
`Result::iterator`.

```c++
int Row::columns() const noexcept;
```

The number of columns in the row. This is the same as `Result::columns()`.

```c++
int64_t Row::index() const noexcept;
```

Returns the (0-based) index of the row within the query result.

```c++
template <typename T> T Row::get(int col) const;
template <typename T> void Row::get(int col, T& t) const;
```

Retrieve a specific column from the row. The column type `T` may be `bool`,
`std::string`, any arithmetic type, or any type implicitly or explicitly
constructible from `std::string`. These may throw `SqliteError`.

```c++
template <typename... Args> void Row::read(Args&... args) const;
```

Retrieve all columns from the row. This may throw `SqliteError`.

## Database transaction class

```c++
class Transaction;
```

Represents a database transaction. Note that Sqlite does not support nested
transactions.

```c++
explicit Transaction::Transaction(Connect& con);
```

Creates a transaction. This may throw `SqliteError`.

```c++
Transaction::~Transaction() noexcept;
```

The destructor will attempt to roll back the transaction if it has not yet
been committed or rolled back, but any Sqlite errors will be silently
ignored.

```c++
void Transaction::commit();
void Transaction::rollback();
```

Commit or roll back the transaction. These may throw `SqliteError`.
