#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_regex_grep() {

    Regex r;
    std::string s = "Hello world. Goodbye.";
    Regex::match_iterator mi;
    Regex::match_range mr;
    size_t n = 0;

    TRY(r = Regex("\\d+"));
    TRY(mr = r.grep(s));
    TEST(mr.begin() == mr.end());
    TRY(n = r.count(s));
    TEST_EQUAL(n, 0u);

    TRY(r = Regex("\\w+"));
    TRY(mr = r.grep(s));
    TEST(mr.begin() != mr.end());
    TRY(mi = mr.begin());
    TEST_EQUAL(mi->str(), "Hello");
    TRY(++mi);
    TEST(mi != mr.end());
    TEST_EQUAL(mi->str(), "world");
    TRY(++mi);
    TEST(mi != mr.end());
    TEST_EQUAL(mi->str(), "Goodbye");
    TRY(++mi);
    TEST(mi == mr.end());
    TRY(n = r.count(s));
    TEST_EQUAL(n, 3u);

}

void test_crow_regex_split() {

    Regex r;
    std::string s = "Hello world. Goodbye.";
    Regex::split_iterator si;
    Regex::split_range sr;

    TRY(r = Regex("@+"));
    TRY(sr = r.split(s));
    TEST(sr.begin() != sr.end());
    TRY(si = sr.begin());
    TEST_EQUAL(*si, s);
    TRY(++si);
    TEST(si == sr.end());

    TRY(r = Regex("[[:punct:]]+\\s*"));
    TRY(sr = r.split(s));
    TEST(sr.begin() != sr.end());
    TRY(si = sr.begin());
    TEST_EQUAL(*si, "Hello world");
    TRY(++si);
    TEST(si != sr.end());
    TEST_EQUAL(*si, "Goodbye");
    TRY(++si);
    TEST(si != sr.end());
    TEST_EQUAL(*si, "");
    TRY(++si);
    TEST(si == sr.end());

}

void test_crow_regex_partition() {

    Regex r;
    std::string s = "Hello world. Goodbye.";
    Regex::partition_type p;

    TRY(r = Regex("@+"));
    TRY(p = r.partition(s));
    TEST_EQUAL(p.left, s);
    TEST_EQUAL(p.mid, "");
    TEST_EQUAL(p.right, "");

    TRY(r = Regex("[[:punct:]]+\\s*"));
    TRY(p = r.partition(s));
    TEST_EQUAL(p.left, "Hello world");
    TEST_EQUAL(p.mid, ". ");
    TEST_EQUAL(p.right, "Goodbye.");

}

void test_crow_regex_tokenize() {

    Regex r1, r2;
    std::string s = "Hello world. Goodbye.";
    Regex::token_iterator ti;
    Regex::token_range tr;

    TRY(r1 = Regex("\\d+"));
    TRY(r2 = Regex("\\s+"));
    TEST_THROW(tr = Regex::tokenize(r1, r2, s), Regex::error);

    TRY(r1 = Regex("\\w+"));
    TRY(r2 = Regex("\\s+"));
    TRY(tr = Regex::tokenize(r1, r2, s));
    TEST(tr.begin() != tr.end());
    TRY(ti = tr.begin());
    TEST_EQUAL(ti->str(), "Hello");
    TRY(++ti);
    TEST(ti != tr.end());
    TEST_EQUAL(ti->str(), "world");
    TEST_THROW(++ti, Regex::error);

    TRY(r1 = Regex("\\w*"));
    TRY(r2 = Regex("\\.?\\s*"));
    TRY(tr = Regex::tokenize(r1, r2, s));
    TEST(tr.begin() != tr.end());
    TRY(ti = tr.begin());
    TEST_EQUAL(ti->str(), "Hello");
    TRY(++ti);
    TEST(ti != tr.end());
    TEST_EQUAL(ti->str(), "world");
    TRY(++ti);
    TEST(ti != tr.end());
    TEST_EQUAL(ti->str(), "Goodbye");
    TRY(++ti);
    TEST(ti != tr.end());
    TEST_EQUAL(ti->str(), "");
    TRY(++ti);
    TEST(ti == tr.end());

}
