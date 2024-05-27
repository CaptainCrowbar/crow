#pragma once

#include "crow/unit-test.hpp"
#include <cmath>
#include <cstdlib>
#include <exception>

// Check that every element of two vectors matches within epsilon

#define TEST_VECTORS(lhs, rhs, epsilon) \
    try { \
        auto lhs_value = lhs; \
        auto rhs_value = rhs; \
        REQUIRE(lhs_value.size() == rhs_value.size()); \
        auto epsilon_value = double(epsilon); \
        for (int i = 0; i < int(lhs_value.size()); ++i) { \
            if (std::abs(double(rhs_value[i]) - double(lhs_value[i])) > epsilon_value) { \
                FAIL_TEST("Vectors are not close enough: " \
                    << # lhs << " = " << lhs_value << ", " \
                    << # rhs << " = " << rhs_value << ", " \
                    << "epsilon = " << epsilon_value); \
                break; \
            } \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }

// HSL/HSV colours need special treatment because the hue can go haywire if the saturation is tiny

#define TEST_VECTORS_HSPACE(lhs, rhs, epsilon) \
    try { \
        auto lhs_value = lhs; \
        auto rhs_value = rhs; \
        REQUIRE(lhs_value.size() == rhs_value.size()); \
        auto epsilon_value = double(epsilon); \
        bool unsaturated_value = lhs_value[1] < epsilon && rhs_value[1] < epsilon; \
        for (auto i = int(unsaturated_value); i < int(lhs_value.size()); ++i) { \
            if (std::abs(double(rhs_value[i]) - double(lhs_value[i])) > epsilon_value) { \
                FAIL_TEST("Vectors are not close enough: " \
                    << # lhs << " = " << lhs_value << ", " \
                    << # rhs << " = " << rhs_value << ", " \
                    << "epsilon = " << epsilon_value); \
                break; \
            } \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }
