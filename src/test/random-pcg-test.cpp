#include "crow/random-engines.hpp"
#include "crow/unit-test.hpp"
#include <vector>

using namespace Crow;

void test_crow_random_pcg64() {

    static const std::vector<uint64_t> expect1 = {
        0xcf7d'be68'4e0c'4045ull, 0x1564'2875'dfe1'e67cull, 0x32f0'49df'2f50'd811ull, 0x98c1'd0a1'63e1'f856ull,
        0x0743'e58f'0360'd766ull, 0xfee9'e4fc'0347'9bf6ull, 0xf7ce'cb16'2663'9417ull, 0x2df8'59db'9576'2826ull,
        0x1c3f'9381'2e14'8ea4ull, 0x8af3'831f'2bb8'5205ull, 0x18a8'332d'd6ff'efc9ull, 0xf4d9'e8a9'f8a2'4f80ull,
        0x93ae'319f'c35b'2252ull, 0x72db'88f7'e0fc'64e7ull, 0x33c3'afd9'5136'5990ull, 0xb466'b761'b08a'33e0ull,
        0x819b'd59c'9546'97d9ull, 0x1e12'dfcf'3603'd13cull, 0xb65d'9c1f'4263'1174ull, 0x879c'9407'8269'79cdull,
        0x2e28'0ad2'288f'501cull, 0x1c4a'7b23'9437'9f32ull, 0x7f0a'705c'9e02'c72dull, 0xbe49'b435'ae70'f9daull,
        0xc685'41ce'0c31'758dull, 0x906d'8d98'5d14'140full, 0xe376'87a6'39e5'9e4bull, 0x4c86'f84e'1bd6'f7a6ull,
        0x8d94'ae5b'c2ef'4fffull, 0x399f'88cd'fc35'c9b8ull, 0xbd2b'35c1'1c2e'bda4ull, 0xfd3b'3cc7'91f4'c41bull,
        0x4cee'57ea'75a2'c553ull, 0x3ee4'0429'e17e'08ccull, 0xd4e8'b915'a77f'55d2ull, 0x17b0'c80b'5f47'7bbdull,
        0x32ea'b935'0284'9a9aull, 0xe459'97b6'4cd4'b70dull, 0x344b'89a2'783b'3d44ull, 0x0dd1'cfdd'02d3'6391ull,
        0x3fac'997d'8735'0aa6ull, 0x42c0'87c9'45dd'1658ull, 0x0742'24ac'0bb4'ca7full, 0xec19'189c'e139'33f5ull,
        0xdb6f'4363'7724'3bc7ull, 0xdd86'c642'78e5'7554ull, 0x97a0'66f7'2130'10c5ull, 0x51fe'e8d4'06fb'0559ull,
        0x1be0'4809'd240'd333ull, 0xdb42'6032'6880'20a3ull, 0x4a23'd545'5f31'7b2cull, 0x10ef'6b20'5c4b'4f76ull,
        0xa44a'b230'4e80'c901ull, 0xda79'6dab'0f57'0ff7ull, 0xebbe'0cde'e293'2dfdull, 0xe658'e297'513b'80beull,
        0xb2fc'bccc'4873'715bull, 0x0fbf'3bb2'ff94'dadeull, 0xf5c9'52ff'7541'4f85ull, 0x94fa'ed41'788e'c226ull,
        0xb699'cd1d'bc63'9cd9ull, 0xc402'a50c'737e'0a62ull, 0x95fa'c59a'4269'c9baull, 0x84ee'24cf'64cb'80a4ull,
    };

    static const std::vector<uint64_t> expect2 = {
        0x7e68'4885'6818'e376ull, 0xc7a3'ca18'7fec'4f97ull, 0x4bf5'9a39'22b1'5522ull, 0x8424'4cd3'df75'a1eaull,
        0xadbf'c289'c379'431eull, 0xb86e'b9b4'b36d'a00eull, 0x474b'9deb'd7a7'061cull, 0x3354'0fe3'b963'9b67ull,
        0x4625'c32b'c26b'36bfull, 0x7021'1a03'b27e'ee0full, 0x6877'db4a'31b9'0b48ull, 0x7161'475b'7d80'ef34ull,
        0x9b48'966a'b8d2'3125ull, 0x6859'd41c'6031'442cull, 0xb190'0069'cfdc'4a61ull, 0xc186'f98c'1f49'd15bull,
        0x2a22'1618'a716'6525ull, 0x6045'f3cc'7d9b'36fbull, 0x5e1c'6a28'a062'c630ull, 0x0fa1'e306'4314'2ee7ull,
        0xd3c8'1d7f'6f9a'914bull, 0xa53e'a651'4fb6'2041ull, 0x2db6'd388'd913'd064ull, 0xef30'01ba'12c1'8df1ull,
        0x79a0'baf7'3c99'79d7ull, 0xcada'9fb5'525c'abdfull, 0x69a1'7a68'7c33'c840ull, 0x6222'6119'de06'c52dull,
        0x95dd'4f9f'110a'9364ull, 0x53fc'5e57'07f2'93c2ull, 0x0613'6b7d'84ba'1ac6ull, 0xd130'4cd1'3f46'22e9ull,
        0x6208'fe06'6947'1579ull, 0xe8bc'14ef'466a'8241ull, 0x2886'5467'fc3a'c6e8ull, 0x4aa8'8ba5'0ab2'a873ull,
        0x1eca'26ce'a602'cad8ull, 0xe42d'feb7'96ca'b0e0ull, 0xd8df'1f37'3279'c71aull, 0x9e16'4fe3'9fa9'1d64ull,
        0x1c00'eed2'8930'bdf9ull, 0x05e4'73b7'aaa7'ae1dull, 0xb335'6dd9'fde7'c6bfull, 0xd2f3'3ea8'f019'75f7ull,
        0x35e4'c439'c899'3246ull, 0x4e93'dab0'21b5'6544ull, 0x92f7'4cf4'f789'c20dull, 0x1bac'6300'fa68'5808ull,
        0xbe35'bb87'5dbe'449aull, 0x87ef'616c'cab2'c820ull, 0x85fb'bbe9'ad49'0394ull, 0xd961'76fa'0e02'0ca1ull,
        0x826d'ddfa'd7c6'c56cull, 0x9781'30b7'471b'a6f3ull, 0x5de5'17ad'6dda'439dull, 0xba1f'b254'6d0d'f742ull,
        0x2468'efff'49b6'21c6ull, 0xfb40'6713'3c55'6926ull, 0x6c50'0e69'3ac7'a271ull, 0x1481'512e'2093'b0feull,
        0x08a8'4c90'76d8'e712ull, 0x8655'ad7a'fe94'3fe1ull, 0x184e'7e05'6017'4c53ull, 0x28f8'ca20'ed34'daeeull,
    };

    Pcg64 rng;
    uint64_t x = 0;

    for (auto y: expect1) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

    TRY(rng.advance(10000));

    for (auto y: expect2) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

    TRY(rng.advance(-10128));

    for (auto y: expect1) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}

void test_crow_random_pcg64dxsm() {

    static const std::vector<uint64_t> expect = {
        0xa9b6'7185'2137'230bull,  0xb3cc'f402'a218'bf42ull,  0x8899'5fef'06e8'7299ull,  0xf297'6e1c'e9c8'fb39ull,
        0xb6ac'2dd4'b8b9'fc7full,  0x93e2'281f'6f03'230dull,  0xdbfd'17dc'c4ae'5577ull,  0x7422'bd2e'34b0'57d4ull,
        0x7a7c'fcf6'f879'80f2ull,  0x0f50'0975'3260'8022ull,  0xb988'c173'a88d'e4f0ull,  0x6fa9'dbc7'8193'9228ull,
        0xfcca'3e1f'd78f'db49ull,  0xf057'dd1c'fe29'cf46ull,  0x7e66'83c0'b4d7'125full,  0xbd9d'3f2d'ec76'80dcull,
        0x6706'6d0b'a710'6e90ull,  0xbfab'7e31'd261'7209ull,  0x30c9'8bb6'73a5'946dull,  0xc1f4'4b6d'660b'793dull,
        0x5456'030f'5eec'32d8ull,  0x9e15'd122'51a2'8be7ull,  0x8f45'a928'e8f3'4d0full,  0xcb14'bc52'8a16'5c25ull,
        0xb611'0438'5fcd'f646ull,  0xf912'e066'96dc'85c5ull,  0x57c5'112b'e759'5b98ull,  0x9a91'97a0'c3a3'aee9ull,
        0xd488'66ed'cca8'6cf7ull,  0x1857'32ad'174d'f42cull,  0xd675'1d3c'5708'07baull,  0xa056'25a6'ab29'71f3ull,
        0x8f60'3032'7a8d'10a1ull,  0x3e3c'3e5f'ecbc'dd1full,  0xb20e'8430'0c2d'c089ull,  0x570b'ec77'b7e5'6f00ull,
        0xb8a1'aa82'291b'f578ull,  0x4711'aa73'e274'd048ull,  0x6dd7'b147'1092'0764ull,  0x4662'f657'11cb'd5b5ull,
        0xc6af'd76c'bfd0'8e66ull,  0x62a8'23ad'4155'b3c5ull,  0x5435'd547'7150'f2edull,  0x6a94'45a0'bc2d'2f80ull,
        0x085d'cfb8'ad4f'a6dcull,  0xac49'def8'215a'59cdull,  0xce0d'12e5'4bf1'1d13ull,  0x3da0'd750'b62d'774eull,
        0xb009'de26'0e56'ce20ull,  0x1cc9'120b'b184'ebc2ull,  0xe3c9'd45b'1b73'bbd9ull,  0x6c48'6cb5'c9bb'fe68ull,
        0x67cc'26e6'7265'f44full,  0xde5f'f5de'4a74'4606ull,  0xe504'129b'f47b'034aull,  0xa3ec'e980'9f89'80a9ull,
        0x1b06'c26a'aa40'2c9aull,  0x5cd5'9ba4'19ba'd600ull,  0x0e06'199a'7610'7cbeull,  0xc40f'500f'5ef1'8cf7ull,
        0x6d64'86e6'1f00'9646ull,  0x3223'376c'4205'08c5ull,  0x4a31'5330'afd4'6867ull,  0x023c'72db'b1fe'94f1ull,
    };

    Pcg64dxsm rng;
    uint64_t x = 0;

    for (auto y: expect) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}
