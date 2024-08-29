#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/iterator.hpp"
#include "crow/path.hpp"
#include "crow/resource.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <compare>
#include <cstdio>
#include <string>
#include <system_error>
#include <utility>

namespace Crow {

    // Supporting types

    class IoBase;

    CROW_ENUM_SCOPED(IoMode, int,
        read = 1,
        write,
        append,
        create_always,
        open_always,
        open_existing
    )

    class IoError:
    public std::system_error {
    public:
        explicit IoError(int err = 0, const std::error_category& cat = std::generic_category());
        explicit IoError(int err, const std::string& details, const std::error_category& cat = std::generic_category());
        explicit IoError(std::errc condition);
        explicit IoError(std::errc condition, const std::string& details);
    };

    class LineIterator:
    public InputIterator<LineIterator, const std::string> {
    public:
        LineIterator() = default;
        explicit LineIterator(IoBase& io): io_(&io), line_() { ++*this; }
        const std::string& operator*() const noexcept { return line_; }
        LineIterator& operator++();
        bool operator==(const LineIterator& rhs) const noexcept { return io_ == rhs.io_; }
    private:
        IoBase* io_ = nullptr;
        std::string line_;
    };

    // I/O abstract base class

    class IoBase {
    public:

        static constexpr size_t default_length = 65'536;

        virtual ~IoBase() noexcept {}

        virtual void close() = 0;
        virtual void flush() = 0;
        virtual int getc();
        virtual bool is_open() const = 0;
        virtual void putc(char c) { write(&c, 1); }
        virtual size_t read(void* ptr, size_t maxlen) = 0;
        virtual std::string read_line();
        virtual void seek(ptrdiff_t offset, int which = SEEK_CUR) = 0;
        virtual ptrdiff_t tell() = 0;
        virtual size_t write(const void* ptr, size_t len) = 0;

        Irange<LineIterator> lines() { return {LineIterator(*this), {}}; }
        std::string read_all();
        size_t read_some(std::string& buf, size_t maxlen = default_length);
        std::string reads(size_t maxlen = default_length);
        void write_chars(size_t n, char c);
        void write_line(const std::string& str = {});
        size_t writes(const std::string& str);

        template <typename... Args> void format(const std::string& pattern, Args&&... args) {
            writes(fmt(pattern, std::forward<Args>(args)...));
        }

        template <typename... Args> void print(Args&&... args) {
            write_line(prints(std::forward<Args>(args)...));
        }

    };

    // C standard I/O

    class Cstdio:
    public IoBase {

    public:

        using handle_type = FILE*;

        Cstdio() = default;
        explicit Cstdio(FILE* f) noexcept: fp_(f) {}
        explicit Cstdio(const Path& f, IoMode m = IoMode::read);
        Cstdio(const Path& f, const std::string& iomode);

        Cstdio(const Cstdio&) = delete;
        Cstdio(Cstdio&&) = default;
        Cstdio& operator=(const Cstdio&) = delete;
        Cstdio& operator=(Cstdio&&) = default;

        void close() override;
        void flush() override;
        int getc() override;
        bool is_open() const override { return bool(fp_); }
        void putc(char c) override;
        size_t read(void* ptr, size_t maxlen) override;
        std::string read_line() override;
        void seek(ptrdiff_t offset, int which = SEEK_CUR) override;
        ptrdiff_t tell() override;
        size_t write(const void* ptr, size_t len) override;

        void clearerr() noexcept;
        bool eof() const noexcept;
        bool error() const noexcept;
        int fd() const;
        FILE* get() const noexcept { return fp_.get(); }
        FILE* release() noexcept { return fp_.release(); }
        void ungetc(char c);

        static Cstdio null();
        static Cstdio std_input() { return Cstdio(stdin); }
        static Cstdio std_output() { return Cstdio(stdout); }
        static Cstdio std_error() { return Cstdio(stderr); }

    private:

        struct deleter {
            void operator()(FILE* fp) const noexcept;
        };

        Resource<FILE*, deleter> fp_;

    };

    // File descriptor I/O

    class Fdio:
    public IoBase {

    public:

        using handle_type = int;

        Fdio() = default;
        explicit Fdio(int f) noexcept: fd_(f) {}
        explicit Fdio(const Path& f, IoMode m = IoMode::read);
        Fdio(const Path& f, int iomode, int perm = 0666);

        Fdio(const Fdio&) = delete;
        Fdio(Fdio&&) = default;
        Fdio& operator=(const Fdio&) = delete;
        Fdio& operator=(Fdio&&) = default;

        void close() override;
        void flush() override;
        bool is_open() const override { return bool(fd_); }
        size_t read(void* ptr, size_t maxlen) override;
        void seek(ptrdiff_t offset, int which = SEEK_CUR) override;
        ptrdiff_t tell() override;
        size_t write(const void* ptr, size_t len) override;

        Fdio dup();
        Fdio dup(int f);
        int get() const noexcept { return fd_.get(); }
        int release() noexcept { return fd_.release(); }

        static Fdio null();
        static std::pair<Fdio, Fdio> pipe(size_t winmem = default_length);
        static Fdio std_input() { return Fdio(0); }
        static Fdio std_output() { return Fdio(1); }
        static Fdio std_error() { return Fdio(2); }

    private:

        struct deleter {
            void operator()(int fd) const noexcept;
        };

        Resource<int, deleter, -1> fd_;

        #ifdef _MSC_VER
            using iosize = unsigned;
            using ofsize = long;
        #else
            using iosize = size_t;
            using ofsize = ptrdiff_t;
        #endif

    };

    #ifdef _WIN32

        // Windows file I/O

        class Winio:
        public IoBase {

        public:

            using handle_type = void*;

            Winio() = default;
            explicit Winio(void* f) noexcept: fh_(f) {}
            explicit Winio(const Path& f, IoMode m = IoMode::read);
            Winio(const Path& f, uint32_t desired_access, uint32_t share_mode, void* security_attributes,
                uint32_t creation_disposition, uint32_t flags_and_attributes = 0, void* template_file = nullptr);

            Winio(const Winio&) = delete;
            Winio(Winio&&) = default;
            Winio& operator=(const Winio&) = delete;
            Winio& operator=(Winio&&) = default;

            void close() override;
            void flush() override;
            bool is_open() const override { return bool(fh_); }
            size_t read(void* ptr, size_t maxlen) override;
            void seek(ptrdiff_t offset, int which = SEEK_CUR) override;
            ptrdiff_t tell() override;
            size_t write(const void* ptr, size_t len) override;

            void* get() const noexcept { return fh_.get(); }
            void* release() noexcept { return fh_.release(); }

            static Winio null();
            static Winio std_input();
            static Winio std_output();
            static Winio std_error();

        private:

            struct deleter {
                void operator()(void* fh) const noexcept;
            };

            Resource<void*, deleter> fh_;

        };

    #endif

    // Temporary file

    class TempFile:
    public Cstdio {

    public:

        TempFile();
        TempFile(const Path& dir, const std::string& prefix);
        ~TempFile() noexcept override;

        TempFile(const TempFile&) = delete;
        TempFile(TempFile&&) = default;
        TempFile& operator=(const TempFile&) = delete;
        TempFile& operator=(TempFile&&) = default;

        Path where() const { return where_; }

    private:

        Path where_;

    };

}
