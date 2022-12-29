# Benchmarking

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/benchmark.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Concepts

```c++
template <typename F> concept BenchmarkCallback =
    std::invocable<F>
    && requires (F f) { { f() } -> std::convertible_to<size_t>; };
```

Concept for a benchmark callback function. Callback functions must return an
integer so that the benchmark code can prevent the compiler from optimizing
the call out of existence.

## Benchmark class

```c++
template <typename D = std::chrono::nanoseconds> class Benchmark;
```

Generic benchmark template. This is only defined for duration types, as shown
below.

```c++
template <typename R, typename P>
class Benchmark<std::chrono::duration<R, P>> {
    using duration_type = std::chrono::duration<R, P>;
    struct result_type {
        size_t iterations;      // Total calls over all threads
        size_t threads;         // Number of threads
        duration_type total;    // Total run time per thread
        duration_type average;  // Average time per call
    };
    Benchmark();
    explicit Benchmark(unsigned threads);
    ~Benchmark() noexcept;
    template <BenchmarkCallback F, typename R2, typename P2>
        result_type run(F call, std::chrono::duration<R2, P2> time);
    unsigned thread_count() const noexcept;
};
```

A class that performs simple benchmarking runs. The template parameter is the
duration type used to return the benchmark results, defaulting to
nanoseconds.

The constructor argument indicates how many threads to use for benchmarking;
it defaults to the hardware concurrency.

The `run()` function performs the actual benchmarking, calling the function in
a loop in the specified number of threads for approximately the specified
time, and reporting the average time per call. The time should be at least
long enough for several hundred calls for meaningful results.
