#include "crow/enum.hpp"
#include "crow/unit-test.hpp"
#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace Crow;

namespace {

    CROW_ENUM_UNSCOPED(UnscopedUnsigned, unsigned,
        uu_alpha,
        uu_bravo,
        uu_charlie
    )

    CROW_ENUM_UNSCOPED(UnscopedSigned, std::int32_t,
        us_alpha = 1,
        us_bravo,
        us_charlie = -1,
        us_delta = 2
    )

    CROW_ENUM_SCOPED(ScopedUnsigned, unsigned,
        alpha,
        bravo,
        charlie
    )

    CROW_ENUM_SCOPED(ScopedSigned, std::int32_t,
        alpha = 1,
        bravo,
        charlie = -1,
        delta = 2
    )

}

void test_crow_enum_definitions() {

    TEST_TYPE(std::underlying_type_t<UnscopedUnsigned>,  unsigned);
    TEST_TYPE(std::underlying_type_t<UnscopedSigned>,    std::int32_t);
    TEST_TYPE(std::underlying_type_t<ScopedUnsigned>,    unsigned);
    TEST_TYPE(std::underlying_type_t<ScopedSigned>,      std::int32_t);

    TEST_EQUAL(static_cast<unsigned>(uu_alpha),                   0u);
    TEST_EQUAL(static_cast<unsigned>(uu_bravo),                   1u);
    TEST_EQUAL(static_cast<unsigned>(uu_charlie),                 2u);
    TEST_EQUAL(static_cast<std::int32_t>(us_alpha),               1);
    TEST_EQUAL(static_cast<std::int32_t>(us_bravo),               2);
    TEST_EQUAL(static_cast<std::int32_t>(us_charlie),             -1);
    TEST_EQUAL(static_cast<std::int32_t>(us_delta),               2);
    TEST_EQUAL(static_cast<unsigned>(ScopedUnsigned::alpha),      0u);
    TEST_EQUAL(static_cast<unsigned>(ScopedUnsigned::bravo),      1u);
    TEST_EQUAL(static_cast<unsigned>(ScopedUnsigned::charlie),    2u);
    TEST_EQUAL(static_cast<std::int32_t>(ScopedSigned::alpha),    1);
    TEST_EQUAL(static_cast<std::int32_t>(ScopedSigned::bravo),    2);
    TEST_EQUAL(static_cast<std::int32_t>(ScopedSigned::charlie),  -1);
    TEST_EQUAL(static_cast<std::int32_t>(ScopedSigned::delta),    2);

}

void test_crow_enum_value_name_vector() {

    std::vector<std::pair<UnscopedUnsigned, std::string>>  uuvec;
    std::vector<std::pair<UnscopedSigned, std::string>>    usvec;
    std::vector<std::pair<ScopedUnsigned, std::string>>    suvec;
    std::vector<std::pair<ScopedSigned, std::string>>      ssvec;

    TRY(uuvec = enum_value_name_vector(UnscopedUnsigned()));
    TEST_EQUAL(uuvec.size(), 3u);
    uuvec.resize(3);

    TEST_EQUAL(static_cast<unsigned>(uuvec[0].first), 0u);  TEST_EQUAL(uuvec[0].second, "uu_alpha");
    TEST_EQUAL(static_cast<unsigned>(uuvec[1].first), 1u);  TEST_EQUAL(uuvec[1].second, "uu_bravo");
    TEST_EQUAL(static_cast<unsigned>(uuvec[2].first), 2u);  TEST_EQUAL(uuvec[2].second, "uu_charlie");

    TRY(usvec = enum_value_name_vector(UnscopedSigned()));
    TEST_EQUAL(usvec.size(), 4u);
    usvec.resize(4);

    TEST_EQUAL(static_cast<std::int32_t>(usvec[0].first), 1);   TEST_EQUAL(usvec[0].second, "us_alpha");
    TEST_EQUAL(static_cast<std::int32_t>(usvec[1].first), 2);   TEST_EQUAL(usvec[1].second, "us_bravo");
    TEST_EQUAL(static_cast<std::int32_t>(usvec[2].first), -1);  TEST_EQUAL(usvec[2].second, "us_charlie");
    TEST_EQUAL(static_cast<std::int32_t>(usvec[3].first), 2);   TEST_EQUAL(usvec[3].second, "us_delta");

    TRY(suvec = enum_value_name_vector(ScopedUnsigned()));
    TEST_EQUAL(suvec.size(), 3u);
    suvec.resize(3);

    TEST_EQUAL(static_cast<unsigned>(suvec[0].first), 0u);  TEST_EQUAL(suvec[0].second, "alpha");
    TEST_EQUAL(static_cast<unsigned>(suvec[1].first), 1u);  TEST_EQUAL(suvec[1].second, "bravo");
    TEST_EQUAL(static_cast<unsigned>(suvec[2].first), 2u);  TEST_EQUAL(suvec[2].second, "charlie");

    TRY(ssvec = enum_value_name_vector(ScopedSigned()));
    TEST_EQUAL(ssvec.size(), 4u);
    ssvec.resize(4);

    TEST_EQUAL(static_cast<std::int32_t>(ssvec[0].first), 1);   TEST_EQUAL(ssvec[0].second, "alpha");
    TEST_EQUAL(static_cast<std::int32_t>(ssvec[1].first), 2);   TEST_EQUAL(ssvec[1].second, "bravo");
    TEST_EQUAL(static_cast<std::int32_t>(ssvec[2].first), -1);  TEST_EQUAL(ssvec[2].second, "charlie");
    TEST_EQUAL(static_cast<std::int32_t>(ssvec[3].first), 2);   TEST_EQUAL(ssvec[3].second, "delta");

}

void test_crow_enum_value_name_map() {

    std::unordered_map<UnscopedUnsigned, std::string>  uumap;
    std::unordered_map<UnscopedSigned, std::string>    usmap;
    std::unordered_map<ScopedUnsigned, std::string>    sumap;
    std::unordered_map<ScopedSigned, std::string>      ssmap;

    TRY(uumap = enum_value_name_map(UnscopedUnsigned()));
    TEST_EQUAL(uumap.size(),       3u);
    TEST_EQUAL(uumap[uu_alpha],    "uu_alpha");
    TEST_EQUAL(uumap[uu_bravo],    "uu_bravo");
    TEST_EQUAL(uumap[uu_charlie],  "uu_charlie");

    TRY(usmap = enum_value_name_map(UnscopedSigned()));
    TEST_EQUAL(usmap.size(),       3u);
    TEST_EQUAL(usmap[us_alpha],    "us_alpha");
    TEST_EQUAL(usmap[us_bravo],    "us_bravo");
    TEST_EQUAL(usmap[us_charlie],  "us_charlie");

    TRY(sumap = enum_value_name_map(ScopedUnsigned()));
    TEST_EQUAL(sumap.size(),                    3u);
    TEST_EQUAL(sumap[ScopedUnsigned::alpha],    "alpha");
    TEST_EQUAL(sumap[ScopedUnsigned::bravo],    "bravo");
    TEST_EQUAL(sumap[ScopedUnsigned::charlie],  "charlie");

    TRY(ssmap = enum_value_name_map(ScopedSigned()));
    TEST_EQUAL(ssmap.size(),                  3u);
    TEST_EQUAL(ssmap[ScopedSigned::alpha],    "alpha");
    TEST_EQUAL(ssmap[ScopedSigned::bravo],    "bravo");
    TEST_EQUAL(ssmap[ScopedSigned::charlie],  "charlie");

}

void test_crow_enum_name_value_map() {

    std::unordered_map<std::string, UnscopedUnsigned>  uumap;
    std::unordered_map<std::string, UnscopedSigned>    usmap;
    std::unordered_map<std::string, ScopedUnsigned>    sumap;
    std::unordered_map<std::string, ScopedSigned>      ssmap;

    TRY(uumap = enum_name_value_map(UnscopedUnsigned()));
    TEST_EQUAL(uumap.size(),                   3u);
    TEST_EQUAL(static_cast<unsigned>(uumap["uu_alpha"]),    0u);
    TEST_EQUAL(static_cast<unsigned>(uumap["uu_bravo"]),    1u);
    TEST_EQUAL(static_cast<unsigned>(uumap["uu_charlie"]),  2u);

    TRY(usmap = enum_name_value_map(UnscopedSigned()));
    TEST_EQUAL(usmap.size(),                       4u);
    TEST_EQUAL(static_cast<std::int32_t>(usmap["us_alpha"]),    1);
    TEST_EQUAL(static_cast<std::int32_t>(usmap["us_bravo"]),    2);
    TEST_EQUAL(static_cast<std::int32_t>(usmap["us_charlie"]),  -1);
    TEST_EQUAL(static_cast<std::int32_t>(usmap["us_delta"]),    2);

    TRY(sumap = enum_name_value_map(ScopedUnsigned()));
    TEST_EQUAL(sumap.size(),                3u);
    TEST_EQUAL(static_cast<unsigned>(sumap["alpha"]),    0u);
    TEST_EQUAL(static_cast<unsigned>(sumap["bravo"]),    1u);
    TEST_EQUAL(static_cast<unsigned>(sumap["charlie"]),  2u);

    TRY(ssmap = enum_name_value_map(ScopedSigned()));
    TEST_EQUAL(ssmap.size(),                    4u);
    TEST_EQUAL(static_cast<std::int32_t>(ssmap["alpha"]),    1);
    TEST_EQUAL(static_cast<std::int32_t>(ssmap["bravo"]),    2);
    TEST_EQUAL(static_cast<std::int32_t>(ssmap["charlie"]),  -1);
    TEST_EQUAL(static_cast<std::int32_t>(ssmap["delta"]),    2);

}

void test_crow_enum_name_to_value() {

    UnscopedUnsigned uu;
    UnscopedSigned us;
    ScopedUnsigned su;
    ScopedSigned ss;

    TEST(enum_value("uu_alpha", uu));    TEST_EQUAL(static_cast<unsigned>(uu),      0u);
    TEST(enum_value("uu_bravo", uu));    TEST_EQUAL(static_cast<unsigned>(uu),      1u);
    TEST(enum_value("uu_charlie", uu));  TEST_EQUAL(static_cast<unsigned>(uu),      2u);
    TEST(enum_value("us_alpha", us));    TEST_EQUAL(static_cast<std::int32_t>(us),  1);
    TEST(enum_value("us_bravo", us));    TEST_EQUAL(static_cast<std::int32_t>(us),  2);
    TEST(enum_value("us_charlie", us));  TEST_EQUAL(static_cast<std::int32_t>(us),  -1);
    TEST(enum_value("us_delta", us));    TEST_EQUAL(static_cast<std::int32_t>(us),  2);
    TEST(enum_value("alpha", su));       TEST_EQUAL(static_cast<unsigned>(su),      0u);
    TEST(enum_value("bravo", su));       TEST_EQUAL(static_cast<unsigned>(su),      1u);
    TEST(enum_value("charlie", su));     TEST_EQUAL(static_cast<unsigned>(su),      2u);
    TEST(enum_value("alpha", ss));       TEST_EQUAL(static_cast<std::int32_t>(ss),  1);
    TEST(enum_value("bravo", ss));       TEST_EQUAL(static_cast<std::int32_t>(ss),  2);
    TEST(enum_value("charlie", ss));     TEST_EQUAL(static_cast<std::int32_t>(ss),  -1);
    TEST(enum_value("delta", ss));       TEST_EQUAL(static_cast<std::int32_t>(ss),  2);

    TEST(! enum_value("nonesuch", uu));
    TEST(! enum_value("nonesuch", us));
    TEST(! enum_value("nonesuch", su));
    TEST(! enum_value("nonesuch", ss));

}

void test_crow_enum_value_to_name() {

    static const auto via_stream = [] (const auto& x) {
        std::ostringstream out;
        out << x;
        return out.str();
    };

    TEST_EQUAL(to_string(uu_alpha),                 "uu_alpha");
    TEST_EQUAL(to_string(uu_bravo),                 "uu_bravo");
    TEST_EQUAL(to_string(uu_charlie),               "uu_charlie");
    TEST_EQUAL(to_string(us_alpha),                 "us_alpha");
    TEST_EQUAL(to_string(us_bravo),                 "us_bravo");
    TEST_EQUAL(to_string(us_charlie),               "us_charlie");
    TEST_EQUAL(to_string(us_delta),                 "us_bravo"); // Duplicate value
    TEST_EQUAL(to_string(ScopedUnsigned::alpha),    "alpha");
    TEST_EQUAL(to_string(ScopedUnsigned::bravo),    "bravo");
    TEST_EQUAL(to_string(ScopedUnsigned::charlie),  "charlie");
    TEST_EQUAL(to_string(ScopedSigned::alpha),      "alpha");
    TEST_EQUAL(to_string(ScopedSigned::bravo),      "bravo");
    TEST_EQUAL(to_string(ScopedSigned::charlie),    "charlie");
    TEST_EQUAL(to_string(ScopedSigned::delta),      "bravo"); // Duplicate value

    TEST_EQUAL(via_stream(uu_alpha),                 "uu_alpha");
    TEST_EQUAL(via_stream(uu_bravo),                 "uu_bravo");
    TEST_EQUAL(via_stream(uu_charlie),               "uu_charlie");
    TEST_EQUAL(via_stream(us_alpha),                 "us_alpha");
    TEST_EQUAL(via_stream(us_bravo),                 "us_bravo");
    TEST_EQUAL(via_stream(us_charlie),               "us_charlie");
    TEST_EQUAL(via_stream(us_delta),                 "us_bravo"); // Duplicate value
    TEST_EQUAL(via_stream(ScopedUnsigned::alpha),    "alpha");
    TEST_EQUAL(via_stream(ScopedUnsigned::bravo),    "bravo");
    TEST_EQUAL(via_stream(ScopedUnsigned::charlie),  "charlie");
    TEST_EQUAL(via_stream(ScopedSigned::alpha),      "alpha");
    TEST_EQUAL(via_stream(ScopedSigned::bravo),      "bravo");
    TEST_EQUAL(via_stream(ScopedSigned::charlie),    "charlie");
    TEST_EQUAL(via_stream(ScopedSigned::delta),      "bravo"); // Duplicate value

}
