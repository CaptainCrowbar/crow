#include "crow/thread-pool.hpp"
#include <random>

using namespace std::chrono;

namespace Crow {

    // Class ThreadPool

    ThreadPool::ThreadPool(int threads):
    clear_count_(0), next_worker_(0), unfinished_jobs_(0), shutting_down_(false), workers_(adjust_threads(threads)) {
        for (auto& work: workers_)
            work.thread = std::thread(thread_payload, this, &work);
    }

    ThreadPool::~ThreadPool() noexcept {
        clear();
        shutting_down_ = true;
        for (auto& work: workers_)
            work.thread.join();
    }

    void ThreadPool::clear() noexcept {
        ++clear_count_;
        for (auto& work: workers_) {
            std::unique_lock lock(work.mutex);
            unfinished_jobs_ -= int(work.queue.size());
            work.queue.clear();
        }
        wait();
        --clear_count_;
    }

    void ThreadPool::wait() noexcept {
        while (unfinished_jobs_)
            std::this_thread::sleep_for(1ms);
    }

    bool ThreadPool::wait_until(clock::time_point t) noexcept {
        for (;;) {
            if (! unfinished_jobs_)
                return true;
            if (clock::now() >= t)
                return false;
            std::this_thread::sleep_for(1ms);
        }
    }

    int ThreadPool::adjust_threads(int threads) noexcept {
        if (threads <= 0)
            threads = int(std::thread::hardware_concurrency());
        if (threads == 0)
            threads = 1;
        return threads;
    }

    void ThreadPool::thread_payload(ThreadPool* pool, worker* work) noexcept {
        auto address = uintptr_t(work);
        if constexpr (sizeof(uintptr_t) > sizeof(uint32_t))
            address ^= address >> 32;
        auto seed = uint32_t(address);
        std::minstd_rand rng(seed);
        std::uniform_int_distribution<int> random_index(0, pool->threads() - 1);
        callback call;
        for (;;) {
            call = {};
            {
                std::unique_lock lock(work->mutex);
                if (! work->queue.empty()) {
                    call = std::move(work->queue.back());
                    work->queue.pop_back();
                }
            }
            if (! call) {
                auto& worker = pool->workers_[random_index(rng)];
                std::unique_lock lock(worker.mutex);
                if (! worker.queue.empty()) {
                    call = std::move(worker.queue.front());
                    worker.queue.pop_front();
                }
            }
            if (call) {
                call();
                --pool->unfinished_jobs_;
            } else if (pool->shutting_down_) {
                break;
            } else {
                std::this_thread::sleep_for(1ms);
            }
        }
    }

}
