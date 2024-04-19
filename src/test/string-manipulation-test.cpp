#include "crow/string.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace Crow;
using namespace std::literals;

void test_crow_string_comparison() {

    TEST_EQUAL(format_object(""s <=> ""s),              "equal");
    TEST_EQUAL(format_object(""s <=> "hello"s),         "less");
    TEST_EQUAL(format_object("hello"s <=> ""s),         "greater");
    TEST_EQUAL(format_object("hello"s <=> "hello"s),    "equal");
    TEST_EQUAL(format_object("hell"s <=> "hello"s),     "less");
    TEST_EQUAL(format_object("hello"s <=> "hell"s),     "greater");
    TEST_EQUAL(format_object("hello"s <=> "world"s),    "less");
    TEST_EQUAL(format_object("world"s <=> "hello"s),    "greater");

    TEST_EQUAL(format_object(""sv <=> ""sv),            "equal");
    TEST_EQUAL(format_object(""sv <=> "hello"sv),       "less");
    TEST_EQUAL(format_object("hello"sv <=> ""sv),       "greater");
    TEST_EQUAL(format_object("hello"sv <=> "hello"sv),  "equal");
    TEST_EQUAL(format_object("hell"sv <=> "hello"sv),   "less");
    TEST_EQUAL(format_object("hello"sv <=> "hell"sv),   "greater");
    TEST_EQUAL(format_object("hello"sv <=> "world"sv),  "less");
    TEST_EQUAL(format_object("world"sv <=> "hello"sv),  "greater");

    TEST_EQUAL(format_object(""s <=> ""sv),             "equal");
    TEST_EQUAL(format_object(""s <=> "hello"sv),        "less");
    TEST_EQUAL(format_object("hello"s <=> ""sv),        "greater");
    TEST_EQUAL(format_object("hello"s <=> "hello"sv),   "equal");
    TEST_EQUAL(format_object("hell"s <=> "hello"sv),    "less");
    TEST_EQUAL(format_object("hello"s <=> "hell"sv),    "greater");
    TEST_EQUAL(format_object("hello"s <=> "world"sv),   "less");
    TEST_EQUAL(format_object("world"s <=> "hello"sv),   "greater");

    TEST_EQUAL(format_object(""sv <=> ""s),             "equal");
    TEST_EQUAL(format_object(""sv <=> "hello"s),        "less");
    TEST_EQUAL(format_object("hello"sv <=> ""s),        "greater");
    TEST_EQUAL(format_object("hello"sv <=> "hello"s),   "equal");
    TEST_EQUAL(format_object("hell"sv <=> "hello"s),    "less");
    TEST_EQUAL(format_object("hello"sv <=> "hell"s),    "greater");
    TEST_EQUAL(format_object("hello"sv <=> "world"s),   "less");
    TEST_EQUAL(format_object("world"sv <=> "hello"s),   "greater");

}

void test_crow_string_common_prefix() {

    TEST_EQUAL(common_prefix_length("", ""), 0u);
    TEST_EQUAL(common_prefix_length("Hello", ""), 0u);
    TEST_EQUAL(common_prefix_length("Hello", "Hello"), 5u);
    TEST_EQUAL(common_prefix_length("Hello", "Hello world"), 5u);
    TEST_EQUAL(common_prefix_length("Hello world", "Hell world"), 4u);
    TEST_EQUAL(common_prefix_length("Hello world", "Goodbye"), 0u);

}

void test_crow_string_concatenation() {

    std::string s;

    TRY(s = concat());                              TEST_EQUAL(s, "");
    TRY(s = concat("Hello"));                       TEST_EQUAL(s, "Hello");
    TRY(s = concat("Hello"s));                      TEST_EQUAL(s, "Hello");
    TRY(s = concat("Hello"sv));                     TEST_EQUAL(s, "Hello");
    TRY(s = concat(U"Hello"));                      TEST_EQUAL(s, "Hello");
    TRY(s = concat(U"Hello"s));                     TEST_EQUAL(s, "Hello");
    TRY(s = concat(U"Hello"sv));                    TEST_EQUAL(s, "Hello");
    TRY(s = concat('A'));                           TEST_EQUAL(s, "A");
    TRY(s = concat(U'A'));                          TEST_EQUAL(s, "A");
    TRY(s = concat(U'α'));                          TEST_EQUAL(s, "α");
    TRY(s = concat("Hello", "World"));              TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello"s, "World"));             TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello"sv, "World"));            TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello", "World"));             TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello"s, "World"));            TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello"sv, "World"));           TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello", U"World"sv));           TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello"s, U"World"sv));          TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello"sv, U"World"sv));         TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello", U"World"sv));          TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello"s, U"World"sv));         TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat(U"Hello"sv, U"World"sv));        TEST_EQUAL(s, "HelloWorld");
    TRY(s = concat("Hello", ' ', "World"));         TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", ' ', "World"s));        TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", ' ', "World"sv));       TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", ' ', U"World"));        TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", ' ', U"World"s));       TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", ' ', U"World"sv));      TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', "World"));      TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', "World"s));     TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', "World"sv));    TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', U"World"));     TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', U"World"s));    TEST_EQUAL(s, "Hello World");
    TRY(s = concat(U"Hello"sv, ' ', U"World"sv));   TEST_EQUAL(s, "Hello World");
    TRY(s = concat("Hello", U'×', "World"));        TEST_EQUAL(s, "Hello×World");
    TRY(s = concat("Hello", U'×', "World"s));       TEST_EQUAL(s, "Hello×World");
    TRY(s = concat("Hello", U'×', "World"sv));      TEST_EQUAL(s, "Hello×World");
    TRY(s = concat("Hello", U'×', U"World"));       TEST_EQUAL(s, "Hello×World");
    TRY(s = concat("Hello", U'×', U"World"s));      TEST_EQUAL(s, "Hello×World");
    TRY(s = concat("Hello", U'×', U"World"sv));     TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', "World"));     TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', "World"s));    TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', "World"sv));   TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', U"World"));    TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', U"World"s));   TEST_EQUAL(s, "Hello×World");
    TRY(s = concat(U"Hello"sv, U'×', U"World"sv));  TEST_EQUAL(s, "Hello×World");

}

void test_crow_string_escape_and_quote() {

    // MSVC 2022 has a bug with backslashes in raw strings

    TEST_EQUAL(escape(""),                             "");
    TEST_EQUAL(escape("Hello world"),                  "Hello world");
    TEST_EQUAL(escape("Hello world\r\n"),              "Hello world\\r\\n");
    TEST_EQUAL(escape("\"Hello\\world\""),             "\"Hello\\\\world\"");
    TEST_EQUAL(escape("αβγδε"),                        "αβγδε");
    TEST_EQUAL(escape("Hello\xffworld"),               "Hello\\xffworld");
    TEST_EQUAL(unescape(""),                           "");
    TEST_EQUAL(unescape("Hello world"),                "Hello world");
    TEST_EQUAL(unescape("Hello world\\r\\n"),          "Hello world\r\n");
    TEST_EQUAL(unescape("\"Hello\\\\world\""),         "\"Hello\\world\"");
    TEST_EQUAL(unescape("αβγδε"),                      "αβγδε");
    TEST_EQUAL(unescape("Hello\\xffworld"),            "Hello\xffworld");
    TEST_EQUAL(quote(""),                              "\"\"");
    TEST_EQUAL(quote("Hello world"),                   "\"Hello world\"");
    TEST_EQUAL(quote("Hello world\r\n"),               "\"Hello world\\r\\n\"");
    TEST_EQUAL(quote("\"Hello\\world\""),              "\"\\\"Hello\\\\world\\\"\"");
    TEST_EQUAL(quote("αβγδε"),                         "\"αβγδε\"");
    TEST_EQUAL(quote("Hello\xffworld"),                "\"Hello\\xffworld\"");
    TEST_EQUAL(quote("", '\''),                        "''");
    TEST_EQUAL(quote("Hello world", '\''),             "'Hello world'");
    TEST_EQUAL(quote("Hello world\r\n", '\''),         "'Hello world\\r\\n'");
    TEST_EQUAL(quote("\"Hello\\world\"", '\''),        "'\"Hello\\\\world\"'");
    TEST_EQUAL(quote("αβγδε", '\''),                   "'αβγδε'");
    TEST_EQUAL(quote("Hello\xffworld", '\''),          "'Hello\\xffworld'");
    TEST_EQUAL(unquote("\"\""),                        "");
    TEST_EQUAL(unquote("\"Hello world\""),             "Hello world");
    TEST_EQUAL(unquote("\"Hello world\\r\\n\""),       "Hello world\r\n");
    TEST_EQUAL(unquote("\"\\\"Hello\\\\world\\\"\""),  "\"Hello\\world\"");
    TEST_EQUAL(unquote("\"αβγδε\""),                   "αβγδε");
    TEST_EQUAL(unquote("\"Hello\\xffworld\""),         "Hello\xffworld");
    TEST_EQUAL(unquote("''", '\''),                    "");
    TEST_EQUAL(unquote("'Hello world'", '\''),         "Hello world");
    TEST_EQUAL(unquote("'Hello world\\r\\n'", '\''),   "Hello world\r\n");
    TEST_EQUAL(unquote("'\"Hello\\\\world\"'", '\''),  "\"Hello\\world\"");
    TEST_EQUAL(unquote("'αβγδε'", '\''),               "αβγδε");
    TEST_EQUAL(unquote("'Hello\\xffworld'", '\''),     "Hello\xffworld");

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

    std::vector<std::string> ss;
    std::vector<std::string_view> vs;

    TRY(ss = split(""));                                     TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split("\r\n"));                                 TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split("alpha"));                                TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[alpha]");
    TRY(ss = split("alpha bravo"));                          TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[alpha,bravo]");
    TRY(ss = split("alpha bravo charlie"));                  TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = split("\r\nalpha\r\nbravo\r\ncharlie\r\n"));    TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = split("", "+-"));                               TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split("+-", "+-"));                             TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split("alpha", "+-"));                          TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[alpha]");
    TRY(ss = split("alpha+bravo", "+-"));                    TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[alpha,bravo]");
    TRY(ss = split("alpha+bravo-charlie", "+-"));            TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(ss = split("+-alpha+-bravo+-charlie+-", "+-"));      TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[alpha,bravo,charlie]");
    TRY(vs = split_v(""));                                   TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_v("\r\n"));                               TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_v("alpha"));                              TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[alpha]");
    TRY(vs = split_v("alpha bravo"));                        TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[alpha,bravo]");
    TRY(vs = split_v("alpha bravo charlie"));                TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split_v("\r\nalpha\r\nbravo\r\ncharlie\r\n"));  TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split_v("", "+-"));                             TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_v("+-", "+-"));                           TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_v("alpha", "+-"));                        TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[alpha]");
    TRY(vs = split_v("alpha+bravo", "+-"));                  TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[alpha,bravo]");
    TRY(vs = split_v("alpha+bravo-charlie", "+-"));          TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");
    TRY(vs = split_v("+-alpha+-bravo+-charlie+-", "+-"));    TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[alpha,bravo,charlie]");

}

void test_crow_string_split_at() {

    std::vector<std::string> ss;
    std::vector<std::string_view> vs;

    TRY(ss = split_at("", "::"));                         TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_at(":", "::"));                        TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[:]");
    TRY(ss = split_at("::", "::"));                       TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,]");
    TRY(ss = split_at(":::", "::"));                      TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,:]");
    TRY(ss = split_at("::::", "::"));                     TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[,,]");
    TRY(ss = split_at("abc", "::"));                      TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[abc]");
    TRY(ss = split_at("abc::def", "::"));                 TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[abc,def]");
    TRY(ss = split_at("abc::def::ghi", "::"));            TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[abc,def,ghi]");
    TRY(ss = split_at("::abc::::def::::ghi::", "::"));    TEST_EQUAL(ss.size(), 7u);  TEST_EQUAL(format_range(ss), "[,abc,,def,,ghi,]");
    TRY(vs = split_at_v("", "::"));                       TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_at_v(":", "::"));                      TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[:]");
    TRY(vs = split_at_v("::", "::"));                     TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,]");
    TRY(vs = split_at_v(":::", "::"));                    TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,:]");
    TRY(vs = split_at_v("::::", "::"));                   TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[,,]");
    TRY(vs = split_at_v("abc", "::"));                    TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[abc]");
    TRY(vs = split_at_v("abc::def", "::"));               TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[abc,def]");
    TRY(vs = split_at_v("abc::def::ghi", "::"));          TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[abc,def,ghi]");
    TRY(vs = split_at_v("::abc::::def::::ghi::", "::"));  TEST_EQUAL(vs.size(), 7u);  TEST_EQUAL(format_range(vs), "[,abc,,def,,ghi,]");

}

void test_crow_string_split_lines() {

    std::vector<std::string> ss;
    std::vector<std::string_view> vs;

    TRY(ss = split_lines(""));                                            TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\n"));                                          TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\n\n"));                                        TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,]");
    TRY(ss = split_lines("\n\n\n"));                                      TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[,,]");
    TRY(ss = split_lines("Hello world\nGoodbye\n"));                      TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[Hello world,Goodbye]");
    TRY(ss = split_lines("Hello world\n\nGoodbye\n\n"));                  TEST_EQUAL(ss.size(), 4u);  TEST_EQUAL(format_range(ss), "[Hello world,,Goodbye,]");
    TRY(ss = split_lines("", true));                                      TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\n", true));                                    TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[\n]");
    TRY(ss = split_lines("\n\n", true));                                  TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[\n,\n]");
    TRY(ss = split_lines("\n\n\n", true));                                TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[\n,\n,\n]");
    TRY(ss = split_lines("Hello world\nGoodbye\n", true));                TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[Hello world\n,Goodbye\n]");
    TRY(ss = split_lines("Hello world\n\nGoodbye\n\n", true));            TEST_EQUAL(ss.size(), 4u);  TEST_EQUAL(format_range(ss), "[Hello world\n,\n,Goodbye\n,\n]");
    TRY(ss = split_lines(""));                                            TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\r\n"));                                        TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\r\n\r\n"));                                    TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[,]");
    TRY(ss = split_lines("\r\n\r\n\r\n"));                                TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[,,]");
    TRY(ss = split_lines("Hello world\r\nGoodbye\r\n"));                  TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[Hello world,Goodbye]");
    TRY(ss = split_lines("Hello world\r\n\r\nGoodbye\r\n\r\n"));          TEST_EQUAL(ss.size(), 4u);  TEST_EQUAL(format_range(ss), "[Hello world,,Goodbye,]");
    TRY(ss = split_lines("", true));                                      TEST_EQUAL(ss.size(), 0u);  TEST_EQUAL(format_range(ss), "[]");
    TRY(ss = split_lines("\r\n", true));                                  TEST_EQUAL(ss.size(), 1u);  TEST_EQUAL(format_range(ss), "[\r\n]");
    TRY(ss = split_lines("\r\n\r\n", true));                              TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[\r\n,\r\n]");
    TRY(ss = split_lines("\r\n\r\n\r\n", true));                          TEST_EQUAL(ss.size(), 3u);  TEST_EQUAL(format_range(ss), "[\r\n,\r\n,\r\n]");
    TRY(ss = split_lines("Hello world\r\nGoodbye\r\n", true));            TEST_EQUAL(ss.size(), 2u);  TEST_EQUAL(format_range(ss), "[Hello world\r\n,Goodbye\r\n]");
    TRY(ss = split_lines("Hello world\r\n\r\nGoodbye\r\n\r\n", true));    TEST_EQUAL(ss.size(), 4u);  TEST_EQUAL(format_range(ss), "[Hello world\r\n,\r\n,Goodbye\r\n,\r\n]");
    TRY(vs = split_lines_v(""));                                          TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\n"));                                        TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\n\n"));                                      TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,]");
    TRY(vs = split_lines_v("\n\n\n"));                                    TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[,,]");
    TRY(vs = split_lines_v("Hello world\nGoodbye\n"));                    TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[Hello world,Goodbye]");
    TRY(vs = split_lines_v("Hello world\n\nGoodbye\n\n"));                TEST_EQUAL(vs.size(), 4u);  TEST_EQUAL(format_range(vs), "[Hello world,,Goodbye,]");
    TRY(vs = split_lines_v("", true));                                    TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\n", true));                                  TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[\n]");
    TRY(vs = split_lines_v("\n\n", true));                                TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[\n,\n]");
    TRY(vs = split_lines_v("\n\n\n", true));                              TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[\n,\n,\n]");
    TRY(vs = split_lines_v("Hello world\nGoodbye\n", true));              TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[Hello world\n,Goodbye\n]");
    TRY(vs = split_lines_v("Hello world\n\nGoodbye\n\n", true));          TEST_EQUAL(vs.size(), 4u);  TEST_EQUAL(format_range(vs), "[Hello world\n,\n,Goodbye\n,\n]");
    TRY(vs = split_lines_v(""));                                          TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\r\n"));                                      TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\r\n\r\n"));                                  TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[,]");
    TRY(vs = split_lines_v("\r\n\r\n\r\n"));                              TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[,,]");
    TRY(vs = split_lines_v("Hello world\r\nGoodbye\r\n"));                TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[Hello world,Goodbye]");
    TRY(vs = split_lines_v("Hello world\r\n\r\nGoodbye\r\n\r\n"));        TEST_EQUAL(vs.size(), 4u);  TEST_EQUAL(format_range(vs), "[Hello world,,Goodbye,]");
    TRY(vs = split_lines_v("", true));                                    TEST_EQUAL(vs.size(), 0u);  TEST_EQUAL(format_range(vs), "[]");
    TRY(vs = split_lines_v("\r\n", true));                                TEST_EQUAL(vs.size(), 1u);  TEST_EQUAL(format_range(vs), "[\r\n]");
    TRY(vs = split_lines_v("\r\n\r\n", true));                            TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[\r\n,\r\n]");
    TRY(vs = split_lines_v("\r\n\r\n\r\n", true));                        TEST_EQUAL(vs.size(), 3u);  TEST_EQUAL(format_range(vs), "[\r\n,\r\n,\r\n]");
    TRY(vs = split_lines_v("Hello world\r\nGoodbye\r\n", true));          TEST_EQUAL(vs.size(), 2u);  TEST_EQUAL(format_range(vs), "[Hello world\r\n,Goodbye\r\n]");
    TRY(vs = split_lines_v("Hello world\r\n\r\nGoodbye\r\n\r\n", true));  TEST_EQUAL(vs.size(), 4u);  TEST_EQUAL(format_range(vs), "[Hello world\r\n,\r\n,Goodbye\r\n,\r\n]");

}

void test_crow_string_trim() {

    TEST_EQUAL(trim(""),                                      "");
    TEST_EQUAL(trim("\r\n"),                                  "");
    TEST_EQUAL(trim("Hello world"),                           "Hello world");
    TEST_EQUAL(trim(" Hello world "),                         "Hello world");
    TEST_EQUAL(trim("\r\nHello\r\nworld\r\n"),                "Hello\r\nworld");
    TEST_EQUAL(trim("", "+-"),                                "");
    TEST_EQUAL(trim(" ", "+-"),                               " ");
    TEST_EQUAL(trim("+-+-", "+-"),                            "");
    TEST_EQUAL(trim("+-\r\n+-", "+-"),                        "\r\n");
    TEST_EQUAL(trim("+-+-Hello+-+-world+-+-", "+-"),          "Hello+-+-world");
    TEST_EQUAL(trim_left(""),                                 "");
    TEST_EQUAL(trim_left("\r\n"),                             "");
    TEST_EQUAL(trim_left("Hello world"),                      "Hello world");
    TEST_EQUAL(trim_left(" Hello world "),                    "Hello world ");
    TEST_EQUAL(trim_left("\r\nHello\r\nworld\r\n"),           "Hello\r\nworld\r\n");
    TEST_EQUAL(trim_left("", "+-"),                           "");
    TEST_EQUAL(trim_left(" ", "+-"),                          " ");
    TEST_EQUAL(trim_left("+-+-", "+-"),                       "");
    TEST_EQUAL(trim_left("+-\r\n+-", "+-"),                   "\r\n+-");
    TEST_EQUAL(trim_left("+-+-Hello+-+-world+-+-", "+-"),     "Hello+-+-world+-+-");
    TEST_EQUAL(trim_right(""),                                "");
    TEST_EQUAL(trim_right("\r\n"),                            "");
    TEST_EQUAL(trim_right("Hello world"),                     "Hello world");
    TEST_EQUAL(trim_right(" Hello world "),                   " Hello world");
    TEST_EQUAL(trim_right("\r\nHello\r\nworld\r\n"),          "\r\nHello\r\nworld");
    TEST_EQUAL(trim_right("", "+-"),                          "");
    TEST_EQUAL(trim_right(" ", "+-"),                         " ");
    TEST_EQUAL(trim_right("+-+-", "+-"),                      "");
    TEST_EQUAL(trim_right("+-\r\n+-", "+-"),                  "+-\r\n");
    TEST_EQUAL(trim_right("+-+-Hello+-+-world+-+-", "+-"),    "+-+-Hello+-+-world");
    TEST_EQUAL(trim_v(""),                                    "");
    TEST_EQUAL(trim_v("\r\n"),                                "");
    TEST_EQUAL(trim_v("Hello world"),                         "Hello world");
    TEST_EQUAL(trim_v(" Hello world "),                       "Hello world");
    TEST_EQUAL(trim_v("\r\nHello\r\nworld\r\n"),              "Hello\r\nworld");
    TEST_EQUAL(trim_v("", "+-"),                              "");
    TEST_EQUAL(trim_v(" ", "+-"),                             " ");
    TEST_EQUAL(trim_v("+-+-", "+-"),                          "");
    TEST_EQUAL(trim_v("+-\r\n+-", "+-"),                      "\r\n");
    TEST_EQUAL(trim_v("+-+-Hello+-+-world+-+-", "+-"),        "Hello+-+-world");
    TEST_EQUAL(trim_left_v(""),                               "");
    TEST_EQUAL(trim_left_v("\r\n"),                           "");
    TEST_EQUAL(trim_left_v("Hello world"),                    "Hello world");
    TEST_EQUAL(trim_left_v(" Hello world "),                  "Hello world ");
    TEST_EQUAL(trim_left_v("\r\nHello\r\nworld\r\n"),         "Hello\r\nworld\r\n");
    TEST_EQUAL(trim_left_v("", "+-"),                         "");
    TEST_EQUAL(trim_left_v(" ", "+-"),                        " ");
    TEST_EQUAL(trim_left_v("+-+-", "+-"),                     "");
    TEST_EQUAL(trim_left_v("+-\r\n+-", "+-"),                 "\r\n+-");
    TEST_EQUAL(trim_left_v("+-+-Hello+-+-world+-+-", "+-"),   "Hello+-+-world+-+-");
    TEST_EQUAL(trim_right_v(""),                              "");
    TEST_EQUAL(trim_right_v("\r\n"),                          "");
    TEST_EQUAL(trim_right_v("Hello world"),                   "Hello world");
    TEST_EQUAL(trim_right_v(" Hello world "),                 " Hello world");
    TEST_EQUAL(trim_right_v("\r\nHello\r\nworld\r\n"),        "\r\nHello\r\nworld");
    TEST_EQUAL(trim_right_v("", "+-"),                        "");
    TEST_EQUAL(trim_right_v(" ", "+-"),                       " ");
    TEST_EQUAL(trim_right_v("+-+-", "+-"),                    "");
    TEST_EQUAL(trim_right_v("+-\r\n+-", "+-"),                "+-\r\n");
    TEST_EQUAL(trim_right_v("+-+-Hello+-+-world+-+-", "+-"),  "+-+-Hello+-+-world");

}

void test_crow_string_unqualify() {

    TEST_EQUAL(unqualify(""),                    "");
    TEST_EQUAL(unqualify("abc.def.ghi"),         "ghi");
    TEST_EQUAL(unqualify("abc.def.ghi."),        "");
    TEST_EQUAL(unqualify("abc::def::ghi"),       "ghi");
    TEST_EQUAL(unqualify("abc.def.ghi", "/"),    "abc.def.ghi");
    TEST_EQUAL(unqualify("abc/def/ghi", "/"),    "ghi");
    TEST_EQUAL(unqualify_v(""),                  "");
    TEST_EQUAL(unqualify_v("abc.def.ghi"),       "ghi");
    TEST_EQUAL(unqualify_v("abc.def.ghi."),      "");
    TEST_EQUAL(unqualify_v("abc::def::ghi"),     "ghi");
    TEST_EQUAL(unqualify_v("abc.def.ghi", "/"),  "abc.def.ghi");
    TEST_EQUAL(unqualify_v("abc/def/ghi", "/"),  "ghi");

}
