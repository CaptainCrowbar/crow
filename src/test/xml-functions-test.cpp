#include "crow/xml.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;
using namespace Crow::Xml;

void test_crow_xml_functions_character_encoding() {

    TEST_EQUAL(encode_text(""),                     "");
    TEST_EQUAL(encode_text("Hello world\n"),        "Hello world\n");
    TEST_EQUAL(encode_text("&<>\"'\x01\x7fαβγδε"),  "&amp;&lt;&gt;&quot;&apos;&#x1;&#x7f;αβγδε");

}

void test_crow_xml_functions_character_decoding() {

    TEST_EQUAL(decode_text(""),                                           "");
    TEST_EQUAL(decode_text("Hello world\n"),                              "Hello world\n");
    TEST_EQUAL(decode_text("&amp;&lt;&gt;&quot;&apos;&#x1;&#x7f;αβγδε"),  "&<>\"'\x01\x7fαβγδε");
    TEST_EQUAL(decode_text("&#x3b1;&#x3b2;&#x3b3;&#x3b4;&#x3b5;"),        "αβγδε");
    TEST_EQUAL(decode_text("&#945;&#946;&#947;&#948;&#949;"),             "αβγδε");
    TEST_EQUAL(decode_text("Hello &nonentity;"),                          "Hello &nonentity;");

}

void test_crow_xml_functions_name_validation() {

    TEST(is_valid_name("Hello"));
    TEST(is_valid_name("HelloWorld"));
    TEST(is_valid_name("hello_world"));
    TEST(is_valid_name("hello:world"));
    TEST(is_valid_name("hello42"));
    TEST(! is_valid_name(""));
    TEST(! is_valid_name(" Hello"));
    TEST(! is_valid_name("Hello "));
    TEST(! is_valid_name("Hello World"));
    TEST(! is_valid_name("42hello"));

}
