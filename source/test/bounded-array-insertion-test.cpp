#include "crow/bounded-array.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::UnitTest;

namespace {

    std::string repeat_string(const std::string& str, size_t n, const std::string& delimiter) {
        if (n == 0)
            return {};
        else if (n == 1)
            return str;
        std::string sd = str + delimiter;
        std::string result;
        for (size_t i = 0; i < n; ++i)
            result += sd;
        result.resize(result.size() - delimiter.size());
        return result;
    }

}

void test_crow_bounded_array_insertion() {

    using IBA = BoundedArray<int, 10>;
    using SBA = BoundedArray<std::string, 10>;

    IBA a;
    SBA b;
    IBA::iterator ai;
    SBA::iterator bi;
    std::vector<int> v;

    for (size_t i = 1; i <= 10; ++i) {
        TRY(a.push_back(42));
        TEST_EQUAL(a.size(), i);
        TEST_EQUAL(a.capacity(), 10u);
        TEST_EQUAL(a[i - 1], 42);
        TEST_EQUAL(format_range(a), "[" + repeat_string("42", i, ",") + "]");
    }

    for (size_t i = 1; i <= 10; ++i) {
        size_t n = 10 - i;
        TRY(a.pop_back());
        TEST_EQUAL(a.size(), n);
        TEST_EQUAL(a.capacity(), 10u);
        if (n)
            TEST_EQUAL(a[n - 1], 42);
        TEST_EQUAL(format_range(a), "[" + repeat_string("42", n, ",") + "]");
    }

    TEST(a.empty());
    TEST_EQUAL(a.size(), 0u);
    TEST_EQUAL(format_range(a), "[]");

    for (size_t i = 1; i <= 10; ++i) {
        TRY(b.emplace_back(3, 'a'));
        TEST_EQUAL(b.size(), i);
        TEST_EQUAL(b.capacity(), 10u);
        TEST_EQUAL(b[i - 1], "aaa");
        TEST_EQUAL(format_range(b), "[" + repeat_string("aaa", i, ",") + "]");
    }

    for (size_t i = 1; i <= 10; ++i) {
        size_t n = 10 - i;
        TRY(b.pop_back());
        TEST_EQUAL(b.size(), n);
        TEST_EQUAL(b.capacity(), 10u);
        if (n)
            TEST_EQUAL(b[n - 1], "aaa");
        TEST_EQUAL(format_range(b), "[" + repeat_string("aaa", n, ",") + "]");
    }

    TEST(b.empty());
    TEST_EQUAL(b.size(), 0u);
    TEST_EQUAL(format_range(b), "[]");

    TRY((a = {1,2,3}));
    TEST_EQUAL(a.size(), 3u);
    TEST_EQUAL(format_range(a), "[1,2,3]");

    TRY(ai = a.insert(a.begin() + 3, 10));  TEST_EQUAL(a.size(), 4u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,10]");
    TRY(ai = a.insert(a.begin() + 3, 20));  TEST_EQUAL(a.size(), 5u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 30));  TEST_EQUAL(a.size(), 6u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 40));  TEST_EQUAL(a.size(), 7u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 50));  TEST_EQUAL(a.size(), 8u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 60));  TEST_EQUAL(a.size(), 9u);   TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,60,50,40,30,20,10]");
    TRY(ai = a.insert(a.begin() + 3, 70));  TEST_EQUAL(a.size(), 10u);  TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,70,60,50,40,30,20,10]");
    TEST_THROW(a.insert(a.begin() + 3, 80), std::length_error);
                                            TEST_EQUAL(a.size(), 10u);  TEST_EQUAL(ai - a.begin(), 3);  TEST_EQUAL(format_range(a), "[1,2,3,70,60,50,40,30,20,10]");

    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 9u);  TEST_EQUAL(format_range(a), "[1,2,3,60,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,50,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 7u);  TEST_EQUAL(format_range(a), "[1,2,3,40,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 6u);  TEST_EQUAL(format_range(a), "[1,2,3,30,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 5u);  TEST_EQUAL(format_range(a), "[1,2,3,20,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,10]");
    TRY(a.erase(a.begin() + 3));  TEST_EQUAL(a.size(), 3u);  TEST_EQUAL(format_range(a), "[1,2,3]");

    TRY((b = {"x","y","z"}));
    TEST_EQUAL(b.size(), 3u);
    TEST_EQUAL(format_range(b), "[x,y,z]");

    TRY(bi = b.emplace(b.begin() + 3, 3, 'a'));  TEST_EQUAL(b.size(), 4u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'b'));  TEST_EQUAL(b.size(), 5u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'c'));  TEST_EQUAL(b.size(), 6u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'd'));  TEST_EQUAL(b.size(), 7u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'e'));  TEST_EQUAL(b.size(), 8u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'f'));  TEST_EQUAL(b.size(), 9u);   TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(bi = b.emplace(b.begin() + 3, 3, 'g'));  TEST_EQUAL(b.size(), 10u);  TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,ggg,fff,eee,ddd,ccc,bbb,aaa]");
    TEST_THROW(b.emplace(b.begin() + 3, 3, 'h'), std::length_error);
                                                 TEST_EQUAL(b.size(), 10u);  TEST_EQUAL(bi - b.begin(), 3);  TEST_EQUAL(format_range(b), "[x,y,z,ggg,fff,eee,ddd,ccc,bbb,aaa]");

    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 9u);  TEST_EQUAL(format_range(b), "[x,y,z,fff,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 8u);  TEST_EQUAL(format_range(b), "[x,y,z,eee,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 7u);  TEST_EQUAL(format_range(b), "[x,y,z,ddd,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 6u);  TEST_EQUAL(format_range(b), "[x,y,z,ccc,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 5u);  TEST_EQUAL(format_range(b), "[x,y,z,bbb,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 4u);  TEST_EQUAL(format_range(b), "[x,y,z,aaa]");
    TRY(b.erase(b.begin() + 3));  TEST_EQUAL(b.size(), 3u);  TEST_EQUAL(format_range(b), "[x,y,z]");

    TRY(a.clear());
    TEST(a.empty());

    v = {1,2,3,4};     TRY(a.insert(a.begin(), v.begin(), v.end()));                                TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    v = {5,6,7,8};     TRY(a.insert(a.begin() + 2, v.begin(), v.end()));                            TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,5,6,7,8,3,4]");
    v = {9,10,11,12};  TEST_THROW(a.insert(a.begin() + 4, v.begin(), v.end()), std::length_error);  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,5,6,7,8,3,4]");

    TRY(a.erase(a.begin() + 2, a.begin() + 6));  TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));      TEST_EQUAL(a.size(), 0u);  TEST_EQUAL(format_range(a), "[]");

    TEST(a.empty());

    v = {1,2,3,4};     TRY(a.insert(a.begin(), v.begin(), v.end()));                            TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    v = {5,6,7,8};     TRY(a.insert(a.begin(), v.begin(), v.end()));                            TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[5,6,7,8,1,2,3,4]");
    v = {9,10,11,12};  TEST_THROW(a.insert(a.begin(), v.begin(), v.end()), std::length_error);  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[5,6,7,8,1,2,3,4]");

    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    TRY(a.erase(a.begin(), a.begin() + 4));  TEST_EQUAL(a.size(), 0u);  TEST_EQUAL(format_range(a), "[]");

    TEST(a.empty());

    v = {1,2,3,4};     TRY(a.insert(a.end(), v.begin(), v.end()));                            TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    v = {5,6,7,8};     TRY(a.insert(a.end(), v.begin(), v.end()));                            TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};  TEST_THROW(a.insert(a.end(), v.begin(), v.end()), std::length_error);  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");

    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    TRY(a.erase(a.end() - 4, a.end()));  TEST_EQUAL(a.size(), 0u);  TEST_EQUAL(format_range(a), "[]");

    TEST(a.empty());

    v = {1,2,3,4};     TRY(a.append(v));                            TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    v = {5,6,7,8};     TRY(a.append(v));                            TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};  TEST_THROW(a.append(v), std::length_error);  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");

    TRY(a.clear());
    TEST(a.empty());

    v = {1,2,3,4};     TRY(a.append(std::move(v)));                            TEST_EQUAL(a.size(), 4u);  TEST_EQUAL(format_range(a), "[1,2,3,4]");
    v = {5,6,7,8};     TRY(a.append(std::move(v)));                            TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");
    v = {9,10,11,12};  TEST_THROW(a.append(std::move(v)), std::length_error);  TEST_EQUAL(a.size(), 8u);  TEST_EQUAL(format_range(a), "[1,2,3,4,5,6,7,8]");

}
