#include "crow/channel.hpp"
#include "crow/unit-test.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <thread>

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

void test_crow_channel_generator() {

    int i = 0, j = 0;
    GeneratorChannel<int> chan([&i] { return ++i; });
    std::optional<int> oi;

    TEST(chan.wait_for(10ms));
    TEST(chan.read(j));
    TEST_EQUAL(j, 1);

    TEST(chan.wait_for(10ms));
    TEST(chan.read(j));
    TEST_EQUAL(j, 2);

    TEST(chan.wait_for(10ms));
    TRY(oi = chan.read_maybe());
    TEST(oi.has_value());
    TEST_EQUAL(oi.value(), 3);

    TRY(chan.close());
    TEST(chan.wait_for(10ms));
    TEST(chan.is_closed());
    TEST(! chan.read(j));
    TRY(oi = chan.read_maybe());
    TEST(! oi.has_value());

}

void test_crow_channel_buffer() {

    BufferChannel chan;
    std::string s;
    size_t n = 0;

    TEST(! chan.wait_for(10ms));

    TEST(chan.write("Hello"));
    TEST(chan.write("World"));
    TEST(chan.wait_for(10ms));
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 10u);
    TEST_EQUAL(s, "HelloWorld");
    TEST(! chan.wait_for(10ms));

    s.clear();
    TEST(chan.write("Hello"));
    TEST(chan.wait_for(10ms));
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 5u);
    TEST_EQUAL(s, "Hello");
    TEST(! chan.wait_for(10ms));

    s.clear();
    TEST(chan.write("Hello"));
    TEST(chan.wait_for(10ms));
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 5u);
    TEST_EQUAL(s, "Hello");
    TEST(! chan.wait_for(10ms));
    TEST(chan.write("World"));
    TEST(chan.wait_for(10ms));
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 5u);
    TEST_EQUAL(s, "HelloWorld");
    TEST(! chan.wait_for(10ms));

    s.clear();
    TEST(! chan.wait_for(10ms));
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(s, "");

    s.clear();
    TRY(chan.close());
    TEST(chan.wait_for(10ms));
    TEST(chan.is_closed());
    TRY(n = chan.append(s));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(s, "");

}

void test_crow_channel_queue() {

    QueueChannel<std::string> chan;
    std::string s;

    TEST(! chan.wait_for(10ms));

    TRY(chan.write("Hello"));
    TEST(chan.wait_for(10ms));
    TEST(chan.read(s));
    TEST_EQUAL(s, "Hello");
    TEST(! chan.wait_for(10ms));

    TRY(chan.write("Hello"));
    TRY(chan.write("World"));
    TEST(chan.wait_for(10ms));
    TEST(chan.read(s));
    TEST_EQUAL(s, "Hello");
    TEST(chan.wait_for(10ms));
    TEST(chan.read(s));
    TEST_EQUAL(s, "World");
    TEST(! chan.wait_for(10ms));

    TRY(chan.close());
    TEST(chan.wait_for(10ms));
    TEST(chan.is_closed());

}

void test_crow_channel_value() {

    ValueChannel<std::string> chan;
    std::string s;

    TEST(! chan.wait_for(10ms));

    TRY(chan.write("Hello"));
    TEST(chan.wait_for(10ms));
    TEST(chan.read(s));
    TEST_EQUAL(s, "Hello");
    TEST(! chan.wait_for(10ms));

    TRY(chan.write("Hello"));
    TRY(chan.write("World"));
    TEST(chan.wait_for(10ms));
    TEST(chan.read(s));
    TEST_EQUAL(s, "World");
    TEST(! chan.wait_for(10ms));

    TRY(chan.write("World"));
    TEST(! chan.wait_for(10ms));

    TRY(chan.close());
    TEST(chan.wait_for(10ms));
    TEST(chan.is_closed());

}

void test_crow_channel_timer() {

    TimerChannel chan(25ms);

    TEST(! chan.wait_for(1ms));
    TEST(chan.wait_for(100ms));

    TRY(std::this_thread::sleep_for(150ms));
    TEST(chan.wait_for(1ms));
    TEST(chan.wait_for(1ms));

    TRY(chan.close());
    TEST(chan.wait_for(10ms));
    TEST(chan.is_closed());

    TimerChannel chan1(25ms, 1);

    TEST(! chan1.wait_for(1ms));
    TEST(chan1.wait_for(100ms));
    TEST(chan1.is_closed());

}
