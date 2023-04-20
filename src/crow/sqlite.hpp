#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/iterator.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

struct sqlite3;
struct sqlite3_stmt;

namespace Crow::Sqlite {

    class Connect;
    class Query;
    class Result;
    class Row;
    class Transaction;

    std::array<int, 3> compile_version() noexcept;
    std::array<int, 3> runtime_version() noexcept;

    enum class Mode: uint32_t {
        none        = 0,
        // Open mode flags
        read        = 1u << 0,  // Open database in read only mode; fail if it does not exist (default)
        write       = 1u << 1,  // Open database in read/write mode; fail if it does not exist
        create      = 1u << 2,  // Open database in read/write mode; create the file if necessary
        memory      = 1u << 3,  // Create anonymous temporary database in memory
        tempfile    = 1u << 4,  // Create anonymous temporary database on disk
        // Other connection flags
        nofollow    = 1u << 5,  // File may not be a symlink
        nomutex     = 1u << 6,  // Connections may not be shared between threads
        uri         = 1u << 7,  // Enable URI file names
        // Query usage hints
        persistent  = 1u << 8,  // Hint that a query should be persistent
    };

    CROW_BITMASK_OPERATORS(Mode)

    class Exception:
    public std::runtime_error {
    protected:
        explicit Exception(const std::string& message):
            std::runtime_error(message) {}
    };

        class InvalidArgument:
        public Exception {
        public:
            explicit InvalidArgument(const std::string& message): Exception(message) {}
        };

        class InvalidOperation:
        public Exception {
        public:
            explicit InvalidOperation(const std::string& message): Exception(message) {}
        };

        class SqliteError:
        public Exception {
        public:
            SqliteError(int error, const std::string& function, const std::string& extra = {}):
                Exception(format_message(error, function, extra)), error_(error) {}
            int error() const noexcept { return error_; }
        private:
            static std::string format_message(int error, const std::string& function, const std::string& extra);
            int error_ = 0;
        };

    class Connect {

    public:

        Connect() = default;
        explicit Connect(Mode flags): Connect({}, flags) {}
        explicit Connect(const std::string& file, Mode flags = Mode::read);
        Connect(const Connect&) = delete;
        Connect(Connect&&) = default;
        ~Connect() = default;
        Connect& operator=(const Connect&) = delete;
        Connect& operator=(Connect&&) = default;

        Query query(const std::string& sql, Mode flags = Mode::none);
        Result run(const std::string& sql);
        Result operator()(const std::string& sql);
        template <typename T> void set_pragma(const std::string& name, const T& value);
        template <typename R, typename P> void set_timeout(std::chrono::duration<R, P> t);
        sqlite3* native_handle() const noexcept { return sqlite_.get(); }

    private:

        friend class Transaction;

        std::shared_ptr<sqlite3> sqlite_;

        void do_set_pragma(const std::string& name, const std::string& value);
        void do_set_timeout(std::chrono::milliseconds t);
        void run_unchecked(const std::string& sql) noexcept;

    };

        template <typename T>
        void Connect::set_pragma(const std::string& name, const T& value) {
            do_set_pragma(name, format_object(value));
        }

        template <typename R, typename P>
        void Connect::set_timeout(std::chrono::duration<R, P> t) {
            using namespace std::chrono;
            do_set_timeout(duration_cast<milliseconds>(t));
        }

    class Query {

    public:

        Query() = default;
        Query(const Query&) = delete;
        Query(Query&&) = default;
        ~Query() = default;
        Query& operator=(const Query&) = delete;
        Query& operator=(Query&&) = default;

        int get_index(const std::string& name) const noexcept;
        std::string get_name(int index) const;
        int parameters() const noexcept { return int(params_.size()); }
        template <typename T, typename... Args> void bind(const T& arg1, const Args&... args);
        template <typename T> void set(int index, const T& value) { bind_helper(index, value); }
        template <typename T> void set(const std::string& name, const T& value);
        void clear();
        Result run();
        template <typename T, typename... Args> Result run(const T& arg1, const Args&... args);
        Result operator()();
        template <typename T, typename... Args> Result operator()(const T& arg1, const Args&... args);
        sqlite3_stmt* native_handle() const noexcept { return stmt_.get(); }

    private:

        friend class Connect;

        std::shared_ptr<sqlite3> sqlite_;
        std::shared_ptr<sqlite3_stmt> stmt_;
        std::vector<bool> params_; // Offset by 1 because Sqlite uses 1-based indexing

        template <typename T, typename... Args> void bind_helper(int index, const T& arg1, const Args&... args);
        void bind_null(int index);
        void bind_integer(int index, int64_t value);
        void bind_real(int index, double value);
        void bind_string(int index, const std::string& value);
        void check_arg_bindings() const;
        void check_arg_count(int argc) const;
        void check_arg_index(int index) const;
        int check_arg_name(const std::string& name) const;

    };

        template <typename T, typename... Args>
        void Query::bind(const T& arg1, const Args&... args) {
            check_arg_count(int(sizeof...(Args)) + 1);
            bind_helper(1, arg1, args...);
        }

        template <typename T>
        void Query::set(const std::string& name, const T& value) {
            int index = check_arg_name(name);
            bind_helper(index, value);
        }

        template <typename T, typename... Args>
        void Query::bind_helper(int index, const T& arg1, const Args&... args) {
            check_arg_index(index);
            if constexpr (std::is_same_v<T, std::nullptr_t>)
                bind_null(index);
            else if constexpr (std::is_same_v<T, char>)
                bind_string(index, std::string{arg1});
            else if constexpr (std::is_integral_v<T>)
                bind_integer(index, int64_t(arg1));
            else if constexpr (std::is_floating_point_v<T>)
                bind_real(index, float(arg1));
            else if constexpr (std::is_convertible_v<T, std::string>)
                bind_string(index, std::string(arg1));
            else
                static_assert(dependent_false<T>, "Unknown parameter type in Query::bind()");
            params_[index - 1] = true;
            if constexpr (sizeof...(Args) > 0)
                bind_helper(index + 1, args...);
        }

    class Result {

    public:

        class iterator;

        Result() = default;
        Result(const Result&) = delete;
        Result(Result&& res) = default;
        ~Result() noexcept { close(); }
        Result& operator=(const Result&) = delete;
        Result& operator=(Result&& res) noexcept;

        iterator begin();
        iterator end();
        int columns() const noexcept;
        bool empty() const noexcept { return ! stmt_; }
        template <typename T> T get(int col = 0) const { T t{}; get(col, t); return t; }
        template <typename T> void get(int col, T& t) const;
        template <typename... Args> void read(Args&... args) const;
        template <typename T> explicit operator T() const { return get<T>(); }

    private:

        friend class Query;
        friend class Row;

        std::shared_ptr<sqlite3> sqlite_;
        std::shared_ptr<sqlite3_stmt> stmt_;
        int64_t count_ = 0;

        void check_arg_count(int argc) const;
        void close() noexcept;
        template <typename T, typename... Args> void do_read(int col, T& t, Args&... args) const;
        int64_t get_int(int col) const;
        double get_float(int col) const;
        std::string get_string(int col) const;
        void next();

    };

        template <typename T>
        void Result::get(int col, T& t) const {
            if constexpr (std::is_integral_v<T>)
                t = static_cast<T>(get_int(col));
            else if constexpr (std::is_floating_point_v<T>)
                t = static_cast<T>(get_float(col));
            else if constexpr (std::is_convertible_v<std::string, T>)
                t = static_cast<T>(get_string(col));
            else
                static_assert(dependent_false<T>, "Unknown parameter type in Result::get()");
        }

        template <typename... Args>
        void Result::read(Args&... args) const {
            check_arg_count(int(sizeof...(Args)));
            do_read(0, args...);
        }

        template <typename T, typename... Args>
        void Result::do_read(int col, T& t, Args&... args) const {
            get(col, t);
            if constexpr (sizeof...(Args) > 0)
                do_read(col + 1, args...);
        }

    class Row {
    public:
        Row() = default;
        int columns() const noexcept { return res_ ? res_->columns() : 0; }
        int64_t index() const noexcept { return res_ ? res_->count_ - 1 : 0; }
        template <typename T> T get(int col) const { return res_ ? res_->get<T>(col) : T(); }
        template <typename T> void get(int col, T& t) const { if (res_) res_->get(col, t); }
        template <typename... Args> void read(Args&... args) const { if (res_) res_->read(args...); }
    private:
        friend class Result;
        Result* res_ = nullptr;
        explicit Row(Result& res) noexcept { if (res.stmt_) res_ = &res; }
    };

        template <typename T, typename... Args>
        Result Query::run(const T& arg1,  const Args&... args) {
            bind(arg1, args...);
            return run();
        }

        template <typename T, typename... Args>
        Result Query::operator()(const T& arg1,  const Args&... args) {
            return run(arg1, args...);
        }

        class Result::iterator:
        public InputIterator<Result::iterator, const Row> {
        public:
            iterator() = default;
            explicit iterator(Result& res) noexcept: row_(res) {}
            const Row& operator*() const noexcept { return row_; }
            iterator& operator++();
            bool operator==(const iterator& rhs) const noexcept { return row_.res_ == rhs.row_.res_; }
        private:
            Row row_;
        };

    class Transaction {
    public:
        explicit Transaction(Connect& con);
        ~Transaction() noexcept;
        Transaction(const Transaction&) = delete;
        Transaction(Transaction&&) = delete;
        Transaction& operator=(const Transaction&) = delete;
        Transaction& operator=(Transaction&&) = delete;
        void commit();
        void rollback();
    private:
        Connect* con_;
    };

}
