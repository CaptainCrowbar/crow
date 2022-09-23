#pragma once

#include "crow/log.hpp"
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename T, typename = void> struct HasAddAssignOperator: std::false_type {};
        template <typename T> struct HasAddAssignOperator<T, std::void_t<decltype(std::declval<T&>() += std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasSubtractAssignOperator: std::false_type {};
        template <typename T> struct HasSubtractAssignOperator<T, std::void_t<decltype(std::declval<T&>() -= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasMultiplyAssignOperator: std::false_type {};
        template <typename T> struct HasMultiplyAssignOperator<T, std::void_t<decltype(std::declval<T&>() *= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasDivideAssignOperator: std::false_type {};
        template <typename T> struct HasDivideAssignOperator<T, std::void_t<decltype(std::declval<T&>() /= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasRemainderAssignOperator: std::false_type {};
        template <typename T> struct HasRemainderAssignOperator<T, std::void_t<decltype(std::declval<T&>() %= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasBitwiseAndAssignOperator: std::false_type {};
        template <typename T> struct HasBitwiseAndAssignOperator<T, std::void_t<decltype(std::declval<T&>() &= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasBitwiseOrAssignOperator: std::false_type {};
        template <typename T> struct HasBitwiseOrAssignOperator<T, std::void_t<decltype(std::declval<T&>() |= std::declval<T>())>>: std::true_type {};
        template <typename T, typename = void> struct HasBitwiseXorAssignOperator: std::false_type {};
        template <typename T> struct HasBitwiseXorAssignOperator<T, std::void_t<decltype(std::declval<T&>() ^= std::declval<T>())>>: std::true_type {};

        template <typename T, typename = void> struct HasIteratorCategory: std::false_type {};
        template <typename T> struct HasIteratorCategory<T, std::void_t<typename std::iterator_traits<T>::iterator_category>>: std::true_type {};
        template <> struct HasIteratorCategory<void*>: std::false_type {};
        template <> struct HasIteratorCategory<const void*>: std::false_type {};

        template <typename T, typename = void> struct HasAdlBeginFunction: std::false_type {};
        template <typename T> struct HasAdlBeginFunction<T, std::void_t<decltype(begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasAdlEndFunction: std::false_type {};
        template <typename T> struct HasAdlEndFunction<T, std::void_t<decltype(end(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdBeginFunction: std::false_type {};
        template <typename T> struct HasStdBeginFunction<T, std::void_t<decltype(std::begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdEndFunction: std::false_type {};
        template <typename T> struct HasStdEndFunction<T, std::void_t<decltype(std::end(std::declval<const T&>()))>>: std::true_type {};

        template <typename T, bool UseAdl = HasAdlBeginFunction<T>::value, bool UseStd = HasStdBeginFunction<T>::value> struct RangeTraits {
            using iterator_type = void;
            using value_type = void;
        };
        template <typename T, bool UseStd> struct RangeTraits<T, true, UseStd> {
            using iterator_type = std::decay_t<decltype(begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };
        template <typename T> struct RangeTraits<T, false, true> {
            using iterator_type = std::decay_t<decltype(std::begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };

        template <typename T, bool IsIterator = HasIteratorCategory<T>::value> struct AltIteratorCategory { using type = void; };
        template <typename T> struct AltIteratorCategory<T, true> { using type = typename std::iterator_traits<T>::iterator_category; };

        template <typename T, typename = void> struct HasFirstMember: std::false_type {};
        template <typename T> struct HasFirstMember<T, std::void_t<decltype(std::declval<T>().first)>>: std::true_type {};
        template <typename T, typename = void> struct HasSecondMember: std::false_type {};
        template <typename T> struct HasSecondMember<T, std::void_t<decltype(std::declval<T>().second)>>: std::true_type {};
        template <typename T> constexpr bool is_pairlike = HasFirstMember<T>::value && HasSecondMember<T>::value;

    }

    // Constants

    constexpr size_t npos = std::string::npos;

    // Memory management types

    struct FreeMem {
        void operator()(void* ptr) const {
            if (ptr != nullptr)
                std::free(ptr);
        }
    };

    // Mixin classes

    template <typename T>
    class BinaryOperators {
    public:
        template <typename U = T> friend T operator+(const T& a,
            const std::enable_if_t<Detail::HasAddAssignOperator<U>::value, T>& b)
                { T c = a; c += b; return c; }
        template <typename U = T> friend T operator-(const T& a,
            const std::enable_if_t<Detail::HasSubtractAssignOperator<U>::value, T>& b)
                { T c = a; c -= b; return c; }
        template <typename U = T> friend T operator*(const T& a,
            const std::enable_if_t<Detail::HasMultiplyAssignOperator<U>::value, T>& b)
                { T c = a; c *= b; return c; }
        template <typename U = T> friend T operator/(const T& a,
            const std::enable_if_t<Detail::HasDivideAssignOperator<U>::value, T>& b)
                { T c = a; c /= b; return c; }
        template <typename U = T> friend T operator%(const T& a,
            const std::enable_if_t<Detail::HasRemainderAssignOperator<U>::value, T>& b)
                { T c = a; c %= b; return c; }
        template <typename U = T> friend T operator&(const T& a,
            const std::enable_if_t<Detail::HasBitwiseAndAssignOperator<U>::value, T>& b)
                { T c = a; c &= b; return c; }
        template <typename U = T> friend T operator|(const T& a,
            const std::enable_if_t<Detail::HasBitwiseOrAssignOperator<U>::value, T>& b)
                { T c = a; c |= b; return c; }
        template <typename U = T> friend T operator^(const T& a,
            const std::enable_if_t<Detail::HasBitwiseXorAssignOperator<U>::value, T>& b)
                { T c = a; c ^= b; return c; }
    };

    template <typename T>
    class TotalOrder {
    public:
        friend bool operator!=(const T& a, const T& b) noexcept { return ! (a == b); }
        friend bool operator>(const T& a, const T& b) noexcept { return b < a; }
        friend bool operator<=(const T& a, const T& b) noexcept { return ! (b < a); }
        friend bool operator>=(const T& a, const T& b) noexcept { return ! (a < b); }
    };

    // SFINAE support

    template <typename T, bool B> struct SfinaeTrue {};
    template <typename T> struct SfinaeTrue<T, true>: std::true_type {};

    // Static assert support

    template <typename T> constexpr bool dependent_false = false;

    // Type traits

    template <typename T> constexpr bool is_iterator = Detail::HasIteratorCategory<T>::value;
    template <typename T> constexpr bool is_range = (Detail::HasAdlBeginFunction<T>::value && Detail::HasAdlEndFunction<T>::value)
        || (Detail::HasStdBeginFunction<T>::value && Detail::HasStdEndFunction<T>::value);

    template <typename T>
    constexpr bool is_input_iterator =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::input_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T>
    constexpr bool is_output_iterator =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::output_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T>
    constexpr bool is_forward_iterator =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T>
    constexpr bool is_bidirectional_iterator =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T>
    constexpr bool is_random_access_iterator =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;

    template <typename T> using RangeIterator = typename Detail::RangeTraits<T>::iterator_type;
    template <typename T> using RangeValue = typename Detail::RangeTraits<T>::value_type;

    template <typename T> constexpr bool is_input_range = is_input_iterator<RangeIterator<T>>;
    template <typename T> constexpr bool is_output_range = is_output_iterator<RangeIterator<T>>;
    template <typename T> constexpr bool is_forward_range = is_forward_iterator<RangeIterator<T>>;
    template <typename T> constexpr bool is_bidirectional_range = is_bidirectional_iterator<RangeIterator<T>>;
    template <typename T> constexpr bool is_random_access_range = is_random_access_iterator<RangeIterator<T>>;

    template <typename T> constexpr bool is_maplike_range = is_range<T> && Detail::is_pairlike<RangeValue<T>>;

}
