#include "crow/stable-map.hpp"
#include "crow/unit-test.hpp"
#include <iterator>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

using Crow::UnitTest::format_map;

using SMM = StableMap<int, std::string, true>;

void test_crow_stable_map_multimap_insert() {

    SMM map;
    std::pair<SMM::iterator, bool> rc;

    TEST(map.empty());
    TEST_EQUAL(map.size(), 0u);
    TEST_EQUAL(format_map(map), "{}");

    TRY(rc = map.insert(99, "abc"));  TEST_EQUAL(rc.first->first, 99);  TEST_EQUAL(rc.first->second, "abc");  TEST(rc.second);

    TEST(! map.empty());
    TEST_EQUAL(map.size(), 1u);
    TEST_EQUAL(format_map(map), "{99:abc}");

    TRY(rc = map.insert(98, "def"));  TEST_EQUAL(rc.first->first, 98);  TEST_EQUAL(rc.first->second, "def");  TEST(rc.second);
    TRY(rc = map.insert(97, "ghi"));  TEST_EQUAL(rc.first->first, 97);  TEST_EQUAL(rc.first->second, "ghi");  TEST(rc.second);
    TRY(rc = map.insert(96, "jkl"));  TEST_EQUAL(rc.first->first, 96);  TEST_EQUAL(rc.first->second, "jkl");  TEST(rc.second);
    TRY(rc = map.insert(95, "mno"));  TEST_EQUAL(rc.first->first, 95);  TEST_EQUAL(rc.first->second, "mno");  TEST(rc.second);

    TEST(! map.empty());
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map), "{99:abc,98:def,97:ghi,96:jkl,95:mno}");

    TRY(rc = map.insert(95, "uvw"));  TEST_EQUAL(rc.first->first, 95);  TEST_EQUAL(rc.first->second, "uvw");  TEST(rc.second);
    TRY(rc = map.insert(99, "xyz"));  TEST_EQUAL(rc.first->first, 99);  TEST_EQUAL(rc.first->second, "xyz");  TEST(rc.second);

    TEST(! map.empty());
    TEST_EQUAL(map.size(), 7u);
    TEST_EQUAL(format_map(map), "{99:abc,98:def,97:ghi,96:jkl,95:mno,95:uvw,99:xyz}");

}

void test_crow_stable_map_multimap_copy() {

    SMM map1, map2, map3;

    TRY(map1.insert(1, "aaa"));
    TRY(map1.insert(2, "bbb"));
    TRY(map1.insert(3, "ccc"));
    TRY(map1.insert(2, "ddd"));
    TRY(map1.insert(1, "eee"));
    TEST_EQUAL(map1.size(), 5u);
    TEST_EQUAL(format_map(map1), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

    TRY(map2.insert(10, "xxx"));
    TRY(map2.insert(20, "yyy"));
    TRY(map2.insert(10, "zzz"));
    TEST_EQUAL(map2.size(), 3u);
    TEST_EQUAL(format_map(map2), "{10:xxx,20:yyy,10:zzz}");

    TRY(map3 = map1);
    TEST_EQUAL(map3.size(), 5u);
    TEST_EQUAL(format_map(map3), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

    TRY(map3 = std::move(map2));
    TEST(map2.empty());
    TEST_EQUAL(map3.size(), 3u);
    TEST_EQUAL(format_map(map3), "{10:xxx,20:yyy,10:zzz}");

    TRY(map1.swap(map3));
    TEST_EQUAL(map1.size(), 3u);
    TEST_EQUAL(format_map(map1), "{10:xxx,20:yyy,10:zzz}");
    TEST_EQUAL(map3.size(), 5u);
    TEST_EQUAL(format_map(map3), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

}

void test_crow_stable_map_multimap_erase() {

    SMM map1, map2;
    SMM::iterator it;
    size_t n = 0;

    TRY(map1.insert(1, "aaa"));
    TRY(map1.insert(2, "bbb"));
    TRY(map1.insert(3, "ccc"));
    TRY(map1.insert(2, "ddd"));
    TRY(map1.insert(1, "eee"));
    TEST_EQUAL(map1.size(), 5u);
    TEST_EQUAL(format_map(map1), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

    TRY(map2 = map1);
    TEST_EQUAL(map2.size(), 5u);
    TEST_EQUAL(format_map(map2), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

    TRY(it = map1.begin());
    TRY(std::advance(it, 2));
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 4u);
    TEST_EQUAL(format_map(map1), "{1:aaa,2:bbb,2:ddd,1:eee}");

    TRY(it = map1.begin());
    TRY(std::advance(it, 2));
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 3u);
    TEST_EQUAL(format_map(map1), "{1:aaa,2:bbb,1:eee}");

    TRY(it = map1.begin());
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 2u);
    TEST_EQUAL(format_map(map1), "{2:bbb,1:eee}");

    TRY(it = map1.begin());
    TRY(++it);
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 1u);
    TEST_EQUAL(format_map(map1), "{2:bbb}");

    TRY(it = map1.begin());
    TRY(map1.erase(it));
    TEST(map1.empty());

    TRY(n = map2.erase(99));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 5u);
    TEST_EQUAL(format_map(map2), "{1:aaa,2:bbb,3:ccc,2:ddd,1:eee}");

    TRY(n = map2.erase(2));
    TEST_EQUAL(n, 2u);
    TEST_EQUAL(map2.size(), 3u);
    TEST_EQUAL(format_map(map2), "{1:aaa,3:ccc,1:eee}");

    TRY(n = map2.erase(2));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 3u);
    TEST_EQUAL(format_map(map2), "{1:aaa,3:ccc,1:eee}");

    TRY(n = map2.erase(3));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map2.size(), 2u);
    TEST_EQUAL(format_map(map2), "{1:aaa,1:eee}");

    TRY(n = map2.erase(3));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 2u);
    TEST_EQUAL(format_map(map2), "{1:aaa,1:eee}");

    TRY(n = map2.erase(1));
    TEST_EQUAL(n, 2u);
    TEST(map2.empty());

    TRY(n = map2.erase(1));
    TEST_EQUAL(n, 0u);
    TEST(map2.empty());

}

void test_crow_stable_map_multimap_lookup() {

    SMM map;
    const SMM& cmap = map;
    SMM::iterator i;
    SMM::const_iterator ci;
    std::vector<SMM::iterator> iv;
    std::vector<SMM::const_iterator> civ;
    std::string s;
    size_t n = 0;

    TRY(map.insert(1, "aaa"));
    TRY(map.insert(2, "bbb"));
    TRY(map.insert(3, "ccc"));
    TRY(map.insert(4, "ddd"));
    TRY(map.insert(5, "eee"));
    TRY(map.insert(2, "fff"));
    TRY(map.insert(3, "ggg"));
    TRY(map.insert(4, "hhh"));
    TRY(map.insert(3, "iii"));
    TEST_EQUAL(map.size(), 9u);
    TEST_EQUAL(format_map(map), "{1:aaa,2:bbb,3:ccc,4:ddd,5:eee,2:fff,3:ggg,4:hhh,3:iii}");

    TEST(! map.contains(0));
    TEST(map.contains(1));
    TEST(map.contains(2));
    TEST(map.contains(3));
    TEST(map.contains(4));
    TEST(map.contains(5));
    TEST(! map.contains(6));

    TRY(n = map.count(0));  TEST_EQUAL(n, 0u);
    TRY(n = map.count(1));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(2));  TEST_EQUAL(n, 2u);
    TRY(n = map.count(3));  TEST_EQUAL(n, 3u);
    TRY(n = map.count(4));  TEST_EQUAL(n, 2u);
    TRY(n = map.count(5));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(6));  TEST_EQUAL(n, 0u);

    TRY(i = map.find(1));    TEST(i != map.end());    TEST_EQUAL(i->first, 1);   TEST_EQUAL(i->second, "aaa");
    TRY(i = map.find(2));    TEST(i != map.end());    TEST_EQUAL(i->first, 2);   TEST_EQUAL(i->second, "bbb");
    TRY(i = map.find(3));    TEST(i != map.end());    TEST_EQUAL(i->first, 3);   TEST_EQUAL(i->second, "ccc");
    TRY(i = map.find(4));    TEST(i != map.end());    TEST_EQUAL(i->first, 4);   TEST_EQUAL(i->second, "ddd");
    TRY(i = map.find(5));    TEST(i != map.end());    TEST_EQUAL(i->first, 5);   TEST_EQUAL(i->second, "eee");
    TRY(ci = cmap.find(1));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 1);  TEST_EQUAL(ci->second, "aaa");
    TRY(ci = cmap.find(2));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 2);  TEST_EQUAL(ci->second, "bbb");
    TRY(ci = cmap.find(3));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 3);  TEST_EQUAL(ci->second, "ccc");
    TRY(ci = cmap.find(4));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 4);  TEST_EQUAL(ci->second, "ddd");
    TRY(ci = cmap.find(5));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 5);  TEST_EQUAL(ci->second, "eee");

    TRY(i = map.find(0));    TEST(i == map.end());
    TRY(i = map.find(6));    TEST(i == map.end());
    TRY(ci = cmap.find(0));  TEST(ci == cmap.end());
    TRY(ci = cmap.find(6));  TEST(ci == cmap.end());

    TRY(i = map.find_last(1));    TEST(i != map.end());    TEST_EQUAL(i->first, 1);   TEST_EQUAL(i->second, "aaa");
    TRY(i = map.find_last(2));    TEST(i != map.end());    TEST_EQUAL(i->first, 2);   TEST_EQUAL(i->second, "fff");
    TRY(i = map.find_last(3));    TEST(i != map.end());    TEST_EQUAL(i->first, 3);   TEST_EQUAL(i->second, "iii");
    TRY(i = map.find_last(4));    TEST(i != map.end());    TEST_EQUAL(i->first, 4);   TEST_EQUAL(i->second, "hhh");
    TRY(i = map.find_last(5));    TEST(i != map.end());    TEST_EQUAL(i->first, 5);   TEST_EQUAL(i->second, "eee");
    TRY(ci = cmap.find_last(1));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 1);  TEST_EQUAL(ci->second, "aaa");
    TRY(ci = cmap.find_last(2));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 2);  TEST_EQUAL(ci->second, "fff");
    TRY(ci = cmap.find_last(3));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 3);  TEST_EQUAL(ci->second, "iii");
    TRY(ci = cmap.find_last(4));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 4);  TEST_EQUAL(ci->second, "hhh");
    TRY(ci = cmap.find_last(5));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 5);  TEST_EQUAL(ci->second, "eee");

    TRY(i = map.find_last(0));    TEST(i == map.end());
    TRY(i = map.find_last(6));    TEST(i == map.end());
    TRY(ci = cmap.find_last(0));  TEST(ci == cmap.end());
    TRY(ci = cmap.find_last(6));  TEST(ci == cmap.end());

    TRY(iv = map.find_all(1));    TEST_EQUAL(iv.size(), 1u);   TEST_EQUAL(iv[0]->first, 1);   TEST_EQUAL(iv[0]->second, "aaa");
    TRY(iv = map.find_all(2));    TEST_EQUAL(iv.size(), 2u);   TEST_EQUAL(iv[0]->first, 2);   TEST_EQUAL(iv[0]->second, "bbb");
    /**/                          /**/                         TEST_EQUAL(iv[1]->first, 2);   TEST_EQUAL(iv[1]->second, "fff");
    TRY(iv = map.find_all(3));    TEST_EQUAL(iv.size(), 3u);   TEST_EQUAL(iv[0]->first, 3);   TEST_EQUAL(iv[0]->second, "ccc");
    /**/                          /**/                         TEST_EQUAL(iv[1]->first, 3);   TEST_EQUAL(iv[1]->second, "ggg");
    /**/                          /**/                         TEST_EQUAL(iv[2]->first, 3);   TEST_EQUAL(iv[2]->second, "iii");
    TRY(iv = map.find_all(4));    TEST_EQUAL(iv.size(), 2u);   TEST_EQUAL(iv[0]->first, 4);   TEST_EQUAL(iv[0]->second, "ddd");
    /**/                          /**/                         TEST_EQUAL(iv[1]->first, 4);   TEST_EQUAL(iv[1]->second, "hhh");
    TRY(iv = map.find_all(5));    TEST_EQUAL(iv.size(), 1u);   TEST_EQUAL(iv[0]->first, 5);   TEST_EQUAL(iv[0]->second, "eee");
    TRY(civ = cmap.find_all(1));  TEST_EQUAL(civ.size(), 1u);  TEST_EQUAL(civ[0]->first, 1);  TEST_EQUAL(civ[0]->second, "aaa");
    TRY(civ = cmap.find_all(2));  TEST_EQUAL(civ.size(), 2u);  TEST_EQUAL(civ[0]->first, 2);  TEST_EQUAL(civ[0]->second, "bbb");
    /**/                          /**/                         TEST_EQUAL(civ[1]->first, 2);  TEST_EQUAL(civ[1]->second, "fff");
    TRY(civ = cmap.find_all(3));  TEST_EQUAL(civ.size(), 3u);  TEST_EQUAL(civ[0]->first, 3);  TEST_EQUAL(civ[0]->second, "ccc");
    /**/                          /**/                         TEST_EQUAL(civ[1]->first, 3);  TEST_EQUAL(civ[1]->second, "ggg");
    /**/                          /**/                         TEST_EQUAL(civ[2]->first, 3);  TEST_EQUAL(civ[2]->second, "iii");
    TRY(civ = cmap.find_all(4));  TEST_EQUAL(civ.size(), 2u);  TEST_EQUAL(civ[0]->first, 4);  TEST_EQUAL(civ[0]->second, "ddd");
    /**/                          /**/                         TEST_EQUAL(civ[1]->first, 4);  TEST_EQUAL(civ[1]->second, "hhh");
    TRY(civ = cmap.find_all(5));  TEST_EQUAL(civ.size(), 1u);  TEST_EQUAL(civ[0]->first, 5);  TEST_EQUAL(civ[0]->second, "eee");

    TRY(iv = map.find_all(0));    TEST(iv.empty());
    TRY(iv = map.find_all(6));    TEST(iv.empty());
    TRY(civ = cmap.find_all(0));  TEST(civ.empty());
    TRY(civ = cmap.find_all(6));  TEST(civ.empty());

}

void test_crow_stable_map_multimap_rehash() {

    SMM map;

    TRY(map.insert(5, "a"));  TEST_EQUAL(map.size(), 1u);  TEST_EQUAL(format_map(map), "{5:a}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 1u);  TEST_EQUAL(format_map(map), "{5:a}");
    TRY(map.insert(4, "b"));  TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{5:a,4:b}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{5:a,4:b}");
    TRY(map.insert(3, "c"));  TEST_EQUAL(map.size(), 3u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 3u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c}");
    TRY(map.insert(2, "d"));  TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d}");
    TRY(map.insert(1, "e"));  TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e}");
    TRY(map.insert(4, "f"));  TEST_EQUAL(map.size(), 6u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 6u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f}");
    TRY(map.insert(3, "g"));  TEST_EQUAL(map.size(), 7u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 7u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g}");
    TRY(map.insert(2, "h"));  TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g,2:h}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g,2:h}");
    TRY(map.insert(3, "i"));  TEST_EQUAL(map.size(), 9u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g,2:h,3:i}");
    TRY(map.rehash());        TEST_EQUAL(map.size(), 9u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,1:e,4:f,3:g,2:h,3:i}");

    TRY(map.erase(1));  TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,4:f,3:g,2:h,3:i}");
    TRY(map.rehash());  TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{5:a,4:b,3:c,2:d,4:f,3:g,2:h,3:i}");
    TRY(map.erase(3));  TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{5:a,4:b,2:d,4:f,2:h}");
    TRY(map.rehash());  TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{5:a,4:b,2:d,4:f,2:h}");
    TRY(map.erase(5));  TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{4:b,2:d,4:f,2:h}");
    TRY(map.rehash());  TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{4:b,2:d,4:f,2:h}");
    TRY(map.erase(2));  TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{4:b,4:f}");
    TRY(map.rehash());  TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{4:b,4:f}");
    TRY(map.erase(4));  TEST_EQUAL(map.size(), 0u);  TEST_EQUAL(format_map(map), "{}");
    TRY(map.rehash());  TEST_EQUAL(map.size(), 0u);  TEST_EQUAL(format_map(map), "{}");

}
