#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>

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

    s = "";        pos = 0;  TEST_THROW(check_decode_char(s, pos), std::out_of_range);      TEST_EQUAL(pos, 0u);
    s = "\xc0\n";  pos = 0;  TEST_THROW(check_decode_char(s, pos), std::invalid_argument);  TEST_EQUAL(pos, 0u);
    s = "\xf5\n";  pos = 0;  TEST_THROW(check_decode_char(s, pos), std::invalid_argument);  TEST_EQUAL(pos, 0u);

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
    TEST_THROW(check_encode_char(0xd800, s), std::invalid_argument);
    TEST_THROW(check_encode_char(0x110000, s), std::invalid_argument);

    u = u"";    pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 0u);  TEST(c == ~ char32_t(0));
    u = u"A";   pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 1u);  TEST(c == U'A');
    u = u"€";   pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 1u);  TEST(c == U'€');
    u = u"😀";  pos = 0;  TRY(c = decode_char(u, pos));  TEST_EQUAL(pos, 2u);  TEST(c == U'😀');

    u = u"";       pos = 0;  TEST_THROW(check_decode_char(u, pos), std::out_of_range);      TEST_EQUAL(pos, 0u);
    u = {0xd800};  pos = 0;  TEST_THROW(check_decode_char(u, pos), std::invalid_argument);  TEST_EQUAL(pos, 0u);
    u = {0xdfff};  pos = 0;  TEST_THROW(check_decode_char(u, pos), std::invalid_argument);  TEST_EQUAL(pos, 0u);

    u.clear();  TEST(encode_char(U'A', u));   TEST(u == u"A");
    u.clear();  TEST(encode_char(U'€', u));   TEST(u == u"€");
    u.clear();  TEST(encode_char(U'😀', u));  TEST(u == u"😀");

    TEST(! encode_char(0xd800, u));
    TEST(! encode_char(0x110000, u));
    TEST_THROW(check_encode_char(0xd800, u), std::invalid_argument);
    TEST_THROW(check_encode_char(0x110000, u), std::invalid_argument);

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

    TEST_THROW(decode_string(x8), std::invalid_argument);
    TEST_THROW(decode_string(x16), std::invalid_argument);
    TEST_THROW(decode_string(xw), std::invalid_argument);
    TEST_THROW(to_utf8(x32), std::invalid_argument);
    TEST_THROW(to_utf16(x32), std::invalid_argument);
    TEST_THROW(to_wstring(x32), std::invalid_argument);

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

}

void test_crow_unicode_length() {

    std::string s;
    std::u16string u;
    std::u32string v;
    std::wstring w;

    s = "";              TEST_EQUAL(s.size(), 0u);   TEST_EQUAL(utf_length(s), 0u);
    s = "Hello world";   TEST_EQUAL(s.size(), 11u);  TEST_EQUAL(utf_length(s), 11u);
    s = "αβγδε";         TEST_EQUAL(s.size(), 10u);  TEST_EQUAL(utf_length(s), 5u);
    s = "Greek αβγδε";   TEST_EQUAL(s.size(), 16u);  TEST_EQUAL(utf_length(s), 11u);
    u = u"";             TEST_EQUAL(u.size(), 0u);   TEST_EQUAL(utf_length(u), 0u);
    u = u"Hello world";  TEST_EQUAL(u.size(), 11u);  TEST_EQUAL(utf_length(u), 11u);
    u = u"αβγδε";        TEST_EQUAL(u.size(), 5u);   TEST_EQUAL(utf_length(u), 5u);
    u = u"Greek αβγδε";  TEST_EQUAL(u.size(), 11u);  TEST_EQUAL(utf_length(u), 11u);
    v = U"";             TEST_EQUAL(v.size(), 0u);   TEST_EQUAL(utf_length(v), 0u);
    v = U"Hello world";  TEST_EQUAL(v.size(), 11u);  TEST_EQUAL(utf_length(v), 11u);
    v = U"αβγδε";        TEST_EQUAL(v.size(), 5u);   TEST_EQUAL(utf_length(v), 5u);
    v = U"Greek αβγδε";  TEST_EQUAL(v.size(), 11u);  TEST_EQUAL(utf_length(v), 11u);
    w = L"";             TEST_EQUAL(w.size(), 0u);   TEST_EQUAL(utf_length(w), 0u);
    w = L"Hello world";  TEST_EQUAL(w.size(), 11u);  TEST_EQUAL(utf_length(w), 11u);
    w = L"αβγδε";        TEST_EQUAL(w.size(), 5u);   TEST_EQUAL(utf_length(w), 5u);
    w = L"Greek αβγδε";  TEST_EQUAL(w.size(), 11u);  TEST_EQUAL(utf_length(w), 11u);

    s = "αβγδε\xff";  TEST_THROW(utf_length(s), std::invalid_argument);

}

void test_crow_unicode_properties() {

    using namespace Crow::Detail;

    TEST(gc_is_cc_cf_mn()(0));
    TEST(gc_is_cc_cf_mn()('\n'));
    TEST(! gc_is_cc_cf_mn()('A'));
    TEST(gc_is_cc_cf_mn()(0x0300));  // (Mn) combining grave accent
    TEST(gc_is_cc_cf_mn()(0x0600));  // (Cf) arabic number sign

    TEST(ws_pws()('\n'));
    TEST(ws_pws()(' '));
    TEST(! ws_pws()('A'));

    TEST(! eaw_is_f_w()('A'));
    TEST(eaw_is_f_w()(0x1100));  // (W) hangul choseong kiyeok
    TEST(eaw_is_f_w()(0x3000));  // (F) ideographic space

}

void test_crow_unicode_width() {

    TEST_EQUAL(utf_width(""s),                                     0u);   // empty
    TEST_EQUAL(utf_width("Hello world"s),                          11u);  // ascii
    TEST_EQUAL(utf_width("aeiou"s),                                5u);   // narrow
    TEST_EQUAL(utf_width("áéíóú"s),                                5u);   // precomposed neutral
    TEST_EQUAL(utf_width("a\u0301e\u0301i\u0301o\u0301u\u0301"s),  5u);   // decomposed narrow
    TEST_EQUAL(utf_width("ÀÀÀ"s),                                  3u);   // neutral
    TEST_EQUAL(utf_width("ààà"s),                                  3u);   // ambiguous
    TEST_EQUAL(utf_width("\u3000\u3000\u3000"s),                   6u);   // fullwidth
    TEST_EQUAL(utf_width("\u20a9\u20a9\u20a9"s),                   3u);   // halfwidth
    TEST_EQUAL(utf_width("AAA"s),                                  3u);   // narrow
    TEST_EQUAL(utf_width("\u3001\u3001\u3001"s),                   6u);   // wide
    TEST_EQUAL(utf_width("àààÀÀÀ"s),                               6u);   // ambiguous + neutral
    TEST_EQUAL(utf_width("ààà\u3000\u3000\u3000"s),                9u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_width("ààà\u20a9\u20a9\u20a9"s),                6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_width("àààAAA"s),                               6u);   // ambiguous + narrow
    TEST_EQUAL(utf_width("ààà\u3001\u3001\u3001"s),                9u);   // ambiguous + wide
    TEST_EQUAL(utf_width(R"(¯\_(ツ)_/¯)"s),                        10u);  // katakana

    TEST_EQUAL(utf_width(u""s),                                     0u);   // empty
    TEST_EQUAL(utf_width(u"Hello world"s),                          11u);  // ascii
    TEST_EQUAL(utf_width(u"aeiou"s),                                5u);   // narrow
    TEST_EQUAL(utf_width(u"áéíóú"s),                                5u);   // precomposed neutral
    TEST_EQUAL(utf_width(u"a\u0301e\u0301i\u0301o\u0301u\u0301"s),  5u);   // decomposed narrow
    TEST_EQUAL(utf_width(u"ÀÀÀ"s),                                  3u);   // neutral
    TEST_EQUAL(utf_width(u"ààà"s),                                  3u);   // ambiguous
    TEST_EQUAL(utf_width(u"\u3000\u3000\u3000"s),                   6u);   // fullwidth
    TEST_EQUAL(utf_width(u"\u20a9\u20a9\u20a9"s),                   3u);   // halfwidth
    TEST_EQUAL(utf_width(u"AAA"s),                                  3u);   // narrow
    TEST_EQUAL(utf_width(u"\u3001\u3001\u3001"s),                   6u);   // wide
    TEST_EQUAL(utf_width(u"àààÀÀÀ"s),                               6u);   // ambiguous + neutral
    TEST_EQUAL(utf_width(u"ààà\u3000\u3000\u3000"s),                9u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_width(u"ààà\u20a9\u20a9\u20a9"s),                6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_width(u"àààAAA"s),                               6u);   // ambiguous + narrow
    TEST_EQUAL(utf_width(u"ààà\u3001\u3001\u3001"s),                9u);   // ambiguous + wide
    TEST_EQUAL(utf_width(uR"(¯\_(ツ)_/¯)"s),                        10u);  // katakana

    TEST_EQUAL(utf_width(U""s),                                     0u);   // empty
    TEST_EQUAL(utf_width(U"Hello world"s),                          11u);  // ascii
    TEST_EQUAL(utf_width(U"aeiou"s),                                5u);   // narrow
    TEST_EQUAL(utf_width(U"áéíóú"s),                                5u);   // precomposed neutral
    TEST_EQUAL(utf_width(U"a\u0301e\u0301i\u0301o\u0301u\u0301"s),  5u);   // decomposed narrow
    TEST_EQUAL(utf_width(U"ÀÀÀ"s),                                  3u);   // neutral
    TEST_EQUAL(utf_width(U"ààà"s),                                  3u);   // ambiguous
    TEST_EQUAL(utf_width(U"\u3000\u3000\u3000"s),                   6u);   // fullwidth
    TEST_EQUAL(utf_width(U"\u20a9\u20a9\u20a9"s),                   3u);   // halfwidth
    TEST_EQUAL(utf_width(U"AAA"s),                                  3u);   // narrow
    TEST_EQUAL(utf_width(U"\u3001\u3001\u3001"s),                   6u);   // wide
    TEST_EQUAL(utf_width(U"àààÀÀÀ"s),                               6u);   // ambiguous + neutral
    TEST_EQUAL(utf_width(U"ààà\u3000\u3000\u3000"s),                9u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_width(U"ààà\u20a9\u20a9\u20a9"s),                6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_width(U"àààAAA"s),                               6u);   // ambiguous + narrow
    TEST_EQUAL(utf_width(U"ààà\u3001\u3001\u3001"s),                9u);   // ambiguous + wide
    TEST_EQUAL(utf_width(UR"(¯\_(ツ)_/¯)"s),                        10u);  // katakana

    TEST_EQUAL(utf_width(L""s),                                     0u);   // empty
    TEST_EQUAL(utf_width(L"Hello world"s),                          11u);  // ascii
    TEST_EQUAL(utf_width(L"aeiou"s),                                5u);   // narrow
    TEST_EQUAL(utf_width(L"áéíóú"s),                                5u);   // precomposed neutral
    TEST_EQUAL(utf_width(L"a\u0301e\u0301i\u0301o\u0301u\u0301"s),  5u);   // decomposed narrow
    TEST_EQUAL(utf_width(L"ÀÀÀ"s),                                  3u);   // neutral
    TEST_EQUAL(utf_width(L"ààà"s),                                  3u);   // ambiguous
    TEST_EQUAL(utf_width(L"\u3000\u3000\u3000"s),                   6u);   // fullwidth
    TEST_EQUAL(utf_width(L"\u20a9\u20a9\u20a9"s),                   3u);   // halfwidth
    TEST_EQUAL(utf_width(L"AAA"s),                                  3u);   // narrow
    TEST_EQUAL(utf_width(L"\u3001\u3001\u3001"s),                   6u);   // wide
    TEST_EQUAL(utf_width(L"àààÀÀÀ"s),                               6u);   // ambiguous + neutral
    TEST_EQUAL(utf_width(L"ààà\u3000\u3000\u3000"s),                9u);   // ambiguous + fullwidth
    TEST_EQUAL(utf_width(L"ààà\u20a9\u20a9\u20a9"s),                6u);   // ambiguous + halfwidth
    TEST_EQUAL(utf_width(L"àààAAA"s),                               6u);   // ambiguous + narrow
    TEST_EQUAL(utf_width(L"ààà\u3001\u3001\u3001"s),                9u);   // ambiguous + wide
    TEST_EQUAL(utf_width(LR"(¯\_(ツ)_/¯)"s),                        10u);  // katakana

}
