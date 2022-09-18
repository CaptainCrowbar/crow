#include "crow/thread.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <mutex>
#include <string>
#include <thread>

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

void test_crow_thread() {

    std::string s;
    std::mutex m;

    auto f = [&] (char c) {
        std::this_thread::sleep_for(50ms);
        auto lock = std::unique_lock(m);
        s += c;
    };

    {

        Thread t1(f, 'a');
        Thread t2(f, 'b');
        Thread t3(f, 'c');
        Thread t4(f, 'd');
        Thread t5(f, 'e');

    }

    TEST_EQUAL(s.size(), 5u);
    std::sort(s.begin(), s.end());
    TEST_EQUAL(s, "abcde");

}
