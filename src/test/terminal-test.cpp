#include "crow/terminal.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_terminal_control() {

    static constexpr Xterm::colour c1 = {0, 0, 0};
    static constexpr Xterm::colour c2 = {1, 2, 3};
    static constexpr Xterm::colour c3 = {5, 4, 3};
    static constexpr Xterm::colour c4 = {5, 5, 5};

    Xterm xt;

    TRY(xt = Xterm(false));

    TEST_EQUAL(xt.bold(),        "");  TEST_EQUAL(xt.reset(),          "");
    TEST_EQUAL(xt.grey(1),       "");  TEST_EQUAL(xt.grey_bg(1),       "");
    TEST_EQUAL(xt.grey(10),      "");  TEST_EQUAL(xt.grey_bg(10),      "");
    TEST_EQUAL(xt.grey(24),      "");  TEST_EQUAL(xt.grey_bg(24),      "");
    TEST_EQUAL(xt.rgb(0, 0, 0),  "");  TEST_EQUAL(xt.rgb_bg(0, 0, 0),  "");
    TEST_EQUAL(xt.rgb(1, 2, 3),  "");  TEST_EQUAL(xt.rgb_bg(1, 2, 3),  "");
    TEST_EQUAL(xt.rgb(5, 4, 3),  "");  TEST_EQUAL(xt.rgb_bg(5, 4, 3),  "");
    TEST_EQUAL(xt.rgb(5, 5, 5),  "");  TEST_EQUAL(xt.rgb_bg(5, 5, 5),  "");

    TRY(xt = Xterm(true));

    TEST_EQUAL(xt.bold(),          "\x1b[1m");         TEST_EQUAL(xt.reset(),            "\x1b[0m");
    TEST_EQUAL(xt.grey(1),         "\x1b[38;5;232m");  TEST_EQUAL(xt.grey_bg(1),         "\x1b[48;5;232m");
    TEST_EQUAL(xt.grey(10),        "\x1b[38;5;241m");  TEST_EQUAL(xt.grey_bg(10),        "\x1b[48;5;241m");
    TEST_EQUAL(xt.grey(24),        "\x1b[38;5;255m");  TEST_EQUAL(xt.grey_bg(24),        "\x1b[48;5;255m");
    TEST_EQUAL(xt.rgb(0, 0, 0),    "\x1b[38;5;16m");   TEST_EQUAL(xt.rgb_bg(0, 0, 0),    "\x1b[48;5;16m");
    TEST_EQUAL(xt.rgb(1, 2, 3),    "\x1b[38;5;67m");   TEST_EQUAL(xt.rgb_bg(1, 2, 3),    "\x1b[48;5;67m");
    TEST_EQUAL(xt.rgb(5, 4, 3),    "\x1b[38;5;223m");  TEST_EQUAL(xt.rgb_bg(5, 4, 3),    "\x1b[48;5;223m");
    TEST_EQUAL(xt.rgb(5, 5, 5),    "\x1b[38;5;231m");  TEST_EQUAL(xt.rgb_bg(5, 5, 5),    "\x1b[48;5;231m");
    TEST_EQUAL(xt.rgb({0, 0, 0}),  "\x1b[38;5;16m");   TEST_EQUAL(xt.rgb_bg({0, 0, 0}),  "\x1b[48;5;16m");
    TEST_EQUAL(xt.rgb({1, 2, 3}),  "\x1b[38;5;67m");   TEST_EQUAL(xt.rgb_bg({1, 2, 3}),  "\x1b[48;5;67m");
    TEST_EQUAL(xt.rgb({5, 4, 3}),  "\x1b[38;5;223m");  TEST_EQUAL(xt.rgb_bg({5, 4, 3}),  "\x1b[48;5;223m");
    TEST_EQUAL(xt.rgb({5, 5, 5}),  "\x1b[38;5;231m");  TEST_EQUAL(xt.rgb_bg({5, 5, 5}),  "\x1b[48;5;231m");
    TEST_EQUAL(xt.rgb(c1),         "\x1b[38;5;16m");   TEST_EQUAL(xt.rgb_bg(c1),         "\x1b[48;5;16m");
    TEST_EQUAL(xt.rgb(c2),         "\x1b[38;5;67m");   TEST_EQUAL(xt.rgb_bg(c2),         "\x1b[48;5;67m");
    TEST_EQUAL(xt.rgb(c3),         "\x1b[38;5;223m");  TEST_EQUAL(xt.rgb_bg(c3),         "\x1b[48;5;223m");
    TEST_EQUAL(xt.rgb(c4),         "\x1b[38;5;231m");  TEST_EQUAL(xt.rgb_bg(c4),         "\x1b[48;5;231m");

}
