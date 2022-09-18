#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <vector>

using namespace Crow;

void test_crow_string_type_name() {

    TEST_MATCH(type_name<int>(),               "^(signed )?int$");
    TEST_EQUAL(type_name<double>(),            "double");
    TEST_MATCH(type_name<std::string>(),       "std::(\\w+::)*(basic_)string\\b");
    TEST_MATCH(type_name<std::vector<int>>(),  "std::(\\w+::)*vector<int\\b");

}
