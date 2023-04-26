#include "crow/xml.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <iterator>
#include <string>

using namespace Crow;
using namespace Crow::Literals;
using namespace Crow::Xml;

namespace {

    std::string get_name(NodePtr node) {
        if (! node)
            return "<null>";
        auto element = dynamic_cast<const Element*>(node.get());
        if (element == nullptr)
            return "<non-element>";
        else
            return element->name();
    }

}

void test_crow_xml_search_all() {

    DocumentPtr doc;
    Node::search_iterator it;
    Node::search_range range;

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
    TRY(range = doc->search());
    TEST(! range.empty());
    TEST_EQUAL(std::distance(range.begin(), range.end()), 24);
    TRY(it = range.begin());

    /**/        REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::xmldecl);  TEST_EQUAL((**it).outer(), "<?xml version=\"1.0\"?>");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "alpha");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "bravo");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\nHello\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "charlie");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "delta");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\nWorld\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "echo");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "foxtrot");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "golf");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::element);  TEST_EQUAL(get_name(*it), "hotel");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\nGoodnight\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");
    TRY(++it);  REQUIRE(it != range.end());  TEST_EQUAL((**it).type(), NodeType::text);     TEST_EQUAL((**it).outer(), "\n");

    TRY(++it);
    TEST(it == range.end());

}

void test_crow_xml_search_selected() {

    DocumentPtr doc;
    Node::search_range range;

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

    TRY(range = doc->search(NodeType::null));        TEST_EQUAL(std::distance(range.begin(), range.end()), 24);
    TRY(range = doc->search(NodeType::xmldecl));     TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search(NodeType::dtd));         TEST_EQUAL(std::distance(range.begin(), range.end()), 0);
    TRY(range = doc->search(NodeType::cdata));       TEST_EQUAL(std::distance(range.begin(), range.end()), 0);
    TRY(range = doc->search(NodeType::comment));     TEST_EQUAL(std::distance(range.begin(), range.end()), 0);
    TRY(range = doc->search(NodeType::entity));      TEST_EQUAL(std::distance(range.begin(), range.end()), 0);
    TRY(range = doc->search(NodeType::processing));  TEST_EQUAL(std::distance(range.begin(), range.end()), 0);
    TRY(range = doc->search(NodeType::text));        TEST_EQUAL(std::distance(range.begin(), range.end()), 15);
    TRY(range = doc->search(NodeType::element));     TEST_EQUAL(std::distance(range.begin(), range.end()), 8);
    TRY(range = doc->search(NodeType::document));    TEST_EQUAL(std::distance(range.begin(), range.end()), 0);

    TRY(range = doc->search(""));         TEST_EQUAL(std::distance(range.begin(), range.end()), 8);
    TRY(range = doc->search("alpha"));    TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("bravo"));    TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("charlie"));  TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("delta"));    TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("echo"));     TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("foxtrot"));  TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("golf"));     TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("hotel"));    TEST_EQUAL(std::distance(range.begin(), range.end()), 1);
    TRY(range = doc->search("india"));    TEST_EQUAL(std::distance(range.begin(), range.end()), 0);

}
