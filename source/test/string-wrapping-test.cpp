#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_string_unwrap() {

    std::string s, t;

    TEST_EQUAL(unwrap_lines(""), "");

    s = "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit.\n";
    t = "Lorem ipsum dolor sit amet, "
        "consectetur adipisicing elit.\n";
    TEST_EQUAL(unwrap_lines(s), t);

    s = "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam,\n"
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n";
    t = "Lorem ipsum dolor sit amet, "
        "consectetur adipisicing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, "
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n";
    TEST_EQUAL(unwrap_lines(s), t);

    s = "    Lorem ipsum dolor sit amet,\n"
        "    consectetur adipisicing elit,\n"
        "    sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "    Ut enim ad minim veniam,\n"
        "    quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n";
    t = "    Lorem ipsum dolor sit amet, "
        "consectetur adipisicing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, "
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n";
    TEST_EQUAL(unwrap_lines(s), t);

    s = "\n"
        "\n"
        "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "    Lorem ipsum dolor sit amet,\n"
        "    consectetur adipisicing elit,\n"
        "    sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "    Ut enim ad minim veniam,\n"
        "    quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit.\n"
        "\n"
        "\n";
    t = "\n"
        "\n"
        "Lorem ipsum dolor sit amet, "
        "consectetur adipisicing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "    Lorem ipsum dolor sit amet, "
        "consectetur adipisicing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, "
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit.\n"
        "\n"
        "\n";
    TEST_EQUAL(unwrap_lines(s), t);

}

void test_crow_string_word_wrap() {

    std::string s, t;

    TEST_EQUAL(wrap_lines(""), "");

    s = "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit.\n";
        //...:....1....:....2....:....3....:....4....:....5
    t = "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit.\n";
    TEST_EQUAL(wrap_lines(s, 40), t);

    s = "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam,\n"
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n";
        //...:....1....:....2....:....3....:....4....:....5
    t = "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna\n"
        "aliqua. Ut enim ad minim veniam, quis\n"
        "nostrud exercitation ullamco laboris\n"
        "nisi ut aliquip ex ea commodo consequat.\n";
    TEST_EQUAL(wrap_lines(s, 40), t);

    s = "\n"
        "\n"
        "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam,\n"
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit.\n"
        "\n"
        "\n";
        //...:....1....:....2....:....3....:....4....:....5
    t = "\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna\n"
        "aliqua.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna\n"
        "aliqua. Ut enim ad minim veniam, quis\n"
        "nostrud exercitation ullamco laboris\n"
        "nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit.\n"
        "\n"
        "\n";
    TEST_EQUAL(wrap_lines(s, 40), t);

    s = "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam,\n"
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit.\n";
        //...:....1....:....2....:....3....:....4....:....5
    t = "        Lorem ipsum dolor sit amet, consectetur\n"
        "        adipisicing elit, sed do eiusmod tempor\n"
        "        incididunt ut labore et dolore magna\n"
        "        aliqua.\n"
        "\n"
        "        Lorem ipsum dolor sit amet, consectetur\n"
        "        adipisicing elit, sed do eiusmod tempor\n"
        "        incididunt ut labore et dolore magna\n"
        "        aliqua. Ut enim ad minim veniam, quis\n"
        "        nostrud exercitation ullamco laboris nisi\n"
        "        ut aliquip ex ea commodo consequat.\n"
        "\n"
        "        Lorem ipsum dolor sit amet, consectetur\n"
        "        adipisicing elit.\n";
    TEST_EQUAL(wrap_lines(s, 50, 8), t);

    s = "    Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "        Lorem ipsum dolor sit amet,\n"
        "consectetur adipisicing elit,\n"
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam,\n"
        "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit.\n";
        //...:....1....:....2....:....3....:....4....:....5
    t = "    Lorem ipsum dolor sit amet, consectetur\n"
        "    adipisicing elit, sed do eiusmod tempor\n"
        "    incididunt ut labore et dolore magna aliqua.\n"
        "\n"
        "        Lorem ipsum dolor sit amet, consectetur\n"
        "        adipisicing elit, sed do eiusmod tempor\n"
        "        incididunt ut labore et dolore magna\n"
        "        aliqua. Ut enim ad minim veniam, quis\n"
        "        nostrud exercitation ullamco laboris nisi\n"
        "        ut aliquip ex ea commodo consequat.\n"
        "\n"
        "Lorem ipsum dolor sit amet, consectetur\n"
        "adipisicing elit.\n";
    TEST_EQUAL(wrap_lines(s, 50), t);

}
