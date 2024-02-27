#include "crow/options.hpp"
#include "crow/path.hpp"
#include "crow/unit-test.hpp"
#include "crow/uri.hpp"
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Detail;

namespace {

    CROW_ENUM_SCOPED(MyEnum, int, alpha, bravo, charlie)

}

void test_crow_options_traits_concepts() {

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
    TEST(ScalarOptionType<Path>);
    TEST(ScalarOptionType<Uri>);
    TEST(! ScalarOptionType<std::vector<int>>);
    TEST(! ScalarOptionType<std::vector<std::string>>);

    TEST(! ContainerOptionType<void>);
    TEST(! ContainerOptionType<int>);
    TEST(! ContainerOptionType<std::string>);
    TEST(! ContainerOptionType<MyEnum>);
    TEST(! ContainerOptionType<Path>);
    TEST(! ContainerOptionType<Uri>);
    TEST(ContainerOptionType<std::vector<int>>);
    TEST(ContainerOptionType<std::vector<std::string>>);

    TEST(! OptionArgumentType<void>);
    TEST(OptionArgumentType<int>);
    TEST(OptionArgumentType<std::string>);
    TEST(OptionArgumentType<MyEnum>);
    TEST(OptionArgumentType<Path>);
    TEST(OptionArgumentType<Uri>);
    TEST(OptionArgumentType<std::vector<int>>);
    TEST(OptionArgumentType<std::vector<std::string>>);

}
