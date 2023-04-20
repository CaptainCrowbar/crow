#include "crow/format-range.hpp"
#include "crow/format-type.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_format_string() {

    TEST_EQUAL(format_string("",             ""),     "");
    TEST_EQUAL(format_string("",             "s"),    "");
    TEST_EQUAL(format_string("",             "s5"),   "");
    TEST_EQUAL(format_string("",             "q"),    "\"\"");
    TEST_EQUAL(format_string("",             "q5"),   "\"\"");
    TEST_EQUAL(format_string("",             "x"),    "");
    TEST_EQUAL(format_string("",             "x5"),   "");
    TEST_EQUAL(format_string("",             "X"),    "");
    TEST_EQUAL(format_string("Hello world",  ""),     "Hello world");
    TEST_EQUAL(format_string("Hello world",  "s"),    "Hello world");
    TEST_EQUAL(format_string("Hello world",  "s5"),   "Hello...");
    TEST_EQUAL(format_string("Hello world",  "q"),    "\"Hello world\"");
    TEST_EQUAL(format_string("Hello world",  "q5"),   "\"Hello...\"");
    TEST_EQUAL(format_string("Hello world",  "x"),    "48 65 6c 6c 6f 20 77 6f 72 6c 64");
    TEST_EQUAL(format_string("Hello world",  "x5"),   "48 65 6c 6c 6f...");
    TEST_EQUAL(format_string("Hello world",  "X"),    "48 65 6C 6C 6F 20 77 6F 72 6C 64");
    TEST_EQUAL(format_string("Hello world",  "xz"),   "48656c6c6f20776f726c64");
    TEST_EQUAL(format_string("Hello world",  "xz5"),  "48656c6c6f...");
    TEST_EQUAL(format_string("Hello world",  "Xz"),   "48656C6C6F20776F726C64");
    TEST_EQUAL(format_string("αβγδε",        ""),     "αβγδε");
    TEST_EQUAL(format_string("αβγδε",        "q"),    "\"αβγδε\"");
    TEST_EQUAL(format_string("αβγδε",        "x"),    "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_string("αβγδε",        "xz"),   "ceb1ceb2ceb3ceb4ceb5");
    TEST_EQUAL(format_string(u"αβγδε",       ""),     "αβγδε");
    TEST_EQUAL(format_string(u"αβγδε",       "q"),    "\"αβγδε\"");
    TEST_EQUAL(format_string(u"αβγδε",       "x"),    "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_string(u"αβγδε",       "xz"),   "ceb1ceb2ceb3ceb4ceb5");
    TEST_EQUAL(format_string(U"αβγδε",       ""),     "αβγδε");
    TEST_EQUAL(format_string(U"αβγδε",       "q"),    "\"αβγδε\"");
    TEST_EQUAL(format_string(U"αβγδε",       "x"),    "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_string(U"αβγδε",       "xz"),   "ceb1ceb2ceb3ceb4ceb5");
    TEST_EQUAL(format_string(L"αβγδε",       ""),     "αβγδε");
    TEST_EQUAL(format_string(L"αβγδε",       "q"),    "\"αβγδε\"");
    TEST_EQUAL(format_string(L"αβγδε",       "x"),    "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_string(L"αβγδε",       "xz"),   "ceb1ceb2ceb3ceb4ceb5");

}

void test_crow_format_ranges() {

    std::vector<int> v;

    v = {};               TEST_EQUAL(format_object(v), "[]");               TEST_EQUAL(format_object(v, "x4"), "[]");
    v = {32};             TEST_EQUAL(format_object(v), "[32]");             TEST_EQUAL(format_object(v, "x4"), "[0020]");
    v = {32,64};          TEST_EQUAL(format_object(v), "[32,64]");          TEST_EQUAL(format_object(v, "x4"), "[0020,0040]");
    v = {32,64,128};      TEST_EQUAL(format_object(v), "[32,64,128]");      TEST_EQUAL(format_object(v, "x4"), "[0020,0040,0080]");
    v = {32,64,128,256};  TEST_EQUAL(format_object(v), "[32,64,128,256]");  TEST_EQUAL(format_object(v, "x4"), "[0020,0040,0080,0100]");

    v = {};               TEST_EQUAL(format_object(v, "*N"), "");               TEST_EQUAL(format_object(v, "xN4"), "");
    v = {32};             TEST_EQUAL(format_object(v, "*N"), "32");             TEST_EQUAL(format_object(v, "xN4"), "0020");
    v = {32,64};          TEST_EQUAL(format_object(v, "*N"), "32,64");          TEST_EQUAL(format_object(v, "xN4"), "0020,0040");
    v = {32,64,128};      TEST_EQUAL(format_object(v, "*N"), "32,64,128");      TEST_EQUAL(format_object(v, "xN4"), "0020,0040,0080");
    v = {32,64,128,256};  TEST_EQUAL(format_object(v, "*N"), "32,64,128,256");  TEST_EQUAL(format_object(v, "xN4"), "0020,0040,0080,0100");

    std::map<int, std::string> m;

    m = {};                                         TEST_EQUAL(format_object(m), "{}");                         TEST_EQUAL(format_object(m, "n2"), "{}");
    m = {{1,"abc"}};                                TEST_EQUAL(format_object(m), "{1:abc}");                    TEST_EQUAL(format_object(m, "n2"), "{01:abc}");
    m = {{1,"abc"},{2,"def"}};                      TEST_EQUAL(format_object(m), "{1:abc,2:def}");              TEST_EQUAL(format_object(m, "n2"), "{01:abc,02:def}");
    m = {{1,"abc"},{2,"def"},{3,"ghi"}};            TEST_EQUAL(format_object(m), "{1:abc,2:def,3:ghi}");        TEST_EQUAL(format_object(m, "n2"), "{01:abc,02:def,03:ghi}");
    m = {{1,"abc"},{2,"def"},{3,"ghi"},{4,"jkl"}};  TEST_EQUAL(format_object(m), "{1:abc,2:def,3:ghi,4:jkl}");  TEST_EQUAL(format_object(m, "n2"), "{01:abc,02:def,03:ghi,04:jkl}");

    m = {};                                         TEST_EQUAL(format_object(m, "*N"), "");                         TEST_EQUAL(format_object(m, "nN2"), "");
    m = {{1,"abc"}};                                TEST_EQUAL(format_object(m, "*N"), "1:abc");                    TEST_EQUAL(format_object(m, "nN2"), "01:abc");
    m = {{1,"abc"},{2,"def"}};                      TEST_EQUAL(format_object(m, "*N"), "1:abc,2:def");              TEST_EQUAL(format_object(m, "nN2"), "01:abc,02:def");
    m = {{1,"abc"},{2,"def"},{3,"ghi"}};            TEST_EQUAL(format_object(m, "*N"), "1:abc,2:def,3:ghi");        TEST_EQUAL(format_object(m, "nN2"), "01:abc,02:def,03:ghi");
    m = {{1,"abc"},{2,"def"},{3,"ghi"},{4,"jkl"}};  TEST_EQUAL(format_object(m, "*N"), "1:abc,2:def,3:ghi,4:jkl");  TEST_EQUAL(format_object(m, "nN2"), "01:abc,02:def,03:ghi,04:jkl");

    m = {};                                         TEST_EQUAL(format_map(m, "n2", "xz"), "{}");
    m = {{1,"abc"}};                                TEST_EQUAL(format_map(m, "n2", "xz"), "{01:616263}");
    m = {{1,"abc"},{2,"def"}};                      TEST_EQUAL(format_map(m, "n2", "xz"), "{01:616263,02:646566}");
    m = {{1,"abc"},{2,"def"},{3,"ghi"}};            TEST_EQUAL(format_map(m, "n2", "xz"), "{01:616263,02:646566,03:676869}");
    m = {{1,"abc"},{2,"def"},{3,"ghi"},{4,"jkl"}};  TEST_EQUAL(format_map(m, "n2", "xz"), "{01:616263,02:646566,03:676869,04:6a6b6c}");

}
