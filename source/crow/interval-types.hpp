#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <concepts>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace Crow {

    namespace Detail {

        template <typename T>
        concept StepwiseOperatorsIntervalType = requires (T t) {
            { ++t } -> std::convertible_to<T>;
            { --t } -> std::convertible_to<T>;
            { t++ } -> std::convertible_to<T>;
            { t-- } -> std::convertible_to<T>;
        };

        template <typename T>
        concept ArithmeticOperatorsIntervalType = requires (T t) {
            { t + t } -> std::convertible_to<T>;
            { t - t } -> std::convertible_to<T>;
            { t * t } -> std::convertible_to<T>;
            { t / t } -> std::convertible_to<T>;
        };

        template <typename T>
        T from_string(const std::string& s) {
            if constexpr (std::is_constructible_v<T, std::string>) {
                return T(s);
            } else if constexpr (requires (T& t, std::istream& in) { { in >> t }; }) {
                T t;
                std::istringstream in(s);
                in >> t;
                return t;
            } else {
                static_assert(dependent_false<T>, "No conversion from string to this type");
            }
        }

    }

    CROW_ENUM_CLASS(IntervalBound, unsigned char, 0,
        empty,   // The interval is empty
        closed,  // The interval includes the boundary value
        open,    // The interval does not include the boundary value
        unbound  // The interval is unbounded in this direction
    )

    constexpr IntervalBound operator~(IntervalBound b) noexcept { return IntervalBound(3 - int(b)); }

    CROW_ENUM_CLASS(IntervalCategory, int, 0,
        none,        // Not usable in an interval
        continuous,  // Models a continuous arithmetic type (e.g. floating point)
        integral,    // Integer arithmetic operations (e.g. integer)
        ordered,     // Ordered but not an arithmetic type (e.g. string)
        stepwise     // Incrementable and decrementable (e.g. pointer)
    )

    CROW_ENUM_CLASS(IntervalMatch, int, -1,
        low,    // The value is less than the interval's lower bound
        match,  // The value is an element of the interval
        high,   // The value is greater than the interval's upper bound
        empty   // The interval is empty
    )

    CROW_ENUM_CLASS(IntervalOrder, int, -8,
        // Name             Index  Picture    Description
        both_empty,         // -8  ...        Both are empty
        b_only,             // -7  BBB        A is empty, B is not
        a_below_b,          // -6  AAA...BBB  Upper bound of A is less than lower bound of B, with a gap
        a_touches_b,        // -5  AAABBB     Upper bound of A is less than lower bound of B, with no gap
        a_overlaps_b,       // -4  AAA***BBB  Upper bound of A overlaps lower bound of B
        a_extends_below_b,  // -3  AAA***     B is a subset of A, with the same upper bound
        a_encloses_b,       // -2  AAA***AAA  B is a subset of A, matching neither bound
        b_extends_above_a,  // -1  ***BBB     A is a subset of B, with the same lower bound
        equal,              // 0   ***        A and B are the same
        a_extends_above_b,  // 1   ***AAA     B is a subset of A, with the same lower bound
        b_encloses_a,       // 2   BBB***BBB  A is a subset of B, matching neither bound
        b_extends_below_a,  // 3   BBB***     A is a subset of B, with the same upper bound
        b_overlaps_a,       // 4   BBB***AAA  Upper bound of B overlaps lower bound of A
        b_touches_a,        // 5   BBBAAA     Upper bound of B is less than lower bound of A, with no gap
        b_below_a,          // 6   BBB...AAA  Upper bound of B is less than lower bound of A, with a gap
        a_only              // 7   AAA        B is empty, A is not
    )

    template <typename T>
    struct IntervalTraits {
        using type = std::remove_cv_t<T>;
        static constexpr IntervalCategory category =
            std::same_as<type, bool> || ! std::regular<type> || ! std::totally_ordered<type> ?
                IntervalCategory::none :
            std::floating_point<T>
                    || (std::numeric_limits<type>::is_specialized && ! std::numeric_limits<type>::is_integer) ?
                IntervalCategory::continuous :
            std::integral<T>
                    || (std::numeric_limits<type>::is_specialized && std::numeric_limits<type>::is_integer) ?
                IntervalCategory::integral :
            Detail::ArithmeticOperatorsIntervalType<type> && Detail::StepwiseOperatorsIntervalType<type> ?
                IntervalCategory::integral :
            Detail::ArithmeticOperatorsIntervalType<type> ?
                IntervalCategory::continuous :
            Detail::StepwiseOperatorsIntervalType<type> ?
                IntervalCategory::stepwise :
                IntervalCategory::ordered;
    };

    template <typename T> constexpr auto interval_category = IntervalTraits<T>::category;

    template <typename T> concept ContinuousIntervalType = interval_category<T> == IntervalCategory::continuous;
    template <typename T> concept IntegralIntervalType = interval_category<T> == IntervalCategory::integral;
    template <typename T> concept OrderedIntervalType = interval_category<T> == IntervalCategory::ordered;
    template <typename T> concept StepwiseIntervalType = interval_category<T> == IntervalCategory::stepwise;
    template <typename T> concept ArithmeticIntervalType = ContinuousIntervalType<T> || IntegralIntervalType<T>;

    namespace Detail {

        CROW_ENUM_CLASS(BoundaryType, int, -3,
            empty,
            minus_infinity,
            closed,
            open,
            plus_infinity
        )

        template <typename T>
        struct Boundary {
            T value = T();
            BoundaryType type = BoundaryType::empty;
            bool adjacent(const Boundary& b) const noexcept;
            bool compare_ll(const Boundary& b) const noexcept;  // Compare left bounds (closed<open)
            bool compare_rr(const Boundary& b) const noexcept;  // Compare right bounds (open<closed)
            bool compare_lr(const Boundary& b) const noexcept;  // Compare left bound with right
            bool compare_rl(const Boundary& b) const noexcept;  // Compare right bound with left
            bool has_value() const noexcept;
            std::string str() const;
            Boundary<T> operator-() const;
            Boundary<T> operator+(const Boundary<T>& b) const;
            Boundary<T> operator-(const Boundary<T>& b) const { return *this + - b; }
            Boundary<T> operator*(const Boundary<T>& b) const;
            bool operator==(const Boundary<T>& b) const noexcept;
            bool operator!=(const Boundary<T>& b) const noexcept { return ! (*this == b); }

        };

            template <typename T>
            bool Boundary<T>::adjacent(const Boundary& b) const noexcept {
                using BT = BoundaryType;
                using IC = IntervalCategory;
                if (! has_value() || ! b.has_value())
                    return false;
                if (type == BT::open && b.type == BT::open)
                    return false;
                if (value == b.value)
                    return type != b.type;
                if constexpr (interval_category<T> == IC::stepwise || interval_category<T> == IC::integral) {
                    if (type == BT::closed && b.type == BT::closed) {
                        if (value < b.value) {
                            T t = value;
                            return ++t == b.value;
                        } else if (b.value < value) {
                            T t = b.value;
                            return ++t == value;
                        }
                    }
                }
                return false;
            }

            template <typename T>
            bool Boundary<T>::compare_ll(const Boundary& b) const noexcept {
                if (has_value() && b.has_value() && value != b.value)
                    return value < b.value;
                else
                    return type < b.type;
            }

            template <typename T>
            bool Boundary<T>::compare_rr(const Boundary& b) const noexcept {
                if (! has_value() || ! b.has_value())
                    return type < b.type;
                else if (value != b.value)
                    return value < b.value;
                else
                    return type > b.type;
            }

            template <typename T>
            bool Boundary<T>::compare_lr(const Boundary& b) const noexcept {
                if (! has_value() || ! b.has_value())
                    return type < b.type;
                else if (value != b.value)
                    return value < b.value;
                else
                    return false;
            }

            template <typename T>
            bool Boundary<T>::compare_rl(const Boundary& b) const noexcept {
                using BT = BoundaryType;
                if (! has_value() || ! b.has_value())
                    return type < b.type;
                else if (value != b.value)
                    return value < b.value;
                else
                    return type == BT::open || b.type == BT::open;
            }

            template <typename T>
            bool Boundary<T>::has_value() const noexcept {
                using BT = BoundaryType;
                return type == BT::closed || type == BT::open;
            }

            template <typename T>
            std::string Boundary<T>::str() const {
                using BT = BoundaryType;
                switch (type) {
                    case BT::empty:           return "{}";
                    case BT::minus_infinity:  return "-inf";
                    case BT::plus_infinity:   return "+inf";
                    default:                   break;
                }
                std::string s;
                if (type == BT::open)
                    s += '(';
                s += format_object(value);
                if (type == BT::open)
                    s += ')';
                return s;
            }

            template <typename T>
            Boundary<T> Boundary<T>::operator-() const {
                using BT = BoundaryType;
                auto b = *this;
                if (has_value())
                    b.value = - b.value;
                if (type == BT::minus_infinity)
                    b.type = BT::plus_infinity;
                else if (type == BT::plus_infinity)
                    b.type = BT::minus_infinity;
                return b;
            }

            template <typename T>
            Boundary<T> Boundary<T>::operator+(const Boundary<T>& b) const {
                // We will only be adding like to like (lower or upper bounds),
                // so minus_infinity+plus_infinity will never happen.
                using BT = BoundaryType;
                if (type == BT::empty || b.type == BT::empty)
                    return {};
                else if (! has_value())
                    return *this;
                else if (! b.has_value())
                    return b;
                Boundary<T> c = {value + b.value, BT::open};
                if (type == BT::closed && b.type == BT::closed)
                    c.type = BT::closed;
                return c;
            }

            template <typename T>
            Boundary<T> Boundary<T>::operator*(const Boundary& b) const {

                using BT = BoundaryType;

                // If either argument is empty, the result is empty
                if (type == BT::empty || b.type == BT::empty)
                    return {};

                // Use symmetry to handle negative arguments
                bool a_minus = type == BT::minus_infinity || (has_value() && value < T());
                bool b_minus = b.type == BT::minus_infinity || (b.has_value() && b.value < T());
                if (a_minus && b_minus)
                    return - *this * - b;
                else if (a_minus)
                    return - (- *this * b);
                else if (b_minus)
                    return - (*this * - b);

                // Use symmetry to ensure a>=b
                if ((type == BT::minus_infinity && b.type != BT::minus_infinity)
                        || (b.type == BT::plus_infinity && type != BT::plus_infinity)
                        || (has_value() && b.has_value() && value < b.value))
                    return b * *this;

                // If either argument is a closed zero, the result is a closed zero
                if ((type == BT::closed && value == T())
                        || (b.type == BT::closed && b.value == T()))
                    return {T(), BT::closed};

                // If either argument is an open zero, the result is an open zero
                if ((type == BT::open && value == T())
                        || (b.type == BT::open && b.value == T()))
                    return {T(), BT::open};

                // If either argument is positive infinity, the result is positive infinity
                if (type == BT::plus_infinity)
                    return *this;

                // If both arguments are closed, the result is closed; otherwise, the result is open
                BT t = type == BT::closed && b.type == BT::closed ? BT::closed : BT::open;
                return {value * b.value, t};

            }

            template <typename T>
            bool Boundary<T>::operator==(const Boundary<T>& b) const noexcept {
                if (type != b.type)
                    return false;
                else if (has_value() && b.has_value())
                    return value == b.value;
                else
                    return true;
            }

            template <typename T>
            std::ostream& operator<<(std::ostream& out, const Boundary<T>& b) {
                return out << b.str();
            }

    }

}
