#include "crow/hexmap.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <functional>

using namespace Crow;

namespace {

    static constexpr auto cmp_hex = std::less<Hexmap::hex>();

}

void test_crow_hexmap_adjacency_properties() {

    Hexmap map;
    Hexmap::hexlist list;

    TEST_EQUAL(map.rings(), 0);
    TEST_EQUAL(map.size(), 0);

    TEST_EQUAL(format_range(map.all()), "[]");
    TEST_EQUAL(format_range(map.border_in()), "[]");
    TEST_EQUAL(format_range(map.border_out()), "[]");

    //      __      //
    //   __/10\__   //
    //  /15\__/11\  //
    //  \__/00\__/  //
    //  /14\__/12\  //
    //  \__/13\__/  //
    //     \__/     //

    TRY(map.set({0,0}, "00"));
    TRY(map.set({1,0}, "10"));
    TRY(map.set({1,1}, "11"));
    TRY(map.set({1,2}, "12"));
    TRY(map.set({1,3}, "13"));
    TRY(map.set({1,4}, "14"));
    TRY(map.set({1,5}, "15"));

    TEST_EQUAL(map.rings(), 2);
    TEST_EQUAL(map.size(), 7);

    TEST_EQUAL(map.label({0,0}), "00");
    TEST_EQUAL(map.label({1,0}), "10");
    TEST_EQUAL(map.label({1,1}), "11");
    TEST_EQUAL(map.label({1,2}), "12");
    TEST_EQUAL(map.label({1,3}), "13");
    TEST_EQUAL(map.label({1,4}), "14");
    TEST_EQUAL(map.label({1,5}), "15");

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.border_in());
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(list.size(), 6u);
    TEST_EQUAL(format_range(list), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 12u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7],[2,8],[2,9],[2,10],[2,11]]");

    TRY(list = map.reachable({0,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({1,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({2,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({3,0}));
    TEST(list.empty());

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __/10\__/22\  //
    //  /15\__/11\__/  //
    //  \__/00\__/23\  //
    //  /14\__/12\__/  //
    //  \__/13\__/24\  //
    //     \__/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TRY(map.set({2,0}, "20"));
    TRY(map.set({2,1}, "21"));
    TRY(map.set({2,2}, "22"));
    TRY(map.set({2,3}, "23"));
    TRY(map.set({2,4}, "24"));
    TRY(map.set({2,5}, "25"));
    TRY(map.set({2,6}, "26"));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 14);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 14u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 17u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    TEST_EQUAL(format_range(map.neighbours_set({0,0})), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TEST_EQUAL(format_range(map.neighbours_unset({0,0})), "[]");
    TEST_EQUAL(format_range(map.neighbours_set({2,0})), "[[2,1],[1,0]]");
    TEST_EQUAL(format_range(map.neighbours_unset({2,0})), "[[3,0],[3,1],[2,11],[3,17]]");
    TEST_EQUAL(format_range(map.neighbours_set({4,0})), "[]");
    TEST_EQUAL(format_range(map.neighbours_unset({4,0})), "[[5,0],[5,1],[4,1],[3,0],[4,23],[5,29]]");

    TEST_EQUAL(format_range(map.neighbours_set_unset({0,0}).first), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TEST_EQUAL(format_range(map.neighbours_set_unset({0,0}).second), "[]");
    TEST_EQUAL(format_range(map.neighbours_set_unset({2,0}).first), "[[2,1],[1,0]]");
    TEST_EQUAL(format_range(map.neighbours_set_unset({2,0}).second), "[[3,0],[3,1],[2,11],[3,17]]");
    TEST_EQUAL(format_range(map.neighbours_set_unset({4,0}).first), "[]");
    TEST_EQUAL(format_range(map.neighbours_set_unset({4,0}).second), "[[5,0],[5,1],[4,1],[3,0],[4,23],[5,29]]");

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __/10\__/22\  //
    //  /15\__/11\__/  //
    //  \__/  \__/23\  //
    //  /14\__/12\__/  //
    //  \__/13\__/24\  //
    //     \__/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TEST_EQUAL(map.size(), 14);
    TRY(map.set({0,0}, ""));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 13);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 13u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 13u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 18u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __   \__/22\  //
    //  /15\  /11\__/  //
    //  \__/  \__/23\  //
    //  /14\  /12\__/  //
    //  \__/  \__/24\  //
    //      __/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TRY(map.erase({1,0}));
    TRY(map.erase({1,3}));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 11);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 20u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[1,0],[1,3],"
        "[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    TRY(list = map.reachable({0,0}));
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.reachable({1,1}));
    TEST_EQUAL(list.size(), 9u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.reachable({1,5}));
    TEST_EQUAL(list.size(), 2u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,4],[1,5]]");

}

void test_crow_hexmap_initializer_list() {

    Hexmap map;

    TRY((map = {
        {{0,0}, "00"},
        {{1,0}, "10"},
        {{1,1}, "11"},
        {{1,2}, "12"},
        {{1,3}, "13"},
        {{1,4}, "14"},
        {{1,5}, "15"},
    }));

    TEST_EQUAL(map.rings(), 2);
    TEST_EQUAL(map.size(), 7);

    TEST_EQUAL(map.label({0,0}), "00");
    TEST_EQUAL(map.label({1,0}), "10");
    TEST_EQUAL(map.label({1,1}), "11");
    TEST_EQUAL(map.label({1,2}), "12");
    TEST_EQUAL(map.label({1,3}), "13");
    TEST_EQUAL(map.label({1,4}), "14");
    TEST_EQUAL(map.label({1,5}), "15");

    TEST_EQUAL(map.str(),
        R"(    __    )" "\n"
        R"( __/10\__ )" "\n"
        R"(/15\__/11\)" "\n"
        R"(\__/00\__/)" "\n"
        R"(/14\__/12\)" "\n"
        R"(\__/13\__/)" "\n"
        R"(   \__/   )" "\n"
    );

    TRY((map = {
        {{0,0}, "aa", {5,3,1}},
        {{1,0}, "ba", {2,5,2}},
        {{1,1}, "bb", {2,2,5}},
        {{1,2}, "bc", {2,5,2}},
        {{1,3}, "bd", {2,2,5}},
        {{1,4}, "be", {2,5,2}},
        {{1,5}, "bf", {2,2,5}},
    }));

    TEST_EQUAL(map.rings(), 2);
    TEST_EQUAL(map.size(), 7);

    TEST_EQUAL(map.label({0,0}), "aa");
    TEST_EQUAL(map.label({1,0}), "ba");
    TEST_EQUAL(map.label({1,1}), "bb");
    TEST_EQUAL(map.label({1,2}), "bc");
    TEST_EQUAL(map.label({1,3}), "bd");
    TEST_EQUAL(map.label({1,4}), "be");
    TEST_EQUAL(map.label({1,5}), "bf");

    TEST_EQUAL(map.colour({0,0}), Hexmap::xcolour(5,3,1));
    TEST_EQUAL(map.colour({1,0}), Hexmap::xcolour(2,5,2));
    TEST_EQUAL(map.colour({1,1}), Hexmap::xcolour(2,2,5));
    TEST_EQUAL(map.colour({1,2}), Hexmap::xcolour(2,5,2));
    TEST_EQUAL(map.colour({1,3}), Hexmap::xcolour(2,2,5));
    TEST_EQUAL(map.colour({1,4}), Hexmap::xcolour(2,5,2));
    TEST_EQUAL(map.colour({1,5}), Hexmap::xcolour(2,2,5));

    TEST_EQUAL(map.str(),
        R"(    __    )" "\n"
        R"( __/ba\__ )" "\n"
        R"(/bf\__/bb\)" "\n"
        R"(\__/aa\__/)" "\n"
        R"(/be\__/bc\)" "\n"
        R"(\__/bd\__/)" "\n"
        R"(   \__/   )" "\n"
    );

}
