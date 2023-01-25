#include "crow/channel.hpp"
#include "crow/guard.hpp"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <stdexcept>
#include <thread>

using namespace std::chrono;
using namespace std::literals;

namespace Crow {

    // Class Channel

    Channel::~Channel() noexcept {
        if (dispatch_)
            dispatch_->drop_channel(*this);
    }

    // Class StreamChannel

    size_t StreamChannel::append(std::string& dst) {
        size_t pos = dst.size(), n = 0;
        auto guard = on_scope_exit([&] { dst.resize(pos + n); });
        dst.resize(pos + block_);
        n = read(&dst[0] + pos, block_);
        return n;
    }

    std::string StreamChannel::read_all() {
        std::string s;
        for (;;) {
            wait();
            if (is_closed())
                break;
            append(s);
        }
        return s;
    }

    // Class TimerChannel

    TimerChannel::TimerChannel(Channel::duration t, size_t count) noexcept {
        count_ = count;
        delta_ = std::max(duration_cast<Channel::duration>(t), Channel::duration());
        next_tick_ = clock::now() + delta_;
    }

    void TimerChannel::close() noexcept {
        std::unique_lock lock(mutex_);
        open_ = false;
        cv_.notify_all();
    }

    void TimerChannel::flush() noexcept {
        std::unique_lock lock(mutex_);
        if (! open_)
            return;
        auto now = clock::now();
        if (now < next_tick_)
            return;
        auto skip = size_t((now - next_tick_).count() / delta_.count()) + 1;
        step(skip);
    }

    bool TimerChannel::do_wait_for(duration t) {
        std::unique_lock lock(mutex_);
        if (! open_)
            return true;
        auto now = clock::now();
        if (next_tick_ <= now) {
            step();
            return true;
        }
        if (t <= duration())
            return false;
        auto remaining = duration_cast<duration>(next_tick_ - now);
        if (t < remaining) {
            cv_.wait_for(lock, t, [&] { return ! open_; });
            return ! open_;
        }
        cv_.wait_for(lock, remaining, [&] { return ! open_; });
        if (open_)
            step();
        return true;
    }

    void TimerChannel::step(size_t n) {
        n = std::min(n, count_);
        count_ -= n;
        next_tick_ += n * delta_;
        if (count_ == 0) {
            open_ = false;
            cv_.notify_all();
        }
    }

    // Class BufferChannel

    void BufferChannel::close() noexcept {
        std::unique_lock lock(mutex_);
        buf_.clear();
        ofs_ = 0;
        open_ = false;
        cv_.notify_all();
    }

    size_t BufferChannel::read(void* dst, size_t maxlen) {
        if (! dst || ! maxlen)
            return 0;
        std::unique_lock lock(mutex_);
        if (! open_)
            return 0;
        size_t n = std::min(buf_.size() - ofs_, maxlen);
        std::memcpy(dst, buf_.data() + ofs_, n);
        ofs_ += n;
        if (ofs_ == buf_.size()) {
            buf_.clear();
            ofs_ = 0;
        } else if (2 * ofs_ >= buf_.size()) {
            buf_.erase(0, ofs_);
            ofs_ = 0;
        }
        return n;
    }

    bool BufferChannel::write(const void* src, size_t len) {
        std::unique_lock lock(mutex_);
        if (! open_)
            return false;
        buf_.append(static_cast<const char*>(src), len);
        if (ofs_ < buf_.size())
            cv_.notify_all();
        return true;
    }

    void BufferChannel::clear() noexcept {
        std::unique_lock lock(mutex_);
        buf_.clear();
        ofs_ = 0;
    }

    bool BufferChannel::do_wait_for(duration t) {
        std::unique_lock lock(mutex_);
        if (open_ && ofs_ == buf_.size() && t > duration())
            cv_.wait_for(lock, t, [&] { return ! open_ || ofs_ < buf_.size(); });
        return ! open_ || ofs_ < buf_.size();
    }

    // Class Dispatch

    Dispatch::~Dispatch() noexcept {
        stop();
        for (auto& [chan,task]: tasks_)
            chan->dispatch_ = nullptr;
    }

    Dispatch::result Dispatch::run() noexcept {
        static constexpr Channel::duration min_interval = 1us;
        static constexpr Channel::duration max_interval = 1ms;
        if (tasks_.empty())
            return {};
        auto interval = min_interval;
        int waits = 0;
        for (;;) {
            int calls = 0;
            for (auto& [chan,task]: tasks_) {
                if (chan->is_synchronous()) {
                    try {
                        if (chan->poll()) {
                            if (chan->is_closed()) {
                                set_fault(*chan);
                                break;
                            }
                            task->handler();
                            ++calls;
                        }
                    }
                    catch (...) {
                        set_fault(*chan, std::current_exception());
                        break;
                    }
                }
            }
            {
                std::unique_lock lock(faults_mutex_);
                if (! faults_.empty()) {
                    auto fault = faults_.front();
                    faults_.pop_front();
                    if (fault.channel)
                        drop_channel(*fault.channel);
                    return fault;
                }
            }
            if (calls == 0) {
                if (++waits == 1)
                    interval = min_interval;
                else
                    interval = std::min(2 * interval, max_interval);
                std::this_thread::sleep_for(interval);
            } else {
                waits = 0;
                std::this_thread::yield();
            }
        }
    }

    void Dispatch::stop() noexcept {
        for (auto& [chan,task]: tasks_)
            chan->close();
        while (! tasks_.empty())
            run();
    }

    void Dispatch::add_channel(Channel& c, Callback f) {
        if (tasks_.count(&c))
            throw std::invalid_argument("Duplicate channel in dispatch set");
        auto task_ptr = std::make_unique<task_info>();
        auto& task = *task_ptr;
        task.handler = f;
        tasks_[&c] = std::move(task_ptr);
        if (! c.is_synchronous()) {
            task.thread = Thread([&] () noexcept {
                try {
                    for (;;) {
                        c.wait();
                        if (c.is_closed()) {
                            set_fault(c);
                            break;
                        }
                        task.handler();
                    }
                }
                catch (...) {
                    set_fault(c, std::current_exception());
                }
            });
        }
        c.dispatch_ = this;
    }

    void Dispatch::drop_channel(Channel& c) noexcept {
        tasks_.erase(&c);
        c.dispatch_ = nullptr;
    }

    void Dispatch::set_fault(Channel& c, std::exception_ptr e) {
        std::unique_lock lock(faults_mutex_);
        faults_.push_back({&c, e});
        c.close();
    }

}
