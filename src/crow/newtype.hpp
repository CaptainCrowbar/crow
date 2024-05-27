#pragma once

#include "crow/binary.hpp"
#include "crow/format.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <functional>
#include <istream>
#include <ostream>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename T> struct ReferenceType: std::false_type {};
        template <typename T> struct ReferenceType<T&>: std::true_type {};
        template <typename T> concept Reference = ReferenceType<T>::value;
        template <typename T> concept NonReference = ! ReferenceType<T>::value;

        template <typename T> struct MutableReferenceType: std::false_type {};
        template <typename T> struct MutableReferenceType<T&>: std::true_type {};
        template <typename T> struct MutableReferenceType<const T&>: std::false_type {};
        template <typename T> concept MutableReference = MutableReferenceType<T>::value;

    }

    template <int N> using Ntag = std::integral_constant<int, N>;

    CROW_ENUM_BITMASK(Ntype, uint8_t,
        none       = 0,
        compare    = 1,
        construct  = 2,
        convert    = 4,
    )

    template <typename T, typename Tag = void, Ntype Flags = Ntype::none>
    class Newtype {

    private:

        static constexpr bool compare_equal = std::equality_comparable<T>;
        static constexpr bool compare_ordered = std::totally_ordered<T>;
        static constexpr bool compare_3way = requires (T t, T u) { { t <=> u }; };
        static constexpr bool compare_heterogeneous = has_bit(Flags, Ntype::compare);
        static constexpr bool implicit_construct = has_bit(Flags, Ntype::construct);
        static constexpr bool implicit_convert = has_bit(Flags, Ntype::convert);

        T value_;

    public:

        // Parameter visibility

        using underlying_type = T;
        using tag_type = Tag;

        static constexpr Ntype flags = Flags;

        // Life cycle operations

        template <bool OK = std::default_initializable<T>, std::enable_if_t<OK, int> = 0>
            Newtype() {}

        template <bool OK = std::copyable<T>, std::enable_if_t<OK, int> = 0>
            Newtype(const Newtype& nt): value_(nt.value_) {}
        template <bool OK = std::movable<T>, std::enable_if_t<OK, int> = 0>
            Newtype(Newtype&& nt): value_(std::move(nt.value_)) {}
        template <bool OK = std::copyable<T>, std::enable_if_t<OK, int> = 0>
            Newtype& operator=(const Newtype& nt) { value_ = nt.value_; return *this; }
        template <bool OK = std::movable<T>, std::enable_if_t<OK, int> = 0>
            Newtype& operator=(Newtype&& nt) { value_ = std::move(nt.value_); return *this; }

        template <bool OK = std::copyable<T>, std::enable_if_t<OK, int> = 0>
            explicit(! implicit_construct) Newtype(const T& t): value_(t) {}
        template <bool OK = std::movable<T>, std::enable_if_t<OK, int> = 0>
            explicit(! implicit_construct) Newtype(T&& t): value_(std::move(t)) {}
        template <bool OK = implicit_construct && std::copyable<T>, std::enable_if_t<OK, int> = 0>
            Newtype& operator=(const T& t) { value_ = t; return *this; }
        template <bool OK = implicit_construct && std::movable<T>, std::enable_if_t<OK, int> = 0>
            Newtype& operator=(T&& t) { value_ = std::move(t); return *this; }

        template <typename Tag2, Ntype F2, bool OK = std::copyable<T>, std::enable_if_t<OK, int> = 0>
            explicit Newtype(const Newtype<T, Tag2, F2>& nt): value_(nt.value_) {}
        template <typename Tag2, Ntype F2, bool OK = std::movable<T>, std::enable_if_t<OK, int> = 0>
            explicit Newtype(Newtype<T, Tag2, F2>&& nt): value_(std::move(nt.value_)) {}

        template <typename... TS, std::enable_if_t<std::constructible_from<T, TS...>, int> = 0>
            explicit Newtype(TS&&... args): value_(std::forward<TS>(args)...) {}

        // Conversion operators

        explicit(! implicit_convert) operator T() const { return value_; }

        template <bool OK = std::constructible_from<bool, T>, std::enable_if_t<OK, int> = 0>
            explicit(! std::convertible_to<bool, T>) operator bool() const { return bool(value_); }

        // Access operators

        T& operator*() noexcept { return value_; }
        const T& operator*() const noexcept { return value_; }
        T* operator->() noexcept { return &value_; }
        const T* operator->() const noexcept { return &value_; }

        // Arithmetic operators

        template <bool OK = requires (T t) { { + t } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            Newtype operator+() const { return Newtype(+ value_); }
        template <bool OK = requires (T t) { { - t } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            Newtype operator-() const { return Newtype(- value_); }

        template <bool OK = requires (T& t) { { ++ t }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator++() { ++ value_; return *this; }
        template <bool OK = requires (T& t) { { t ++ }; }, std::enable_if_t<OK, int> = 0>
            Newtype operator++(int) { auto old = *this; value_ ++; return old; }
        template <bool OK = requires (T& t) { { -- t }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator--() { -- value_; return *this; }
        template <bool OK = requires (T& t) { { t -- }; }, std::enable_if_t<OK, int> = 0>
            Newtype operator--(int) { auto old = *this; value_ --; return old; }

        template <bool OK = requires (T& t, T u) { { t += u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator+=(const Newtype& y) { value_ += y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t -= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator-=(const Newtype& y) { value_ -= y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t *= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator*=(const Newtype& y) { value_ *= y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t /= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator/=(const Newtype& y) { value_ /= y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t %= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator%=(const Newtype& y) { value_ %= y.value_; return *this; }

        template <bool OK = requires (T t, T u) { { t + u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator+(const Newtype& x, const Newtype& y) { return Newtype(x.value_ + y.value_); }
        template <bool OK = requires (T t, T u) { { t - u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator-(const Newtype& x, const Newtype& y) { return Newtype(x.value_ - y.value_); }
        template <bool OK = requires (T t, T u) { { t * u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator*(const Newtype& x, const Newtype& y) { return Newtype(x.value_ * y.value_); }
        template <bool OK = requires (T t, T u) { { t / u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator/(const Newtype& x, const Newtype& y) { return Newtype(x.value_ / y.value_); }
        template <bool OK = requires (T t, T u) { { t % u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator%(const Newtype& x, const Newtype& y) { return Newtype(x.value_ % y.value_); }

        // Bitwise operators

        template <bool OK = requires (T t) { { ~ t } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            Newtype operator~() const { return Newtype(~ value_); }

        template <bool OK = requires (T& t, T u) { { t &= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator&=(const Newtype& y) { value_ &= y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t |= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator|=(const Newtype& y) { value_ |= y.value_; return *this; }
        template <bool OK = requires (T& t, T u) { { t ^= u }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator^=(const Newtype& y) { value_ ^= y.value_; return *this; }
        template <bool OK = requires (T& t, int i) { { t <<= i }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator<<=(int y) { value_ <<= y; return *this; }
        template <bool OK = requires (T& t, int i) { { t >>= i }; }, std::enable_if_t<OK, int> = 0>
            Newtype& operator>>=(int y) { value_ >>= y; return *this; }

        template <bool OK = requires (T t, T u) { { t & u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator&(const Newtype& x, const Newtype& y) { return Newtype(x.value_ & y.value_); }
        template <bool OK = requires (T t, T u) { { t | u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator|(const Newtype& x, const Newtype& y) { return Newtype(x.value_ | y.value_); }
        template <bool OK = requires (T t, T u) { { t ^ u } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator^(const Newtype& x, const Newtype& y) { return Newtype(x.value_ ^ y.value_); }
        template <bool OK = requires (T t, int i) { { t << i } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator<<(const Newtype& x, int y) { return Newtype(x.value_ << y); }
        template <bool OK = requires (T t, int i) { { t >> i } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            friend Newtype operator>>(const Newtype& x, int y) { return Newtype(x.value_ >> y); }

        // Comparison operators

        template <bool OK = compare_equal, std::enable_if_t<OK, int> = 0>
            friend bool operator==(const Newtype& x, const Newtype& y) { return x.value_ == y.value_; }
        template <bool OK = compare_equal, std::enable_if_t<OK, int> = 0>
            friend bool operator!=(const Newtype& x, const Newtype& y) { return ! (x == y); }
        template <bool OK = compare_ordered, std::enable_if_t<OK, int> = 0>
            friend bool operator<(const Newtype& x, const Newtype& y) { return x.value_ < y.value_; }
        template <bool OK = compare_ordered, std::enable_if_t<OK, int> = 0>
            friend bool operator>(const Newtype& x, const Newtype& y) { return y < x; }
        template <bool OK = compare_ordered, std::enable_if_t<OK, int> = 0>
            friend bool operator<=(const Newtype& x, const Newtype& y) { return ! (y < x); }
        template <bool OK = compare_ordered, std::enable_if_t<OK, int> = 0>
            friend bool operator>=(const Newtype& x, const Newtype& y) { return ! (x < y); }
        template <bool OK = compare_3way, std::enable_if_t<OK, int> = 0>
            friend auto operator<=>(const Newtype& x, const Newtype& y) { return x.value_ <=> y.value_; }

        template <bool OK = compare_equal && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator==(const Newtype& x, const T& y) { return x.value_ == y; }
        template <bool OK = compare_equal && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator!=(const Newtype& x, const T& y) { return ! (x == y); }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator<(const Newtype& x, const T& y) { return x.value_ < y; }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator>(const Newtype& x, const T& y) { return y < x; }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator<=(const Newtype& x, const T& y) { return ! (y < x); }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator>=(const Newtype& x, const T& y) { return ! (x < y); }
        template <bool OK = compare_3way && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend auto operator<=>(const Newtype& x, const T& y) { return x.value_ <=> y; }

        template <bool OK = compare_equal && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator==(const T& x, const Newtype& y) { return x == y.value_; }
        template <bool OK = compare_equal && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator!=(const T& x, const Newtype& y) { return ! (x == y); }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator<(const T& x, const Newtype& y) { return x < y.value_; }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator>(const T& x, const Newtype& y) { return y < x; }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator<=(const T& x, const Newtype& y) { return ! (y < x); }
        template <bool OK = compare_ordered && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend bool operator>=(const T& x, const Newtype& y) { return ! (x < y); }
        template <bool OK = compare_3way && compare_heterogeneous, std::enable_if_t<OK, int> = 0>
            friend auto operator<=>(const T& x, const Newtype& y) { return x <=> y.value_; }

        // Formatting functions

        template <bool OK = requires (T& t, std::istream& in) { { in >> t }; }, std::enable_if_t<OK, int> = 0>
            friend std::istream& operator>>(std::istream& in, Newtype& x) { in >> x.value_; return in; }

        template <bool OK = FixedFormatType<T>, std::enable_if_t<OK, int> = 0>
            friend std::ostream& operator<<(std::ostream& out, const Newtype& x) {
                if constexpr (requires (const T& t, std::ostream& out) { { out << t }; })
                    out << x.value_;
                else
                    out << format_object(x.value_);
                return out;
            }

        template <bool OK = VariableFormatType<T>, std::enable_if_t<OK, int> = 0>
            std::string str(const FormatSpec& spec) const { return format_object(value_, spec); }
        template <bool OK = FixedFormatType<T>, std::enable_if_t<OK, int> = 0>
            std::string str() const { return format_object(value_); }

        // Hash functions

        template <bool OK = Hashable<T>, std::enable_if_t<OK, int> = 0>
            size_t hash() const noexcept { return std::hash<T>()(value_); }

        // Range access

        template <bool OK = requires (T& t) { { t[0] } -> Detail::MutableReference; }, std::enable_if_t<OK, int> = 0>
            auto& operator[](size_t i) { return value_[i]; }
        template <bool OK = requires (const T& t) { { t[0] } -> Detail::Reference; }, std::enable_if_t<OK, int> = 0>
            const auto& operator[](size_t i) const { return value_[i]; }
        template <bool OK = requires (const T& t) { { t[0] } -> Detail::NonReference; }, std::enable_if_t<OK, int> = 0>
            auto operator[](size_t i) const { return value_[i]; }

        template <bool OK = requires (T& t) { { std::ranges::begin(t) }; }, std::enable_if_t<OK, int> = 0>
            auto begin() { return std::ranges::begin(value_); }
        template <bool OK = requires (const T& t) { { std::ranges::begin(t) }; }, std::enable_if_t<OK, int> = 0>
            auto begin() const { return std::ranges::begin(value_); }
        template <bool OK = requires (T& t) { { std::ranges::end(t) }; }, std::enable_if_t<OK, int> = 0>
            auto end() { return std::ranges::end(value_); }
        template <bool OK = requires (const T& t) { { std::ranges::end(t) }; }, std::enable_if_t<OK, int> = 0>
            auto end() const { return std::ranges::end(value_); }

        template <bool OK = requires (const T& t) { { std::ranges::size(t) }; }, std::enable_if_t<OK, int> = 0>
            size_t size() const { return size_t(std::ranges::size(value_)); }
        template <bool OK = requires (const T& t) { { std::ranges::empty(t) }; }, std::enable_if_t<OK, int> = 0>
            bool empty() const { return std::ranges::empty(value_); }

        // String functions

        template <bool OK = requires (const T& t) { { t.substr(0, 1) } -> std::convertible_to<T>; }, std::enable_if_t<OK, int> = 0>
            Newtype substr(size_t pos, size_t len = npos) const { return Newtype(value_.substr(pos, len)); }

    };

}

CROW_STD_HASH_3(Newtype, typename, typename, Crow::Ntype)
