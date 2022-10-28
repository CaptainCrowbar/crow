#include "crow/string.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

void test_crow_string_common_prefix() {

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

    std::pair<std::string_view, std::string_view> pair;

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

    std::vector<std::string_view> vs;
    std::vector<std::string> ss;

    TRY(vs = split(""));                                    TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split("\r\n"));                                TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split("alpha"));                               TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[alpha]");
    TRY(vs = split("alpha bravo"));                         TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[alpha,bravo]");
    TRY(vs = split("alpha bravo charlie"));                 TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split("\r\nalpha\r\nbravo\r\ncharlie\r\n"));   TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split("", "+-"));                              TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split("+-", "+-"));                            TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split("alpha", "+-"));                         TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[alpha]");
    TRY(vs = split("alpha+bravo", "+-"));                   TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[alpha,bravo]");
    TRY(vs = split("alpha+bravo-charlie", "+-"));           TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split("+-alpha+-bravo+-charlie+-", "+-"));     TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(ss = splits(""));                                   TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits("\r\n"));                               TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits("alpha"));                              TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[alpha]");
    TRY(ss = splits("alpha bravo"));                        TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[alpha,bravo]");
    TRY(ss = splits("alpha bravo charlie"));                TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = splits("\r\nalpha\r\nbravo\r\ncharlie\r\n"));  TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = splits("", "+-"));                             TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits("+-", "+-"));                           TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits("alpha", "+-"));                        TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[alpha]");
    TRY(ss = splits("alpha+bravo", "+-"));                  TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[alpha,bravo]");
    TRY(ss = splits("alpha+bravo-charlie", "+-"));          TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = splits("+-alpha+-bravo+-charlie+-", "+-"));    TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");

}

void test_crow_string_split_at() {

    std::vector<std::string_view> vs;
    std::vector<std::string> ss;

    TRY(vs = split_at("", "::"));                        TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_at(":", "::"));                       TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[:]");
    TRY(vs = split_at("::", "::"));                      TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,]");
    TRY(vs = split_at(":::", "::"));                     TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,:]");
    TRY(vs = split_at("::::", "::"));                    TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[,,]");
    TRY(vs = split_at("abc", "::"));                     TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[abc]");
    TRY(vs = split_at("abc::def", "::"));                TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[abc,def]");
    TRY(vs = split_at("abc::def::ghi", "::"));           TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[abc,def,ghi]");
    TRY(vs = split_at("::abc::::def::::ghi::", "::"));   TEST_EQUAL(vs.size(), 7u);  TEST_EQUAL(format_range(vs), "[,abc,,def,,ghi,]");
    TRY(ss = splits_at("", "::"));                       TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits_at(":", "::"));                      TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[:]");
    TRY(ss = splits_at("::", "::"));                     TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,]");
    TRY(ss = splits_at(":::", "::"));                    TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,:]");
    TRY(ss = splits_at("::::", "::"));                   TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[,,]");
    TRY(ss = splits_at("abc", "::"));                    TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[abc]");
    TRY(ss = splits_at("abc::def", "::"));               TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[abc,def]");
    TRY(ss = splits_at("abc::def::ghi", "::"));          TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[abc,def,ghi]");
    TRY(ss = splits_at("::abc::::def::::ghi::", "::"));  TEST_EQUAL(ss.size(), 7u);  TEST_EQUAL(format_range(ss), "[,abc,,def,,ghi,]");

}

void test_crow_string_split_lines() {

    std::vector<std::string_view> vs;
    std::vector<std::string> ss;

    TRY(vs = split_lines(""));                             TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines("\n"));                           TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines("\n\n"));                         TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,]");
    TRY(vs = split_lines("\n\n\n"));                       TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[,,]");
    TRY(vs = split_lines("Hello world\nGoodbye\n"));       TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[Hello world,Goodbye]");
    TRY(vs = split_lines("Hello world\n\nGoodbye\n\n"));   TEST_EQUAL(vs.size(), 4u);  TEST_EQUAL(format_range(vs), "[Hello world,,Goodbye,]");
    TRY(ss = splits_lines(""));                            TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits_lines("\n"));                          TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = splits_lines("\n\n"));                        TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,]");
    TRY(ss = splits_lines("\n\n\n"));                      TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[,,]");
    TRY(ss = splits_lines("Hello world\nGoodbye\n"));      TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[Hello world,Goodbye]");
    TRY(ss = splits_lines("Hello world\n\nGoodbye\n\n"));  TEST_EQUAL(ss.size(), 4u);  TEST_EQUAL(format_range(ss), "[Hello world,,Goodbye,]");

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
