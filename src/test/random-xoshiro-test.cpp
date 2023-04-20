#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"
#include <vector>

using namespace Crow;
using namespace Crow::Detail;

void test_crow_random_splitmix64() {

    static const std::vector<uint64_t> expect = {
        0xe220'a839'7b1d'cdafull, 0x6e78'9e6a'a1b9'65f4ull, 0x06c4'5d18'8009'454full, 0xf88b'b8a8'724c'81ecull,
        0x1b39'896a'51a8'749bull, 0x53cb'9f0c'747e'a2eaull, 0x2c82'9abe'1f45'32e1ull, 0xc584'133a'c916'ab3cull,
        0x3ee5'7890'41c9'8ac3ull, 0xf3b8'488c'368c'b0a6ull, 0x657e'ecdd'3cb1'3d09ull, 0xc2d3'26e0'055b'def6ull,
        0x8621'a03f'e0bb'db7bull, 0x8e1f'7555'983a'a92full, 0xb54e'0f16'00cc'4d19ull, 0x84bb'3f97'971d'80abull,
        0x7d29'825c'7552'1255ull, 0xc3cf'1710'2b7f'7f86ull, 0x3466'e9a0'8391'4f64ull, 0xd81a'8d2b'5a44'85acull,
        0xdb01'602b'100b'9ed7ull, 0xa903'8a92'1825'f10dull, 0xedf5'f1d9'0dca'2f6aull, 0x5449'6ad6'7bd2'634cull,
        0xdd7c'01d4'f540'7269ull, 0x935e'82f1'db4c'4f7bull, 0x69b8'2ebc'9223'3300ull, 0x40d2'9eb5'7de1'd510ull,
        0xa2f0'9dab'b45c'6316ull, 0xee52'1d7a'0f4d'3872ull, 0xf169'52ee'72f3'454full, 0x377d'35de'a8e4'0225ull,
        0x0c7d'e806'4963'bab0ull, 0x0558'2d37'111a'c529ull, 0xd254'741f'599d'c6f7ull, 0x6963'0f75'93d1'08c3ull,
        0x417e'f961'81da'a383ull, 0x3c3c'41a3'b433'43a1ull, 0x6e19'905d'cbe5'31dfull, 0x4fa9'fa73'2485'1729ull,
        0x84eb'4454'a792'922aull, 0x134f'7096'9181'75ceull, 0x07dc'930b'3022'78a8ull, 0x12c0'15a9'7019'e937ull,
        0xcc06'c316'52eb'f438ull, 0xecee'6563'0a69'1e37ull, 0x3e84'ecb1'763e'79adull, 0x690e'd476'743a'ae49ull,
        0x7746'15d7'b1a1'f2e1ull, 0x22b3'53f0'4f4f'52daull, 0xe3dd'd86b'a71a'5eb1ull, 0xdf26'8ade'b651'3356ull,
        0x2098'eb73'd436'7d77ull, 0x03d6'8453'23ce'3c71ull, 0xc952'c562'0043'c714ull, 0x9b19'6bca'844f'1705ull,
        0x3026'0345'dd9e'0ec1ull, 0xcf44'8a58'82bb'9698ull, 0xf4a5'78dc'cbc8'7656ull, 0xbfde'aed9'a17b'3c8full,
        0xed79'402d'1d5c'5d7bull, 0x55f0'70ab'1cbb'f170ull, 0x3e00'a349'29a8'8f1dull, 0xe255'b237'b8bb'18fbull,
    };

    SplitMix64 rng;
    uint64_t x = 0;

    for (auto y: expect) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}

void test_crow_random_xoshiro256ss() {

    static const std::vector<uint64_t> expect = {
        0x99ec'5f36'cb75'f2b4ull, 0xbf6e'1f78'4956'452aull, 0x1a5f'849d'4933'e6e0ull, 0x6aa5'94f1'262d'2d2cull,
        0xbba5'ad4a'1f84'2e59ull, 0xffef'8375'd9eb'cacaull, 0x6c16'0dee'd2f5'4c98ull, 0x8920'ad64'8fc3'0a3full,
        0xdb03'2c0b'a753'9731ull, 0xeb3a'475a'3e74'9a3dull, 0x1d42'993f'a43f'2a54ull, 0x1136'1bf5'26a1'4bb5ull,
        0x1b4f'07a5'ab3d'8e9cull, 0xa7a3'257f'6986'db7full, 0x7efd'aa95'605d'fc9cull, 0x4bde'97c0'a78e'aab8ull,
        0xb455'eac4'3518'666cull, 0x304d'bf6c'0673'0690ull, 0x8cbe'7776'598a'798cull, 0x0ecb'df7f'fcd7'27e5ull,
        0x4ff5'2157'533f'e270ull, 0x7e61'475b'8724'2f2eull, 0x5255'8c68'a931'6824ull, 0xa0bd'00c5'9247'1176ull,
        0xfc9b'83a3'a0c6'3b9eull, 0x4d78'6c0f'0a8b'88efull, 0xa524'73c4'f62f'2338ull, 0xe9dc'0037'db25'd6d9ull,
        0xfce5'eba9'd250'94c3ull, 0xe3db'e61e'e2d6'4b51ull, 0x23f6'2e43'2b12'72dfull, 0x4ac7'443a'342c'4913ull,
        0xc31c'f1a9'658c'1991ull, 0x290c'97ff'ce91'8b1dull, 0xf544'55e0'2e90'636aull, 0xf577'4575'8bb8'f33full,
        0xe5e1'b685'1228'23d9ull, 0x2c16'cde0'fd80'97ecull, 0x3cde'bc44'a5bc'1936ull, 0x6833'bafa'723c'2dbdull,
        0xb6fa'6c4b'a1d3'd39eull, 0xe5b9'32b6'56c2'edc3ull, 0x09cf'0b61'2161'5c9full, 0x214e'25d5'7fc6'36d5ull,
        0xcf3d'1721'806e'2537ull, 0xcf79'6fc6'335d'dc02ull, 0x353c'8b86'489b'0322ull, 0xfc48'6582'2547'b6aaull,
        0xe8c9'3d84'ee8b'3f8cull, 0xd1b4'2120'a323'f2d6ull, 0xa73a'11d2'47ff'36b2ull, 0xae42'2369'58bb'a58cull,
        0xb622'679e'2aff'cf3aull, 0xcc3b'ab00'60f6'45f4ull, 0x2e01'e45c'78f0'daa7ull, 0x0856'6c5f'16be'948aull,
        0x73be'ac21'87e1'f640ull, 0x8e90'3d75'2c1b'5d6eull, 0x5b34'6810'94d7'511dull, 0x70eb'ad38'2047'f5c1ull,
        0xeae5'ca14'48d4'e9ccull, 0x3d2d'6277'5b63'1bd5ull, 0x8cb7'2ebc'5b4f'7dc3ull, 0x099c'2939'ea69'0a80ull,
    };

    Xoshiro rng;
    uint64_t x = 0;

    for (auto y: expect) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}
