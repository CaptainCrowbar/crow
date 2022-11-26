#include "crow/hash.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <array>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

using namespace Crow;

void test_crow_hash_mix() {

    int num = 42;
    std::string str = "Hello world";
    uint64_t big = 123'456'789;
    auto tpl = std::make_tuple(num, str, big);
    int i = 123;
    int j = 456;
    int k = 789;
    auto it = std::make_tuple(i, j, k);
    auto iv = std::vector{i, j, k};

    auto h1 = hash_mix(num, str, big);
    auto h2 = hash_mix(tpl);
    auto h3 = hash_mix(i, j, k);
    auto h4 = hash_mix(it);
    auto h5 = hash_mix(iv);

    TEST(h1 != 0);
    TEST(h2 != 0);
    TEST(h3 != 0);
    TEST(h4 != 0);
    TEST(h5 != 0);
    TEST_EQUAL(h1, h2);
    TEST_EQUAL(h3, h4);
    TEST_EQUAL(h3, h5);
    TEST(h1 != h3);

}

void test_crow_hash_multiplicative() {

    std::string s;
    uint32_t h = 0;

    s = "";             TRY(h = BernsteinHash()(s));  TEST_EQUAL(h, 0x0000'1505ul);
    s = "Hello world";  TRY(h = BernsteinHash()(s));  TEST_EQUAL(h, 0x89bb'20a1ul);
    s = "";             TRY(h = KernighanHash()(s));  TEST_EQUAL(h, 0x0000'0000ul);
    s = "Hello world";  TRY(h = KernighanHash()(s));  TEST_EQUAL(h, 0xce59'8aa4ul);

}

void test_crow_hash_siphash() {

    static constexpr std::array<uint64_t, 64> vectors_sip64 = {{
        0x726f'db47'dd0e'0e31ull, 0x74f8'39c5'93dc'67fdull, 0x0d6c'8009'd9a9'4f5aull, 0x8567'6696'd7fb'7e2dull,
        0xcf27'94e0'2771'87b7ull, 0x1876'5564'cd99'a68dull, 0xcbc9'466e'58fe'e3ceull, 0xab02'00f5'8b01'd137ull,
        0x93f5'f579'9a93'2462ull, 0x9e00'82df'0ba9'e4b0ull, 0x7a5d'bbc5'94dd'b9f3ull, 0xf4b3'2f46'226b'ada7ull,
        0x751e'8fbc'860e'e5fbull, 0x14ea'5627'c084'3d90ull, 0xf723'ca90'8e7a'f2eeull, 0xa129'ca61'49be'45e5ull,
        0x3f2a'cc7f'57c2'9bdbull, 0x699a'e9f5'2cbe'4794ull, 0x4bc1'b3f0'968d'd39cull, 0xbb6d'c91d'a779'61bdull,
        0xbed6'5cf2'1aa2'ee98ull, 0xd0f2'cbb0'2e3b'67c7ull, 0x9353'6795'e3a3'3e88ull, 0xa80c'038c'cd5c'cec8ull,
        0xb8ad'50c6'f649'af94ull, 0xbce1'92de'8a85'b8eaull, 0x17d8'35b8'5bbb'15f3ull, 0x2f2e'6163'076b'cfadull,
        0xde4d'aaac'a71d'c9a5ull, 0xa6a2'5066'8795'6571ull, 0xad87'a353'5c49'ef28ull, 0x32d8'92fa'd841'c342ull,
        0x7127'512f'72f2'7cceull, 0xa7f3'2346'f959'78e3ull, 0x12e0'b01a'bb05'1238ull, 0x15e0'34d4'0fa1'97aeull,
        0x314d'ffbe'0815'a3b4ull, 0x0279'90f0'2962'3981ull, 0xcadc'd4e5'9ef4'0c4dull, 0x9abf'd876'6a33'735cull,
        0x0e3e'a96b'5304'a7d0ull, 0xad0c'42d6'fc58'5992ull, 0x1873'06c8'9bc2'15a9ull, 0xd4a6'0abc'f379'2b95ull,
        0xf935'451d'e4f2'1df2ull, 0xa953'8f04'1975'5787ull, 0xdb9a'cddf'f56c'a510ull, 0xd06c'98cd'5c09'75ebull,
        0xe612'a3cb'9ecb'a951ull, 0xc766'e62c'fcad'af96ull, 0xee64'435a'9752'fe72ull, 0xa192'd576'b245'165aull,
        0x0a87'87bf'8ecb'74b2ull, 0x81b3'e73d'20b4'9b6full, 0x7fa8'220b'a3b2'eceaull, 0x2457'31c1'3ca4'2499ull,
        0xb78d'bfaf'3a8d'83bdull, 0xea1a'd565'322a'1a0bull, 0x60e6'1c23'a379'5013ull, 0x6606'd7e4'4628'2b93ull,
        0x6ca4'ecb1'5c5f'91e1ull, 0x9f62'6da1'5c96'25f3ull, 0xe51b'3860'8ef2'5f57ull, 0x958a'324c'eb06'4572ull,
    }};

    static constexpr uint64_t key0 = 0x0706050403020100ull;
    static constexpr uint64_t key1 = 0x0f0e0d0c0b0a0908ull;

    SipHash sip(key0, key1);
    std::array<uint8_t, 64> in;
    std::iota(in.begin(), in.end(), uint8_t(0));
    uint64_t out;

    for (int i = 0; i < 64; ++i) {
        TRY(out = sip(in.data(), i));
        TEST_EQUAL(out, vectors_sip64[i]);
    }

}

namespace {

    const std::string text1 = "Hello world";
    const std::string text2 =
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad "
        "minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
        "aliquip ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
        "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
        "culpa qui officia deserunt mollit anim id est laborum.";

    template <typename H>
    void run_hash_tests(const std::string& result1, const std::string& result2) {

        H hash;
        std::string result, hex;

        TRY(result = hash(text1));
        TRY(hex = format_object(result, "xz"));
        TEST_EQUAL(hex, result1);

        TRY(result = hash(text2));
        TRY(hex = format_object(result, "xz"));
        TEST_EQUAL(hex, result2);

        size_t block = 42;
        TRY(hash.clear());
        for (size_t pos = 0; pos < text2.size(); pos += block) {
            auto part = text2.substr(pos, block);
            TRY(hash.add(part));
        }
        TRY(result = hash.get());
        TRY(hex = format_object(result, "xz"));
        TEST_EQUAL(hex, result2);

        block = 1;
        TRY(hash.clear());
        for (size_t pos = 0; pos < text1.size(); pos += block) {
            auto part = text1.substr(pos, block);
            TRY(hash.add(part));
        }
        TRY(result = hash.get());
        TRY(hex = format_object(result, "xz"));
        TEST_EQUAL(hex, result1);

        TRY(hash.clear());
        for (size_t pos = 0; pos < text2.size(); pos += block) {
            auto part = text2.substr(pos, block);
            TRY(hash.add(part));
        }
        TRY(result = hash.get());
        TRY(hex = format_object(result, "xz"));
        TEST_EQUAL(hex, result2);

    }

}

void test_crow_hash_md5() {

    run_hash_tests<MD5>(
        "3e25960a79dbc69b674cd4ec67a72c62",
        "fa5c89f3c88b81bfd5e821b0316569af"
    );

}

void test_crow_hash_sha1() {

    run_hash_tests<SHA1>(
        "7b502c3a1f48c8609ae212cdfb639dee39673f5e",
        "19afa2a4a37462c7b940a6c4c61363d49c3a35f4"
    );

}

void test_crow_hash_sha256() {

    run_hash_tests<SHA256>(
        "64ec88ca00b268e5ba1a35678a1b5316d212f4f366b2477232534a8aeca37f3c",
        "2c7c3d5f244f1a40069a32224215e0cf9b42485c99d80f357d76f006359c7a18"
    );

}

void test_crow_hash_sha512() {

    run_hash_tests<SHA512>(
        "b7f783baed8297f0db917462184ff4f08e69c2d5e5f79a942600f9725f58ce1f29c18139bf80b06c0fff2bdd34738452ecf40c488c22a7e3d80cdf6f9c1c0d47",
        "f41d92bc9fc1157a0d1387e67f3d0893b70f7039d3d46d8115b5079d45ad601159398c79c281681e2da09bf7d9f8c23b41d1a0a3c5b528a7f2735933a4353194"
    );

}
