#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace Crow;
using namespace std::literals;

namespace {

    std::string format_version(const std::vector<int>& v) {
        if (v.empty())
            return "null";
        std::string s;
        for (int i: v)
            s += std::to_string(i) + '.';
        s.pop_back();
        return s;
    }

}

void test_crow_regex_pcre_version() {

    Regex::version_type v;

    TRY(v = Regex::compile_version());  TEST((v >= Regex::version_type{10,23}));  std::cout << "... PCRE compile version: " << format_version(v) << "\n";
    TRY(v = Regex::runtime_version());  TEST((v >= Regex::version_type{10,23}));  std::cout << "... PCRE runtime version: " << format_version(v) << "\n";
    TRY(v = Regex::unicode_version());  TEST((v >= Regex::version_type{13}));     std::cout << "... PCRE Unicode version: " << format_version(v) << "\n";

}

void test_crow_regex_escape() {

    Regex r;
    Regex::match m;
    std::string s1, s2;

    s1 = "";                 TRY(s2 = Regex::escape(s1));  TRY(r = Regex(s2, Regex::full));  TRY(m = r(s1));  TEST(m);
    s1 = "Hello world";      TRY(s2 = Regex::escape(s1));  TRY(r = Regex(s2, Regex::full));  TRY(m = r(s1));  TEST(m);
    s1 = "$()*+.?[\\]^{|}";  TRY(s2 = Regex::escape(s1));  TRY(r = Regex(s2, Regex::full));  TRY(m = r(s1));  TEST(m);
    s1 = "\0\x1f\x7f\r\n"s;  TRY(s2 = Regex::escape(s1));  TRY(r = Regex(s2, Regex::full));  TRY(m = r(s1));  TEST(m);

}
