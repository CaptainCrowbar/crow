#pragma once

#include "crow/types.hpp"
#include <thread>
#include <utility>

namespace Crow {

    class Thread:
    public std::thread {
    public:
        Thread() = default;
        template <typename F, typename... Args> explicit Thread(F&& f, Args&&... args):
            std::thread(std::forward<F>(f), std::forward<Args>(args)...) {}
        ~Thread() noexcept { close(); }
        Thread(const Thread&) = delete;
        Thread(Thread&&) = default;
        Thread& operator=(const Thread&) = delete;
        Thread& operator=(Thread&& t) noexcept { close(); std::thread::operator=(std::move(t)); return *this; }
        void swap(Thread& t) noexcept { t.std::thread::swap(*this); }
    private:
        void close() noexcept { try { if (joinable()) join(); } catch (...) {} }
    };

}
