// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/binary.hpp"
#include "crow/fixed-binary.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <array>
#include <bit>
#include <compare>
#include <concepts>
#include <functional>
#include <random>
#include <type_traits>

namespace Crow {

    // Concepts

    template <typename T>
    concept RandomEngineType =
        std::unsigned_integral<typename T::result_type>
        && std::invocable<T&>
        && std::convertible_to<std::invoke_result_t<T&>, typename T::result_type>
        && requires (T t) {
            { t.min() } -> std::convertible_to<typename T::result_type>;
            { t.max() } -> std::convertible_to<typename T::result_type>;
        };

    template <typename T>
    concept RandomDistributionType =
        requires { typename T::result_type; }
        && std::invocable<T&, std::minstd_rand&>
        && std::invocable<T&, std::random_device&>
        && std::convertible_to<std::invoke_result_t<T&, std::minstd_rand&>, typename T::result_type>
        && std::convertible_to<std::invoke_result_t<T&, std::random_device&>, typename T::result_type>;

    template <typename T, typename RT>
    concept SpecificDistributionType =
        RandomDistributionType<T>
        && std::convertible_to<typename T::result_type, RT>;

    // Good LCG transformations for 32 and 64 bit integers
    // Pierre L'Ecuyer (1999), "Tables of Linear Congruential Generators of Different Sizes and Good Lattice Structure"
    // http://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf

    constexpr uint32_t lcg32(uint32_t x) noexcept {
        constexpr uint32_t m = 32'310'901ul;
        constexpr uint32_t c = 850'757'001ul;
        return uint32_t(m * x + c);
    }

    constexpr uint64_t lcg64(uint64_t x) noexcept {
        constexpr uint64_t m = 3'935'559'000'370'003'845ull;
        constexpr uint64_t c = 8'831'144'850'135'198'739ull;
        return uint64_t(m * x + c);
    }

    constexpr Uint128 lcg128(Uint128 x) noexcept {
        constexpr Uint128 m = {0x2360'ed05'1fc6'5da4ull, 0x4385'df64'9fcc'f645ull};
        constexpr Uint128 c = {0x55bf'e625'0318'f820ull, 0xe2d4'afe5'108d'a1e3ull};
        return m * x + c;
    }

    class Lcg32 {
    public:
        using result_type = uint32_t;
        constexpr Lcg32() noexcept {}
        explicit constexpr Lcg32(uint32_t s) noexcept: state_(s) {}
        constexpr uint32_t operator()() noexcept { state_ = lcg32(state_); return state_; }
        constexpr bool operator==(const Lcg32& rhs) const noexcept { return state_ == rhs.state_; }
        constexpr bool operator!=(const Lcg32& rhs) const noexcept { return state_ != rhs.state_; }
        constexpr void seed(uint32_t s) noexcept { state_ = s; }
        static constexpr uint32_t min() noexcept { return 0; }
        static constexpr uint32_t max() noexcept { return ~ uint32_t(0); }
    private:
        uint32_t state_ = 0;
    };

    class Lcg64 {
    public:
        using result_type = uint64_t;
        constexpr Lcg64() noexcept {}
        explicit constexpr Lcg64(uint64_t s) noexcept: state_(s) {}
        uint64_t constexpr operator()() noexcept { state_ = lcg64(state_); return state_; }
        bool constexpr operator==(const Lcg64& rhs) const noexcept { return state_ == rhs.state_; }
        bool constexpr operator!=(const Lcg64& rhs) const noexcept { return state_ != rhs.state_; }
        void constexpr seed(uint64_t s) noexcept { state_ = s; }
        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }
    private:
        uint64_t state_ = 0;
    };

    class Lcg128 {
    public:
        using result_type = Uint128;
        constexpr Lcg128() noexcept {}
        explicit constexpr Lcg128(Uint128 s) noexcept: state_(s) {}
        explicit constexpr Lcg128(uint64_t s, uint64_t t) noexcept: state_{s, t} {}
        Uint128 constexpr operator()() noexcept { state_ = lcg128(state_); return state_; }
        bool constexpr operator==(const Lcg128& rhs) const noexcept { return state_ == rhs.state_; }
        bool constexpr operator!=(const Lcg128& rhs) const noexcept { return state_ != rhs.state_; }
        void constexpr seed(Uint128 s) noexcept { state_ = s; }
        void constexpr seed(uint64_t s, uint64_t t) noexcept { state_ = {s, t}; }
        static constexpr Uint128 min() noexcept { return 0; }
        static constexpr Uint128 max() noexcept { return ~ Uint128(0); }
    private:
        Uint128 state_ = 0;
    };

    // Squirrel3 hash functions by Squirrel Eiserloh

    constexpr uint32_t squirrel32(uint32_t x) noexcept {
        constexpr uint32_t a = 0xb529'7a4dul;
        constexpr uint32_t b = 0x68e3'1da4ul;
        constexpr uint32_t c = 0x1b56'c4e9ul;
        x *= a;
        x ^= x >> 8;
        x += b;
        x ^= x << 8;
        x *= c;
        x ^= x >> 8;
        return x;
    }

    constexpr uint64_t squirrel64(uint64_t x) noexcept {
        constexpr uint64_t a = 0x9e37'79b1'85eb'ca87ull;
        constexpr uint64_t b = 0xc2b2'ae3d'27d4'eb4full;
        constexpr uint64_t c = 0x27d4'eb2f'1656'67c5ull;
        x *= a;
        x ^= x >> 8;
        x += b;
        x ^= x << 8;
        x *= c;
        x ^= x >> 8;
        return x;
    }

    class Squirrel32 {
    public:
        using result_type = uint32_t;
        constexpr Squirrel32() noexcept {}
        explicit constexpr Squirrel32(uint32_t s) noexcept: state_(s) {}
        constexpr uint32_t operator()() noexcept { state_ = squirrel32(state_); return state_; }
        constexpr bool operator==(const Squirrel32& rhs) const noexcept { return state_ == rhs.state_; }
        constexpr bool operator!=(const Squirrel32& rhs) const noexcept { return state_ != rhs.state_; }
        constexpr void seed(uint32_t s) noexcept { state_ = s; }
        static constexpr uint32_t min() noexcept { return 0; }
        static constexpr uint32_t max() noexcept { return ~ uint32_t(0); }
    private:
        uint32_t state_ = 0;
    };

    class Squirrel64 {
    public:
        using result_type = uint64_t;
        constexpr Squirrel64() noexcept {}
        explicit constexpr Squirrel64(uint64_t s) noexcept: state_(s) {}
        constexpr uint64_t operator()() noexcept { state_ = squirrel64(state_); return state_; }
        constexpr bool operator==(const Squirrel64& rhs) const noexcept { return state_ == rhs.state_; }
        constexpr bool operator!=(const Squirrel64& rhs) const noexcept { return state_ != rhs.state_; }
        constexpr void seed(uint64_t s) noexcept { state_ = s; }
        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }
    private:
        uint64_t state_ = 0;
    };

    // PCG generator by Melissa O'Neill
    // http://www.pcg-random.org/

    class Pcg64 {

    public:

        using result_type = uint64_t;

        constexpr Pcg64() noexcept: state_(default_seed_) { init(); }
        constexpr explicit Pcg64(uint64_t s) noexcept: state_(s) { init(); }
        constexpr explicit Pcg64(uint64_t hi, uint64_t lo) noexcept: state_{hi, lo} { init(); }

        constexpr uint64_t operator()() noexcept {
            state_ *= a_;
            state_ += b_;
            auto x = uint64_t((state_ >> 64) ^ state_);
            auto y = int(state_ >> 122) & 63;
            return std::rotr(x, y);
        }

        constexpr void advance(int64_t offset) noexcept {
            state_type u = uint64_t(offset);
            if (offset < 0)
                u |= ~ Uint128(0) << 64;
            state_type add;
            state_type mul = 1;
            auto c = a_;
            auto d = b_;
            while (u) {
                if (u & 1) {
                    mul *= c;
                    add *= c;
                    add += d;
                }
                auto c1 = c;
                ++c1;
                d *= c1;
                c *= c;
                u >>= 1;
            }
            state_ *= mul;
            state_ += add;
        }

        constexpr void seed(uint64_t s) noexcept { state_ = {0, s}; init(); }
        constexpr void seed(uint64_t hi, uint64_t lo) noexcept { state_ = {hi, lo}; init(); }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        using state_type = Uint128;

        static constexpr uint64_t default_seed_ = 0xcafe'f00d'd15e'a5e5ull;
        static constexpr state_type a_ = {0x2360'ed05'1fc6'5da4ull, 0x4385'df64'9fcc'f645ull};
        static constexpr state_type b_ = {0x5851'f42d'4c95'7f2dull, 0x1405'7b7e'f767'814full};

        state_type state_;

        constexpr void init() noexcept {
            state_ += b_;
            state_ *= a_;
            state_ += b_;
        }

    };

    // PCG64 DXSM implementation
    // Based on code by Tony Finch
    // https://dotat.at/@/2023-06-21-pcg64-dxsm.html

    class Pcg64dxsm {

    public:

        using result_type = uint64_t;

        constexpr Pcg64dxsm() noexcept { seed(default_seed); }
        constexpr explicit Pcg64dxsm(uint64_t s) noexcept { seed(s); }
        constexpr explicit Pcg64dxsm(uint64_t s0, uint64_t s1) noexcept { seed(s0, s1); }
        constexpr explicit Pcg64dxsm(uint64_t s0, uint64_t s1, uint64_t s2, uint64_t s3) noexcept { seed(s0, s1, s2, s3); }

        constexpr uint64_t operator()() noexcept {
            auto st = this->state_;
            this->state_ = st * mul + this->inc_;
            auto hi = uint64_t(st >> 64);
            auto lo = uint64_t(st | 1);
            hi ^= hi >> 32;
            hi *= mul;
            hi ^= hi >> 48;
            hi *= lo;
            return hi;
        }

        constexpr void seed(uint64_t s) noexcept { seed(0, s, 0, 0); }
        constexpr void seed(uint64_t s0, uint64_t s1) noexcept { seed(s0, s1, 0, 0); }

        constexpr void seed(uint64_t s0, uint64_t s1, uint64_t s2, uint64_t s3) noexcept {
            state_ = {s0, s1};
            inc_ = {s2, s3};
            inc_ = (inc_ << 1) | 1;
            state_ += inc_;
            (*this)();
        }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        using state_type = Uint128;

        static constexpr uint64_t default_seed = 0xcafe'f00d'd15e'a5e5ull;
        static constexpr uint64_t mul = 0xda94'2042'e4dd'58b5ull;

        state_type state_;
        state_type inc_;

    };

    // Xoshiro256** generator by David Blackman and Sebastiano Vigna
    // http://xoshiro.di.unimi.it/

    namespace Detail {

        class SplitMix64 {
        public:
            using result_type = uint64_t;
            constexpr SplitMix64() noexcept: state_(0) {}
            constexpr explicit SplitMix64(uint64_t s) noexcept: state_ (s) {}
            constexpr uint64_t operator()() noexcept {
                constexpr uint64_t a = 0x9e37'79b9'7f4a'7c15ull;
                constexpr uint64_t b = 0xbf58'476d'1ce4'e5b9ull;
                constexpr uint64_t c = 0x94d0'49bb'1331'11ebull;
                state_ += a;
                uint64_t x = (state_ ^ (state_ >> 30)) * b;
                x = (x ^ (x >> 27)) * c;
                return x ^ (x >> 31);
            }
            constexpr void seed(uint64_t s = 0) noexcept { state_ = s; }
            static constexpr uint64_t min() noexcept { return 0; }
            static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }
        private:
            uint64_t state_;
        };

    }

    class Xoshiro {

    public:

        using result_type = uint64_t;

        constexpr Xoshiro() noexcept: state_() { seed(0); }
        constexpr explicit Xoshiro(uint64_t s) noexcept: state_() { seed(s); }
        constexpr Xoshiro(uint64_t s, uint64_t t) noexcept: state_() { seed(s, t); }
        constexpr Xoshiro(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept: state_() { seed(s, t, u, v); }

        constexpr uint64_t operator()() noexcept {
            uint64_t x = std::rotl(state_[1] * 5, 7) * 9;
            uint64_t y = state_[1] << 17;
            state_[2] ^= state_[0];
            state_[3] ^= state_[1];
            state_[1] ^= state_[2];
            state_[0] ^= state_[3];
            state_[2] ^= y;
            state_[3] = std::rotl(state_[3], 45);
            return x;
        }

        constexpr void seed(uint64_t s = 0) noexcept {
            Detail::SplitMix64 sm(s);
            state_[0] = sm();
            state_[1] = sm();
            state_[2] = sm();
            state_[3] = sm();
        }

        constexpr void seed(uint64_t s, uint64_t t) noexcept {
            Detail::SplitMix64 sm(s);
            state_[0] = sm();
            state_[1] = sm();
            sm.seed(t);
            state_[2] = sm();
            state_[3] = sm();
        }

        constexpr void seed(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept {
            state_[0] = s;
            state_[1] = t;
            state_[2] = u;
            state_[3] = v;
        }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        std::array<uint64_t, 4> state_;

    };

    // Default choice of RNG

    using StdRng = Pcg64dxsm;

    // Seeding functions

    inline uint64_t seed64() {
        std::random_device dev;
        uint64_t a = dev();
        uint64_t b = dev();
        return (a << 32) + b;
    }

    template <typename RNG>
    void seed_from_device(RNG& rng) {

        using T = typename RNG::result_type;

        std::random_device dev;
        std::function<T()> gen;

        if constexpr (sizeof(T) <= sizeof(uint32_t))

            gen = [&dev] {
                uint32_t x = dev();
                return T(x);
            };

        else

            gen = [&dev] {
                uint64_t x = dev();
                uint64_t y = dev();
                return T((x << 32) | y);
            };

        if constexpr (requires (T t) { { rng.seed(t, t, t, t) }; }) {

            auto s = gen();
            auto t = gen();
            auto u = gen();
            auto v = gen();
            rng.seed(s, t, u, v);

        } else if constexpr (requires (T t) { { rng.seed(t, t) }; }) {

            auto s = gen();
            auto t = gen();
            rng.seed(s, t);

        } else if constexpr (requires (T t) { { rng.seed(t) }; }) {

            auto s = gen();
            rng.seed(s);

        } else {

            static_assert(dependent_false<RNG>, "Type not usable with seed_from_device()");

        }

    }

}
