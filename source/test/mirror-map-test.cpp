#include "crow/mirror-map.hpp"
#include "crow/unit-test.hpp"
#include <iterator>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::UnitTest;

using IntStrMap = MirrorMap<int, std::string>;
using StrStrMap = MirrorMap<std::string, std::string>;

void test_crow_mirror_map_construct() {

    IntStrMap map;
    std::vector<std::pair<int, std::string>> vec;

    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);
    TEST(map.left_comp()(1, 2));
    TEST(! map.left_comp()(2, 1));
    TEST(map.right_comp()("hello", "world"));
    TEST(! map.right_comp()("world", "hello"));
    TEST_EQUAL(format_map(map.left()), "{}");
    TEST_EQUAL(format_map(map.right()), "{}");

    TRY((map = {{1,"H"},{2,"He"},{3,"Li"},{4,"Be"},{5,"B"},{6,"C"},{7,"N"},{8,"O"},{9,"F"},{10,"Ne"}}));
    TEST(! map.empty());
    TEST_EQUAL(map.size(), 10u);
    TEST_EQUAL(format_map(map.left()), "{1:H,2:He,3:Li,4:Be,5:B,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{5:B,4:Be,6:C,9:F,1:H,2:He,3:Li,7:N,10:Ne,8:O}");

    TRY(map.clear());
    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);

    TRY((vec = {{10,"Ne"},{9,"F"},{8,"O"},{7,"N"},{6,"C"},{5,"B"},{4,"Be"},{3,"Li"},{2,"He"},{1,"H"}}));
    TRY(map = IntStrMap(vec.begin(), vec.end()));
    TEST(! map.empty());
    TEST_EQUAL(map.size(), 10u);
    TEST_EQUAL(format_map(map.left()), "{1:H,2:He,3:Li,4:Be,5:B,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{5:B,4:Be,6:C,9:F,1:H,2:He,3:Li,7:N,10:Ne,8:O}");

}

void test_crow_mirror_map_iterators() {

    IntStrMap map;
    IntStrMap::left_iterator li;
    IntStrMap::right_iterator ri;

    TRY(li = map.begin_left());
    TEST(li == map.end_left());
    TRY(ri = map.mirror(li));
    TEST(ri == map.begin_right());
    TEST(ri == map.end_right());
    TRY(li = map.mirror(ri));
    TEST(li == map.begin_left());
    TEST(li == map.end_left());

    TRY((map = {{1,"H"},{2,"He"},{3,"Li"},{4,"Be"},{5,"B"},{6,"C"},{7,"N"},{8,"O"},{9,"F"},{10,"Ne"}}));
    TEST_EQUAL(map.size(), 10u);

    TRY(li = map.begin_left());  TEST(li != map.end_left());   TEST_EQUAL(li->first, 1);  TEST_EQUAL(li->second, "H");
    TRY(ri = map.mirror(li));    TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 1);  TEST_EQUAL(ri->second, "H");
    TRY(++ri);                   TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 2);  TEST_EQUAL(ri->second, "He");
    TRY(li = map.mirror(ri));    TEST(li != map.end_left());   TEST_EQUAL(li->first, 2);  TEST_EQUAL(li->second, "He");
    TRY(++li);                   TEST(li != map.end_left());   TEST_EQUAL(li->first, 3);  TEST_EQUAL(li->second, "Li");
    TRY(ri = map.mirror(li));    TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 3);  TEST_EQUAL(ri->second, "Li");
    TRY(++ri);                   TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 7);  TEST_EQUAL(ri->second, "N");
    TRY(li = map.mirror(ri));    TEST(li != map.end_left());   TEST_EQUAL(li->first, 7);  TEST_EQUAL(li->second, "N");
    TRY(++li);                   TEST(li != map.end_left());   TEST_EQUAL(li->first, 8);  TEST_EQUAL(li->second, "O");
    TRY(ri = map.mirror(li));    TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 8);  TEST_EQUAL(ri->second, "O");
    TRY(++ri);                   TEST(ri == map.end_right());
    TRY(li = map.mirror(ri));    TEST(li == map.end_left());

}

void test_crow_mirror_map_insert() {

    IntStrMap map;
    IntStrMap::insert_result ir;

    TRY(ir = map.insert({1, "H"}));
    TEST(ir.inserted);
    TEST(ir.left != map.end_left());    TEST_EQUAL(ir.left->first, 1);   TEST_EQUAL(ir.left->second, "H");
    TEST(ir.right != map.end_right());  TEST_EQUAL(ir.right->first, 1);  TEST_EQUAL(ir.right->second, "H");
    TEST_EQUAL(map.size(), 1u);
    TEST_EQUAL(format_map(map.left()), "{1:H}");
    TEST_EQUAL(format_map(map.right()), "{1:H}");

    TRY(ir = map.insert({2, "He"}));
    TEST(ir.inserted);
    TEST(ir.left != map.end_left());    TEST_EQUAL(ir.left->first, 2);   TEST_EQUAL(ir.left->second, "He");
    TEST(ir.right != map.end_right());  TEST_EQUAL(ir.right->first, 2);  TEST_EQUAL(ir.right->second, "He");
    TEST_EQUAL(map.size(), 2u);
    TEST_EQUAL(format_map(map.left()), "{1:H,2:He}");
    TEST_EQUAL(format_map(map.right()), "{1:H,2:He}");

    TRY(map.insert({3, "Li"}));
    TRY(map.insert({4, "Be"}));
    TRY(ir = map.insert({5, "B"}));
    TEST(ir.inserted);
    TEST(ir.left != map.end_left());    TEST_EQUAL(ir.left->first, 5);   TEST_EQUAL(ir.left->second, "B");
    TEST(ir.right != map.end_right());  TEST_EQUAL(ir.right->first, 5);  TEST_EQUAL(ir.right->second, "B");
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map.left()), "{1:H,2:He,3:Li,4:Be,5:B}");
    TEST_EQUAL(format_map(map.right()), "{5:B,4:Be,1:H,2:He,3:Li}");

    TRY(map.insert({10, "Ne"}));
    TRY(map.insert({9, "F"}));
    TRY(map.insert({8, "O"}));
    TRY(map.insert({7, "N"}));
    TRY(ir = map.insert({6, "C"}));
    TEST(ir.inserted);
    TEST(ir.left != map.end_left());    TEST_EQUAL(ir.left->first, 6);   TEST_EQUAL(ir.left->second, "C");
    TEST(ir.right != map.end_right());  TEST_EQUAL(ir.right->first, 6);  TEST_EQUAL(ir.right->second, "C");
    TEST_EQUAL(map.size(), 10u);
    TEST_EQUAL(format_map(map.left()), "{1:H,2:He,3:Li,4:Be,5:B,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{5:B,4:Be,6:C,9:F,1:H,2:He,3:Li,7:N,10:Ne,8:O}");

}

void test_crow_mirror_map_erase() {

    IntStrMap map;
    size_t n = 0;

    TRY((map = {{1,"H"},{2,"He"},{3,"Li"},{4,"Be"},{5,"B"},{6,"C"},{7,"N"},{8,"O"},{9,"F"},{10,"Ne"}}));
    TEST_EQUAL(map.size(), 10u);

    TRY(map.erase(map.begin_left()));
    TEST_EQUAL(map.size(), 9u);
    TEST_EQUAL(format_map(map.left()), "{2:He,3:Li,4:Be,5:B,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{5:B,4:Be,6:C,9:F,2:He,3:Li,7:N,10:Ne,8:O}");

    TRY(map.erase(map.begin_right()));
    TEST_EQUAL(map.size(), 8u);
    TEST_EQUAL(format_map(map.left()), "{2:He,3:Li,4:Be,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{4:Be,6:C,9:F,2:He,3:Li,7:N,10:Ne,8:O}");

    TEST(map.erase({2, "He"}));
    TEST_EQUAL(map.size(), 7u);
    TEST_EQUAL(format_map(map.left()), "{3:Li,4:Be,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{4:Be,6:C,9:F,3:Li,7:N,10:Ne,8:O}");

    TEST(! map.erase({3, "Be"}));
    TEST_EQUAL(map.size(), 7u);
    TEST(! map.erase({3, "Xy"}));
    TEST_EQUAL(map.size(), 7u);
    TEST(! map.erase({11, "Be"}));
    TEST_EQUAL(map.size(), 7u);
    TEST(! map.erase({11, "Na"}));
    TEST_EQUAL(map.size(), 7u);

    TRY(n = map.erase_left(4));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(format_map(map.left()), "{3:Li,6:C,7:N,8:O,9:F,10:Ne}");
    TEST_EQUAL(format_map(map.right()), "{6:C,9:F,3:Li,7:N,10:Ne,8:O}");

    TRY(n = map.erase_right("Ne"));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map.left()), "{3:Li,6:C,7:N,8:O,9:F}");
    TEST_EQUAL(format_map(map.right()), "{6:C,9:F,3:Li,7:N,8:O}");

    TRY(n = map.erase_left(11));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map.size(), 5u);
    TRY(n = map.erase_right("Na"));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map.size(), 5u);

}

void test_crow_mirror_map_search() {

    IntStrMap map;
    IntStrMap::left_iterator li;
    IntStrMap::left_range lr;
    IntStrMap::right_iterator ri;
    IntStrMap::right_range rr;
    size_t n = 0;

    TRY(li = map.find_left(1));         TEST(li == map.end_left());
    TRY(ri = map.find_right("H"));      TEST(ri == map.end_right());
    TRY(li = map.find_left({1,"H"}));   TEST(li == map.end_left());
    TRY(ri = map.find_right({1,"H"}));  TEST(ri == map.end_right());

    TRY((map = {{1,"H"},{2,"He"},{3,"Li"},{4,"Be"},{5,"B"},{6,"C"},{7,"N"},{8,"O"},{9,"F"},{10,"Ne"}}));
    TEST_EQUAL(map.size(), 10u);

    TRY(li = map.find_left(1));           TEST(li != map.end_left());   TEST_EQUAL(li->first, 1);   TEST_EQUAL(li->second, "H");
    TRY(ri = map.find_right("H"));        TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 1);   TEST_EQUAL(ri->second, "H");
    TRY(li = map.find_left(10));          TEST(li != map.end_left());   TEST_EQUAL(li->first, 10);  TEST_EQUAL(li->second, "Ne");
    TRY(ri = map.find_right("Ne"));       TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 10);  TEST_EQUAL(ri->second, "Ne");
    TRY(li = map.find_left({1,"H"}));     TEST(li != map.end_left());   TEST_EQUAL(li->first, 1);   TEST_EQUAL(li->second, "H");
    TRY(ri = map.find_right({1,"H"}));    TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 1);   TEST_EQUAL(ri->second, "H");
    TRY(li = map.find_left({10,"Ne"}));   TEST(li != map.end_left());   TEST_EQUAL(li->first, 10);  TEST_EQUAL(li->second, "Ne");
    TRY(ri = map.find_right({10,"Ne"}));  TEST(ri != map.end_right());  TEST_EQUAL(ri->first, 10);  TEST_EQUAL(ri->second, "Ne");

    TRY(li = map.find_left(11));          TEST(li == map.end_left());
    TRY(ri = map.find_right("Na"));       TEST(ri == map.end_right());
    TRY(li = map.find_left({1,"He"}));    TEST(li == map.end_left());
    TRY(ri = map.find_right({1,"He"}));   TEST(ri == map.end_right());
    TRY(li = map.find_left({2,"H"}));     TEST(li == map.end_left());
    TRY(ri = map.find_right({2,"H"}));    TEST(ri == map.end_right());
    TRY(li = map.find_left({11,"Na"}));   TEST(li == map.end_left());
    TRY(ri = map.find_right({11,"Na"}));  TEST(ri == map.end_right());

    TRY(lr = map.equal_left(5));
    TEST_EQUAL(std::distance(lr.begin(), lr.end()), 1);
    TEST(lr.first != map.end_left());   TEST_EQUAL(lr.first->first, 5);   TEST_EQUAL(lr.first->second, "B");
    TEST(lr.second != map.end_left());  TEST_EQUAL(lr.second->first, 6);  TEST_EQUAL(lr.second->second, "C");

    TRY(rr = map.equal_right("B"));
    TEST_EQUAL(std::distance(rr.begin(), rr.end()), 1);
    TEST(rr.first != map.end_right());   TEST_EQUAL(rr.first->first, 5);   TEST_EQUAL(rr.first->second, "B");
    TEST(rr.second != map.end_right());  TEST_EQUAL(rr.second->first, 4);  TEST_EQUAL(rr.second->second, "Be");

    TRY(lr = map.equal_left(-1));
    TEST(lr.empty());
    TEST(lr.first == map.begin_left());
    TEST(lr.second == map.begin_left());

    TRY(lr = map.equal_left(11));
    TEST(lr.empty());
    TEST(lr.first == map.end_left());
    TEST(lr.second == map.end_left());

    TRY(rr = map.equal_right("Ar"));
    TEST(rr.empty());
    TEST(rr.first == map.begin_right());
    TEST(rr.second == map.begin_right());

    TRY(rr = map.equal_right("Zr"));
    TEST(rr.empty());
    TEST(rr.first == map.end_right());
    TEST(rr.second == map.end_right());

    TRY(rr = map.equal_right("Na"));
    TEST(rr.empty());
    TEST(rr.first != map.end_right());   TEST_EQUAL(rr.first->first, 10);   TEST_EQUAL(rr.first->second, "Ne");
    TEST(rr.second != map.end_right());  TEST_EQUAL(rr.second->first, 10);  TEST_EQUAL(rr.second->second, "Ne");

    TRY(n = map.count_left(1));   TEST_EQUAL(n, 1u);
    TRY(n = map.count_left(5));   TEST_EQUAL(n, 1u);
    TRY(n = map.count_left(10));  TEST_EQUAL(n, 1u);
    TRY(n = map.count_left(11));  TEST_EQUAL(n, 0u);

    TRY(n = map.count_right("H"));   TEST_EQUAL(n, 1u);
    TRY(n = map.count_right("B"));   TEST_EQUAL(n, 1u);
    TRY(n = map.count_right("Ne"));  TEST_EQUAL(n, 1u);
    TRY(n = map.count_right("Na"));  TEST_EQUAL(n, 0u);

}


void test_crow_mirror_map_duplicates() {

    StrStrMap map;
    StrStrMap::left_iterator li;
    StrStrMap::left_range lr;
    StrStrMap::right_iterator ri;
    StrStrMap::right_range rr;
    size_t n = 0;

    TEST(map.insert({"Lockheed", "Falcon"}).inserted);
    TEST_EQUAL(map.size(), 1u);
    TEST_EQUAL(format_map(map.left()), "{Lockheed:Falcon}");
    TEST_EQUAL(format_map(map.right()), "{Lockheed:Falcon}");

    TEST(map.insert({"Dassault", "Falcon"}).inserted);
    TEST_EQUAL(map.size(), 2u);
    TEST_EQUAL(format_map(map.left()), "{Dassault:Falcon,Lockheed:Falcon}");
    TEST_EQUAL(format_map(map.right()), "{Dassault:Falcon,Lockheed:Falcon}");

    TEST(map.insert({"Boeing", "Hornet"}).inserted);
    TEST_EQUAL(map.size(), 3u);
    TEST_EQUAL(format_map(map.left()), "{Boeing:Hornet,Dassault:Falcon,Lockheed:Falcon}");
    TEST_EQUAL(format_map(map.right()), "{Dassault:Falcon,Lockheed:Falcon,Boeing:Hornet}");

    TEST(map.insert({"Boeing", "Eagle"}).inserted);
    TEST(map.insert({"Dassault", "Mirage"}).inserted);
    TEST(map.insert({"Dassault", "Rafale"}).inserted);
    TEST(map.insert({"Lockheed", "Lightning"}).inserted);
    TEST(map.insert({"Lockheed", "Raptor"}).inserted);
    TEST_EQUAL(map.size(), 8u);
    TEST_EQUAL(format_map(map.left()),
        "{Boeing:Eagle,Boeing:Hornet,Dassault:Falcon,Dassault:Mirage,Dassault:Rafale,Lockheed:Falcon,Lockheed:Lightning,Lockheed:Raptor}");
    TEST_EQUAL(format_map(map.right()),
        "{Boeing:Eagle,Dassault:Falcon,Lockheed:Falcon,Boeing:Hornet,Lockheed:Lightning,Dassault:Mirage,Dassault:Rafale,Lockheed:Raptor}");

    TEST(! map.insert({"Lockheed", "Lightning"}).inserted);
    TEST_EQUAL(map.size(), 8u);

    TRY(li = map.find_left("Dassault"));
    TEST(li != map.end_left());
    TEST_EQUAL(li->first, "Dassault");
    TRY(li = map.find_left("Panavia"));
    TEST(li == map.end_left());
    TRY(ri = map.find_right("Lightning"));
    TEST(ri != map.end_right());
    TEST_EQUAL(ri->second, "Lightning");
    TRY(ri = map.find_right("Tornado"));
    TEST(ri == map.end_right());

    TRY(lr = map.equal_left("Dassault"));
    TEST_EQUAL(std::distance(lr.begin(), lr.end()), 3);
    TEST_EQUAL(format_map(lr), "{Dassault:Falcon,Dassault:Mirage,Dassault:Rafale}");
    TRY(lr = map.equal_left("Panavia"));
    TEST(lr.empty());
    TRY(rr = map.equal_right("Falcon"));
    TEST_EQUAL(std::distance(rr.begin(), rr.end()), 2);
    TEST_EQUAL(format_map(rr), "{Dassault:Falcon,Lockheed:Falcon}");
    TRY(rr = map.equal_right("Tornado"));
    TEST(rr.empty());

    TRY(n = map.erase_left("Boeing"));
    TEST_EQUAL(n, 2u);
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(format_map(map.left()), "{Dassault:Falcon,Dassault:Mirage,Dassault:Rafale,Lockheed:Falcon,Lockheed:Lightning,Lockheed:Raptor}");
    TEST_EQUAL(format_map(map.right()), "{Dassault:Falcon,Lockheed:Falcon,Lockheed:Lightning,Dassault:Mirage,Dassault:Rafale,Lockheed:Raptor}");
    TRY(n = map.erase_left("Panavia"));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map.size(), 6u);

    TRY(n = map.erase_right("Falcon"));
    TEST_EQUAL(n, 2u);
    TEST_EQUAL(map.size(), 4u);
    TEST_EQUAL(format_map(map.left()), "{Dassault:Mirage,Dassault:Rafale,Lockheed:Lightning,Lockheed:Raptor}");
    TEST_EQUAL(format_map(map.right()), "{Lockheed:Lightning,Dassault:Mirage,Dassault:Rafale,Lockheed:Raptor}");
    TRY(n = map.erase_left("Tornado"));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map.size(), 4u);

}
