#include "crow/options.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Detail;

CROW_ENUM_CLASS(MyEnum, int, 0, alpha, bravo, charlie)

void test_crow_options_concepts() {

    TEST(! ScalarOptionType<void>);
    TEST(ScalarOptionType<bool>);
    TEST(ScalarOptionType<char>);
    TEST(ScalarOptionType<signed char>);
    TEST(ScalarOptionType<unsigned char>);
    TEST(ScalarOptionType<short>);
    TEST(ScalarOptionType<unsigned short>);
    TEST(ScalarOptionType<int>);
    TEST(ScalarOptionType<unsigned>);
    TEST(ScalarOptionType<long>);
    TEST(ScalarOptionType<unsigned long>);
    TEST(ScalarOptionType<long long>);
    TEST(ScalarOptionType<unsigned long long>);
    TEST(ScalarOptionType<float>);
    TEST(ScalarOptionType<double>);
    TEST(ScalarOptionType<long double>);
    TEST(ScalarOptionType<std::string>);
    TEST(ScalarOptionType<MyEnum>);
    TEST(! ScalarOptionType<std::vector<int>>);
    TEST(! ScalarOptionType<std::vector<std::string>>);

    TEST(! ContainerOptionType<void>);
    TEST(! ContainerOptionType<int>);
    TEST(! ContainerOptionType<std::string>);
    TEST(! ContainerOptionType<MyEnum>);
    TEST(ContainerOptionType<std::vector<int>>);
    TEST(ContainerOptionType<std::vector<std::string>>);

    TEST(! OptionArgumentType<void>);
    TEST(OptionArgumentType<int>);
    TEST(OptionArgumentType<std::string>);
    TEST(OptionArgumentType<MyEnum>);
    TEST(OptionArgumentType<std::vector<int>>);
    TEST(OptionArgumentType<std::vector<std::string>>);

}
