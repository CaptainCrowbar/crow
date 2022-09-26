#include "crow/types.hpp"
#include "crow/unit-test.hpp"
#include <forward_list>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace Crow;
using namespace std::literals;

void test_crow_types_comparison() {

    using namespace Crow::Detail;

    auto c = SO::equal;

    TRY(c = compare3way(86, 99));  TEST(c == SO::less);
    TRY(c = compare3way(86, 86));  TEST(c == SO::equal);
    TRY(c = compare3way(99, 86));  TEST(c == SO::greater);

    TRY(c = compare3way("hello"s, "world"s));  TEST(c == SO::less);
    TRY(c = compare3way("hello"s, "hello"s));  TEST(c == SO::equal);
    TRY(c = compare3way("world"s, "hello"s));  TEST(c == SO::greater);

    TRY(c = to_order(-42));  TEST(c == SO::less);
    TRY(c = to_order(0));    TEST(c == SO::equal);
    TRY(c = to_order(42));   TEST(c == SO::greater);

}

void test_crow_types_concepts() {

    TEST(ArithmeticType<int>);
    TEST(ArithmeticType<const int>);
    TEST(ArithmeticType<float>);
    TEST(ArithmeticType<const float>);
    TEST(! ArithmeticType<void>);
    TEST(! ArithmeticType<bool>);
    TEST(! ArithmeticType<int*>);
    TEST(! ArithmeticType<std::string>);

    TEST((SameBasicType<int, int>));
    TEST((SameBasicType<const int, int&>));
    TEST((SameBasicType<const std::string, std::string&>));

}

void test_crow_types_traits() {

    TEST(! is_iterator<void>);
    TEST(! is_iterator<int>);
    TEST(! is_iterator<std::string>);
    TEST(is_iterator<std::string::iterator>);
    TEST(is_iterator<std::string::const_iterator>);
    TEST(is_iterator<std::vector<int>::iterator>);
    TEST(is_iterator<std::vector<int>::const_iterator>);
    TEST(is_iterator<std::vector<std::string>::iterator>);
    TEST(is_iterator<std::vector<std::string>::const_iterator>);

    TEST((! is_range<void>));
    TEST((! is_range<int>));
    TEST((is_range<std::string>));
    TEST((is_range<std::vector<int>>));
    TEST((is_range<std::vector<std::string>>));
    TEST((is_range<std::vector<std::pair<int, std::string>>>));
    TEST((is_range<std::map<int, std::string>>));
    TEST((is_range<std::unordered_map<int, std::string>>));

    TEST((! is_maplike_range<void>));
    TEST((! is_maplike_range<int>));
    TEST((! is_maplike_range<std::string>));
    TEST((! is_maplike_range<std::vector<int>>));
    TEST((! is_maplike_range<std::vector<std::string>>));
    TEST((is_maplike_range<std::vector<std::pair<int, std::string>>>));
    TEST((is_maplike_range<std::map<int, std::string>>));
    TEST((is_maplike_range<std::unordered_map<int, std::string>>));

    TEST_TYPE(RangeIterator<void>,                            void);
    TEST_TYPE(RangeValue<void>,                               void);
    TEST_TYPE(RangeIterator<int>,                             void);
    TEST_TYPE(RangeValue<int>,                                void);
    TEST_TYPE(RangeIterator<std::string>,                     std::string::iterator);
    TEST_TYPE(RangeIterator<const std::string>,               std::string::const_iterator);
    TEST_TYPE(RangeValue<std::string>,                        char);
    TEST_TYPE(RangeValue<const std::string>,                  char);
    TEST_TYPE(RangeIterator<std::vector<int>>,                std::vector<int>::iterator);
    TEST_TYPE(RangeIterator<const std::vector<int>>,          std::vector<int>::const_iterator);
    TEST_TYPE(RangeValue<std::vector<int>>,                   int);
    TEST_TYPE(RangeValue<const std::vector<int>>,             int);
    TEST_TYPE(RangeIterator<std::vector<std::string>>,        std::vector<std::string>::iterator);
    TEST_TYPE(RangeIterator<const std::vector<std::string>>,  std::vector<std::string>::const_iterator);
    TEST_TYPE(RangeValue<std::vector<std::string>>,           std::string);
    TEST_TYPE(RangeValue<const std::vector<std::string>>,     std::string);

}

void test_crow_types_iterator_category() {

    TEST(! is_input_iterator<void>);
    TEST(! is_output_iterator<void>);
    TEST(! is_forward_iterator<void>);
    TEST(! is_bidirectional_iterator<void>);
    TEST(! is_random_access_iterator<void>);

    TEST(! is_input_iterator<int>);
    TEST(! is_output_iterator<int>);
    TEST(! is_forward_iterator<int>);
    TEST(! is_bidirectional_iterator<int>);
    TEST(! is_random_access_iterator<int>);

    TEST(! is_input_iterator<void*>);
    TEST(! is_output_iterator<void*>);
    TEST(! is_forward_iterator<void*>);
    TEST(! is_bidirectional_iterator<void*>);
    TEST(! is_random_access_iterator<void*>);

    TEST(is_input_iterator<int*>);
    TEST(is_output_iterator<int*>);
    TEST(is_forward_iterator<int*>);
    TEST(is_bidirectional_iterator<int*>);
    TEST(is_random_access_iterator<int*>);

    TEST(is_input_iterator<std::string::iterator>);
    TEST(is_output_iterator<std::string::iterator>);
    TEST(is_forward_iterator<std::string::iterator>);
    TEST(is_bidirectional_iterator<std::string::iterator>);
    TEST(is_random_access_iterator<std::string::iterator>);

    TEST(is_input_iterator<std::vector<int>::iterator>);
    TEST(is_output_iterator<std::vector<int>::iterator>);
    TEST(is_forward_iterator<std::vector<int>::iterator>);
    TEST(is_bidirectional_iterator<std::vector<int>::iterator>);
    TEST(is_random_access_iterator<std::vector<int>::iterator>);

    TEST(is_input_iterator<std::forward_list<int>::iterator>);
    TEST(is_output_iterator<std::forward_list<int>::iterator>);
    TEST(is_forward_iterator<std::forward_list<int>::iterator>);
    TEST(! is_bidirectional_iterator<std::forward_list<int>::iterator>);
    TEST(! is_random_access_iterator<std::forward_list<int>::iterator>);

    TEST(is_input_iterator<std::list<int>::iterator>);
    TEST(is_output_iterator<std::list<int>::iterator>);
    TEST(is_forward_iterator<std::list<int>::iterator>);
    TEST(is_bidirectional_iterator<std::list<int>::iterator>);
    TEST(! is_random_access_iterator<std::list<int>::iterator>);

    TEST(is_input_iterator<std::istream_iterator<int>>);
    TEST(! is_output_iterator<std::istream_iterator<int>>);
    TEST(! is_forward_iterator<std::istream_iterator<int>>);
    TEST(! is_bidirectional_iterator<std::istream_iterator<int>>);
    TEST(! is_random_access_iterator<std::istream_iterator<int>>);

    TEST(! is_input_iterator<std::ostream_iterator<int>>);
    TEST(is_output_iterator<std::ostream_iterator<int>>);
    TEST(! is_forward_iterator<std::ostream_iterator<int>>);
    TEST(! is_bidirectional_iterator<std::ostream_iterator<int>>);
    TEST(! is_random_access_iterator<std::ostream_iterator<int>>);

}

void test_crow_types_range_category() {

    TEST(! is_input_range<void>);
    TEST(! is_output_range<void>);
    TEST(! is_forward_range<void>);
    TEST(! is_bidirectional_range<void>);
    TEST(! is_random_access_range<void>);

    TEST(is_input_range<std::string>);
    TEST(is_output_range<std::string>);
    TEST(is_forward_range<std::string>);
    TEST(is_bidirectional_range<std::string>);
    TEST(is_random_access_range<std::string>);

    TEST(is_input_range<std::vector<int>>);
    TEST(is_output_range<std::vector<int>>);
    TEST(is_forward_range<std::vector<int>>);
    TEST(is_bidirectional_range<std::vector<int>>);
    TEST(is_random_access_range<std::vector<int>>);

    TEST(is_input_range<std::forward_list<int>>);
    TEST(is_output_range<std::forward_list<int>>);
    TEST(is_forward_range<std::forward_list<int>>);
    TEST(! is_bidirectional_range<std::forward_list<int>>);
    TEST(! is_random_access_range<std::forward_list<int>>);

    TEST(is_input_range<std::list<int>>);
    TEST(is_output_range<std::list<int>>);
    TEST(is_forward_range<std::list<int>>);
    TEST(is_bidirectional_range<std::list<int>>);
    TEST(! is_random_access_range<std::list<int>>);

}
