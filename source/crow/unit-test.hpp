#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

namespace Crow::UnitTest {

    namespace Detail {

        constexpr int max_failures = 100;

        inline bool isatty_function(int fd) noexcept {
            #ifdef _WIN32
                return _isatty(fd) != 0;
            #else
                return isatty(fd) != 0;
            #endif
        }

        inline std::string text_colour(int r, int g, int b) {
            if (isatty_function(1))
                return "\x1b[38;5;" + std::to_string(36 * r + 6 * g + b + 16) + "m";
            else
                return {};
        }

        const std::string reset_style = isatty_function(1) ? "\x1b[0m" : "";
        const std::string info_style = text_colour(5,5,3);
        const std::string pass_style = text_colour(2,5,2);
        const std::string fail_style = text_colour(5,3,0);
        const std::string delimiter_line = info_style + std::string(30, '=') + reset_style;

        inline std::atomic<int> failure_count(0);
        inline std::chrono::system_clock::time_point start_time;
        inline std::regex test_pattern(".*");

    }

    // Utility functions

    // Format a range as "[a,b,c,...]"

    template <typename Range>
    std::string format_range(const Range& r) {
        std::ostringstream out;
        out << '[';
        for (auto& x: r)
            out << x << ',';
        auto str = out.str();
        if (str.size() > 1)
            str.pop_back();
        str += ']';
        return str;
    }

    // Format a range of pairs as "{a:1,b:2,c:3,...}"

    template <typename Range>
    std::string format_map(const Range& r) {
        std::ostringstream out;
        out << '{';
        for (auto& p: r)
            out << p.first << ':' << p.second << ',';
        auto str = out.str();
        if (str.size() > 1)
            str.pop_back();
        str += '}';
        return str;
    }

    // Human readable type name

    template <typename T>
    std::string type_name() {
        std::string name = typeid(T).name();
        #ifdef __GNUC__
            auto mangled = name;
            std::shared_ptr<char> demangled;
            int status = 0;
            for (;;) {
                if (mangled.empty())
                    break;
                demangled.reset(abi::__cxa_demangle(mangled.data(), nullptr, nullptr, &status), std::free);
                if (status == -1)
                    throw std::bad_alloc();
                if (status == 0 && demangled)
                    return demangled.get();
                if (mangled[0] != '_')
                    break;
                mangled.erase(0, 1);
            }
        #endif
        return name;
    }

    // Type wrapper with population tracking

    template <typename T>
    class Counted {
    public:
        Counted(): value_() { ++number(); }
        Counted(const T& t): value_(t) { ++number(); }
        Counted(const Counted& c): value_(c.value_) { ++number(); }
        Counted(Counted&& c) noexcept: value_(std::exchange(c.value_, T())) { ++number(); }
        ~Counted() noexcept { --number(); }
        Counted& operator=(const Counted& c) = default;
        Counted& operator=(Counted&& c) noexcept { if (&c != this) value_ = std::exchange(c.value_, T()); return *this; }
        Counted& operator=(const T& t) { value_ = t; return *this; }
        T& operator*() noexcept { return value_; }
        const T& operator*() const noexcept { return value_; }
        T* operator->() noexcept { return &value_; }
        const T* operator->() const noexcept { return &value_; }
        const T& get() const noexcept { return value_; }
        void set(const T& t) { value_ = t; }
        static int count() noexcept { return number(); }
        static void reset() noexcept { number() = 0; }
        friend bool operator==(const Counted& a, const Counted& b) { return a.value_ == b.value_; }
        friend bool operator!=(const Counted& a, const Counted& b) { return ! (a == b); }
        friend bool operator<(const Counted& a, const Counted& b) { return a.value_ < b.value_; }
        friend bool operator>(const Counted& a, const Counted& b) { return b < a; }
        friend bool operator<=(const Counted& a, const Counted& b) { return ! (b < a); }
        friend bool operator>=(const Counted& a, const Counted& b) { return ! (a < b); }
        friend std::ostream& operator<<(std::ostream& out, const Counted& c) { return out << c.value_; }
    private:
        T value_;
        static std::atomic<int>& number() noexcept { static std::atomic<int> n(0); return n; }
    };

    // Call these at the beginning and end of main()

    inline void begin_tests(int argc, char** argv) {
        using namespace std::chrono;
        if (argc > 1) {
            std::string pattern_str;
            for (int i = 1; i < argc; ++i)
                pattern_str += "(" + std::string(argv[i]) + ")|";
            pattern_str.pop_back();
            Detail::test_pattern = std::regex(pattern_str, std::regex::icase | std::regex::nosubs);
        }
        std::cout << "\n"
            << Detail::info_style << "Running unit tests" << Detail::reset_style << "\n"
            << Detail::delimiter_line << std::endl;
        Detail::start_time = system_clock::now();
    }

    inline int end_tests() {
        using namespace std::chrono;
        auto time = system_clock::now() - Detail::start_time;
        std::cout << Detail::delimiter_line << "\n";
        double seconds = 1e-9 * duration_cast<nanoseconds>(time).count();
        if (Detail::failure_count == 0)
            std::cout << Detail::pass_style << "OK: all tests passed" << Detail::reset_style << "\n";
        else
            std::cout << Detail::fail_style << "*** Tests failed: " << Detail::failure_count << Detail::reset_style << "\n";
        std::cout << Detail::info_style << "Time: " << seconds << " s" << Detail::reset_style << "\n\n";
        return int(Detail::failure_count > 0);
    }

}

namespace std {

    template <typename T>
    struct hash<::Crow::UnitTest::Counted<T>> {
        std::size_t operator()(const ::Crow::UnitTest::Counted<T>& ct) const {
            return ::std::hash<T>()(*ct);
        }
    };

}

// Call this from main() for each test

#define UNIT_TEST(name) \
    if (std::regex_search(# name, ::Crow::UnitTest::Detail::test_pattern)) { \
        extern void test_##name(); \
        try { \
            std::string title(#name); \
            std::replace(title.begin(), title.end(), '_', ' '); \
            std::cout << ::Crow::UnitTest::Detail::info_style << "Test " << title << ::Crow::UnitTest::Detail::reset_style << std::endl; \
            test_##name(); \
        } \
        catch (const std::exception& ex) { \
            std::cout << ::Crow::UnitTest::Detail::fail_style << "*** " << ex.what() << std::endl; \
        } \
    }

// Fail a test (normally only used internally)

#define FAIL_TEST(message) { \
    std::cout << ::Crow::UnitTest::Detail::fail_style \
        << "*** [" << __FILE__ << ":" << __LINE__ << "] " \
        << message << ::Crow::UnitTest::Detail::reset_style << std::endl; \
    if (++::Crow::UnitTest::Detail::failure_count >= ::Crow::UnitTest::Detail::max_failures) \
        return; \
}

// Fail if the expression throws an exception

#define TRY(expr) \
    try { \
        expr; \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the expression is false or throws an exception

#define TEST(expr) \
    try { \
        if (! (expr)) \
            FAIL_TEST("Expression is false: " << # expr); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail and cut testing short if the expression is false or throws an exception

#define REQUIRE(expr) \
    try { \
        if (! (expr)) { \
            FAIL_TEST("Expression is false: " << # expr); \
            return; \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
        return; \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
        return; \
    }

// Fail if the two expressions are not equal, or either of them throws an exception

#define TEST_EQUAL(lhs, rhs) \
    try { \
        auto lhs_value = (lhs); \
        auto rhs_value = (rhs); \
        if (lhs_value != rhs_value) \
            FAIL_TEST("Expressions are not equal: " \
                << # lhs << " = " << lhs_value << ", " \
                << # rhs << " = " << rhs_value); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

#define TEST_EQUAL_RANGES(lhs, rhs) \
    try { \
        using std::begin; \
        using std::end; \
        auto lhs_it = begin(lhs); \
        auto lhs_end = end(lhs); \
        auto rhs_it = begin(rhs); \
        auto rhs_end = end(rhs); \
        auto lhs_size = std::distance(lhs_it, lhs_end); \
        auto rhs_size = std::distance(rhs_it, rhs_end); \
        if (lhs_size != rhs_size) \
            FAIL_TEST("Ranges have different lengths: " \
                << # lhs << " [" << lhs_size << "], " \
                << # rhs << " [" << rhs_size << "]"); \
        int index_value = 0; \
        for (; lhs_it != lhs_end; ++lhs_it, ++rhs_it, ++index_value) \
            if (*lhs_it != *rhs_it) \
                FAIL_TEST("Ranges are not equal: " \
                    << # lhs << " [" << index_value << "] = " << *lhs_it << ", " \
                    << # rhs << " [" << index_value << "] = " << *rhs_it); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// Fail if the first expression is outside the range

#define TEST_IN_RANGE(expr, min, max) \
    try { \
        auto expr_value = (expr); \
        auto min_value = (min); \
        auto max_value = (max); \
        if (expr_value < min_value || expr_value > max_value) \
            FAIL_TEST("Expression is out of range: " \
                << # expr << " = " << expr_value << " vs " \
                << # min << " = " << min_value << "," \
                << # max << " = " << max_value); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// Fail if two floating point values are not within epsilon of each other

#define TEST_NEAR(lhs, rhs, epsilon) \
    try { \
        auto lhs_value = static_cast<long double>(lhs); \
        auto rhs_value = static_cast<long double>(rhs); \
        auto epsilon_value = static_cast<long double>(epsilon); \
        if (std::abs(rhs_value - lhs_value) > epsilon_value) \
            FAIL_TEST("Expressions are not close enough: " \
                << # lhs << " = " << lhs_value << ", " \
                << # rhs << " = " << rhs_value << ", " \
                << "epsilon = " << epsilon_value); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// Fail if the first expression does not match the regex pattern, or throws an exception

#define TEST_MATCH(expr, pattern) \
    try { \
        auto expr_value = (expr); \
        std::regex pattern_regex(pattern); \
        if (! std::regex_search(expr_value, pattern_regex)) \
            FAIL_TEST("Expression does not match regex: " \
                << # expr << " = " << expr_value \
                << ", pattern = " << # pattern); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

#define TEST_MATCH_ICASE(expr, pattern) \
    try { \
        auto expr_value = (expr); \
        std::regex pattern_regex(pattern, std::regex::icase); \
        if (! std::regex_search(expr_value, pattern_regex)) \
            FAIL_TEST("Expression does not match regex: " \
                << # expr << " = " << expr_value \
                << ", pattern = " << # pattern); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the expression does not throw an exception, or throws one of the wrong type

#define TEST_THROW(expr, extype) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype&) {} \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Also check that the error message matches

#define TEST_THROW_MESSAGE(expr, extype, message) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype& ex) { \
        std::string ex_what(ex.what()); \
        std::string message_str(message); \
        if (ex_what != message_str) \
            FAIL_TEST("Exception message does not match: " \
                << "what = " << ex_what \
                << ", expected = " << message_str); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

#define TEST_THROW_MATCH(expr, extype, pattern) \
    try { \
        expr; \
        FAIL_TEST("Expression failed to throw: " << # expr); \
    } \
    catch (const extype& ex) { \
        std::string ex_what(ex.what()); \
        std::regex pattern_regex(pattern, std::regex::icase); \
        if (! std::regex_search(ex_what, pattern_regex)) \
            FAIL_TEST("Exception message does not match regex: " \
                << "what = " << ex_what \
                << ", pattern = " << std::string(pattern)); \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception from " << # expr << ": " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception from " << # expr); \
    }

// Fail if the two types are not the same

#define TEST_TYPE(lhs, rhs) { \
    if (! std::is_same_v<lhs, rhs>) \
        FAIL_TEST("Types are not the same: " \
            << # lhs << " => " << ::Crow::UnitTest::type_name<lhs>() << ", " \
            << # rhs << " => " << ::Crow::UnitTest::type_name<rhs>()); \
}
