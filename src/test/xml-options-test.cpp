#include "crow/xml.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;
using namespace Crow::Literals;
using namespace Crow::Xml;
using namespace Crow::Xml::Detail;

void test_crow_xml_options_autoclose() {

    DocumentPtr doc;
    std::string str;

    str = "<abc><def><ghi>hello</abc>"
          "<jkl><mno><pqr>world</uvw></pqr></xyz></mno></jkl>";

    TEST_THROW(Document::make(str), Error);
    TRY(doc = Document::make(str, Options::autoclose));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                element: def
                    element: ghi
                        text: "hello"
            element: jkl
                element: mno
                    element: pqr
                        text: "world"
        )"_doc);

    TEST_EQUAL(doc->outer(),
        "<?xml version=\"1.0\"?>\n"
        "<abc><def><ghi>hello</ghi></def></abc>"
        "<jkl><mno><pqr>world</pqr></mno></jkl>");

}

void test_crow_xml_options_foldws() {

    DocumentPtr doc;
    std::string str;

    str = R"(
        <abc>
            <def>
                <ghi>
                    hello    world
                </ghi>
            </def>
        </abc>
        )"_doc;

    TRY(doc = Document::make(str));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                text: "\n    "
                element: def
                    text: "\n        "
                    element: ghi
                        text: "\n            hello    world\n        "
                    text: "\n    "
                text: "\n"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <abc>
            <def>
                <ghi>
                    hello    world
                </ghi>
            </def>
        </abc>
        )"_doc);

    TRY(doc = Document::make(str, Options::foldws));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                text: " "
                element: def
                    text: " "
                    element: ghi
                        text: " hello world "
                    text: " "
                text: " "
            text: " "
        )"_doc);

    TEST_EQUAL(doc->outer(),
        "<?xml version=\"1.0\"?>\n"
        "<abc> <def> <ghi> hello world </ghi> </def> </abc> "
    );

}

void test_crow_xml_options_icase() {

    DocumentPtr doc;
    std::string str;

    str = R"(
        <ABC X=1 Y=2 Z=3><def><GHI>
        hello
        </ghi></DEF></abc>
        )"_doc;

    TEST_THROW(Document::make(str), Error);
    TRY(doc = Document::make(str, Options::icase));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                element: def
                    element: ghi
                        text: "\nhello\n"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <abc x="1" y="2" z="3"><def><ghi>
        hello
        </ghi></def></abc>
        )"_doc);

}

void test_crow_xml_options_keyonly() {

    DocumentPtr doc;
    std::string str;

    str = "<abc x y z>hello</abc>\n";

    TEST_THROW(Document::make(str), Error);
    TRY(doc = Document::make(str, Options::keyonly));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                text: "hello"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <abc x="x" y="y" z="z">hello</abc>
        )"_doc);

    TEST_EQUAL(doc->outer(Options::keyonly), R"(
        <?xml version="1.0"?>
        <abc x y z>hello</abc>
        )"_doc);

}

void test_crow_xml_options_noxmldecl() {

    DocumentPtr doc;
    std::string str;

    str = "<abc>hello</abc>\n";

    TRY(doc = Document::make(str));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                text: "hello"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <abc>hello</abc>
        )"_doc);

    TRY(doc = Document::make(str, Options::noxmldecl));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            element: abc
                text: "hello"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <abc>hello</abc>
        )"_doc);

}

void test_crow_xml_options_selfclose() {

    DocumentPtr doc;
    std::string str;

    str = "<abc><br><img>hello</abc>\n";

    TEST_THROW(Document::make(str), Error);
    TRY(doc = Document::make(str, Options::selfclose));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            element: abc
                element: br
                element: img
                text: "hello"
            text: "\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        <abc><br /><img />hello</abc>
        )"_doc);

    TEST_EQUAL(doc->outer(Options::selfclose), R"(
        <?xml version="1.0"?>
        <abc><br><img>hello</abc>
        )"_doc);

}

void test_crow_xml_options_xentity() {

    DocumentPtr doc;
    std::string str;

    str = R"(
        &amp; amp
        &lt; lt
        &gt; gt
        &quot; quot
        &apos; apos
        &alpha; alpha
        &beta; beta
        &gamma; gamma
        )"_doc;

    TRY(doc = Document::make(str));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            text: "&amp; amp\n&lt; lt\n&gt; gt\n&quot; quot\n&apos; apos\n"
            entity: "&alpha;"
            text: " alpha\n"
            entity: "&beta;"
            text: " beta\n"
            entity: "&gamma;"
            text: " gamma\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        &amp; amp
        &lt; lt
        &gt; gt
        &quot; quot
        &apos; apos
        &alpha; alpha
        &beta; beta
        &gamma; gamma
        )"_doc);

    TRY(doc = Document::make(str, Options::xentity));
    REQUIRE(doc);

    TEST_EQUAL(dump(doc), R"(
        document
            xmldecl: "<?xml version=\"1.0\"?>"
            text: "&amp; amp\n&lt; lt\n&gt; gt\n&quot; quot\n&apos; apos\nα alpha\nβ beta\nγ gamma\n"
        )"_doc);

    TEST_EQUAL(doc->outer(), R"(
        <?xml version="1.0"?>
        &amp; amp
        &lt; lt
        &gt; gt
        &quot; quot
        &apos; apos
        α alpha
        β beta
        γ gamma
        )"_doc);

}
