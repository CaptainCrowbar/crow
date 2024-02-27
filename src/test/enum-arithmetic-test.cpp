#include "crow/enum.hpp"
#include "crow/unit-test.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace Crow;

namespace {

    CROW_ENUM_BITMASK(ArithmeticMask, std::uint32_t,
        none,
        alpha    = 1u << 2,
        bravo    = alpha << 2,
        charlie,
        delta    = alpha | bravo | charlie,
        echo     = delta | 0x1000,
        foxtrot  = echo + 100,
    )

}

void test_crow_enum_arithmetic_values() {

    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::none),     0u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::alpha),    4u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::bravo),    0x10u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::charlie),  0x11u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::delta),    0x15u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::echo),     0x1015u);
    TEST_EQUAL(static_cast<std::uint32_t>(ArithmeticMask::foxtrot),  0x1079u);

}

void test_crow_enum_arithmetic_functions() {

    std::vector<std::pair<ArithmeticMask, std::string>> nv_vec;
    TRY(nv_vec = enum_value_name_vector(ArithmeticMask()));
    TEST_EQUAL(nv_vec.size(), 7u);
    nv_vec.resize(7);

    TEST_EQUAL(static_cast<unsigned>(nv_vec[0].first), 0u);       TEST_EQUAL(nv_vec[0].second, "none");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[1].first), 4u);       TEST_EQUAL(nv_vec[1].second, "alpha");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[2].first), 0x10u);    TEST_EQUAL(nv_vec[2].second, "bravo");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[3].first), 0x11u);    TEST_EQUAL(nv_vec[3].second, "charlie");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[4].first), 0x15u);    TEST_EQUAL(nv_vec[4].second, "delta");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[5].first), 0x1015u);  TEST_EQUAL(nv_vec[5].second, "echo");
    TEST_EQUAL(static_cast<unsigned>(nv_vec[6].first), 0x1079u);  TEST_EQUAL(nv_vec[6].second, "foxtrot");

    std::unordered_map<ArithmeticMask, std::string> vn_map;
    TRY(vn_map = enum_value_name_map(ArithmeticMask()));

    TEST_EQUAL(vn_map.size(),                    7u);
    TEST_EQUAL(vn_map[ArithmeticMask::none],     "none");
    TEST_EQUAL(vn_map[ArithmeticMask::alpha],    "alpha");
    TEST_EQUAL(vn_map[ArithmeticMask::bravo],    "bravo");
    TEST_EQUAL(vn_map[ArithmeticMask::charlie],  "charlie");
    TEST_EQUAL(vn_map[ArithmeticMask::delta],    "delta");
    TEST_EQUAL(vn_map[ArithmeticMask::echo],     "echo");
    TEST_EQUAL(vn_map[ArithmeticMask::foxtrot],  "foxtrot");

    std::unordered_map<std::string, ArithmeticMask> nv_map;
    TRY(nv_map = enum_name_value_map(ArithmeticMask()));

    TEST_EQUAL(nv_map.size(),                             7u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["none"]),     0u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["alpha"]),    4u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["bravo"]),    0x10u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["charlie"]),  0x11u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["delta"]),    0x15u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["echo"]),     0x1015u);
    TEST_EQUAL(static_cast<unsigned>(nv_map["foxtrot"]),  0x1079u);

    ArithmeticMask value = {};

    TEST(enum_value("none", value));     TEST_EQUAL(static_cast<unsigned>(value), 0u);
    TEST(enum_value("alpha", value));    TEST_EQUAL(static_cast<unsigned>(value), 4u);
    TEST(enum_value("bravo", value));    TEST_EQUAL(static_cast<unsigned>(value), 0x10u);
    TEST(enum_value("charlie", value));  TEST_EQUAL(static_cast<unsigned>(value), 0x11u);
    TEST(enum_value("delta", value));    TEST_EQUAL(static_cast<unsigned>(value), 0x15u);
    TEST(enum_value("echo", value));     TEST_EQUAL(static_cast<unsigned>(value), 0x1015u);
    TEST(enum_value("foxtrot", value));  TEST_EQUAL(static_cast<unsigned>(value), 0x1079u);

    TEST(! enum_value("nonesuch", value));

}
