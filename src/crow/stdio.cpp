#include "crow/stdio.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <cerrno>
#include <fcntl.h>
#include <random>

#ifdef _XOPEN_SOURCE

    #include <sys/stat.h>
    #include <unistd.h>

    #ifdef __APPLE__
        #include <sys/syslimits.h>
    #endif

    #define IO_FUNCTION(f) ::f

#else

    #include <io.h>
    #include <windows.h>

    #define IO_FUNCTION(f) _##f

#endif

namespace Crow {

    namespace {

        constexpr size_t block_size = 65'536;
        constexpr size_t small_block = 256;

        constexpr const char* null_device =
            #ifdef _XOPEN_SOURCE
                "/dev/null";
            #else
                "NUL:";
            #endif

        void check_for_error(int err, const std::error_category& cat = std::generic_category()) {
            if (err != 0)
                throw IoError(err, cat);
        }

    }

    // Class IoError

    IoError::IoError(int err, const std::error_category& cat):
    std::system_error(err == 0 ? std::make_error_code(std::errc::io_error) : std::error_code(err, cat)) {}

    IoError::IoError(int err, const std::string& details, const std::error_category& cat):
    std::system_error(err == 0 ? std::make_error_code(std::errc::io_error) : std::error_code(err, cat), details) {}

    IoError::IoError(std::errc condition):
    std::system_error(std::make_error_code(condition)) {}

    IoError::IoError(std::errc condition, const std::string& details):
    std::system_error(std::make_error_code(condition), details) {}

    // Class LineIterator

    LineIterator& LineIterator::operator++() {
        if (io_) {
            line_ = io_->read_line();
            if (line_.empty())
                io_ = nullptr;
        }
        return *this;
    }

    // Class IoBase

    int IoBase::getc() {
        unsigned char b = 0;
        return read(&b, 1) ? int(b) : EOF;
    }

    std::string IoBase::read_line() {
        std::string s;
        for (;;) {
            int c = getc();
            if (c == EOF)
                break;
            s += char(c);
            if (c == '\n')
                break;
        }
        return s;
    }

    std::string IoBase::read_all() {
        std::string buf;
        while (read_some(buf, block_size)) {}
        return buf;
    }

    size_t IoBase::read_some(std::string& buf, size_t maxlen) {
        size_t offset = buf.size();
        buf.resize(offset + maxlen);
        size_t n = read(&buf[0] + offset, maxlen);
        buf.resize(offset + n);
        return n;
    }

    std::string IoBase::reads(size_t maxlen) {
        std::string s(maxlen, 0);
        s.resize(read(&s[0], maxlen));
        return s;
    }

    void IoBase::write_chars(size_t n, char c) {
        std::string buf(std::min(n, block_size), c);
        size_t quo = n / block_size;
        size_t rem = n % block_size;
        for (size_t i = 0; i < quo; ++i)
            write(buf.data(), block_size);
        if (rem != 0)
            write(buf.data(), rem);
    }

    void IoBase::write_line(const std::string& str) {
        if (str.empty())
            putc('\n');
        else if (str.back() == '\n')
            writes(str);
        else
            writes(std::string(str) + '\n');
    }

    size_t IoBase::writes(const std::string& str) {
        static constexpr char dummy = '\0';
        const char* ptr = str.empty() ? &dummy : str.data();
        size_t ofs = 0, len = str.size();
        do ofs += write(ptr + ofs, len - ofs);
            while (ofs < len);
        return ofs;
    }

    // Class Cstdio

    void Cstdio::deleter::operator()(FILE* fp) const noexcept {
        if (fp != stdin && fp != stdout && fp != stderr)
            std::fclose(fp);
    }

    Cstdio::Cstdio(const Path& f, IoMode m) {

        if (f.is_empty() || f.name() == "-") {

            if (m == IoMode::read)
                *this = std_input();
            else if (m == IoMode::write || m == IoMode::append)
                *this = std_output();

        } else if (m == IoMode::open_always) {

            // There is no C stdio mode corresponding directly to the
            // behaviour we want from IoMode::open_always. Try "rb+" first
            // (open for R/W if the file exists, otherwise fail), and if that
            // fails, try "wb+" (open for R/W regardless, destroying any
            // existing file). There's a race condition here but I don't
            // think there's any way around it.

            std::string iomode = "rb+";
            *this = Cstdio(f, "rb+");
            if (! fp_)
                *this = Cstdio(f, "wb+");

        } else {

            std::string cmode;

            switch (m) {
                case IoMode::read:           cmode = "rb"; break;
                case IoMode::write:          cmode = "wb"; break;
                case IoMode::append:         cmode = "ab"; break;
                case IoMode::create_always:  cmode = "wb+"; break;
                case IoMode::open_existing:  cmode = "rb+"; break;
                default:                     break;
            }

            *this = Cstdio(f, cmode);

        }

    }

    Cstdio::Cstdio(const Path& f, const std::string& iomode) {

        int err;

        #ifdef _XOPEN_SOURCE

            errno = 0;
            auto rc = ::fopen(f.c_name(), iomode.data());
            err = errno;

        #else

            auto wmode = to_wstring(decode_string(iomode));
            errno = 0;
            auto rc = _wfopen(f.c_name(), wmode.data());
            err = errno;

        #endif

        if (err != 0)
            throw IoError(err, f.name());

        *this = Cstdio(rc);

    }

    void Cstdio::close() {
        errno = 0;
        fp_.reset();
        check_for_error(errno);
    }

    void Cstdio::flush() {
        errno = 0;
        ::fflush(fp_.get());
        check_for_error(errno);
    }

    int Cstdio::getc() {
        errno = 0;
        int c = ::fgetc(fp_.get());
        check_for_error(errno);
        return c;
    }

    void Cstdio::putc(char c) {
        errno = 0;
        ::fputc(int(uint8_t(c)), fp_.get());
        check_for_error(errno);
    }

    size_t Cstdio::read(void* ptr, size_t maxlen) {
        errno = 0;
        size_t n = ::fread(ptr, 1, maxlen, fp_.get());
        check_for_error(errno);
        return n;
    }

    std::string Cstdio::read_line() {
        std::string buf;
        for (;;) {
            size_t offset = buf.size();
            buf.resize(offset + small_block, '\0');
            errno = 0;
            auto rc = ::fgets(&buf[0] + offset, small_block, fp_.get());
            check_for_error(errno);
            if (rc == nullptr)
                return buf.substr(0, offset);
            size_t lfpos = buf.find('\n', offset);
            if (lfpos != npos)
                return buf.substr(0, lfpos + 1);
            size_t ntpos = buf.find_last_not_of('\0') + 1;
            if (ntpos < small_block - 1)
                return buf.substr(0, ntpos);
            buf.pop_back();
        }
    }

    void Cstdio::seek(ptrdiff_t offset, int which) {
        errno = 0;
        #ifdef _XOPEN_SOURCE
            ::fseeko(fp_.get(), offset, which);
        #else
            _fseeki64(fp_.get(), offset, which);
        #endif
        check_for_error(errno);
    }

    ptrdiff_t Cstdio::tell() {
        errno = 0;
        #ifdef _XOPEN_SOURCE
            ptrdiff_t offset = ::ftello(fp_.get());
        #else
            ptrdiff_t offset = _ftelli64(fp_.get());
        #endif
        check_for_error(errno);
        return offset;
    }

    size_t Cstdio::write(const void* ptr, size_t len) {
        errno = 0;
        size_t n = ::fwrite(ptr, 1, len, fp_.get());
        check_for_error(errno);
        return n;
    }

    void Cstdio::clearerr() noexcept {
        std::clearerr(fp_.get());
    }

    bool Cstdio::eof() const noexcept {
        return fp_ && std::feof(fp_.get()) != 0;
    }

    bool Cstdio::error() const noexcept {
        return fp_ && std::ferror(fp_.get()) != 0;
    }

    int Cstdio::fd() const {
        return fp_ ? IO_FUNCTION(fileno)(fp_.get()) : -1;
    }

    bool Cstdio::is_ready() const noexcept {
        return fp_ && ! error() && ! eof();
    }

    void Cstdio::ungetc(char c) {
        errno = 0;
        ::ungetc(int(uint8_t(c)), fp_.get());
        check_for_error(errno);
    }

    Cstdio Cstdio::null() {
        Cstdio io(null_device, "r+");
        ::fgetc(io.fp_.get()); // Clear bogus ioctl error
        return io;
    }

    // Class Fdio

    void Fdio::deleter::operator()(int fd) const noexcept {
        if (fd > 2)
            IO_FUNCTION(close)(fd);
    }

    Fdio::Fdio(const Path& f, IoMode m) {

        if (f.is_empty() || f.name() == "-") {

            if (m == IoMode::read) {
                *this = std_input();
                return;
            } else if (m == IoMode::write || m == IoMode::append) {
                *this = std_output();
                return;
            }

        }

        int fmode = 0;

        switch (m) {
            case IoMode::read:           fmode = O_RDONLY; break;
            case IoMode::write:          fmode = O_WRONLY|O_CREAT|O_TRUNC; break;
            case IoMode::append:         fmode = O_WRONLY|O_APPEND|O_CREAT; break;
            case IoMode::create_always:  fmode = O_RDWR|O_CREAT|O_TRUNC; break;
            case IoMode::open_always:    fmode = O_RDWR|O_CREAT; break;
            case IoMode::open_existing:  fmode = O_RDWR; break;
            default:                     break;
        }

        *this = Fdio(f, fmode, 0666);

    }

    Fdio::Fdio(const Path& f, int iomode, int perm) {

        int err;

        #ifdef _XOPEN_SOURCE

            errno = 0;
            auto rc = ::open(f.c_name(), iomode, perm);
            err = errno;

        #else

            if (! (iomode & (_O_BINARY | _O_TEXT | _O_U8TEXT | _O_U16TEXT | _O_WTEXT)))
                iomode |= _O_BINARY;
            errno = 0;
            auto rc = _wopen(f.c_name(), iomode, perm);
            err = errno;

        #endif

        if (err != 0)
            throw IoError(err, f.name());

        *this = Fdio(rc);

    }

    void Fdio::close() {
        errno = 0;
        fd_.reset();
        check_for_error(errno);
    }

    void Fdio::flush() {
        #ifdef _XOPEN_SOURCE
            errno = 0;
            ::fsync(fd_.get());
            check_for_error(errno);
        #else
            auto h = reinterpret_cast<HANDLE>(_get_osfhandle(fd_.get()));
            if (h == INVALID_HANDLE_VALUE)
                check_for_error(EBADF);
            else if (! FlushFileBuffers(h))
                check_for_error(GetLastError(), std::system_category());
        #endif
    }

    size_t Fdio::read(void* ptr, size_t maxlen) {
        errno = 0;
        auto rc = IO_FUNCTION(read)(fd_.get(), ptr, iosize(maxlen));
        check_for_error(errno);
        return rc;
    }

    void Fdio::seek(ptrdiff_t offset, int which) {
        errno = 0;
        IO_FUNCTION(lseek)(fd_.get(), ofsize(offset), which);
        check_for_error(errno);
    }

    ptrdiff_t Fdio::tell() {
        errno = 0;
        auto offset = IO_FUNCTION(lseek)(fd_.get(), 0, SEEK_CUR);
        check_for_error(errno);
        return offset;
    }

    size_t Fdio::write(const void* ptr, size_t len) {
        errno = 0;
        size_t n = IO_FUNCTION(write)(fd_.get(), ptr, iosize(len));
        check_for_error(errno);
        return n;
    }

    Fdio Fdio::dup() {
        errno = 0;
        int rc = IO_FUNCTION(dup)(fd_.get());
        check_for_error(errno);
        return Fdio(rc);
    }

    Fdio Fdio::dup(int f) {
        errno = 0;
        IO_FUNCTION(dup2)(fd_.get(), f);
        check_for_error(errno);
        return Fdio(f);
    }

    Fdio Fdio::null() {
        int iomode = O_RDWR;
        #ifdef O_CLOEXEC
            iomode |= O_CLOEXEC;
        #endif
        return Fdio(null_device, iomode);
    }

    std::pair<Fdio, Fdio> Fdio::pipe(size_t winmem) {
        int fds[2];
        int rc;
        errno = 0;
        #ifdef _XOPEN_SOURCE
            (void)winmem;
            rc = ::pipe(fds);
        #else
            rc = _pipe(fds, iosize(winmem), O_BINARY);
        #endif
        if (rc != 0)
            check_for_error(errno);
        std::pair<Fdio, Fdio> pair;
        pair.first = Fdio(fds[0]);
        pair.second = Fdio(fds[1]);
        return pair;
    }

    #ifdef _WIN32

        // Class Winio

        void Winio::deleter::operator()(void* fh) const noexcept {
            static const auto stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
            static const auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            static const auto stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
            if (fh != INVALID_HANDLE_VALUE && fh != stdin_handle && fh != stdout_handle && fh != stderr_handle)
                CloseHandle(fh);
        }

        Winio::Winio(const Path& f, IoMode m) {

            if (f.empty() || f.name() == "-") {
                if (m == IoMode::read) {
                    *this = std_input();
                    return;
                } else if (m == IoMode::write || m == IoMode::append) {
                    *this = std_output();
                    return;
                }
            }

            uint32_t access = 0, creation = 0;

            switch (m) {
                case IoMode::read:           access = GENERIC_READ; creation = OPEN_EXISTING; break;
                case IoMode::write:          access = GENERIC_WRITE; creation = CREATE_ALWAYS; break;
                case IoMode::append:         access = GENERIC_WRITE; creation = OPEN_ALWAYS; break;
                case IoMode::create_always:  access = GENERIC_READ | GENERIC_WRITE; creation = CREATE_ALWAYS; break;
                case IoMode::open_always:    access = GENERIC_READ | GENERIC_WRITE; creation = OPEN_ALWAYS; break;
                case IoMode::open_existing:  access = GENERIC_READ | GENERIC_WRITE; creation = OPEN_EXISTING; break;
                default:                     break;
            }

            *this = Winio(f, access, 0, nullptr, creation);

            if (m == IoMode::append) {
                LARGE_INTEGER distance;
                distance.QuadPart = 0;
                auto rc = SetFilePointerEx(fh_.get(), distance, nullptr, FILE_END);
                auto err = GetLastError();
                if (rc == 0)
                    throw IoError(err, f.name(), std::system_category());
            }

        }

        Winio::Winio(const Path& f, uint32_t desired_access, uint32_t share_mode, void* security_attributes,
                uint32_t creation_disposition, uint32_t flags_and_attributes, void* template_file) {
            auto sa = LPSECURITY_ATTRIBUTES(security_attributes);
            auto tf = HANDLE(template_file);
            auto rc = CreateFileW(f.c_name(), desired_access, share_mode, sa,
                creation_disposition, flags_and_attributes, tf);
            int err = GetLastError();
            if (rc == INVALID_HANDLE_VALUE)
                throw IoError(err, f.name(), std::system_category());
            *this = Winio(rc);
        }

        void Winio::close() {
            SetLastError(0);
            fh_.reset();
            check_for_error(GetLastError(), std::system_category());
        }

        void Winio::flush() {
            if (! FlushFileBuffers(fh_.get()))
                check_for_error(GetLastError(), std::system_category());
        }

        size_t Winio::read(void* ptr, size_t maxlen) {
            DWORD n = 0;
            SetLastError(0);
            ReadFile(fh_.get(), ptr, uint32_t(maxlen), &n, nullptr);
            check_for_error(GetLastError(), std::system_category());
            return n;
        }

        void Winio::seek(ptrdiff_t offset, int which) {
            LARGE_INTEGER distance;
            distance.QuadPart = offset;
            DWORD method = 0;
            switch (which) {
                case SEEK_SET:  method = FILE_BEGIN; break;
                case SEEK_CUR:  method = FILE_CURRENT; break;
                case SEEK_END:  method = FILE_END; break;
                default:        break;
            }
            SetLastError(0);
            SetFilePointerEx(fh_.get(), distance, nullptr, method);
            check_for_error(GetLastError(), std::system_category());
        }

        ptrdiff_t Winio::tell() {
            LARGE_INTEGER distance, result;
            distance.QuadPart = result.QuadPart = 0;
            SetLastError(0);
            SetFilePointerEx(fh_.get(), distance, &result, FILE_CURRENT);
            check_for_error(GetLastError(), std::system_category());
            return result.QuadPart;
        }

        size_t Winio::write(const void* ptr, size_t len) {
            DWORD n = 0;
            SetLastError(0);
            WriteFile(fh_.get(), ptr, uint32_t(len), &n, nullptr);
            check_for_error(GetLastError(), std::system_category());
            return n;
        }

        Winio Winio::null() {
            return Winio(null_device, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING);
        }

        Winio Winio::std_input() {
            return Winio(GetStdHandle(STD_INPUT_HANDLE));
        }

        Winio Winio::std_output() {
            return Winio(GetStdHandle(STD_OUTPUT_HANDLE));
        }

        Winio Winio::std_error() {
            return Winio(GetStdHandle(STD_ERROR_HANDLE));
        }

    #endif

    // Class TempFile

    TempFile::TempFile() {
        errno = 0;
        auto file = std::tmpfile();
        int err = errno;
        if (! file)
            throw IoError(err);
        Cstdio io(file);
        Cstdio::operator=(std::move(io));
    }

    TempFile::TempFile(const Path& dir, const std::string& prefix) {

        static constexpr int max_tries = 100;

        if (! dir.is_empty() && ! dir.is_directory())
            throw IoError(std::errc::no_such_file_or_directory);

        std::random_device dev;
        Fdio fdio;
        int err = 0;

        for (int i = 0; i < max_tries && ! fdio.is_open(); ++i) {
            uint64_t x = dev();
            uint64_t y = dev();
            uint64_t z = (x << 32) + y;
            where_ = dir / (std::string(prefix) + hex(z));
            where_ = where_.resolve();
            errno = 0;
            fdio = Fdio(where_, O_RDWR | O_CREAT | O_EXCL);
            err = errno;
        }

        if (! fdio.is_open())
            throw IoError(err);

        FILE* file;

        #if defined(_XOPEN_SOURCE)
            file = fdopen(fdio.release(), "rb+");
        #else
            file = _wfdopen(fdio.release(), L"rb+");
        #endif

        Cstdio io(file);
        Cstdio::operator=(std::move(io));

    }

    TempFile::~TempFile() noexcept {
        if (! where_.is_empty()) {
            try { close(); } catch (...) {}
            try { where_.remove(); } catch (...) {}
        }
    }

}
