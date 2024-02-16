#include "crow/stable-map.hpp"
#include "crow/unit-test.hpp"
#include <iterator>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

using Crow::UnitTest::format_map;

using SM = StableMap<int, std::string>;

void test_crow_stable_map_unique_insert() {

    SM map;
    std::pair<SM::iterator, bool> rc;

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

    TRY(rc = map.insert(95, "uvw"));  TEST_EQUAL(rc.first->first, 95);  TEST_EQUAL(rc.first->second, "mno");  TEST(! rc.second);
    TRY(rc = map.insert(99, "xyz"));  TEST_EQUAL(rc.first->first, 99);  TEST_EQUAL(rc.first->second, "abc");  TEST(! rc.second);

    TEST(! map.empty());
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map), "{99:abc,98:def,97:ghi,96:jkl,95:mno}");

    TRY(rc = map.update(90, "rst"));  TEST_EQUAL(rc.first->first, 90);  TEST_EQUAL(rc.first->second, "rst");  TEST(rc.second);
    TRY(rc = map.update(95, "uvw"));  TEST_EQUAL(rc.first->first, 95);  TEST_EQUAL(rc.first->second, "uvw");  TEST(! rc.second);
    TRY(rc = map.update(99, "xyz"));  TEST_EQUAL(rc.first->first, 99);  TEST_EQUAL(rc.first->second, "xyz");  TEST(! rc.second);

    TEST(! map.empty());
    TEST_EQUAL(map.size(), 6u);
    TEST_EQUAL(format_map(map), "{99:xyz,98:def,97:ghi,96:jkl,95:uvw,90:rst}");

}

void test_crow_stable_map_unique_copy() {

    SM map1, map2, map3;

    TRY(map1.insert(1, "aaa"));
    TRY(map1.insert(2, "bbb"));
    TRY(map1.insert(3, "ccc"));
    TRY(map1.insert(4, "ddd"));
    TRY(map1.insert(5, "eee"));
    TEST_EQUAL(map1.size(), 5u);
    TEST_EQUAL(format_map(map1), "{1:aaa,2:bbb,3:ccc,4:ddd,5:eee}");

    TRY(map2.insert(10, "xxx"));
    TRY(map2.insert(20, "yyy"));
    TRY(map2.insert(30, "zzz"));
    TEST_EQUAL(map2.size(), 3u);
    TEST_EQUAL(format_map(map2), "{10:xxx,20:yyy,30:zzz}");

    TRY(map3 = map1);
    TEST_EQUAL(map3.size(), 5u);
    TEST_EQUAL(format_map(map3), "{1:aaa,2:bbb,3:ccc,4:ddd,5:eee}");

    TRY(map3 = std::move(map2));
    TEST(map2.empty());
    TEST_EQUAL(map3.size(), 3u);
    TEST_EQUAL(format_map(map3), "{10:xxx,20:yyy,30:zzz}");

    TRY(map1.swap(map3));
    TEST_EQUAL(map1.size(), 3u);
    TEST_EQUAL(format_map(map1), "{10:xxx,20:yyy,30:zzz}");
    TEST_EQUAL(map3.size(), 5u);
    TEST_EQUAL(format_map(map3), "{1:aaa,2:bbb,3:ccc,4:ddd,5:eee}");

}

void test_crow_stable_map_unique_erase() {

    SM map1, map2;
    SM::iterator it;
    size_t n = 0;

    TRY(map1.insert(91, "aaa"));
    TRY(map1.insert(82, "bbb"));
    TRY(map1.insert(73, "ccc"));
    TRY(map1.insert(64, "ddd"));
    TRY(map1.insert(55, "eee"));
    TEST_EQUAL(map1.size(), 5u);
    TEST_EQUAL(format_map(map1), "{91:aaa,82:bbb,73:ccc,64:ddd,55:eee}");

    TRY(map2 = map1);
    TEST_EQUAL(map2.size(), 5u);
    TEST_EQUAL(format_map(map2), "{91:aaa,82:bbb,73:ccc,64:ddd,55:eee}");

    TRY(it = map1.begin());
    TRY(std::advance(it, 2));
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 4u);
    TEST_EQUAL(format_map(map1), "{91:aaa,82:bbb,64:ddd,55:eee}");

    TRY(it = map1.begin());
    TRY(std::advance(it, 2));
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 3u);
    TEST_EQUAL(format_map(map1), "{91:aaa,82:bbb,55:eee}");

    TRY(it = map1.begin());
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 2u);
    TEST_EQUAL(format_map(map1), "{82:bbb,55:eee}");

    TRY(it = map1.begin());
    TRY(++it);
    TRY(map1.erase(it));
    TEST_EQUAL(map1.size(), 1u);
    TEST_EQUAL(format_map(map1), "{82:bbb}");

    TRY(it = map1.begin());
    TRY(map1.erase(it));
    TEST(map1.empty());

    TRY(n = map2.erase(99));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 5u);
    TEST_EQUAL(format_map(map2), "{91:aaa,82:bbb,73:ccc,64:ddd,55:eee}");

    TRY(n = map2.erase(64));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map2.size(), 4u);
    TEST_EQUAL(format_map(map2), "{91:aaa,82:bbb,73:ccc,55:eee}");

    TRY(n = map2.erase(64));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 4u);
    TEST_EQUAL(format_map(map2), "{91:aaa,82:bbb,73:ccc,55:eee}");

    TRY(n = map2.erase(91));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map2.size(), 3u);
    TEST_EQUAL(format_map(map2), "{82:bbb,73:ccc,55:eee}");

    TRY(n = map2.erase(55));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map2.size(), 2u);
    TEST_EQUAL(format_map(map2), "{82:bbb,73:ccc}");

    TRY(n = map2.erase(82));
    TEST_EQUAL(n, 1u);
    TEST_EQUAL(map2.size(), 1u);
    TEST_EQUAL(format_map(map2), "{73:ccc}");

    TRY(n = map2.erase(82));
    TEST_EQUAL(n, 0u);
    TEST_EQUAL(map2.size(), 1u);
    TEST_EQUAL(format_map(map2), "{73:ccc}");

    TRY(n = map2.erase(73));
    TEST_EQUAL(n, 1u);
    TEST(map2.empty());

    TRY(n = map2.erase(73));
    TEST_EQUAL(n, 0u);
    TEST(map2.empty());

}

void test_crow_stable_map_unique_lookup() {

    SM map;
    const SM& cmap = map;
    SM::iterator i;
    SM::const_iterator ci;
    std::vector<SM::iterator> iv;
    std::vector<SM::const_iterator> civ;
    std::string s;
    size_t n = 0;

    TRY(map.insert(10, "aaa"));
    TRY(map.insert(20, "bbb"));
    TRY(map.insert(30, "ccc"));
    TRY(map.insert(40, "ddd"));
    TRY(map.insert(50, "eee"));
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map), "{10:aaa,20:bbb,30:ccc,40:ddd,50:eee}");

    TEST(! map.contains(0));
    TEST(map.contains(10));
    TEST(map.contains(20));
    TEST(map.contains(30));
    TEST(map.contains(40));
    TEST(map.contains(50));
    TEST(! map.contains(60));

    TRY(n = map.count(0));   TEST_EQUAL(n, 0u);
    TRY(n = map.count(10));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(20));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(30));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(40));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(50));  TEST_EQUAL(n, 1u);
    TRY(n = map.count(60));  TEST_EQUAL(n, 0u);

    TRY(i = map.find(10));    TEST(i != map.end());    TEST_EQUAL(i->first, 10);   TEST_EQUAL(i->second, "aaa");
    TRY(i = map.find(50));    TEST(i != map.end());    TEST_EQUAL(i->first, 50);   TEST_EQUAL(i->second, "eee");
    TRY(ci = cmap.find(10));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 10);  TEST_EQUAL(ci->second, "aaa");
    TRY(ci = cmap.find(50));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 50);  TEST_EQUAL(ci->second, "eee");

    TRY(i = map.find(0));     TEST(i == map.end());
    TRY(i = map.find(60));    TEST(i == map.end());
    TRY(ci = cmap.find(0));   TEST(ci == cmap.end());
    TRY(ci = cmap.find(60));  TEST(ci == cmap.end());

    TRY(i = map.find_last(10));    TEST(i != map.end());    TEST_EQUAL(i->first, 10);   TEST_EQUAL(i->second, "aaa");
    TRY(i = map.find_last(50));    TEST(i != map.end());    TEST_EQUAL(i->first, 50);   TEST_EQUAL(i->second, "eee");
    TRY(ci = cmap.find_last(10));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 10);  TEST_EQUAL(ci->second, "aaa");
    TRY(ci = cmap.find_last(50));  TEST(ci != cmap.end());  TEST_EQUAL(ci->first, 50);  TEST_EQUAL(ci->second, "eee");

    TRY(i = map.find_last(0));     TEST(i == map.end());
    TRY(i = map.find_last(60));    TEST(i == map.end());
    TRY(ci = cmap.find_last(0));   TEST(ci == cmap.end());
    TRY(ci = cmap.find_last(60));  TEST(ci == cmap.end());

    TRY(iv = map.find_all(10));    TEST_EQUAL(iv.size(), 1u);   TEST_EQUAL(iv[0]->first, 10);   TEST_EQUAL(iv[0]->second, "aaa");
    TRY(iv = map.find_all(50));    TEST_EQUAL(iv.size(), 1u);   TEST_EQUAL(iv[0]->first, 50);   TEST_EQUAL(iv[0]->second, "eee");
    TRY(civ = cmap.find_all(10));  TEST_EQUAL(civ.size(), 1u);  TEST_EQUAL(civ[0]->first, 10);  TEST_EQUAL(civ[0]->second, "aaa");
    TRY(civ = cmap.find_all(50));  TEST_EQUAL(civ.size(), 1u);  TEST_EQUAL(civ[0]->first, 50);  TEST_EQUAL(civ[0]->second, "eee");

    TRY(iv = map.find_all(0));     TEST(iv.empty());
    TRY(iv = map.find_all(60));    TEST(iv.empty());
    TRY(civ = cmap.find_all(0));   TEST(civ.empty());
    TRY(civ = cmap.find_all(60));  TEST(civ.empty());

    TRY(s = map[10]);  TEST_EQUAL(s, "aaa");
    TRY(s = map[20]);  TEST_EQUAL(s, "bbb");
    TRY(s = map[30]);  TEST_EQUAL(s, "ccc");
    TRY(s = map[40]);  TEST_EQUAL(s, "ddd");
    TRY(s = map[50]);  TEST_EQUAL(s, "eee");

    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map), "{10:aaa,20:bbb,30:ccc,40:ddd,50:eee}");

    TRY(map[10] = "AAA");
    TRY(map[20] = "BBB");
    TRY(map[30] = "CCC");
    TRY(map[40] = "DDD");
    TRY(map[50] = "EEE");
    TEST_EQUAL(map.size(), 5u);
    TEST_EQUAL(format_map(map), "{10:AAA,20:BBB,30:CCC,40:DDD,50:EEE}");

    TRY(map[1] = "vvv");
    TRY(map[2] = "www");
    TRY(map[3] = "xxx");
    TRY(map[4] = "yyy");
    TRY(map[5] = "zzz");
    TEST_EQUAL(map.size(), 10u);
    TEST_EQUAL(format_map(map), "{10:AAA,20:BBB,30:CCC,40:DDD,50:EEE,1:vvv,2:www,3:xxx,4:yyy,5:zzz}");

}

void test_crow_stable_map_unique_rehash() {

    SM map;

    TRY(map.insert(10, "a"));  TEST_EQUAL(map.size(), 1u);   TEST_EQUAL(format_map(map), "{10:a}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 1u);   TEST_EQUAL(format_map(map), "{10:a}");
    TRY(map.insert(9, "b"));   TEST_EQUAL(map.size(), 2u);   TEST_EQUAL(format_map(map), "{10:a,9:b}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 2u);   TEST_EQUAL(format_map(map), "{10:a,9:b}");
    TRY(map.insert(8, "c"));   TEST_EQUAL(map.size(), 3u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 3u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c}");
    TRY(map.insert(7, "d"));   TEST_EQUAL(map.size(), 4u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 4u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d}");
    TRY(map.insert(6, "e"));   TEST_EQUAL(map.size(), 5u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 5u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e}");
    TRY(map.insert(5, "f"));   TEST_EQUAL(map.size(), 6u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 6u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f}");
    TRY(map.insert(4, "g"));   TEST_EQUAL(map.size(), 7u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 7u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g}");
    TRY(map.insert(3, "h"));   TEST_EQUAL(map.size(), 8u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 8u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h}");
    TRY(map.insert(2, "i"));   TEST_EQUAL(map.size(), 9u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,2:i}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 9u);   TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,2:i}");
    TRY(map.insert(1, "j"));   TEST_EQUAL(map.size(), 10u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,2:i,1:j}");
    TRY(map.rehash());         TEST_EQUAL(map.size(), 10u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,2:i,1:j}");

    TRY(map.erase(2));   TEST_EQUAL(map.size(), 9u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,1:j}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 9u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,4:g,3:h,1:j}");
    TRY(map.erase(4));   TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,3:h,1:j}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 8u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,6:e,5:f,3:h,1:j}");
    TRY(map.erase(6));   TEST_EQUAL(map.size(), 7u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,5:f,3:h,1:j}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 7u);  TEST_EQUAL(format_map(map), "{10:a,9:b,8:c,7:d,5:f,3:h,1:j}");
    TRY(map.erase(8));   TEST_EQUAL(map.size(), 6u);  TEST_EQUAL(format_map(map), "{10:a,9:b,7:d,5:f,3:h,1:j}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 6u);  TEST_EQUAL(format_map(map), "{10:a,9:b,7:d,5:f,3:h,1:j}");
    TRY(map.erase(10));  TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f,3:h,1:j}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 5u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f,3:h,1:j}");
    TRY(map.erase(1));   TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f,3:h}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 4u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f,3:h}");
    TRY(map.erase(3));   TEST_EQUAL(map.size(), 3u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 3u);  TEST_EQUAL(format_map(map), "{9:b,7:d,5:f}");
    TRY(map.erase(5));   TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{9:b,7:d}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 2u);  TEST_EQUAL(format_map(map), "{9:b,7:d}");
    TRY(map.erase(7));   TEST_EQUAL(map.size(), 1u);  TEST_EQUAL(format_map(map), "{9:b}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 1u);  TEST_EQUAL(format_map(map), "{9:b}");
    TRY(map.erase(9));   TEST_EQUAL(map.size(), 0u);  TEST_EQUAL(format_map(map), "{}");
    TRY(map.rehash());   TEST_EQUAL(map.size(), 0u);  TEST_EQUAL(format_map(map), "{}");

}
