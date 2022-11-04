#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_regex_match() {

    Regex r;
    Regex::match m;
    std::string s;

    TEST(r.is_null());
    TEST_EQUAL(r.pattern(), "");
    TEST_EQUAL(uint32_t(r.flags()), 0u);
    TEST_EQUAL(r.groups(), 0u);
    s = "ABC123";
    TRY(m = r(s));
    TEST(! m);
    TEST(! m.full());
    TEST(! m.partial());
    TEST(! m.matched());

    TRY(r = Regex("[a-z]+"));
    TEST(! r.is_null());
    TEST_EQUAL(r.pattern(), "[a-z]+");
    TEST_EQUAL(uint32_t(r.flags()), 0u);
    TEST_EQUAL(r.groups(), 1u);
    s = "ABC123";
    TRY(m = r(s));
    TEST(! m);
    TEST(! m.full());
    TEST(! m.partial());
    TEST(! m.matched());
    s = "ABC123 xyz789";
    TRY(m = r(s));
    TEST(m);
    TEST(m.full());
    TEST(! m.partial());
    TEST(m.matched());
    TEST_EQUAL(m.offset(), 7u);
    TEST_EQUAL(m.endpos(), 10u);
    TEST_EQUAL(m.count(), 3u);
    TEST_EQUAL(m.str(), "xyz");
    TEST(! m.matched(1));

    TRY(r = Regex("([a-z]+)(\\d+)"));
    TEST(! r.is_null());
    TEST_EQUAL(r.pattern(), "([a-z]+)(\\d+)");
    TEST_EQUAL(uint32_t(r.flags()), 0u);
    TEST_EQUAL(r.groups(), 3u);
    s = "ABC123 xyz789";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.offset(), 7u);
    TEST_EQUAL(m.endpos(), 13u);
    TEST_EQUAL(m.count(), 6u);
    TEST_EQUAL(m.str(), "xyz789");
    TEST(m.matched(1));
    TEST_EQUAL(m.str(1), "xyz");
    TEST(m.matched(2));
    TEST_EQUAL(m.str(2), "789");
    TEST(! m.matched(3));
    TEST_EQUAL(m.str(3), "");

    TRY(r = Regex("([a-z]+)(\\d+)", Regex::icase));
    TEST(! r.is_null());
    TEST_EQUAL(r.pattern(), "([a-z]+)(\\d+)");
    TEST_EQUAL(uint32_t(r.flags()), uint32_t(Regex::icase));
    TEST_EQUAL(r.groups(), 3u);
    s = "ABC123 xyz789";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.offset(), 0u);
    TEST_EQUAL(m.endpos(), 6u);
    TEST_EQUAL(m.count(), 6u);
    TEST_EQUAL(m.str(), "ABC123");
    TEST(m.matched(1));
    TEST_EQUAL(m.str(1), "ABC");
    TEST(m.matched(2));
    TEST_EQUAL(m.str(2), "123");
    TEST(! m.matched(3));
    TEST_EQUAL(m.str(3), "");

    TRY(r = Regex("[a-z]+"));
    s = "abc123 xyz789";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "abc");
    TRY(m = r(s, 3));
    TEST(m);
    TEST_EQUAL(m.str(), "xyz");
    TRY(m = r(s, 12));
    TEST(! m);
    TRY(m = r(s, 13));
    TEST(! m);
    TRY(m = r(s, 14));
    TEST(! m);

    TRY(r = Regex("[[:alpha:]]+"));
    s = "αβγδε";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "αβγδε");
    TEST_THROW(m = r(s, 1), Regex::error);
    TRY(m = r(s, 2));
    TEST(m);
    TEST_EQUAL(m.str(), "βγδε");
    TEST_THROW(m = r(s, 3), Regex::error);
    TRY(m = r(s, 4));
    TEST(m);
    TEST_EQUAL(m.str(), "γδε");
    TEST_THROW(m = r(s, 5), Regex::error);
    TRY(m = r(s, 6));
    TEST(m);
    TEST_EQUAL(m.str(), "δε");
    TEST_THROW(m = r(s, 7), Regex::error);
    TRY(m = r(s, 8));
    TEST(m);
    TEST_EQUAL(m.str(), "ε");
    TEST_THROW(m = r(s, 9), Regex::error);
    TRY(m = r(s, 10));
    TEST(! m);
    TRY(m = r(s, 11));
    TEST(! m);

    TRY(r = Regex("([0-9]+)|([A-Z]+)|([a-z]+)"));
    s = "...123...456...";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "123");
    TEST_EQUAL(m.str(1), "123");
    TEST_EQUAL(m.str(2), "");
    TEST_EQUAL(m.str(3), "");
    TEST_EQUAL(m.first(), 1u);
    TEST_EQUAL(m.last(), 1u);
    s = "...abc...def...";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "abc");
    TEST_EQUAL(m.str(1), "");
    TEST_EQUAL(m.str(2), "");
    TEST_EQUAL(m.str(3), "abc");
    TEST_EQUAL(m.first(), 3u);
    TEST_EQUAL(m.last(), 3u);

    TRY(r = Regex("([0-9]*)([A-Z]*)([a-z]*)", Regex::not_empty));
    s = "...123ABC...";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "123ABC");
    TEST_EQUAL(m.str(1), "123");
    TEST_EQUAL(m.str(2), "ABC");
    TEST_EQUAL(m.str(3), "");
    TEST_EQUAL(m.first(), 1u);
    TEST_EQUAL(m.last(), 2u);
    s = "!@#$%^&*()";
    TRY(m = r(s));
    TEST(! m);
    TEST_EQUAL(m.first(), std::string::npos);
    TEST_EQUAL(m.last(), std::string::npos);

    TRY(r = Regex("^(a|b)c"));
    s = "bc";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "bc");
    TEST_EQUAL(m.last_mark(), "");
    TRY(r = Regex("^(*MARK:A)((*MARK:B)a|b)c"));
    s = "bc";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "bc");
    TEST_EQUAL(m.last_mark(), "A");
    s = "bx";
    TRY(m = r(s));
    TEST(! m);
    TEST_EQUAL(m.str(), "");
    TEST_EQUAL(m.last_mark(), "B");

    TRY(r = Regex("\\w+", Regex::hard_fail));
    s = "abc";
    TRY(m = r(s));
    TEST(m);
    TEST_EQUAL(m.str(), "abc");
    s = "@#$";
    TEST_THROW(r(s), Regex::error);

    TEST_THROW(r = Regex("[z-a]"), Regex::error);

}
