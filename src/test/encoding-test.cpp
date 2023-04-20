#include "crow/encoding.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

void test_crow_encoding_hexcode() {

    static const std::vector<std::pair<std::string, std::string>> tests = {

        { "",                        ""                                                },
        { "Hello",                   "48656c6c6f"                                      },
        { "Hello world\nGoodbye\n",  "48656c6c6f\n20776f726c\n640a476f6f\n646279650a"  },

    };

    Hexcode code(11);
    std::string str;

    for (auto& [plain,encoded]: tests) {
        TRY(str = code.encode(plain));
        TEST_EQUAL(str, encoded);
        TRY(str = code.decode(encoded));
        TEST_EQUAL(str, plain);
    }

}

void test_crow_encoding_base64() {

    static const std::vector<std::pair<std::string, std::string>> tests = {

        { "",               ""                          },
        { "a",              "YQ=="                      },
        { "ab",             "YWI="                      },
        { "abc",            "YWJj"                      },
        { "abcd",           "YWJjZA=\n="                },
        { "abcde",          "YWJjZGU\n="                },
        { "abcdef",         "YWJjZGV\nm"                },
        { "abcdefg",        "YWJjZGV\nmZw=="            },
        { "abcdefgh",       "YWJjZGV\nmZ2g="            },
        { "abcdefghi",      "YWJjZGV\nmZ2hp"            },
        { "abcdefghij",     "YWJjZGV\nmZ2hpag\n=="      },
        { "abcdefghijk",    "YWJjZGV\nmZ2hpam\ns="      },
        { "abcdefghijkl",   "YWJjZGV\nmZ2hpam\nts"      },
        { "abcdefghijklm",  "YWJjZGV\nmZ2hpam\ntsbQ=="  },

    };

    Base64 code(7);
    Base64 code_nopad(npos, "+/", false);
    std::string str;

    for (auto [plain,encoded]: tests) {
        auto stripped = trim_right(remove(encoded, "\n"), "=");
        TRY(str = code.encode(plain));
        TEST_EQUAL(str, encoded);
        TRY(str = code_nopad.encode(plain));
        TEST_EQUAL(str, stripped);
        TRY(str = code.decode(encoded));
        TEST_EQUAL(str, plain);
        TRY(str = code.decode(stripped));
        TEST_EQUAL(str, plain);
    }

}
