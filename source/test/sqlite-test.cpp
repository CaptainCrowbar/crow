#include "crow/sqlite.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace Crow;
using namespace Crow::Sqlite;
using namespace std::literals;

void test_crow_sqlite_connection() {

    std::cout << "... Sqlite compile version: " << format_range(compile_version()) << "\n";
    std::cout << "... Sqlite runtime version: " << format_range(runtime_version()) << "\n";

    Connect dbc;
    Query query;
    Result result;

    TEST_THROW_MESSAGE(Connect("NoSuchFile"), SqliteError,
        "Sqlite error 14 in sqlite3_open_v2(): unable to open database file");

    TRY(dbc = Connect(Mode::memory));
    TRY(dbc.run(
        "create table tango("
            "number integer not null primary key,"
            "name text not null"
        ")"
    ));

    int count = 0, index = 0;
    std::string name;

    TRY(dbc.set_timeout(100ms));
    TRY(query = dbc.query("insert into tango values($a,$b)"));

    TRY(count = query.parameters());     TEST_EQUAL(count, 2);
    TRY(index = query.get_index("$a"));  TEST_EQUAL(index, 1);
    TRY(index = query.get_index("$b"));  TEST_EQUAL(index, 2);
    TRY(index = query.get_index("$c"));  TEST_EQUAL(index, 0);
    TRY(name = query.get_name(0));       TEST_EQUAL(name, "");
    TRY(name = query.get_name(1));       TEST_EQUAL(name, "$a");
    TRY(name = query.get_name(2));       TEST_EQUAL(name, "$b");
    TRY(name = query.get_name(3));       TEST_EQUAL(name, "");

    TEST_THROW_MESSAGE(query.run(), InvalidArgument,
        "Not all parameters set in query (0 out of 2 set)");
    TEST_THROW_MESSAGE(query.bind(42), InvalidArgument,
        "Wrong number of parameters in query: found 1, expected 2");

    TRY(query.bind(42, "Douglas Adams"));
    TRY(result = query.run());
    TEST(result.empty());
    TRY(result = {});
    TRY(query.set("$a", 86));
    TRY(query.set("$b", "Maxwell Smart"));
    TRY(query());
    TRY(query.set("$b", "James Bond"));
    TRY(query.set("$a", 007));
    TRY(query());
    TRY(query(6, "Patrick McGoohan"));

    TRY(query = dbc.query("select number, name from tango order by number"));
    TRY(result = query.run());
    TEST(! result.empty());
    TEST_EQUAL(result.columns(), 2);

    std::vector<std::string> list;
    std::string item;
    name.clear();
    int number = 0;
    count = 0;

    for (auto& row: result) {
        ++count;
        TRY(row.read(number, name));
        TEST(number >= 6);
        TEST(number <= 86);
        TEST_MATCH(name, "^[A-Z][a-z]+ (Mc)?[A-Z][a-z]+$");
        item = fmt("{0}: {1}", number, name);
        list.push_back(item);
    }

    TEST(result.empty());
    TEST_EQUAL(list.size(), 4u);
    list.resize(4);
    TEST_EQUAL(list[0], "6: Patrick McGoohan");
    TEST_EQUAL(list[1], "7: James Bond");
    TEST_EQUAL(list[2], "42: Douglas Adams");
    TEST_EQUAL(list[3], "86: Maxwell Smart");

    TRY(count = int(dbc("select count(*) from tango")));
    TEST_EQUAL(count, 4);
    TRY(count = int(dbc("select count(*) from tango where number<50")));
    TEST_EQUAL(count, 3);
    TEST_THROW_MESSAGE(dbc("select count(*) from Nonesuch"), SqliteError,
        "Sqlite error 1 in sqlite3_prepare_v3(): SQL logic error: no such table: Nonesuch");

    TRY(result = dbc("select sql from sqlite_master"));
    list.clear();

    for (auto& row: result) {
        TRY(row.read(item));
        list.push_back(item);
    }

    TEST_EQUAL(list.size(), 1u);
    list.resize(1);
    TEST_EQUAL(list[0],
        "CREATE TABLE tango(number integer not null primary key,name text not null)");

}
