#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

using namespace Crow;
using namespace std::literals;

void test_crow_unicode_implementation_details() {

    using namespace Crow::Detail;

    TEST_EQUAL(hex_data("", 0),              "");
    TEST_EQUAL(hex_data("A", 1),             "41");
    TEST_EQUAL(hex_data("Hello world", 11),  "48 65 6c 6c 6f 20 77 6f 72 6c 64");
    TEST_EQUAL(hex_data("αβγδε", 10),        "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(hex_data(u"αβγδε", 5),        "03b1 03b2 03b3 03b4 03b5");
    TEST_EQUAL(hex_data(U"αβγδε", 5),        "000003b1 000003b2 000003b3 000003b4 000003b5");

    TEST_EQUAL(hex_char32(0),      "U+0000");
    TEST_EQUAL(hex_char32(U'A'),   "U+0041");
    TEST_EQUAL(hex_char32(U'€'),   "U+20AC");
    TEST_EQUAL(hex_char32(U'😀'),  "U+1F600");

}

void test_crow_unicode_properties() {

    TEST_EQUAL(general_category(0),         GC::Cc);  // null
    TEST_EQUAL(general_category(U'\n'),     GC::Cc);  // line feed
    TEST_EQUAL(general_category(U' '),      GC::Zs);  // space
    TEST_EQUAL(general_category(U'!'),      GC::Po);  // exclamation mark
    TEST_EQUAL(general_category(U'$'),      GC::Sc);  // dollar sign
    TEST_EQUAL(general_category(U'('),      GC::Ps);  // left parenthesis
    TEST_EQUAL(general_category(U')'),      GC::Pe);  // right parenthesis
    TEST_EQUAL(general_category(U'+'),      GC::Sm);  // plus sign
    TEST_EQUAL(general_category(U'-'),      GC::Pd);  // hyphen-minus
    TEST_EQUAL(general_category(U'/'),      GC::Po);  // solidus
    TEST_EQUAL(general_category(U'0'),      GC::Nd);  // digit zero
    TEST_EQUAL(general_category(U'9'),      GC::Nd);  // digit nine
    TEST_EQUAL(general_category(U'A'),      GC::Lu);  // latin capital letter a
    TEST_EQUAL(general_category(U'Z'),      GC::Lu);  // latin capital letter z
    TEST_EQUAL(general_category(U'^'),      GC::Sk);  // circumflex accent
    TEST_EQUAL(general_category(U'_'),      GC::Pc);  // low line
    TEST_EQUAL(general_category(U'a'),      GC::Ll);  // latin small letter a
    TEST_EQUAL(general_category(U'z'),      GC::Ll);  // latin small letter z
    TEST_EQUAL(general_category(0xa6),      GC::So);  // broken bar
    TEST_EQUAL(general_category(0xaa),      GC::Lo);  // feminine ordinal indicator
    TEST_EQUAL(general_category(0xab),      GC::Pi);  // left-pointing double angle quotation mark
    TEST_EQUAL(general_category(0xad),      GC::Cf);  // soft hyphen
    TEST_EQUAL(general_category(0xb2),      GC::No);  // superscript two
    TEST_EQUAL(general_category(0xbb),      GC::Pf);  // right-pointing double angle quotation mark
    TEST_EQUAL(general_category(0xf7),      GC::Sm);  // division sign
    TEST_EQUAL(general_category(0x1c5),     GC::Lt);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(general_category(0x2b0),     GC::Lm);  // modifier letter small h
    TEST_EQUAL(general_category(0x2b9),     GC::Lm);  // modifier letter prime
    TEST_EQUAL(general_category(0x2c5),     GC::Sk);  // modifier letter down arrowhead
    TEST_EQUAL(general_category(0x300),     GC::Mn);  // combining grave accent
    TEST_EQUAL(general_category(0x309),     GC::Mn);  // combining hook above
    TEST_EQUAL(general_category(0x378),     GC::Cn);  // unassigned
    TEST_EQUAL(general_category(0x488),     GC::Me);  // combining cyrillic hundred thousands sign
    TEST_EQUAL(general_category(0x5d1),     GC::Lo);  // hebrew letter bet
    TEST_EQUAL(general_category(0x6fd),     GC::So);  // arabic sign sindhi ampersand
    TEST_EQUAL(general_category(0x903),     GC::Mc);  // devanagari sign visarga
    TEST_EQUAL(general_category(0x94e),     GC::Mc);  // devanagari vowel sign prishthamatra e
    TEST_EQUAL(general_category(0x9f7),     GC::No);  // bengali currency numerator four
    TEST_EQUAL(general_category(0x9fb),     GC::Sc);  // bengali ganda mark
    TEST_EQUAL(general_category(0x16ee),    GC::Nl);  // runic arlaug symbol
    TEST_EQUAL(general_category(0x180e),    GC::Cf);  // mongolian vowel separator
    TEST_EQUAL(general_category(0x1f8d),    GC::Lt);  // greek capital letter alpha with dasia and oxia and prosgegrammeni
    TEST_EQUAL(general_category(0x2006),    GC::Zs);  // six-per-em space
    TEST_EQUAL(general_category(0x2014),    GC::Pd);  // em dash
    TEST_EQUAL(general_category(0x2028),    GC::Zl);  // line separator
    TEST_EQUAL(general_category(0x2029),    GC::Zp);  // paragraph separator
    TEST_EQUAL(general_category(0x207d),    GC::Ps);  // superscript left parenthesis
    TEST_EQUAL(general_category(0x2166),    GC::Nl);  // roman numeral seven
    TEST_EQUAL(general_category(0x2309),    GC::Pe);  // right ceiling
    TEST_EQUAL(general_category(0x2e0c),    GC::Pi);  // left raised omission bracket
    TEST_EQUAL(general_category(0x2e21),    GC::Pf);  // right vertical bar with quill
    TEST_EQUAL(general_category(0xa670),    GC::Me);  // combining cyrillic ten millions sign
    TEST_EQUAL(general_category(0xd800),    GC::Cs);  // non private use high surrogate
    TEST_EQUAL(general_category(0xdfff),    GC::Cs);  // low surrogate
    TEST_EQUAL(general_category(0xe000),    GC::Co);  // private use
    TEST_EQUAL(general_category(0xff3f),    GC::Pc);  // fullwidth low line
    TEST_EQUAL(general_category(0x10fffd),  GC::Co);  // plane 16 private use
    TEST_EQUAL(general_category(0x110000),  GC::Cn);  // not unicode

}

void test_crow_unicode_character_encoding() {

    // UTF-32    UTF-16     UTF-8
    // 0000004d  004d       4d
    // 00000430  0430       d0 b0
    // 00004e8c  4e8c       e4 ba 8c
    // 00010302  d800 df02  f0 90 8c 82
    // 0010fffd  dbff dffd  f4 8f bf bd

    std::string s;
    std::u16string u;
    std::wstring w;
    char32_t c = 0;
    size_t pos = 0;

    s = "";                    pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 0u);  TEST(c == ~ char32_t(0));
    s = "\x4d\n";              pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 1u);  TEST(c == 0x4du);
    s = "\xd0\xb0\n";          pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 2u);  TEST(c == 0x430u);
    s = "\xe4\xba\x8c\n";      pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 3u);  TEST(c == 0x4e8cu);
    s = "\xf0\x90\x8c\x82\n";  pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 4u);  TEST(c == 0x10302u);
    s = "\xf4\x8f\xbf\xbd\n";  pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 4u);  TEST(c == 0x10fffdu);
    s = "\xc0\n";              pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 1u);  TEST(c == ~ char32_t(0));
    s = "\xf5\n";              pos = 0;  TRY(c = decode_char(s, pos));        TEST_EQUAL(pos, 1u);  TEST(c == ~ char32_t(0));
    s = "\x4d\n";              pos = 0;  TRY(c = check_decode_char(s, pos));  TEST_EQUAL(pos, 1u);  TEST(c == 0x4du);
    s = "\xd0\xb0\n";          pos = 0;  TRY(c = check_decode_char(s, pos));  TEST_EQUAL(pos, 2u);  TEST(c == 0x430u);
    s = "\xe4\xba\x8c\n";      pos = 0;  TRY(c = check_decode_char(s, pos));  TEST_EQUAL(pos, 3u);  TEST(c == 0x4e8cu);
    s = "\xf0\x90\x8c\x82\n";  pos = 0;  TRY(c = check_decode_char(s, pos));  TEST_EQUAL(pos, 4u);  TEST(c == 0x10302u);
    s = "\xf4\x8f\xbf\xbd\n";  pos = 0;  TRY(c = check_decode_char(s, pos));  TEST_EQUAL(pos, 4u);  TEST(c == 0x10fffdu);

    s = "";        pos = 0;  TEST_THROW(check_decode_char(s, pos), std::out_of_range);  TEST_EQUAL(pos, 0u);
    s = "\xc0\n";  pos = 0;  TEST_THROW(check_decode_char(s, pos), UnicodeError);       TEST_EQUAL(pos, 0u);
    s = "\xf5\n";  pos = 0;  TEST_THROW(check_decode_char(s, pos), UnicodeError);       TEST_EQUAL(pos, 0u);

    s.clear();  TEST(encode_char(0, s));              TEST_EQUAL(s, std::string(1, '\0'));
    s.clear();  TEST(encode_char(0x4d, s));           TEST_EQUAL(s, "\x4d");
    s.clear();  TEST(encode_char(0x430, s));          TEST_EQUAL(s, "\xd0\xb0");
    s.clear();  TEST(encode_char(0x4e8c, s));         TEST_EQUAL(s, "\xe4\xba\x8c");
    s.clear();  TEST(encode_char(0x10302, s));        TEST_EQUAL(s, "\xf0\x90\x8c\x82");
    s.clear();  TEST(encode_char(0x10fffd, s));       TEST_EQUAL(s, "\xf4\x8f\xbf\xbd");
    s.clear();  TRY(check_encode_char(0, s));         TEST_EQUAL(s, std::string(1, '\0'));
    s.clear();  TRY(check_encode_char(0x4d, s));      TEST_EQUAL(s, "\x4d");
    s.clear();  TRY(check_encode_char(0x430, s));     TEST_EQUAL(s, "\xd0\xb0");
    s.clear();  TRY(check_encode_char(0x4e8c, s));    TEST_EQUAL(s, "\xe4\xba\x8c");
    s.clear();  TRY(check_encode_char(0x10302, s));   TEST_EQUAL(s, "\xf0\x90\x8c\x82");
    s.clear();  TRY(check_encode_char(0x10fffd, s));  TEST_EQUAL(s, "\xf4\x8f\xbf\xbd");

    TEST(! encode_char(0xd800, s));
    TEST(! encode_char(0x110000, s));
    TEST_THROW(check_encode_char(0xd800, s), UnicodeError);
    TEST_THROW(check_encode_char(0x110000, s), UnicodeError);

    u = u"";    pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 0u);  TEST(c == ~ char32_t(0));
    u = u"A";   pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 1u);  TEST(c == U'A');
    u = u"€";   pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 1u);  TEST(c == U'€');
    u = u"😀";  pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 2u);  TEST(c == U'😀');

    u = u"";       pos = 0;  TEST_THROW(check_decode_char(u, pos), std::out_of_range);  TEST_EQUAL(pos, 0u);
    u = {0xd800};  pos = 0;  TEST_THROW(check_decode_char(u, pos), UnicodeError);       TEST_EQUAL(pos, 0u);
    u = {0xdfff};  pos = 0;  TEST_THROW(check_decode_char(u, pos), UnicodeError);       TEST_EQUAL(pos, 0u);

    u.clear();  TEST(encode_char(U'A', u));   TEST(u == u"A");
    u.clear();  TEST(encode_char(U'€', u));   TEST(u == u"€");
    u.clear();  TEST(encode_char(U'😀', u));  TEST(u == u"😀");

    TEST(! encode_char(0xd800, u));
    TEST(! encode_char(0x110000, u));
    TEST_THROW(check_encode_char(0xd800, u), UnicodeError);
    TEST_THROW(check_encode_char(0x110000, u), UnicodeError);

}

void test_crow_unicode_string_encoding() {

    const std::string a8;
    const std::string b8 = "Hello";
    const std::string c8 = {"\x4d\xd0\xb0\xe4\xba\x8c\xf0\x90\x8c\x82\xf4\x8f\xbf\xbd"};
    const std::string x8 = "Hello\xffworld";
    const std::u16string a16;
    const std::u16string b16 = u"Hello";
    const std::u16string c16 = {0x4d,0x430,0x4e8c,0xd800,0xdf02,0xdbff,0xdffd};
    const std::u16string x16 = u"Hello" + std::u16string{0xd800} + u"world";
    const std::u32string a32;
    const std::u32string b32 = U"Hello";
    const std::u32string c32 = {0x4d,0x430,0x4e8c,0x10302,0x10fffd};
    const std::u32string x32 = U"Hello" + std::u32string{0xd800} + U"world";
    const std::wstring aw;
    const std::wstring bw = L"Hello";
    const std::wstring xw = L"Hello" + std::wstring{0xd800} + L"world";

    TEST(decode_string(a8) == a32);
    TEST(decode_string(b8) == b32);
    TEST(decode_string(c8) == c32);
    TEST(decode_string(a16) == a32);
    TEST(decode_string(b16) == b32);
    TEST(decode_string(c16) == c32);
    TEST(decode_string(aw) == a32);
    TEST(decode_string(bw) == b32);

    TEST_EQUAL(to_utf8(a32), a8);
    TEST_EQUAL(to_utf8(b32), b8);
    TEST_EQUAL(to_utf8(c32), c8);
    TEST(to_utf16(a32) == a16);
    TEST(to_utf16(b32) == b16);
    TEST(to_utf16(c32) == c16);
    TEST(to_wstring(a32) == aw);
    TEST(to_wstring(b32) == bw);

    TEST_THROW(decode_string(x8), UnicodeError);
    TEST_THROW(decode_string(x16), UnicodeError);
    TEST_THROW(decode_string(xw), UnicodeError);
    TEST_THROW(to_utf8(x32), UnicodeError);
    TEST_THROW(to_utf16(x32), UnicodeError);
    TEST_THROW(to_wstring(x32), UnicodeError);

    TEST(is_valid_utf(a8));
    TEST(is_valid_utf(b8));
    TEST(is_valid_utf(c8));
    TEST(! is_valid_utf(x8));
    TEST(is_valid_utf(a16));
    TEST(is_valid_utf(b16));
    TEST(is_valid_utf(c16));
    TEST(! is_valid_utf(x16));
    TEST(is_valid_utf(a32));
    TEST(is_valid_utf(b32));
    TEST(is_valid_utf(c32));
    TEST(! is_valid_utf(x32));
    TEST(is_valid_utf(aw));
    TEST(is_valid_utf(bw));
    TEST(! is_valid_utf(xw));

    TRY(is_valid_utf(a8, true));
    TRY(is_valid_utf(b8, true));
    TRY(is_valid_utf(c8, true));
    TEST_THROW(is_valid_utf(x8, true), UnicodeError);
    TRY(is_valid_utf(a16, true));
    TRY(is_valid_utf(b16, true));
    TRY(is_valid_utf(c16, true));
    TEST_THROW(is_valid_utf(x16, true), UnicodeError);
    TRY(is_valid_utf(a32, true));
    TRY(is_valid_utf(b32, true));
    TRY(is_valid_utf(c32, true));
    TEST_THROW(is_valid_utf(x32, true), UnicodeError);
    TRY(is_valid_utf(aw, true));
    TRY(is_valid_utf(bw, true));
    TEST_THROW(is_valid_utf(xw, true), UnicodeError);

}

void test_crow_unicode_utf_iterators() {

    const std::string a8;
    const std::string b8 = "Hello";
    const std::string c8 = {"\x4d\xd0\xb0\xe4\xba\x8c\xf0\x90\x8c\x82\xf4\x8f\xbf\xbd"};
    const std::string x8 = "Hello\xffworld";
    const std::u32string a32;
    const std::u32string b32 = U"Hello";
    const std::u32string c32 = {0x4d,0x430,0x4e8c,0x10302,0x10fffd};
    const std::u32string x32 = U"Hello\ufffdworld";

    UtfIterator i;
    UtfRange u;
    std::u32string s;

    TRY(u = utf_range(a8));  std::copy(u.begin(), u.end(), overwrite(s));  TEST(s == a32);
    TRY(u = utf_range(b8));  std::copy(u.begin(), u.end(), overwrite(s));  TEST(s == b32);
    TRY(u = utf_range(c8));  std::copy(u.begin(), u.end(), overwrite(s));  TEST(s == c32);
    TRY(u = utf_range(x8));  std::copy(u.begin(), u.end(), overwrite(s));  TEST(s == x32);

    TRY(u = utf_range(x8, true));
    TEST_THROW(std::copy(u.begin(), u.end(), std::back_inserter(s)), UnicodeError);

    TRY(i = utf_begin(c8));  TEST(*i == U'M');           TEST_EQUAL(i.view(), "M");
    TRY(++i);                TEST(*i == U'\u0430');      TEST_EQUAL(i.view(), "\u0430");
    TRY(++i);                TEST(*i == U'\u4e8c');      TEST_EQUAL(i.view(), "\u4e8c");
    TRY(++i);                TEST(*i == U'\U00010302');  TEST_EQUAL(i.view(), "\U00010302");
    TRY(++i);                TEST(*i == U'\U0010fffd');  TEST_EQUAL(i.view(), "\U0010fffd");

    TRY(++i);
    TEST(i == utf_end(c8));

    TRY(i = utf_begin(x8, true));  TEST(*i == U'H');  TEST_EQUAL(i.view(), "H");
    TRY(++i);                      TEST(*i == U'e');  TEST_EQUAL(i.view(), "e");
    TRY(++i);                      TEST(*i == U'l');  TEST_EQUAL(i.view(), "l");
    TRY(++i);                      TEST(*i == U'l');  TEST_EQUAL(i.view(), "l");
    TRY(++i);                      TEST(*i == U'o');  TEST_EQUAL(i.view(), "o");

    TEST_THROW(++i, UnicodeError);

}

void test_crow_unicode_grapheme_iterators() {

    std::string str;
    GraphemeIterator b, e, i;

    TRY(b = graphemes_begin(str));
    TRY(e = graphemes_end(str));
    TEST(b == e);

    str =
        "Hello\n"
        "αβγδε\n"
        "áéíóú\n"                                // composed
        "a\u0301e\u0301i\u0301o\u0301u\u0301\n"  // decomposed
        "😀👍👩\n"                               // simple emoji
        "😀👍🏽👩🏽\n"                               // modified emoji
        "🇳🇿🇺🇸🇩🇪🇦🇺\n";                            // flags

    TRY(b = graphemes_begin(str));
    TRY(e = graphemes_end(str));

    TRY(i = b);  REQUIRE(i != e);  TEST_EQUAL(*i, "H");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "e");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "l");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "l");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "o");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "α");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "β");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "γ");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "δ");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ε");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "á");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "é");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "í");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ó");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ú");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "a\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "e\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "i\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "o\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "u\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "😀");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👍");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👩");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "😀");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👍🏽");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👩🏽");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇳🇿");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇺🇸");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇩🇪");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇦🇺");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");

    TRY(++i);
    TEST(i == e);

    str = "αβγ\xffxyz";

    TRY(b = graphemes_begin(str, true));
    TRY(e = graphemes_end(str));

    TRY(i = b);  REQUIRE(i != e);  TEST_EQUAL(*i, "α");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "β");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "γ");

    TEST_THROW(++i, UnicodeError);

}

void test_crow_unicode_size_units() {

    std::string s;
    std::u16string u;
    std::u32string v;
    std::wstring w;

    TEST_EQUAL(utf_size(""s,             Usize::units),  0u);
    TEST_EQUAL(utf_size("Hello world"s,  Usize::units),  11u);
    TEST_EQUAL(utf_size("αβγδε"s,        Usize::units),  10u);
    TEST_EQUAL(utf_size("Greek αβγδε"s,  Usize::units),  16u);
    TEST_EQUAL(utf_size("😀👍👩"s,       Usize::units),  12u);
    TEST_EQUAL(utf_size("😀👍🏽👩🏽"s,       Usize::units),  20u);
    TEST_EQUAL(utf_size("🇳🇿🇺🇸🇩🇪🇦🇺"s,     Usize::units),  32u);
    TEST_EQUAL(utf_size("αβγδε\xff"s,    Usize::units),  11u);

}

void test_crow_unicode_size_scalars() {

    std::string s;
    std::u16string u;
    std::u32string v;
    std::wstring w;

    TEST_EQUAL(utf_size(""s,             Usize::scalars),  0u);
    TEST_EQUAL(utf_size("Hello world"s,  Usize::scalars),  11u);
    TEST_EQUAL(utf_size("αβγδε"s,        Usize::scalars),  5u);
    TEST_EQUAL(utf_size("Greek αβγδε"s,  Usize::scalars),  11u);
    TEST_EQUAL(utf_size("😀👍👩"s,       Usize::scalars),  3u);
    TEST_EQUAL(utf_size("😀👍🏽👩🏽"s,       Usize::scalars),  5u);
    TEST_EQUAL(utf_size("🇳🇿🇺🇸🇩🇪🇦🇺"s,     Usize::scalars),  8u);

    TEST_THROW(utf_size("αβγδε\xff"s, Usize::scalars), UnicodeError);

}

void test_crow_unicode_size_graphemes() {

    TEST_EQUAL(utf_size(""s,                                     Usize::graphemes),  0u);   // empty
    TEST_EQUAL(utf_size("Hello world"s,                          Usize::graphemes),  11u);  // ascii
    TEST_EQUAL(utf_size("aeiou"s,                                Usize::graphemes),  5u);   // narrow
    TEST_EQUAL(utf_size("áéíóú"s,                                Usize::graphemes),  5u);   // precomposed neutral
    TEST_EQUAL(utf_size("a\u0301e\u0301i\u0301o\u0301u\u0301"s,  Usize::graphemes),  5u);   // decomposed narrow
    TEST_EQUAL(utf_size("ÀÀÀ"s,                                  Usize::graphemes),  3u);   // neutral
    TEST_EQUAL(utf_size("ààà"s,                                  Usize::graphemes),  3u);   // ambiguous
    TEST_EQUAL(utf_size("\u3000\u3000\u3000"s,                   Usize::graphemes),  3u);   // fullwidth
    TEST_EQUAL(utf_size("\u20a9\u20a9\u20a9"s,                   Usize::graphemes),  3u);   // halfwidth
    TEST_EQUAL(utf_size("AAA"s,                                  Usize::graphemes),  3u);   // narrow
    TEST_EQUAL(utf_size("\u3001\u3001\u3001"s,                   Usize::graphemes),  3u);   // wide
    TEST_EQUAL(utf_size("àààÀÀÀ"s,                               Usize::graphemes),  6u);   // ambiguous + neutral
    TEST_EQUAL(utf_size("ààà\u3000\u3000\u3000"s,                Usize::graphemes),  6u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_size("ààà\u20a9\u20a9\u20a9"s,                Usize::graphemes),  6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_size("àààAAA"s,                               Usize::graphemes),  6u);   // ambiguous + narrow
    TEST_EQUAL(utf_size("ààà\u3001\u3001\u3001"s,                Usize::graphemes),  6u);   // ambiguous + wide
    TEST_EQUAL(utf_size("😀👍👩"s,                               Usize::graphemes),  3u);   // simple emoji
    TEST_EQUAL(utf_size("😀👍🏽👩🏽"s,                               Usize::graphemes),  3u);   // modified emoji
    TEST_EQUAL(utf_size("🇳🇿🇺🇸🇩🇪🇦🇺"s,                             Usize::graphemes),  4u);   // flags

    TEST_THROW(utf_size("αβγδε\xff"s, Usize::graphemes), UnicodeError);

}

void test_crow_unicode_size_columns() {

    TEST_EQUAL(utf_size(""s,                                     Usize::columns),  0u);   // empty
    TEST_EQUAL(utf_size("Hello world"s,                          Usize::columns),  11u);  // ascii
    TEST_EQUAL(utf_size("aeiou"s,                                Usize::columns),  5u);   // narrow
    TEST_EQUAL(utf_size("áéíóú"s,                                Usize::columns),  5u);   // precomposed neutral
    TEST_EQUAL(utf_size("a\u0301e\u0301i\u0301o\u0301u\u0301"s,  Usize::columns),  5u);   // decomposed narrow
    TEST_EQUAL(utf_size("ÀÀÀ"s,                                  Usize::columns),  3u);   // neutral
    TEST_EQUAL(utf_size("ààà"s,                                  Usize::columns),  3u);   // ambiguous
    TEST_EQUAL(utf_size("\u3000\u3000\u3000"s,                   Usize::columns),  6u);   // fullwidth
    TEST_EQUAL(utf_size("\u20a9\u20a9\u20a9"s,                   Usize::columns),  3u);   // halfwidth
    TEST_EQUAL(utf_size("AAA"s,                                  Usize::columns),  3u);   // narrow
    TEST_EQUAL(utf_size("\u3001\u3001\u3001"s,                   Usize::columns),  6u);   // wide
    TEST_EQUAL(utf_size("àààÀÀÀ"s,                               Usize::columns),  6u);   // ambiguous + neutral
    TEST_EQUAL(utf_size("ààà\u3000\u3000\u3000"s,                Usize::columns),  9u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_size("ààà\u20a9\u20a9\u20a9"s,                Usize::columns),  6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_size("àààAAA"s,                               Usize::columns),  6u);   // ambiguous + narrow
    TEST_EQUAL(utf_size("ààà\u3001\u3001\u3001"s,                Usize::columns),  9u);   // ambiguous + wide
    TEST_EQUAL(utf_size("😀👍👩"s,                               Usize::columns),  6u);   // simple emoji
    TEST_EQUAL(utf_size("😀👍🏽👩🏽"s,                               Usize::columns),  6u);   // modified emoji
    TEST_EQUAL(utf_size("🇳🇿🇺🇸🇩🇪🇦🇺"s,                             Usize::columns),  8u);   // flags

    TEST_THROW(utf_size("αβγδε\xff"s, Usize::columns), UnicodeError);

}

void test_crow_unicode_canonical_combining_class() {

    using namespace Crow::Detail;

    TEST_EQUAL(canonical_combining_class(0), 0);
    TEST_EQUAL(canonical_combining_class(U'A'), 0);
    TEST_EQUAL(canonical_combining_class(0x300), 230);   // combining grave accent
    TEST_EQUAL(canonical_combining_class(0x316), 220);   // combining grave accent below
    TEST_EQUAL(canonical_combining_class(0x334), 1);     // combining tilde overlay
    TEST_EQUAL(canonical_combining_class(0x94d), 9);     // devanagari sign virama
    TEST_EQUAL(canonical_combining_class(0x10ffff), 0);

}

void test_crow_unicode_canonical_composition() {

    using namespace Crow::Detail;

    TEST_EQUAL(uint32_t(canonical_composition(0x41, 0x42)), 0u);
    TEST_EQUAL(uint32_t(canonical_composition(0x41, 0x300)), 0xc0u);
    TEST_EQUAL(uint32_t(canonical_composition(0x79, 0x308)), 0xffu);
    TEST_EQUAL(uint32_t(canonical_composition(0x1111, 0x1171)), 0xd4ccu);
    TEST_EQUAL(uint32_t(canonical_composition(0xd4cc, 0x11b6)), 0xd4dbu);

}

void test_crow_unicode_canonical_decomposition() {

    using namespace Crow::Detail;

    std::pair<char32_t, char32_t> pair;

    TRY(pair = canonical_decomposition(U'A'));    TEST_EQUAL(uint32_t(pair.first), 0u);       TEST_EQUAL(uint32_t(pair.second), 0u);
    TRY(pair = canonical_decomposition(0xc0));    TEST_EQUAL(uint32_t(pair.first), 0x41u);    TEST_EQUAL(uint32_t(pair.second), 0x300u);
    TRY(pair = canonical_decomposition(0xff));    TEST_EQUAL(uint32_t(pair.first), 0x79u);    TEST_EQUAL(uint32_t(pair.second), 0x308u);
    TRY(pair = canonical_decomposition(0xd4cc));  TEST_EQUAL(uint32_t(pair.first), 0x1111u);  TEST_EQUAL(uint32_t(pair.second), 0x1171u);
    TRY(pair = canonical_decomposition(0xd4db));  TEST_EQUAL(uint32_t(pair.first), 0xd4ccu);  TEST_EQUAL(uint32_t(pair.second), 0x11b6u);

}

void test_crow_unicode_hangul_syllable_type() {

    using namespace Crow::Detail;

    TEST_EQUAL(int(hangul_syllable_type(0)), int(Hangul_Syllable_Type::NA));
    TEST_EQUAL(int(hangul_syllable_type(0x1100)), int(Hangul_Syllable_Type::L));
    TEST_EQUAL(int(hangul_syllable_type(0x1160)), int(Hangul_Syllable_Type::V));
    TEST_EQUAL(int(hangul_syllable_type(0x11a8)), int(Hangul_Syllable_Type::T));
    TEST_EQUAL(int(hangul_syllable_type(0xac00)), int(Hangul_Syllable_Type::LV));
    TEST_EQUAL(int(hangul_syllable_type(0xac01)), int(Hangul_Syllable_Type::LVT));
    TEST_EQUAL(int(hangul_syllable_type(0xd4cc)), int(Hangul_Syllable_Type::LV));
    TEST_EQUAL(int(hangul_syllable_type(0xd4db)), int(Hangul_Syllable_Type::LVT));

}

void test_crow_unicode_noncharacters() {

    TEST(! is_noncharacter(0));
    TEST(! is_noncharacter(0xfdcf));
    TEST(is_noncharacter(0xfdd0));
    TEST(is_noncharacter(0xfdef));
    TEST(! is_noncharacter(0xfdf0));
    TEST(! is_noncharacter(0xfffd));
    TEST(is_noncharacter(0xfffe));
    TEST(is_noncharacter(0xffff));
    TEST(! is_noncharacter(0x1fffd));
    TEST(is_noncharacter(0x1fffe));
    TEST(is_noncharacter(0x1ffff));
    TEST(! is_noncharacter(0x10fffd));
    TEST(is_noncharacter(0x10fffe));
    TEST(is_noncharacter(0x10ffff));

}

void test_crow_unicode_pattern_syntax() {

    TEST(! is_pattern_syntax(0));
    TEST(is_pattern_syntax(U'!'));
    TEST(! is_pattern_syntax(U'0'));
    TEST(! is_pattern_syntax(U'A'));
    TEST(is_pattern_syntax(0xa1));        // inverted exclamation mark
    TEST(! is_pattern_syntax(0x3a9));     // greek capital letter omega
    TEST(! is_pattern_syntax(0x10ffff));

}

void test_crow_unicode_xid_properties() {

    TEST(! is_xid_start(0));         TEST(! is_xid_continue(0));
    TEST(! is_xid_start(U'!'));      TEST(! is_xid_continue(U'!'));
    TEST(! is_xid_start(U'0'));      TEST(is_xid_continue(U'0'));
    TEST(is_xid_start(U'A'));        TEST(is_xid_continue(U'A'));
    TEST(! is_xid_start(0xa1));      TEST(! is_xid_continue(0xa1));      // inverted exclamation mark
    TEST(is_xid_start(0x3a9));       TEST(is_xid_continue(0x3a9));       // greek capital letter omega
    TEST(! is_xid_start(0x10ffff));  TEST(! is_xid_continue(0x10ffff));

}
