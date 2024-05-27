#include "crow/newtype.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <functional>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Crow;
using namespace std::literals;

namespace {

    #define LIFE_CYCLE_TEST_TYPE(name, defcon, copy, move, equal) \
        struct name { \
            std::string str; \
            name() = defcon; \
            name(const name&) = copy; \
            name(name&&) = move; \
            name& operator=(const name&) = copy; \
            name& operator=(name&&) = move; \
            bool operator==(const name& b) const = equal; \
            explicit name(const char* p): str(p) {} \
            explicit name(const char* p, size_t n): str(p, n) {} \
        }; \
        using New ## name = Newtype<name>;

    LIFE_CYCLE_TEST_TYPE(DefaultMove, default, delete, default, delete);
    LIFE_CYCLE_TEST_TYPE(DefaultCopy, default, default, default, delete);
    LIFE_CYCLE_TEST_TYPE(MoveOnly, delete, delete, default, delete);
    LIFE_CYCLE_TEST_TYPE(CopyOnly, delete, default, default, delete);
    LIFE_CYCLE_TEST_TYPE(Equality, default, default, default, default);

    struct FixedStr {
        std::string s;
        std::string str() const { return s; }
    };

    struct VariableStr {
        std::string s;
        std::string str(const FormatSpec& spec) const { return format_object(s, spec); }
    };

    template <typename T>
    concept BooleanTestable = requires (T t) {
        { static_cast<bool>(t) };
        { ! t } -> std::convertible_to<bool>;
    };

    template <typename T>
    concept AdditionOperators = requires (T t, T u, T& tr) {
        { t + u } -> std::convertible_to<T>;
        { tr += u } -> std::convertible_to<T&>;
    };

    template <typename T>
    concept ArithmeticOperators = AdditionOperators<T>
        && requires (T t, T u, T& tr) {
            { + t } -> std::convertible_to<T>;
            { - t } -> std::convertible_to<T>;
            { t - u } -> std::convertible_to<T>;
            { t * u } -> std::convertible_to<T>;
            { t / u } -> std::convertible_to<T>;
            { tr -= u } -> std::convertible_to<T&>;
            { tr *= u } -> std::convertible_to<T&>;
            { tr /= u } -> std::convertible_to<T&>;
        };

    template <typename T>
    concept BitwiseOperators = requires (T t, T u, T& tr, int i) {
        { ~ t } -> std::convertible_to<T>;
        { t & u } -> std::convertible_to<T>;
        { t | u } -> std::convertible_to<T>;
        { t ^ u } -> std::convertible_to<T>;
        { t << i } -> std::convertible_to<T>;
        { t >> i } -> std::convertible_to<T>;
        { tr &= u } -> std::convertible_to<T&>;
        { tr |= u } -> std::convertible_to<T&>;
        { tr ^= u } -> std::convertible_to<T&>;
        { tr <<= i } -> std::convertible_to<T&>;
        { tr >>= i } -> std::convertible_to<T&>;
    };

    template <typename T>
    concept IntegerOperators = ArithmeticOperators<T>
        && BitwiseOperators<T>
        && requires (T t, T u, T& tr) {
            { ++ tr } -> std::convertible_to<T&>;
            { tr ++ } -> std::convertible_to<T>;
            { -- tr } -> std::convertible_to<T&>;
            { tr -- } -> std::convertible_to<T>;
            { t % u } -> std::convertible_to<T>;
            { tr %= u } -> std::convertible_to<T&>;
        };

    template <typename T>
    concept Indexed = requires (T t, size_t i) {
        { t[i] };
    };

    template <typename T>
    concept MutableIndexed = requires (T& t, size_t i) {
        { t[i] } -> Detail::MutableReference;
    };

}

void test_crow_newtype_constructors() {

    TEST(std::default_initializable<DefaultMove>);  TEST(std::default_initializable<NewDefaultMove>);
    TEST(std::movable<DefaultMove>);                TEST(std::movable<NewDefaultMove>);
    TEST(! std::copyable<DefaultMove>);             TEST(! std::copyable<NewDefaultMove>);

    TEST(std::default_initializable<DefaultCopy>);  TEST(std::default_initializable<NewDefaultCopy>);
    TEST(std::movable<DefaultCopy>);                TEST(std::movable<NewDefaultCopy>);
    TEST(std::copyable<DefaultCopy>);               TEST(std::copyable<NewDefaultCopy>);

    TEST(! std::default_initializable<MoveOnly>);  TEST(! std::default_initializable<NewMoveOnly>);
    TEST(std::movable<MoveOnly>);                  TEST(std::movable<NewMoveOnly>);
    TEST(! std::copyable<MoveOnly>);               TEST(! std::copyable<NewMoveOnly>);

    TEST(! std::default_initializable<CopyOnly>);  TEST(! std::default_initializable<NewCopyOnly>);
    TEST(std::movable<CopyOnly>);                  TEST(std::movable<NewCopyOnly>);
    TEST(std::copyable<CopyOnly>);                 TEST(std::copyable<NewCopyOnly>);

    DefaultMove dm;
    DefaultCopy dc;

    TRY(dm = DefaultMove("Hello world"));     TEST_EQUAL(dm.str, "Hello world");
    TRY(dc = DefaultCopy("Hello world"));     TEST_EQUAL(dc.str, "Hello world");
    TRY(dm = DefaultMove("Hello world", 5));  TEST_EQUAL(dm.str, "Hello");
    TRY(dc = DefaultCopy("Hello world", 5));  TEST_EQUAL(dc.str, "Hello");

}

void test_crow_newtype_conversions() {

    using NS1 = Newtype<std::string, Ntag<1>>;
    using NS2 = Newtype<std::string, Ntag<2>>;
    using NS3 = Newtype<std::string, Ntag<0>, Ntype::construct>;
    using NS4 = Newtype<std::string, Ntag<0>, Ntype::convert>;

    TEST((std::constructible_from<NS1, std::string>));  TEST((std::constructible_from<std::string, NS1>));
    TEST((std::constructible_from<NS2, std::string>));  TEST((std::constructible_from<std::string, NS2>));
    TEST((std::constructible_from<NS3, std::string>));  TEST((std::constructible_from<std::string, NS3>));
    TEST((std::constructible_from<NS4, std::string>));  TEST((std::constructible_from<std::string, NS4>));
    TEST((std::constructible_from<NS2, NS1>));          TEST((std::constructible_from<NS1, NS2>));

    TEST((! std::convertible_to<std::string, NS1>));  TEST((! std::convertible_to<NS1, std::string>));
    TEST((! std::convertible_to<std::string, NS2>));  TEST((! std::convertible_to<NS2, std::string>));
    TEST((std::convertible_to<std::string, NS3>));    TEST((! std::convertible_to<NS3, std::string>));
    TEST((! std::convertible_to<std::string, NS4>));  TEST((std::convertible_to<NS4, std::string>));
    TEST((! std::convertible_to<NS1, NS2>));          TEST((! std::convertible_to<NS2, NS1>));

    NS1 ns1;
    NS2 ns2;
    NS3 ns3;
    NS4 ns4;
    std::string s;

    TRY(ns1 = NS1("Alpha"s));    TEST_EQUAL(*ns1, "Alpha");
    TRY(ns2 = NS2("Alpha"s));    TEST_EQUAL(*ns2, "Alpha");
    TRY(ns3 = "Bravo"s);         TEST_EQUAL(*ns3, "Bravo");
    TRY(ns4 = NS4("Charlie"s));  TEST_EQUAL(*ns4, "Charlie");

    TRY(s = std::string(ns1));  TEST_EQUAL(s, "Alpha");
    TRY(s = std::string(ns2));  TEST_EQUAL(s, "Alpha");
    TRY(s = std::string(ns3));  TEST_EQUAL(s, "Bravo");
    TRY(s = ns4);               TEST_EQUAL(s, "Charlie");

    using NI = Newtype<int>;

    TEST(BooleanTestable<NI>);
    TEST(! BooleanTestable<NS1>);

    NI ni(0);

    TEST(! ni);
    TRY(ni = NI(1));
    TEST(ni);

}

void test_crow_newtype_arithmetic_operators() {

    using NI = Newtype<int>;
    using ND = Newtype<double>;
    using NS = Newtype<std::string>;

    TEST(AdditionOperators<NI>);  TEST(ArithmeticOperators<NI>);    TEST(BitwiseOperators<NI>);    TEST(IntegerOperators<NI>);
    TEST(AdditionOperators<ND>);  TEST(ArithmeticOperators<ND>);    TEST(! BitwiseOperators<ND>);  TEST(! IntegerOperators<ND>);
    TEST(AdditionOperators<NS>);  TEST(! ArithmeticOperators<NS>);  TEST(! BitwiseOperators<NS>);  TEST(! IntegerOperators<NS>);

    NI i(3), j(5), k, l;
    ND x(1.25), y(2.5), z;
    NS s("abc"), t("xyz"), u;

    TRY(k = + i);    TEST_EQUAL(*k, 3);
    TRY(k = - i);    TEST_EQUAL(*k, -3);
    TRY(k = i + j);  TEST_EQUAL(*k, 8);
    TRY(k = i - j);  TEST_EQUAL(*k, -2);
    TRY(k = i * j);  TEST_EQUAL(*k, 15);
    TRY(k = i / j);  TEST_EQUAL(*k, 0);
    TRY(k = i % j);  TEST_EQUAL(*k, 3);

    TRY(k = i);  TRY(l = ++ k);  TEST_EQUAL(*k, 4);  TEST_EQUAL(*l, 4);
    TRY(k = i);  TRY(l = k ++);  TEST_EQUAL(*k, 4);  TEST_EQUAL(*l, 3);
    TRY(k = i);  TRY(l = -- k);  TEST_EQUAL(*k, 2);  TEST_EQUAL(*l, 2);
    TRY(k = i);  TRY(l = k --);  TEST_EQUAL(*k, 2);  TEST_EQUAL(*l, 3);

    TRY(k = i);  TRY(k += j);  TEST_EQUAL(*k, 8);
    TRY(k = i);  TRY(k -= j);  TEST_EQUAL(*k, -2);
    TRY(k = i);  TRY(k *= j);  TEST_EQUAL(*k, 15);
    TRY(k = i);  TRY(k /= j);  TEST_EQUAL(*k, 0);
    TRY(k = i);  TRY(k %= j);  TEST_EQUAL(*k, 3);

    TRY(z = - x);    TEST_EQUAL(*z, -1.25);
    TRY(z = x + y);  TEST_EQUAL(*z, 3.75);
    TRY(z = x + y);  TEST_EQUAL(*z, 3.75);
    TRY(z = x - y);  TEST_EQUAL(*z, -1.25);
    TRY(z = x * y);  TEST_EQUAL(*z, 3.125);
    TRY(z = x / y);  TEST_EQUAL(*z, 0.5);

    TRY(z = x);  TRY(z += y);  TEST_EQUAL(*z, 3.75);
    TRY(z = x);  TRY(z -= y);  TEST_EQUAL(*z, -1.25);
    TRY(z = x);  TRY(z *= y);  TEST_EQUAL(*z, 3.125);
    TRY(z = x);  TRY(z /= y);  TEST_EQUAL(*z, 0.5);

    TRY(u = s + t);  TEST_EQUAL(*u, "abcxyz");

    TRY(u = s);  TRY(u += t);  TEST_EQUAL(*u, "abcxyz");

}

void test_crow_newtype_bitwise_operators() {

    using NI = Newtype<int>;
    using ND = Newtype<double>;
    using NS = Newtype<std::string>;

    TEST(BitwiseOperators<NI>);
    TEST(! BitwiseOperators<ND>);
    TEST(! BitwiseOperators<NS>);

    NI i(3), j(5), k;

    TRY(k = i & j);   TEST_EQUAL(*k, 1);
    TRY(k = i | j);   TEST_EQUAL(*k, 7);
    TRY(k = i ^ j);   TEST_EQUAL(*k, 6);
    TRY(k = i << 1);  TEST_EQUAL(*k, 6);
    TRY(k = i >> 1);  TEST_EQUAL(*k, 1);

    TRY(k = i);  TRY(k &= j);   TEST_EQUAL(*k, 1);
    TRY(k = i);  TRY(k |= j);   TEST_EQUAL(*k, 7);
    TRY(k = i);  TRY(k ^= j);   TEST_EQUAL(*k, 6);
    TRY(k = i);  TRY(k <<= 1);  TEST_EQUAL(*k, 6);
    TRY(k = i);  TRY(k >>= 1);  TEST_EQUAL(*k, 1);

}

void test_crow_newtype_comparison_operators() {

    TEST(! std::equality_comparable<NewDefaultCopy>);
    TEST(std::equality_comparable<NewEquality>);
    TEST(std::equality_comparable<std::string>);

    TEST(! std::totally_ordered<NewDefaultCopy>);
    TEST(! std::totally_ordered<NewEquality>);
    TEST(std::totally_ordered<std::string>);

    NewEquality a("abc"), b("abc"), c("xyz");
    Newtype<std::string> d("abc"), e("abc"), f("xyz");
    Newtype<std::string, Ntag<0>, Ntype::compare> g("abc"), h("abc"), i("xyz");
    auto so = std::strong_ordering::equal;

    TEST(a == b);
    TEST(a != c);
    TEST(! (a != b));
    TEST(! (a == c));
    TEST(d == e);
    TEST(d <= e);
    TEST(d >= e);
    TEST(! (d != e));
    TEST(! (d < e));
    TEST(! (d > e));
    TEST(d != f);
    TEST(d < f);
    TEST(d <= f);
    TEST(! (d == f));
    TEST(! (d > f));
    TEST(! (d >= f));

    TEST(g == h);
    TEST(g <= h);
    TEST(g >= h);
    TEST(! (g != h));
    TEST(! (g < h));
    TEST(! (g > h));
    TEST(g != i);
    TEST(g < i);
    TEST(g <= i);
    TEST(! (g == i));
    TEST(! (g > i));
    TEST(! (g >= i));

    TRY(so = d <=> e);
    TEST(so == std::strong_ordering::equal);
    TRY(so = d <=> f);
    TEST(so == std::strong_ordering::less);

    TEST_EQUAL(g, "abc");  TEST_EQUAL("abc", g);
    TEST_EQUAL(h, "abc");  TEST_EQUAL("abc", h);
    TEST_EQUAL(i, "xyz");  TEST_EQUAL("xyz", i);

}

void test_crow_newtype_formatting_functions() {

    Newtype<int> ni(42);
    Newtype<std::string> ns("Hello world"), nt;
    Newtype<FixedStr> nfs{FixedStr{"Alpha"}};
    Newtype<VariableStr> nvs{VariableStr{"Bravo"}};
    std::string s;

    TEST_EQUAL(format_object(ni),        "42");
    TEST_EQUAL(format_object(ni, "x4"),  "002a");
    TEST_EQUAL(format_object(ns),        "Hello world");
    TEST_EQUAL(format_object(ns, "x"),   "48 65 6c 6c 6f 20 77 6f 72 6c 64");
    TEST_EQUAL(format_object(nfs),       "Alpha");
    TEST_EQUAL(format_object(nfs, "x"),  "Alpha");
    TEST_EQUAL(format_object(nvs),       "Bravo");
    TEST_EQUAL(format_object(nvs, "x"),  "42 72 61 76 6f");

    {
        std::istringstream in("Hello");
        TRY(in >> nt);
        TEST_EQUAL(*nt, "Hello");
    }

    {
        std::ostringstream out;
        TRY(*nt = "Goodbye");
        TRY(out << nt);
        s = out.str();
        TEST_EQUAL(s, "Goodbye");
    }

    {
        std::ostringstream out;
        TRY(out << nfs);
        s = out.str();
        TEST_EQUAL(s, "Alpha");
    }

    {
        std::ostringstream out;
        TRY(out << nvs);
        s = out.str();
        TEST_EQUAL(s, "Bravo");
    }

}

void test_crow_newtype_hash_functions() {

    using NS = Newtype<std::string>;

    std::string a, b("Hello world"), c("Goodbye");
    NS s(a), t(b), u(c);
    std::hash<std::string> hs;
    size_t m = 0, n = 0;

    m = hs(a);  TRY(n = s.hash());  TEST_EQUAL(m, n);
    m = hs(b);  TRY(n = t.hash());  TEST_EQUAL(m, n);
    m = hs(c);  TRY(n = u.hash());  TEST_EQUAL(m, n);

    std::unordered_set<NS> set;

    TRY(set.insert(s));
    TRY(set.insert(t));
    TRY(set.insert(u));
    TRY(set.insert(s));
    TRY(set.insert(t));
    TRY(set.insert(u));

    TEST_EQUAL(set.size(), 3u);
    TEST(set.contains(s));
    TEST(set.contains(t));
    TEST(set.contains(u));

}

void test_crow_newtype_range_access() {

    using NI = Newtype<int>;
    using NS = Newtype<std::string>;
    using NV = Newtype<std::vector<bool>>;

    TEST(! std::ranges::range<NI>);
    TEST(! std::ranges::random_access_range<NI>);
    TEST(! Indexed<NI>);
    TEST(! MutableIndexed<NI>);

    TEST(std::ranges::range<NS>);
    TEST(std::ranges::random_access_range<NS>);
    TEST(Indexed<NS>);
    TEST(MutableIndexed<NS>);

    TEST(std::ranges::range<NV>);
    TEST(std::ranges::random_access_range<NV>);
    TEST(Indexed<NV>);
    TEST(! MutableIndexed<NV>);

    NS ns("Hello");
    const auto& cns = ns;

    TEST_EQUAL(ns[0], 'H');   TEST_EQUAL(cns[0], 'H');
    TEST_EQUAL(ns[1], 'e');   TEST_EQUAL(cns[1], 'e');
    TEST_EQUAL(ns[2], 'l');   TEST_EQUAL(cns[2], 'l');
    TEST_EQUAL(ns[3], 'l');   TEST_EQUAL(cns[3], 'l');
    TEST_EQUAL(ns[4], 'o');   TEST_EQUAL(cns[4], 'o');
    TEST_EQUAL(ns[5], '\0');  TEST_EQUAL(cns[5], '\0');

    TRY(ns[4] = '!');
    TEST_EQUAL(*ns, "Hell!");

    int n = 0;
    for (auto c: ns)
        if (c >= 'a' && c <= 'z')
            ++n;
    TEST_EQUAL(n, 3);

    for (auto& c: ns)
        c = '*';
    TEST_EQUAL(*ns, "*****");

}

void test_crow_newtype_string_functions() {

    Newtype<std::string> ns("Hello world"), nt;

    TRY(nt = ns.substr(0, 5));  TEST_EQUAL(*nt, "Hello");
    TRY(nt = ns.substr(6, 5));  TEST_EQUAL(*nt, "world");
    TRY(nt = ns.substr(6));     TEST_EQUAL(*nt, "world");

}
