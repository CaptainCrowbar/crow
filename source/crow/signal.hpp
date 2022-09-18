#pragma once

#include "crow/channel.hpp"
#include "crow/io-utility.hpp"
#include "crow/types.hpp"
#include <atomic>
#include <csignal>
#include <deque>
#include <initializer_list>
#include <string>
#include <vector>

namespace Crow {

    class PosixSignal:
    public MessageChannel<int> {
    public:
        using signal_list = std::vector<int>;
        PosixSignal(std::initializer_list<int> list);
        explicit PosixSignal(const signal_list& list);
        ~PosixSignal() noexcept override;
        PosixSignal(const PosixSignal&) = delete;
        PosixSignal(PosixSignal&&) = delete;
        PosixSignal& operator=(const PosixSignal&) = delete;
        PosixSignal& operator=(PosixSignal&&) = delete;
        void close() noexcept override;
        bool is_closed() const noexcept override;
        bool is_synchronous() const noexcept override { return false; }
        bool read(int& t) override;
        bool wait_until(time_point t) override;
        static std::string name(int s);
    protected:
        bool do_wait_for(duration t) override;
    private:
        #ifdef _XOPEN_SOURCE
            signal_list signals_;
            sigset_t newmask_;
            sigset_t oldmask_;
            std::deque<int> queue_;
            std::atomic<bool> open_;
        #else
            static constexpr int max_signals = 128;
            std::vector<int> signals_;
            std::sig_atomic_t status_ = 0;
            static std::sig_atomic_t signal_status_[max_signals];
            bool available() const noexcept;
            void enable(int s);
            void reset() noexcept;
            static void signal_handler(int s);
        #endif
    };

}
