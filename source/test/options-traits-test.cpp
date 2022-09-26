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

void test_crow_options_concepts() {

    TEST(! AppendableContainer<void>);
    TEST(! AppendableContainer<int>);
    TEST(AppendableContainer<std::string>);
    TEST(AppendableContainer<std::vector<int>>);
    TEST(AppendableContainer<std::set<int>>);

    TEST(! ScalarArgumentType<void>);
    TEST(ScalarArgumentType<bool>);
    TEST(ScalarArgumentType<char>);
    TEST(ScalarArgumentType<signed char>);
    TEST(ScalarArgumentType<unsigned char>);
    TEST(ScalarArgumentType<short>);
    TEST(ScalarArgumentType<unsigned short>);
    TEST(ScalarArgumentType<int>);
    TEST(ScalarArgumentType<unsigned>);
    TEST(ScalarArgumentType<long>);
    TEST(ScalarArgumentType<unsigned long>);
    TEST(ScalarArgumentType<long long>);
    TEST(ScalarArgumentType<unsigned long long>);
    TEST(ScalarArgumentType<float>);
    TEST(ScalarArgumentType<double>);
    TEST(ScalarArgumentType<long double>);
    TEST(ScalarArgumentType<std::string>);
    TEST(ScalarArgumentType<MyEnum>);
    TEST(! ScalarArgumentType<std::vector<int>>);
    TEST(! ScalarArgumentType<std::vector<std::string>>);

    TEST(! ContainerArgumentType<void>);
    TEST(! ContainerArgumentType<int>);
    TEST(! ContainerArgumentType<std::string>);
    TEST(! ContainerArgumentType<MyEnum>);
    TEST(ContainerArgumentType<std::vector<int>>);
    TEST(ContainerArgumentType<std::vector<std::string>>);

    TEST(! ValidArgumentType<void>);
    TEST(ValidArgumentType<int>);
    TEST(ValidArgumentType<std::string>);
    TEST(ValidArgumentType<MyEnum>);
    TEST(ValidArgumentType<std::vector<int>>);
    TEST(ValidArgumentType<std::vector<std::string>>);

}
