#include "crow/table.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

void test_crow_table_layout() {

    Table tab;
    std::string s;

    TRY(tab.add("Alpha", "Bravo", "Charlie"));
    TRY(tab.add(123, 456, 789));
    TRY(tab.add("Something"));
    TRY(s = tab.str());

    TEST_EQUAL(s,
        "Alpha      Bravo  Charlie\n"
        "123        456    789\n"
        "Something  --     --\n"
    );

    TRY(s = tab.str("M"));

    TEST_EQUAL(s,
        "| Alpha      | Bravo  | Charlie  |\n"
        "| 123        | 456    | 789      |\n"
        "| Something  |        |          |\n"
    );

    TRY(tab.header("X-ray\n°C", "Yankee", "Zulu"));
    TRY(s = tab.str());

    TEST_EQUAL(s,
        "X-ray      Yankee  Zulu\n"
        "°C         ..      ..\n"
        "=========  ======  =======\n"
        "Alpha      Bravo   Charlie\n"
        "123        456     789\n"
        "Something  --      --\n"
    );

    TRY(s = tab.str("B"));

    TEST_EQUAL(s,
        "╭───────────┬────────┬─────────╮\n"
        "│ X-ray     │ Yankee │ Zulu    │\n"
        "│ °C        │        │         │\n"
        "╞═══════════╪════════╪═════════╡\n"
        "│ Alpha     │ Bravo  │ Charlie │\n"
        "│ 123       │ 456    │ 789     │\n"
        "│ Something │        │         │\n"
        "╰───────────┴────────┴─────────╯\n"
    );

    TRY(s = tab.str("M"));

    TEST_EQUAL(s,
        "| X-ray<br>°C  | Yankee  | Zulu     |\n"
        "| -----------  | ------  | -------  |\n"
        "| Alpha        | Bravo   | Charlie  |\n"
        "| 123          | 456     | 789      |\n"
        "| Something    |         |          |\n"
    );

}

void test_crow_table_formatting() {

    Table tab;
    std::string s;

    TRY(tab.header("X-ray\n°C", "Yankee", "Zulu"));
    TRY(tab.format("", "n8", "e2"));
    TRY(tab.add("Alpha", "Bravo", "Charlie"));
    TRY(tab.add(123, 456, 789));
    TRY(tab.add("Something"));
    TRY(s = tab.str());

    TEST_EQUAL(s,
        "X-ray      Yankee    Zulu\n"
        "°C         ..        ..\n"
        "=========  ========  =======\n"
        "Alpha      Bravo     Charlie\n"
        "123        00000456  7.9e2\n"
        "Something  --        --\n"
    );

    TRY(s = tab.str("B"));

    TEST_EQUAL(s,
        "╭───────────┬──────────┬─────────╮\n"
        "│ X-ray     │ Yankee   │ Zulu    │\n"
        "│ °C        │          │         │\n"
        "╞═══════════╪══════════╪═════════╡\n"
        "│ Alpha     │ Bravo    │ Charlie │\n"
        "│ 123       │ 00000456 │ 7.9e2   │\n"
        "│ Something │          │         │\n"
        "╰───────────┴──────────┴─────────╯\n"
    );

    TRY(s = tab.str("M"));

    TEST_EQUAL(s,
        "| X-ray<br>°C  | Yankee    | Zulu     |\n"
        "| -----------  | --------  | -------  |\n"
        "| Alpha        | Bravo     | Charlie  |\n"
        "| 123          | 00000456  | 7.9e2    |\n"
        "| Something    |           |          |\n"
    );

}

void test_crow_table_piecewise() {

    Table tab;
    std::string s;

    TRY(tab.header("X-ray\n°C", "Yankee", "Zulu"));
    TRY(tab.format("", "n8", "e2"));
    TRY(tab << "Alpha");
    TRY(tab << "Bravo");
    TRY(tab << "Charlie");
    TRY(tab << '\n');
    TRY(tab << 123 << 456 << 789 << '\n');
    TRY(tab << "Something" << '\n');
    TRY(s = tab.str());

    TEST_EQUAL(s,
        "X-ray      Yankee    Zulu\n"
        "°C         ..        ..\n"
        "=========  ========  =======\n"
        "Alpha      Bravo     Charlie\n"
        "123        00000456  7.9e2\n"
        "Something  --        --\n"
    );

}
