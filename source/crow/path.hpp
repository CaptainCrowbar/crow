#pragma once

#include "crow/enum.hpp"
#include "crow/io-utility.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/unicode.hpp"
#include <chrono>
#include <compare>
#include <functional>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

namespace Crow {

    class Path {

    public:

        // Types

        #ifdef _XOPEN_SOURCE
            using os_char = char;
        #else
            using os_char = wchar_t;
        #endif

        using id_type = std::pair<uint64_t, uint64_t>;
        using os_string = std::basic_string<os_char>;
        using time_point = std::chrono::system_clock::time_point;

        enum class flag_type: int {
            no_flags      = 0,
            append        = 1 << 0,   // Append if file exists
            bottom_up     = 1 << 1,   // Bottom up order
            legal_name    = 1 << 2,   // Throw on illegal file name
            may_copy      = 1 << 3,   // Copy if operation not allowed
            may_fail      = 1 << 4,   // Empty string if read fails
            no_backtrack  = 1 << 5,   // Don't search upward through directories
            no_follow     = 1 << 6,   // Don't follow symlinks
            no_hidden     = 1 << 7,   // Skip hidden files
            overwrite     = 1 << 8,   // Delete existing file if necessary
            recurse       = 1 << 9,   // Recursive directory operations
            stdio         = 1 << 10,  // Use stdin/out if file is "" or "-"
            unicode       = 1 << 11,  // Skip files with non-Unicode names
        };

        using enum flag_type;

        enum class form: int {
            empty,
            absolute,
            drive_absolute,
            drive_relative,
            relative,
        };

        enum class kind: int {
            none,
            directory,
            file,
            special,
            symlink
        };

        class directory_iterator:
        public InputIterator<directory_iterator, const Path> {
        public:
            directory_iterator() = default;
            directory_iterator(const Path& dir, flag_type flags);
            const Path& operator*() const noexcept;
            directory_iterator& operator++();
            bool operator==(const directory_iterator& i) const noexcept { return impl_ == i.impl_; }
        private:
            struct impl_type;
            std::shared_ptr<impl_type> impl_;
        };

        class search_iterator:
        public InputIterator<search_iterator, const Path> {
        public:
            search_iterator() = default;
            search_iterator(const Path& dir, flag_type flags);
            const Path& operator*() const noexcept;
            search_iterator& operator++();
            bool operator==(const search_iterator& i) const noexcept { return impl_ == i.impl_; }
        private:
            struct impl_type;
            std::shared_ptr<impl_type> impl_;
        };

        using directory_range = Irange<directory_iterator>;
        using search_range = Irange<search_iterator>;

        // Constants

        #ifdef _XOPEN_SOURCE
            static constexpr char delimiter = '/';
            static constexpr char os_delimiter = '/';
        #else
            static constexpr char delimiter = '\\';
            static constexpr wchar_t os_delimiter = L'\\';
        #endif

        #if defined(__APPLE__)
            #if TARGET_OS_IPHONE // True on all non-Mac targets
                static constexpr bool os_case_sensitive = true;
            #else
                static constexpr bool os_case_sensitive = false;
            #endif
        #elif defined(_WIN32)
            static constexpr bool os_case_sensitive = false;
        #else
            static constexpr bool os_case_sensitive = true;
        #endif

        // Comparison objects

        enum class cmp: int {
            cased,
            icase,
            native = os_case_sensitive ? cased : icase,
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
        Path(std::string_view file, flag_type flags = no_flags);
        Path(const std::string& file, flag_type flags = no_flags): Path(std::string_view(file), flags) {}
        Path(const char* file, flag_type flags = no_flags): Path(std::string_view(file), flags) {}

        #ifdef _WIN32
            Path(std::wstring_view file, flag_type flags = no_flags);
            Path(const std::wstring& file, flag_type flags = no_flags): Path(std::wstring_view(file), flags) {}
            Path(const wchar_t* file, flag_type flags = no_flags): Path(std::wstring_view(file), flags) {}
        #endif

        // Path name functions

        std::string name() const;
        std::string str() const { return name(); }
        os_string os_name() const { return filename_; }
        const os_char* c_name() const noexcept { return filename_.data(); }

        std::string as_url() const;
        std::vector<std::string> breakdown() const;
        std::vector<os_string> os_breakdown() const;
        Path change_ext(std::string_view new_ext) const;
        size_t hash() const noexcept { return std::hash<os_string>()(filename_); }

        bool is_empty() const noexcept { return filename_.empty(); }
        bool is_absolute() const noexcept;
        bool is_drive_absolute() const noexcept;
        bool is_drive_relative() const noexcept;
        bool is_relative() const noexcept;
        bool is_leaf() const noexcept;
        bool is_legal() const noexcept;
        bool is_root() const noexcept;
        bool is_unicode() const noexcept { return is_valid_utf(filename_); }

        form path_form() const noexcept;
        Path relative_to(const Path& base, flag_type flags = no_flags) const;
        std::pair<std::string, std::string> split_leaf() const;
        std::pair<os_string, os_string> split_os_leaf() const;
        std::pair<Path, Path> split_path() const;
        std::pair<Path, Path> split_root() const;
        std::string base() const { return split_leaf().first; }
        std::string ext() const { return split_leaf().second; }
        os_string os_base() const { return split_os_leaf().first; }
        os_string os_ext() const { return split_os_leaf().second; }
        Path parent() const { return split_path().first; }
        Path leaf() const { return split_path().second; }
        Path root() const { return split_root().first; }
        Path from_root() const { return split_root().second; }

        static Path common(const Path& lhs, const Path& rhs);
        template <typename Range> static Path common(const Range& files);
        static Path join(const Path& lhs, const Path& rhs);
        template <typename Range> static Path join(const Range& files);
        friend Path operator/(const Path& lhs, const Path& rhs) { return Path::join(lhs, rhs); }
        friend Path& operator/=(Path& lhs, const Path& rhs) { lhs = Path::join(lhs, rhs); return lhs; }

        friend std::ostream& operator<<(std::ostream& out, const Path& p) { return out << p.name(); }
        friend std::ostream& operator<<(std::ostream& out, form f);
        friend std::ostream& operator<<(std::ostream& out, kind k);
        friend bool operator==(const Path& lhs, const Path& rhs) noexcept { return lhs.filename_ == rhs.filename_; }
        friend std::strong_ordering operator<=>(const Path& lhs, const Path& rhs) noexcept;

        // File system query functions

        time_point access_time(flag_type flags = no_flags) const noexcept;
        time_point create_time(flag_type flags = no_flags) const noexcept;
        time_point modify_time(flag_type flags = no_flags) const noexcept;
        time_point status_time(flag_type flags = no_flags) const noexcept;

        directory_range directory(flag_type flags = no_flags) const;
        search_range deep_search(flag_type flags = no_flags) const;
        bool exists(flag_type flags = no_flags) const noexcept;
        id_type id(flag_type flags = no_flags) const noexcept;

        kind file_kind(flag_type flags = no_flags) const noexcept;
        bool is_directory(flag_type flags = no_flags) const noexcept;
        bool is_file(flag_type flags = no_flags) const noexcept;
        bool is_special(flag_type flags = no_flags) const noexcept;
        bool is_hidden() const noexcept;
        bool is_symlink() const noexcept;

        Path resolve() const;
        Path resolve_symlink() const;
        uint64_t size(flag_type flags = no_flags) const;

        // File system update functions

        void copy_to(const Path& dst, flag_type flags = no_flags) const;
        void create() const;
        void make_directory(flag_type flags = no_flags) const;
        void make_symlink(const Path& linkname, flag_type flags = no_flags) const;
        void move_to(const Path& dst, flag_type flags = no_flags) const;
        void remove(flag_type flags = no_flags) const;

        void set_access_time(flag_type flags = no_flags) const { set_access_time(std::chrono::system_clock::now(), flags); }
        void set_access_time(time_point t, flag_type flags = no_flags) const;
        void set_create_time(flag_type flags = no_flags) const { set_create_time(std::chrono::system_clock::now(), flags); }
        void set_create_time(time_point t, flag_type flags = no_flags) const;
        void set_modify_time(flag_type flags = no_flags) const { set_modify_time(std::chrono::system_clock::now(), flags); }
        void set_modify_time(time_point t, flag_type flags = no_flags) const;

        // I/O functions

        void load(std::string& content, size_t maxlen = npos, flag_type flags = no_flags) const;
        void save(std::string_view content, flag_type flags = no_flags) const;

        // Process state functions

        void change_directory() const;
        static Path current_directory();

    private:

        os_string filename_;

        std::pair<os_string, os_string> get_base_ext() const noexcept;
        os_string get_leaf() const noexcept;
        void make_canonical(flag_type flags);
        template <typename Range, typename BinaryFunction> static Path do_combine(const Range& range, BinaryFunction f);

        #ifdef _XOPEN_SOURCE
            void set_file_times(time_point atime, time_point mtime, flag_type flags) const;
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

    CROW_BITMASK_OPERATORS(Path::flag_type);

}

CROW_STD_HASH_0(Path)
