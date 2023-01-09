#pragma once

#include "crow/log.hpp"
#include <compare>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#define CROW_ASSERT(expr) \
    do { \
        if (! static_cast<bool>(expr)) { \
            std::fprintf(stderr, "Assertion failed: (%s), function %s, file %s, line %d.\n", \
                # expr, __func__, __FILE__, __LINE__); \
            std::abort(); \
        } \
    } while (false)

#define CROW_XASSERT(expr) \
    do { \
        if (! static_cast<bool>(expr)) \
            throw AssertionFailure(# expr, __func__, __FILE__, __LINE__); \
    } while (false)

#define CROW_STD_HASH_0(T) \
    namespace std { \
        template <> struct hash<Crow::T> { \
            size_t operator()(const Crow::T& t) const noexcept { return t.hash(); } \
        }; \
    }

#define CROW_STD_HASH_1(T, U) \
    namespace std { \
        template <U u> struct hash<Crow::T<u>> { \
            size_t operator()(const Crow::T<u>& t) const noexcept { return t.hash(); } \
        }; \
    }

#define CROW_STD_HASH_2(T, U, V) \
    namespace std { \
        template <U u, V v> struct hash<Crow::T<u, v>> { \
            size_t operator()(const Crow::T<u, v>& t) const noexcept { return t.hash(); } \
        }; \
    }

#define CROW_STD_HASH_3(T, U, V, W) \
    namespace std { \
        template <U u, V v, W w> struct hash<Crow::T<u, v, w>> { \
            size_t operator()(const Crow::T<u, v, w>& t) const noexcept { return t.hash(); } \
        }; \
    }

namespace Crow {

    // Constants

    constexpr size_t npos = std::string::npos;

    // Concepts

    template <typename T>
    concept ArithmeticType = std::is_arithmetic_v<T> && ! std::is_same_v<T, bool>;

    template <typename T>
    concept BitwiseType = requires (T t, T u) {
        { ~ t };
        { t & u };
        { t | u };
        { t ^ u };
    };

    template <typename T>
    concept NumericType = std::numeric_limits<T>::is_specialized;

    template <typename T>
    concept IntegralNumericType = std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::is_integer;

    template <typename T>
    concept RealNumericType = std::numeric_limits<T>::is_specialized && ! std::numeric_limits<T>::is_integer;

    template <typename T>
    concept PrimitiveScalarType = std::is_scalar_v<T>;

    template <typename T, typename U>
    concept SameBasicType = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

    template <typename T>
    concept ThreeWayComparable = requires (T t) {
        { t <=> t };
    };

    template <typename T>
    concept IteratorType = requires {
        typename std::iterator_traits<T>::iterator_category;
    };

    // Exceptions

    class AssertionFailure:
    public std::runtime_error {
    public:
        AssertionFailure(std::string_view expression, std::string_view function,
            std::string_view file, int line);
        std::string expression() const { return expression_; }
        std::string function() const { return function_; }
        std::string file() const { return file_; }
        int line() const noexcept { return line_; }
    private:
        std::string expression_;
        std::string function_;
        std::string file_;
        int line_ = 0;
        static std::string message(std::string_view expression, std::string_view function,
            std::string_view file, int line);
    };

        inline AssertionFailure::AssertionFailure(std::string_view expression, std::string_view function,
            std::string_view file, int line):
        std::runtime_error(message(expression, function, file, line)),
        expression_(expression),
        function_(function),
        file_(file),
        line_(line) {}

        inline std::string AssertionFailure::message(std::string_view expression, std::string_view function,
                std::string_view file, int line) {
            std::string msg = "Assertion failed: (";
            msg += expression;
            msg += "), function ";
            msg += function;
            msg += ", file ";
            msg += file;
            msg += ", line ";
            msg += std::to_string(line);
            msg += ".";
            return msg;
        }

    namespace Detail {

        using SO = std::strong_ordering;

        template <typename T>
        concept StdOrderingType = (
            std::same_as<T, std::partial_ordering>
            || std::same_as<T, std::strong_ordering>
            || std::same_as<T, std::weak_ordering>
        );

        template <typename T>
        concept AdlBeginEndType = requires (T t) {
            { begin(t) } -> IteratorType;
            { end(t) } -> IteratorType;
        };

        template <typename T>
        concept StdBeginEndType = requires (T t) {
            { std::begin(t) } -> IteratorType;
            { std::end(t) } -> IteratorType;
        };

        template <typename T, bool Adl = AdlBeginEndType<T>, bool Std = StdBeginEndType<T>>
        struct RangeTraits {
            using iterator_type = void;
            using value_type = void;
        };

        template <typename T, bool Std>
        struct RangeTraits<T, true, Std> {
            using iterator_type = std::decay_t<decltype(begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };

        template <typename T>
        struct RangeTraits<T, false, true> {
            using iterator_type = std::decay_t<decltype(std::begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };

    }

    template <typename T> using RangeIterator = typename Detail::RangeTraits<T>::iterator_type;
    template <typename T> using RangeValue = typename Detail::RangeTraits<T>::value_type;

    template <typename T>
    concept RangeType = Detail::AdlBeginEndType<T> || Detail::StdBeginEndType<T>;

    template <typename T>
    concept InputIteratorType = IteratorType<T> && (
        std::same_as<typename std::iterator_traits<T>::iterator_category, std::input_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::forward_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
    );

    template <typename T>
    concept OutputIteratorType = IteratorType<T> && (
        std::same_as<typename std::iterator_traits<T>::iterator_category, std::output_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::forward_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
    );

    template <typename T>
    concept ForwardIteratorType = IteratorType<T> && (
        std::same_as<typename std::iterator_traits<T>::iterator_category, std::forward_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
    );

    template <typename T>
    concept BidirectionalIteratorType = IteratorType<T> && (
        std::same_as<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>
        || std::same_as<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
    );

    template <typename T>
    concept RandomAccessIteratorType = IteratorType<T> && (
        std::same_as<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
    );

    template <typename T>
    concept InputRangeType = InputIteratorType<RangeIterator<T>>;

    template <typename T>
    concept OutputRangeType = OutputIteratorType<RangeIterator<T>>;

    template <typename T>
    concept ForwardRangeType = ForwardIteratorType<RangeIterator<T>>;

    template <typename T>
    concept BidirectionalRangeType = BidirectionalIteratorType<RangeIterator<T>>;

    template <typename T>
    concept RandomAccessRangeType = RandomAccessIteratorType<RangeIterator<T>>;

    template <typename T>
    concept MaplikeRangeType = RangeType<T>
        && (requires (RangeValue<T> v) {
            { v.first };
            { v.second };
        });

    template <typename T>
    concept SimpleContainerType = requires (T t) {
        typename T::value_type;
        typename T::iterator;
        typename T::const_iterator;
        { t.empty() } -> std::convertible_to<bool>;
        { t.size() } -> std::convertible_to<size_t>;
        { t.begin() } -> IteratorType;
        { t.end() } -> IteratorType;
        { t.clear() };
        { t.insert(t.end(), *t.begin()) };
        { t.erase(t.begin()) };
    };

    template <typename T>
    concept AssociativeContainerType = SimpleContainerType<T>
        && requires (T t, typename T::value_type v, typename T::key_type k) {
            { v.first } -> std::convertible_to<typename T::key_type>;
            { v.second } -> std::convertible_to<typename T::mapped_type>;
            { t[k] } -> std::convertible_to<typename T::mapped_type&>;
            { t.contains(k) } -> std::convertible_to<bool>;
            { t.count(k) } -> std::convertible_to<size_t>;
            { t.erase(t.begin(), t.end()) };
            { t.find(k) } -> std::convertible_to<typename T::iterator>;
        };

    template <typename T>
    concept ContiguousContainerType = SimpleContainerType<T>
        // Missing range concepts in Xcode 14
        #if __apple_build_version__ / 10000 == 1400
            && (requires (T t) {
                { t.data() } -> std::same_as<typename T::value_type*>;
            }
        #else
            && std::ranges::contiguous_range<T>
        #endif
        );

    // Comparison functions

    template <std::totally_ordered T>
    std::strong_ordering compare3way(const T& a, const T& b) noexcept {
        using SO = std::strong_ordering;
        if constexpr (ThreeWayComparable<T>) {
            return a <=> b;
        } else {
            if (a == b)
                return SO::equal;
            else if (a < b)
                return SO::less;
            else
                return SO::greater;
        }
    }

    constexpr std::strong_ordering to_order(ArithmeticType auto t) noexcept {
        using SO = std::strong_ordering;
        if (t < 0)
            return SO::less;
        else if (t == 0)
            return SO::equal;
        else
            return SO::greater;
    }

    // Memory management types

    struct FreeMem {
        void operator()(void* ptr) const noexcept {
            if (ptr != nullptr)
                std::free(ptr);
        }
    };

    // Metaprogramming support

    template <typename T, bool B> struct SfinaeTrue {};
    template <typename T> struct SfinaeTrue<T, true>: std::true_type {};

    template <typename T> constexpr bool dependent_false = false;

}
