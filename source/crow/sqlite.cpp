#include "crow/sqlite.hpp"
#include <algorithm>
#include <utility>
#include <sqlite3.h>

using namespace Crow::Literals;

namespace Crow::Sqlite {

    namespace {

        int check_result(int rc, const std::string& function, const std::shared_ptr<sqlite3>& sqlite) {
            int code = rc & 0xff;
            if (code == SQLITE_OK || code == SQLITE_DONE || code == SQLITE_ROW)
                return rc;
            else
                throw SqliteError(rc, function, sqlite3_errmsg(sqlite.get()));
        }

        std::array<int, 3> expand_version(unsigned ver) {
            int v = int(ver);
            return {{v / 1'000'000, v / 1000 % 1000, v % 1000}};
        }

    }

    // Sqlite version

    std::array<int, 3> compile_version() noexcept {
        return expand_version(SQLITE_VERSION_NUMBER);
    }

    std::array<int, 3> runtime_version() noexcept {
        return expand_version(sqlite3_libversion_number());
    }

    // Exceptions

    std::string SqliteError::format_message(int error, const std::string& function, const std::string& extra) {
        auto message = "Sqlite error {0} in {1}: {2}"_fmt(error, function, sqlite3_errstr(error));
        if (! extra.empty())
            message += ": " + extra;
        return message;
    }

    // Connect class

    Connect::Connect(const std::string& file, int flags) {

        static constexpr int* int_nullptr = nullptr;
        static constexpr const char* memory_file = ":memory:";

        bool anonymous = file.empty();
        bool read_flag = (flags & read) != 0;
        bool write_flag = (flags & write) != 0;
        bool create_flag = (flags & create) != 0;
        bool memory_flag = (flags & memory) != 0;
        bool tempfile_flag = (flags & tempfile) != 0;
        bool nofollow_flag = (flags & nofollow) != 0;
        bool nomutex_flag = (flags & nomutex) != 0;
        bool uri_flag = (flags & uri) != 0;

        if (int(read_flag) + int(write_flag) + int(create_flag) + int(memory_flag) + int(tempfile_flag) > 1
                || ((memory_flag || tempfile_flag) && (nofollow_flag || uri_flag)))
            throw InvalidArgument("Inconsistent Sqlite I/O mode flags");
        if ((flags & persistent) != 0)
            throw InvalidArgument("Persistent flag cannot be used when opening a connection");

        auto name = file;
        int sqlite_flags = 0;

        if (memory_flag) {

            if (! anonymous && name != memory_file)
                throw InvalidArgument("File name cannot be used with in-memory Sqlite database");

            name = memory_file;
            sqlite_flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

        } else if (tempfile_flag) {

            if (! anonymous)
                throw InvalidArgument("File name cannot be used with anonymous temporary Sqlite database");

            sqlite_flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

        } else {

            if (anonymous)
                throw InvalidArgument("No file name was supplied for Sqlite database");

            if (create_flag)
                sqlite_flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            else if (write_flag)
                sqlite_flags |= SQLITE_OPEN_READWRITE;
            else
                sqlite_flags |= SQLITE_OPEN_READONLY;

            if (uri_flag)
                sqlite_flags |= SQLITE_OPEN_URI;

        }

        if (nofollow_flag)
            sqlite_flags |= SQLITE_OPEN_NOFOLLOW;

        if (nomutex_flag)
            sqlite_flags |= SQLITE_OPEN_NOMUTEX;
        else
            sqlite_flags |= SQLITE_OPEN_FULLMUTEX;

        sqlite3* handle = nullptr;
        int rc = sqlite3_open_v2(name.data(), &handle, sqlite_flags, nullptr);
        if (handle)
            sqlite_.reset(handle, [] (auto db) { if (db) sqlite3_close_v2(db); });
        if (rc != SQLITE_OK)
            throw SqliteError(rc, "sqlite3_open_v2()");

        sqlite3_extended_result_codes(native_handle(), 1);

        #ifdef SQLITE_DBCONFIG_DEFENSIVE
            check_result(sqlite3_db_config(native_handle(), SQLITE_DBCONFIG_DEFENSIVE, 1, int_nullptr),
                "sqlite3_db_config()", sqlite_);
        #endif

        check_result(sqlite3_db_config(native_handle(), SQLITE_DBCONFIG_ENABLE_FKEY, 1, int_nullptr),
            "sqlite3_db_config()", sqlite_);
        check_result(sqlite3_db_config(native_handle(), SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, int_nullptr),
            "sqlite3_db_config()", sqlite_);
        check_result(sqlite3_db_config(native_handle(), SQLITE_DBCONFIG_ENABLE_VIEW, 1, int_nullptr),
            "sqlite3_db_config()", sqlite_);
        check_result(sqlite3_db_config(native_handle(), SQLITE_DBCONFIG_TRUSTED_SCHEMA, 0, int_nullptr),
            "sqlite3_db_config()", sqlite_);

        set_pragma("recursive_triggers", "true");

    }

    Query Connect::query(const std::string& sql, int hints) {
        Query qry;
        qry.sqlite_ = sqlite_;
        int stmt_flags = 0;
        if ((hints & ~ persistent) != 0)
            throw InvalidArgument("Flags other than persistent cannot be used when preparing a query");
        if ((hints & persistent) != 0)
            stmt_flags |= SQLITE_PREPARE_PERSISTENT;
        sqlite3_stmt* handle = nullptr;
        check_result(sqlite3_prepare_v3(native_handle(), sql.data(), int(sql.size()), stmt_flags, &handle, nullptr),
            "sqlite3_prepare_v3()", sqlite_);
        qry.stmt_.reset(handle, [] (auto stmt) { if (stmt) sqlite3_finalize(stmt); });
        int n_params = sqlite3_bind_parameter_count(qry.stmt_.get());
        qry.params_.assign(size_t(n_params), false);
        return qry;
    }

    Result Connect::run(const std::string& sql) {
        Query qry = query(sql);
        return qry.run();
    }

    Result Connect::operator()(const std::string& sql) {
        return run(sql);
    }

    void Connect::set_pragma(const std::string& name, const std::string& value) {
        auto sql = "pragma {0} = {1}"_fmt(name, value);
        Query qry = query(sql);
        qry.run();
    }

    void Connect::do_set_timeout(std::chrono::milliseconds t) {
        check_result(sqlite3_busy_timeout(native_handle(), int(t.count())),
            "sqlite3_busy_timeout()", sqlite_);
    }

    void Connect::run_unchecked(const std::string& sql) noexcept {
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v3(native_handle(), sql.data(), int(sql.size()), 0, &stmt, nullptr);
        if (stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    // Query class

    int Query::get_index(const std::string& name) const noexcept {
        return sqlite3_bind_parameter_index(native_handle(), name.data());
    }

    std::string Query::get_name(int index) const {
        auto name = sqlite3_bind_parameter_name(native_handle(), index);
        if (name)
            return name;
        else
            return {};
    }

    void Query::clear() {
        check_result(sqlite3_clear_bindings(native_handle()),
            "sqlite3_clear_bindings()", sqlite_);
        std::fill(params_.begin(), params_.end(), false);
    }

    Result Query::run() {
        check_arg_bindings();
        Result res;
        res.sqlite_ = sqlite_;
        res.stmt_ = stmt_;
        res.next();
        return res;
    }

    Result Query::operator()() {
        return run();
    }

    void Query::bind_null(int index) {
        check_result(sqlite3_bind_null(native_handle(), index),
            "sqlite3_bind_null()", sqlite_);
    }

    void Query::bind_integer(int index, int64_t value) {
        check_result(sqlite3_bind_int64(native_handle(), index, value),
            "sqlite3_bind_int64()", sqlite_);
    }

    void Query::bind_floating(int index, double value) {
        check_result(sqlite3_bind_double(native_handle(), index, value),
            "sqlite3_bind_double()", sqlite_);
    }

    void Query::bind_string(int index, const std::string& value) {
        check_result(sqlite3_bind_blob64(native_handle(), index, value.data(), value.size(), SQLITE_TRANSIENT),
            "sqlite3_bind_blob64()", sqlite_);
    }

    void Query::check_arg_bindings() const {
        auto n_set = std::count(params_.begin(), params_.end(), true);
        if (n_set < ptrdiff_t(params_.size()))
            throw InvalidArgument("Not all parameters set in query ({0} out of {1} set)"_fmt(n_set, params_.size()));
    }

    void Query::check_arg_count(int argc) const {
        if (argc != parameters())
            throw InvalidArgument("Wrong number of parameters in Sqlite query: found {0}, expected {1}"_fmt(argc, parameters()));
    }

    void Query::check_arg_index(int index) const {
        if (index < 1 || index > parameters())
            throw InvalidArgument("Invalid parameter index in Sqlite query: index {0} of {1}"_fmt(index, parameters()));
    }

    // Result class

    Result& Result::operator=(Result&& res) noexcept {
        if (&res != this) {
            close();
            sqlite_ = std::move(res.sqlite_);
            stmt_ = std::move(res.stmt_);
        }
        return *this;
    }

    Result::iterator Result::begin() {
        if (stmt_ && count_ > 1) {
            check_result(sqlite3_reset(stmt_.get()),
                "sqlite3_reset()", sqlite_);
            count_ = 0;
            next();
        }
        return iterator(*this);
    }

    Result::iterator Result::end() {
        return {};
    }

    int Result::columns() const noexcept {
        return stmt_ ? sqlite3_column_count(stmt_.get()) : 0;
    }

    void Result::close() noexcept {
        if (stmt_)
            sqlite3_reset(stmt_.get());
    }

    int64_t Result::get_int(int col) const {
        return sqlite3_column_int64(stmt_.get(), col);
    }

    double Result::get_float(int col) const {
        return sqlite3_column_double(stmt_.get(), col);
    }

    std::string Result::get_string(int col) const {
        auto ptr = static_cast<const char*>(sqlite3_column_blob(stmt_.get(), col));
        if (! ptr)
            return {};
        size_t len = sqlite3_column_bytes(stmt_.get(), col);
        return std::string(ptr, len);
    }

    void Result::next() {
        if (! stmt_)
            return;
        int rc = check_result(sqlite3_step(stmt_.get()),
            "sqlite3_step()", sqlite_);
        if (rc == SQLITE_DONE) {
            sqlite3_reset(stmt_.get());
            stmt_.reset();
            count_ = 0;
        } else {
            ++count_;
        }
    }

    // Result::iterator class

    Result::iterator& Result::iterator::operator++() {
        if (row_.res_) {
            row_.res_->next();
            if (row_.res_->empty())
                row_.res_ = nullptr;
        }
        return *this;
    }

    // Transaction class

    Transaction::Transaction(Connect& con):
    con_(&con) {
        con_->run("begin transaction");
    }

    Transaction::~Transaction() noexcept {
        if (con_ != nullptr)
            con_->run_unchecked("rollback transaction");
    }

    void Transaction::commit() {
        if (con_ == nullptr)
            throw InvalidOperation("Transaction has already been committed or rolled back");
        std::exchange(con_, nullptr)->run("commit transaction");
    }

    void Transaction::rollback() {
        if (con_ == nullptr)
            throw InvalidOperation("Transaction has already been committed or rolled back");
        std::exchange(con_, nullptr)->run("rollback transaction");
    }

}
