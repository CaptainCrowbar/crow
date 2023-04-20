#include "crow/string.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Literals;

void test_crow_string_literals() {

    std::string s;
    std::vector<std::string> v;

    TRY(s = ""_doc);
    TEST_EQUAL(s, "");

    TRY(s = R"(
        )"_doc);
    TEST_EQUAL(s, "");

    TRY(s = R"(
        Hello world
        )"_doc);
    TEST_EQUAL(s,
        "Hello world\n"
    );

    TRY(s = R"(
        Hello world
    )"_doc);
    TEST_EQUAL(s,
        "    Hello world\n"
    );

    TRY(s = R"(
        Hello world
            Goodnight moon

        One fish two fish
        )"_doc);
    TEST_EQUAL(s,
        "Hello world\n"
        "    Goodnight moon\n"
        "\n"
        "One fish two fish\n"
    );

    TRY(s = R"(
        Hello world
            Goodnight moon

        One fish two fish
    )"_doc);
    TEST_EQUAL(s,
        "    Hello world\n"
        "        Goodnight moon\n"
        "\n"
        "    One fish two fish\n"
    );

    TRY(v = ""_qw);                        TEST_EQUAL(v.size(), 0u);  TRY(s = format_range(v));  TEST_EQUAL(s, "[]");
    TRY(v = "\r\n"_qw);                    TEST_EQUAL(v.size(), 0u);  TRY(s = format_range(v));  TEST_EQUAL(s, "[]");
    TRY(v = "Hello"_qw);                   TEST_EQUAL(v.size(), 1u);  TRY(s = format_range(v));  TEST_EQUAL(s, "[Hello]");
    TRY(v = "Hello world"_qw);             TEST_EQUAL(v.size(), 2u);  TRY(s = format_range(v));  TEST_EQUAL(s, "[Hello,world]");
    TRY(v = "\tabc\tdef\tghi\tjkl\t"_qw);  TEST_EQUAL(v.size(), 4u);  TRY(s = format_range(v));  TEST_EQUAL(s, "[abc,def,ghi,jkl]");

}
