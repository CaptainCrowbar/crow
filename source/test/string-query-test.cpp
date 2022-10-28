#include "crow/string.hpp"
#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>
#include <utility>

using namespace Crow;

void test_crow_string_line_and_column() {

    std::string s;
    std::pair<size_t, size_t> lc;

    TRY(lc = line_and_column(s, 0, Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 1, Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);

    s = "Hello\n\nαβγδε\n";

    TRY(lc = line_and_column(s, 0, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 1, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 2, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 3, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 4, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 5, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 5u);
    TRY(lc = line_and_column(s, 6, Usize::columns));   TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 7, Usize::columns));   TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 9, Usize::columns));   TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 11, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 13, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 15, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 17, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 5u);
    TRY(lc = line_and_column(s, 18, Usize::columns));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 19, Usize::columns));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);

    TEST_THROW(line_and_column(s, 8, Usize::columns), std::invalid_argument);
    TEST_THROW(line_and_column(s, 10, Usize::columns), std::invalid_argument);
    TEST_THROW(line_and_column(s, 12, Usize::columns), std::invalid_argument);
    TEST_THROW(line_and_column(s, 14, Usize::columns), std::invalid_argument);
    TEST_THROW(line_and_column(s, 16, Usize::columns), std::invalid_argument);

    s = "😀👍👩\n"
        "😀👍🏽👩🏽\n"
        "🇳🇿🇺🇸🇩🇪🇦🇺\n";

    TRY(lc = line_and_column(s, 0, Usize::units));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 4, Usize::units));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 8, Usize::units));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 8u);
    TRY(lc = line_and_column(s, 12, Usize::units));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 12u);
    TRY(lc = line_and_column(s, 13, Usize::units));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 17, Usize::units));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 25, Usize::units));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 12u);
    TRY(lc = line_and_column(s, 33, Usize::units));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 20u);
    TRY(lc = line_and_column(s, 34, Usize::units));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 42, Usize::units));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 8u);
    TRY(lc = line_and_column(s, 50, Usize::units));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 16u);
    TRY(lc = line_and_column(s, 58, Usize::units));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 24u);
    TRY(lc = line_and_column(s, 66, Usize::units));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 32u);
    TRY(lc = line_and_column(s, 67, Usize::units));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);

    TRY(lc = line_and_column(s, 0, Usize::scalars));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 4, Usize::scalars));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 8, Usize::scalars));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 12, Usize::scalars));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 13, Usize::scalars));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 17, Usize::scalars));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 25, Usize::scalars));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 33, Usize::scalars));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 5u);
    TRY(lc = line_and_column(s, 34, Usize::scalars));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 42, Usize::scalars));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 50, Usize::scalars));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 58, Usize::scalars));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 6u);
    TRY(lc = line_and_column(s, 66, Usize::scalars));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 8u);
    TRY(lc = line_and_column(s, 67, Usize::scalars));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);

    TRY(lc = line_and_column(s, 0, Usize::graphemes));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 4, Usize::graphemes));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 8, Usize::graphemes));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 12, Usize::graphemes));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 13, Usize::graphemes));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 17, Usize::graphemes));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 25, Usize::graphemes));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 33, Usize::graphemes));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 34, Usize::graphemes));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 42, Usize::graphemes));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 1u);
    TRY(lc = line_and_column(s, 50, Usize::graphemes));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 58, Usize::graphemes));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 3u);
    TRY(lc = line_and_column(s, 66, Usize::graphemes));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 67, Usize::graphemes));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);

    TRY(lc = line_and_column(s, 0, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 4, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 8, Usize::columns));   TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 12, Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 6u);
    TRY(lc = line_and_column(s, 13, Usize::columns));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 17, Usize::columns));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 25, Usize::columns));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 33, Usize::columns));  TEST_EQUAL(lc.first, 1u);  TEST_EQUAL(lc.second, 6u);
    TRY(lc = line_and_column(s, 34, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 0u);
    TRY(lc = line_and_column(s, 42, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 2u);
    TRY(lc = line_and_column(s, 50, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 4u);
    TRY(lc = line_and_column(s, 58, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 6u);
    TRY(lc = line_and_column(s, 66, Usize::columns));  TEST_EQUAL(lc.first, 2u);  TEST_EQUAL(lc.second, 8u);
    TRY(lc = line_and_column(s, 67, Usize::columns));  TEST_EQUAL(lc.first, 3u);  TEST_EQUAL(lc.second, 0u);

    s = "\tω";          TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 5u);
    s = "α\tω";         TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 5u);
    s = "αβ\tω";        TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 5u);
    s = "αβγ\tω";       TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 5u);
    s = "αβγδ\tω";      TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 9u);
    s = "αβγδε\tω";     TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 9u);
    s = "αβγδεζ\tω";    TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 9u);
    s = "αβγδεζη\tω";   TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 9u);
    s = "αβγδεζηθ\tω";  TRY(lc = line_and_column(s, s.size(), Usize::columns));  TEST_EQUAL(lc.first, 0u);  TEST_EQUAL(lc.second, 13u);

}
