#pragma once

#include "crow/enum.hpp"
#include "crow/io-utility.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/unicode.hpp"
#include <chrono>
#include <functional>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace Crow {

    class Path:
    public TotalOrder<Path> {

    public:

        // Types

        #ifdef _XOPEN_SOURCE
            using character_type = char;
        #else
            using character_type = wchar_t;
        #endif

        class directory_iterator;
        class search_iterator;

        using search_range = Irange<search_iterator>;
        using directory_range = Irange<directory_iterator>;
        using id_type = std::pair<uint64_t, uint64_t>;
        using string_type = std::basic_string<character_type>;
        using time_point = std::chrono::system_clock::time_point;

        enum class flag: int {
            none        = 0,
            append      = 1 << 0,   // Append if file exists
            bottom_up   = 1 << 1,   // Bottom up order
            legal_name  = 1 << 2,   // Throw on illegal file name
            may_copy    = 1 << 3,   // Copy if operation not allowed
            may_fail    = 1 << 4,   // Empty string if read fails
            no_follow   = 1 << 5,   // Don't follow symlinks
            no_hidden   = 1 << 6,   // Skip hidden files
            overwrite   = 1 << 7,   // Delete existing file if necessary
            recurse     = 1 << 8,   // Recursive directory operations
            stdio       = 1 << 9,   // Use stdin/out if file is "" or "-"
            unicode     = 1 << 10,  // Skip files with non-Unicode names
        };

        enum class form: int {
            empty,
            absolute,
            drive_absolute,
            drive_relative,
            relative,
        };

        // Constants

        #ifdef _XOPEN_SOURCE
            static constexpr char delimiter = '/';
            static constexpr char native_delimiter = '/';
        #else
            static constexpr char delimiter = '\\';
            static constexpr wchar_t native_delimiter = L'\\';
        #endif

        #if defined(__APPLE__) || defined(_WIN32)
            static constexpr bool native_case = false;
        #else
            static constexpr bool native_case = true;
        #endif

        // Comparison objects

        enum class cmp: int {
            cased,
            icase,
            native = native_case ? cased : icase,
        };

        class equal {
        public:
            equal() = default;
            explicit equal(cmp mode) noexcept: cmode(mode) {}
            bool operator()(const Path& lhs, const Path& rhs) const;
        private:
            cmp cmode = cmp::native;
        };

        class less {
        public:
            less() = default;
            explicit less(cmp mode) noexcept: cmode(mode) {}
            bool operator()(const Path& lhs, const Path& rhs) const;
        private:
            cmp cmode = cmp::native;
        };

        // Life cycle functions

        Path() = default;
        Path(const std::string& file, flag flags = flag::none);
        Path(const char* file, flag flags = flag::none): Path(std::string(file), flags) {}

        #ifdef _WIN32
            Path(const std::wstring& file, flag flags = flag::none);
            Path(const wchar_t* file, flag flags = flag::none): Path(std::wstring(file), flags) {}
        #endif

        // Path name functions

        std::string name() const;
        std::string str() const { return name(); }
        string_type os_name() const { return filename_; }
        const character_type* c_name() const noexcept { return filename_.data(); }

        std::string as_url() const;
        std::vector<std::string> breakdown() const;
        std::vector<string_type> os_breakdown() const;
        Path change_ext(const std::string& new_ext) const;
        bool empty() const noexcept { return filename_.empty(); }
        size_t hash() const noexcept { return std::hash<string_type>()(filename_); }

        bool is_absolute() const noexcept;
        bool is_drive_absolute() const noexcept;
        bool is_drive_relative() const noexcept;
        bool is_relative() const noexcept;
        bool is_leaf() const noexcept;
        bool is_legal() const noexcept;
        bool is_root() const noexcept;
        bool is_unicode() const noexcept { return is_valid_utf(filename_); }

        form path_form() const noexcept;
        Path relative_to(const Path& base) const;
        std::pair<std::string, std::string> split_leaf() const;
        std::pair<string_type, string_type> split_os_leaf() const;
        std::pair<Path, Path> split_path() const;
        std::pair<Path, Path> split_root() const;

        static Path common(const Path& lhs, const Path& rhs);
        template <typename Range> static Path common(const Range& files);
        static Path join(const Path& lhs, const Path& rhs);
        template <typename Range> static Path join(const Range& files);
        friend Path operator/(const Path& lhs, const Path& rhs) { return Path::join(lhs, rhs); }
        friend Path& operator/=(Path& lhs, const Path& rhs) { lhs = Path::join(lhs, rhs); return lhs; }

        friend std::ostream& operator<<(std::ostream& out, const Path& p) { return out << p.name(); }
        friend std::ostream& operator<<(std::ostream& out, Path::form f);
        friend bool operator==(const Path& lhs, const Path& rhs) noexcept { return lhs.filename_ == rhs.filename_; }
        friend bool operator<(const Path& lhs, const Path& rhs) noexcept { return lhs.filename_ < rhs.filename_; }

        // File system query functions

        time_point access_time(flag flags = flag::none) const noexcept;
        time_point create_time(flag flags = flag::none) const noexcept;
        time_point modify_time(flag flags = flag::none) const noexcept;
        time_point status_time(flag flags = flag::none) const noexcept;

        directory_range directory(flag flags = flag::none) const;
        search_range deep_search(flag flags = flag::none) const;
        bool exists(flag flags = flag::none) const noexcept;
        id_type id(flag flags = flag::none) const noexcept;

        bool is_directory(flag flags = flag::none) const noexcept;
        bool is_file(flag flags = flag::none) const noexcept;
        bool is_special(flag flags = flag::none) const noexcept;
        bool is_hidden() const noexcept;
        bool is_symlink() const noexcept;

        Path resolve() const;
        Path resolve_symlink() const;
        uint64_t size(flag flags = flag::none) const;

        // File system update functions

        void copy_to(const Path& dst, flag flags = flag::none) const;
        void create() const;
        void make_directory(flag flags = flag::none) const;
        void make_symlink(const Path& linkname, flag flags = flag::none) const;
        void move_to(const Path& dst, flag flags = flag::none) const;
        void remove(flag flags = flag::none) const;

        void set_access_time(flag flags = flag::none) const { set_access_time(std::chrono::system_clock::now(), flags); }
        void set_access_time(time_point t, flag flags = flag::none) const;
        void set_create_time(flag flags = flag::none) const { set_create_time(std::chrono::system_clock::now(), flags); }
        void set_create_time(time_point t, flag flags = flag::none) const;
        void set_modify_time(flag flags = flag::none) const { set_modify_time(std::chrono::system_clock::now(), flags); }
        void set_modify_time(time_point t, flag flags = flag::none) const;

        // I/O functions

        void load(std::string& str, size_t maxlen = npos, flag flags = flag::none) const;
        void save(const std::string& str, flag flags = flag::none) const;

        // Process state functions

        void change_directory() const;
        static Path current_directory();

    private:

        string_type filename_;

        std::pair<string_type, string_type> get_base_ext() const noexcept;
        string_type get_leaf() const noexcept;
        string_type get_root(bool allow_drive_special) const noexcept;
        void make_canonical(flag flags);
        template <typename Range, typename BinaryFunction> static Path do_combine(const Range& range, BinaryFunction f);

        #ifdef _XOPEN_SOURCE
            void set_file_times(time_point atime, time_point mtime, flag flags) const;
        #else
            time_point get_file_time(int index) const noexcept;
            void set_file_time(time_point t, int index) const;
        #endif

    };

        template <typename Range>
        Path Path::common(const Range& files) {
            return do_combine(files, [] (const Path& lhs, const Path& rhs) { return common(lhs, rhs); });
        }

        template <typename Range>
        Path Path::join(const Range& files) {
            return do_combine(files, [] (const Path& lhs, const Path& rhs) { return join(lhs, rhs); });
        }

        template <typename Range, typename BinaryFunction>
        Path Path::do_combine(const Range& range, BinaryFunction f) {
            using std::begin;
            using std::end;
            auto i = begin(range), j = end(range);
            if (i == j)
                return {};
            Path p = *i;
            for (++i; i != j; ++i)
                p = f(p, *i);
            return p;
        }

        class Path::search_iterator:
        public InputIterator<search_iterator, const Path> {
        public:
            search_iterator() = default;
            search_iterator(const Path& dir, flag flags);
            const Path& operator*() const noexcept;
            search_iterator& operator++();
            bool operator==(const search_iterator& i) const noexcept { return impl_ == i.impl_; }
        private:
            struct impl_type;
            std::shared_ptr<impl_type> impl_;
        };

        class Path::directory_iterator:
        public InputIterator<directory_iterator, const Path> {
        public:
            directory_iterator() = default;
            directory_iterator(const Path& dir, flag flags);
            const Path& operator*() const noexcept;
            directory_iterator& operator++();
            bool operator==(const directory_iterator& i) const noexcept { return impl_ == i.impl_; }
        private:
            struct impl_type;
            std::shared_ptr<impl_type> impl_;
        };

    CROW_BITMASK_OPERATORS(Path::flag);

}

namespace std {

    template <>
    class hash<Crow::Path> {
        size_t operator()(const Crow::Path& path) const {
            return path.hash();
        }
    };

}
