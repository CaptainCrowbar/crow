#include "crow/multi-array.hpp"
#include "crow/bounded-array.hpp"
#include "crow/compact-array.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <iterator>

using namespace Crow;

void test_crow_multi_array_2d() {

    using MA = MultiArray<int, 2>;

    MA array;
    const auto& carray = array;

    TEST(carray.empty());
    TEST_EQUAL(carray.shape(), Int2(0, 0));
    TEST_EQUAL(carray.size(), 0u);

    auto i = carray.begin();

    TEST(i == carray.end());
    TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array = MA(2, 5));
    TEST(! carray.empty());
    TEST_EQUAL(carray.shape(), Int2(2, 5));
    TEST_EQUAL(carray.size(), 10u);
    TEST_EQUAL(std::distance(carray.begin(), carray.end()), 10);

    TEST(! carray.contains(-1, -1));
    TEST(! carray.contains(0, -1));
    TEST(! carray.contains(1, -1));
    TEST(! carray.contains(2, -1));
    TEST(! carray.contains(-1, 0));
    TEST(carray.contains(0, 0));
    TEST(carray.contains(1, 0));
    TEST(! carray.contains(2, 0));
    TEST(! carray.contains(-1, 4));
    TEST(carray.contains(0, 4));
    TEST(carray.contains(1, 4));
    TEST(! carray.contains(2, 4));
    TEST(! carray.contains(-1, 5));
    TEST(! carray.contains(0, 5));
    TEST(! carray.contains(1, 5));
    TEST(! carray.contains(2, 5));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TRY((array(x, y) = 11 + 10 * x + y));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL((carray(x, y)), 11 + 10 * x + y);

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL(*carray.locate(x, y), 11 + 10 * x + y);

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);      TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(++i);                 TEST_EQUAL(*i, 21);      TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(++i);                 TEST_EQUAL(*i, 12);      TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(++i);                 TEST_EQUAL(*i, 22);      TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(++i);                 TEST_EQUAL(*i, 13);      TEST_EQUAL(i.pos(), Int2(0, 2));
    TRY(++i);                 TEST_EQUAL(*i, 23);      TEST_EQUAL(i.pos(), Int2(1, 2));
    TRY(++i);                 TEST_EQUAL(*i, 14);      TEST_EQUAL(i.pos(), Int2(0, 3));
    TRY(++i);                 TEST_EQUAL(*i, 24);      TEST_EQUAL(i.pos(), Int2(1, 3));
    TRY(++i);                 TEST_EQUAL(*i, 15);      TEST_EQUAL(i.pos(), Int2(0, 4));
    TRY(++i);                 TEST_EQUAL(*i, 25);      TEST_EQUAL(i.pos(), Int2(1, 4));
    TRY(++i);                 TEST(i == carray.end());

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(i.move(0));           TEST_EQUAL(*i, 21);  TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(i.move(1));           TEST_EQUAL(*i, 22);  TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(i.move(0, -1));       TEST_EQUAL(*i, 12);  TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(i.move(1, -1));       TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array.fill(42));
    TEST_EQUAL((carray(0, 0)), 42);
    TEST_EQUAL((carray(1, 0)), 42);
    TEST_EQUAL((carray(0, 4)), 42);
    TEST_EQUAL((carray(1, 4)), 42);

}

void test_crow_multi_array_3d() {

    using MA = MultiArray<int, 3>;

    MA array;
    const auto& carray = array;

    TEST(carray.empty());
    TEST_EQUAL(carray.shape(), Int3(0, 0, 0));
    TEST_EQUAL(carray.size(), 0u);

    auto i = carray.begin();

    TEST(i == carray.end());
    TEST_EQUAL(i.pos(), Int3(0, 0, 0));

    TRY(array = MA(2, 3, 4));
    TEST(! carray.empty());
    TEST_EQUAL(carray.shape(), Int3(2, 3, 4));
    TEST_EQUAL(carray.size(), 24u);
    TEST_EQUAL(std::distance(carray.begin(), carray.end()), 24);

    TEST(! carray.contains(-1, -1, -1));
    TEST(! carray.contains(0, -1, -1));
    TEST(! carray.contains(1, -1, -1));
    TEST(! carray.contains(2, -1, -1));
    TEST(! carray.contains(-1, 0, -1));
    TEST(! carray.contains(0, 0, -1));
    TEST(! carray.contains(1, 0, -1));
    TEST(! carray.contains(2, 0, -1));
    TEST(! carray.contains(-1, 2, -1));
    TEST(! carray.contains(0, 2, -1));
    TEST(! carray.contains(1, 2, -1));
    TEST(! carray.contains(2, 2, -1));
    TEST(! carray.contains(-1, 3, -1));
    TEST(! carray.contains(0, 3, -1));
    TEST(! carray.contains(1, 3, -1));
    TEST(! carray.contains(2, 3, -1));
    TEST(! carray.contains(-1, -1, 0));
    TEST(! carray.contains(0, -1, 0));
    TEST(! carray.contains(1, -1, 0));
    TEST(! carray.contains(2, -1, 0));
    TEST(! carray.contains(-1, 0, 0));
    TEST(carray.contains(0, 0, 0));
    TEST(carray.contains(1, 0, 0));
    TEST(! carray.contains(2, 0, 0));
    TEST(! carray.contains(-1, 2, 0));
    TEST(carray.contains(0, 2, 0));
    TEST(carray.contains(1, 2, 0));
    TEST(! carray.contains(2, 2, 0));
    TEST(! carray.contains(-1, 3, 0));
    TEST(! carray.contains(0, 3, 0));
    TEST(! carray.contains(1, 3, 0));
    TEST(! carray.contains(2, 3, 0));
    TEST(! carray.contains(-1, -1, 3));
    TEST(! carray.contains(0, -1, 3));
    TEST(! carray.contains(1, -1, 3));
    TEST(! carray.contains(2, -1, 3));
    TEST(! carray.contains(-1, 0, 3));
    TEST(carray.contains(0, 0, 3));
    TEST(carray.contains(1, 0, 3));
    TEST(! carray.contains(2, 0, 3));
    TEST(! carray.contains(-1, 2, 3));
    TEST(carray.contains(0, 2, 3));
    TEST(carray.contains(1, 2, 3));
    TEST(! carray.contains(2, 2, 3));
    TEST(! carray.contains(-1, 3, 3));
    TEST(! carray.contains(0, 3, 3));
    TEST(! carray.contains(1, 3, 3));
    TEST(! carray.contains(2, 3, 3));
    TEST(! carray.contains(-1, -1, 4));
    TEST(! carray.contains(0, -1, 4));
    TEST(! carray.contains(1, -1, 4));
    TEST(! carray.contains(2, -1, 4));
    TEST(! carray.contains(-1, 0, 4));
    TEST(! carray.contains(0, 0, 4));
    TEST(! carray.contains(1, 0, 4));
    TEST(! carray.contains(2, 0, 4));
    TEST(! carray.contains(-1, 2, 4));
    TEST(! carray.contains(0, 2, 4));
    TEST(! carray.contains(1, 2, 4));
    TEST(! carray.contains(2, 2, 4));
    TEST(! carray.contains(-1, 3, 4));
    TEST(! carray.contains(0, 3, 4));
    TEST(! carray.contains(1, 3, 4));
    TEST(! carray.contains(2, 3, 4));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 4; ++z)
                TRY((array(x, y, z) = 111 + 100 * x + 10 * y + z));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 4; ++z)
                TEST_EQUAL((carray(x, y, z)), 111 + 100 * x + 10 * y + z);

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 4; ++z)
                TEST_EQUAL(*carray.locate(x, y, z), 111 + 100 * x + 10 * y + z);

    TRY(i = carray.begin());  TEST_EQUAL(*i, 111);      TEST_EQUAL(i.pos(), Int3(0, 0, 0));
    TRY(++i);                 TEST_EQUAL(*i, 211);      TEST_EQUAL(i.pos(), Int3(1, 0, 0));
    TRY(++i);                 TEST_EQUAL(*i, 121);      TEST_EQUAL(i.pos(), Int3(0, 1, 0));
    TRY(++i);                 TEST_EQUAL(*i, 221);      TEST_EQUAL(i.pos(), Int3(1, 1, 0));
    TRY(++i);                 TEST_EQUAL(*i, 131);      TEST_EQUAL(i.pos(), Int3(0, 2, 0));
    TRY(++i);                 TEST_EQUAL(*i, 231);      TEST_EQUAL(i.pos(), Int3(1, 2, 0));
    TRY(++i);                 TEST_EQUAL(*i, 112);      TEST_EQUAL(i.pos(), Int3(0, 0, 1));
    TRY(++i);                 TEST_EQUAL(*i, 212);      TEST_EQUAL(i.pos(), Int3(1, 0, 1));
    TRY(++i);                 TEST_EQUAL(*i, 122);      TEST_EQUAL(i.pos(), Int3(0, 1, 1));
    TRY(++i);                 TEST_EQUAL(*i, 222);      TEST_EQUAL(i.pos(), Int3(1, 1, 1));
    TRY(++i);                 TEST_EQUAL(*i, 132);      TEST_EQUAL(i.pos(), Int3(0, 2, 1));
    TRY(++i);                 TEST_EQUAL(*i, 232);      TEST_EQUAL(i.pos(), Int3(1, 2, 1));
    TRY(++i);                 TEST_EQUAL(*i, 113);      TEST_EQUAL(i.pos(), Int3(0, 0, 2));
    TRY(++i);                 TEST_EQUAL(*i, 213);      TEST_EQUAL(i.pos(), Int3(1, 0, 2));
    TRY(++i);                 TEST_EQUAL(*i, 123);      TEST_EQUAL(i.pos(), Int3(0, 1, 2));
    TRY(++i);                 TEST_EQUAL(*i, 223);      TEST_EQUAL(i.pos(), Int3(1, 1, 2));
    TRY(++i);                 TEST_EQUAL(*i, 133);      TEST_EQUAL(i.pos(), Int3(0, 2, 2));
    TRY(++i);                 TEST_EQUAL(*i, 233);      TEST_EQUAL(i.pos(), Int3(1, 2, 2));
    TRY(++i);                 TEST_EQUAL(*i, 114);      TEST_EQUAL(i.pos(), Int3(0, 0, 3));
    TRY(++i);                 TEST_EQUAL(*i, 214);      TEST_EQUAL(i.pos(), Int3(1, 0, 3));
    TRY(++i);                 TEST_EQUAL(*i, 124);      TEST_EQUAL(i.pos(), Int3(0, 1, 3));
    TRY(++i);                 TEST_EQUAL(*i, 224);      TEST_EQUAL(i.pos(), Int3(1, 1, 3));
    TRY(++i);                 TEST_EQUAL(*i, 134);      TEST_EQUAL(i.pos(), Int3(0, 2, 3));
    TRY(++i);                 TEST_EQUAL(*i, 234);      TEST_EQUAL(i.pos(), Int3(1, 2, 3));
    TRY(++i);                 TEST(i == carray.end());

    TRY(i = carray.begin());  TEST_EQUAL(*i, 111);  TEST_EQUAL(i.pos(), Int3(0, 0, 0));
    TRY(i.move(0));           TEST_EQUAL(*i, 211);  TEST_EQUAL(i.pos(), Int3(1, 0, 0));
    TRY(i.move(1));           TEST_EQUAL(*i, 221);  TEST_EQUAL(i.pos(), Int3(1, 1, 0));
    TRY(i.move(2));           TEST_EQUAL(*i, 222);  TEST_EQUAL(i.pos(), Int3(1, 1, 1));
    TRY(i.move(0, -1));       TEST_EQUAL(*i, 122);  TEST_EQUAL(i.pos(), Int3(0, 1, 1));
    TRY(i.move(1, -1));       TEST_EQUAL(*i, 112);  TEST_EQUAL(i.pos(), Int3(0, 0, 1));
    TRY(i.move(2, -1));       TEST_EQUAL(*i, 111);  TEST_EQUAL(i.pos(), Int3(0, 0, 0));

    TRY(array.fill(42));
    TEST_EQUAL((carray(0, 0, 0)), 42);
    TEST_EQUAL((carray(1, 0, 0)), 42);
    TEST_EQUAL((carray(0, 2, 0)), 42);
    TEST_EQUAL((carray(1, 2, 0)), 42);
    TEST_EQUAL((carray(0, 0, 3)), 42);
    TEST_EQUAL((carray(1, 0, 3)), 42);
    TEST_EQUAL((carray(0, 2, 3)), 42);
    TEST_EQUAL((carray(1, 2, 3)), 42);

}

void test_crow_multi_array_bounded() {

    using MA = MultiArray<int, 2, BoundedArray<int, 10>>;

    MA array;
    const auto& carray = array;

    TEST(carray.empty());
    TEST_EQUAL(carray.shape(), Int2(0, 0));
    TEST_EQUAL(carray.size(), 0u);

    auto i = carray.begin();

    TEST(i == carray.end());
    TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array = MA(2, 5));
    TEST(! carray.empty());
    TEST_EQUAL(carray.shape(), Int2(2, 5));
    TEST_EQUAL(carray.size(), 10u);
    TEST_EQUAL(std::distance(carray.begin(), carray.end()), 10);

    TEST(! carray.contains(-1, -1));
    TEST(! carray.contains(0, -1));
    TEST(! carray.contains(1, -1));
    TEST(! carray.contains(2, -1));
    TEST(! carray.contains(-1, 0));
    TEST(carray.contains(0, 0));
    TEST(carray.contains(1, 0));
    TEST(! carray.contains(2, 0));
    TEST(! carray.contains(-1, 4));
    TEST(carray.contains(0, 4));
    TEST(carray.contains(1, 4));
    TEST(! carray.contains(2, 4));
    TEST(! carray.contains(-1, 5));
    TEST(! carray.contains(0, 5));
    TEST(! carray.contains(1, 5));
    TEST(! carray.contains(2, 5));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TRY((array(x, y) = 11 + 10 * x + y));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL((carray(x, y)), 11 + 10 * x + y);

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL(*carray.locate(x, y), 11 + 10 * x + y);

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);      TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(++i);                 TEST_EQUAL(*i, 21);      TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(++i);                 TEST_EQUAL(*i, 12);      TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(++i);                 TEST_EQUAL(*i, 22);      TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(++i);                 TEST_EQUAL(*i, 13);      TEST_EQUAL(i.pos(), Int2(0, 2));
    TRY(++i);                 TEST_EQUAL(*i, 23);      TEST_EQUAL(i.pos(), Int2(1, 2));
    TRY(++i);                 TEST_EQUAL(*i, 14);      TEST_EQUAL(i.pos(), Int2(0, 3));
    TRY(++i);                 TEST_EQUAL(*i, 24);      TEST_EQUAL(i.pos(), Int2(1, 3));
    TRY(++i);                 TEST_EQUAL(*i, 15);      TEST_EQUAL(i.pos(), Int2(0, 4));
    TRY(++i);                 TEST_EQUAL(*i, 25);      TEST_EQUAL(i.pos(), Int2(1, 4));
    TRY(++i);                 TEST(i == carray.end());

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(i.move(0));           TEST_EQUAL(*i, 21);  TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(i.move(1));           TEST_EQUAL(*i, 22);  TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(i.move(0, -1));       TEST_EQUAL(*i, 12);  TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(i.move(1, -1));       TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array.fill(42));
    TEST_EQUAL((carray(0, 0)), 42);
    TEST_EQUAL((carray(1, 0)), 42);
    TEST_EQUAL((carray(0, 4)), 42);
    TEST_EQUAL((carray(1, 4)), 42);

}

void test_crow_multi_array_compact() {

    using MA = MultiArray<int, 2, CompactArray<int, 5>>;

    MA array;
    const auto& carray = array;

    TEST(carray.empty());
    TEST_EQUAL(carray.shape(), Int2(0, 0));
    TEST_EQUAL(carray.size(), 0u);

    auto i = carray.begin();

    TEST(i == carray.end());
    TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array = MA(2, 5));
    TEST(! carray.empty());
    TEST_EQUAL(carray.shape(), Int2(2, 5));
    TEST_EQUAL(carray.size(), 10u);
    TEST_EQUAL(std::distance(carray.begin(), carray.end()), 10);

    TEST(! carray.contains(-1, -1));
    TEST(! carray.contains(0, -1));
    TEST(! carray.contains(1, -1));
    TEST(! carray.contains(2, -1));
    TEST(! carray.contains(-1, 0));
    TEST(carray.contains(0, 0));
    TEST(carray.contains(1, 0));
    TEST(! carray.contains(2, 0));
    TEST(! carray.contains(-1, 4));
    TEST(carray.contains(0, 4));
    TEST(carray.contains(1, 4));
    TEST(! carray.contains(2, 4));
    TEST(! carray.contains(-1, 5));
    TEST(! carray.contains(0, 5));
    TEST(! carray.contains(1, 5));
    TEST(! carray.contains(2, 5));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TRY((array(x, y) = 11 + 10 * x + y));

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL((carray(x, y)), 11 + 10 * x + y);

    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 5; ++y)
            TEST_EQUAL(*carray.locate(x, y), 11 + 10 * x + y);

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);      TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(++i);                 TEST_EQUAL(*i, 21);      TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(++i);                 TEST_EQUAL(*i, 12);      TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(++i);                 TEST_EQUAL(*i, 22);      TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(++i);                 TEST_EQUAL(*i, 13);      TEST_EQUAL(i.pos(), Int2(0, 2));
    TRY(++i);                 TEST_EQUAL(*i, 23);      TEST_EQUAL(i.pos(), Int2(1, 2));
    TRY(++i);                 TEST_EQUAL(*i, 14);      TEST_EQUAL(i.pos(), Int2(0, 3));
    TRY(++i);                 TEST_EQUAL(*i, 24);      TEST_EQUAL(i.pos(), Int2(1, 3));
    TRY(++i);                 TEST_EQUAL(*i, 15);      TEST_EQUAL(i.pos(), Int2(0, 4));
    TRY(++i);                 TEST_EQUAL(*i, 25);      TEST_EQUAL(i.pos(), Int2(1, 4));
    TRY(++i);                 TEST(i == carray.end());

    TRY(i = carray.begin());  TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));
    TRY(i.move(0));           TEST_EQUAL(*i, 21);  TEST_EQUAL(i.pos(), Int2(1, 0));
    TRY(i.move(1));           TEST_EQUAL(*i, 22);  TEST_EQUAL(i.pos(), Int2(1, 1));
    TRY(i.move(0, -1));       TEST_EQUAL(*i, 12);  TEST_EQUAL(i.pos(), Int2(0, 1));
    TRY(i.move(1, -1));       TEST_EQUAL(*i, 11);  TEST_EQUAL(i.pos(), Int2(0, 0));

    TRY(array.fill(42));
    TEST_EQUAL((carray(0, 0)), 42);
    TEST_EQUAL((carray(1, 0)), 42);
    TEST_EQUAL((carray(0, 4)), 42);
    TEST_EQUAL((carray(1, 4)), 42);

}
