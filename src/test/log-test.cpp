#include "crow/log.hpp"
#include "crow/unit-test.hpp"
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>

using namespace Crow;
using namespace std::literals;

namespace {

    const std::string match_timestamp = R"(\d{4}-\d\d-\d\d \d\d:\d\d:\d\d\.\d{6})";
    const std::string match_context = R"([^ :\[\]]+ \w+\(\) \d+)";
    const std::string short_prefix = "^\\[" + match_timestamp + "\\]";
    const std::string long_prefix = "^\\[" + match_timestamp + " " + match_context + "\\]";

    class LogFile {
    public:
        explicit LogFile(const std::string& name);
        ~LogFile() noexcept;
        LogFile(const LogFile&) = delete;
        LogFile(LogFile&&) = delete;
        LogFile& operator=(const LogFile&) = delete;
        LogFile& operator=(LogFile&&) = delete;
        operator FILE*() const noexcept { return stream_; }
        std::vector<std::string> content();
        void reset();
    private:
        std::string name_;
        FILE* stream_ = nullptr;
        void open(const char* mode);
        void close() noexcept;
    };

        LogFile::LogFile(const std::string& name):
        name_(name), stream_() {
            open("wb");
        }

        LogFile::~LogFile() noexcept {
            close();
            std::remove(name_.data());
        }

        std::vector<std::string> LogFile::content() {
            static constexpr int bufsize = 1024;
            open("rb");
            std::vector<std::string> lines;
            std::string buf(bufsize, '\0');
            for (;;) {
                auto lineptr = std::fgets(buf.data(), bufsize, stream_);
                if (lineptr == nullptr)
                    break;
                lines.emplace_back(lineptr);
                if (lines.back().back() == '\n')
                    lines.back().pop_back();
            }
            open("ab");
            return lines;
        }

        void LogFile::reset() {
            open("wb");
        }

        void LogFile::open(const char* mode) {
            close();
            stream_ = std::fopen(name_.data(), mode);
            if (stream_ == nullptr)
                throw std::runtime_error("Failed to open log file: " + name_);
        }

        void LogFile::close() noexcept {
            if (stream_ != nullptr) {
                std::fclose(stream_);
                stream_ = nullptr;
            }
        }

}

void test_crow_logging() {

    LogFile log("_log.txt");
    SetLog guard(log);
    std::vector<std::string> lines;

    TRY(logx("Hello", "world"));
    TRY(logx(123, 456, 789));

    TRY(lines = log.content());
    TEST_EQUAL(lines.size(), 2u);
    lines.resize(2);
    TEST_MATCH(lines[0], short_prefix + " Hello world$");
    TEST_MATCH(lines[1], short_prefix + " 123 456 789$");

    TRY(log.reset());
    TRY(CROW_LOG("Hello", "world"));
    TRY(CROW_LOG(123, 456, 789));

    TRY(lines = log.content());
    TEST_EQUAL(lines.size(), 2u);
    lines.resize(2);
    TEST_MATCH(lines[0], long_prefix + " Hello world$");
    TEST_MATCH(lines[1], long_prefix + " 123 456 789$");

}

void test_crow_logging_output() {

    static constexpr int n_threads = 5;

    std::vector<std::thread> threads;

    CROW_LOG("Main thread");

    for (int i = 1; i <= n_threads; ++i) {
        threads.emplace_back([i] {
            CROW_LOG("Begin thread " + std::to_string(i));
            std::this_thread::sleep_for(100ms);
            CROW_LOG("End thread " + std::to_string(i));
        });
    }

    CROW_LOG("Main thread");

    for (auto& t: threads)
        t.join();

}
