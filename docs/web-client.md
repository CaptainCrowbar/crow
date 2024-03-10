# Web Client

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/web-client.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Exceptions

```c++
class CurlError: public std::runtime_error {
    int error() const noexcept;
};
```

Exception used for passing on errors returned by the Libcurl API.


## WebClient class

```c++
class WebClient;
```

The main web client class. This class is a C++ wrapper around Libcurl's API.

(Currently not very complete.)

### Member types

```c++
using WebClient::headers = StableMap<std::string, std::string>;
```

A map used to record HTTP headers.

```c++
enum class WebClient::method: int {
    del, // delete
    get,
    head,
    post,
    put,
};
```

The HTTP methods supported by Libcurl. `DELETE, POST,` and `PUT` are not
supported yet by this library.

```c++
struct WebClient::parameters {
    headers head;
    std::string body;
    parameters();
    parameters(const headers& h);
    parameters(const std::string& b);
    parameters(const headers& h, const std::string& b);
    void clear() noexcept;
};
```

Used for passing the input and output parameters to a request.

```c++
class WebClient::progress {
    using callback = std::function<bool(int64_t dltotal, int64_t dlnow)>;
    progress(WebClient& c, callback on_download);
    ~progress() noexcept;
};
```

Add a progress recording function by constructing a `progress` object on a
`WebClient`. The constructor may throw `CurlError`.

This keeps a reference to the client; behaviour is undefined if the client is
destroyed while it has any progress trackers attached to it.

The callback is passed the expected total and the current download byte count.
The expected total is often only an estimate, and may be unreliable or
unavailable; the callback should be prepared to handle the possibility that
`dltotal` may be zero or less than `dlnow`.

### Constants

```c++
static constexpr [duration type] WebClient::default_connect_timeout = 15s;
static constexpr [duration type] WebClient::default_request_timeout = 60s;
static constexpr int WebClient::default_redirect_limit = 10;
```

Default values for client settings.

### Life cycle functions

```c++
WebClient::WebClient();
```

The constructor initializes the Libcurl API, and may throw `CurlError` if
anything goes wrong.

```c++
WebClient::~WebClient() noexcept;
WebClient::WebClient(WebClient&& c) noexcept;
WebClient& WebClient::operator=(WebClient&& c) noexcept;
```

Other life cycle functions. `WebClient` objects are not copyable.

### Web request functions

```c++
HttpStatus WebClient::request(const Uri& uri, parameters& response,
    const parameters& params = {}, method m = method::get);
HttpStatus WebClient::operator()(const Uri& uri, parameters& response,
    const parameters& params = {}, method m = method::get);
```

Perform an HTTP[S] request. The `response` fields will be filled with the
headers and body from the HTTP response. These may throw `CurlError` if
anything goes wrong internally, but actual HTTP errors will be returned as
status codes. If the request fails, either by throwing an exception or
returning an error code, the response fields will be empty.

Currently this does not support `DELETE, POST,` or `PUT` requests; the `body`
field of the `params` argument is ignored.

### Parameter setting functions

```c++
template <typename R, typename P>
    void WebClient::set_connect_timeout(std::chrono::duration<R, P> t);
template <typename R, typename P>
    void WebClient::set_request_timeout(std::chrono::duration<R, P> t);
```

Set timeouts for the initial connection and the complete request. These may
throw `CurlError`.

```c++
void WebClient::set_redirect_limit(int n);
```

Set the maximum number of redirections before giving up. This may throw
`CurlError`.

```c++
void WebClient::set_user_agent(const std::string& user_agent);
```

Set the user agent string. This may throw `CurlError`.

```c++
void WebClient::set_verbose(bool flag);
```

Set Libcurl's verbose mode for debugging. Output will go to standard error.

### Native libcurl functions

```c++
const std::string& WebClient::native_error() const noexcept;
```

Returns the Libcurl error message from the most recent call, if any.

```c++
Curl_easy* WebClient::native_handle() const noexcept;
```

Returns the underlying Libcurl API handle.
