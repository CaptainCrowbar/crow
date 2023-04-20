#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Literals;

namespace {

    class Foo:
    public Formatted {
    public:
        Foo() = default;
        explicit Foo(int n) noexcept: n_(n) {}
        std::string str(const FormatSpec& spec = {}) const override {
            return "*" + format_integer(n_, spec);
        }
    private:
        int n_ = 0;
    };

    class Bar {
    public:
        Bar() = default;
        explicit Bar(int n) noexcept: n_(n) {}
        std::string str(const FormatSpec& spec = {}) const {
            return "@" + format_integer(n_, spec);
        }
    private:
        int n_ = 0;
    };

    class Zap {
    public:
        Zap() = default;
        explicit Zap(int n) noexcept: n_(n) {}
        int get() const noexcept { return n_; }
    private:
        int n_ = 0;
    };

    class Argh {};

}

namespace Crow {

    template <>
    struct FormatType<Zap> {
        std::string operator()(const Zap& t, const FormatSpec& spec) const {
            return "#" + format_integer(t.get(), spec);
        }
    };

}

void test_crow_format_concepts() {

    TEST(FixedFormatType<void*>);
    TEST(FixedFormatType<int>);
    TEST(FixedFormatType<int*>);
    TEST(FixedFormatType<std::string>);
    TEST(FixedFormatType<const char*>);
    TEST(FixedFormatType<std::vector<int>>);
    TEST((FixedFormatType<std::map<int, std::string>>));
    TEST(FixedFormatType<Foo>);
    TEST(FixedFormatType<Bar>);
    TEST(! FixedFormatType<Zap>);

    TEST(! VariableFormatType<void*>);
    TEST(VariableFormatType<int>);
    TEST(! VariableFormatType<int*>);
    TEST(VariableFormatType<std::string>);
    TEST(VariableFormatType<const char*>);
    TEST(VariableFormatType<std::vector<int>>);
    TEST((VariableFormatType<std::map<int, std::string>>));
    TEST(VariableFormatType<Foo>);
    TEST(VariableFormatType<Bar>);
    TEST(! VariableFormatType<Zap>);

}

void test_crow_format_custom_types() {

    TEST_EQUAL(format_object(Foo()),          "*0");
    TEST_EQUAL(format_object(Foo(42)),        "*42");
    TEST_EQUAL(format_object(Bar()),          "@0");
    TEST_EQUAL(format_object(Bar(42)),        "@42");
    TEST_EQUAL(format_object(Zap()),          "#0");
    TEST_EQUAL(format_object(Zap(42)),        "#42");
    TEST_EQUAL(format_object(Foo(), "x4"),    "*0000");
    TEST_EQUAL(format_object(Foo(42), "x4"),  "*002a");
    TEST_EQUAL(format_object(Bar(), "x4"),    "@0000");
    TEST_EQUAL(format_object(Bar(42), "x4"),  "@002a");
    TEST_EQUAL(format_object(Zap(), "x4"),    "#0000");
    TEST_EQUAL(format_object(Zap(42), "x4"),  "#002a");

}

void test_crow_format_boolean() {

    TEST_EQUAL(format_object(false,  ""),    "false");
    TEST_EQUAL(format_object(true,   ""),    "true");
    TEST_EQUAL(format_object(false,  "b"),   "false");
    TEST_EQUAL(format_object(true,   "b"),   "true");
    TEST_EQUAL(format_object(false,  "B"),   "False");
    TEST_EQUAL(format_object(true,   "B"),   "True");
    TEST_EQUAL(format_object(false,  "y"),   "no");
    TEST_EQUAL(format_object(true,   "y"),   "yes");
    TEST_EQUAL(format_object(false,  "Y"),   "No");
    TEST_EQUAL(format_object(true,   "Y"),   "Yes");
    TEST_EQUAL(format_object(false,  "n"),   "0");
    TEST_EQUAL(format_object(true,   "n"),   "1");
    TEST_EQUAL(format_object(false,  "n4"),  "0000");
    TEST_EQUAL(format_object(true,   "n4"),  "0001");

}

void test_crow_format_characters_and_strings() {

    TEST_EQUAL(format_object("",             ""),    "");
    TEST_EQUAL(format_object("",             "s"),   "");
    TEST_EQUAL(format_object("",             "s5"),  "");
    TEST_EQUAL(format_object("",             "q"),   "\"\"");
    TEST_EQUAL(format_object("",             "q5"),  "\"\"");
    TEST_EQUAL(format_object("",             "x"),   "");
    TEST_EQUAL(format_object("",             "x5"),  "");
    TEST_EQUAL(format_object("",             "X"),   "");
    TEST_EQUAL(format_object("Hello world",  ""),    "Hello world");
    TEST_EQUAL(format_object("Hello world",  "s"),   "Hello world");
    TEST_EQUAL(format_object("Hello world",  "s5"),  "Hello...");
    TEST_EQUAL(format_object("Hello world",  "q"),   "\"Hello world\"");
    TEST_EQUAL(format_object("Hello world",  "q5"),  "\"Hello...\"");
    TEST_EQUAL(format_object("Hello world",  "x"),   "48 65 6c 6c 6f 20 77 6f 72 6c 64");
    TEST_EQUAL(format_object("Hello world",  "x5"),  "48 65 6c 6c 6f...");
    TEST_EQUAL(format_object("Hello world",  "X"),   "48 65 6C 6C 6F 20 77 6F 72 6C 64");
    TEST_EQUAL(format_object("αβγδε",        ""),    "αβγδε");
    TEST_EQUAL(format_object("αβγδε",        "q"),   "\"αβγδε\"");
    TEST_EQUAL(format_object("αβγδε",        "x"),   "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_object(u"αβγδε",       ""),    "αβγδε");
    TEST_EQUAL(format_object(u"αβγδε",       "q"),   "\"αβγδε\"");
    TEST_EQUAL(format_object(u"αβγδε",       "x"),   "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_object(U"αβγδε",       ""),    "αβγδε");
    TEST_EQUAL(format_object(U"αβγδε",       "q"),   "\"αβγδε\"");
    TEST_EQUAL(format_object(U"αβγδε",       "x"),   "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_object(L"αβγδε",       ""),    "αβγδε");
    TEST_EQUAL(format_object(L"αβγδε",       "q"),   "\"αβγδε\"");
    TEST_EQUAL(format_object(L"αβγδε",       "x"),   "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(format_object('\x1b',         ""),    "\x1b");
    TEST_EQUAL(format_object('\x1b',         "q"),   "\"\\x1b\"");
    TEST_EQUAL(format_object('\x1b',         "x"),   "1b");
    TEST_EQUAL(format_object('X',            ""),    "X");
    TEST_EQUAL(format_object('X',            "q"),   "\"X\"");
    TEST_EQUAL(format_object('X',            "x"),   "58");
    TEST_EQUAL(format_object(U'\u001f',      ""),    "\x1f");
    TEST_EQUAL(format_object(U'\u001f',      "q"),   "\"\\x1f\"");
    TEST_EQUAL(format_object(U'\u001f',      "x"),   "1f");
    TEST_EQUAL(format_object(U'X',           ""),    "X");
    TEST_EQUAL(format_object(U'X',           "q"),   "\"X\"");
    TEST_EQUAL(format_object(U'X',           "x"),   "58");
    TEST_EQUAL(format_object(U'€',           ""),    "€");
    TEST_EQUAL(format_object(U'€',           "q"),   "\"€\"");
    TEST_EQUAL(format_object(U'€',           "x"),   "e2 82 ac");
    TEST_EQUAL(format_object(U'😀',          ""),    "😀");
    TEST_EQUAL(format_object(U'😀',          "q"),   "\"😀\"");
    TEST_EQUAL(format_object(U'😀',          "x"),   "f0 9f 98 80");
    TEST_EQUAL(format_object(u'€',           ""),    "€");
    TEST_EQUAL(format_object(u'€',           "q"),   "\"€\"");
    TEST_EQUAL(format_object(u'€',           "x"),   "e2 82 ac");
    TEST_EQUAL(format_object(L'€',           ""),    "€");
    TEST_EQUAL(format_object(L'€',           "q"),   "\"€\"");
    TEST_EQUAL(format_object(L'€',           "x"),   "e2 82 ac");

}

void test_crow_format_pointers() {

    std::string s = "Hello";
    char* cp = s.data();
    const char* ccp = s.data();
    std::string* sp = &s;
    const std::string* csp = &s;
    void* vp = sp;
    const void* cvp = csp;
    char* null_cp = nullptr;
    std::string* null_sp = nullptr;
    void* null_vp = nullptr;

    TEST_EQUAL(format_object(cp),       "Hello");
    TEST_EQUAL(format_object(ccp),      "Hello");
    TEST_MATCH(format_object(sp),       "^<0x[1-9a-f][0-9a-f]*>$");
    TEST_MATCH(format_object(csp),      "^<0x[1-9a-f][0-9a-f]*>$");
    TEST_MATCH(format_object(vp),       "^<0x[1-9a-f][0-9a-f]*>$");
    TEST_MATCH(format_object(cvp),      "^<0x[1-9a-f][0-9a-f]*>$");
    TEST_EQUAL(format_object(null_cp),  "<null>");
    TEST_EQUAL(format_object(null_sp),  "<null>");
    TEST_EQUAL(format_object(null_vp),  "<null>");

}

void test_crow_format_type_names() {

    int i = 0;
    double d = 0;
    std::string s;
    std::vector<int> v;
    const double cd = 0;
    double& dr = d;
    const double& cdr = cd;
    FormatSpec fs;

    TEST_MATCH(format_object(i,    "T"),  "^(signed )?int$");
    TEST_EQUAL(format_object(d,    "T"),  "double");
    TEST_MATCH(format_object(s,    "T"),  "^std::(\\w+::)*basic_string<");
    TEST_MATCH(format_object(v,    "T"),  "^std::(\\w+::)*vector<int,");
    TEST_EQUAL(format_object(cd,   "T"),  "double");
    TEST_EQUAL(format_object(dr,   "T"),  "double");
    TEST_EQUAL(format_object(cdr,  "T"),  "double");
    TEST_EQUAL(format_object(fs,   "T"),  "Crow::FormatSpec");

    TEST_MATCH(format_object(i,    "Tu"),  "^(signed )?int$");
    TEST_EQUAL(format_object(d,    "Tu"),  "double");
    TEST_EQUAL(format_object(s,    "Tu"),  "basic_string");
    TEST_EQUAL(format_object(v,    "Tu"),  "vector");
    TEST_EQUAL(format_object(cd,   "Tu"),  "double");
    TEST_EQUAL(format_object(dr,   "Tu"),  "double");
    TEST_EQUAL(format_object(cdr,  "Tu"),  "double");
    TEST_EQUAL(format_object(fs,   "Tu"),  "FormatSpec");

}

void test_crow_format_object() {

    FormatObject f;
    FormatObject fq("q");
    FormatObject fx4("x4");

    TEST_EQUAL(f(0),              "0");            TEST_EQUAL(fx4(0),             "0000");
    TEST_EQUAL(f(42),             "42");           TEST_EQUAL(fx4(42),            "002a");
    TEST_EQUAL(f(Foo()),          "*0");           TEST_EQUAL(fx4(Foo()),         "*0000");
    TEST_EQUAL(f(Foo(42)),        "*42");          TEST_EQUAL(fx4(Foo(42)),       "*002a");
    TEST_EQUAL(f(Bar()),          "@0");           TEST_EQUAL(fx4(Bar()),         "@0000");
    TEST_EQUAL(f(Bar(42)),        "@42");          TEST_EQUAL(fx4(Bar(42)),       "@002a");
    TEST_EQUAL(f(Zap()),          "#0");           TEST_EQUAL(fx4(Zap()),         "#0000");
    TEST_EQUAL(f(Zap(42)),        "#42");          TEST_EQUAL(fx4(Zap(42)),       "#002a");
    TEST_EQUAL(f(""),             "");             TEST_EQUAL(fq(""),             "\"\"");
    TEST_EQUAL(f("Hello world"),  "Hello world");  TEST_EQUAL(fq("Hello world"),  "\"Hello world\"");
    TEST_EQUAL(f("αβγδε"),        "αβγδε");        TEST_EQUAL(fq("αβγδε"),        "\"αβγδε\"");
    TEST_EQUAL(f(u"αβγδε"),       "αβγδε");        TEST_EQUAL(fq(u"αβγδε"),       "\"αβγδε\"");
    TEST_EQUAL(f(U"αβγδε"),       "αβγδε");        TEST_EQUAL(fq(U"αβγδε"),       "\"αβγδε\"");
    TEST_EQUAL(f(L"αβγδε"),       "αβγδε");        TEST_EQUAL(fq(L"αβγδε"),       "\"αβγδε\"");
    TEST_EQUAL(f('\x1b'),         "\x1b");         TEST_EQUAL(fq('\x1b'),         "\"\\x1b\"");
    TEST_EQUAL(f('X'),            "X");            TEST_EQUAL(fq('X'),            "\"X\"");
    TEST_EQUAL(f(U'\u001f'),      "\x1f");         TEST_EQUAL(fq(U'\u001f'),      "\"\\x1f\"");
    TEST_EQUAL(f(U'X'),           "X");            TEST_EQUAL(fq(U'X'),           "\"X\"");
    TEST_EQUAL(f(U'€'),           "€");            TEST_EQUAL(fq(U'€'),           "\"€\"");
    TEST_EQUAL(f(U'😀'),          "😀");           TEST_EQUAL(fq(U'😀'),          "\"😀\"");
    TEST_EQUAL(f(u'€'),           "€");            TEST_EQUAL(fq(u'€'),           "\"€\"");
    TEST_EQUAL(f(L'€'),           "€");            TEST_EQUAL(fq(L'€'),           "\"€\"");

}

void test_crow_format_class() {

    std::vector<int> v = {123,456,789};

    TEST_EQUAL(Formatter("")                     (),             "");
    TEST_EQUAL(Formatter("Hello world!")         (),             "Hello world!");
    TEST_EQUAL(Formatter("Hello {0}!")           ("world"),      "Hello world!");
    TEST_EQUAL(Formatter("Hello {0}!")           (42),           "Hello 42!");
    TEST_EQUAL(Formatter("Hello \\{0\\}!")       (42),           "Hello {0}!");
    TEST_EQUAL(Formatter("Hello \\{{0}\\}!")     (42),           "Hello {42}!");
    TEST_EQUAL(Formatter("Hello {0} {1}!")       ("world", 42),  "Hello world 42!");
    TEST_EQUAL(Formatter("Hello {1} {0}!")       ("world", 42),  "Hello 42 world!");
    TEST_EQUAL(Formatter("({0})")                (0.0),          "(0)");
    TEST_EQUAL(Formatter("({0}) ({1})")          (42.0, 86.99),  "(42) (86.99)");
    TEST_EQUAL(Formatter("({0})")                (42.0, 86.99),  "(42)");
    TEST_EQUAL(Formatter("({1})")                (42.0, 86.99),  "(86.99)");
    TEST_EQUAL(Formatter("({0:e4}) ({1:e4})")    (42.0, 86.99),  "(4.200e1) (8.699e1)");
    TEST_EQUAL(Formatter("({0:f4}) ({1:f4})")    (42.0, 86.99),  "(42.0000) (86.9900)");
    TEST_EQUAL(Formatter("({0:g4}) ({1:g4})")    (42.0, 86.99),  "(42.00) (86.99)");
    TEST_EQUAL(Formatter("({0:gz4}) ({1:gz4})")  (42.0, 86.99),  "(42) (86.99)");
    TEST_EQUAL(Formatter("{0:T} = {0}")          (1234.5),       "double = 1234.5");
    TEST_MATCH(Formatter("{0}")                  (Argh()),       "::Argh:<0x[[:xdigit:]]+>$");
    TEST_MATCH(Formatter("{0}")                  (v),            "[123,456,789]");
    TEST_MATCH(Formatter("{0:x4}")               (v),            "[007b,01c8,0315]");
    TEST_MATCH(Formatter("{0:f2}")               (v),            "[123.00,456.00,789.00]");
    TEST_THROW(Formatter("{0} {1} {2}")          (86, 99),       std::out_of_range);

}

void test_crow_format_function() {

    std::vector<int> v = {123,456,789};

    TEST_EQUAL(fmt(""),                    /**/           "");
    TEST_EQUAL(fmt("Hello world!"),        /**/           "Hello world!");
    TEST_EQUAL(fmt("Hello {0}!",           "world"),      "Hello world!");
    TEST_EQUAL(fmt("Hello {0}!",           42),           "Hello 42!");
    TEST_EQUAL(fmt("Hello \\{0\\}!",       42),           "Hello {0}!");
    TEST_EQUAL(fmt("Hello \\{{0}\\}!",     42),           "Hello {42}!");
    TEST_EQUAL(fmt("Hello {0} {1}!",       "world", 42),  "Hello world 42!");
    TEST_EQUAL(fmt("Hello {1} {0}!",       "world", 42),  "Hello 42 world!");
    TEST_EQUAL(fmt("({0})",                0.0),          "(0)");
    TEST_EQUAL(fmt("({0}) ({1})",          42.0, 86.99),  "(42) (86.99)");
    TEST_EQUAL(fmt("({0})",                42.0, 86.99),  "(42)");
    TEST_EQUAL(fmt("({1})",                42.0, 86.99),  "(86.99)");
    TEST_EQUAL(fmt("({0:e4}) ({1:e4})",    42.0, 86.99),  "(4.200e1) (8.699e1)");
    TEST_EQUAL(fmt("({0:f4}) ({1:f4})",    42.0, 86.99),  "(42.0000) (86.9900)");
    TEST_EQUAL(fmt("({0:g4}) ({1:g4})",    42.0, 86.99),  "(42.00) (86.99)");
    TEST_EQUAL(fmt("({0:gz4}) ({1:gz4})",  42.0, 86.99),  "(42) (86.99)");
    TEST_EQUAL(fmt("{0:T} = {0}",          1234.5),       "double = 1234.5");
    TEST_MATCH(fmt("{0}",                  Argh()),       "::Argh:<0x[[:xdigit:]]+>$");
    TEST_MATCH(fmt("{0}",                  v),            "[123,456,789]");
    TEST_MATCH(fmt("{0:x4}",               v),            "[007b,01c8,0315]");
    TEST_MATCH(fmt("{0:f2}",               v),            "[123.00,456.00,789.00]");
    TEST_THROW(fmt("{0} {1} {2}",          86, 99),       std::out_of_range);

}

void test_crow_format_literals() {

    Formatter f;
    std::vector<int> v = {123,456,789};

    TRY(f = ""_fmt);                     TEST_EQUAL(f(),             "");
    TRY(f = "Hello world!"_fmt);         TEST_EQUAL(f(),             "Hello world!");
    TRY(f = "Hello {0}!"_fmt);           TEST_EQUAL(f("world"),      "Hello world!");
    TRY(f = "Hello {0}!"_fmt);           TEST_EQUAL(f(42),           "Hello 42!");
    TRY(f = "Hello \\{0\\}!"_fmt);       TEST_EQUAL(f(42),           "Hello {0}!");
    TRY(f = "Hello \\{{0}\\}!"_fmt);     TEST_EQUAL(f(42),           "Hello {42}!");
    TRY(f = "Hello {0} {1}!"_fmt);       TEST_EQUAL(f("world", 42),  "Hello world 42!");
    TRY(f = "Hello {1} {0}!"_fmt);       TEST_EQUAL(f("world", 42),  "Hello 42 world!");
    TRY(f = "({0})"_fmt);                TEST_EQUAL(f(0.0),          "(0)");
    TRY(f = "({0}) ({1})"_fmt);          TEST_EQUAL(f(42.0, 86.99),  "(42) (86.99)");
    TRY(f = "({0})"_fmt);                TEST_EQUAL(f(42.0, 86.99),  "(42)");
    TRY(f = "({1})"_fmt);                TEST_EQUAL(f(42.0, 86.99),  "(86.99)");
    TRY(f = "({0:e4}) ({1:e4})"_fmt);    TEST_EQUAL(f(42.0, 86.99),  "(4.200e1) (8.699e1)");
    TRY(f = "({0:f4}) ({1:f4})"_fmt);    TEST_EQUAL(f(42.0, 86.99),  "(42.0000) (86.9900)");
    TRY(f = "({0:g4}) ({1:g4})"_fmt);    TEST_EQUAL(f(42.0, 86.99),  "(42.00) (86.99)");
    TRY(f = "({0:gz4}) ({1:gz4})"_fmt);  TEST_EQUAL(f(42.0, 86.99),  "(42) (86.99)");
    TRY(f = "{0}"_fmt);                  TEST_EQUAL(f(v),            "[123,456,789]");
    TRY(f = "{0:x4}"_fmt);               TEST_EQUAL(f(v),            "[007b,01c8,0315]");
    TRY(f = "{0:f2}"_fmt);               TEST_EQUAL(f(v),            "[123.00,456.00,789.00]");
    TRY(f = "{0} {1} {2}"_fmt);          TEST_THROW(f(86, 99),       std::out_of_range);

}
