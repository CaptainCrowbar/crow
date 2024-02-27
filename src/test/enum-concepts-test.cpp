#include "crow/enum.hpp"
#include "crow/unit-test.hpp"
#include <cstdint>
#include <string>

using namespace Crow;

namespace {

    enum Unscoped1 { u1 };
    enum Unscoped2: std::uint64_t { u2 };
    enum class Scoped1 { s1 };
    enum class Scoped2: std::uint64_t { s2 };

}

void test_crow_enum_concepts() {

    TEST(! EnumType<void>);           TEST(! UnscopedEnumType<void>);           TEST(! ScopedEnumType<void>);
    TEST(! EnumType<int>);            TEST(! UnscopedEnumType<int>);            TEST(! ScopedEnumType<int>);
    TEST(! EnumType<std::uint64_t>);  TEST(! UnscopedEnumType<std::uint64_t>);  TEST(! ScopedEnumType<std::uint64_t>);
    TEST(! EnumType<std::string>);    TEST(! UnscopedEnumType<std::string>);    TEST(! ScopedEnumType<std::string>);
    TEST(EnumType<Unscoped1>);        TEST(UnscopedEnumType<Unscoped1>);        TEST(! ScopedEnumType<Unscoped1>);
    TEST(EnumType<Unscoped2>);        TEST(UnscopedEnumType<Unscoped2>);        TEST(! ScopedEnumType<Unscoped2>);
    TEST(EnumType<Scoped1>);          TEST(! UnscopedEnumType<Scoped1>);        TEST(ScopedEnumType<Scoped1>);
    TEST(EnumType<Scoped2>);          TEST(! UnscopedEnumType<Scoped2>);        TEST(ScopedEnumType<Scoped2>);

}
