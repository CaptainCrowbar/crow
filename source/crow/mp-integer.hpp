#pragma once

#include "crow/binary.hpp"
#include "crow/format.hpp"
#include "crow/random.hpp"
#include "crow/rational.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <compare>
#include <cstdlib>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    // Unsigned integer class

    class MPN {

    public:

        MPN() = default;
        MPN(uint64_t x);
        explicit MPN(std::string_view s, int base = 0) { init(s, base); }

        template <typename T> explicit operator T() const;
        explicit operator bool() const noexcept { return ! rep_.empty(); }

        bool operator!() const noexcept { return ! bool(*this); }
        MPN operator+() const { return *this; }
        MPN& operator++() { return *this += 1; }
        MPN operator++(int) { auto x = *this; ++*this; return x; }
        MPN& operator--() { return *this -= 1; }
        MPN operator--(int) { auto x = *this; --*this; return x; }
        MPN& operator+=(const MPN& rhs);
        MPN& operator-=(const MPN& rhs);
        MPN& operator*=(const MPN& rhs) { MPN z; do_multiply(*this, rhs, z); rep_.swap(z.rep_); return *this; }
        MPN& operator/=(const MPN& rhs) { MPN q, r; do_divide(*this, rhs, q, r); rep_.swap(q.rep_); return *this; }
        MPN& operator%=(const MPN& rhs) { MPN q, r; do_divide(*this, rhs, q, r); rep_.swap(r.rep_); return *this; }
        MPN& operator&=(const MPN& rhs);
        MPN& operator|=(const MPN& rhs);
        MPN& operator^=(const MPN& rhs);
        MPN& operator<<=(ptrdiff_t rhs);
        MPN& operator>>=(ptrdiff_t rhs);

        size_t bits() const noexcept;
        size_t bits_set() const noexcept;
        size_t bytes() const noexcept;
        bool get_bit(size_t i) const noexcept;
        uint8_t get_byte(size_t i) const noexcept;
        void set_bit(size_t i, bool b = true);
        void set_byte(size_t i, uint8_t b);
        void flip_bit(size_t i);
        size_t hash() const noexcept { return hash_mix(rep_); }
        bool is_even() const noexcept { return rep_.empty() || (rep_.front() & 1) == 0; }
        bool is_odd() const noexcept { return ! is_even(); }
        MPN pow(const MPN& n) const;
        int sign() const noexcept { return int(bool(*this)); }
        std::string str(FormatSpec spec = {}) const;
        void write_be(void* ptr, size_t n) const noexcept;
        void write_le(void* ptr, size_t n) const noexcept;

        static MPN from_double(double x);
        static MPN read_be(const void* ptr, size_t n);
        static MPN read_le(const void* ptr, size_t n);

        friend MPN operator*(const MPN& lhs, const MPN& rhs) { MPN z; MPN::do_multiply(lhs, rhs, z); return z; }
        friend MPN operator/(const MPN& lhs, const MPN& rhs) { MPN q, r; MPN::do_divide(lhs, rhs, q, r); return q; }
        friend MPN operator%(const MPN& lhs, const MPN& rhs) { MPN q, r; MPN::do_divide(lhs, rhs, q, r); return r; }
        friend bool operator==(const MPN& lhs, const MPN& rhs) noexcept { return lhs.compare(rhs) == 0; }
        friend auto operator<=>(const MPN& lhs, const MPN& rhs) noexcept { return lhs.compare(rhs); }

        friend std::pair<MPN, MPN> divide(const MPN& lhs, const MPN& rhs) { MPN q, r; MPN::do_divide(lhs, rhs, q, r); return {q, r}; }

    private:

        friend class MPZ;

        static constexpr auto mask32 = ~ uint32_t(0);

        std::vector<uint32_t> rep_; // Least significant word first

        std::strong_ordering compare(const MPN& rhs) const noexcept;
        void init(std::string_view s, int base);
        void trim() noexcept;

        static void do_divide(const MPN& x, const MPN& y, MPN& q, MPN& r);
        static void do_multiply(const MPN& x, const MPN& y, MPN& z);

    };

        inline MPN operator+(const MPN& lhs, const MPN& rhs) { auto z = lhs; z += rhs; return z; }
        inline MPN operator-(const MPN& lhs, const MPN& rhs) { auto z = lhs; z -= rhs; return z; }
        inline MPN operator&(const MPN& lhs, const MPN& rhs) { auto z = lhs; z &= rhs; return z; }
        inline MPN operator|(const MPN& lhs, const MPN& rhs) { auto z = lhs; z |= rhs; return z; }
        inline MPN operator^(const MPN& lhs, const MPN& rhs) { auto z = lhs; z ^= rhs; return z; }
        inline MPN operator<<(const MPN& lhs, size_t rhs) { auto z = lhs; z <<= rhs; return z; }
        inline MPN operator>>(const MPN& lhs, size_t rhs) { auto z = lhs; z >>= rhs; return z; }
        inline std::string to_string(const MPN& x) { return x.str(); }
        inline std::ostream& operator<<(std::ostream& out, const MPN& x) { return out << x.str(); }

        template <typename T>
        MPN::operator T() const {
            T t = 0;
            int bit = 0;
            for (auto w: rep_) {
                if constexpr (std::is_floating_point_v<T>) {
                    t += std::ldexp(T(w), bit);
                } else {
                    if (bit >= int(8 * sizeof(T)))
                        break;
                    t += T(w) << bit;
                }
                bit += 32;
            }
            return t;
        }

    // Signed integer class

    class MPZ {

    public:

        MPZ() = default;
        MPZ(int64_t x): mag_(uint64_t(std::abs(x))), neg_(x < 0) {}
        MPZ(const MPN& x): mag_(x), neg_(false) {}
        explicit MPZ(std::string_view s, int base = 0) { init(s, base); }

        template <typename T> explicit operator T() const;
        explicit operator MPN() const { return mag_; }
        explicit operator bool() const noexcept { return bool(mag_); }

        bool operator!() const noexcept { return ! bool(*this); }
        MPZ operator+() const { return *this; }
        MPZ operator-() const { MPZ z = *this; if (z) z.neg_ = ! z.neg_; return z; }
        MPZ& operator++() { return *this += 1; }
        MPZ operator++(int) { auto x = *this; ++*this; return x; }
        MPZ& operator--() { return *this -= 1; }
        MPZ operator--(int) { auto x = *this; --*this; return x; }
        MPZ& operator+=(const MPZ& rhs);
        MPZ& operator-=(const MPZ& rhs) { return *this += - rhs; }
        MPZ& operator*=(const MPZ& rhs) { MPZ z; do_multiply(*this, rhs, z); std::swap(*this, z); return *this; }
        MPZ& operator/=(const MPZ& rhs) { MPZ q, r; do_divide(*this, rhs, q, r); std::swap(*this, q); return *this; }
        MPZ& operator%=(const MPZ& rhs) { MPZ q, r; do_divide(*this, rhs, q, r); std::swap(*this, r); return *this; }

        MPN abs() const { return mag_; }
        size_t hash() const noexcept { return hash_mix(mag_, neg_); }
        bool is_even() const noexcept { return mag_.is_even(); }
        bool is_odd() const noexcept { return mag_.is_odd(); }
        MPZ pow(const MPZ& n) const;
        int sign() const noexcept { return neg_ ? -1 : mag_.sign(); }
        std::string str(FormatSpec spec = {}) const;

        static MPZ from_double(double x);

        friend MPZ operator*(const MPZ& lhs, const MPZ& rhs) { MPZ z; MPZ::do_multiply(lhs, rhs, z); return z; }
        friend MPZ operator/(const MPZ& lhs, const MPZ& rhs) { MPZ q, r; MPZ::do_divide(lhs, rhs, q, r); return q; }
        friend MPZ operator%(const MPZ& lhs, const MPZ& rhs) { MPZ q, r; MPZ::do_divide(lhs, rhs, q, r); return r; }
        friend bool operator==(const MPZ& lhs, const MPZ& rhs) noexcept { return lhs.compare(rhs) == 0; }
        friend auto operator<=>(const MPZ& lhs, const MPZ& rhs) noexcept { return lhs.compare(rhs); }
        friend std::pair<MPZ, MPZ> divide(const MPZ& lhs, const MPZ& rhs) { MPZ q, r; MPZ::do_divide(lhs, rhs, q, r); return {q, r}; }

    private:

        MPN mag_;
        bool neg_ = false;

        std::strong_ordering compare(const MPZ& rhs) const noexcept;
        void init(std::string_view s, int base);

        static void do_divide(const MPZ& x, const MPZ& y, MPZ& q, MPZ& r);
        static void do_multiply(const MPZ& x, const MPZ& y, MPZ& z);

    };

        using MPQ = Ratio<MPZ>;

        inline MPZ operator+(const MPZ& lhs, const MPZ& rhs) { auto z = lhs; z += rhs; return z; }
        inline MPZ operator-(const MPZ& lhs, const MPZ& rhs) { auto z = lhs; z -= rhs; return z; }
        inline std::string to_string(const MPZ& x) { return x.str(); }
        inline std::ostream& operator<<(std::ostream& out, const MPZ& x) { return out << x.str(); }

        template <typename T>
        MPZ::operator T() const {
            auto t = T(mag_);
            if (neg_)
                t = T(0) - t;
            return t;
        }

    // Literals

    namespace Literals {

        inline MPZ operator""_mpi(const char* raw) { return MPZ(raw); }
        inline MPN operator""_mpu(const char* raw) { return MPN(raw); }

    }

}

namespace std {

    template <>
    struct hash<Crow::MPZ> {
    public:
        size_t operator()(const Crow::MPZ& x) const noexcept {
            return x.hash();
        }
    };

    template <>
    struct hash<Crow::MPN> {
    public:
        size_t operator()(const Crow::MPN& x) const noexcept {
            return x.hash();
        }
    };

}
