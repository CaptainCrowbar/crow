#include "crow/xml.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <memory>

using namespace Crow;
using namespace Crow::Literals;
using namespace Crow::Xml;
using namespace Crow::Xml::Detail;

void test_crow_xml_parse_cdata() {

    DocumentPtr doc;

    TRY(doc = Document::create(R"(
        <![CDATA[<Hello world>]]>
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::cdata);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <![CDATA[<Hello world>]]>
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            cdata: "<![CDATA[<Hello world>]]>"
            text: "\n"
        )"_doc);

}

void test_crow_xml_parse_comment() {

    DocumentPtr doc;

    TRY(doc = Document::create(R"(
        <!-- Hello world -->
        )"_doc, Options::comments));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::comment);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <!-- Hello world -->
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            comment: "<!-- Hello world -->"
            text: "\n"
        )"_doc);

}

void test_crow_xml_parse_entity() {

    DocumentPtr doc;

    TRY(doc = Document::create(R"(
        &hello;
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::entity);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        &hello;
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            entity: "&hello;"
            text: "\n"
        )"_doc);

}

void test_crow_xml_parse_processing() {

    DocumentPtr doc;

    TRY(doc = Document::create(R"(
        <? Hello world ?>
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::processing);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <? Hello world ?>
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            processing: "<? Hello world ?>"
            text: "\n"
        )"_doc);

}

void test_crow_xml_parse_text() {

    DocumentPtr doc;
    TextPtr text;

    TRY(doc = Document::create(R"(
        Hello world
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 2u);
    REQUIRE(doc->children() >= 2u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        Hello world
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            text: "Hello world\n"
        )"_doc);

    TRY(text = std::dynamic_pointer_cast<Text>(doc->child(1)));
    REQUIRE(text);
    TEST_EQUAL(text->outer(), "Hello world\n");
    TEST_EQUAL(text->plain(), "Hello world\n");

}

void test_crow_xml_parse_text_entities() {

    DocumentPtr doc;
    TextPtr text;

    TRY(doc = Document::create(R"(
        Hello&lt;world&gt;
        Goodnight&abc;moon&xyz;
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 6u);
    REQUIRE(doc->children() >= 6u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::text);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::entity);
    TEST_EQUAL(node_type(doc->child(3)), NodeType::text);
    TEST_EQUAL(node_type(doc->child(4)), NodeType::entity);
    TEST_EQUAL(node_type(doc->child(5)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        Hello&lt;world&gt;
        Goodnight&abc;moon&xyz;
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            text: "Hello&lt;world&gt;\nGoodnight"
            entity: "&abc;"
            text: "moon"
            entity: "&xyz;"
            text: "\n"
        )"_doc);

}

void test_crow_xml_parse_prologueext() {

    DocumentPtr doc;
    TextPtr text;

    TRY(doc = Document::create(R"(
        <?xml version="1.0" standalone="yes"?>
        Hello world
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 2u);
    REQUIRE(doc->children() >= 2u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0" standalone="yes"?>
        Hello world
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\" standalone=\"yes\"?>"
            text: "Hello world\n"
        )"_doc);

    TRY(doc = Document::create(R"(
        <!DOCTYPE note SYSTEM "Note.dtd">
        Hello world
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::dtd);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <!DOCTYPE note SYSTEM "Note.dtd">
        Hello world
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            dtd: "<!DOCTYPE note SYSTEM \"Note.dtd\">"
            text: "Hello world\n"
        )"_doc);

    TRY(doc = Document::create(R"(
        <?xml version="1.0" standalone="yes"?>
        <!DOCTYPE note
        [
            <!ELEMENT note (to,from,heading,body)>
            <!ELEMENT to (#PCDATA)>
            <!ELEMENT from (#PCDATA)>
            <!ELEMENT heading (#PCDATA)>
            <!ELEMENT body (#PCDATA)>
        ]>
        Hello world
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::dtd);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0" standalone="yes"?>
        <!DOCTYPE note
        [
            <!ELEMENT note (to,from,heading,body)>
            <!ELEMENT to (#PCDATA)>
            <!ELEMENT from (#PCDATA)>
            <!ELEMENT heading (#PCDATA)>
            <!ELEMENT body (#PCDATA)>
        ]>
        Hello world
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\" standalone=\"yes\"?>"
            dtd: "<!DOCTYPE note\n[\n    <!ELEMENT note (to,from,heading,body)>\n    <!ELEMENT to (#PCDATA)>\n    <!ELEMENT from (#PCDATA)>\n    <!ELEMENT heading (#PCDATA)>\n    <!ELEMENT body (#PCDATA)>\n]>"
            text: "Hello world\n"
        )"_doc);

}

void test_crow_xml_parse_simple_elements() {

    DocumentPtr doc;
    ElementPtr elem;
    TextPtr text;

    TRY(doc = Document::create(R"(
        <Hello/>
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::element);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <Hello />
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: Hello
            text: "\n"
        )"_doc);

    TRY(elem = std::dynamic_pointer_cast<Element>(doc->child(1)));
    REQUIRE(elem);
    TEST(elem->empty());
    TEST_EQUAL(elem->inner(), "");
    TEST_EQUAL(elem->outer(), "<Hello />");
    TEST_EQUAL(elem->children(), 0u);
    TEST_EQUAL(elem->num_attrs(), 0u);

    TRY(doc = Document::create(R"(
        <Hello abc="xyz" def="uvw"/>
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::element);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <Hello abc="xyz" def="uvw" />
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: Hello
            text: "\n"
        )"_doc);

    TRY(elem = std::dynamic_pointer_cast<Element>(doc->child(1)));
    REQUIRE(elem);
    TEST(elem->empty());
    TEST_EQUAL(elem->inner(), "");
    TEST_EQUAL(elem->outer(), R"(<Hello abc="xyz" def="uvw" />)");
    TEST_EQUAL(elem->children(), 0u);
    TEST_EQUAL(elem->num_attrs(), 2u);
    TEST_EQUAL(elem->attr("abc"), "xyz");
    TEST_EQUAL(elem->attr("def"), "uvw");

    TRY(doc = Document::create(R"(
        <Hello>World</Hello>
        )"_doc));
    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 3u);
    REQUIRE(doc->children() >= 3u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::element);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <Hello>World</Hello>
        )"_doc);
    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: Hello
                text: "World"
            text: "\n"
        )"_doc);

    TRY(elem = std::dynamic_pointer_cast<Element>(doc->child(1)));
    REQUIRE(elem);
    TEST(! elem->empty());
    TEST_EQUAL(elem->inner(), "World");
    TEST_EQUAL(elem->outer(), "<Hello>World</Hello>");
    TEST_EQUAL(elem->children(), 1u);
    TEST_EQUAL(elem->num_attrs(), 0u);

    TRY(text = std::dynamic_pointer_cast<Text>(elem->child(0)));
    REQUIRE(text);
    TEST_EQUAL(text->outer(), "World");

}

void test_crow_xml_parse_complex_elements() {

    DocumentPtr doc;

    TRY(doc = Document::create(R"(
        <alpha a=1 b=2 c=3>
        <bravo>
        Hello
        </bravo>
        <charlie>
        <delta>
        World
        </delta>
        </charlie>
        <echo/>
        </alpha>
        <foxtrot>
        <golf>
        <hotel x=42 y=86 z=99>
        Goodnight
        </hotel>
        </golf>
        </foxtrot>
        )"_doc));

    REQUIRE(doc);
    TEST_EQUAL(doc->children(), 5u);
    REQUIRE(doc->children() >= 5u);
    TEST_EQUAL(node_type(doc->child(0)), NodeType::xmldecl);
    TEST_EQUAL(node_type(doc->child(1)), NodeType::element);
    TEST_EQUAL(node_type(doc->child(2)), NodeType::text);
    TEST_EQUAL(node_type(doc->child(3)), NodeType::element);
    TEST_EQUAL(node_type(doc->child(4)), NodeType::text);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <alpha a="1" b="2" c="3">
        <bravo>
        Hello
        </bravo>
        <charlie>
        <delta>
        World
        </delta>
        </charlie>
        <echo />
        </alpha>
        <foxtrot>
        <golf>
        <hotel x="42" y="86" z="99">
        Goodnight
        </hotel>
        </golf>
        </foxtrot>
        )"_doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: alpha
                text: "\n"
                element: bravo
                    text: "\nHello\n"
                text: "\n"
                element: charlie
                    text: "\n"
                    element: delta
                        text: "\nWorld\n"
                    text: "\n"
                text: "\n"
                element: echo
                text: "\n"
            text: "\n"
            element: foxtrot
                text: "\n"
                element: golf
                    text: "\n"
                    element: hotel
                        text: "\nGoodnight\n"
                    text: "\n"
                text: "\n"
            text: "\n"
        )"_doc);

}
