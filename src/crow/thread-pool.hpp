#pragma once

#include "crow/types.hpp"
#include <atomic>
#include <chrono>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    class ThreadPool {

    public:

        using clock = std::chrono::system_clock;

        ThreadPool(): ThreadPool(0) {}
        explicit ThreadPool(int threads);
        ~ThreadPool() noexcept;
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        int threads() const noexcept { return int(workers_.size()); }
        template <typename F> void insert(F&& f);
        template <typename F> void each(int n, F&& f) { each(0, 1, n, std::forward<F>(f)); }
        template <typename F> void each(int start, int delta, int stop, F&& f);
        template <typename Range, typename F> void each(Range& range, F&& f);
        void clear() noexcept;
        bool poll() { return ! unfinished_jobs_; }
        void wait() noexcept;
        template <typename R, typename P> bool wait_for(std::chrono::duration<R, P> t) noexcept;
        bool wait_until(clock::time_point t) noexcept;

    private:

        struct worker {
            std::mutex mutex;
            std::deque<Callback> queue;
            std::thread thread;
        };

        std::atomic<int> clear_count_;
        std::atomic<int> next_worker_;
        std::atomic<int> unfinished_jobs_;
        std::atomic<bool> shutting_down_;
        std::vector<worker> workers_;

        static int adjust_threads(int threads) noexcept;
        static void thread_payload(ThreadPool* pool, worker* work) noexcept;

    };

        template <typename F>
        void ThreadPool::insert(F&& f) {
            if (clear_count_)
                return;
            int index = next_worker_;
            next_worker_ = (index + 1) % threads();
            auto& work = workers_[index];
            std::unique_lock lock(work.mutex);
            ++unfinished_jobs_;
            work.queue.emplace_back(std::forward<F>(f));
        }

        template <typename F>
        void ThreadPool::each(int start, int delta, int stop, F&& f) {
            using function_type = std::function<void(int)>;
            const auto do_insert = [this,f] (int i) {
                (void)i; // MSVC brain damage
                if constexpr (std::is_convertible_v<F, function_type>)
                    insert([f,i] { f(i); });
                else
                    insert(f);
            };
            if (delta >= 0) {
                for (int i = start; i < stop; i += delta)
                    do_insert(i);
            } else {
                for (int i = start; i > stop; i += delta)
                    do_insert(i);
            }
        }

        template <typename Range, typename F>
        void ThreadPool::each(Range& range, F&& f) {
            using value_type = RangeValue<Range>;
            using function_type = std::function<void(value_type&)>;
            for (auto& x: range) {
                (void)x; // MSVC brain damage
                if constexpr (std::is_convertible_v<F, function_type>)
                    insert([f,&x] { f(x); });
                else
                    insert(f);
            }
        }

        template <typename R, typename P>
        bool ThreadPool::wait_for(std::chrono::duration<R, P> t) noexcept {
            return wait_until(clock::now() + t);
        }

}
