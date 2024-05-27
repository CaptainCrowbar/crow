#pragma once

#include "crow/log.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
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

    // Basic types

    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::ptrdiff_t;
    using std::size_t;

    using Callback = std::function<void()>;

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
    concept NonIntegralNumericType = std::numeric_limits<T>::is_specialized && ! std::numeric_limits<T>::is_integer;

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

    template <typename T>
    concept MutableIteratorType = IteratorType<T> && requires (T t, T u) {
        { *t = *u };
    };

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
    template <typename T> concept RangeType = Detail::AdlBeginEndType<T> || Detail::StdBeginEndType<T>;
    template <typename T> concept MutableRangeType = RangeType<T> && MutableIteratorType<RangeIterator<T>>;

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

    template <typename T> concept MutableInputIteratorType = InputIteratorType<T> && MutableIteratorType<T>;
    template <typename T> concept MutableOutputIteratorType = OutputIteratorType<T> && MutableIteratorType<T>;
    template <typename T> concept MutableForwardIteratorType = ForwardIteratorType<T> && MutableIteratorType<T>;
    template <typename T> concept MutableBidirectionalIteratorType = BidirectionalIteratorType<T> && MutableIteratorType<T>;
    template <typename T> concept MutableRandomAccessIteratorType = RandomAccessIteratorType<T> && MutableIteratorType<T>;
    template <typename T> concept InputRangeType = InputIteratorType<RangeIterator<T>>;
    template <typename T> concept OutputRangeType = OutputIteratorType<RangeIterator<T>>;
    template <typename T> concept ForwardRangeType = ForwardIteratorType<RangeIterator<T>>;
    template <typename T> concept BidirectionalRangeType = BidirectionalIteratorType<RangeIterator<T>>;
    template <typename T> concept RandomAccessRangeType = RandomAccessIteratorType<RangeIterator<T>>;
    template <typename T> concept MutableInputRangeType = MutableInputIteratorType<RangeIterator<T>>;
    template <typename T> concept MutableOutputRangeType = MutableOutputIteratorType<RangeIterator<T>>;
    template <typename T> concept MutableForwardRangeType = MutableForwardIteratorType<RangeIterator<T>>;
    template <typename T> concept MutableBidirectionalRangeType = MutableBidirectionalIteratorType<RangeIterator<T>>;
    template <typename T> concept MutableRandomAccessRangeType = MutableRandomAccessIteratorType<RangeIterator<T>>;

    template <typename T>
    concept MaplikeRangeType = RangeType<T>
        && (requires (RangeValue<T> v) {
            { v.first };
            { v.second };
        });

    template <typename T>
    concept MutableMaplikeRangeType = MaplikeRangeType<T>
        && (requires (RangeIterator<T> i, RangeIterator<T> j) {
            { i->second = j->second };
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
        #if __apple_build_version__ / 1'000'000 == 14
            && requires (T t) {
                { t.data() } -> std::same_as<typename T::value_type*>;
            };
        #else
            && std::ranges::contiguous_range<T>;
        #endif

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

    // Constants

    constexpr size_t npos = std::string::npos;

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

    // Function objects

    template <typename T = void, typename U = T> struct Plus        { auto operator()(const T& t, const U& u) const { return t + u; } };
    template <typename T = void, typename U = T> struct Minus       { auto operator()(const T& t, const U& u) const { return t - u; } };
    template <typename T = void, typename U = T> struct Multiplies  { auto operator()(const T& t, const U& u) const { return t * u; } };
    template <typename T = void, typename U = T> struct Divides     { auto operator()(const T& t, const U& u) const { return t / u; } };
    template <typename T = void, typename U = T> struct Modulus     { auto operator()(const T& t, const U& u) const { return t % u; } };
    template <typename T = void, typename U = T> struct BitAnd      { auto operator()(const T& t, const U& u) const { return t & u; } };
    template <typename T = void, typename U = T> struct BitOr       { auto operator()(const T& t, const U& u) const { return t | u; } };
    template <typename T = void, typename U = T> struct BitXor      { auto operator()(const T& t, const U& u) const { return t ^ u; } };

    template <> struct Plus<void, void>        { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t + u; } };
    template <> struct Minus<void, void>       { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t - u; } };
    template <> struct Multiplies<void, void>  { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t * u; } };
    template <> struct Divides<void, void>     { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t / u; } };
    template <> struct Modulus<void, void>     { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t % u; } };
    template <> struct BitAnd<void, void>      { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t & u; } };
    template <> struct BitOr<void, void>       { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t | u; } };
    template <> struct BitXor<void, void>      { template <typename T, typename U> auto operator()(const T& t, const U& u) const { return t ^ u; } };

    // Literals

    namespace Detail {

        template <int B, char... CS>
        struct BasicLiteral;

        template <int B, char C>
        struct BasicLiteral<B, C> {
            static_assert(
                (B <= 10 && C >= '0' && C <= '0' + B)
                || (B > 10 && (
                    (C >= '0' && C <= '9')
                    || (C >= 'A' && C <= 'A' + (B - 10))
                    || (C >= 'a' && C <= 'a' + (B - 10))
                )),
                "Invalid digit in integer literal"
            );
            static constexpr auto base = static_cast<unsigned long long>(B);
            static constexpr auto scale = base;
            static constexpr auto value = static_cast<unsigned long long>
                (C >= 'a' ? C - 'a' + 10 : C >= 'A' ? C - 'A' + 10 : C - '0');
        };

        template <int B, char C, char... CS>
        struct BasicLiteral<B, C, CS...> {
            using L = BasicLiteral<B, C>;
            using R = BasicLiteral<B, CS...>;
            static constexpr auto base = static_cast<unsigned long long>(B);
            static constexpr auto left = L::value;
            static constexpr auto right = R::value;
            static constexpr auto scale = base * R::scale;
            static constexpr auto value = R::scale * L::value + R::value;
        };

        template <int B, char... CS>
        struct BasicLiteral<B, '\'', CS...>:
        BasicLiteral<B, CS...> {};

        template <int B, char... CS>
        struct RadixLiteral:
        BasicLiteral<B, CS...> {};

        template <char... CS>
        struct RadixLiteral<0, CS...>:
        BasicLiteral<10, CS...> {};

        template <char... CS>
        struct RadixLiteral<0, '0', 'b', CS...>:
        BasicLiteral<2, CS...> {};

        template <char... CS>
        struct RadixLiteral<0, '0', 'B', CS...>:
        BasicLiteral<2, CS...> {};

        template <char... CS>
        struct RadixLiteral<0, '0', 'x', CS...>:
        BasicLiteral<16, CS...> {};

        template <char... CS>
        struct RadixLiteral<0, '0', 'X', CS...>:
        BasicLiteral<16, CS...> {};

        template <typename T, char... CS>
        struct CheckedLiteral {
            using TL = std::numeric_limits<T>;
            using PL = RadixLiteral<0, CS...>;
            static constexpr auto max = static_cast<unsigned long long>(TL::max());
            static_assert(PL::value <= max, "Integer literal value is out of range");
            static constexpr T value = static_cast<T>(PL::value);
        };

    }

    namespace Literals {

        template <char... CS> constexpr int8_t operator""_s8() noexcept { return Detail::CheckedLiteral<int8_t, CS...>::value; }
        template <char... CS> constexpr int16_t operator""_s16() noexcept { return Detail::CheckedLiteral<int16_t, CS...>::value; }
        template <char... CS> constexpr int32_t operator""_s32() noexcept { return Detail::CheckedLiteral<int32_t, CS...>::value; }
        template <char... CS> constexpr int64_t operator""_s64() noexcept { return Detail::CheckedLiteral<int64_t, CS...>::value; }
        template <char... CS> constexpr uint8_t operator""_u8() noexcept { return Detail::CheckedLiteral<uint8_t, CS...>::value; }
        template <char... CS> constexpr uint16_t operator""_u16() noexcept { return Detail::CheckedLiteral<uint16_t, CS...>::value; }
        template <char... CS> constexpr uint32_t operator""_u32() noexcept { return Detail::CheckedLiteral<uint32_t, CS...>::value; }
        template <char... CS> constexpr uint64_t operator""_u64() noexcept { return Detail::CheckedLiteral<uint64_t, CS...>::value; }
        template <char... CS> constexpr ptrdiff_t operator""_z() noexcept { return Detail::CheckedLiteral<ptrdiff_t, CS...>::value; }
        template <char... CS> constexpr size_t operator""_uz() noexcept { return Detail::CheckedLiteral<size_t, CS...>::value; }

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

    // Mixin classes

    template <typename T>
    concept UniqueCloneable = std::has_virtual_destructor_v<T>
        && requires (const T& obj) {
            { obj.clone() } -> std::convertible_to<std::unique_ptr<T>>;
        };

    template <typename T>
    concept SharedCloneable = std::has_virtual_destructor_v<T>
        && ! UniqueCloneable<T>
        && requires (const T& obj) {
            { obj.clone() } -> std::convertible_to<std::shared_ptr<T>>;
        };

    template <typename T>
    concept Cloneable = UniqueCloneable<T> || SharedCloneable<T>;

    template <typename T, typename U, Cloneable V = U>
    requires (std::derived_from<U, V>)
    class BasicClone:
    public U {
    private:
        using clone_return_type = std::conditional_t<UniqueCloneable<V>,
            std::unique_ptr<V>, std::shared_ptr<V>>;
    public:
        clone_return_type clone() const override {
            auto tptr = static_cast<const T*>(this);
            if constexpr (UniqueCloneable<V>)
                return std::make_unique<T>(*tptr);
            else
                return std::make_shared<T>(*tptr);
        }
    };

}
