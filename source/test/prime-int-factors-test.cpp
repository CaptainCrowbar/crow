#include "crow/prime.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <map>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_prime_factorization_int() {

    std::vector<int> v;
    std::map<int, int> m;
    std::string s;

    TRY(v = prime_factors(1));          TRY(s = format_range(v));  TEST_EQUAL(s, "[]");
    TRY(v = prime_factors(2));          TRY(s = format_range(v));  TEST_EQUAL(s, "[2]");
    TRY(v = prime_factors(3));          TRY(s = format_range(v));  TEST_EQUAL(s, "[3]");
    TRY(v = prime_factors(4));          TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2]");
    TRY(v = prime_factors(5));          TRY(s = format_range(v));  TEST_EQUAL(s, "[5]");
    TRY(v = prime_factors(6));          TRY(s = format_range(v));  TEST_EQUAL(s, "[2,3]");
    TRY(v = prime_factors(7));          TRY(s = format_range(v));  TEST_EQUAL(s, "[7]");
    TRY(v = prime_factors(8));          TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2,2]");
    TRY(v = prime_factors(9));          TRY(s = format_range(v));  TEST_EQUAL(s, "[3,3]");
    TRY(v = prime_factors(10));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,5]");
    TRY(v = prime_factors(11));         TRY(s = format_range(v));  TEST_EQUAL(s, "[11]");
    TRY(v = prime_factors(12));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2,3]");
    TRY(v = prime_factors(13));         TRY(s = format_range(v));  TEST_EQUAL(s, "[13]");
    TRY(v = prime_factors(14));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,7]");
    TRY(v = prime_factors(15));         TRY(s = format_range(v));  TEST_EQUAL(s, "[3,5]");
    TRY(v = prime_factors(16));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2,2,2]");
    TRY(v = prime_factors(17));         TRY(s = format_range(v));  TEST_EQUAL(s, "[17]");
    TRY(v = prime_factors(18));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,3,3]");
    TRY(v = prime_factors(19));         TRY(s = format_range(v));  TEST_EQUAL(s, "[19]");
    TRY(v = prime_factors(20));         TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2,5]");
    TRY(v = prime_factors(31));         TRY(s = format_range(v));  TEST_EQUAL(s, "[31]");
    TRY(v = prime_factors(127));        TRY(s = format_range(v));  TEST_EQUAL(s, "[127]");
    TRY(v = prime_factors(8191));       TRY(s = format_range(v));  TEST_EQUAL(s, "[8191]");
    TRY(v = prime_factors(131071));     TRY(s = format_range(v));  TEST_EQUAL(s, "[131071]");
    TRY(v = prime_factors(524287));     TRY(s = format_range(v));  TEST_EQUAL(s, "[524287]");
    TRY(v = prime_factors(123));        TRY(s = format_range(v));  TEST_EQUAL(s, "[3,41]");
    TRY(v = prime_factors(1234));       TRY(s = format_range(v));  TEST_EQUAL(s, "[2,617]");
    TRY(v = prime_factors(12345));      TRY(s = format_range(v));  TEST_EQUAL(s, "[3,5,823]");
    TRY(v = prime_factors(123456));     TRY(s = format_range(v));  TEST_EQUAL(s, "[2,2,2,2,2,2,3,643]");
    TRY(v = prime_factors(1234567));    TRY(s = format_range(v));  TEST_EQUAL(s, "[127,9721]");
    TRY(v = prime_factors(12345678));   TRY(s = format_range(v));  TEST_EQUAL(s, "[2,3,3,47,14593]");
    TRY(v = prime_factors(123456789));  TRY(s = format_range(v));  TEST_EQUAL(s, "[3,3,3607,3803]");
    TRY(v = prime_factors(987654319));  TRY(s = format_range(v));  TEST_EQUAL(s, "[987654319]");

    TRY(m = prime_factors_map(1));          TRY(s = format_map(m));  TEST_EQUAL(s, "{}");
    TRY(m = prime_factors_map(2));          TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1}");
    TRY(m = prime_factors_map(3));          TRY(s = format_map(m));  TEST_EQUAL(s, "{3:1}");
    TRY(m = prime_factors_map(4));          TRY(s = format_map(m));  TEST_EQUAL(s, "{2:2}");
    TRY(m = prime_factors_map(5));          TRY(s = format_map(m));  TEST_EQUAL(s, "{5:1}");
    TRY(m = prime_factors_map(6));          TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,3:1}");
    TRY(m = prime_factors_map(7));          TRY(s = format_map(m));  TEST_EQUAL(s, "{7:1}");
    TRY(m = prime_factors_map(8));          TRY(s = format_map(m));  TEST_EQUAL(s, "{2:3}");
    TRY(m = prime_factors_map(9));          TRY(s = format_map(m));  TEST_EQUAL(s, "{3:2}");
    TRY(m = prime_factors_map(10));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,5:1}");
    TRY(m = prime_factors_map(11));         TRY(s = format_map(m));  TEST_EQUAL(s, "{11:1}");
    TRY(m = prime_factors_map(12));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:2,3:1}");
    TRY(m = prime_factors_map(13));         TRY(s = format_map(m));  TEST_EQUAL(s, "{13:1}");
    TRY(m = prime_factors_map(14));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,7:1}");
    TRY(m = prime_factors_map(15));         TRY(s = format_map(m));  TEST_EQUAL(s, "{3:1,5:1}");
    TRY(m = prime_factors_map(16));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:4}");
    TRY(m = prime_factors_map(17));         TRY(s = format_map(m));  TEST_EQUAL(s, "{17:1}");
    TRY(m = prime_factors_map(18));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,3:2}");
    TRY(m = prime_factors_map(19));         TRY(s = format_map(m));  TEST_EQUAL(s, "{19:1}");
    TRY(m = prime_factors_map(20));         TRY(s = format_map(m));  TEST_EQUAL(s, "{2:2,5:1}");
    TRY(m = prime_factors_map(31));         TRY(s = format_map(m));  TEST_EQUAL(s, "{31:1}");
    TRY(m = prime_factors_map(127));        TRY(s = format_map(m));  TEST_EQUAL(s, "{127:1}");
    TRY(m = prime_factors_map(8191));       TRY(s = format_map(m));  TEST_EQUAL(s, "{8191:1}");
    TRY(m = prime_factors_map(131071));     TRY(s = format_map(m));  TEST_EQUAL(s, "{131071:1}");
    TRY(m = prime_factors_map(524287));     TRY(s = format_map(m));  TEST_EQUAL(s, "{524287:1}");
    TRY(m = prime_factors_map(123));        TRY(s = format_map(m));  TEST_EQUAL(s, "{3:1,41:1}");
    TRY(m = prime_factors_map(1234));       TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,617:1}");
    TRY(m = prime_factors_map(12345));      TRY(s = format_map(m));  TEST_EQUAL(s, "{3:1,5:1,823:1}");
    TRY(m = prime_factors_map(123456));     TRY(s = format_map(m));  TEST_EQUAL(s, "{2:6,3:1,643:1}");
    TRY(m = prime_factors_map(1234567));    TRY(s = format_map(m));  TEST_EQUAL(s, "{127:1,9721:1}");
    TRY(m = prime_factors_map(12345678));   TRY(s = format_map(m));  TEST_EQUAL(s, "{2:1,3:2,47:1,14593:1}");
    TRY(m = prime_factors_map(123456789));  TRY(s = format_map(m));  TEST_EQUAL(s, "{3:2,3607:1,3803:1}");
    TRY(m = prime_factors_map(987654319));  TRY(s = format_map(m));  TEST_EQUAL(s, "{987654319:1}");

}
