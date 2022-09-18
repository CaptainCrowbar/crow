#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <string>
#include <type_traits>
#include <vector>

CROW_ENUM(Etype, unsigned, 1, alpha, bravo, charlie)
CROW_ENUM_CLASS(Eclass, long long, -1, xray, yankee, zulu)

using namespace Crow;

void test_crow_format_enum_definition() {

    std::vector<std::string> names;
    std::vector<Etype> etvec;
    std::vector<Eclass> ecvec;
    Etype et = {};
    Eclass ec = {};

    TEST(std::is_enum_v<Etype>);
    TEST(std::is_enum_v<Eclass>);

    TEST_TYPE(std::underlying_type_t<Etype>, unsigned);
    TEST_TYPE(std::underlying_type_t<Eclass>, long long);

    TEST_EQUAL(int(alpha),           1);
    TEST_EQUAL(int(bravo),           2);
    TEST_EQUAL(int(charlie),         3);
    TEST_EQUAL(int(Eclass::xray),    -1);
    TEST_EQUAL(int(Eclass::yankee),  0);
    TEST_EQUAL(int(Eclass::zulu),    1);

    TEST_EQUAL(min_enum_value(Etype()), alpha);          TEST_EQUAL(int(min_enum_value(Etype())), 1);
    TEST_EQUAL(max_enum_value(Etype()), charlie);        TEST_EQUAL(int(max_enum_value(Etype())), 3);
    TEST_EQUAL(min_enum_value(Eclass()), Eclass::xray);  TEST_EQUAL(int(min_enum_value(Eclass())), -1);
    TEST_EQUAL(max_enum_value(Eclass()), Eclass::zulu);  TEST_EQUAL(int(max_enum_value(Eclass())), 1);

    TRY(names = list_enum_names(Etype()));
    TEST_EQUAL(format_range(names), "[alpha,bravo,charlie]");
    TRY(etvec = list_enum_values(Etype()));
    TEST_EQUAL(etvec.size(), 3u);
    REQUIRE(etvec.size() == 3);
    TEST_EQUAL(etvec[0], alpha);
    TEST_EQUAL(etvec[1], bravo);
    TEST_EQUAL(etvec[2], charlie);

    TRY(names = list_enum_names(Eclass()));
    TEST_EQUAL(format_range(names), "[xray,yankee,zulu]");
    TRY(ecvec = list_enum_values(Eclass()));
    TEST_EQUAL(ecvec.size(), 3u);
    REQUIRE(ecvec.size() == 3);
    TEST_EQUAL(ecvec[0], Eclass::xray);
    TEST_EQUAL(ecvec[1], Eclass::yankee);
    TEST_EQUAL(ecvec[2], Eclass::zulu);

    TEST_EQUAL(to_string(alpha),           "alpha");
    TEST_EQUAL(to_string(bravo),           "bravo");
    TEST_EQUAL(to_string(charlie),         "charlie");
    TEST_EQUAL(to_string(Etype(0)),        "0");
    TEST_EQUAL(to_string(Eclass::xray),    "xray");
    TEST_EQUAL(to_string(Eclass::yankee),  "yankee");
    TEST_EQUAL(to_string(Eclass::zulu),    "zulu");
    TEST_EQUAL(to_string(Eclass(-2)),      "-2");

    TEST(parse_enum("alpha", et));    TEST_EQUAL(et, alpha);
    TEST(parse_enum("bravo", et));    TEST_EQUAL(et, bravo);
    TEST(parse_enum("charlie", et));  TEST_EQUAL(et, charlie);
    TEST(parse_enum("xray", ec));     TEST_EQUAL(ec, Eclass::xray);
    TEST(parse_enum("yankee", ec));   TEST_EQUAL(ec, Eclass::yankee);
    TEST(parse_enum("zulu", ec));     TEST_EQUAL(ec, Eclass::zulu);

    TEST(! parse_enum("xray", et));
    TEST(! parse_enum("alpha", ec));

}

enum class Mask: uint16_t {
    z = 0,
    a = 1,
    b = 2,
    c = 4,
};

class C {
public:
    enum class Mtype: uint16_t {
        z = 0,
        a = 1,
        b = 2,
        c = 4,
    };
};

CROW_BITMASK_OPERATORS(Mask)
CROW_BITMASK_OPERATORS(C::Mtype)

inline std::ostream& operator<<(std::ostream& out, Mask m) { return out << "M" << uint16_t(m); }
inline std::ostream& operator<<(std::ostream& out, C::Mtype m) { return out << "CM" << uint16_t(m); }

void test_crow_format_enum_bitmask_operators() {

    Mask m = {};
    C::Mtype cm = {};

    TEST(! m);
    TEST_EQUAL(~ m, Mask(65535));

    TEST(! cm);
    TEST_EQUAL(~ cm, C::Mtype(65535));

    m = Mask::a & Mask::c;  TEST_EQUAL(m, Mask::z);
    m = Mask::a | Mask::c;  TEST_EQUAL(m, Mask(5));
    m = Mask::a ^ Mask::c;  TEST_EQUAL(m, Mask(5));
    m = Mask(7) & Mask::b;  TEST_EQUAL(m, Mask::b);
    m = Mask(7) | Mask::b;  TEST_EQUAL(m, Mask(7));
    m = Mask(7) ^ Mask::b;  TEST_EQUAL(m, Mask(5));

    m = Mask::a;  m &= Mask::c;  TEST_EQUAL(m, Mask::z);
    m = Mask::a;  m |= Mask::c;  TEST_EQUAL(m, Mask(5));
    m = Mask::a;  m ^= Mask::c;  TEST_EQUAL(m, Mask(5));
    m = Mask(7);  m &= Mask::b;  TEST_EQUAL(m, Mask::b);
    m = Mask(7);  m |= Mask::b;  TEST_EQUAL(m, Mask(7));
    m = Mask(7);  m ^= Mask::b;  TEST_EQUAL(m, Mask(5));

    cm = C::Mtype::a & C::Mtype::c;  TEST_EQUAL(cm, C::Mtype::z);
    cm = C::Mtype::a | C::Mtype::c;  TEST_EQUAL(cm, C::Mtype(5));
    cm = C::Mtype::a ^ C::Mtype::c;  TEST_EQUAL(cm, C::Mtype(5));
    cm = C::Mtype(7) & C::Mtype::b;  TEST_EQUAL(cm, C::Mtype::b);
    cm = C::Mtype(7) | C::Mtype::b;  TEST_EQUAL(cm, C::Mtype(7));
    cm = C::Mtype(7) ^ C::Mtype::b;  TEST_EQUAL(cm, C::Mtype(5));

    cm = C::Mtype::a;  cm &= C::Mtype::c;  TEST_EQUAL(cm, C::Mtype::z);
    cm = C::Mtype::a;  cm |= C::Mtype::c;  TEST_EQUAL(cm, C::Mtype(5));
    cm = C::Mtype::a;  cm ^= C::Mtype::c;  TEST_EQUAL(cm, C::Mtype(5));
    cm = C::Mtype(7);  cm &= C::Mtype::b;  TEST_EQUAL(cm, C::Mtype::b);
    cm = C::Mtype(7);  cm |= C::Mtype::b;  TEST_EQUAL(cm, C::Mtype(7));
    cm = C::Mtype(7);  cm ^= C::Mtype::b;  TEST_EQUAL(cm, C::Mtype(5));

}
