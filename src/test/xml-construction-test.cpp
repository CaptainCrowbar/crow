#include "crow/xml.hpp"
#include "crow/unit-test.hpp"
#include <memory>
#include <string>

using namespace Crow;
using namespace Crow::Xml;

void test_crow_xml_construct_simple_nodes() {

    NodePtr node;
    TextPtr text;
    std::string str;

    TEST_EQUAL(node_type(node), NodeType::null);

    str = "Hello world";
    TRY(node = Cdata::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<![CDATA[Hello world]]>");
    TEST_EQUAL(node->type(), NodeType::cdata);

    str = "<![CDATA[Hello world]]>";
    TRY(node = Cdata::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<![CDATA[Hello world]]>");
    TEST_EQUAL(node->type(), NodeType::cdata);

    str = "Hello world";
    TRY(node = Comment::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<!--Hello world-->");
    TEST_EQUAL(node->type(), NodeType::comment);

    str = "<!-- Hello world -->";
    TRY(node = Comment::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<!-- Hello world -->");
    TEST_EQUAL(node->type(), NodeType::comment);

    str = "thing";
    TRY(node = Entity::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "&thing;");
    TEST_EQUAL(node->type(), NodeType::entity);

    str = "&thing;";
    TRY(node = Entity::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "&thing;");
    TEST_EQUAL(node->type(), NodeType::entity);

    str = "Hello world";
    TRY(node = Processing::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<?Hello world?>");
    TEST_EQUAL(node->type(), NodeType::processing);

    str = "<?Hello world?>";
    TRY(node = Processing::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<?Hello world?>");
    TEST_EQUAL(node->type(), NodeType::processing);

    str = "Hello world";
    TRY(node = Text::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "Hello world");
    TRY(text = std::dynamic_pointer_cast<Text>(node));
    REQUIRE(text);
    TEST_EQUAL(text->plain(), "Hello world");
    TEST_EQUAL(node->type(), NodeType::text);

    str = "Hello <world>";
    TRY(node = Text::create(str));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "Hello &lt;world&gt;");
    TRY(text = std::dynamic_pointer_cast<Text>(node));
    REQUIRE(text);
    TEST_EQUAL(text->plain(), "Hello <world>");
    TEST_EQUAL(node->type(), NodeType::text);

    str = "Hello &lt;world&gt;";
    TRY(node = Text::create(str, Options::encoded));
    REQUIRE(node);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "Hello &lt;world&gt;");
    TRY(text = std::dynamic_pointer_cast<Text>(node));
    REQUIRE(text);
    TEST_EQUAL(text->plain(), "Hello <world>");
    TEST_EQUAL(node->type(), NodeType::text);

}

void test_crow_xml_construct_element() {

    ElementPtr node;

    TRY(node = Element::create("hello"));
    REQUIRE(node);
    TEST_EQUAL(node->num_attrs(), 0u);
    TEST_EQUAL(node->inner(), "");
    TEST_EQUAL(node->outer(), "<hello />");
    TEST_EQUAL(node->type(), NodeType::element);

    TRY(*node += Text::create("The quick brown "));
    TRY(*node += Entity::create("fox"));
    TEST_EQUAL(node->inner(), "The quick brown &fox;");
    TEST_EQUAL(node->outer(), "<hello>The quick brown &fox;</hello>");

    TRY(*node += Text::create(" jumps over the lazy "));
    TRY(*node += Element::create("dog"));
    TEST_EQUAL(node->inner(), "The quick brown &fox; jumps over the lazy <dog />");
    TEST_EQUAL(node->outer(), "<hello>The quick brown &fox; jumps over the lazy <dog /></hello>");

    TRY(node->set_attr("alpha", "xray"));
    TRY(node->set_attr("bravo", "yankee"));
    TRY(node->set_attr("charlie", "zulu"));
    TEST_EQUAL(node->num_attrs(), 3u);
    TEST(node->has_attr("alpha"));
    TEST(node->has_attr("bravo"));
    TEST(node->has_attr("charlie"));
    TEST(! node->has_attr("delta"));
    TEST_EQUAL(node->inner(), "The quick brown &fox; jumps over the lazy <dog />");
    TEST_EQUAL(node->outer(),
        R"(<hello alpha="xray" bravo="yankee" charlie="zulu">)"
        R"(The quick brown &fox; jumps over the lazy <dog />)"
        R"(</hello>)"
    );

}

void test_crow_xml_construct_document() {

    DocumentPtr doc;
    ElementPtr elem;
    NodePtr node;

    TRY(doc = Document::create());
    REQUIRE(doc);
    TEST_EQUAL(doc->type(), NodeType::document);
    TRY(node = doc->xmldecl());
    REQUIRE(node);
    TEST_EQUAL(node->type(), NodeType::xmldecl);
    TEST_EQUAL(node->outer(), "<?xml version=\"1.0\"?>");
    TRY(node = doc->dtd());
    TEST(! node);
    TEST_EQUAL(doc->outer(), "<?xml version=\"1.0\"?>\n");

    TRY(elem = Element::create("hello"));
    REQUIRE(elem);
    TRY(*doc += elem);
    TEST_EQUAL(doc->outer(), "<?xml version=\"1.0\"?>\n<hello />");

    TRY(*elem += Text::create("The quick brown "));
    TRY(*elem += Entity::create("fox"));
    TEST_EQUAL(doc->outer(), "<?xml version=\"1.0\"?>\n<hello>The quick brown &fox;</hello>");

    TRY(*elem += Text::create(" jumps over the lazy "));
    TRY(*elem += Element::create("dog"));
    TEST_EQUAL(doc->outer(),
        "<?xml version=\"1.0\"?>\n"
        "<hello>The quick brown &fox; jumps over the lazy <dog /></hello>"
    );

}
