#include "crow/algorithm.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Crow;
using namespace std::literals;

void test_crow_algorithm_lookup() {

    std::map<int, std::string> map = {
        { 1, "alpha" },
        { 2, "bravo" },
        { 3, "charlie" },
    };

    std::string s;

    TRY(s = lookup(map, 0));          TEST_EQUAL(s, "");
    TRY(s = lookup(map, 1));          TEST_EQUAL(s, "alpha");
    TRY(s = lookup(map, 2));          TEST_EQUAL(s, "bravo");
    TRY(s = lookup(map, 3));          TEST_EQUAL(s, "charlie");
    TRY(s = lookup(map, 4));          TEST_EQUAL(s, "");
    TRY(s = lookup(map, 0, "none"));  TEST_EQUAL(s, "none");
    TRY(s = lookup(map, 1, "none"));  TEST_EQUAL(s, "alpha");
    TRY(s = lookup(map, 2, "none"));  TEST_EQUAL(s, "bravo");
    TRY(s = lookup(map, 3, "none"));  TEST_EQUAL(s, "charlie");
    TRY(s = lookup(map, 4, "none"));  TEST_EQUAL(s, "none");

}

void test_crow_algorithm_unique() {

    const auto is_upper = [] (char c) { return c >= 'A' && c <= 'Z'; };
    const auto is_lower = [] (char c) { return c >= 'a' && c <= 'z'; };
    const auto same_case = [=] (char a, char b) { return (is_lower(a) && is_lower(b)) || (is_upper(a) && is_upper(b)); };

    std::string s;

    s = "";                 TRY(unique_in(s));                             TEST_EQUAL(s, "");
    s = "abcdeabcde";       TRY(unique_in(s));                             TEST_EQUAL(s, "abcdeabcde");
    s = "abbcccddddeeeee";  TRY(unique_in(s));                             TEST_EQUAL(s, "abcde");
    s = "";                 TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "");
    s = "AbCaBcAbCaBc";     TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "AbCaBcAbCaBc");
    s = "ABCabcABCabc";     TRY(unique_in(s, same_case));                  TEST_EQUAL(s, "AaAa");
    s = "";                 TRY(sort_unique_in(s));                        TEST_EQUAL(s, "");
    s = "zyxwvutsrqpon";    TRY(sort_unique_in(s));                        TEST_EQUAL(s, "nopqrstuvwxyz");
    s = "abcdeabcdabcaba";  TRY(sort_unique_in(s));                        TEST_EQUAL(s, "abcde");
    s = "";                 TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "");
    s = "nopqrstuvwxyz";    TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "zyxwvutsrqpon");
    s = "abcdeabcdabcaba";  TRY(sort_unique_in(s, std::greater<char>()));  TEST_EQUAL(s, "edcba");

}

namespace {

    template <typename Range, typename DiffList>
    std::string report_diff(const Range& lhs, const Range& rhs, const DiffList& delta) {
        std::string list;
        for (auto& d: delta) {
            list += "A" + std::to_string(d.del.begin() - std::begin(lhs));
            if (d.del.begin() != d.del.end())
                list += "+" + std::to_string(d.del.end() - d.del.begin());
            list += "=>B" + std::to_string(d.ins.begin() - std::begin(rhs));
            if (d.ins.begin() != d.ins.end())
                list += "+" + std::to_string(d.ins.end() - d.ins.begin());
            list += ";";
        }
        return list;
    }

}

void test_crow_algorithm_diff() {

    using vector_type = std::vector<int>;
    using diff_list = decltype(diff(vector_type(), vector_type()));

    vector_type a, b;
    diff_list delta;

    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 0u);

    a = {1,2,3};
    b = {1,2,3,4,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A3=>B3+2;"
    );

    a = {1,2,3,4,5};
    b = {3,4,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A0+2=>B0;"
    );

    a = {2,4,6};
    b = {1,2,3,4,5,6};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 3u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A0=>B0+1;"
        "A1=>B2+1;"
        "A2=>B4+1;"
    );

    a = {1,2,3,4,5,6};
    b = {1,3,5};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 3u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A1+1=>B1;"
        "A3+1=>B2;"
        "A5+1=>B3;"
    );

    a = {1,2,3,4,5,6,10,11,12};
    b = {1,2,3,7,8,9,10,11,12};
    TRY(delta = diff(a, b));
    TEST_EQUAL(delta.size(), 1u);
    TEST_EQUAL(report_diff(a, b, delta),
        "A3+3=>B3+3;"
    );

}

void test_crow_algorithm_edit_distance_levenshtein() {

    TEST_EQUAL(Levenshtein()(""s, ""s), 0);
    TEST_EQUAL(Levenshtein()("Hello"s, ""s), 5);
    TEST_EQUAL(Levenshtein()(""s, "Hello"s), 5);
    TEST_EQUAL(Levenshtein()("Hello"s, "Hello"s), 0);
    TEST_EQUAL(Levenshtein()("Hello"s, "hello"s), 1);
    TEST_EQUAL(Levenshtein()("Hell"s, "Hello"s), 1);
    TEST_EQUAL(Levenshtein()("Hello"s, "Hell"s), 1);
    TEST_EQUAL(Levenshtein()("Hello"s, "Hel"s), 2);
    TEST_EQUAL(Levenshtein()("Hello"s, "He"s), 3);
    TEST_EQUAL(Levenshtein()("Hello"s, "H"s), 4);
    TEST_EQUAL(Levenshtein()("Hello"s, "World"s), 4);
    TEST_EQUAL(Levenshtein(3, 2, 5)(""s, ""s), 0);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, ""s), 10);
    TEST_EQUAL(Levenshtein(3, 2, 5)(""s, "Hello"s), 15);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "Hello"s), 0);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hell"s, "Hello"s), 3);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "Hell"s), 2);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "Hel"s), 4);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "He"s), 6);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "H"s), 8);
    TEST_EQUAL(Levenshtein(3, 2, 1)("Hello"s, "hello"s), 1);
    TEST_EQUAL(Levenshtein(3, 2, 2)("Hello"s, "hello"s), 2);
    TEST_EQUAL(Levenshtein(3, 2, 3)("Hello"s, "hello"s), 3);
    TEST_EQUAL(Levenshtein(3, 2, 4)("Hello"s, "hello"s), 4);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "hello"s), 5);
    TEST_EQUAL(Levenshtein(3, 2, 6)("Hello"s, "hello"s), 5);
    TEST_EQUAL(Levenshtein(1, 1, 5)("Hello"s, "World"s), 8);
    TEST_EQUAL(Levenshtein(2, 2, 5)("Hello"s, "World"s), 16);
    TEST_EQUAL(Levenshtein(3, 3, 5)("Hello"s, "World"s), 20);
    TEST_EQUAL(Levenshtein(4, 4, 5)("Hello"s, "World"s), 20);
    TEST_EQUAL(Levenshtein(3, 2, 1)("Hello"s, "World"s), 4);
    TEST_EQUAL(Levenshtein(3, 2, 2)("Hello"s, "World"s), 8);
    TEST_EQUAL(Levenshtein(3, 2, 3)("Hello"s, "World"s), 12);
    TEST_EQUAL(Levenshtein(3, 2, 4)("Hello"s, "World"s), 16);
    TEST_EQUAL(Levenshtein(3, 2, 5)("Hello"s, "World"s), 20);
    TEST_EQUAL(Levenshtein(3, 2, 6)("Hello"s, "World"s), 20);

}

void test_crow_algorithm_edit_distance_damerau_levenshtein() {

    TEST_EQUAL(DamerauLevenshtein()(""s, ""s), 0);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, ""s), 5);
    TEST_EQUAL(DamerauLevenshtein()(""s, "abcde"s), 5);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "abcde"s), 0);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "abcdz"s), 1);
    TEST_EQUAL(DamerauLevenshtein()("abcd"s, "abcde"s), 1);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "abcd"s), 1);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "abc"s), 2);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "ab"s), 3);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "a"s), 4);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "fghij"s), 5);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "badce"s), 2);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "badec"s), 3);
    TEST_EQUAL(DamerauLevenshtein()("abcde"s, "ecdba"s), 4);

}

void test_crow_algorithm_edit_distance_jaro_winkler() {

    TEST_EQUAL(JaroWinkler()(""s, ""s), 0);
    TEST_EQUAL(JaroWinkler()("Hello"s, "Hello"s), 0);
    TEST_NEAR(JaroWinkler()("Hello"s, "hello"s), 0.1333, 1e-4);
    TEST_NEAR(JaroWinkler()("Hello"s, "HellO"s), 0.0800, 1e-4);
    TEST_NEAR(JaroWinkler()("Hello"s, "HELLO"s), 0.5333, 1e-4);
    TEST_NEAR(JaroWinkler()("Hello"s, "World"s), 0.5333, 1e-4);
    TEST_EQUAL(JaroWinkler()("abcde"s, "fghij"s), 1);

}

namespace {

    class FakeHash {
    public:
        explicit FakeHash(size_t seed): seed_(seed) {}
        size_t operator()(const std::string& s) const noexcept {
            size_t h = seed_;
            for (char c: s)
                h += uint8_t(c);
            return h;
        }
    private:
        size_t seed_;
    };

}

void test_crow_algorithm_hash_compare() {

    using string_set = std::unordered_set<std::string, FakeHash>;
    using string_list = std::vector<std::string>;

    static constexpr size_t n_strings = 25;
    static constexpr size_t n_hashes = 1000;
    static constexpr size_t n_samples = 4;
    static constexpr double p_equal = 0.1;

    std::minstd_rand rng(42);
    std::bernoulli_distribution select_equal(p_equal);

    for (size_t n = 1; n <= n_strings; ++n) {

        string_list all_strings;

        for (size_t i = 1; i <= n; ++i) {
            char c = char('a' + i - 1);
            std::string s(3, c);
            s += std::to_string(i);
            all_strings.push_back(s);
        }

        for (uint32_t seed = 0; seed < n_hashes; ++seed) {

            FakeHash hash1(seed);
            FakeHash hash2(seed + n_hashes);

            for (size_t sample = 1; sample <= n_samples; ++sample) {

                double fraction = double(sample) / double(n_samples);
                std::bernoulli_distribution select_sample(fraction);
                bool same = select_equal(rng);
                string_set set1(0, hash1);
                string_set set2(0, hash2);
                string_list vec1, vec2;

                for (auto& s: all_strings) {
                    bool accept1 = select_sample(rng);
                    bool accept2 = same ? accept1 : select_sample(rng);
                    if (accept1) {
                        set1.insert(s);
                        vec1.push_back(s);
                    }
                    if (accept2) {
                        set2.insert(s);
                        vec2.push_back(s);
                    }
                }

                int set_compare = hash_compare(set1, set2);
                int vec_compare = vec1 == vec2 ? 0 : vec1 < vec2 ? -1 : 1;

                if (set_compare != vec_compare) {
                    std::cout
                        << "Failed hash_compare():\n"
                        << "    vec1 = " << format_range(vec1) << "\n"
                        << "    vec2 = " << format_range(vec2) << "\n"
                        << "    set1 = " << format_range(set1) << "\n"
                        << "    set2 = " << format_range(set2) << "\n"
                        << "    vec_compare = " << vec_compare << "\n"
                        << "    set_compare = " << set_compare << "\n";
                    return;
                }

            }

        }

    }

}

namespace {

    template <typename T, typename RNG>
    void precision_sum_test(size_t copies, size_t cycles, T highval, RNG& rng) {

        std::vector<T> values;
        values.reserve(4 * copies);

        for (size_t i = 0; i < copies; ++i) {
            values.push_back(1);
            values.push_back(highval);
            values.push_back(1);
            values.push_back(- highval);
        }

        PrecisionSum<T> psum;

        for (size_t i = 0; i < cycles; ++i) {
            std::shuffle(values.begin(), values.end(), rng);
            TRY(psum.clear());
            for (T t: values)
                TRY(psum.add(t));
            T sum = 0;
            TRY(sum = psum.get());
            TEST_EQUAL(sum, T(2 * copies));
        }

        for (size_t i = 0; i < cycles; ++i) {
            std::shuffle(values.begin(), values.end(), rng);
            T sum = 0;
            TRY(sum = precision_sum(values));
            TEST_EQUAL(sum, T(2 * copies));
        }

    }

}

void test_crow_algorithm_precision_sum() {

    std::mt19937 rng(42);

    precision_sum_test<float>(1000, 100, 1e20f, rng);
    precision_sum_test<double>(1000, 100, 1e100, rng);

    // GCC doesn't provide strict IEEE mode for long double
    // (not even with -ffloat-store)

    #if ! defined(__GNUC__) || defined(__clang__)
        precision_sum_test<long double>(1000, 100, 1e100l, rng);
    #endif

}

void test_crow_algorithm_line_integral() {

    double integral = 0;
    auto f = [] (auto x) { return x * x; };

    TRY((integral = line_integral(0.0, 10.0, 10, f)));
    TEST_EQUAL(integral, 335);

}

void test_crow_algorithm_volume_integral() {

    double integral = 0;
    auto f = [] (auto x) { return x[0] * x[1]; };

    TRY((integral = volume_integral(Double2(0, 0), Double2(5, 5), 5, f)));
    TEST_EQUAL(integral, 156.25);

}

void test_crow_algorithm_cartesian_power() {

    using Crow::UnitTest::format_range;

    std::string alpha = "abc";
    std::vector<std::string> vec;

    auto vec2str = [] (const std::vector<char>& v) { return std::string(v.data(), v.size()); };

    auto cp0 = cartesian_power(alpha, 0);
    auto cp1 = cartesian_power(alpha, 1);
    auto cp2 = cartesian_power(alpha, 2);
    auto cp3 = cartesian_power(alpha, 3);
    auto cp4 = cartesian_power(alpha, 4);

    TEST_EQUAL(std::distance(cp0.begin(), cp0.end()), 1);
    TEST_EQUAL(std::distance(cp1.begin(), cp1.end()), 3);
    TEST_EQUAL(std::distance(cp2.begin(), cp2.end()), 9);
    TEST_EQUAL(std::distance(cp3.begin(), cp3.end()), 27);
    TEST_EQUAL(std::distance(cp4.begin(), cp4.end()), 81);

    vec.clear();
    TRY(std::transform(cp0.begin(), cp0.end(), std::back_inserter(vec), vec2str));
    TEST_EQUAL(vec.size(), 1u);
    TEST_EQUAL(format_range(vec), "[]");

    vec.clear();
    TRY(std::transform(cp1.begin(), cp1.end(), std::back_inserter(vec), vec2str));
    TEST_EQUAL(vec.size(), 3u);
    TEST_EQUAL(format_range(vec), "[a,b,c]");

    vec.clear();
    TRY(std::transform(cp2.begin(), cp2.end(), std::back_inserter(vec), vec2str));
    TEST_EQUAL(vec.size(), 9u);
    TEST_EQUAL(format_range(vec), "[aa,ab,ac,ba,bb,bc,ca,cb,cc]");

    vec.clear();
    TRY(std::transform(cp3.begin(), cp3.end(), std::back_inserter(vec), vec2str));
    TEST_EQUAL(vec.size(), 27u);
    TEST_EQUAL(format_range(vec),
        "[aaa,aab,aac,aba,abb,abc,aca,acb,acc,"
        "baa,bab,bac,bba,bbb,bbc,bca,bcb,bcc,"
        "caa,cab,cac,cba,cbb,cbc,cca,ccb,ccc]"
    );

    vec.clear();
    TRY(std::transform(cp4.begin(), cp4.end(), std::back_inserter(vec), vec2str));
    TEST_EQUAL(vec.size(), 81u);
    TEST_EQUAL(format_range(vec),
        "[aaaa,aaab,aaac,aaba,aabb,aabc,aaca,aacb,aacc,"
        "abaa,abab,abac,abba,abbb,abbc,abca,abcb,abcc,"
        "acaa,acab,acac,acba,acbb,acbc,acca,accb,accc,"
        "baaa,baab,baac,baba,babb,babc,baca,bacb,bacc,"
        "bbaa,bbab,bbac,bbba,bbbb,bbbc,bbca,bbcb,bbcc,"
        "bcaa,bcab,bcac,bcba,bcbb,bcbc,bcca,bccb,bccc,"
        "caaa,caab,caac,caba,cabb,cabc,caca,cacb,cacc,"
        "cbaa,cbab,cbac,cbba,cbbb,cbbc,cbca,cbcb,cbcc,"
        "ccaa,ccab,ccac,ccba,ccbb,ccbc,ccca,cccb,cccc]"
    );

}

void test_crow_algorithm_find_optimum() {

    std::vector<std::string> vec;
    auto i = vec.begin();
    auto str_size = [] (const std::string& s) { return s.size(); };

    TRY(i = find_optimum(vec, str_size));
    TEST(i == vec.end());

    vec = {"alpha"};
    TRY(i = find_optimum(vec, str_size));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "alpha");

    vec = {"alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel"};
    TRY(i = find_optimum(vec, str_size));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "charlie");
    TRY(i = find_optimum(vec, str_size, std::less<>()));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "echo");

}

void test_crow_algorithm_subsets() {

    std::string s = "abcde";
    std::vector<std::string> v;

    TRY(v = subsets(s));
    TEST_EQUAL(v.size(), 32u);
    REQUIRE(v.size() == 32u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "");
    TEST_EQUAL(v[1], "a");
    TEST_EQUAL(v[2], "ab");
    TEST_EQUAL(v[3], "abc");
    TEST_EQUAL(v[4], "abcd");
    TEST_EQUAL(v[5], "abcde");
    TEST_EQUAL(v[6], "abce");
    TEST_EQUAL(v[7], "abd");
    TEST_EQUAL(v[8], "abde");
    TEST_EQUAL(v[9], "abe");
    TEST_EQUAL(v[10], "ac");
    TEST_EQUAL(v[11], "acd");
    TEST_EQUAL(v[12], "acde");
    TEST_EQUAL(v[13], "ace");
    TEST_EQUAL(v[14], "ad");
    TEST_EQUAL(v[15], "ade");
    TEST_EQUAL(v[16], "ae");
    TEST_EQUAL(v[17], "b");
    TEST_EQUAL(v[18], "bc");
    TEST_EQUAL(v[19], "bcd");
    TEST_EQUAL(v[20], "bcde");
    TEST_EQUAL(v[21], "bce");
    TEST_EQUAL(v[22], "bd");
    TEST_EQUAL(v[23], "bde");
    TEST_EQUAL(v[24], "be");
    TEST_EQUAL(v[25], "c");
    TEST_EQUAL(v[26], "cd");
    TEST_EQUAL(v[27], "cde");
    TEST_EQUAL(v[28], "ce");
    TEST_EQUAL(v[29], "d");
    TEST_EQUAL(v[30], "de");
    TEST_EQUAL(v[31], "e");

    TRY(v = subsets(s, 0));
    TEST_EQUAL(v.size(), 1u);
    REQUIRE(v.size() == 1u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "");

    TRY(v = subsets(s, 1));
    TEST_EQUAL(v.size(), 5u);
    REQUIRE(v.size() == 5u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "a");
    TEST_EQUAL(v[1], "b");
    TEST_EQUAL(v[2], "c");
    TEST_EQUAL(v[3], "d");
    TEST_EQUAL(v[4], "e");

    TRY(v = subsets(s, 2));
    TEST_EQUAL(v.size(), 10u);
    REQUIRE(v.size() == 10u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "ab");
    TEST_EQUAL(v[1], "ac");
    TEST_EQUAL(v[2], "ad");
    TEST_EQUAL(v[3], "ae");
    TEST_EQUAL(v[4], "bc");
    TEST_EQUAL(v[5], "bd");
    TEST_EQUAL(v[6], "be");
    TEST_EQUAL(v[7], "cd");
    TEST_EQUAL(v[8], "ce");
    TEST_EQUAL(v[9], "de");

    TRY(v = subsets(s, 3));
    TEST_EQUAL(v.size(), 10u);
    REQUIRE(v.size() == 10u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abc");
    TEST_EQUAL(v[1], "abd");
    TEST_EQUAL(v[2], "abe");
    TEST_EQUAL(v[3], "acd");
    TEST_EQUAL(v[4], "ace");
    TEST_EQUAL(v[5], "ade");
    TEST_EQUAL(v[6], "bcd");
    TEST_EQUAL(v[7], "bce");
    TEST_EQUAL(v[8], "bde");
    TEST_EQUAL(v[9], "cde");

    TRY(v = subsets(s, 4));
    TEST_EQUAL(v.size(), 5u);
    REQUIRE(v.size() == 5u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abcd");
    TEST_EQUAL(v[1], "abce");
    TEST_EQUAL(v[2], "abde");
    TEST_EQUAL(v[3], "acde");
    TEST_EQUAL(v[4], "bcde");

    TRY(v = subsets(s, 5));
    TEST_EQUAL(v.size(), 1u);
    REQUIRE(v.size() == 1u);
    std::sort(v.begin(), v.end());

    TEST_EQUAL(v[0], "abcde");

}
