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

void test_crow_types_mixins() {

    class FP:
    public BinaryOperators<FP> {
    public:
        FP() = default;
        explicit FP(double x): value_(x) {}
        double get() const { return value_; }
        FP& operator+=(FP y) { value_ += y.value_; return *this; }
        FP& operator-=(FP y) { value_ -= y.value_; return *this; }
        FP& operator*=(FP y) { value_ *= y.value_; return *this; }
        FP& operator/=(FP y) { value_ /= y.value_; return *this; }
    private:
        double value_ = 0;
    };

    class Int:
    public BinaryOperators<Int> {
    public:
        Int() = default;
        explicit Int(int x): value_(x) {}
        int get() const { return value_; }
        Int& operator+=(Int y) { value_ += y.value_; return *this; }
        Int& operator-=(Int y) { value_ -= y.value_; return *this; }
        Int& operator*=(Int y) { value_ *= y.value_; return *this; }
        Int& operator/=(Int y) { value_ /= y.value_; return *this; }
        Int& operator%=(Int y) { value_ %= y.value_; return *this; }
        Int& operator&=(Int y) { value_ &= y.value_; return *this; }
        Int& operator|=(Int y) { value_ |= y.value_; return *this; }
        Int& operator^=(Int y) { value_ ^= y.value_; return *this; }
    private:
        int value_ = 0;
    };

    FP f(3);
    FP g(4);
    FP h;

    TRY(h = f + g);  TEST_EQUAL(h.get(), 7);
    TRY(h = f - g);  TEST_EQUAL(h.get(), -1);
    TRY(h = f * g);  TEST_EQUAL(h.get(), 12);
    TRY(h = f / g);  TEST_EQUAL(h.get(), 0.75);

    Int i(11);
    Int j(6);
    Int k;

    TRY(k = i + j);   TEST_EQUAL(k.get(), 17);
    TRY(k = i - j);   TEST_EQUAL(k.get(), 5);
    TRY(k = i * j);   TEST_EQUAL(k.get(), 66);
    TRY(k = i / j);   TEST_EQUAL(k.get(), 1);
    TRY(k = i % j);   TEST_EQUAL(k.get(), 5);
    TRY(k = i & j);   TEST_EQUAL(k.get(), 2);
    TRY(k = i | j);   TEST_EQUAL(k.get(), 15);
    TRY(k = i ^ j);   TEST_EQUAL(k.get(), 13);

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
