#include "crow/named-mutex.hpp"
#include "crow/time.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <cerrno>
#include <ctime>
#include <system_error>
#include <thread>

#ifdef _XOPEN_SOURCE
    #include <fcntl.h>
    #include <semaphore.h>
#else
    #include <windows.h>
#endif

using namespace std::chrono;
using namespace std::literals;

namespace Crow {

    namespace {

        std::string lower_case(std::string str) {
            for (char& c: str)
                if (c >= 'A' && c <= 'Z')
                    c += 0x20;
            return str;
        }

        std::string strip_path_prefix(std::string name) {
            if (name[0] == '/')
                name.erase(0, 1);
            else if (lower_case(name.substr(0, 5)) == "local" && (name[5] == '/' || name[5] == '\\'))
                name.erase(0, 6);
            return name;
        }

    }

    #ifdef _XOPEN_SOURCE

        using SemPtr = sem_t*;

        NamedMutex::NamedMutex(const std::string& name) {
            path_ = "/" + strip_path_prefix(name);
            errno = 0;
            handle_ = sem_open(path_.data(), O_CREAT, 0666, 1);
            int err = errno;
            if (SemPtr(handle_) == SEM_FAILED)
                throw std::system_error(err, std::system_category(), path_);
        }

        NamedMutex::~NamedMutex() noexcept {
            sem_close(SemPtr(handle_));
        }

        std::string NamedMutex::name() const {
            return path_.substr(1);
        }

        void NamedMutex::lock() {
            errno = 0;
            int rc = sem_wait(SemPtr(handle_));
            int err = errno;
            if (rc == -1)
                throw std::system_error(err, std::system_category(), path_);
        }

        bool NamedMutex::try_lock() {
            errno = 0;
            int rc = sem_wait(SemPtr(handle_));
            int err = errno;
            if (rc == 0)
                return true;
            else if (rc == EAGAIN)
                return false;
            else
                throw std::system_error(err, std::system_category(), path_);
        }

        #ifdef __APPLE__

            bool NamedMutex::try_lock_until(system_clock::time_point abs_time) {
                static const auto max_delta = 100ms;
                auto delta = 1ms;
                for (;;) {
                    if (try_lock())
                        return true;
                    auto now = system_clock::now();
                    if (now >= abs_time)
                        return false;
                    std::this_thread::sleep_for(delta);
                    delta = std::min({2 * delta, max_delta, duration_cast<milliseconds>(abs_time - now)});
                }
            }

        #else

            bool NamedMutex::try_lock_until(system_clock::time_point abs_time) {
                std::timespec ts;
                duration_to_timespec(abs_time.time_since_epoch(), ts);
                errno = 0;
                int rc = sem_timedwait(SemPtr(handle_), &ts);
                int err = errno;
                if (rc == 0)
                    return true;
                else if (rc == ETIMEDOUT)
                    return false;
                else
                    throw std::system_error(err, std::system_category(), path_);
            }

        #endif

        void NamedMutex::unlock() noexcept {
            sem_post(SemPtr(handle_));
        }

    #else

        namespace {

            std::wstring to_wstring(const std::string& utf8) {
                int rc = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), int(utf8.size()), nullptr, 0);
                auto err = GetLastError();
                if (rc == 0)
                    throw std::system_error(err, std::system_category(), utf8);
                std::wstring wstr(rc, 0);
                MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), int(utf8.size()), wstr.data(), int(wstr.size()));
                return wstr;
            }

        }

        NamedMutex::NamedMutex(const std::string& name) {
            path_ = "Local\\" + strip_path_prefix(name);
            auto wpath = to_wstring(path_);
            handle_ = CreateMutexW(nullptr, false, wpath.data());
            auto err = GetLastError();
            if (! handle_)
                throw std::system_error(err, std::system_category(), path_);
        }

        NamedMutex::~NamedMutex() noexcept {
            CloseHandle(handle_);
        }

        std::string NamedMutex::name() const {
            return to_utf8(decode_string(path_.substr(6)));
        }

        void NamedMutex::lock() {
            auto rc = WaitForSingleObject(handle_, INFINITE);
            auto err = GetLastError();
            if (rc == WAIT_FAILED)
                throw std::system_error(err, std::system_category(), path_);
        }

        bool NamedMutex::try_lock() {
            auto rc = WaitForSingleObject(handle_, 0);
            auto err = GetLastError();
            if (rc == WAIT_FAILED)
                throw std::system_error(err, std::system_category(), path_);
            return rc != WAIT_TIMEOUT;
        }

        bool NamedMutex::try_lock_until(system_clock::time_point abs_time) {
            auto msec = duration_cast<milliseconds>(abs_time - system_clock::now()).count();
            uint32_t msec32 = msec > 0 ? uint32_t(msec) : 0;
            auto rc = WaitForSingleObject(handle_, msec32);
            auto err = GetLastError();
            if (rc == WAIT_FAILED)
                throw std::system_error(err, std::system_category(), path_);
            return rc != WAIT_TIMEOUT;
        }

        void NamedMutex::unlock() noexcept {
            ReleaseMutex(handle_);
        }

    #endif

}
