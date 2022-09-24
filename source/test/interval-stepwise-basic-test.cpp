#include "crow/interval.hpp"
#include "crow/interval-set.hpp"
#include "crow/interval-types.hpp"
#include "crow/unit-test.hpp"
#include "test/interval-stepwise-example.hpp"
#include <compare>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Crow;

using Itv = Interval<Stepwise>;
using Set = IntervalSet<Stepwise>;
using IB = IntervalBound;
using IC = IntervalCategory;
using IM = IntervalMatch;
using IO = IntervalOrder;
using SO = std::strong_ordering;

constexpr auto eq = SO::equal;
constexpr auto lt = SO::less;
constexpr auto gt = SO::greater;

void test_crow_interval_stepwise_interval_basic_properties() {

    Itv in;

    TEST_TYPE(Itv::value_type, Stepwise);
    TEST_EQUAL(Itv::category, IC::stepwise);
    TEST(in.empty());
    TEST_EQUAL(in.size(), 0u);
    TEST(! in);
    TEST_EQUAL(in.str(), "{}");
    TEST_EQUAL(in.match(0_sw), IM::empty);
    TEST_EQUAL(in.match(42_sw), IM::empty);
    TEST(! in(0_sw));
    TEST(! in(42_sw));

}

void test_crow_interval_stepwise_interval_construction() {

    Itv in;
    std::string str;

    TRY(in = Itv());                                  TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw));                              TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = Itv(1_sw, IB::closed, IB::closed));      TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = Itv(1_sw, IB::closed, IB::open));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw, IB::open, IB::closed));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = Itv(1_sw, IB::closed, IB::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = Itv(1_sw, IB::unbound, IB::closed));     TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = Itv(1_sw, IB::open, IB::unbound));       TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = Itv(1_sw, IB::unbound, IB::open));       TRY(str = in.str());  TEST_EQUAL(str, "<=0");
    TRY(in = Itv(1_sw, IB::unbound, IB::unbound));    TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = Itv(1_sw, 5_sw));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, IB::closed));            TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, IB::open));              TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = Itv(1_sw, 5_sw, IB::closed, IB::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = Itv(1_sw, 5_sw, IB::open, IB::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = Itv(1_sw, 5_sw, "[]"));                  TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = Itv(1_sw, 5_sw, "()"));                  TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = Itv(1_sw, 5_sw, "[)"));                  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = Itv(1_sw, 5_sw, "(]"));                  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = Itv(1_sw, 5_sw, "<"));                   TRY(str = in.str());  TEST_EQUAL(str, "<=4");
    TRY(in = Itv(1_sw, 5_sw, "<="));                  TRY(str = in.str());  TEST_EQUAL(str, "<=5");
    TRY(in = Itv(1_sw, 5_sw, ">"));                   TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = Itv(1_sw, 5_sw, ">="));                  TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = Itv(1_sw, 5_sw, "*"));                   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = Itv(5_sw, 1_sw));                        TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = make_interval(1_sw));                              TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1_sw, IB::closed, IB::closed));      TRY(str = in.str());  TEST_EQUAL(str, "1");
    TRY(in = make_interval(1_sw, IB::closed, IB::open));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1_sw, IB::open, IB::closed));        TRY(str = in.str());  TEST_EQUAL(str, "{}");
    TRY(in = make_interval(1_sw, IB::closed, IB::unbound));     TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1_sw, IB::unbound, IB::closed));     TRY(str = in.str());  TEST_EQUAL(str, "<=1");
    TRY(in = make_interval(1_sw, IB::open, IB::unbound));       TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1_sw, IB::unbound, IB::open));       TRY(str = in.str());  TEST_EQUAL(str, "<=0");
    TRY(in = make_interval(1_sw, IB::unbound, IB::unbound));    TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(1_sw, 5_sw));                        TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, IB::closed));            TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, IB::open));              TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1_sw, 5_sw, IB::closed, IB::open));  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1_sw, 5_sw, IB::open, IB::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1_sw, 5_sw, "[]"));                  TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = make_interval(1_sw, 5_sw, "()"));                  TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = make_interval(1_sw, 5_sw, "[)"));                  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");
    TRY(in = make_interval(1_sw, 5_sw, "(]"));                  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = make_interval(1_sw, 5_sw, "<"));                   TRY(str = in.str());  TEST_EQUAL(str, "<=4");
    TRY(in = make_interval(1_sw, 5_sw, "<="));                  TRY(str = in.str());  TEST_EQUAL(str, "<=5");
    TRY(in = make_interval(1_sw, 5_sw, ">"));                   TRY(str = in.str());  TEST_EQUAL(str, ">=2");
    TRY(in = make_interval(1_sw, 5_sw, ">="));                  TRY(str = in.str());  TEST_EQUAL(str, ">=1");
    TRY(in = make_interval(1_sw, 5_sw, "*"));                   TRY(str = in.str());  TEST_EQUAL(str, "*");
    TRY(in = make_interval(5_sw, 1_sw));                        TRY(str = in.str());  TEST_EQUAL(str, "{}");

    TRY(in = ordered_interval(5_sw, 1_sw, IB::closed));            TRY(str = in.str());  TEST_EQUAL(str, "[1,5]");
    TRY(in = ordered_interval(5_sw, 1_sw, IB::open));              TRY(str = in.str());  TEST_EQUAL(str, "[2,4]");
    TRY(in = ordered_interval(5_sw, 1_sw, IB::closed, IB::open));  TRY(str = in.str());  TEST_EQUAL(str, "[2,5]");
    TRY(in = ordered_interval(5_sw, 1_sw, IB::open, IB::closed));  TRY(str = in.str());  TEST_EQUAL(str, "[1,4]");

}

void test_crow_interval_stepwise_interval_from_string() {

    Itv in;

    TRY(in = Itv::from_string(""));         TEST_EQUAL(in.str(), "{}");
    TRY(in = Itv::from_string("{}"));       TEST_EQUAL(in.str(), "{}");
    TRY(in = Itv::from_string("*"));        TEST_EQUAL(in.str(), "*");
    TRY(in = Itv::from_string("5"));        TEST_EQUAL(in.str(), "5");
    TRY(in = Itv::from_string(">5"));       TEST_EQUAL(in.str(), ">=6");
    TRY(in = Itv::from_string("<5"));       TEST_EQUAL(in.str(), "<=4");
    TRY(in = Itv::from_string(">=5"));      TEST_EQUAL(in.str(), ">=5");
    TRY(in = Itv::from_string("<=5"));      TEST_EQUAL(in.str(), "<=5");
    TRY(in = Itv::from_string("5+"));       TEST_EQUAL(in.str(), ">=5");
    TRY(in = Itv::from_string("5-"));       TEST_EQUAL(in.str(), "<=5");
    TRY(in = Itv::from_string("[5,10]"));   TEST_EQUAL(in.str(), "[5,10]");
    TRY(in = Itv::from_string("[5,10)"));   TEST_EQUAL(in.str(), "[5,9]");
    TRY(in = Itv::from_string("(5,10]"));   TEST_EQUAL(in.str(), "[6,10]");
    TRY(in = Itv::from_string("(5,10)"));   TEST_EQUAL(in.str(), "[6,9]");
    TRY(in = Itv::from_string("5-10"));     TEST_EQUAL(in.str(), "[5,10]");
    TRY(in = Itv::from_string("5..10"));    TEST_EQUAL(in.str(), "[5,10]");
    TRY(in = Itv::from_string("5...10"));   TEST_EQUAL(in.str(), "[5,10]");
    TRY(in = Itv::from_string("5..<10"));   TEST_EQUAL(in.str(), "[5,9]");
    TRY(in = Itv::from_string("5<..10"));   TEST_EQUAL(in.str(), "[6,10]");
    TRY(in = Itv::from_string("5<..<10"));  TEST_EQUAL(in.str(), "[6,9]");

}

void test_crow_interval_stepwise_interval_iterators() {

    Itv in;
    Itv::iterator it1 = {}, it2 = {};
    ptrdiff_t diff = 0;

    TRY(in = {});               TEST_EQUAL(in.size(), 0u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 0);
    TRY(in = 1_sw);             TEST_EQUAL(in.size(), 1u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 1);
    TRY(in = Itv(1_sw, 5_sw));  TEST_EQUAL(in.size(), 5u);  TRY(it1 = in.begin());  TRY(it2 = in.end());  TRY(diff = std::distance(it1, it2));  TEST_EQUAL(diff, 5);

    TEST_EQUAL(*it1, 1_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 2_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 3_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 4_sw);  TRY(++it1);
    TEST_EQUAL(*it1, 5_sw);  TRY(++it1);
    TEST(it1 == it2);

}

void test_crow_interval_stepwise_interval_order() {

    Itv a, b;
    IO io = {};

    TRY(a = Itv());            TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::both_empty);         // ...
    TRY(a = Itv());            TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // bbb
    TRY(a = Itv());            TRY(b = Itv(10));          TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_only);             // b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(25,30,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_below_b);          // aaa...bbb
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_below_b);          // aaa.bbb
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_touches_b);        // aaabbb
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv(10,10,">"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_touches_b);        // aaabbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(20,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_touches_b);        // aaabbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(20,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_touches_b);        // aaabbb
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv(10,10,">="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(20,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // aaa*bbb
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(15,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // aaa***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(15,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // aaa***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,25,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // a***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_overlaps_b);       // a***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_below_b);  // aaa***
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(15,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_below_b);  // aaa***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_below_b);  // a***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_below_b);  // a***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(12,18,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_encloses_b);       // aaa***aaa
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(12,18,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_encloses_b);       // aaa***aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(15,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_encloses_b);       // aaa***a
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,15,"(]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_encloses_b);       // a***aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_encloses_b);       // a***a
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,25,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_above_a);  // ***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,25,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_above_a);  // ***bbb
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_above_a);  // ***b
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::equal);              // ***
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::equal);              // ***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::equal);              // ***
    TRY(a = Itv(10,20,"[)"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::equal);              // ***
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_above_b);  // ***a
    TRY(a = Itv(10,25,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_above_b);  // ***aaa
    TRY(a = Itv(10,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_extends_above_b);  // ***aaa
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_encloses_a);       // b***b
    TRY(a = Itv(10,15,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_encloses_a);       // b***bbb
    TRY(a = Itv(15,20,"[)"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_encloses_a);       // bbb***b
    TRY(a = Itv(12,18,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_encloses_a);       // bbb***bbb
    TRY(a = Itv(12,18,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_encloses_a);       // bbb***bbb
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_below_a);  // b***
    TRY(a = Itv(10,20,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_below_a);  // b***
    TRY(a = Itv(15,20,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_below_a);  // bbb***
    TRY(a = Itv(15,20,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_extends_below_a);  // bbb***
    TRY(a = Itv(10,20,"(]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // b***a
    TRY(a = Itv(10,25,"(]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // b***aaa
    TRY(a = Itv(15,20,"[]"));  TRY(b = Itv(10,20,"[)"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // bbb***a
    TRY(a = Itv(15,25,"()"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv(15,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // bbb***aaa
    TRY(a = Itv(20,25,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_overlaps_a);       // bbb*aaa
    TRY(a = Itv(20,25,"[]"));  TRY(b = Itv(10,20,"()"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_touches_a);        // bbbaaa
    TRY(a = Itv(20,25,"()"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv(10,10,"<="));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_touches_a);        // bbbaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv(10,10,"<"));   TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_below_a);          // bbb.aaa
    TRY(a = Itv(25,30,"[]"));  TRY(b = Itv(10,20,"[]"));  TRY(io = a.order(b));  TEST_EQUAL(io, IO::b_below_a);          // bbb...aaa
    TRY(a = Itv(10));          TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // a
    TRY(a = Itv(10,20,"()"));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaa
    TRY(a = Itv(10,20,"[]"));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaa
    TRY(a = Itv(10,10,"<="));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaaa
    TRY(a = Itv(10,10,"<"));   TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaaa
    TRY(a = Itv(10,10,">="));  TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaaa
    TRY(a = Itv(10,10,">"));   TRY(b = Itv());            TRY(io = a.order(b));  TEST_EQUAL(io, IO::a_only);             // aaaa

}

void test_crow_interval_stepwise_interval_complement() {

    Itv in;
    Set set;
    std::string str;

    TRY((in = {}));                  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{*}");
    TRY((in = Itv::all()));          TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{}");
    TRY((in = 42_sw));               TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=41,>=43}");
    TRY((in = {42_sw,42_sw,"<"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>=42}");
    TRY((in = {42_sw,42_sw,"<="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{>=43}");
    TRY((in = {42_sw,42_sw,">"}));   TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=42}");
    TRY((in = {42_sw,42_sw,">="}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=41}");
    TRY((in = {86_sw,99_sw,"()"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=99}");
    TRY((in = {86_sw,99_sw,"(]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=86,>=100}");
    TRY((in = {86_sw,99_sw,"[)"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=85,>=99}");
    TRY((in = {86_sw,99_sw,"[]"}));  TRY(set = ~ in);  TRY(str = set.str());  TEST_EQUAL(str, "{<=85,>=100}");

}

void test_crow_interval_stepwise_interval_binary_operations() {

    struct test_info {
        int line;
        Itv lhs;
        Itv rhs;
        SO compare;
        bool includes;
        bool overlaps;
        bool touches;
        std::string envelope;
        std::string set_intersection;
        std::string set_union;
        std::string set_difference;
        std::string set_symmetric_difference;
    };

    static const std::vector<test_info> tests = {

        // line      lhs                rhs                cmp  incl    over    touch   envel     inter     union           diff            symm              --
        { __LINE__,  {},                {},                eq,  false,  false,  false,  "{}",     "{}",     "{}",           "{}",           "{}",             },
        { __LINE__,  {0_sw,0_sw,"*"},   {},                gt,  false,  false,  false,  "*",      "{}",     "{*}",          "{*}",          "{*}",            },
        { __LINE__,  {0_sw,0_sw,"*"},   {0_sw,0_sw,"*"},   eq,  true,   true,   true,   "*",      "*",      "{*}",          "{}",           "{}",             },
        { __LINE__,  {3_sw},            {},                gt,  false,  false,  false,  "3",      "{}",     "{3}",          "{3}",          "{3}",            },
        { __LINE__,  {3_sw},            {0_sw,0_sw,"*"},   gt,  false,  true,   true,   "*",      "3",      "{*}",          "{}",           "{<=2,>=4}",      },
        { __LINE__,  {3_sw},            {1_sw},            gt,  false,  false,  false,  "[1,3]",  "{}",     "{1,3}",        "{3}",          "{1,3}",          },
        { __LINE__,  {3_sw},            {2_sw},            gt,  false,  false,  true,   "[2,3]",  "{}",     "{[2,3]}",      "{3}",          "{[2,3]}",        },
        { __LINE__,  {3_sw},            {3_sw},            eq,  true,   true,   true,   "3",      "3",      "{3}",          "{}",           "{}",             },
        { __LINE__,  {3_sw},            {4_sw},            lt,  false,  false,  true,   "[3,4]",  "{}",     "{[3,4]}",      "{3}",          "{[3,4]}",        },
        { __LINE__,  {3_sw},            {5_sw},            lt,  false,  false,  false,  "[3,5]",  "{}",     "{3,5}",        "{3}",          "{3,5}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {},                gt,  false,  false,  false,  "<=2",    "{}",     "{<=2}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {0_sw,0_sw,"*"},   lt,  false,  true,   true,   "*",      "<=2",    "{*}",          "{}",           "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {1_sw},            lt,  true,   true,   true,   "<=2",    "1",      "{<=2}",        "{<=0,2}",      "{<=0,2}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {2_sw},            lt,  true,   true,   true,   "<=2",    "2",      "{<=2}",        "{<=1}",        "{<=1}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {3_sw},            lt,  false,  false,  true,   "<=3",    "{}",     "{<=3}",        "{<=2}",        "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,"<"},   {4_sw},            lt,  false,  false,  false,  "<=4",    "{}",     "{<=2,4}",      "{<=2}",        "{<=2,4}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {5_sw},            lt,  false,  false,  false,  "<=5",    "{}",     "{<=2,5}",      "{<=2}",        "{<=2,5}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {1_sw,1_sw,"<"},   gt,  true,   true,   true,   "<=2",    "<=0",    "{<=2}",        "{[1,2]}",      "{[1,2]}",        },
        { __LINE__,  {3_sw,3_sw,"<"},   {2_sw,2_sw,"<"},   gt,  true,   true,   true,   "<=2",    "<=1",    "{<=2}",        "{2}",          "{2}",            },
        { __LINE__,  {3_sw,3_sw,"<"},   {3_sw,3_sw,"<"},   eq,  true,   true,   true,   "<=2",    "<=2",    "{<=2}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<"},   {4_sw,4_sw,"<"},   lt,  false,  true,   true,   "<=3",    "<=2",    "{<=3}",        "{}",           "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<"},   {5_sw,5_sw,"<"},   lt,  false,  true,   true,   "<=4",    "<=2",    "{<=4}",        "{}",           "{[3,4]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {},                gt,  false,  false,  false,  "<=3",    "{}",     "{<=3}",        "{<=3}",        "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {0_sw,0_sw,"*"},   lt,  false,  true,   true,   "*",      "<=3",    "{*}",          "{}",           "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw},            lt,  true,   true,   true,   "<=3",    "1",      "{<=3}",        "{<=0,[2,3]}",  "{<=0,[2,3]}",    },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw},            lt,  true,   true,   true,   "<=3",    "2",      "{<=3}",        "{<=1,3}",      "{<=1,3}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw},            lt,  true,   true,   true,   "<=3",    "3",      "{<=3}",        "{<=2}",        "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw},            lt,  false,  false,  true,   "<=4",    "{}",     "{<=4}",        "{<=3}",        "{<=4}",          },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw},            lt,  false,  false,  false,  "<=5",    "{}",     "{<=3,5}",      "{<=3}",        "{<=3,5}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw,1_sw,"<"},   gt,  true,   true,   true,   "<=3",    "<=0",    "{<=3}",        "{[1,3]}",      "{[1,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw,2_sw,"<"},   gt,  true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw,3_sw,"<"},   gt,  true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw,4_sw,"<"},   eq,  true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw,5_sw,"<"},   lt,  false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {1_sw,1_sw,"<="},  gt,  true,   true,   true,   "<=3",    "<=1",    "{<=3}",        "{[2,3]}",      "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,"<="},  {2_sw,2_sw,"<="},  gt,  true,   true,   true,   "<=3",    "<=2",    "{<=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {3_sw,3_sw,"<="},  eq,  true,   true,   true,   "<=3",    "<=3",    "{<=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,"<="},  {4_sw,4_sw,"<="},  lt,  false,  true,   true,   "<=4",    "<=3",    "{<=4}",        "{}",           "{4}",            },
        { __LINE__,  {3_sw,3_sw,"<="},  {5_sw,5_sw,"<="},  lt,  false,  true,   true,   "<=5",    "<=3",    "{<=5}",        "{}",           "{[4,5]}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {},                gt,  false,  false,  false,  ">=4",    "{}",     "{>=4}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {0_sw,0_sw,"*"},   gt,  false,  true,   true,   "*",      ">=4",    "{*}",          "{}",           "{<=3}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw},            gt,  false,  false,  false,  ">=1",    "{}",     "{1,>=4}",      "{>=4}",        "{1,>=4}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw},            gt,  false,  false,  false,  ">=2",    "{}",     "{2,>=4}",      "{>=4}",        "{2,>=4}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw},            gt,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{>=4}",        "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw},            gt,  true,   true,   true,   ">=4",    "4",      "{>=4}",        "{>=5}",        "{>=5}",          },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw},            lt,  true,   true,   true,   ">=4",    "5",      "{>=4}",        "{4,>=6}",      "{4,>=6}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,"<"},   gt,  false,  false,  false,  "*",      "{}",     "{<=0,>=4}",    "{>=4}",        "{<=0,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,"<"},   gt,  false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,"<"},   gt,  false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,"<"},   gt,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,"<"},   gt,  false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,"<="},  gt,  false,  false,  false,  "*",      "{}",     "{<=1,>=4}",    "{>=4}",        "{<=1,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,"<="},  gt,  false,  false,  false,  "*",      "{}",     "{<=2,>=4}",    "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,"<="},  gt,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=4}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,"<="},  gt,  false,  true,   true,   "*",      "4",      "{*}",          "{>=5}",        "{<=3,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,"<="},  gt,  false,  true,   true,   "*",      "[4,5]",  "{*}",          "{>=6}",        "{<=3,>=6}",      },
        { __LINE__,  {3_sw,3_sw,">"},   {1_sw,1_sw,">"},   gt,  false,  true,   true,   ">=2",    ">=4",    "{>=2}",        "{}",           "{[2,3]}",        },
        { __LINE__,  {3_sw,3_sw,">"},   {2_sw,2_sw,">"},   gt,  false,  true,   true,   ">=3",    ">=4",    "{>=3}",        "{}",           "{3}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {3_sw,3_sw,">"},   eq,  true,   true,   true,   ">=4",    ">=4",    "{>=4}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">"},   {4_sw,4_sw,">"},   lt,  true,   true,   true,   ">=4",    ">=5",    "{>=4}",        "{4}",          "{4}",            },
        { __LINE__,  {3_sw,3_sw,">"},   {5_sw,5_sw,">"},   lt,  true,   true,   true,   ">=4",    ">=6",    "{>=4}",        "{[4,5]}",      "{[4,5]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {},                gt,  false,  false,  false,  ">=3",    "{}",     "{>=3}",        "{>=3}",        "{>=3}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {0_sw,0_sw,"*"},   gt,  false,  true,   true,   "*",      ">=3",    "{*}",          "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw},            gt,  false,  false,  false,  ">=1",    "{}",     "{1,>=3}",      "{>=3}",        "{1,>=3}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw},            gt,  false,  false,  true,   ">=2",    "{}",     "{>=2}",        "{>=3}",        "{>=2}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw},            gt,  true,   true,   true,   ">=3",    "3",      "{>=3}",        "{>=4}",        "{>=4}",          },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw},            lt,  true,   true,   true,   ">=3",    "4",      "{>=3}",        "{3,>=5}",      "{3,>=5}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw},            lt,  true,   true,   true,   ">=3",    "5",      "{>=3}",        "{[3,4],>=6}",  "{[3,4],>=6}",    },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,"<"},   gt,  false,  false,  false,  "*",      "{}",     "{<=0,>=3}",    "{>=3}",        "{<=0,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,"<"},   gt,  false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,"<"},   gt,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,"<"},   gt,  false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,"<"},   gt,  false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,"<="},  gt,  false,  false,  false,  "*",      "{}",     "{<=1,>=3}",    "{>=3}",        "{<=1,>=3}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,"<="},  gt,  false,  false,  true,   "*",      "{}",     "{*}",          "{>=3}",        "{*}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,"<="},  gt,  false,  true,   true,   "*",      "3",      "{*}",          "{>=4}",        "{<=2,>=4}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,"<="},  gt,  false,  true,   true,   "*",      "[3,4]",  "{*}",          "{>=5}",        "{<=2,>=5}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,"<="},  gt,  false,  true,   true,   "*",      "[3,5]",  "{*}",          "{>=6}",        "{<=2,>=6}",      },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,">"},   gt,  false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,">"},   eq,  true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,">"},   lt,  true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,">"},   lt,  true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,">"},   lt,  true,   true,   true,   ">=3",    ">=6",    "{>=3}",        "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {1_sw,1_sw,">="},  gt,  false,  true,   true,   ">=1",    ">=3",    "{>=1}",        "{}",           "{[1,2]}",        },
        { __LINE__,  {3_sw,3_sw,">="},  {2_sw,2_sw,">="},  gt,  false,  true,   true,   ">=2",    ">=3",    "{>=2}",        "{}",           "{2}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {3_sw,3_sw,">="},  eq,  true,   true,   true,   ">=3",    ">=3",    "{>=3}",        "{}",           "{}",             },
        { __LINE__,  {3_sw,3_sw,">="},  {4_sw,4_sw,">="},  lt,  true,   true,   true,   ">=3",    ">=4",    "{>=3}",        "{3}",          "{3}",            },
        { __LINE__,  {3_sw,3_sw,">="},  {5_sw,5_sw,">="},  lt,  true,   true,   true,   ">=3",    ">=5",    "{>=3}",        "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {},                gt,  false,  false,  false,  "[3,6]",  "{}",     "{[3,6]}",      "{[3,6]}",      "{[3,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {0_sw,0_sw,"*"},   gt,  false,  true,   true,   "*",      "[3,6]",  "{*}",          "{}",           "{<=2,>=7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw},            gt,  false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw},            gt,  false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw},            gt,  true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw},            lt,  true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw},            lt,  true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw},            lt,  true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw},            lt,  false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw},            lt,  false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"<"},   gt,  false,  false,  false,  "<=6",    "{}",     "{<=0,[3,6]}",  "{[3,6]}",      "{<=0,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"<"},   gt,  false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"<"},   gt,  false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"<"},   gt,  false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"<"},   gt,  false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"<"},   gt,  false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"<"},   gt,  false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"<"},   gt,  false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"<="},  gt,  false,  false,  false,  "<=6",    "{}",     "{<=1,[3,6]}",  "{[3,6]}",      "{<=1,[3,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"<="},  gt,  false,  false,  true,   "<=6",    "{}",     "{<=6}",        "{[3,6]}",      "{<=6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"<="},  gt,  false,  true,   true,   "<=6",    "3",      "{<=6}",        "{[4,6]}",      "{<=2,[4,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"<="},  gt,  false,  true,   true,   "<=6",    "[3,4]",  "{<=6}",        "{[5,6]}",      "{<=2,[5,6]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"<="},  gt,  false,  true,   true,   "<=6",    "[3,5]",  "{<=6}",        "{6}",          "{<=2,6}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"<="},  gt,  false,  true,   true,   "<=6",    "[3,6]",  "{<=6}",        "{}",           "{<=2}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"<="},  gt,  false,  true,   true,   "<=7",    "[3,6]",  "{<=7}",        "{}",           "{<=2,7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"<="},  gt,  false,  true,   true,   "<=8",    "[3,6]",  "{<=8}",        "{}",           "{<=2,[7,8]}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,">"},   gt,  false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,">"},   lt,  false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,">"},   lt,  false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,">"},   lt,  false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,">"},   lt,  false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,">"},   lt,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,">"},   lt,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,">"},   lt,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=9}",  "{[3,6]}",      "{[3,6],>=9}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,">="},  gt,  false,  true,   true,   ">=1",    "[3,6]",  "{>=1}",        "{}",           "{[1,2],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,">="},  gt,  false,  true,   true,   ">=2",    "[3,6]",  "{>=2}",        "{}",           "{2,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,">="},  lt,  false,  true,   true,   ">=3",    "[3,6]",  "{>=3}",        "{}",           "{>=7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,">="},  lt,  false,  true,   true,   ">=3",    "[4,6]",  "{>=3}",        "{3}",          "{3,>=7}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,">="},  lt,  false,  true,   true,   ">=3",    "[5,6]",  "{>=3}",        "{[3,4]}",      "{[3,4],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,">="},  lt,  false,  true,   true,   ">=3",    "6",      "{>=3}",        "{[3,5]}",      "{[3,5],>=7}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,">="},  lt,  false,  false,  true,   ">=3",    "{}",     "{>=3}",        "{[3,6]}",      "{>=3}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,">="},  lt,  false,  false,  false,  ">=3",    "{}",     "{[3,6],>=8}",  "{[3,6]}",      "{[3,6],>=8}",    },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,1_sw,"[]"},  gt,  false,  false,  false,  "[1,6]",  "{}",     "{1,[3,6]}",    "{[3,6]}",      "{1,[3,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,2_sw,"[]"},  gt,  false,  false,  true,   "[1,6]",  "{}",     "{[1,6]}",      "{[3,6]}",      "{[1,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,3_sw,"[]"},  gt,  false,  true,   true,   "[1,6]",  "3",      "{[1,6]}",      "{[4,6]}",      "{[1,2],[4,6]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,4_sw,"[]"},  gt,  false,  true,   true,   "[1,6]",  "[3,4]",  "{[1,6]}",      "{[5,6]}",      "{[1,2],[5,6]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,5_sw,"[]"},  gt,  false,  true,   true,   "[1,6]",  "[3,5]",  "{[1,6]}",      "{6}",          "{[1,2],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,6_sw,"[]"},  gt,  false,  true,   true,   "[1,6]",  "[3,6]",  "{[1,6]}",      "{}",           "{[1,2]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,7_sw,"[]"},  gt,  false,  true,   true,   "[1,7]",  "[3,6]",  "{[1,7]}",      "{}",           "{[1,2],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {1_sw,8_sw,"[]"},  gt,  false,  true,   true,   "[1,8]",  "[3,6]",  "{[1,8]}",      "{}",           "{[1,2],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,2_sw,"[]"},  gt,  false,  false,  true,   "[2,6]",  "{}",     "{[2,6]}",      "{[3,6]}",      "{[2,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,3_sw,"[]"},  gt,  false,  true,   true,   "[2,6]",  "3",      "{[2,6]}",      "{[4,6]}",      "{2,[4,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,4_sw,"[]"},  gt,  false,  true,   true,   "[2,6]",  "[3,4]",  "{[2,6]}",      "{[5,6]}",      "{2,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,5_sw,"[]"},  gt,  false,  true,   true,   "[2,6]",  "[3,5]",  "{[2,6]}",      "{6}",          "{2,6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,6_sw,"[]"},  gt,  false,  true,   true,   "[2,6]",  "[3,6]",  "{[2,6]}",      "{}",           "{2}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,7_sw,"[]"},  gt,  false,  true,   true,   "[2,7]",  "[3,6]",  "{[2,7]}",      "{}",           "{2,7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {2_sw,8_sw,"[]"},  gt,  false,  true,   true,   "[2,8]",  "[3,6]",  "{[2,8]}",      "{}",           "{2,[7,8]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,3_sw,"[]"},  gt,  true,   true,   true,   "[3,6]",  "3",      "{[3,6]}",      "{[4,6]}",      "{[4,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,4_sw,"[]"},  gt,  true,   true,   true,   "[3,6]",  "[3,4]",  "{[3,6]}",      "{[5,6]}",      "{[5,6]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,5_sw,"[]"},  gt,  true,   true,   true,   "[3,6]",  "[3,5]",  "{[3,6]}",      "{6}",          "{6}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,6_sw,"[]"},  eq,  true,   true,   true,   "[3,6]",  "[3,6]",  "{[3,6]}",      "{}",           "{}",             },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,7_sw,"[]"},  lt,  false,  true,   true,   "[3,7]",  "[3,6]",  "{[3,7]}",      "{}",           "{7}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {3_sw,8_sw,"[]"},  lt,  false,  true,   true,   "[3,8]",  "[3,6]",  "{[3,8]}",      "{}",           "{[7,8]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,4_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "4",      "{[3,6]}",      "{3,[5,6]}",    "{3,[5,6]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,5_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "[4,5]",  "{[3,6]}",      "{3,6}",        "{3,6}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,6_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "[4,6]",  "{[3,6]}",      "{3}",          "{3}",            },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,7_sw,"[]"},  lt,  false,  true,   true,   "[3,7]",  "[4,6]",  "{[3,7]}",      "{3}",          "{3,7}",          },
        { __LINE__,  {3_sw,6_sw,"[]"},  {4_sw,8_sw,"[]"},  lt,  false,  true,   true,   "[3,8]",  "[4,6]",  "{[3,8]}",      "{3}",          "{3,[7,8]}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,5_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "5",      "{[3,6]}",      "{[3,4],6}",    "{[3,4],6}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,6_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "[5,6]",  "{[3,6]}",      "{[3,4]}",      "{[3,4]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,7_sw,"[]"},  lt,  false,  true,   true,   "[3,7]",  "[5,6]",  "{[3,7]}",      "{[3,4]}",      "{[3,4],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {5_sw,8_sw,"[]"},  lt,  false,  true,   true,   "[3,8]",  "[5,6]",  "{[3,8]}",      "{[3,4]}",      "{[3,4],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,6_sw,"[]"},  lt,  true,   true,   true,   "[3,6]",  "6",      "{[3,6]}",      "{[3,5]}",      "{[3,5]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,7_sw,"[]"},  lt,  false,  true,   true,   "[3,7]",  "6",      "{[3,7]}",      "{[3,5]}",      "{[3,5],7}",      },
        { __LINE__,  {3_sw,6_sw,"[]"},  {6_sw,8_sw,"[]"},  lt,  false,  true,   true,   "[3,8]",  "6",      "{[3,8]}",      "{[3,5]}",      "{[3,5],[7,8]}",  },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,7_sw,"[]"},  lt,  false,  false,  true,   "[3,7]",  "{}",     "{[3,7]}",      "{[3,6]}",      "{[3,7]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {7_sw,8_sw,"[]"},  lt,  false,  false,  true,   "[3,8]",  "{}",     "{[3,8]}",      "{[3,6]}",      "{[3,8]}",        },
        { __LINE__,  {3_sw,6_sw,"[]"},  {8_sw,8_sw,"[]"},  lt,  false,  false,  false,  "[3,8]",  "{}",     "{[3,6],8}",    "{[3,6]}",      "{[3,6],8}",      },

    };

    SO cmp = eq;
    bool flag = false;
    std::string str;

    for (auto& t: tests) {

        int errors = 0;

        TRY(cmp = t.lhs <=> t.rhs);                              TEST(cmp == t.compare);                       errors += int(cmp != t.compare);
        TRY(flag = t.lhs.includes(t.rhs));                       TEST_EQUAL(flag, t.includes);                 errors += int(flag != t.includes);
        TRY(flag = t.lhs.overlaps(t.rhs));                       TEST_EQUAL(flag, t.overlaps);                 errors += int(flag != t.overlaps);
        TRY(flag = t.lhs.touches(t.rhs));                        TEST_EQUAL(flag, t.touches);                  errors += int(flag != t.touches);
        TRY(str = t.lhs.envelope(t.rhs).str());                  TEST_EQUAL(str, t.envelope);                  errors += int(str != t.envelope);
        TRY(str = t.lhs.set_intersection(t.rhs).str());          TEST_EQUAL(str, t.set_intersection);          errors += int(str != t.set_intersection);
        TRY(str = t.lhs.set_union(t.rhs).str());                 TEST_EQUAL(str, t.set_union);                 errors += int(str != t.set_union);
        TRY(str = t.lhs.set_difference(t.rhs).str());            TEST_EQUAL(str, t.set_difference);            errors += int(str != t.set_difference);
        TRY(str = t.lhs.set_symmetric_difference(t.rhs).str());  TEST_EQUAL(str, t.set_symmetric_difference);  errors += int(str != t.set_symmetric_difference);

        if (errors)
            std::cout << "... [" << t.line << "] " << t.lhs << " " << t.rhs << "\n";

    }

}

void test_crow_interval_stepwise_interval_hashing() {

    std::unordered_set<Itv> set;

    TRY(set.insert(Itv(1_sw)));
    TRY(set.insert(Itv(2_sw, 3_sw)));
    TRY(set.insert(Itv(4_sw, 4_sw, ">")));
    TEST_EQUAL(set.size(), 3u);

}
