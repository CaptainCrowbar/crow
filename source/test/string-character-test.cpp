#include "crow/string.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_character_functions() {

    TEST(! ascii_isupper(0));     TEST(! ascii_islower(0));     TEST(! ascii_isalpha(0));
    TEST(! ascii_isupper('\n'));  TEST(! ascii_islower('\n'));  TEST(! ascii_isalpha('\n'));
    TEST(! ascii_isupper(' '));   TEST(! ascii_islower(' '));   TEST(! ascii_isalpha(' '));
    TEST(! ascii_isupper('!'));   TEST(! ascii_islower('!'));   TEST(! ascii_isalpha('!'));
    TEST(! ascii_isupper('0'));   TEST(! ascii_islower('0'));   TEST(! ascii_isalpha('0'));
    TEST(! ascii_isupper('9'));   TEST(! ascii_islower('9'));   TEST(! ascii_isalpha('9'));
    TEST(ascii_isupper('A'));     TEST(! ascii_islower('A'));   TEST(ascii_isalpha('A'));
    TEST(ascii_isupper('Z'));     TEST(! ascii_islower('Z'));   TEST(ascii_isalpha('Z'));
    TEST(! ascii_isupper('_'));   TEST(! ascii_islower('_'));   TEST(! ascii_isalpha('_'));
    TEST(! ascii_isupper('a'));   TEST(ascii_islower('a'));     TEST(ascii_isalpha('a'));
    TEST(! ascii_isupper('z'));   TEST(ascii_islower('z'));     TEST(ascii_isalpha('z'));
    TEST(! ascii_isupper('~'));   TEST(! ascii_islower('~'));   TEST(! ascii_isalpha('~'));

    TEST(! ascii_isdigit(0));     TEST(! ascii_isxdigit(0));     TEST(! ascii_isalnum(0));
    TEST(! ascii_isdigit('\n'));  TEST(! ascii_isxdigit('\n'));  TEST(! ascii_isalnum('\n'));
    TEST(! ascii_isdigit(' '));   TEST(! ascii_isxdigit(' '));   TEST(! ascii_isalnum(' '));
    TEST(! ascii_isdigit('!'));   TEST(! ascii_isxdigit('!'));   TEST(! ascii_isalnum('!'));
    TEST(ascii_isdigit('0'));     TEST(ascii_isxdigit('0'));     TEST(ascii_isalnum('0'));
    TEST(ascii_isdigit('9'));     TEST(ascii_isxdigit('9'));     TEST(ascii_isalnum('9'));
    TEST(! ascii_isdigit('A'));   TEST(ascii_isxdigit('A'));     TEST(ascii_isalnum('A'));
    TEST(! ascii_isdigit('Z'));   TEST(! ascii_isxdigit('Z'));   TEST(ascii_isalnum('Z'));
    TEST(! ascii_isdigit('_'));   TEST(! ascii_isxdigit('_'));   TEST(! ascii_isalnum('_'));
    TEST(! ascii_isdigit('a'));   TEST(ascii_isxdigit('a'));     TEST(ascii_isalnum('a'));
    TEST(! ascii_isdigit('z'));   TEST(! ascii_isxdigit('z'));   TEST(ascii_isalnum('z'));
    TEST(! ascii_isdigit('~'));   TEST(! ascii_isxdigit('~'));   TEST(! ascii_isalnum('~'));

    TEST(! ascii_ispunct(0));     TEST(! ascii_isgraph(0));     TEST(! ascii_isprint(0));
    TEST(! ascii_ispunct('\n'));  TEST(! ascii_isgraph('\n'));  TEST(! ascii_isprint('\n'));
    TEST(! ascii_ispunct(' '));   TEST(! ascii_isgraph(' '));   TEST(ascii_isprint(' '));
    TEST(ascii_ispunct('!'));     TEST(ascii_isgraph('!'));     TEST(ascii_isprint('!'));
    TEST(! ascii_ispunct('0'));   TEST(ascii_isgraph('0'));     TEST(ascii_isprint('0'));
    TEST(! ascii_ispunct('9'));   TEST(ascii_isgraph('9'));     TEST(ascii_isprint('9'));
    TEST(! ascii_ispunct('A'));   TEST(ascii_isgraph('A'));     TEST(ascii_isprint('A'));
    TEST(! ascii_ispunct('Z'));   TEST(ascii_isgraph('Z'));     TEST(ascii_isprint('Z'));
    TEST(ascii_ispunct('_'));     TEST(ascii_isgraph('_'));     TEST(ascii_isprint('_'));
    TEST(! ascii_ispunct('a'));   TEST(ascii_isgraph('a'));     TEST(ascii_isprint('a'));
    TEST(! ascii_ispunct('z'));   TEST(ascii_isgraph('z'));     TEST(ascii_isprint('z'));
    TEST(ascii_ispunct('~'));     TEST(ascii_isgraph('~'));     TEST(ascii_isprint('~'));

    TEST(! ascii_isspace(0));    TEST(ascii_iscntrl(0));
    TEST(ascii_isspace('\n'));   TEST(ascii_iscntrl('\n'));
    TEST(ascii_isspace(' '));    TEST(! ascii_iscntrl(' '));
    TEST(! ascii_isspace('!'));  TEST(! ascii_iscntrl('!'));
    TEST(! ascii_isspace('0'));  TEST(! ascii_iscntrl('0'));
    TEST(! ascii_isspace('9'));  TEST(! ascii_iscntrl('9'));
    TEST(! ascii_isspace('A'));  TEST(! ascii_iscntrl('A'));
    TEST(! ascii_isspace('Z'));  TEST(! ascii_iscntrl('Z'));
    TEST(! ascii_isspace('_'));  TEST(! ascii_iscntrl('_'));
    TEST(! ascii_isspace('a'));  TEST(! ascii_iscntrl('a'));
    TEST(! ascii_isspace('z'));  TEST(! ascii_iscntrl('z'));
    TEST(! ascii_isspace('~'));  TEST(! ascii_iscntrl('~'));

    TEST(! ascii_isalpha_w(0));     TEST(! ascii_isalnum_w(0));     TEST(! ascii_ispunct_w(0));
    TEST(! ascii_isalpha_w('\n'));  TEST(! ascii_isalnum_w('\n'));  TEST(! ascii_ispunct_w('\n'));
    TEST(! ascii_isalpha_w(' '));   TEST(! ascii_isalnum_w(' '));   TEST(! ascii_ispunct_w(' '));
    TEST(! ascii_isalpha_w('!'));   TEST(! ascii_isalnum_w('!'));   TEST(ascii_ispunct_w('!'));
    TEST(! ascii_isalpha_w('0'));   TEST(ascii_isalnum_w('0'));     TEST(! ascii_ispunct_w('0'));
    TEST(! ascii_isalpha_w('9'));   TEST(ascii_isalnum_w('9'));     TEST(! ascii_ispunct_w('9'));
    TEST(ascii_isalpha_w('A'));     TEST(ascii_isalnum_w('A'));     TEST(! ascii_ispunct_w('A'));
    TEST(ascii_isalpha_w('Z'));     TEST(ascii_isalnum_w('Z'));     TEST(! ascii_ispunct_w('Z'));
    TEST(ascii_isalpha_w('_'));     TEST(ascii_isalnum_w('_'));     TEST(! ascii_ispunct_w('_'));
    TEST(ascii_isalpha_w('a'));     TEST(ascii_isalnum_w('a'));     TEST(! ascii_ispunct_w('a'));
    TEST(ascii_isalpha_w('z'));     TEST(ascii_isalnum_w('z'));     TEST(! ascii_ispunct_w('z'));
    TEST(! ascii_isalpha_w('~'));   TEST(! ascii_isalnum_w('~'));   TEST(ascii_ispunct_w('~'));

    TEST_EQUAL(ascii_toupper(0), 0);        TEST_EQUAL(ascii_tolower(0), 0);
    TEST_EQUAL(ascii_toupper('\n'), '\n');  TEST_EQUAL(ascii_tolower('\n'), '\n');
    TEST_EQUAL(ascii_toupper(' '), ' ');    TEST_EQUAL(ascii_tolower(' '), ' ');
    TEST_EQUAL(ascii_toupper('!'), '!');    TEST_EQUAL(ascii_tolower('!'), '!');
    TEST_EQUAL(ascii_toupper('0'), '0');    TEST_EQUAL(ascii_tolower('0'), '0');
    TEST_EQUAL(ascii_toupper('9'), '9');    TEST_EQUAL(ascii_tolower('9'), '9');
    TEST_EQUAL(ascii_toupper('A'), 'A');    TEST_EQUAL(ascii_tolower('A'), 'a');
    TEST_EQUAL(ascii_toupper('Z'), 'Z');    TEST_EQUAL(ascii_tolower('Z'), 'z');
    TEST_EQUAL(ascii_toupper('_'), '_');    TEST_EQUAL(ascii_tolower('_'), '_');
    TEST_EQUAL(ascii_toupper('a'), 'A');    TEST_EQUAL(ascii_tolower('a'), 'a');
    TEST_EQUAL(ascii_toupper('z'), 'Z');    TEST_EQUAL(ascii_tolower('z'), 'z');
    TEST_EQUAL(ascii_toupper('~'), '~');    TEST_EQUAL(ascii_tolower('~'), '~');

}
