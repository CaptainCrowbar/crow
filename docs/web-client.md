# Web Client

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/web-client.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
class CurlError: public std::runtime_error {
    int error() const noexcept;
};
```

Exception used for passing on errors returned by the Libcurl API.

```c++
using WebHeaders = std::multimap<std::string, std::string>;
```

A map used to record HTTP headers.

```c++
enum class WebMethod: int {
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
struct WebParameters {
    WebHeaders head;
    std::string body;
    WebParameters();
    WebParameters(const WebHeaders& h);
    WebParameters(const std::string& b);
    WebParameters(const WebHeaders& h, const std::string& b);
    void clear() noexcept;
};
```

Used for passing the input and output parameters to a request.

## WebClient class

```c++
class WebClient;
```

The main web client class. This class is a C++ wrapper around Libcurl's API.

(Currently not very complete.)

```c++
static constexpr [duration type] WebClient::default_connect_timeout = 15s;
static constexpr [duration type] WebClient::default_request_timeout = 60s;
static constexpr int WebClient::default_redirect_limit = 10;
```

Default values for client settings.

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

```c++
HttpStatus WebClient::request(const Uri& uri, WebParameters& response,
    WebMethod method = WebMethod::get, const WebParameters& params = {});
HttpStatus WebClient::operator()(const Uri& uri, WebParameters& response,
    WebMethod method = WebMethod::get, const WebParameters& params = {});
```

Perform an HTTP[S] request. The `response` fields will be filled with the
headers and body from the HTTP response. These may throw `CurlError` if
anything goes wrong internally, but actual HTTP errors will be returned as
status codes. If the request fails, either by throwing an exception or
returning an error code, the response fields will be empty.

Currently this does not support `DELETE, POST,` or `PUT` requests; the `body`
field of the `params` argument is ignored.

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

```c++
Curl_easy* WebClient::native_handle() const noexcept;
```

Returns the underlying Libcurl API handle.

## Progress callback class

```c++
class WebProgress {
    using callback = std::function<bool(int64_t dltotal, int64_t dlnow)>;
    WebProgress(WebClient& c, callback on_download);
    ~WebProgress() noexcept;
};
```

Add a progress recording function by constructing an `WebProgress` object on a
`WebClient`. The constructor may throw `CurlError`.

This keeps a reference to the client; behaviour is undefined if the client is
destroyed while it has any progress trackers attached to it.

The callback is passed the expected total and the current download byte count.
The expected total is often only an estimate, and may be unreliable or
unavailable; the callback should be prepared to handle the possibility that
`dltotal` may be zero or less than `dlnow`.
