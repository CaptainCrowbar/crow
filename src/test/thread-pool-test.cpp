#include "crow/thread-pool.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <random>
#include <string>
#include <thread>

using namespace Crow;
using namespace std::chrono;

namespace {

    const std::string& alphabet() {
        static const std::string s = [] {
            std::string t;
            for (char c = 'a'; c <= 'z'; ++c)
                t += c;
            return t;
        }();
        return s;
    }

}

void test_crow_thread_pool_class() {

    ThreadPool pool;
    std::string log;
    std::mutex mutex;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 100);

    TEST_EQUAL(pool.threads(), int(std::thread::hardware_concurrency()));

    auto f = [&] (char c, int t) {
        std::this_thread::sleep_for(milliseconds(t));
        std::unique_lock lock(mutex);
        log += c;
    };

    for (char c = 'a'; c <= 'z'; ++c) {
        auto t = dist(rng);
        TRY(pool.insert([=] { f(c, t); }));
    }

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    TEST(log != alphabet());
    std::sort(log.begin(), log.end());
    TEST_EQUAL(log, alphabet());

    TRY(pool.clear());
    TEST_EQUAL(pool.threads(), int(std::thread::hardware_concurrency()));
    log.clear();

    for (char c = 'a'; c <= 'z'; ++c) {
        auto t = dist(rng);
        Callback cb = [=] { f(c, t); };
        TRY(pool.insert(cb));
    }

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    TEST(log != alphabet());
    std::sort(log.begin(), log.end());
    TEST_EQUAL(log, alphabet());

}

void test_crow_thread_pool_each() {

    ThreadPool pool;
    std::string log;
    std::mutex mutex;

    TRY(pool.each(10, [&] {
        std::unique_lock lock(mutex);
        log += 'a';
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log, std::string(10, 'a'));

    TRY(pool.clear());
    log.clear();

    TRY(pool.each(26, [&] (int i) {
        std::unique_lock lock(mutex);
        log += char('a' + i);
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    std::sort(log.begin(), log.end());
    TEST_EQUAL(log, alphabet());

    TRY(pool.clear());
    log.clear();

    TRY(pool.each('a', 1, 'z' + 1, [&] (int i) {
        std::unique_lock lock(mutex);
        log += char(i);
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    std::sort(log.begin(), log.end());
    TEST_EQUAL(log, alphabet());

    TRY(pool.clear());
    log.clear();

    TRY(pool.each(alphabet(), [&] {
        std::unique_lock lock(mutex);
        log += 'a';
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    TEST_EQUAL(log, std::string(alphabet().size(), 'a'));

    TRY(pool.clear());
    log.clear();

    TRY(pool.each(alphabet(), [&] (char c) {
        std::unique_lock lock(mutex);
        log += c;
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log.size(), alphabet().size());
    std::sort(log.begin(), log.end());
    TEST_EQUAL(log, alphabet());

    TRY(pool.clear());
    log = alphabet();

    TRY(pool.each(log, [&] (char& c) {
        std::unique_lock lock(mutex);
        c = ascii_toupper(c);
    }));

    TEST(pool.wait_for(5s));
    TEST_EQUAL(log, ascii_uppercase(alphabet()));

}

void test_crow_thread_pool_timing() {

    static constexpr int iterations = 100'000;

    ThreadPool pool;
    auto task = [&] {};

    auto start = system_clock::now();
    for (int i = 0; i < iterations; ++i)
        TRY(pool.insert(task));
    TEST(pool.wait_for(5s));
    auto stop = system_clock::now();

    double total = double(duration_cast<nanoseconds>(stop - start).count());
    double each = total / double(iterations);
    double rate = 1e9 / each;
    std::cout << "... Calls per second = " << uint64_t(rate) << std::endl;

}
