#include "crow/mp-integer.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>

using namespace Crow;

void test_crow_mp_integer_unsigned_conversion_integers() {

    MPN x;
    std::string s;

    TEST_EQUAL(uint64_t(x), 0u);
    TEST_EQUAL(double(x), 0);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "0");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "0");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "0");

    TRY(x = 0x12345678ul);
    TEST_EQUAL(x.bits(), 29u);
    TEST_EQUAL(uint64_t(x), 0x12345678ul);
    TEST_EQUAL(double(x), 305419896);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "10010001101000101011001111000");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "305419896");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "12345678");

    TRY(x = 0x123456789abcdef0ull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(uint64_t(x), 0x123456789abcdef0ull);
    TEST_NEAR(double(x), 1.311768e18, 1e12);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "1001000110100010101100111100010011010101111001101111011110000");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "123456789abcdef0");

}

void test_crow_mp_integer_unsigned_conversion_strings() {

    MPN x;
    std::string s;

    TRY(x = MPN("110011000011110111111011111001011100011101100011001111101111100000001000101111100010101", 2));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("123456789123456789123456789", 10));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("661efdf2e3b19f7c045f15", 16));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("123456789123456789123456789", 0));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("0x661efdf2e3b19f7c045f15", 0));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str("b"));  TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str("n"));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str("x"));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

}

void test_crow_mp_integer_unsigned_conversion_base_from_string() {

    MPN x;
    std::string s;

    TRY(x = MPN("1", 2));                                     TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPN("12", 3));                                    TRY(s = x.str());  TEST_EQUAL(s, "5");
    TRY(x = MPN("123", 4));                                   TRY(s = x.str());  TEST_EQUAL(s, "27");
    TRY(x = MPN("1234", 5));                                  TRY(s = x.str());  TEST_EQUAL(s, "194");
    TRY(x = MPN("12345", 6));                                 TRY(s = x.str());  TEST_EQUAL(s, "1865");
    TRY(x = MPN("123456", 7));                                TRY(s = x.str());  TEST_EQUAL(s, "22875");
    TRY(x = MPN("1234567", 8));                               TRY(s = x.str());  TEST_EQUAL(s, "342391");
    TRY(x = MPN("12345678", 9));                              TRY(s = x.str());  TEST_EQUAL(s, "6053444");
    TRY(x = MPN("123456789", 10));                            TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPN("123456789a", 11));                           TRY(s = x.str());  TEST_EQUAL(s, "2853116705");
    TRY(x = MPN("123456789ab", 12));                          TRY(s = x.str());  TEST_EQUAL(s, "73686780563");
    TRY(x = MPN("123456789abc", 13));                         TRY(s = x.str());  TEST_EQUAL(s, "2103299351334");
    TRY(x = MPN("123456789abcd", 14));                        TRY(s = x.str());  TEST_EQUAL(s, "65751519677857");
    TRY(x = MPN("123456789abcde", 15));                       TRY(s = x.str());  TEST_EQUAL(s, "2234152501943159");
    TRY(x = MPN("123456789abcdef", 16));                      TRY(s = x.str());  TEST_EQUAL(s, "81985529216486895");
    TRY(x = MPN("123456789abcdefg", 17));                     TRY(s = x.str());  TEST_EQUAL(s, "3231407272993502984");
    TRY(x = MPN("123456789abcdefgh", 18));                    TRY(s = x.str());  TEST_EQUAL(s, "136146740744970718253");
    TRY(x = MPN("123456789abcdefghi", 19));                   TRY(s = x.str());  TEST_EQUAL(s, "6106233505124424657789");
    TRY(x = MPN("123456789abcdefghij", 20));                  TRY(s = x.str());  TEST_EQUAL(s, "290464265927977839335179");
    TRY(x = MPN("123456789abcdefghijk", 21));                 TRY(s = x.str());  TEST_EQUAL(s, "14606467545964956303452810");
    TRY(x = MPN("123456789abcdefghijkl", 22));                TRY(s = x.str());  TEST_EQUAL(s, "774212873841767703847271481");
    TRY(x = MPN("123456789abcdefghijklm", 23));               TRY(s = x.str());  TEST_EQUAL(s, "43141462809603124037923621715");
    TRY(x = MPN("123456789abcdefghijklmn", 24));              TRY(s = x.str());  TEST_EQUAL(s, "2521239653781255433741174806887");
    TRY(x = MPN("123456789abcdefghijklmno", 25));             TRY(s = x.str());  TEST_EQUAL(s, "154197642309049519503282176123724");
    TRY(x = MPN("123456789abcdefghijklmnop", 26));            TRY(s = x.str());  TEST_EQUAL(s, "9849791328331451697274678861440325");
    TRY(x = MPN("123456789abcdefghijklmnopq", 27));           TRY(s = x.str());  TEST_EQUAL(s, "655956343554789600515162175472115225");
    TRY(x = MPN("123456789abcdefghijklmnopqr", 28));          TRY(s = x.str());  TEST_EQUAL(s, "45467109894723422308055868660308101251");
    TRY(x = MPN("123456789abcdefghijklmnopqrs", 29));         TRY(s = x.str());  TEST_EQUAL(s, "3275109889236238692043148233075275671534");
    TRY(x = MPN("123456789abcdefghijklmnopqrst", 30));        TRY(s = x.str());  TEST_EQUAL(s, "244817041729665873959571938168846611177169");
    TRY(x = MPN("123456789abcdefghijklmnopqrstu", 31));       TRY(s = x.str());  TEST_EQUAL(s, "18965749034136928842900714476732571951160815");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuv", 32));      TRY(s = x.str());  TEST_EQUAL(s, "1520813358304789717173449357665226867487963103");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvw", 33));     TRY(s = x.str());  TEST_EQUAL(s, "126084023523204128749625029233070835487633968144");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwx", 34));    TRY(s = x.str());  TEST_EQUAL(s, "10795811667005157035274610194040730536839248769117");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwxy", 35));   TRY(s = x.str());  TEST_EQUAL(s, "953726210513969460165905219215837671424690827366389");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwxyz", 36));  TRY(s = x.str());  TEST_EQUAL(s, "86846823611197163108337531226495015298096208677436155");

}

void test_crow_mp_integer_unsigned_conversion_base_to_string() {

    MPN x;
    std::string s;

    TRY(x = MPN("1"));                                                      TRY(s = x.base(2));   TEST_EQUAL(s, "1");
    TRY(x = MPN("5"));                                                      TRY(s = x.base(3));   TEST_EQUAL(s, "12");
    TRY(x = MPN("27"));                                                     TRY(s = x.base(4));   TEST_EQUAL(s, "123");
    TRY(x = MPN("194"));                                                    TRY(s = x.base(5));   TEST_EQUAL(s, "1234");
    TRY(x = MPN("1865"));                                                   TRY(s = x.base(6));   TEST_EQUAL(s, "12345");
    TRY(x = MPN("22875"));                                                  TRY(s = x.base(7));   TEST_EQUAL(s, "123456");
    TRY(x = MPN("342391"));                                                 TRY(s = x.base(8));   TEST_EQUAL(s, "1234567");
    TRY(x = MPN("6053444"));                                                TRY(s = x.base(9));   TEST_EQUAL(s, "12345678");
    TRY(x = MPN("123456789"));                                              TRY(s = x.base(10));  TEST_EQUAL(s, "123456789");
    TRY(x = MPN("2853116705"));                                             TRY(s = x.base(11));  TEST_EQUAL(s, "123456789a");
    TRY(x = MPN("73686780563"));                                            TRY(s = x.base(12));  TEST_EQUAL(s, "123456789ab");
    TRY(x = MPN("2103299351334"));                                          TRY(s = x.base(13));  TEST_EQUAL(s, "123456789abc");
    TRY(x = MPN("65751519677857"));                                         TRY(s = x.base(14));  TEST_EQUAL(s, "123456789abcd");
    TRY(x = MPN("2234152501943159"));                                       TRY(s = x.base(15));  TEST_EQUAL(s, "123456789abcde");
    TRY(x = MPN("81985529216486895"));                                      TRY(s = x.base(16));  TEST_EQUAL(s, "123456789abcdef");
    TRY(x = MPN("3231407272993502984"));                                    TRY(s = x.base(17));  TEST_EQUAL(s, "123456789abcdefg");
    TRY(x = MPN("136146740744970718253"));                                  TRY(s = x.base(18));  TEST_EQUAL(s, "123456789abcdefgh");
    TRY(x = MPN("6106233505124424657789"));                                 TRY(s = x.base(19));  TEST_EQUAL(s, "123456789abcdefghi");
    TRY(x = MPN("290464265927977839335179"));                               TRY(s = x.base(20));  TEST_EQUAL(s, "123456789abcdefghij");
    TRY(x = MPN("14606467545964956303452810"));                             TRY(s = x.base(21));  TEST_EQUAL(s, "123456789abcdefghijk");
    TRY(x = MPN("774212873841767703847271481"));                            TRY(s = x.base(22));  TEST_EQUAL(s, "123456789abcdefghijkl");
    TRY(x = MPN("43141462809603124037923621715"));                          TRY(s = x.base(23));  TEST_EQUAL(s, "123456789abcdefghijklm");
    TRY(x = MPN("2521239653781255433741174806887"));                        TRY(s = x.base(24));  TEST_EQUAL(s, "123456789abcdefghijklmn");
    TRY(x = MPN("154197642309049519503282176123724"));                      TRY(s = x.base(25));  TEST_EQUAL(s, "123456789abcdefghijklmno");
    TRY(x = MPN("9849791328331451697274678861440325"));                     TRY(s = x.base(26));  TEST_EQUAL(s, "123456789abcdefghijklmnop");
    TRY(x = MPN("655956343554789600515162175472115225"));                   TRY(s = x.base(27));  TEST_EQUAL(s, "123456789abcdefghijklmnopq");
    TRY(x = MPN("45467109894723422308055868660308101251"));                 TRY(s = x.base(28));  TEST_EQUAL(s, "123456789abcdefghijklmnopqr");
    TRY(x = MPN("3275109889236238692043148233075275671534"));               TRY(s = x.base(29));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrs");
    TRY(x = MPN("244817041729665873959571938168846611177169"));             TRY(s = x.base(30));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrst");
    TRY(x = MPN("18965749034136928842900714476732571951160815"));           TRY(s = x.base(31));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstu");
    TRY(x = MPN("1520813358304789717173449357665226867487963103"));         TRY(s = x.base(32));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstuv");
    TRY(x = MPN("126084023523204128749625029233070835487633968144"));       TRY(s = x.base(33));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstuvw");
    TRY(x = MPN("10795811667005157035274610194040730536839248769117"));     TRY(s = x.base(34));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstuvwx");
    TRY(x = MPN("953726210513969460165905219215837671424690827366389"));    TRY(s = x.base(35));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstuvwxy");
    TRY(x = MPN("86846823611197163108337531226495015298096208677436155"));  TRY(s = x.base(36));  TEST_EQUAL(s, "123456789abcdefghijklmnopqrstuvwxyz");

    TRY(x = 42);
    TEST_THROW(x.base(1), std::invalid_argument);
    TEST_THROW(x.base(37), std::invalid_argument);

}

void test_crow_mp_integer_unsigned_conversion_double() {

    MPN x;
    std::string s;

    TRY(x = MPN::from_double(0.0));            TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPN::from_double(0.99));           TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPN::from_double(1.0));            TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPN::from_double(123456789.0));    TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPN::from_double(1.23456789e40));  TRY(s = x.str());  TEST_MATCH(s, "^12345678\\d{33}$");

}
