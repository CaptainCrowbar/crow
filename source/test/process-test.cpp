#include "crow/process.hpp"
#include "crow/channel.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <string_view>

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

namespace {

    #ifdef _XOPEN_SOURCE
        const std::string list_command = "ls /";
        const std::string file1 = "bin";
        const std::string file2 = "usr";
    #else
        const std::string list_command = "dir /b \"c:\\\"";
        const std::string file1 = "Program Files";
        const std::string file2 = "Windows";
    #endif

    template <typename C, typename T>
    bool range_contains(const C& c, const T& t) {
        return std::find(c.begin(), c.end(), t) != c.end();
    }

}

void test_crow_process_stream() {

    std::unique_ptr<StreamProcess> chan;
    std::string s;
    std::vector<std::string_view> views;
    int st = -1;
    size_t n = 0;

    TRY(chan = std::make_unique<StreamProcess>(list_command));
    TEST(chan->wait_for(100ms));
    TEST(! chan->is_closed());
    TRY(n = chan->append(s));
    TEST(n > 0u);
    TEST_MATCH(s, "[\\r\\n A-Za-z0-9._-]+");

    TRY(views = splitv(s, "\r\n"));
    TEST(! views.empty());
    TEST(range_contains(views, file1));
    TEST(range_contains(views, file2));

    TEST(chan->wait_for(10ms));
    TEST(! chan->is_closed());
    TRY(n = chan->append(s));
    TEST_EQUAL(n, 0u);
    TEST(chan->wait_for(10ms));
    TEST(chan->is_closed());
    TRY(st = chan->status());
    TEST_EQUAL(st, 0);

    chan.reset();
    TRY(chan = std::make_unique<StreamProcess>(list_command));
    TRY(s = chan->read_all());
    TRY(views = splitv(s, "\r\n"));
    TEST(! views.empty());
    TEST(range_contains(views, file1));
    TEST(range_contains(views, file2));

}

void test_crow_process_text() {

    std::unique_ptr<TextProcess> chan;
    std::string s;
    std::vector<std::string> ss;
    std::vector<std::string_view> views;
    int st = -1;
    bool rc = false;

    TRY(chan = std::make_unique<TextProcess>(list_command));

    for (int i = 1; i <= 1000; ++i) {
        TRY(rc = bool(chan->wait_for(10ms)));
        if (chan->is_closed())
            break;
        if (rc) {
            TEST(chan->read(s));
            ss.push_back(s);
        }
    }

    TEST(! ss.empty());
    TEST(range_contains(ss, file1));
    TEST(range_contains(ss, file2));

    TRY(chan->close());
    TRY(st = chan->status());
    TEST_EQUAL(st, 0);

    chan.reset();
    TRY(chan = std::make_unique<TextProcess>(list_command));
    TRY(s = chan->read_all());
    TRY(views = splitv(s, "\r\n"));
    TEST(! views.empty());
    TEST(range_contains(views, file1));
    TEST(range_contains(views, file2));

}

void test_crow_process_shell_command() {

    std::string s;
    std::vector<std::string_view> views;

    TRY(s = shell(list_command));
    TEST(! s.empty());
    TRY(views = splitv(s, "\r\n"));
    TEST(! views.empty());
    TEST(range_contains(views, file1));
    TEST(range_contains(views, file2));

}
