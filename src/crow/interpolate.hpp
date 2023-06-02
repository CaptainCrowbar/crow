#pragma once

#include "crow/binary.hpp"
#include "crow/enum.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <initializer_list>
#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    // Supporting types

    enum class Logs: uint8_t {
        none  = 0,
        X     = 1,
        Y     = 2,
        XY    = 3,
    };

    CROW_BITMASK_OPERATORS(Logs)

    namespace Detail {

        template <typename T, bool Log>
        struct LerpTraits;

        template <typename T>
        struct LerpTraits<T, false> {
            static constexpr void acquire(T&) noexcept {}
            static constexpr void release(T&) noexcept {}
            static constexpr void checked_acquire(T&) noexcept {}
        };

        template <std::floating_point T>
        struct LerpTraits<T, true> {
            static constexpr void acquire(T& t) noexcept { t = std::log(t); }
            static constexpr void release(T& t) noexcept { t = std::exp(t); }
            static void checked_acquire(T& t) {
                if (t <= 0)
                    throw std::invalid_argument("Invalid reference point for logarithmic interpolation");
                acquire(t);
            }
        };

    }

    // Concepts

    template <typename X, typename Y, Logs L>
    concept LerpTypes =
        std::floating_point<X>
        && (has_bit(L, Logs::Y) ? std::floating_point<Y> : std::regular<Y>)
        && requires (X x, Y y) {
            { y + x * (y - y) } -> std::convertible_to<Y>;
        };

    template <typename X, typename Y, Logs L>
    concept ConstexprLerpTypes =
        LerpTypes<X, Y, L>
        && requires {
            { std::bool_constant<(Y() + X() * (Y() - Y()), true)>() } -> std::same_as<std::true_type>;
        };

    // Two-point interpolation

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y lerp(Y y1, Y y2, X x) {
        return y1 + x * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y lerp(Y y1, Y y2, X x) noexcept {
        return y1 + x * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y lerp(X x1, Y y1, X x2, Y y2, X x) {
        return lerp(y1, y2, (x - x1) / (x2 - x1));
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y lerp(X x1, Y y1, X x2, Y y2, X x) noexcept {
        return lerp(y1, y2, (x - x1) / (x2 - x1));
    }

    template <Logs L = Logs::none>
    struct Lerp {
        static constexpr Logs flags = L;
        template <typename X, typename Y>
            requires (LerpTypes<X, Y, L>)
                Y operator()(Y y1, Y y2, X x) const;
        template <typename X, typename Y>
            requires (ConstexprLerpTypes<X, Y, L>)
                constexpr Y operator()(Y y1, Y y2, X x) const noexcept;
        template <typename X, typename Y>
            requires (LerpTypes<X, Y, L>)
                Y operator()(X x1, Y y1, X x2, Y y2, X x) const;
        template <typename X, typename Y>
            requires (ConstexprLerpTypes<X, Y, L>)
                constexpr Y operator()(X x1, Y y1, X x2, Y y2, X x) const noexcept;
    };

        template <Logs L>
        template <typename X, typename Y>
        requires (LerpTypes<X, Y, L>)
        Y Lerp<L>::operator()(Y y1, Y y2, X x) const {
            using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
            using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;
            LTX::acquire(x);
            LTY::acquire(y1);
            LTY::acquire(y2);
            Y y = lerp(y1, y2, x);
            LTY::release(y);
            return y;
        }

        template <Logs L>
        template <typename X, typename Y>
        requires (ConstexprLerpTypes<X, Y, L>)
        constexpr Y Lerp<L>::operator()(Y y1, Y y2, X x) const noexcept {
            using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
            using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;
            LTX::acquire(x);
            LTY::acquire(y1);
            LTY::acquire(y2);
            Y y = lerp(y1, y2, x);
            LTY::release(y);
            return y;
        }

        template <Logs L>
        template <typename X, typename Y>
        requires (LerpTypes<X, Y, L>)
        Y Lerp<L>::operator()(X x1, Y y1, X x2, Y y2, X x) const {
            using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
            using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;
            LTX::acquire(x1);
            LTX::acquire(x2);
            LTX::acquire(x);
            LTY::acquire(y1);
            LTY::acquire(y2);
            Y y = lerp(x1, y1, x2, y2, x);
            LTY::release(y);
            return y;
        }

        template <Logs L>
        template <typename X, typename Y>
        requires (ConstexprLerpTypes<X, Y, L>)
        constexpr Y Lerp<L>::operator()(X x1, Y y1, X x2, Y y2, X x) const noexcept {
            using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
            using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;
            LTX::acquire(x1);
            LTX::acquire(x2);
            LTX::acquire(x);
            LTY::acquire(y1);
            LTY::acquire(y2);
            Y y = lerp(x1, y1, x2, y2, x);
            LTY::release(y);
            return y;
        }

    // Related functions

    namespace Detail {

        template <std::floating_point X>
        constexpr X smoothstep_S1(X x) noexcept {
            return x * x * (3 - 2 * x);
        }

        template <std::floating_point X>
        constexpr X smoothstep_S2(X x) noexcept {
            return x * x * x * (3 * x * (2 * x - 5) + 10);
        }

    }

    template <std::floating_point X>
    constexpr X smoothstep(X x) noexcept {
        return x <= 0 ? 0 : x >= 1 ? 1 :
            Detail::smoothstep_S1(x);
    }

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y smoothstep(Y y1, Y y2, X x) {
        return x <= 0 ? y1 : x >= 1 ? y2 :
            y1 + Detail::smoothstep_S1(x) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y smoothstep(Y y1, Y y2, X x) noexcept {
        return x <= 0 ? y1 : x >= 1 ? y2 :
            y1 + Detail::smoothstep_S1(x) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y smoothstep(X x1, Y y1, X x2, Y y2, X x) {
        return x <= x1 ? y1 : x >= x2 ? y2 :
            y1 + Detail::smoothstep_S1((x - x1) / (x2 - x1)) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y smoothstep(X x1, Y y1, X x2, Y y2, X x) noexcept {
        return x <= x1 ? y1 : x >= x2 ? y2 :
            y1 + Detail::smoothstep_S1((x - x1) / (x2 - x1)) * (y2 - y1);
    }

    template <std::floating_point X>
    constexpr X smootherstep(X x) noexcept {
        return x <= 0 ? 0 : x >= 1 ? 1 :
            Detail::smoothstep_S2(x);
    }

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y smootherstep(Y y1, Y y2, X x) {
        return x <= 0 ? y1 : x >= 1 ? y2 :
            y1 + Detail::smoothstep_S2(x) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y smootherstep(Y y1, Y y2, X x) noexcept {
        return x <= 0 ? y1 : x >= 1 ? y2 :
            y1 + Detail::smoothstep_S2(x) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (LerpTypes<X, Y, Logs::none>)
    Y smootherstep(X x1, Y y1, X x2, Y y2, X x) {
        return x <= x1 ? y1 : x >= x2 ? y2 :
            y1 + Detail::smoothstep_S2((x - x1) / (x2 - x1)) * (y2 - y1);
    }

    template <typename X, typename Y>
    requires (ConstexprLerpTypes<X, Y, Logs::none>)
    constexpr Y smootherstep(X x1, Y y1, X x2, Y y2, X x) noexcept {
        return x <= x1 ? y1 : x >= x2 ? y2 :
            y1 + Detail::smoothstep_S2((x - x1) / (x2 - x1)) * (y2 - y1);
    }

    // Interpolated map

    template <typename X, typename Y = X, Logs L = Logs::none>
    requires (LerpTypes<X, Y, L>)
    class LerpMap {

    public:

        using key_type = X;
        using value_type = Y;

        static constexpr Logs flags = L;

        LerpMap() = default;
        LerpMap(std::initializer_list<std::pair<const X, Y>> list);
        explicit LerpMap(const std::vector<std::pair<X, Y>> points);

        LerpMap& insert(X x, Y y);
        Y operator[](X x) const;

    private:

        using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
        using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;

        std::map<X, Y> map_;

        void init();

    };

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        LerpMap<X, Y, L>::LerpMap(std::initializer_list<std::pair<const X, Y>> list) {
            for (auto& [x,y]: list)
                insert(x, y);
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        LerpMap<X, Y, L>::LerpMap(const std::vector<std::pair<X, Y>> points) {
            for (auto& [x,y]: points)
                insert(x, y);
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        LerpMap<X, Y, L>& LerpMap<X, Y, L>::insert(X x, Y y) {
            LTX::checked_acquire(x);
            LTY::checked_acquire(y);
            map_[x] = y;
            return *this;
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        Y LerpMap<X, Y, L>::operator[](X x) const {

            if (map_.empty())
                return Y();

            Y y;

            if (map_.size() == 1) {

                y = map_.begin()->second;

            } else {

                LTX::acquire(x);
                auto i = map_.lower_bound(x);

                if (i != map_.end() && i->first == x) {

                    y = i->second;

                } else {

                    if (i == map_.end())
                        --i;

                    auto j = i;

                    if (i == map_.begin())
                        ++j;
                    else
                        --i;

                    y = lerp(i->first, i->second, j->first, j->second, x);

                }

            }

            LTY::release(y);
            return y;

        }

    // Piecewise interpolated map

    template <typename X, typename Y = X, Logs L = Logs::none>
    requires (LerpTypes<X, Y, L>)
    class SegmentMap {

    private:

        struct init_type;

    public:

        using key_type = X;
        using value_type = Y;

        static constexpr Logs flags = L;

        SegmentMap() = default;
        SegmentMap(std::initializer_list<init_type> list);

        Y operator[](X x) const;
        void clear() noexcept { map_.clear(); }
        bool empty() const noexcept { return map_.empty(); }
        void insert(X x, Y y);
        void insert(X x, Y y1, Y y2);
        void insert(X x, Y y1, Y y2, Y y3);
        void erase(X x) noexcept { map_.erase(x); }
        void erase(X x1, X x2) noexcept;
        X min() const noexcept { return map_.empty() ? X() : map_.begin()->first; }
        X max() const noexcept { return map_.empty() ? X() : std::prev(map_.end())->first; }

    private:

        using LTX = Detail::LerpTraits<X, has_bit(L, Logs::X)>;
        using LTY = Detail::LerpTraits<Y, has_bit(L, Logs::Y)>;

        struct entry_type {
            X x_key;
            Y y_left, y_mid, y_right;
        };

        std::map<X, entry_type> map_;

        static Y midpoint(Y a, Y b) { return a + X(0.5) * (b - a); }

    };

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        struct SegmentMap<X, Y, L>::init_type {

            X x_orig, x_key;
            Y y_left, y_mid, y_right;

            init_type(X x, Y y):
            x_orig(x), x_key(x), y_left(y) {
                LTX::checked_acquire(x_key);
                LTY::checked_acquire(y_left);
                y_mid = y_right = y_left;
            }

            init_type(X x, Y y1, Y y2):
            x_orig(x), x_key(x), y_left(y1), y_right(y2) {
                LTX::checked_acquire(x_key);
                LTY::checked_acquire(y_left);
                LTY::checked_acquire(y_right);
                y_mid = midpoint(y_left, y_right);
            }

            init_type(X x, Y y1, Y y2, Y y3):
            x_orig(x), x_key(x), y_left(y1), y_mid(y2), y_right(y3) {
                LTX::checked_acquire(x_key);
                LTY::checked_acquire(y_left);
                LTY::checked_acquire(y_mid);
                LTY::checked_acquire(y_right);
            }

        };

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        SegmentMap<X, Y, L>::SegmentMap(std::initializer_list<init_type> list) {
            for (auto& item: list)
                map_.insert({item.x_orig, {item.x_key, item.y_left, item.y_mid, item.y_right}});
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        Y SegmentMap<X, Y, L>::operator[](X x) const {
            if (map_.empty())
                return Y();
            auto i = map_.lower_bound(x);
            Y y;
            if (i == map_.end()) {
                y = std::prev(i)->second.y_right;
            } else if (i->first == x) {
                y = i->second.y_mid;
            } else if (i == map_.begin()) {
                y = i->second.y_left;
            } else {
                auto j = std::prev(i);
                LTX::acquire(x);
                y = lerp(j->second.x_key, j->second.y_right, i->second.x_key, i->second.y_left, x);
            }
            LTY::release(y);
            return y;
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        void SegmentMap<X, Y, L>::insert(X x, Y y) {
            X x_key = x;
            LTX::checked_acquire(x_key);
            LTY::checked_acquire(y);
            map_[x] = {x_key, y, y, y};
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        void SegmentMap<X, Y, L>::insert(X x, Y y1, Y y2) {
            X x_key = x;
            LTX::checked_acquire(x_key);
            LTY::checked_acquire(y1);
            LTY::checked_acquire(y2);
            Y y_mid = midpoint(y1, y2);
            map_[x] = {x_key, y1, y_mid, y2};
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        void SegmentMap<X, Y, L>::insert(X x, Y y1, Y y2, Y y3) {
            X x_key = x;
            LTX::checked_acquire(x_key);
            LTY::checked_acquire(y1);
            LTY::checked_acquire(y2);
            LTY::checked_acquire(y3);
            map_[x] = {x_key, y1, y2, y3};
        }

        template <typename X, typename Y, Logs L>
        requires (LerpTypes<X, Y, L>)
        void SegmentMap<X, Y, L>::erase(X x1, X x2) noexcept {
            if (x1 <= x2) {
                auto i = map_.lower_bound(x1);
                auto j = map_.upper_bound(x2);
                map_.erase(i, j);
            }
        }

    // Cubic spline interpolated map

    template <std::floating_point T, Logs L = Logs::none>
    class CubicSplineMap {

    public:

        using key_type = T;

        static constexpr Logs flags = L;

        CubicSplineMap() = default;
        CubicSplineMap(std::initializer_list<std::pair<T, T>> list): points_(list) { init(); }
        explicit CubicSplineMap(const std::vector<std::pair<T, T>> points): points_(points) { init(); }

        T operator[](T x) const noexcept;

    private:

        using LTX = Detail::LerpTraits<T, has_bit(L, Logs::X)>;
        using LTY = Detail::LerpTraits<T, has_bit(L, Logs::Y)>;

        std::vector<std::pair<T, T>> points_;
        std::vector<T> deriv2_;

        void init();
        T px(ptrdiff_t i) const noexcept { return points_[i].first; }
        T py(ptrdiff_t i) const noexcept { return points_[i].second; }

    };

        template <std::floating_point T, Logs L>
        T CubicSplineMap<T, L>::operator[](T x) const noexcept {

            LTX::acquire(x);

            auto it = std::upper_bound(points_.begin(), points_.end(), std::make_pair(x, 0.0));

            if (it == points_.end())
                --it;
            if (it != points_.begin())
                --it;

            auto j = it - points_.begin();
            T dx = px(j + 1) - px(j);
            T q_prev = (x - px(j)) / dx;
            T q_next = (px(j + 1) - x) / dx;
            T qp_cubic = q_prev * (q_prev * q_prev - 1);
            T qn_cubic = q_next * (q_next * q_next - 1);
            T d20 = deriv2_[j];
            T d21 = deriv2_[j + 1];
            T y = q_next * py(j) + q_prev * py(j + 1) + dx * dx * (qn_cubic * d20 + qp_cubic * d21) / 6;
            LTY::release(y);

            return y;

        }

        template <std::floating_point T, Logs L>
        void CubicSplineMap<T, L>::init() {

            if (points_.size() < 4)
                throw std::invalid_argument("Not enough points for cubic spline (requires 4)");

            std::sort(points_.begin(), points_.end(),
                [] (auto& a, auto& b) { return a.first == b.first ? a.second > b.second : a.first < b.first; });
            auto it = std::unique(points_.begin(), points_.end(),
                [] (auto& a, auto& b) { return a.first == b.first; });
            points_.erase(it, points_.end());

            if constexpr (has_bit(L, Logs::XY)) {
                for (auto& [x,y]: points_) {
                    LTX::checked_acquire(x);
                    LTY::checked_acquire(y);
                }
            }

            deriv2_.resize(points_.size(), 0);
            std::vector<T> d2_offset(points_.size() - 1, 0);
            auto n = int(points_.size());

            for (int i = 1; i < n - 1; ++i) {
                T dx_prev = px(i) - px(i - 1);
                T dx_next = px(i + 1) - px(i);
                T dy_prev = py(i) - py(i - 1);
                T dy_next = py(i + 1) - py(i);
                T delta_d = dy_next / dx_next - dy_prev / dx_prev;
                T divisor = dx_prev * (deriv2_[i - 1] + 2) + dx_next * 2;
                deriv2_[i] = - dx_next / divisor;
                d2_offset[i] = (delta_d * 6 - dx_prev * d2_offset[i - 1]) / divisor;
            }

            for (int i = n - 2; i >= 0; --i)
                deriv2_[i] = deriv2_[i] * deriv2_[i + 1] + d2_offset[i];

        }

}
