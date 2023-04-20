#pragma once

#include "crow/time.hpp"
#include "crow/types.hpp"
#include <atomic>
#include <chrono>
#include <concepts>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

namespace Crow {

    template <typename F>
    concept BenchmarkCallback = std::invocable<F>
        && requires (F f) { { f() } -> std::convertible_to<size_t>; };

    template <typename D = std::chrono::nanoseconds>
    class Benchmark;

    template <typename R, typename P>
    class Benchmark<std::chrono::duration<R, P>> {

    public:

        using duration_type = std::chrono::duration<R, P>;

        struct result_type {
            size_t iterations;
            size_t threads;
            duration_type total;
            duration_type average;
        };

        Benchmark(): Benchmark(std::thread::hardware_concurrency()) {}
        explicit Benchmark(unsigned threads): n_threads_(threads) {}

        ~Benchmark() = default;
        Benchmark(const Benchmark&) = delete;
        Benchmark(Benchmark&&) = delete;
        Benchmark& operator=(const Benchmark&) = delete;
        Benchmark& operator=(Benchmark&&) = delete;

        template <BenchmarkCallback F, typename R2, typename P2>
            result_type run(F call, std::chrono::duration<R2, P2> time);

        unsigned thread_count() const noexcept { return n_threads_; }

    private:

        unsigned n_threads_;

    };

        template <typename R, typename P>
        template <BenchmarkCallback F, typename R2, typename P2>
        typename Benchmark<std::chrono::duration<R, P>>::result_type
        Benchmark<std::chrono::duration<R, P>>::run(F call, std::chrono::duration<R2, P2> time) {

            using namespace std::chrono;

            static constexpr unsigned cycle = 100;

            std::atomic<size_t> count(0);
            std::atomic<size_t> sum(0);
            std::atomic<bool> stop(false);

            auto payload = [&] () {
                while (! stop) {
                    size_t acc = 0;
                    for (unsigned i = 0; i < cycle; ++i)
                        acc += size_t(call());
                    count += cycle;
                    sum += acc;
                }
            };

            std::vector<std::thread> threads;
            threads.reserve(n_threads_);
            auto start_time = system_clock::now();

            for (unsigned i = 0; i < n_threads_; ++i)
                threads.emplace_back(payload);

            std::this_thread::sleep_for(time);
            stop = true;

            for (auto& t: threads)
                t.join();

            auto stop_time = system_clock::now();
            auto total_seconds = duration_cast<Seconds>(stop_time - start_time);
            auto average_seconds = total_seconds * double(n_threads_) / double(count);
            auto total_time = duration_cast<duration_type>(total_seconds);
            auto average_time = duration_cast<duration_type>(average_seconds);

            if (sum == average_seconds.count())
                std::cerr << "\n";

            return {
                .iterations  = count,
                .threads     = n_threads_,
                .total       = total_time,
                .average     = average_time,
            };

        }

}
