#pragma once

#include "crow/fixed-binary.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <array>
#include <compare>
#include <ostream>
#include <string>

namespace Crow {

    class Uuid {

    public:

        constexpr Uuid() noexcept: bytes_{} { for (auto& b: bytes_) b = 0; }
        constexpr Uuid(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
            uint8_t i, uint8_t j, uint8_t k, uint8_t l, uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept;
        constexpr Uuid(const void* ptr, size_t len) noexcept;
        constexpr explicit Uuid(Uint128 u) noexcept: Uuid(u.data(), 16) {}
        explicit Uuid(const std::string& str);

        constexpr uint8_t& operator[](size_t i) noexcept { return bytes_[i]; }
        constexpr const uint8_t& operator[](size_t i) const noexcept { return bytes_[i]; }
        constexpr uint8_t* begin() noexcept { return bytes_.data(); }
        constexpr const uint8_t* begin() const noexcept { return bytes_.data(); }
        constexpr uint8_t* end() noexcept { return begin() + 16; }
        constexpr const uint8_t* end() const noexcept { return begin() + 16; }

        constexpr Uint128 as_uint() const noexcept;
        constexpr size_t hash() const noexcept { return BernsteinHash()(begin(), 16); }
        std::string str() const;

        constexpr explicit operator Uint128() const noexcept { return as_uint(); }
        explicit operator std::string() const { return str(); }

    private:

        std::array<uint8_t, 16> bytes_;

        static void add_byte(uint8_t b, std::string& str);

        constexpr static bool is_alnum(char c) noexcept { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
        constexpr static bool is_xdigit(char c) noexcept { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }

    };

        constexpr Uuid::Uuid(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
            uint8_t i, uint8_t j, uint8_t k, uint8_t l, uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept:
        bytes_{{a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p}} {}

        constexpr Uuid::Uuid(const void* ptr, size_t len) noexcept:
        bytes_{} {
            auto bptr = static_cast<const uint8_t*>(ptr);
            if (len > 16)
                len = 16;
            for (size_t i = 0; i < len; ++i)
                bytes_[i] = bptr[i];
            for (size_t i = len; i < 16; ++i)
                bytes_[i] = 0;
        }

        constexpr Uint128 Uuid::as_uint() const noexcept {
            Uint128 u;
            for (size_t i = 0; i < 16; ++i)
                u.data()[i] = bytes_[i];
            return u;
        }

        constexpr bool operator==(const Uuid& u, const Uuid& v) noexcept {
            for (int i = 0; i < 16; ++i)
                if (u[i] != v[i])
                    return false;
            return true;
        }

        constexpr std::strong_ordering operator<=>(const Uuid& u, const Uuid& v) noexcept {
            for (int i = 0; i < 16; ++i) {
                auto c = u[i] <=> v[i];
                if (c != 0)
                    return c;
            }
            return std::strong_ordering::equal;
        }

        inline std::ostream& operator<<(std::ostream& out, const Uuid& u) { return out << u.str(); }

}

CROW_STD_HASH_0(Uuid)
