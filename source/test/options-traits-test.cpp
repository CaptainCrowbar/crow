#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Detail;

CROW_ENUM_CLASS(MyEnum, int, 0, alpha, bravo, charlie)

void test_crow_options_traits() {

    TEST(! HasBackInserter<void>::value);
    TEST(! HasBackInserter<int>::value);
    TEST(HasBackInserter<std::string>::value);
    TEST(HasBackInserter<std::vector<int>>::value);
    TEST(HasBackInserter<std::set<int>>::value);

    TEST(! is_scalar_argument_type<void>);
    TEST(is_scalar_argument_type<bool>);
    TEST(is_scalar_argument_type<char>);
    TEST(is_scalar_argument_type<signed char>);
    TEST(is_scalar_argument_type<unsigned char>);
    TEST(is_scalar_argument_type<short>);
    TEST(is_scalar_argument_type<unsigned short>);
    TEST(is_scalar_argument_type<int>);
    TEST(is_scalar_argument_type<unsigned>);
    TEST(is_scalar_argument_type<long>);
    TEST(is_scalar_argument_type<unsigned long>);
    TEST(is_scalar_argument_type<long long>);
    TEST(is_scalar_argument_type<unsigned long long>);
    TEST(is_scalar_argument_type<float>);
    TEST(is_scalar_argument_type<double>);
    TEST(is_scalar_argument_type<long double>);
    TEST(is_scalar_argument_type<std::string>);
    TEST(is_scalar_argument_type<MyEnum>);
    TEST(! is_scalar_argument_type<std::vector<int>>);
    TEST(! is_scalar_argument_type<std::vector<std::string>>);

    TEST(! is_container_argument_type<void>);
    TEST(! is_container_argument_type<int>);
    TEST(! is_container_argument_type<std::string>);
    TEST(! is_container_argument_type<MyEnum>);
    TEST(is_container_argument_type<std::vector<int>>);
    TEST(is_container_argument_type<std::vector<std::string>>);

    TEST(! is_valid_argument_type<void>);
    TEST(is_valid_argument_type<int>);
    TEST(is_valid_argument_type<std::string>);
    TEST(is_valid_argument_type<MyEnum>);
    TEST(is_valid_argument_type<std::vector<int>>);
    TEST(is_valid_argument_type<std::vector<std::string>>);

}
