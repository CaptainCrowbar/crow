#pragma once

#include "crow/io-utility.hpp"
#include "crow/thread.hpp"
#include "crow/time.hpp"
#include "crow/types.hpp"
#include <chrono>
#include <condition_variable>
#include <deque>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Crow {

    // Forward declarations

    class BufferChannel;
    class Channel;
    class Dispatch;
    template <typename T> class GeneratorChannel;
    template <typename T> class MessageChannel;
    template <typename T> class QueueChannel;
    class StreamChannel;
    class TimerChannel;
    template <typename T> class ValueChannel;

    // Channel base class

    class Channel:
    public Waiter {

    public:

        ~Channel() noexcept override;
        Channel(const Channel&) = delete;
        Channel(Channel&& c) = delete;
        Channel& operator=(const Channel&) = delete;
        Channel& operator=(Channel&& c) = delete;

        virtual void close() noexcept = 0;
        virtual bool is_closed() const noexcept = 0;
        virtual bool is_synchronous() const noexcept { return false; }

    protected:

        #ifdef _XOPEN_SOURCE
            using native_handle = int; // file handle
        #else
            using native_handle = void*; // HANDLE
        #endif

        Channel() = default;

        virtual native_handle get_handle() const noexcept { return {}; }

    private:

        friend class Dispatch;

        Dispatch* dispatch_ = nullptr;

    };

    // Intermediate base classes

    template <typename T>
    class MessageChannel:
    public Channel {
    public:
        using value_type = T;
        virtual bool read(T& t) = 0;
        std::optional<T> read_maybe() { T t; if (read(t)) return t; else return {}; }
    protected:
        MessageChannel() = default;
    };

    template <>
    class MessageChannel<void>:
    public Channel {
    public:
        using value_type = void;
    protected:
        MessageChannel() = default;
    };

    class StreamChannel:
    public Channel {
    public:
        static constexpr size_t default_block_size = 65536;
        virtual size_t read(void* dst, size_t maxlen) = 0;
        size_t append(std::string& dst);
        size_t block_size() const noexcept { return block_; }
        std::string read_all();
        void set_block_size(size_t n) noexcept { block_ = n; }
    protected:
        StreamChannel() = default;
    private:
        size_t block_ = default_block_size;
    };

    // Concrete channel classes

    class TimerChannel:
    public MessageChannel<void> {
    public:
        explicit TimerChannel(Channel::duration t, size_t count = npos) noexcept;
        TimerChannel(const TimerChannel&) = delete;
        TimerChannel(TimerChannel&&) = delete;
        TimerChannel& operator=(const TimerChannel&) = delete;
        TimerChannel& operator=(TimerChannel&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override { return ! open_; }
        void flush() noexcept;
        duration interval() const noexcept { return delta_; }
        auto next() const noexcept { return next_tick_; }
    protected:
        bool do_wait_for(duration t) override;
    private:
        mutable std::mutex mutex_;
        std::condition_variable cv_;
        time_point next_tick_;
        size_t count_;
        duration delta_;
        bool open_ = true;
        void step(size_t n = 1);
    };

    template <typename T>
    class GeneratorChannel:
    public MessageChannel<T> {
    public:
        using generator = std::function<T()>;
        template <typename F> explicit GeneratorChannel(F f): mutex_(), gen_(f) {}
        GeneratorChannel(const GeneratorChannel&) = delete;
        GeneratorChannel(GeneratorChannel&&) = delete;
        GeneratorChannel& operator=(const GeneratorChannel&) = delete;
        GeneratorChannel& operator=(GeneratorChannel&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override { return ! gen_; }
        bool read(T& t) override;
    protected:
        bool do_wait_for(Channel::duration /*t*/) override { return true; }
    private:
        std::mutex mutex_;
        generator gen_;
    };

        template <typename T>
        void GeneratorChannel<T>::close() noexcept {
            std::unique_lock lock(mutex_);
            gen_ = nullptr;
        }

        template <typename T>
        bool GeneratorChannel<T>::read(T& t) {
            std::unique_lock lock(mutex_);
            if (gen_)
                t = gen_();
            return bool(gen_);
        }

    template <typename T>
    class QueueChannel:
    public MessageChannel<T> {
    public:
        QueueChannel() = default;
        QueueChannel(const QueueChannel&) = delete;
        QueueChannel(QueueChannel&&) = delete;
        QueueChannel& operator=(const QueueChannel&) = delete;
        QueueChannel& operator=(QueueChannel&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override { return ! open_; }
        bool read(T& t) override;
        void clear() noexcept;
        bool write(const T& t);
        bool write(T&& t);
    protected:
        bool do_wait_for(Channel::duration t) override;
    private:
        std::mutex mutex_;
        std::condition_variable cv_;
        bool open_ = true;
        std::deque<T> queue_;
    };

        template <typename T>
        void QueueChannel<T>::close() noexcept {
            std::unique_lock lock(mutex_);
            open_ = false;
            cv_.notify_all();
        }

        template <typename T>
        bool QueueChannel<T>::read(T& t) {
            std::unique_lock lock(mutex_);
            if (! open_ || queue_.empty())
                return false;
            t = queue_.front();
            queue_.pop_front();
            if (! queue_.empty())
                cv_.notify_all();
            return true;
        }

        template <typename T>
        void QueueChannel<T>::clear() noexcept {
            std::unique_lock lock(mutex_);
            queue_.clear();
        }

        template <typename T>
        bool QueueChannel<T>::write(const T& t) {
            std::unique_lock lock(mutex_);
            if (! open_)
                return false;
            queue_.push_back(t);
            cv_.notify_all();
            return true;
        }

        template <typename T>
        bool QueueChannel<T>::write(T&& t) {
            std::unique_lock lock(mutex_);
            if (! open_)
                return false;
            queue_.push_back(std::move(t));
            cv_.notify_all();
            return true;
        }

        template <typename T>
        bool QueueChannel<T>::do_wait_for(Channel::duration t) {
            std::unique_lock lock(mutex_);
            if (open_ && queue_.empty() && t > Channel::duration())
                cv_.wait_for(lock, t, [&] { return ! open_ || ! queue_.empty(); });
            return ! open_ || ! queue_.empty();
        }

    template <typename T>
    class ValueChannel:
    public MessageChannel<T> {
    public:
        ValueChannel() = default;
        explicit ValueChannel(const T& t): value_(t) {}
        ValueChannel(const ValueChannel&) = delete;
        ValueChannel(ValueChannel&&) = delete;
        ValueChannel& operator=(const ValueChannel&) = delete;
        ValueChannel& operator=(ValueChannel&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override { return status == -1; }
        bool read(T& t) override;
        void clear() noexcept;
        bool write(const T& t);
        bool write(T&& t);
    protected:
        bool do_wait_for(Channel::duration t) override;
    private:
        std::mutex mutex_;
        std::condition_variable cv_;
        T value_;
        int status = 0; // +1 = new value, 0 = no change, -1 = closed
    };

        template <typename T>
        void ValueChannel<T>::close() noexcept {
            std::unique_lock lock(mutex_);
            status = -1;
            cv_.notify_all();
        }

        template <typename T>
        bool ValueChannel<T>::read(T& t) {
            std::unique_lock lock(mutex_);
            if (status != 1)
                return false;
            t = value_;
            status = 0;
            return true;
        }

        template <typename T>
        bool ValueChannel<T>::write(const T& t) {
            std::unique_lock lock(mutex_);
            if (status == -1)
                return false;
            if (t == value_)
                return true;
            value_ = t;
            status = 1;
            cv_.notify_all();
            return true;
        }

        template <typename T>
        bool ValueChannel<T>::write(T&& t) {
            std::unique_lock lock(mutex_);
            if (status == -1)
                return false;
            if (t == value_)
                return true;
            value_ = std::move(t);
            status = 1;
            cv_.notify_all();
            return true;
        }

        template <typename T>
        bool ValueChannel<T>::do_wait_for(Channel::duration t) {
            std::unique_lock lock(mutex_);
            if (status == 0 && t > Channel::duration())
                cv_.wait_for(lock, t, [&] { return status != 0; });
            return status;
        }

    class BufferChannel:
    public StreamChannel {
    public:
        BufferChannel() = default;
        BufferChannel(const BufferChannel&) = delete;
        BufferChannel(BufferChannel&&) = delete;
        BufferChannel& operator=(const BufferChannel&) = delete;
        BufferChannel& operator=(BufferChannel&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override { return ! open_; }
        size_t read(void* dst, size_t maxlen) override;
        void clear() noexcept;
        bool write(std::string_view src) { return write(src.data(), src.size()); }
        bool write(const void* src, size_t len);
    protected:
        bool do_wait_for(duration t) override;
    private:
        std::mutex mutex_;
        std::condition_variable cv_;
        std::string buf_;
        size_t ofs_ = 0;
        bool open_ = true;
    };

    // Dispatch controller class

    class Dispatch {

    public:

        struct result {
            Channel* channel = nullptr;
            std::exception_ptr error;
            bool is_closed() const noexcept { return channel && ! error; }
            bool is_empty() const noexcept { return ! channel; }
            bool is_error() const noexcept { return bool(error); }
        };

        Dispatch() = default;
        ~Dispatch() noexcept;
        Dispatch(const Dispatch&) = delete;
        Dispatch(Dispatch&&) = delete;
        Dispatch& operator=(const Dispatch&) = delete;
        Dispatch& operator=(Dispatch&&) = delete;

        template <typename T, typename F> void add(MessageChannel<T>& c, F f);
        template <typename F> void add(MessageChannel<void>& c, F f);
        template <typename F> void add(StreamChannel& c, F f);
        bool empty() noexcept { return tasks_.empty(); }
        result run() noexcept;
        void stop() noexcept;

    private:

        friend class Channel;

        struct task_info {
            Thread thread;
            std::function<void()> handler;
        };

        std::map<Channel*, std::unique_ptr<task_info>> tasks_;
        std::deque<result> faults_;
        std::mutex faults_mutex_;

        void add_channel(Channel& c, std::function<void()> f);
        void drop_channel(Channel& c) noexcept;
        void set_fault(Channel& c, std::exception_ptr e = {});

        template <typename Arg, typename F> static void check_call(F& f);

    };

        template <typename T, typename F>
        void Dispatch::add(MessageChannel<T>& c, F f) {
            check_call<const T&>(f);
            add_channel(c, [&c,f,t=T()] () mutable { if (c.read(t)) f(t); });
        }

        template <typename F>
        void Dispatch::add(MessageChannel<void>& c, F f) {
            check_call<void>(f);
            add_channel(c, f);
        }

        template <typename F>
        void Dispatch::add(StreamChannel& c, F f) {
            check_call<std::string&>(f);
            add_channel(c, [&c,f,s=std::string()] () mutable { if (c.append(s)) f(s); });
        }

        template <typename Arg, typename F>
        void Dispatch::check_call(F& f) {
            if constexpr (std::is_void_v<Arg>)
                static_assert(std::is_convertible_v<F, std::function<void()>>, "Invalid callback type");
            else
                static_assert(std::is_convertible_v<F, std::function<void(Arg)>>, "Invalid callback type");
            if constexpr (std::is_constructible_v<bool, F>)
                if (! bool(f))
                    throw std::bad_function_call();
        }

}
