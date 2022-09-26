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

    TEST(! IteratorType<void>);
    TEST(! IteratorType<int>);
    TEST(! IteratorType<std::string>);
    TEST(IteratorType<std::string::iterator>);
    TEST(IteratorType<std::string::const_iterator>);
    TEST(IteratorType<std::vector<int>::iterator>);
    TEST(IteratorType<std::vector<int>::const_iterator>);
    TEST(IteratorType<std::vector<std::string>::iterator>);
    TEST(IteratorType<std::vector<std::string>::const_iterator>);

    TEST((! RangeType<void>));
    TEST((! RangeType<int>));
    TEST((RangeType<std::string>));
    TEST((RangeType<std::vector<int>>));
    TEST((RangeType<std::vector<std::string>>));
    TEST((RangeType<std::vector<std::pair<int, std::string>>>));
    TEST((RangeType<std::map<int, std::string>>));
    TEST((RangeType<std::unordered_map<int, std::string>>));

    TEST((! MaplikeRangeType<void>));
    TEST((! MaplikeRangeType<int>));
    TEST((! MaplikeRangeType<std::string>));
    TEST((! MaplikeRangeType<std::vector<int>>));
    TEST((! MaplikeRangeType<std::vector<std::string>>));
    TEST((MaplikeRangeType<std::vector<std::pair<int, std::string>>>));
    TEST((MaplikeRangeType<std::map<int, std::string>>));
    TEST((MaplikeRangeType<std::unordered_map<int, std::string>>));

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

    TEST(! InputIteratorType<void>);
    TEST(! OutputIteratorType<void>);
    TEST(! ForwardIteratorType<void>);
    TEST(! BidirectionalIteratorType<void>);
    TEST(! RandomAccessIteratorType<void>);

    TEST(! InputIteratorType<int>);
    TEST(! OutputIteratorType<int>);
    TEST(! ForwardIteratorType<int>);
    TEST(! BidirectionalIteratorType<int>);
    TEST(! RandomAccessIteratorType<int>);

    TEST(! InputIteratorType<void*>);
    TEST(! OutputIteratorType<void*>);
    TEST(! ForwardIteratorType<void*>);
    TEST(! BidirectionalIteratorType<void*>);
    TEST(! RandomAccessIteratorType<void*>);

    TEST(InputIteratorType<int*>);
    TEST(OutputIteratorType<int*>);
    TEST(ForwardIteratorType<int*>);
    TEST(BidirectionalIteratorType<int*>);
    TEST(RandomAccessIteratorType<int*>);

    TEST(InputIteratorType<std::string::iterator>);
    TEST(OutputIteratorType<std::string::iterator>);
    TEST(ForwardIteratorType<std::string::iterator>);
    TEST(BidirectionalIteratorType<std::string::iterator>);
    TEST(RandomAccessIteratorType<std::string::iterator>);

    TEST(InputIteratorType<std::vector<int>::iterator>);
    TEST(OutputIteratorType<std::vector<int>::iterator>);
    TEST(ForwardIteratorType<std::vector<int>::iterator>);
    TEST(BidirectionalIteratorType<std::vector<int>::iterator>);
    TEST(RandomAccessIteratorType<std::vector<int>::iterator>);

    TEST(InputIteratorType<std::forward_list<int>::iterator>);
    TEST(OutputIteratorType<std::forward_list<int>::iterator>);
    TEST(ForwardIteratorType<std::forward_list<int>::iterator>);
    TEST(! BidirectionalIteratorType<std::forward_list<int>::iterator>);
    TEST(! RandomAccessIteratorType<std::forward_list<int>::iterator>);

    TEST(InputIteratorType<std::list<int>::iterator>);
    TEST(OutputIteratorType<std::list<int>::iterator>);
    TEST(ForwardIteratorType<std::list<int>::iterator>);
    TEST(BidirectionalIteratorType<std::list<int>::iterator>);
    TEST(! RandomAccessIteratorType<std::list<int>::iterator>);

    TEST(InputIteratorType<std::istream_iterator<int>>);
    TEST(! OutputIteratorType<std::istream_iterator<int>>);
    TEST(! ForwardIteratorType<std::istream_iterator<int>>);
    TEST(! BidirectionalIteratorType<std::istream_iterator<int>>);
    TEST(! RandomAccessIteratorType<std::istream_iterator<int>>);

    TEST(! InputIteratorType<std::ostream_iterator<int>>);
    TEST(OutputIteratorType<std::ostream_iterator<int>>);
    TEST(! ForwardIteratorType<std::ostream_iterator<int>>);
    TEST(! BidirectionalIteratorType<std::ostream_iterator<int>>);
    TEST(! RandomAccessIteratorType<std::ostream_iterator<int>>);

}

void test_crow_types_range_category() {

    TEST(! InputRangeType<void>);
    TEST(! OutputRangeType<void>);
    TEST(! ForwardRangeType<void>);
    TEST(! BidirectionalRangeType<void>);
    TEST(! RandomAccessRangeType<void>);

    TEST(InputRangeType<std::string>);
    TEST(OutputRangeType<std::string>);
    TEST(ForwardRangeType<std::string>);
    TEST(BidirectionalRangeType<std::string>);
    TEST(RandomAccessRangeType<std::string>);

    TEST(InputRangeType<std::vector<int>>);
    TEST(OutputRangeType<std::vector<int>>);
    TEST(ForwardRangeType<std::vector<int>>);
    TEST(BidirectionalRangeType<std::vector<int>>);
    TEST(RandomAccessRangeType<std::vector<int>>);

    TEST(InputRangeType<std::forward_list<int>>);
    TEST(OutputRangeType<std::forward_list<int>>);
    TEST(ForwardRangeType<std::forward_list<int>>);
    TEST(! BidirectionalRangeType<std::forward_list<int>>);
    TEST(! RandomAccessRangeType<std::forward_list<int>>);

    TEST(InputRangeType<std::list<int>>);
    TEST(OutputRangeType<std::list<int>>);
    TEST(ForwardRangeType<std::list<int>>);
    TEST(BidirectionalRangeType<std::list<int>>);
    TEST(! RandomAccessRangeType<std::list<int>>);

}

void test_crow_types_comparison_functions() {

    using SO = std::strong_ordering;

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
