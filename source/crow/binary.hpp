#pragma once

#include "crow/types.hpp"
#include <bit>
#include <cstring>
#include <limits>
#include <optional>
#include <type_traits>

namespace Crow {

    // Byte order

    template <typename T>
    constexpr T swap_ends(T t, size_t N = sizeof(T)) noexcept {
        static_assert(std::is_scalar_v<T>);
        using U = std::make_unsigned_t<T>;
        return N == 1 ? t :
            T(
                (
                    swap_ends(
                        U(t) & (
                            (U(1) << (4 * N)) - 1
                        ), N / 2
                    ) << (4 * N)
                ) | swap_ends(
                    U(t) >> (4 * N), N / 2
                )
            );
    }

    template <typename T>
    constexpr T big_endian(T t) noexcept {
        static_assert(std::is_scalar_v<T>);
        if constexpr (std::endian::native == std::endian::big)
            return t;
        else
            return swap_ends(t);
    }

    template <typename T>
    constexpr T little_endian(T t) noexcept {
        static_assert(std::is_scalar_v<T>);
        if constexpr (std::endian::native == std::endian::big)
            return swap_ends(t);
        else
            return t;
    }

    template <typename T>
    constexpr T read_be(const void* ptr) noexcept {
        static_assert(std::is_scalar_v<T>);
        T t;
        std::memcpy(&t, ptr, sizeof(T));
        return big_endian(t);
    }

    template <typename T>
    constexpr T read_le(const void* ptr) noexcept {
        static_assert(std::is_scalar_v<T>);
        T t;
        std::memcpy(&t, ptr, sizeof(T));
        return little_endian(t);
    }

    template <typename T>
    constexpr void write_be(T t, void* ptr) noexcept {
        static_assert(std::is_scalar_v<T>);
        T u = big_endian(t);
        std::memcpy(ptr, &u, sizeof(T));
    }

    template <typename T>
    constexpr void write_le(T t, void* ptr) noexcept {
        static_assert(std::is_scalar_v<T>);
        T u = little_endian(t);
        std::memcpy(ptr, &u, sizeof(T));
    }

    // Bitwise operations

    #ifdef __GNUC__

        template <typename T>
        constexpr int popcount(T t) noexcept {
            static_assert(std::is_integral_v<T>);
            if constexpr (sizeof(T) <= sizeof(int))
                return __builtin_popcount(static_cast<unsigned>(t));
            else if constexpr (sizeof(T) <= sizeof(long))
                return __builtin_popcountl(static_cast<unsigned long>(t));
            else
                return __builtin_popcountll(static_cast<unsigned long long>(t));
        }

        template <typename T>
        constexpr int bit_width(T t) noexcept {
            static_assert(std::is_integral_v<T>);
            if (t == 0)
                return 0;
            if constexpr (sizeof(T) <= sizeof(int))
                return 8 * sizeof(unsigned) - __builtin_clz(static_cast<unsigned>(t));
            else if constexpr (sizeof(T) <= sizeof(long))
                return 8 * sizeof(unsigned long) - __builtin_clzl(static_cast<unsigned long>(t));
            else
                return 8 * sizeof(unsigned long long) - __builtin_clzll(static_cast<unsigned long long>(t));
        }

    #else

        template <typename T>
        constexpr int popcount(T t) noexcept {
            static_assert(std::is_integral_v<T>);
            constexpr int8_t bits16[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
            int n = 0;
            for (; t; t >>= 4) { n += bits16[t & 0xf]; }
            return n;
        }

        template <typename T>
        constexpr int bit_width(T t) noexcept {
            static_assert(std::is_integral_v<T>);
            int n = 0;
            for (; t > 0xff; t >>= 8) { n += 8; }
            for (; t; t >>= 1) { ++n; }
            return n;
        }

    #endif

    template <typename T>
    constexpr T bit_ceil(T t) noexcept {
        static_assert(std::is_integral_v<T>);
        return t > 1 ? T(1) << (bit_width(t - 1) - 1) << 1 : t;
    }

    template <typename T>
    constexpr T bit_floor(T t) noexcept {
        static_assert(std::is_integral_v<T>);
        return t ? T(1) << (bit_width(t) - 1) : 0;
    }

    template <typename T>
    constexpr bool has_single_bit(T t) noexcept {
        static_assert(std::is_integral_v<T>);
        return popcount(t) == 1;
    }

    namespace Detail {

        template <typename T>
        constexpr T rotl_helper(T t, int n) noexcept {
            static_assert(std::is_integral_v<T>);
            constexpr int tbits = 8 * sizeof(T);
            int nbits = n % tbits;
            return nbits == 0 ? t : T(t << nbits) | T(t >> (tbits - nbits));
        }

        template <typename T>
        constexpr T rotr_helper(T t, int n) noexcept {
            static_assert(std::is_integral_v<T>);
            constexpr int tbits = 8 * sizeof(T);
            int nbits = n % tbits;
            return nbits == 0 ? t : T(t >> nbits) | T(t << (tbits - nbits));
        }

    }

    template <typename T>
    constexpr T rotl(T t, int n) noexcept {
        static_assert(std::is_integral_v<T>);
        return n == 0 ? t : n < 0 ? Detail::rotr_helper(t, - n) : Detail::rotl_helper(t, n);
    }

    template <typename T>
    constexpr T rotr(T t, int n) noexcept {
        static_assert(std::is_integral_v<T>);
        return n == 0 ? t : n < 0 ? Detail::rotl_helper(t, - n) : Detail::rotr_helper(t, n);
    }

    // Overflow detection

    template <typename T>
    constexpr std::optional<T> checked_add(T x, T y) noexcept {
        static_assert(std::is_integral_v<T>);
        using limits = std::numeric_limits<T>;
        if (x > 0 && y > limits::max() - x)
            return {};
        if constexpr (std::is_signed_v<T>)
            if (x < 0 && y < limits::min() - x)
                return {};
        return T(x + y);
    }

    template <typename T>
    constexpr std::optional<T> checked_subtract(T x, T y) noexcept {
        static_assert(std::is_integral_v<T>);
        using limits = std::numeric_limits<T>;
        if constexpr (std::is_signed_v<T>) {
            if (y != limits::min())
                return checked_add(x, T(- y));
            else if (x <= 0)
                return {};
        } else {
            if (x < y)
                return {};
        }
        return T(x - y);
    }

}
