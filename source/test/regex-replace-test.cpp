#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_regex_replace() {

    Regex r;
    std::string s1, s2;

    TRY(r = Regex("[a-z]"));
    s1 = "";

    s2 = "";  TRY(s2 = r.replace(s1, ""));                    TEST_EQUAL(s2, "");
    s2 = s1;  TRY(r.replace_in(s2, ""));                      TEST_EQUAL(s2, "");
    s2 = "";  TRY(s2 = r.replace(s1, "", 0, Regex::global));  TEST_EQUAL(s2, "");
    s2 = s1;  TRY(r.replace_in(s2, "", 0, Regex::global));    TEST_EQUAL(s2, "");

    s1 = "Hello world. Goodbye.";

    s2 = "";  TRY(s2 = r.replace(s1, "*"));                    TEST_EQUAL(s2, "H*llo world. Goodbye.");
    s2 = s1;  TRY(r.replace_in(s2, "*"));                      TEST_EQUAL(s2, "H*llo world. Goodbye.");
    s2 = "";  TRY(s2 = r.replace(s1, "*", 0, Regex::global));  TEST_EQUAL(s2, "H**** *****. G******.");
    s2 = s1;  TRY(r.replace_in(s2, "*", 0, Regex::global));    TEST_EQUAL(s2, "H**** *****. G******.");

    TRY(r = Regex("[a-z]+"));

    s2 = "";  TRY(s2 = r.replace(s1, "*"));                    TEST_EQUAL(s2, "H* world. Goodbye.");
    s2 = s1;  TRY(r.replace_in(s2, "*"));                      TEST_EQUAL(s2, "H* world. Goodbye.");
    s2 = "";  TRY(s2 = r.replace(s1, "*", 0, Regex::global));  TEST_EQUAL(s2, "H* *. G*.");
    s2 = s1;  TRY(r.replace_in(s2, "*", 0, Regex::global));    TEST_EQUAL(s2, "H* *. G*.");

    TRY(r = Regex("[a-z]+", Regex::icase));

    s2 = "";  TRY(s2 = r.replace(s1, "*"));                    TEST_EQUAL(s2, "* world. Goodbye.");
    s2 = s1;  TRY(r.replace_in(s2, "*"));                      TEST_EQUAL(s2, "* world. Goodbye.");
    s2 = "";  TRY(s2 = r.replace(s1, "*", 0, Regex::global));  TEST_EQUAL(s2, "* *. *.");
    s2 = s1;  TRY(r.replace_in(s2, "*", 0, Regex::global));    TEST_EQUAL(s2, "* *. *.");

    TRY(r = Regex("([a-z]+)(\\d+)?", Regex::icase));
    s1 = "abc123 def456 ghi789 XYZ";

    TRY(s2 = r.replace(s1, "<$1/$2>"));                                        TEST_EQUAL(s2, "<abc/123> def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "<$1/$2>", 0, Regex::global));                      TEST_EQUAL(s2, "<abc/123> <def/456> <ghi/789> <XYZ/>");
    TRY(s2 = r.replace(s1, "\\Q$1$2\\E"));                                     TEST_EQUAL(s2, "$1$2 def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\Q$1$2\\E", 0, Regex::global));                   TEST_EQUAL(s2, "$1$2 $1$2 $1$2 $1$2");
    TRY(s2 = r.replace(s1, "\\U$0\\E"));                                       TEST_EQUAL(s2, "ABC123 def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\U$0\\E", 0, Regex::global));                     TEST_EQUAL(s2, "ABC123 DEF456 GHI789 XYZ");
    TRY(s2 = r.replace(s1, "\\L$0\\E"));                                       TEST_EQUAL(s2, "abc123 def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\L$0\\E", 0, Regex::global));                     TEST_EQUAL(s2, "abc123 def456 ghi789 xyz");
    TRY(s2 = r.replace(s1, "\\u$0"));                                          TEST_EQUAL(s2, "Abc123 def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\u$0", 0, Regex::global));                        TEST_EQUAL(s2, "Abc123 Def456 Ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\l$0"));                                          TEST_EQUAL(s2, "abc123 def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "\\l$0", 0, Regex::global));                        TEST_EQUAL(s2, "abc123 def456 ghi789 xYZ");
    TRY(s2 = r.replace(s1, "<${1:-?}/${2:-?}>"));                              TEST_EQUAL(s2, "<abc/123> def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "<${1:-?}/${2:-?}>", 0, Regex::global));            TEST_EQUAL(s2, "<abc/123> <def/456> <ghi/789> <XYZ/?>");
    TRY(s2 = r.replace(s1, "<${1:+[$1]:?}/${2:+[$2]:?}>"));                    TEST_EQUAL(s2, "<[abc]/[123]> def456 ghi789 XYZ");
    TRY(s2 = r.replace(s1, "<${1:+[$1]:?}/${2:+[$2]:?}>", 0, Regex::global));  TEST_EQUAL(s2, "<[abc]/[123]> <[def]/[456]> <[ghi]/[789]> <[XYZ]/?>");

}

void test_crow_regex_transform() {

    Regex::transform rt;
    std::string s1, s2;

    s1 = "";             s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, "");
    s1 = "";             s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, "");
    s1 = "Hello world";  s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, s1);
    s1 = "Hello world";  s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, s1);

    TRY(rt = Regex::transform("[a-z]+", "(\\U$0)"));

    s1 = "";             s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, "");
    s1 = "";             s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, "");
    s1 = "Hello world";  s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, "H(ELLO) world");
    s1 = "Hello world";  s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, "H(ELLO) world");

    TRY(rt = Regex::transform("[a-z]+", "(\\U$0)", Regex::global | Regex::icase));

    s1 = "";             s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, "");
    s1 = "";             s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, "");
    s1 = "Hello world";  s2 = "";  TRY(s2 = rt(s1));        TEST_EQUAL(s2, "(HELLO) (WORLD)");
    s1 = "Hello world";  s2 = s1;  TRY(rt.replace_in(s2));  TEST_EQUAL(s2, "(HELLO) (WORLD)");

}
