#include "crow/enum.hpp"
#include "crow/unit-test.hpp"
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::EnumHelper;

using Crow::UnitTest::format_range;

void test_crow_enum_trim() {

    TEST_EQUAL(trim(""),                     "");
    TEST_EQUAL(trim("hello world"),          "hello world");
    TEST_EQUAL(trim(" hello world"),         "hello world");
    TEST_EQUAL(trim("hello world "),         "hello world");
    TEST_EQUAL(trim(" hello world "),        "hello world");
    TEST_EQUAL(trim("\r\nhello world\r\n"),  "hello world");

}

void test_crow_enum_split() {

    std::vector<std::string_view> v;
    std::string s;

    TRY(v = split("", ','));                     TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split("abc", ','));                  TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[abc]");
    TRY(v = split("abc,", ','));                 TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[abc]");
    TRY(v = split("abc,def,ghi", ','));          TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[abc,def,ghi]");
    TRY(v = split("abc,def,ghi,", ','));         TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[abc,def,ghi]");
    TRY(v = split(" abc , def , ghi , ", ','));  TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[abc,def,ghi]");

}

void test_crow_enum_parse_integer() {

    TEST_EQUAL(parse_integer<std::int32_t>("0").value(),              0);
    TEST_EQUAL(parse_integer<std::int32_t>("1").value(),              1);
    TEST_EQUAL(parse_integer<std::int32_t>("2147483647").value(),     2147483647);
    TEST_EQUAL(parse_integer<std::int32_t>("123456789").value(),      123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("123456789abc").value(),   123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("01234567").value(),       342391);
    TEST_EQUAL(parse_integer<std::int32_t>("0xabcdef").value(),       11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("0xabcdefghi").value(),    11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("0b101010").value(),       42);
    TEST_EQUAL(parse_integer<std::int32_t>("123'456'789").value(),    123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("0xabc'def").value(),      11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("0b101'010").value(),      42);
    TEST_EQUAL(parse_integer<std::int32_t>("42abc").value(),          42);
    TEST_EQUAL(parse_integer<std::int32_t>("42_abc").value(),         42);
    TEST_EQUAL(parse_integer<std::int32_t>("+1").value(),             1);
    TEST_EQUAL(parse_integer<std::int32_t>("-1").value(),             -1);
    TEST_EQUAL(parse_integer<std::int32_t>("-2147483647").value(),    -2147483647);
    TEST_EQUAL(parse_integer<std::int32_t>("-123456789").value(),     -123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("-123456789abc").value(),  -123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("-01234567").value(),      -342391);
    TEST_EQUAL(parse_integer<std::int32_t>("-0xabcdef").value(),      -11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("-0xabcdefghi").value(),   -11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("-0b101010").value(),      -42);
    TEST_EQUAL(parse_integer<std::int32_t>("-123'456'789").value(),   -123456789);
    TEST_EQUAL(parse_integer<std::int32_t>("-0xabc'def").value(),     -11259375);
    TEST_EQUAL(parse_integer<std::int32_t>("-0b101'010").value(),     -42);
    TEST_EQUAL(parse_integer<std::int32_t>("-42abc").value(),         -42);
    TEST_EQUAL(parse_integer<std::int32_t>("-42_abc").value(),        -42);
    TEST_EQUAL(parse_integer<std::uint32_t>("0").value(),             0ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("1").value(),             1ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("4294967295").value(),    4294967295ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("123456789").value(),     123456789ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("123456789abc").value(),  123456789ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("01234567").value(),      342391ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("0xabcdef").value(),      11259375ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("0xabcdefghi").value(),   11259375ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("0b101010").value(),      42ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("123'456'789").value(),   123456789ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("0xabc'def").value(),     11259375ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("0b101'010").value(),     42ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("42abc").value(),         42ul);
    TEST_EQUAL(parse_integer<std::uint32_t>("42_abc").value(),        42ul);

    TEST(! parse_integer<std::int32_t>("").has_value());
    TEST(! parse_integer<std::uint32_t>("-1").has_value());
    TEST(! parse_integer<std::int32_t>("0123456789").has_value());
    TEST(! parse_integer<std::int32_t>("-0123456789").has_value());
    TEST(! parse_integer<std::uint32_t>("0123456789").has_value());

}
