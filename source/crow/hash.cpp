#include "crow/hash.hpp"

#if defined(__APPLE__)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #include <CommonCrypto/CommonDigest.h>
#elif defined(_XOPEN_SOURCE)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #include <openssl/md5.h>
    #include <openssl/sha.h>
#else
    #include <windows.h>
    #include <wincrypt.h>
#endif

namespace Crow {

    // Cryptographic hash functions

    #if defined(__APPLE__)

        #define HASH_CONTEXT(unix_function_prefix, unix_context_prefix)   CC_ ## unix_function_prefix ## _CTX
        #define HASH_INIT(unix_function_prefix, windows_function_suffix)  CC_ ## unix_function_prefix ## _Init(ctx);
        #define HASH_UPDATE(unix_function_prefix)                         CC_ ## unix_function_prefix ## _Update(ctx, ptr, uint32_t(len));
        #define HASH_FINAL(unix_function_prefix)                          CC_ ## unix_function_prefix ## _Final(byte_data(), ctx);

    #elif defined(_XOPEN_SOURCE)

        #define HASH_CONTEXT(unix_function_prefix, unix_context_prefix)   unix_context_prefix ## _CTX
        #define HASH_INIT(unix_function_prefix, windows_function_suffix)  unix_function_prefix ## _Init(ctx);
        #define HASH_UPDATE(unix_function_prefix)                         unix_function_prefix ## _Update(ctx, ptr, uint32_t(len));
        #define HASH_FINAL(unix_function_prefix)                          unix_function_prefix ## _Final(byte_data(), ctx);

    #else

        struct HashContext {
            HCRYPTPROV hcprov;
            HCRYPTHASH hchash;
            DWORD hashlen;
        };

        #define HASH_CONTEXT(unix_function_prefix, unix_context_prefix) HashContext
        #define HASH_INIT(unix_function_prefix, windows_function_suffix) \
            CryptAcquireContextW(&ctx->hcprov, nullptr, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_SILENT | CRYPT_VERIFYCONTEXT); \
            CryptCreateHash(ctx->hcprov, CALG_ ## windows_function_suffix, 0, 0, &ctx->hchash);
        #define HASH_UPDATE(unix_function_prefix) \
            CryptHashData(ctx->hchash, static_cast<const unsigned char*>(ptr), DWORD(len), 0);
        #define HASH_FINAL(unix_function_prefix) \
            ctx->hashlen = DWORD(bytes()); \
            CryptGetHashParam(ctx->hchash, HP_HASHVAL, byte_data(), &ctx->hashlen, 0); \
            CryptDestroyHash(ctx->hchash); \
            CryptReleaseContext(ctx->hcprov, 0);

    #endif

    #define IMPLEMENT_CRYPTOGRAPHIC_HASH(ClassName, unix_function_prefix, unix_context_prefix, windows_function_suffix) \
        void ClassName::do_add(const void* ptr, size_t len) { \
            using context_type = HASH_CONTEXT(unix_function_prefix, unix_context_prefix); \
            auto ctx = static_cast<context_type*>(anon_ctx_); \
            if (! anon_ctx_) { \
                anon_ctx_ = ctx = new context_type; \
                HASH_INIT(unix_function_prefix, windows_function_suffix) \
            } \
            HASH_UPDATE(unix_function_prefix) \
        } \
        void ClassName::do_final() noexcept { \
            using context_type = HASH_CONTEXT(unix_function_prefix, unix_context_prefix); \
            if (anon_ctx_) { \
                auto ctx = static_cast<context_type*>(anon_ctx_); \
                HASH_FINAL(unix_function_prefix) \
                delete ctx; \
                anon_ctx_ = nullptr; \
            } \
        }

    IMPLEMENT_CRYPTOGRAPHIC_HASH(MD5, MD5, MD5, MD5)
    IMPLEMENT_CRYPTOGRAPHIC_HASH(SHA1, SHA1, SHA, SHA1)
    IMPLEMENT_CRYPTOGRAPHIC_HASH(SHA256, SHA256, SHA256, SHA_256)
    IMPLEMENT_CRYPTOGRAPHIC_HASH(SHA512, SHA512, SHA512, SHA_512)

}
