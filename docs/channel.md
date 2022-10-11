# Message Dispatch

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/channel.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Class hierarchy

```c++
/*abstract*/ class Channel: public Waiter;
    /*abstract*/ template <typename T> class MessageChannel: public Channel;
        template <typename T> class GeneratorChannel: public MessageChannel<T>;
        template <typename T> class QueueChannel: public MessageChannel<T>;
        class TimerChannel: public MessageChannel<void>;
        template <typename T> class ValueChannel: public MessageChannel<T>;
    /*abstract*/ class StreamChannel: public Channel;
        class BufferChannel: public StreamChannel;
class Dispatch;
```

## Channel base class

```c++
class Channel: public Waiter;
    virtual Channel::~Channel() noexcept;
    virtual void Channel::close() noexcept = 0;
    virtual bool Channel::is_closed() const noexcept = 0;
    virtual bool Channel::is_synchronous() const noexcept;
```

The base class for all readable message channels. All concrete channel classes
must derive from one of the two intermediate classes below, not directly from
`Channel`.

Derived classes need to implement at least `close()`, `is_closed()`, and one
of the `wait_for/until()` functions. The `close()` implementation must be
async safe and idempotent.

Any of the poll or wait functions will return true if the channel is either
ready for reading or closed.

If `is_synchronous()` is true, the channel can only be used in a synchronous
dispatch handler, usually because it calls an underlying native API that is
only intended to be used from the main thread. The default implementation
returns false;

## Intermediate base classes

### Class MessageChannel

```c++
template <typename T> class MessageChannel: public Channel;
    using MessageChannel::value_type = T;
    virtual bool MessageChannel::read(T& t) = 0;
    Optional<T> MessageChannel::read_maybe();
    protected MessageChannel::MessageChannel() noexcept;
template <> class MessageChannel<void>: public Channel;
    using MessageChannel::value_type = void;
    protected MessageChannel::MessageChannel() noexcept;
```

Intermediate base class for message channels (channels that send discrete
objects). When a message channel is added to a dispatch set, the callback
function will be applied to each incoming message object. The message type
`T` must be at least movable or copyable; if `read_maybe()` is called it must
also be default constructible. Derived classes may impose additional
restrictions.

A concrete message channel class must implement `read()` as well as the usual
`Channel` members. The `read()` function should return true on a successful
read, false if the channel is closed. It will normally be called only after a
successful wait; if no data is immediately available, `read()` may block or
return false. The state of the argument to `read()` if it returns false is
unspecified.

The `read_maybe()` function calls `read()` and returns the message on success,
or an empty value on failure.

The `MessageChannel<void>` specialisation should be used as a base class for
channels that carry no information beyond the fact that an event has
happened.

### Class StreamChannel

```c++
class StreamChannel: public Channel;
    static constexpr size_t StreamChannel::default_block_size = 65536;
    virtual size_t StreamChannel::read(void* dst, size_t maxlen) = 0;
    size_t StreamChannel::append(std::string& dst);
    size_t StreamChannel::block_size() const noexcept;
    std::string StreamChannel::read_all();
    void StreamChannel::set_block_size(size_t n) noexcept;
    protected StreamChannel::StreamChannel() noexcept;
```

Intermediate base class for byte stream channels. When a stream channel is
added to a dispatch set, the callback function will be invoked whenever more
data arrives.

Stream channels must implement `read()` as well as the usual `Channel`
members. The `read()` function should return the number of bytes read on
success, or zero if the channel is closed; if no data is available it may
block or return zero. If `dst` is null or `maxlen` is zero, `read()` should
return zero immediately.

The `append()` function calls `read()` and appends any incoming data to a
string. The `read_all()` function will read until the channel is closed,
blocking as necessary. The buffer size functions control the internal block
size used in `append()` and `read_all()`.

## Concrete channel classes

Member functions inherited from the channel base classes are not documented
again here unless they have significantly different semantics.

### Class TimerChannel

```c++
class TimerChannel: public MessageChannel<void>;
    explicit TimerChannel::TimerChannel(Channel::duration t,
        size_t count = npos) noexcept;
    void TimerChannel::flush() noexcept;
    Channel::duration TimerChannel::interval() const noexcept;
    auto TimerChannel::next() const noexcept;
```

A channel that delivers one tick every interval, starting at one interval
after the time of construction. Multiple ticks may be delivered at once
(represented by repeated calls to wait functions returning success
immediately) if multiple intervals have elapsed since the last check.
Optionally a limited number of ticks can be specified in the constructor; the
channel will automatically close itself after the specified number of ticks
have been delivered.

The `next()` function returns the time of the next tick (this may be in the
past if multiple ticks are pending); `flush()` discards any pending ticks.
These are async safe and can be called from any thread.

### Class GeneratorChannel

```c++
template <typename T> class GeneratorChannel: public MessageChannel<T>;
    using GeneratorChannel::generator = std::function<T()>;
    template <typename F> explicit GeneratorChannel::GeneratorChannel(F f);
```

A channel that calls a function to generate incoming messages. A default
constructed channel is always closed. Waiting on an open channel always
succeeds immediately, apart from any delay involved in the callback function
itself.

### Class QueueChannel

```c++
template <typename T> class QueueChannel: public MessageChannel<T>;
    QueueChannel::QueueChannel();
    void QueueChannel::clear() noexcept;
    bool QueueChannel::write(const T& t);
    bool QueueChannel::write(T&& t);
```

A last in, first out message queue.

### Class ValueChannel

```c++
template <typename T> class ValueChannel: public MessageChannel<T>;
    ValueChannel::ValueChannel();
    explicit ValueChannel::ValueChannel(const T& t);
    void ValueChannel::clear() noexcept;
    bool ValueChannel::write(const T& t);
    bool ValueChannel::write(T&& t);
```

A single value buffer channel. Any value written into the channel overwrites
any other value that was written earlier but has not yet been read. The
channel only reports a new message when the value is changed; writing the
same value again will succeed but will not trigger a read. `T` must be
equality comparable.

### Class BufferChannel

```c++
class BufferChannel: public StreamChannel;
    BufferChannel::BufferChannel();
    void BufferChannel::clear() noexcept;
    bool BufferChannel::write(std::string_view src);
    bool BufferChannel::write(const void* src, size_t len);
```

A byte stream buffer channel. Message boundaries are not preserved; calling
`read()` will extract all pending data up to the length limit, regardless of
whether it was written as a single block or multiple smaller blocks.

## Dispatch controller class

```c++
class Dispatch;
    struct Dispatch::result;
        Channel* result::channel = nullptr;
        std::exception_ptr result::error;
        bool result::is_closed() const noexcept;
        bool result::is_empty() const noexcept;
        bool result::is_error() const noexcept;
    Dispatch::Dispatch();
    Dispatch::~Dispatch() noexcept;
    template <typename T, typename F>
        void Dispatch::add(MessageChannel<T>& c, F f);
    template <typename F>
        void Dispatch::add(MessageChannel<void>& c, F f);
    template <typename F>
        void Dispatch::add(StreamChannel& c, F f);
    bool Dispatch::empty() noexcept;
    Dispatch::result Dispatch::run() noexcept;
    void Dispatch::stop() noexcept;
```

The `Dispatch` class controls message dispatch.

The `add()` functions take a channel and a callback function. The callback
type depends on the channel type:

* `MessageChannel<T>` expects a callback that takes a `const T&` argument.
* `MessageChannel<void>` expects a callback that takes no arguments.
* `StreamChannel` expects a callback that takes a `std::string&` argument.

For stream channels, the callback is expected to remove any bytes from the
string that it has processed; the dispatcher will add to the string but not
remove anything from it.

Add channels and their associated callback functions to the queue, then call
`run()` to start the dispatch loop. Behaviour is undefined if `run()` is
called from multiple threads at the same time; normally it should only be
called from the main thread.

Dispatch functions must only be called synchronously (i.e. not from inside an
asynchronous callback function). Channels can safely be destroyed while in a
dispatch set (the dispatch queue will notice this and silently drop the
channel), provided this is done synchronously.

The `add()` functions start a synchronous or asynchronous task reading from
the channel. Asynchronous handlers are started immediately in a separate
thread; synchronous handlers will not be executed until `run()` is called. An
`add()` function will throw `std::invalid_argument` if the same channel is
added more than once, or `std::bad_function_call` if the callback function is
null.

The `empty()` function returns true if the dispatch list is currently empty.

œThe `run()` function runs until a channel is closed or a callback function
throws an exception; it returns immediately if the dispatch set is empty.
Asynchronous message handlers will continue to run in their own threads
regardless of whether the dispatch thread is currently calling `run()`.

The `stop()` function closes all channels and waits for any currently running
event handlers to finish. This is called by the destructor if the dispatch
queue is not empty.

The `result` structure returned from `run()` indicates why the dispatch loop
was stopped. Exactly one of the `result::is_*()` functions will return true.

Status         | Description                                         | `result::channel`     | `result::error`
------         | -----------                                         | -----------------     | ---------------
`is_closed()`  | A channel was closed                                | The closed channel    | Null
`is_empty()`   | The dispatch set was empty when `run()` was called  | Null                  | Null
`is_error()`   | A message handler callback threw an exception       | The throwing channel  | The exception thrown

Behaviour is undefined if `run()` is called in a thread other than the
application's main thread when the dispatch thread contains any synchronous
channels.
