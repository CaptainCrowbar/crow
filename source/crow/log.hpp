#pragma once

#include <atomic>
#include <chrono>
#include <cstdio>
#include <functional>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace Crow {

    namespace Detail {

        template <typename T, typename = void> struct HasStrMethod: std::false_type {};
        template <typename T> struct HasStrMethod<T,
            std::void_t<decltype(std::declval<std::string&>() = std::declval<const T&>().str())>>: std::true_type {};

        template <typename T, typename = void> struct HasOutputOperator: std::false_type {};
        template <typename T> struct HasOutputOperator<T,
            std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>: std::true_type {};

        template <typename T>
        std::string log_format(const T& t) {
            if constexpr (std::is_same_v<T, bool>) {
                return t ? " true" : " false";
            } else if constexpr (std::is_arithmetic_v<T>) {
                return ' ' + std::to_string(t);
            } else if constexpr (HasStrMethod<T>::value) {
                return ' ' + t.str();
            } else {
                std::ostringstream out;
                out << ' ' << t;
                return out.str();
            }
        }

        inline std::atomic<FILE*>& log_stream() noexcept {
            static std::atomic<FILE*> stream(stderr);
            return stream;
        }

        inline bool is_tty(FILE* fp) noexcept {
            #ifdef _WIN32
                int fd = _fileno(fp);
                return _isatty(fd) != 0;
            #else
                int fd = fileno(fp);
                return isatty(fd) != 0;
            #endif
        }

        inline std::string timestamp() {

            using namespace std::chrono;

            auto now = system_clock::now();
            auto t = system_clock::to_time_t(now);
            std::tm tm = *std::gmtime(&t);
            std::string text(30, '\0');
            auto rc = std::strftime(&text[0], text.size(), "%Y-%m-%d %H:%M:%S", &tm);
            text.resize(rc);

            double sec = duration_cast<duration<double>>(now.time_since_epoch()).count();
            double isec = 0;
            double fsec = std::modf(sec, &isec);
            std::string buf(9, '\0');
            std::snprintf(&buf[0], buf.size(), "%.*f", 6, fsec);
            text += buf.data() + 1;

            return text;

        }

        template <typename... Args>
        void log_helper(const char* file, const char* func, int line, const Args&... args) {

            using namespace std::chrono;

            static constexpr const char* delimiters =
                #ifdef _XOPEN_SOURCE
                    "/";
                #else
                    "/\\";
                #endif

            if (! log_stream())
                return;

            bool tty = is_tty(log_stream());
            std::string message;

            if (tty) {

                static thread_local std::string prefix = [] {
                    auto id = std::this_thread::get_id();
                    auto seed = uint32_t(std::hash<std::thread::id>()(id));
                    std::minstd_rand rng(seed);
                    std::uniform_int_distribution<int> rgb_dist(1, 4);
                    int r = rgb_dist(rng);
                    int g = rgb_dist(rng);
                    int b = rgb_dist(rng);
                    int index = 36 * r + 6 * g + b + 16;
                    return "\x1b[38;5;" + std::to_string(index) + 'm';
                }();

                message += prefix;
            }

            message += '[' + timestamp();

            if (file != nullptr) {
                std::string name = file;
                size_t cut = name.find_last_of(delimiters);
                if (cut != std::string::npos)
                    name.erase(0, cut + 1);
                message += ' ' + name + ' ' + func + "() " + std::to_string(line);
            }

            message += ']' + (log_format(args) + ...);
            if (tty)
                message += "\x1b[0m";
            message += '\n';
            std::fwrite(message.data(), 1, message.size(), log_stream());

        }

    }

    class SetLog {
    public:
        SetLog() noexcept: SetLog(nullptr) {}
        explicit SetLog(FILE* stream) noexcept: saved_(Detail::log_stream().exchange(stream)) {}
        ~SetLog() noexcept { Detail::log_stream() = saved_; }
        SetLog(const SetLog&) = delete;
        SetLog(SetLog&&) = delete;
        SetLog& operator=(const SetLog&) = delete;
        SetLog& operator=(SetLog&&) = delete;
    private:
        FILE* saved_;
    };

    inline void set_log(FILE* stream = nullptr) noexcept {
        Detail::log_stream() = stream;
    }

    template <typename... Args>
    void logx(const Args&... args) {
        Detail::log_helper(nullptr, nullptr, -1, args...);
    }

}

#define CROW_LOG(...) ::Crow::Detail::log_helper(__FILE__, __func__, __LINE__, __VA_ARGS__)
