#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_regex_compile_flags() {

    Regex r;
    Regex::match m;
    std::string s;

    // anchor = Anchor match at start of subject range
    TRY(r = Regex("[a-z]+", Regex::anchor));
    s = "Hello world";
    TRY(m = r(s));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "hello");

    // ascii = ASCII character properties
    TRY(r = Regex("[[:alpha:]]+"));
    s = "(αβγδε)";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "αβγδε");
    TRY(r = Regex("[[:alpha:]]+", Regex::ascii));
    TRY(m = r(s));
    TEST(! m);

    // byte = Match byte strings instead of UTF-8
    TRY(r = Regex("\\(.....\\)"));
    s = "(αβγδε)";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "(αβγδε)");
    TRY(r = Regex("\\(.....\\)", Regex::byte));
    TRY(m = r(s));
    TEST(! m);

    // crlf = Lines end with CRLF
    TRY(r = Regex("^[a-z]+$", Regex::multiline));
    s = "hello world\r\ngoodbye\r\n";
    TRY(m = r(s));
    TEST(! m);
    TRY(r = Regex("^[a-z]+$", Regex::crlf | Regex::multiline));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "goodbye");

    // dollar_end = $ does not match line breaks at end of string
    TRY(r = Regex("^\\w+$"));
    s = "Hello\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello");
    s = "Hello\n\n";
    TRY(m = r(s));
    TEST(! m);
    TRY(r = Regex("^\\w+$", Regex::dollar_end));
    s = "Hello\n";
    TRY(m = r(s));
    TEST(! m);

    // dot_all = . matches all characters including line breaks
    TRY(r = Regex(".+"));
    s = "Hello world\nGoodbye\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello world");
    TRY(r = Regex(".+", Regex::dot_all));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello world\nGoodbye\n");

    // extended = Ignore whitespace and # comments
    TRY(r = Regex(R"(
        [[:alpha:]]+  # first word
        \s            # space
        [[:alpha:]]+  # second word
        )", Regex::extended));
    s = "Hello world\nGoodbye\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello world");

    // first_line = Match must start in first line of string
    TRY(r = Regex("[a-z]{5}", Regex::first_line));
    s = "Hello world\nGoodbye\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "world");
    TRY(r = Regex("[a-z]{6}", Regex::first_line));
    TRY(m = r(s));
    TEST(! m);

    // full = Anchor match at both ends, matching complete string
    TRY(r = Regex("[a-z ]+", Regex::full));
    s = "Hello world";
    TRY(m = r(s));
    TEST(! m);
    s = "hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "hello world");

    // icase = Case insensitive match
    TRY(r = Regex("[a-z]+"));
    s = "Hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "ello");
    TRY(r = Regex("[a-z]+", Regex::icase));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello");

    // line = Match whole line
    // word = Match whole word
    TRY(r = Regex("[a-z]+"));
    s = "hello world\ngoodbye\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "hello");
    TRY(r = Regex("[a-z]+", Regex::line | Regex::multiline));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "goodbye");
    TRY(r = Regex("[a-z]+", Regex::line | Regex::multiline | Regex::word));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "goodbye");
    TRY(r = Regex("[a-z]+"));
    s = "Hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "ello");
    TRY(r = Regex("[a-z]+", Regex::word));
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "world");

    // multiline = ^ and $ match beginning and end of each line
    TRY(r = Regex("[a-z]+$", Regex::multiline));
    s = "Hello world\nGoodbye\n";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "world");

    // no_capture = No automatic capture, named captures only
    TRY(r = Regex("([A-Z])([a-z]+)", Regex::no_capture));
    s = "Hello world";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "Hello");
    TEST(m.matched(0));
    TEST(! m.matched(1));
    TEST(! m.matched(2));

    // partial_hard = Hard partial match, prefer partial to full match
    TRY(r = Regex("\\b[a-z]+", Regex::partial_hard));
    s = "Hello world";
    TRY(m = r(s));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");
    TRY(r = Regex("\\b[a-z]+\\d+", Regex::partial_hard));
    TRY(m = r(s));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");

    // partial_soft = Soft partial match, prefer full to partial match
    TRY(r = Regex("\\b[a-z]+", Regex::partial_soft));
    s = "Hello world";
    TRY(m = r(s));
    TEST(m);
    TEST(m.full());
    TEST(! m.partial());
    TEST_EQUAL(m.str(), "world");
    TRY(r = Regex("\\b[a-z]+\\d+", Regex::partial_soft));
    TRY(m = r(s));
    TEST(m);
    TEST(! m.full());
    TEST(m.partial());
    TEST_EQUAL(m.str(), "world");

}
