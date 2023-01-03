#include "crow/string-view.hpp"
#include "crow/unit-test.hpp"
#include <string_view>

using namespace Crow;

void test_crow_string_view_null() {

    std::string_view a;
    std::string_view b = "";
    std::string_view c = "Hello world";

    TEST(view_is_null(a));
    TEST(! view_is_null(b));
    TEST(! view_is_null(c));

}

void test_crow_string_view_is_substring() {

    std::string_view s = "Hello world";
    std::string_view t = s.substr(0, 0);
    std::string_view u = s.substr(6, 0);
    std::string_view v = s.substr(11, 0);
    std::string_view w = s.substr(0, 5);
    std::string_view x = s.substr(6, 5);
    std::string_view y = "Hello";
    std::string_view z;

    TEST(view_is_substring(s, s));
    TEST(view_is_substring(t, s));
    TEST(view_is_substring(u, s));
    TEST(view_is_substring(v, s));
    TEST(view_is_substring(w, s));
    TEST(view_is_substring(x, s));
    TEST(! view_is_substring(y, s));
    TEST(! view_is_substring(z, s));
    TEST(! view_is_substring(s, z));
    TEST(view_is_substring(z, z));

}

void test_crow_string_view_position() {

    std::string_view s = "Hello world";
    std::string_view t = s.substr(0, 5);
    std::string_view u = s.substr(6, 5);

    TEST_EQUAL(view_pos(s, t), 0u);
    TEST_EQUAL(view_pos(s, u), 6u);
    TEST_EQUAL(view_endpos(s, t), 5u);
    TEST_EQUAL(view_endpos(s, u), 11u);

}

void test_crow_string_view_begin_end() {

    std::string_view s = "";
    std::string_view t = "Hello world";
    std::string_view u;

    TRY(u = view_begin(s));  TEST_EQUAL(view_pos(s, u), 0u);   TEST_EQUAL(u, "");
    TRY(u = view_end(s));    TEST_EQUAL(view_pos(s, u), 0u);   TEST_EQUAL(u, "");
    TRY(u = view_begin(t));  TEST_EQUAL(view_pos(t, u), 0u);   TEST_EQUAL(u, "");
    TRY(u = view_end(t));    TEST_EQUAL(view_pos(t, u), 11u);  TEST_EQUAL(u, "");

}

void test_crow_string_view_cat() {

    std::string_view s = "Hello world";
    std::string_view t, u, v;

    t = s.substr(0, 0);  u = s.substr(0, 0);  TRY(v = view_cat(t, u));  TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "");
    t = s.substr(0, 0);  u = s.substr(0, 5);  TRY(v = view_cat(t, u));  TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "Hello");
    t = s.substr(0, 5);  u = s.substr(6, 5);  TRY(v = view_cat(t, u));  TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "Hello world");
    t = s.substr(6, 1);  u = s.substr(6, 5);  TRY(v = view_cat(t, u));  TEST_EQUAL(view_pos(s, v), 6u);  TEST_EQUAL(v, "world");

}

void test_crow_string_view_extend() {

    std::string_view s = "Hello world";
    std::string_view t, u;

    t = s.substr(0, 5);  TRY(u = view_extend(t, 1));        TEST_EQUAL(view_pos(s, u), 0u);  TEST_EQUAL(u, "Hello ");
    t = s.substr(0, 5);  TRY(u = view_extend(t, 6));        TEST_EQUAL(view_pos(s, u), 0u);  TEST_EQUAL(u, "Hello world");
    t = s.substr(5, 1);  TRY(u = view_extend(t, 5));        TEST_EQUAL(view_pos(s, u), 5u);  TEST_EQUAL(u, " world");
    t = s.substr(0, 5);  TRY(u = view_extend(s, t, 1));     TEST_EQUAL(view_pos(s, u), 0u);  TEST_EQUAL(u, "Hello ");
    t = s.substr(0, 5);  TRY(u = view_extend(s, t, 6));     TEST_EQUAL(view_pos(s, u), 0u);  TEST_EQUAL(u, "Hello world");
    t = s.substr(0, 5);  TRY(u = view_extend(s, t, npos));  TEST_EQUAL(view_pos(s, u), 0u);  TEST_EQUAL(u, "Hello world");
    t = s.substr(5, 1);  TRY(u = view_extend(s, t, 5));     TEST_EQUAL(view_pos(s, u), 5u);  TEST_EQUAL(u, " world");
    t = s.substr(5, 1);  TRY(u = view_extend(s, t, npos));  TEST_EQUAL(view_pos(s, u), 5u);  TEST_EQUAL(u, " world");

}

void test_crow_string_view_left_right() {

    std::string_view s = "";
    std::string_view t = "Hello world";
    std::string_view u, v;

    u = s.substr(0, 0);  TRY(v = view_left_of(s, u));     TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "");
    u = s.substr(0, 0);  TRY(v = view_right_of(s, u));    TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "");
    u = t.substr(5, 1);  TRY(v = view_left_of(t, u));     TEST_EQUAL(view_pos(t, v), 0u);  TEST_EQUAL(v, "Hello");
    u = t.substr(5, 1);  TRY(v = view_right_of(t, u));    TEST_EQUAL(view_pos(t, v), 6u);  TEST_EQUAL(v, "world");
    u = s.substr(0, 0);  TRY(v = view_from_left(s, u));   TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "");
    u = s.substr(0, 0);  TRY(v = view_from_right(s, u));  TEST_EQUAL(view_pos(s, v), 0u);  TEST_EQUAL(v, "");
    u = t.substr(4, 3);  TRY(v = view_from_left(t, u));   TEST_EQUAL(view_pos(t, v), 0u);  TEST_EQUAL(v, "Hello w");
    u = t.substr(4, 3);  TRY(v = view_from_right(t, u));  TEST_EQUAL(view_pos(t, v), 4u);  TEST_EQUAL(v, "o world");

}
