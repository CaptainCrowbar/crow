#pragma once

#include "crow/binary.hpp"
#include "crow/types.hpp"
#include <bit>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>

namespace Crow {

    // Hash functions

    namespace Detail {

        constexpr size_t mix_hashes(size_t h1, size_t h2) noexcept {
            return h1 ^ ((h1 << 6) + (h1 >> 2) + h2 + 0x9e3779b9);
        }

    }

    template <typename T1, typename T2, typename... Args>
    size_t hash_mix(const T1& t1, const T2& t2, const Args&... args) {
        using namespace Detail;
        size_t h = std::hash<T1>()(t1);
        if constexpr (sizeof...(Args) == 0)
            h = mix_hashes(h, std::hash<T2>()(t2));
        else
            h = mix_hashes(h, hash_mix(t2, args...));
        return h;
    }

    template <typename Range>
    size_t hash_mix(const Range& r) {
        using std::begin;
        using T = std::decay_t<decltype(*begin(r))>;
        std::hash<T> t_hash;
        size_t h = 0;
        for (auto& x: r)
            h = Detail::mix_hashes(h, t_hash(x));
        return h;
    }

    inline uint32_t bernstein_hash(const void* ptr, size_t len) noexcept {
        auto bptr = static_cast<const uint8_t*>(ptr);
        uint32_t h = 5381;
        for (size_t i = 0; i < len; ++i)
            h = 33 * h + bptr[i];
        return h;
    }

    // SipHash-2-4-64 by Jean-Philippe Aumasson and Daniel J. Bernstein
    // https://github.com/veorq/SipHash

    class SipHash {

    public:

        using result_type = uint64_t;

        constexpr SipHash() noexcept {}
        constexpr SipHash(uint64_t key0, uint64_t key1) noexcept: key0_(key0), key1_(key1) {}

        constexpr uint64_t operator()(const void* ptr, size_t len) const noexcept {

            uint64_t v0 = 0x736f'6d65'7073'6575ull;
            uint64_t v1 = 0x646f'7261'6e64'6f6dull;
            uint64_t v2 = 0x6c79'6765'6e65'7261ull;
            uint64_t v3 = 0x7465'6462'7974'6573ull;

            v0 ^= key0_;
            v1 ^= key1_;
            v2 ^= key0_;
            v3 ^= key1_;

            auto bptr = static_cast<const uint8_t*>(ptr);
            auto end = bptr + (len - len % sizeof(uint64_t));

            while (bptr != end) {
                uint64_t m = 0;
                for (int i = 0; i < 64; i += 8)
                    m |= uint64_t(*bptr++) << i;
                v3 ^= m;
                siprounds(2, v0, v1, v2, v3);
                v0 ^= m;
            }

            int left = int(len & 7);
            uint64_t b = uint64_t(len) << 56;

            for (int i = left - 1; i >= 0; --i)
                b |= uint64_t(bptr[i]) << 8 * i;

            v3 ^= b;
            siprounds(2, v0, v1, v2, v3);
            v0 ^= b;
            v2 ^= 0xff;
            siprounds(4, v0, v1, v2, v3);

            return v0 ^ v1 ^ v2 ^ v3;

        }

    private:

        uint64_t key0_ = 0;
        uint64_t key1_ = 0;

        static constexpr void siprounds(int n, uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3) noexcept {
            for (int i = 0; i < n; ++i) {
                v0 += v1;
                v1 = std::rotl(v1, 13);
                v1 ^= v0;
                v0 = std::rotl(v0, 32);
                v2 += v3;
                v3 = std::rotl(v3, 16);
                v3 ^= v2;
                v0 += v3;
                v3 = std::rotl(v3, 21);
                v3 ^= v0;
                v2 += v1;
                v1 = std::rotl(v1, 17);
                v1 ^= v2;
                v2 = std::rotl(v2, 32);
            }
        }

    };

    // Cryptographic hash functions

    class CryptographicHash {

    public:

        using result_type = std::string;

        virtual ~CryptographicHash() noexcept {}
        CryptographicHash(const CryptographicHash&) = delete;
        CryptographicHash(CryptographicHash&&) = delete;
        CryptographicHash& operator=(const CryptographicHash&) = delete;
        CryptographicHash& operator=(CryptographicHash&&) = delete;

        std::string operator()(const void* ptr, size_t len) { clear(); add(ptr, len); return get(); }
        std::string operator()(const std::string& str) { clear(); add(str); return get(); }

        size_t bits() const noexcept { return bits_; }
        size_t bytes() const noexcept { return bits_ / 8; }
        void add(const void* ptr, size_t len) { do_add(ptr, len); }
        void add(const std::string& str) { do_add(str.data(), str.size()); }
        std::string get() { do_final(); return hash_; }
        void clear() noexcept { do_final(); hash_.assign(bytes(), '\0'); }

    protected:

        size_t bits_;
        void* anon_ctx_;
        std::string hash_;

        explicit CryptographicHash(size_t b): bits_(b), anon_ctx_(nullptr), hash_(bytes(), '\0') {}

        virtual void do_add(const void* ptr, size_t len) = 0;
        virtual void do_final() noexcept = 0;

        auto byte_data() noexcept { return reinterpret_cast<unsigned char*>(hash_.data()); }

    };

    class MD5:
    public CryptographicHash {
    public:
        MD5(): CryptographicHash(128) {}
        ~MD5() noexcept override { do_final(); }
    private:
        void do_add(const void* ptr, size_t len) override;
        void do_final() noexcept override;
    };

    class SHA1:
    public CryptographicHash {
    public:
        SHA1(): CryptographicHash(160) {}
        ~SHA1() noexcept override { do_final(); }
    private:
        void do_add(const void* ptr, size_t len) override;
        void do_final() noexcept override;
    };

    class SHA256:
    public CryptographicHash {
    public:
        SHA256(): CryptographicHash(256) {}
        ~SHA256() noexcept override { do_final(); }
    private:
        void do_add(const void* ptr, size_t len) override;
        void do_final() noexcept override;
    };

    class SHA512:
    public CryptographicHash {
    public:
        SHA512(): CryptographicHash(512) {}
        ~SHA512() noexcept override { do_final(); }
    private:
        void do_add(const void* ptr, size_t len) override;
        void do_final() noexcept override;
    };

}
