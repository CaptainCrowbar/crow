#include "crow/compact-array.hpp"
#include "crow/unit-test.hpp"
#include <utility>
#include <vector>

using namespace Crow;
using namespace Crow::UnitTest;

void test_crow_compact_array_tracking() {

    using CI = Counted<int>;
    using CA = CompactArray<CI, 5>;

    const std::vector<int> v5 = {1,2,3,4,5};
    const std::vector<int> v10 = {1,2,3,4,5,6,7,8,9,10};

    CA a;                                                TEST_EQUAL(CI::count(), 0);
    CA b(5, 42);                                         TEST_EQUAL(CI::count(), 5);
    TRY(a = b);                                          TEST_EQUAL(CI::count(), 10);
    TRY(a = std::move(b));                               TEST_EQUAL(CI::count(), 5);
    CA c(10, 42);                                        TEST_EQUAL(CI::count(), 15);
    TRY(a = c);                                          TEST_EQUAL(CI::count(), 20);
    TRY(a = std::move(c));                               TEST_EQUAL(CI::count(), 10);
    TRY((a = CA{1,2,3,4,5}));                            TEST_EQUAL(CI::count(), 5);
    TRY((a = CA{1,2,3,4,5,6,7,8,9,10}));                 TEST_EQUAL(CI::count(), 10);
    TRY((a = {1,2,3,4,5}));                              TEST_EQUAL(CI::count(), 5);
    TRY((a = {1,2,3,4,5,6,7,8,9,10}));                   TEST_EQUAL(CI::count(), 10);
    TRY(a = CA(v5.begin(), v5.end()));                   TEST_EQUAL(CI::count(), 5);
    TRY(a = CA(v10.begin(), v10.end()));                 TEST_EQUAL(CI::count(), 10);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.resize(5, 42));                                TEST_EQUAL(CI::count(), 5);
    TRY(a.resize(10));                                   TEST_EQUAL(CI::count(), 10);
    TRY(a.shrink_to_fit());                              TEST_EQUAL(CI::count(), 10);
    TRY(a.resize(4));                                    TEST_EQUAL(CI::count(), 4);
    TRY(a.shrink_to_fit());                              TEST_EQUAL(CI::count(), 4);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 1);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 2);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 3);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 4);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 5);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 6);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 7);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 8);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 9);
    TRY(a.push_back(42));                                TEST_EQUAL(CI::count(), 10);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 9);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 8);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 7);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 6);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 5);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 4);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 3);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 2);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 1);
    TRY(a.pop_back());                                   TEST_EQUAL(CI::count(), 0);
    TRY((a = {1,2,3}));                                  TEST_EQUAL(CI::count(), 3);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 4);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 6);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 7);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 8);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 9);
    TRY(a.insert(a.begin() + 3, 42));                    TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 9);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 8);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 7);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 6);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 4);
    TRY(a.erase(a.begin() + 3));                         TEST_EQUAL(CI::count(), 3);
    TRY(a.clear());                                      TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.begin(), v5.begin(), v5.end()));      TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.begin() + 1, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.begin() + 2, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.begin() + 3, v5.begin(), v5.end()));  TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.begin() + 3, a.begin() + 8));          TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.begin() + 2, a.begin() + 7));          TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.begin() + 1, a.begin() + 6));          TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.begin(), a.begin() + 5));              TEST_EQUAL(CI::count(), 0);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 5);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 10);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 15);
    TRY(a.insert(a.end(), v5.begin(), v5.end()));        TEST_EQUAL(CI::count(), 20);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 15);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 10);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 5);
    TRY(a.erase(a.end() - 5, a.end()));                  TEST_EQUAL(CI::count(), 0);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 5);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 10);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 15);
    TRY(a.append(v5));                                   TEST_EQUAL(CI::count(), 20);
    TRY(a.append(std::move(v5)));                        TEST_EQUAL(CI::count(), 25);

}
