#include "crow/uuid.hpp"
#include "crow/fixed-binary.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>

using namespace Crow;

void test_crow_uuid() {

    Uuid u;
    Uint128 i, j;
    std::string s;

    TEST_EQUAL(u.str(), "00000000-0000-0000-0000-000000000000");

    TRY(u = Uuid(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
    TEST_EQUAL(u.str(), "01020304-0506-0708-090a-0b0c0d0e0f10");
    for (unsigned i = 0; i < 16; ++i)
        TEST_EQUAL(u[i], i + 1);
    TRY(u = Uuid(0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0));
    TEST_EQUAL(u.str(), "12345678-9abc-def0-1234-56789abcdef0");
    TEST_EQUAL(std::string(u), "12345678-9abc-def0-1234-56789abcdef0");

    i = 0;
    TRY(u = Uuid(i));
    TEST_EQUAL(u.str(), "00000000-0000-0000-0000-000000000000");
    TRY(j = Uint128(u));
    TEST_EQUAL(j, 0);
    i = {0xfedc'ba98'7654'3210ull, 0x1234'5678'9abc'def0ull};
    TRY(u = Uuid(i));
    TEST_EQUAL(u.str(), "f0debc9a-7856-3412-1032-547698badcfe");
    TRY(j = Uint128(u));
    TEST_EQUAL(j, (Uint128{0xfedc'ba98'7654'3210ull, 0x1234'5678'9abc'def0ull}));

    s = "abcdefghijklmnop";

    TRY(u = Uuid(s.data(), 0));   TEST_EQUAL(u.str(), "00000000-0000-0000-0000-000000000000");
    TRY(u = Uuid(s.data(), 8));   TEST_EQUAL(u.str(), "61626364-6566-6768-0000-000000000000");
    TRY(u = Uuid(s.data(), 16));  TEST_EQUAL(u.str(), "61626364-6566-6768-696a-6b6c6d6e6f70");
    TRY(u = Uuid(s.data(), 24));  TEST_EQUAL(u.str(), "61626364-6566-6768-696a-6b6c6d6e6f70");

    TRY(u = Uuid("123456789abcdef123456789abcdef12"));                                                   TEST_EQUAL(u.str(), "12345678-9abc-def1-2345-6789abcdef12");
    TRY(u = Uuid("3456789a-bcde-f123-4567-89abcdef1234"));                                               TEST_EQUAL(u.str(), "3456789a-bcde-f123-4567-89abcdef1234");
    TRY(u = Uuid("56,78,9a,bc,de,f1,23,45,67,89,ab,cd,ef,12,34,56"));                                    TEST_EQUAL(u.str(), "56789abc-def1-2345-6789-abcdef123456");
    TRY(u = Uuid("{0x78,0x9a,0xbc,0xde,0xf1,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0x12,0x34,0x56,0x78}"));  TEST_EQUAL(u.str(), "789abcde-f123-4567-89ab-cdef12345678");
    TRY(u = Uuid("{0xbcdef123, 0x4567, 0x89ab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};"));      TEST_EQUAL(u.str(), "bcdef123-4567-89ab-cdef-123456789abc");

    TEST_THROW(u = Uuid(""), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789abcdef123456789abcdefgh"), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789abcdef123456789abcdef"), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789abcdef123456789abcdef1"), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789abcdef123456789abcdef123"), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789abcdef123456789abcdef1234"), std::invalid_argument);
    TEST_THROW(u = Uuid("123456789-abc-def1-2345-6789abcdef12"), std::invalid_argument);

}
