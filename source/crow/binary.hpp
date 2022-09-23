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
