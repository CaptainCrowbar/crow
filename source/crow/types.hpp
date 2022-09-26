#pragma once

#include "crow/log.hpp"
#include <compare>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

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

    namespace Detail {

        using SO = std::strong_ordering;

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

        // Xcode 14 brain damage

        template <typename T, typename = void> struct IsThreeWayComparable: std::false_type {};
        template <typename T> struct IsThreeWayComparable<T, std::void_t<decltype(std::declval<T>() <=> std::declval<T>())>>: std::true_type {};

    }

    // Related types

    template <typename T> using RangeIterator = typename Detail::RangeTraits<T>::iterator_type;
    template <typename T> using RangeValue = typename Detail::RangeTraits<T>::value_type;

    // Constants

    constexpr size_t npos = std::string::npos;

    // Concepts

    template <typename T> concept ArithmeticType = std::is_arithmetic_v<T> && ! std::is_same_v<T, bool>;

    template <typename T> concept IteratorType = Detail::HasIteratorCategory<T>::value;
    template <typename T> concept RangeType = (Detail::HasAdlBeginFunction<T>::value && Detail::HasAdlEndFunction<T>::value)
        || (Detail::HasStdBeginFunction<T>::value && Detail::HasStdEndFunction<T>::value);

    template <typename T> concept InputIteratorType =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::input_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T> concept OutputIteratorType =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::output_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T> concept ForwardIteratorType =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::forward_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T> concept BidirectionalIteratorType =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::bidirectional_iterator_tag>
        || std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;
    template <typename T> concept RandomAccessIteratorType =
        std::is_same_v<typename Detail::AltIteratorCategory<T>::type, std::random_access_iterator_tag>;

    template <typename T> concept InputRangeType = InputIteratorType<RangeIterator<T>>;
    template <typename T> concept OutputRangeType = OutputIteratorType<RangeIterator<T>>;
    template <typename T> concept ForwardRangeType = ForwardIteratorType<RangeIterator<T>>;
    template <typename T> concept BidirectionalRangeType = BidirectionalIteratorType<RangeIterator<T>>;
    template <typename T> concept RandomAccessRangeType = RandomAccessIteratorType<RangeIterator<T>>;
    template <typename T> concept MaplikeRangeType = RangeType<T> && Detail::is_pairlike<RangeValue<T>>;

    template <typename T> concept PrimitiveScalarType = std::is_scalar_v<T>;
    template <typename T, typename U> concept SameBasicType = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
    template <typename T> concept ThreeWayComparable = Detail::IsThreeWayComparable<T>::value;

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
        void operator()(void* ptr) const {
            if (ptr != nullptr)
                std::free(ptr);
        }
    };

    // Metaprogramming support

    template <typename T, bool B> struct SfinaeTrue {};
    template <typename T> struct SfinaeTrue<T, true>: std::true_type {};

    template <typename T> constexpr bool dependent_false = false;

}
