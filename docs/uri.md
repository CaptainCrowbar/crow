# URI

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/uri.hpp"
namespace Crow;
```

## URI class

```c++
class Uri;
```

This class holds a URI, assumed to follow the format:

    scheme: [//] [ [user [:password] @] host [:port] ] [/path] [?query] [#fragment]

```c++
static constexpr int Uri::lone_keys = 1;
```

Flag used in the `make_query()` function.

```c++
Uri::Uri() noexcept;
```

The default constructor produces an empty URI.

```c++
Uri::explicit Uri(const std::string& s);
```

This constructor parses a URI supplied as a string. This will throw
`std::invalid_argument` if the URI is invalid.

```c++
Uri::Uri(const std::string& scheme, const std::string& user, const std::string& password, const std::string& host, uint16_t port = 0,
    const std::string& path = {}, const std::string& query = {}, const std::string& fragment = {});
```

This constructor assembles a URI from its component parts (following the same
rules as the individual `set_*()` functions described below). This will throw
`std::invalid_argument` if the arguments do not form a valid URI.

```c++
Uri::Uri(const Uri& u);
Uri::Uri(Uri&& u) noexcept;
Uri::~Uri() noexcept;
Uri& Uri::operator=(const Uri& u);
Uri& Uri::operator=(Uri&& u) noexcept;
```

Other life cycle functions.

```c++
bool Uri::has_slashes() const noexcept;
bool Uri::has_user() const noexcept;
bool Uri::has_password() const noexcept;
bool Uri::has_host() const noexcept;
bool Uri::has_port() const noexcept;
bool Uri::has_path() const noexcept;
bool Uri::has_query() const noexcept;
bool Uri::has_fragment() const noexcept;
```

Query whether a given URI element is present. There is no `has_scheme()`
because the scheme is always present if the URI is not empty.

```c++
std::string Uri::scheme() const;
std::string Uri::user() const;
std::string Uri::password() const;
std::string Uri::host() const;
uint16_t Uri::port() const noexcept;
std::string Uri::path() const;
std::string Uri::query() const;
std::string Uri::fragment() const;
```

Query the value of a given URI element. The returned strings do not include
the associated punctuation (e.g. `scheme()` will not have a trailing colon or
slashes). In some cases this means that the corresponding `has_*()` function
must also be checked in order to distinguish between an element that is
absent and one that is present but with an empty value (e.g. a URI ending in
a question mark is considered to include an empty query string, which is not
the same thing as one with no query part).

```c++
void Uri::set_scheme(const std::string& new_scheme, bool smart = true);
void Uri::set_user(const std::string& new_user);
void Uri::set_password(const std::string& new_password);
void Uri::set_host(const std::string& new_host);
void Uri::set_port(uint16_t new_port);
void Uri::set_path(const std::string& new_path);
void Uri::set_query(const std::string& new_query);
void Uri::set_fragment(const std::string& new_fragment);
```

Change the value of a given URI element. The string supplied will be escaped
when necessary. Passing an empty string is equivalent to clearing the element
only for elements that do not distinguish between an empty value and a missing
element; this is not true of the query and fragment elements (e.g.
`set_user("")` is equivalent to `clear_user()`, but `set_query("")` is not
equivalent to `clear_query()`).

The scheme will always be converted to lower case. If the `smart` argument to
`set_scheme()` is true (this is the default), and the scheme supplied does
not end in `"://"`, the slashes will be added unless the scheme is
`"mailto"`.

All of these except `set_scheme()` will throw `std::invalid_argument` if the
existing URI is empty, or under the following circumstances:

* `set_scheme()` if the scheme is invalid (it must match `/[a-z][a-z0-9.+-]*(:(//)?)?/`).
* `set_user()` if the host is empty and the new user is not empty.
* `set_password()` if the user or host is empty and the new password is not empty.
* `set_path()` if the path contains `"//"`.

```c++
void Uri::clear_user() noexcept;
void Uri::clear_password() noexcept;
void Uri::clear_host() noexcept;
void Uri::clear_port() noexcept;
void Uri::clear_path() noexcept;
void Uri::clear_query() noexcept;
void Uri::clear_fragment() noexcept;
```

Remove a given URI element. There is no function to clear the scheme because
the scheme is always required.

```c++
void Uri::append_path(const std::string& new_path);
Uri& Uri::operator/=(const std::string& s);
Uri operator/(const Uri& u, const std::string& s);
```

Append one or more file path elements to the URI's path. Appending an absolute
path (one that starts with a slash) will discard the original path and behave
like `set_path()`. These will throw `std::invalid_argument` if the path is
invalid (contains two consecutive slashes).

```c++
void Uri::clear() noexcept;
```

Resets the URI to an empty string.

```c++
Uri Uri::doc() const;
Uri Uri::base() const;
Uri Uri::parent() const;
Uri Uri::root() const;
```

These return related URLs:

* `doc()` function discards the fragment part (as for `clear_fragment()`).
* `base()` discards the query and fragment parts.
* `parent()` discards the last element of the path, if any.
* `root()` discards the entire path and returns the root of the host directory tree (as for `set_path("/")`).

```c++
bool Uri::empty() const noexcept;
```

True if the URI is empty.

```c++
size_t Uri::hash() const noexcept;
struct std::hash<Uri>;
```

Hash functions.

```c++
bool Uri::is_root() const noexcept;
```

True if the URI refers to the root of the host directory tree (the path part
is empty or `"/"`).

```c++
std::string Uri::path_dir() const;
std::string Uri::path_leaf() const;
```

Split the path element into a directory path and a leaf name. If `path_dir()`
is not empty, it will always start and end with a slash (it may just be a lone
slash). If the whole path ends with a slash, it is assumed to be a directory
name, and `path_leaf()` will be empty.

```c++
std::string Uri::str() const;
std::ostream& operator<<(std::ostream& out, const Uri& u);
```

Return the URI as a string.

```c++
bool Uri::try_parse(const std::string& s);
```

Attempts to parse the given string as a URI. On success, this changes the
current object to hold the new URI and returns true; on failure, it returns
false and leaves the object unchanged.

```c++
static std::string Uri::encode(const std::string& s, const std::string& exempt = {});
static std::string Uri::decode(const std::string& s);
```

These apply percent encoding to a string. Safe bytes, left unencoded, are the
ASCII alphanumerics plus `[-._~]`; all other bytes are encoded unless they
appear in the exempt string.

```c++
template <typename R> static std::string Uri::make_query(const R& range, char delimiter = '&', int flags = 0);
static std::vector<std::pair<std::string, std::string>> Uri::parse_query(const std::string& query, char delimiter = 0);
```

Construct or deconstruct a query string. The range argument to `make_query()`
is expected to contain pairs whose first and second fields are string views or
convertible to them; the delimiter character is inserted between key/value
pairs; an equals sign is inserted between keys and values, except that it will
be left out if the value is empty and the `lone_keys` flag is set.

The `parse_query()` function breaks down a query string into a vector of
key/value pairs; if no delimiter is explicitly supplied, whichever of `'&'`
or `';'` appears first in the string will be used. Key and value strings are
percent encoded by `make_query()` and decoded by `parse_query()`.

```c++
std::strong_ordering operator<=>(const Uri& u, const Uri& v) noexcept;
bool operator==(const Uri& u, const Uri& v) noexcept;
bool operator!=(const Uri& u, const Uri& v) noexcept;
bool operator<(const Uri& u, const Uri& v) noexcept;
bool operator>(const Uri& u, const Uri& v) noexcept;
bool operator<=(const Uri& u, const Uri& v) noexcept;
bool operator>=(const Uri& u, const Uri& v) noexcept;
```

Comparison operators. These perform a simple string comparison on the URI
string.

```c++
namespace Literals {
    inline Uri operator""_uri(const char* p, size_t n);
}
```

URI literal operator. This calls the string-based constructor, and will throw
the same exceptions.
