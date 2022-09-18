#include "crow/markup.hpp"
#include "crow/unit-test.hpp"
#include <sstream>
#include <string>

using namespace Crow;

void test_crow_markup_xml() {

    const std::string expected =
        "<h1>Header</h1>\n"
        "<br/>\n"
        "<ul>\n"
        "<li><code>alpha</code></li>\n"
        "<li><code>bravo</code></li>\n"
        "<li><code>charlie</code></li>\n"
        "</ul>\n";

    {
        std::string out;
        {
            Xtag h1;
            TRY(h1 = Xtag("<h1>\n", out));
            TEST_EQUAL(h1.tag(), "h1");
            out += "Header";
        }
        Xtag br;
        TRY(br = Xtag("<br/>\n", out));
        {
            TEST_EQUAL(br.tag(), "");
            Xtag ul;
            TRY(ul = Xtag("<ul>\n\n", out));
            TEST_EQUAL(ul.tag(), "ul");
            for (auto item: {"alpha", "bravo", "charlie"}) {
                Xtag li, code;
                TRY(li = Xtag("<li>\n", out));
                TRY(code = Xtag("<code>", out));
                out += item;
            }
        }
        TEST_EQUAL(out, expected);
    }

    {
        std::ostringstream out;
        {
            Xtag h1;
            TRY(h1 = Xtag("<h1>\n", out));
            TEST_EQUAL(h1.tag(), "h1");
            out << "Header";
        }
        Xtag br;
        TRY(br = Xtag("<br/>\n", out));
        {
            TEST_EQUAL(br.tag(), "");
            Xtag ul;
            TRY(ul = Xtag("<ul>\n\n", out));
            TEST_EQUAL(ul.tag(), "ul");
            for (auto item: {"alpha", "bravo", "charlie"}) {
                Xtag li, code;
                TRY(li = Xtag("<li>\n", out));
                TRY(code = Xtag("<code>", out));
                out << item;
            }
        }
        std::string str = out.str();
        TEST_EQUAL(str, expected);
    }

    {
        std::string out;
        {
            Xtag h1("h1\n", out);
            out += "Header";
        }
        Xtag br("br/\n", out);
        {
            Xtag ul("ul\n\n", out);
            for (auto item: {"alpha", "bravo", "charlie"}) {
                Xtag li("li\n", out);
                Xtag code("code", out);
                out += item;
            }
        }
        TEST_EQUAL(out, expected);
    }

    {
        std::string out;
        {
            Xtag h1("h1\n", out);
            out += "Header";
        }
        Xtag br("br/\n", out);
        {
            Xtag ul("ul\n\n", out);
            Xtag li("", out);
            for (auto item: {"alpha", "bravo", "charlie"}) {
                li.next("li\n");
                Xtag code("code", out);
                out += item;
            }
        }
        TEST_EQUAL(out, expected);
    }

    {
        std::string out;
        Xtag::nest(out, "h1\n", "Header");
        Xtag br("br/\n", out);
        {
            Xtag ul("ul\n\n", out);
            for (auto item: {"alpha", "bravo", "charlie"})
                Xtag::nest(out, "li\n", "code", item);
        }
        TEST_EQUAL(out, expected);
    }

    {
        std::string out;
        {
            Xtag ul("ul\n\n", out);
            Xtag li("li\n", out);
            out += "alpha";
            TRY(li.next());
            out += "bravo";
            TRY(li.next());
            out += "charlie";
        }
        TEST_EQUAL(out,
            "<ul>\n"
            "<li>alpha</li>\n"
            "<li>bravo</li>\n"
            "<li>charlie</li>\n"
            "</ul>\n"
        );
    }

    {
        std::string out;
        {
            Xtag img("img", out);
            out += "hello";
        }
        TEST_EQUAL(out, "<img>hello</img>");
    }

    {
        std::string out;
        {
            Xtag img("img/", out);
            out += "hello";
        }
        TEST_EQUAL(out, "<img/>hello");
    }

}

void test_crow_markup_html() {

    {
        std::string out;
        {
            Htag img("img", out);
            out += "hello";
        }
        TEST_EQUAL(out, "<img>hello");
    }

    {
        std::string out;
        {
            Htag img("img/", out);
            out += "hello";
        }
        TEST_EQUAL(out, "<img/>hello");
    }

}
