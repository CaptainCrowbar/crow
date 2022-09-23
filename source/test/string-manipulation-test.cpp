#include "crow/string.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

void test_crow_common_prefix() {

    TEST_EQUAL(common_prefix_length("", ""), 0u);
    TEST_EQUAL(common_prefix_length("Hello", ""), 0u);
    TEST_EQUAL(common_prefix_length("Hello", "Hello"), 5u);
    TEST_EQUAL(common_prefix_length("Hello", "Hello world"), 5u);
    TEST_EQUAL(common_prefix_length("Hello world", "Hell world"), 4u);
    TEST_EQUAL(common_prefix_length("Hello world", "Goodbye"), 0u);

}

void test_crow_string_indentation() {

    std::string s;

    TEST_EQUAL(indent_lines(s), "");

    s = "Line one.\n"
        "Line two.\n"
        "Line three.\n";

    TEST_EQUAL(indent_lines(s),
        "    Line one.\n"
        "    Line two.\n"
        "    Line three.\n"
    );

    s = "Line one.\n\n"
        "Line three.\n\n"
        "Line five.\n\n";

    TEST_EQUAL(indent_lines(s),
        "    Line one.\n"
        "\n"
        "    Line three.\n"
        "\n"
        "    Line five.\n"
        "\n"
    );

}

void test_crow_string_join() {

    std::vector<std::string> vec;

    vec = {};                     TEST_EQUAL(join(vec, ""),    "");
    vec = {"abc"};                TEST_EQUAL(join(vec, ""),    "abc");
    vec = {"abc", "def"};         TEST_EQUAL(join(vec, ""),    "abcdef");
    vec = {"abc", "def", "ghi"};  TEST_EQUAL(join(vec, ""),    "abcdefghi");
    vec = {};                     TEST_EQUAL(join(vec, "::"),  "");
    vec = {"abc"};                TEST_EQUAL(join(vec, "::"),  "abc");
    vec = {"abc", "def"};         TEST_EQUAL(join(vec, "::"),  "abc::def");
    vec = {"abc", "def", "ghi"};  TEST_EQUAL(join(vec, "::"),  "abc::def::ghi");

}

void test_crow_string_padding() {

    TEST_EQUAL(pad_left("", 10),                   "          ");
    TEST_EQUAL(pad_left("", 10, '*'),              "**********");
    TEST_EQUAL(pad_left("Hello world", 10),        "Hello world");
    TEST_EQUAL(pad_left("Hello world", 15),        "    Hello world");
    TEST_EQUAL(pad_left("Hello world", 15, '*'),   "****Hello world");
    TEST_EQUAL(pad_right("", 10),                  "          ");
    TEST_EQUAL(pad_right("", 10, '*'),             "**********");
    TEST_EQUAL(pad_right("Hello world", 10),       "Hello world");
    TEST_EQUAL(pad_right("Hello world", 15),       "Hello world    ");
    TEST_EQUAL(pad_right("Hello world", 15, '*'),  "Hello world****");

}

void test_crow_string_partition() {

    std::pair<std::string, std::string> pair;

    TRY(pair = partition(""));                            TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "");
    TRY(pair = partition(" "));                           TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc"));                         TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc def"));                     TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def");
    TRY(pair = partition(" abc def"));                    TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "abc def");
    TRY(pair = partition("abc def ghi"));                 TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def ghi");
    TRY(pair = partition("abc\r\n"));                     TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc\r\ndef\r\n"));              TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def\r\n");
    TRY(pair = partition("abc\r\ndef\r\nghi\r\n"));       TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def\r\nghi\r\n");
    TRY(pair = partition("abc def ghi", ""));             TEST_EQUAL(pair.first, "abc def ghi");  TEST_EQUAL(pair.second, "");
    TRY(pair = partition("", "-+"));                      TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc", "-+"));                   TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc def", "-+"));               TEST_EQUAL(pair.first, "abc def");      TEST_EQUAL(pair.second, "");
    TRY(pair = partition("abc+-def", "-+"));              TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def");
    TRY(pair = partition("+-abc+-def", "-+"));            TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "abc+-def");
    TRY(pair = partition("abc+-def+-ghi", "-+"));         TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def+-ghi");
    TRY(pair = partition_at("", ""));                     TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "");
    TRY(pair = partition_at("abc def ghi", ""));          TEST_EQUAL(pair.first, "abc def ghi");  TEST_EQUAL(pair.second, "");
    TRY(pair = partition_at("", "::"));                   TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "");
    TRY(pair = partition_at("abc", "::"));                TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "");
    TRY(pair = partition_at("abc::def", "::"));           TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def");
    TRY(pair = partition_at("::abc::def", "::"));         TEST_EQUAL(pair.first, "");             TEST_EQUAL(pair.second, "abc::def");
    TRY(pair = partition_at("abc::def::ghi", "::"));      TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "def::ghi");
    TRY(pair = partition_at("abc::::def::::ghi", "::"));  TEST_EQUAL(pair.first, "abc");          TEST_EQUAL(pair.second, "::def::::ghi");

}

void test_crow_string_repeat() {

    TEST_EQUAL(repeat("abc", 0),   "");
    TEST_EQUAL(repeat("abc", 1),   "abc");
    TEST_EQUAL(repeat("abc", 2),   "abcabc");
    TEST_EQUAL(repeat("abc", 3),   "abcabcabc");
    TEST_EQUAL(repeat("abc", 4),   "abcabcabcabc");
    TEST_EQUAL(repeat("abc", 5),   "abcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 6),   "abcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 7),   "abcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 8),   "abcabcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 9),   "abcabcabcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 10),  "abcabcabcabcabcabcabcabcabcabc");

}

void test_crow_string_replace() {

    TEST_EQUAL(replace("", "", ""),                   "");
    TEST_EQUAL(replace("Hello world", "", ""),        "Hello world");
    TEST_EQUAL(replace("Hello world", "", "xyz"),     "Hello world");
    TEST_EQUAL(replace("Hello world", "o", ""),       "Hell wrld");
    TEST_EQUAL(replace("Hello world", "o", "***"),    "Hell*** w***rld");
    TEST_EQUAL(replace("Hello world", "abc", ""),     "Hello world");
    TEST_EQUAL(replace("Hello world", "abc", "xyz"),  "Hello world");

}

void test_crow_string_remove() {

    TEST_EQUAL(remove("", ""),                  "");
    TEST_EQUAL(remove("Hello world", ""),       "Hello world");
    TEST_EQUAL(remove("Hello world", "Hello"),  " world");
    TEST_EQUAL(remove("Hello world", "world"),  "Hello ");
    TEST_EQUAL(remove("Hello world", "o"),      "Hell wrld");
    TEST_EQUAL(remove("Hello world", "abc"),    "Hello world");

}

void test_crow_string_split() {

    std::vector<std::string> v;

    TRY(v = split(""));                                   TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split("\r\n"));                               TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split("alpha"));                              TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[alpha]");
    TRY(v = split("alpha bravo"));                        TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[alpha,bravo]");
    TRY(v = split("alpha bravo charlie"));                TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[alpha,bravo,charlie]");
    TRY(v = split("\r\nalpha\r\nbravo\r\ncharlie\r\n"));  TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[alpha,bravo,charlie]");
    TRY(v = split("", "+-"));                             TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split("+-", "+-"));                           TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split("alpha", "+-"));                        TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[alpha]");
    TRY(v = split("alpha+bravo", "+-"));                  TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[alpha,bravo]");
    TRY(v = split("alpha+bravo-charlie", "+-"));          TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[alpha,bravo,charlie]");
    TRY(v = split("+-alpha+-bravo+-charlie+-", "+-"));    TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[alpha,bravo,charlie]");

}

void test_crow_string_split_at() {

    std::vector<std::string> v;

    TRY(v = split_at("", "::"));                       TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split_at(":", "::"));                      TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[:]");
    TRY(v = split_at("::", "::"));                     TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[,]");
    TRY(v = split_at(":::", "::"));                    TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[,:]");
    TRY(v = split_at("::::", "::"));                   TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[,,]");
    TRY(v = split_at("abc", "::"));                    TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[abc]");
    TRY(v = split_at("abc::def", "::"));               TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[abc,def]");
    TRY(v = split_at("abc::def::ghi", "::"));          TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[abc,def,ghi]");
    TRY(v = split_at("::abc::::def::::ghi::", "::"));  TEST_EQUAL(v.size(), 7u);  TEST_EQUAL(format_range(v), "[,abc,,def,,ghi,]");

}

void test_crow_string_split_lines() {

    std::vector<std::string> v;

    TRY(v = split_lines(""));                            TEST_EQUAL(v.size(), 0u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split_lines("\n"));                          TEST_EQUAL(v.size(), 1u);  TEST_EQUAL(format_range(v), "[]");
    TRY(v = split_lines("\n\n"));                        TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[,]");
    TRY(v = split_lines("\n\n\n"));                      TEST_EQUAL(v.size(), 3u);  TEST_EQUAL(format_range(v), "[,,]");
    TRY(v = split_lines("Hello world\nGoodbye\n"));      TEST_EQUAL(v.size(), 2u);  TEST_EQUAL(format_range(v), "[Hello world,Goodbye]");
    TRY(v = split_lines("Hello world\n\nGoodbye\n\n"));  TEST_EQUAL(v.size(), 4u);  TEST_EQUAL(format_range(v), "[Hello world,,Goodbye,]");

}

void test_crow_string_trim() {

    TEST_EQUAL(trim(""),                                    "");
    TEST_EQUAL(trim("\r\n"),                                "");
    TEST_EQUAL(trim("Hello world"),                         "Hello world");
    TEST_EQUAL(trim(" Hello world "),                       "Hello world");
    TEST_EQUAL(trim("\r\nHello\r\nworld\r\n"),              "Hello\r\nworld");
    TEST_EQUAL(trim("", "+-"),                              "");
    TEST_EQUAL(trim(" ", "+-"),                             " ");
    TEST_EQUAL(trim("+-+-", "+-"),                          "");
    TEST_EQUAL(trim("+-\r\n+-", "+-"),                      "\r\n");
    TEST_EQUAL(trim("+-+-Hello+-+-world+-+-", "+-"),        "Hello+-+-world");
    TEST_EQUAL(trim_left(""),                               "");
    TEST_EQUAL(trim_left("\r\n"),                           "");
    TEST_EQUAL(trim_left("Hello world"),                    "Hello world");
    TEST_EQUAL(trim_left(" Hello world "),                  "Hello world ");
    TEST_EQUAL(trim_left("\r\nHello\r\nworld\r\n"),         "Hello\r\nworld\r\n");
    TEST_EQUAL(trim_left("", "+-"),                         "");
    TEST_EQUAL(trim_left(" ", "+-"),                        " ");
    TEST_EQUAL(trim_left("+-+-", "+-"),                     "");
    TEST_EQUAL(trim_left("+-\r\n+-", "+-"),                 "\r\n+-");
    TEST_EQUAL(trim_left("+-+-Hello+-+-world+-+-", "+-"),   "Hello+-+-world+-+-");
    TEST_EQUAL(trim_right(""),                              "");
    TEST_EQUAL(trim_right("\r\n"),                          "");
    TEST_EQUAL(trim_right("Hello world"),                   "Hello world");
    TEST_EQUAL(trim_right(" Hello world "),                 " Hello world");
    TEST_EQUAL(trim_right("\r\nHello\r\nworld\r\n"),        "\r\nHello\r\nworld");
    TEST_EQUAL(trim_right("", "+-"),                        "");
    TEST_EQUAL(trim_right(" ", "+-"),                       " ");
    TEST_EQUAL(trim_right("+-+-", "+-"),                    "");
    TEST_EQUAL(trim_right("+-\r\n+-", "+-"),                "+-\r\n");
    TEST_EQUAL(trim_right("+-+-Hello+-+-world+-+-", "+-"),  "+-+-Hello+-+-world");

}

void test_crow_string_unqualify() {

    TEST_EQUAL(unqualify(""),                  "");
    TEST_EQUAL(unqualify("abc.def.ghi"),       "ghi");
    TEST_EQUAL(unqualify("abc.def.ghi."),      "");
    TEST_EQUAL(unqualify("abc::def::ghi"),     "ghi");
    TEST_EQUAL(unqualify("abc.def.ghi", "/"),  "abc.def.ghi");
    TEST_EQUAL(unqualify("abc/def/ghi", "/"),  "ghi");

}
