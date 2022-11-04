#include "crow/name.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_name_parsing() {

    Name name;

    TEST(name.empty());
    TEST_EQUAL(name.size(), 0u);
    TEST_EQUAL(format_object(name), "[]");

    TRY(name = "abc def ghi 123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[abc,def,ghi,123]");
    TRY(name = "abc-def-ghi-123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[abc,def,ghi,123]");
    TRY(name = "abc_def_ghi_123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[abc,def,ghi,123]");
    TRY(name = "Abc def ghi 123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,def,ghi,123]");
    TRY(name = "Abc_def_ghi_123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,def,ghi,123]");
    TRY(name = "abcDefGhi123");     TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[abc,Def,Ghi,123]");
    TRY(name = "AbcDefGhi123");     TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,Def,Ghi,123]");
    TRY(name = "Abc Def Ghi 123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,Def,Ghi,123]");
    TRY(name = "Abc-Def-Ghi-123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,Def,Ghi,123]");
    TRY(name = "Abc_Def_Ghi_123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,Def,Ghi,123]");
    TRY(name = "ABC DEF GHI 123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[ABC,DEF,GHI,123]");
    TRY(name = "ABC-DEF-GHI-123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[ABC,DEF,GHI,123]");
    TRY(name = "ABC_DEF_GHI_123");  TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[ABC,DEF,GHI,123]");
    TRY(name = "ABCDefGhi123");     TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[ABC,Def,Ghi,123]");
    TRY(name = "AbcDEFGhi123");     TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,DEF,Ghi,123]");
    TRY(name = "AbcDefGHI123");     TEST_EQUAL(name.size(), 4u);  TEST_EQUAL(format_object(name), "[Abc,Def,GHI,123]");

    TRY(name = Name("Don't panic", Name::apos))
    TEST_EQUAL(name.size(), 2u);
    TEST_EQUAL(format_object(name), "[Don't,panic]");

}

void test_crow_name_formatting() {

    Name name;

    TEST_EQUAL(name.str(Name::lower),        "");
    TEST_EQUAL(name.str(Name::kebab),        "");
    TEST_EQUAL(name.str(Name::snake),        "");
    TEST_EQUAL(name.str(Name::sentence),     "");
    TEST_EQUAL(name.str(Name::stroustrup),   "");
    TEST_EQUAL(name.str(Name::camel),        "");
    TEST_EQUAL(name.str(Name::pascal),       "");
    TEST_EQUAL(name.str(Name::title),        "");
    TEST_EQUAL(name.str(Name::title_kebab),  "");
    TEST_EQUAL(name.str(Name::title_snake),  "");
    TEST_EQUAL(name.str(Name::upper),        "");
    TEST_EQUAL(name.str(Name::upper_kebab),  "");
    TEST_EQUAL(name.str(Name::upper_snake),  "");

    TRY(name = "Aperture Science Innovators 1952");

    TEST_EQUAL(name.str(Name::lower),        "aperture science innovators 1952");
    TEST_EQUAL(name.str(Name::kebab),        "aperture-science-innovators-1952");
    TEST_EQUAL(name.str(Name::snake),        "aperture_science_innovators_1952");
    TEST_EQUAL(name.str(Name::sentence),     "Aperture science innovators 1952");
    TEST_EQUAL(name.str(Name::stroustrup),   "Aperture_science_innovators_1952");
    TEST_EQUAL(name.str(Name::camel),        "apertureScienceInnovators1952");
    TEST_EQUAL(name.str(Name::pascal),       "ApertureScienceInnovators1952");
    TEST_EQUAL(name.str(Name::title),        "Aperture Science Innovators 1952");
    TEST_EQUAL(name.str(Name::title_kebab),  "Aperture-Science-Innovators-1952");
    TEST_EQUAL(name.str(Name::title_snake),  "Aperture_Science_Innovators_1952");
    TEST_EQUAL(name.str(Name::upper),        "APERTURE SCIENCE INNOVATORS 1952");
    TEST_EQUAL(name.str(Name::upper_kebab),  "APERTURE-SCIENCE-INNOVATORS-1952");
    TEST_EQUAL(name.str(Name::upper_snake),  "APERTURE_SCIENCE_INNOVATORS_1952");

    TRY(name = "THE LADY OR THE TIGER");

    TEST_EQUAL(name.str(Name::lower,        Name::stop),  "the lady or the tiger");
    TEST_EQUAL(name.str(Name::kebab,        Name::stop),  "the-lady-or-the-tiger");
    TEST_EQUAL(name.str(Name::snake,        Name::stop),  "the_lady_or_the_tiger");
    TEST_EQUAL(name.str(Name::sentence,     Name::stop),  "The lady or the tiger");
    TEST_EQUAL(name.str(Name::stroustrup,   Name::stop),  "The_lady_or_the_tiger");
    TEST_EQUAL(name.str(Name::camel,        Name::stop),  "theLadyortheTiger");
    TEST_EQUAL(name.str(Name::pascal,       Name::stop),  "TheLadyortheTiger");
    TEST_EQUAL(name.str(Name::title,        Name::stop),  "The Lady or the Tiger");
    TEST_EQUAL(name.str(Name::title_kebab,  Name::stop),  "The-Lady-or-the-Tiger");
    TEST_EQUAL(name.str(Name::title_snake,  Name::stop),  "The_Lady_or_the_Tiger");
    TEST_EQUAL(name.str(Name::upper,        Name::stop),  "THE LADY OR THE TIGER");
    TEST_EQUAL(name.str(Name::upper_kebab,  Name::stop),  "THE-LADY-OR-THE-TIGER");
    TEST_EQUAL(name.str(Name::upper_snake,  Name::stop),  "THE_LADY_OR_THE_TIGER");

}
