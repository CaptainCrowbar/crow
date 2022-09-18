#include "crow/process.hpp"
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <system_error>

#ifdef _XOPEN_SOURCE
    #include <sys/select.h>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #define IO_FUNCTION(f) ::f
#else
    #include <windows.h>
    #include <io.h>
    #include <stringapiset.h>
    #define IO_FUNCTION(f) _##f
#endif

using namespace std::chrono;

namespace Crow {

    namespace {

        #ifndef _XOPEN_SOURCE

            std::wstring to_wstring(const std::string& utf8) {
                int rc = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), int(utf8.size()), nullptr, 0);
                auto err = GetLastError();
                if (rc == 0)
                    throw std::system_error(err, std::system_category(), utf8);
                std::wstring wstr(rc, 0);
                MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), int(utf8.size()), wstr.data(), int(wstr.size()));
                return wstr;
            }

        #endif

    }

    // Class StreamProcess

    StreamProcess::StreamProcess(const std::string& cmd) {
        #ifdef _XOPEN_SOURCE
            errno = 0;
            fp_ = ::popen(cmd.data(), "r");
        #else
            auto wcmd = to_wstring(cmd);
            errno = 0;
            fp_ = ::_wpopen(wcmd.data(), L"rb");
        #endif
        int err = errno;
        if (! fp_)
            throw std::system_error(err, std::generic_category());
    }

    size_t StreamProcess::read(void* dst, size_t maxlen) {
        if (! dst || ! maxlen || ! fp_)
            return 0;
        errno = 0;
        size_t rc = fread(dst, 1, maxlen, fp_);
        int err = errno;
        if (rc > 0)
            return rc;
        if (err != 0 && err != EBADF)
            throw std::system_error(err, std::generic_category());
        do_close();
        return 0;
    }

    bool StreamProcess::do_wait_for(duration t) {
        using namespace std::chrono;
        if (! fp_)
            return true;
        if (t < duration())
            t = {};
        int fd = IO_FUNCTION(fileno)(fp_);
        #ifdef _XOPEN_SOURCE
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            timeval tv;
            duration_to_timeval(t, tv);
            errno = 0;
            int rc = ::select(fd + 1, &fds, nullptr, nullptr, &tv);
            int err = errno;
            if (rc == -1 && err == EBADF)
                return true;
            else if (rc == -1)
                throw std::system_error(err, std::generic_category());
            else
                return rc > 0;
        #else
            auto fh = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
            auto ms = duration_cast<milliseconds>(t).count();
            SetLastError(0);
            auto rc = WaitForSingleObject(fh, uint32_t(ms));
            auto err = GetLastError();
            if (rc == WAIT_OBJECT_0)
                return true;
            else if (rc == WAIT_TIMEOUT)
                return false;
            else if (err == ERROR_INVALID_HANDLE)
                throw std::system_error(err, std::system_category());
            else
                return true;
        #endif
    }

    void StreamProcess::do_close() noexcept {
        if (! fp_)
            return;
        int rc = IO_FUNCTION(pclose)(fp_);
        if (status_ == -1)
            status_ = rc;
        fp_ = nullptr;
    }

    // Class TextProcess

    void TextProcess::close() noexcept {
        stream_.close();
        buf_.clear();
    }

    bool TextProcess::read(std::string& t) {
        if (buf_.empty())
            return false;
        size_t lf = buf_.find('\n');
        if (lf == std::string::npos) {
            t = move(buf_);
            buf_.clear();
        } else {
            if (lf > 0 && buf_[lf - 1] == '\r')
                t.assign(buf_, 0, lf - 1);
            else
                t.assign(buf_, 0, lf);
            buf_.erase(0, lf + 1);
        }
        return true;
    }

    bool TextProcess::do_wait_for(duration t) {
        using namespace std::chrono;
        t = std::max(t, duration());
        auto deadline = system_clock::now() + t;
        duration delta = {};
        for (;;) {
            stream_.wait_for(delta);
            if (stream_.is_closed() || ! stream_.append(buf_))
                return true;
            size_t lf = buf_.find('\n');
            if (lf != std::string::npos)
                return true;
            auto now = system_clock::now();
            if (now > deadline)
                return false;
            delta = duration_cast<duration>(deadline - now);
        }
    }

    // Shell commands

    std::string shell(const std::string& cmd) {
        StreamProcess proc(cmd);
        return proc.read_all();
    }

}
