#pragma once

#include "crow/flexible-map.hpp"
#include "crow/format.hpp"
#include "crow/random.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <compare>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace Crow {

    template <std::equality_comparable T>
    class FlatSuperposition {

    private:

        using branch_set = FlexibleSet<T>;
        using branch_iterator = typename branch_set::const_iterator;

    public:

        using value_type = T;
        using iterator = typename branch_set::const_iterator;

        FlatSuperposition() = default;
        FlatSuperposition(const T& x): branches_{{x}} {}
        FlatSuperposition(std::initializer_list<T> list): branches_(list) {}
        FlatSuperposition(std::initializer_list<FlatSuperposition> list);
        FlatSuperposition(const FlatSuperposition& s) = default;
        FlatSuperposition(FlatSuperposition&& s) noexcept;
        ~FlatSuperposition() = default;
        FlatSuperposition& operator=(const FlatSuperposition& s) = default;
        FlatSuperposition& operator=(FlatSuperposition&& s) noexcept;

        template <std::regular_invocable<const T&> UF>
            auto apply(UF uf) const
            -> FlatSuperposition<std::invoke_result_t<UF, T>>;
        template <std::regular_invocable<const T&, const T&> BF, std::equality_comparable U>
            auto apply(BF bf, const FlatSuperposition<U>& s) const
            -> FlatSuperposition<std::invoke_result_t<BF, T, U>>;

        iterator begin() const { return branches_.begin(); }
        iterator end() const { return branches_.end(); }
        RandomChoice<T> choice() const { return RandomChoice<T>(branches_); }
        FlatSuperposition<std::strong_ordering> compare(const FlatSuperposition& s) const;
        std::partial_ordering partial_compare(const FlatSuperposition& s) const;
        T min(std::enable_if<std::totally_ordered<T>>* = nullptr) const { return *branches_.begin(); }
        T max(std::enable_if<std::totally_ordered<T>>* = nullptr) const { return *std::prev(branches_.end()); }
        FlatSuperposition set_difference(const FlatSuperposition& y) const;
        FlatSuperposition set_intersection(const FlatSuperposition& y) const;
        FlatSuperposition set_symmetric_difference(const FlatSuperposition& y) const;
        FlatSuperposition set_union(const FlatSuperposition& y) const;
        size_t size() const noexcept { return branches_.size(); }
        bool empty() const noexcept { return branches_.empty(); }
        bool is_unique() const noexcept { return branches_.size() == 1; }
        std::string str(const FormatSpec& spec = {}) const;
        const T& value() const;

        const T& operator*() const { return *branches_.begin(); }
        FlatSuperposition operator+() const { return *this; }
        FlatSuperposition operator-() const { return apply(std::negate<T>()); }
        template <typename U> FlatSuperposition& operator+=(const FlatSuperposition<U>& y)
            { return *this = apply(Plus<T, U>(), y); }
        template <typename U> FlatSuperposition& operator-=(const FlatSuperposition<U>& y)
            { return *this = apply(Minus<T, U>(), y); }
        template <typename U> FlatSuperposition& operator*=(const FlatSuperposition<U>& y)
            { return *this = apply(Multiplies<T, U>(), y); }
        template <typename U> FlatSuperposition& operator/=(const FlatSuperposition<U>& y)
            { return *this = apply(Divides<T, U>(), y); }

        friend FlatSuperposition operator+(const FlatSuperposition& x, const FlatSuperposition& y)
            { return x.apply(Plus<T>(), y); }
        friend FlatSuperposition operator-(const FlatSuperposition& x, const FlatSuperposition& y)
            { return x.apply(Minus<T>(), y); }
        friend FlatSuperposition operator*(const FlatSuperposition& x, const FlatSuperposition& y)
            { return x.apply(Multiplies<T>(), y); }
        friend FlatSuperposition operator/(const FlatSuperposition& x, const FlatSuperposition& y)
            { return x.apply(Divides<T>(), y); }
        friend bool operator==(const FlatSuperposition& x, const FlatSuperposition& y)
            { return (x.empty() && y.empty()) || (x.is_unique() && y.is_unique() && *x == *y); }
        friend std::partial_ordering operator<=>(const FlatSuperposition& x, const FlatSuperposition& y)
            { return x.compare(y); }
        friend std::ostream& operator<<(std::ostream& out, const FlatSuperposition& x)
            { return out << x.str(); }

    private:

        branch_set branches_;

    };

        template <std::equality_comparable T>
        FlatSuperposition<T>::FlatSuperposition(std::initializer_list<FlatSuperposition> list) {
            for (auto& s: list)
                std::copy(s.branches_.begin(), s.branches_.end(), append(branches_));
        }

        template <std::equality_comparable T>
        FlatSuperposition<T>::FlatSuperposition(FlatSuperposition&& s) noexcept:
        branches_(std::move(s.branches_)) {
            s.branches_.clear();
        }

        template <std::equality_comparable T>
        FlatSuperposition<T>& FlatSuperposition<T>::operator=(FlatSuperposition&& s) noexcept {
            if (&s != this) {
                branches_ = std::move(s.branches_);
                s.branches_.clear();
            }
            return *this;
        }

        template <std::equality_comparable T>
        template <std::regular_invocable<const T&> UF>
        auto FlatSuperposition<T>::apply(UF uf) const
        -> FlatSuperposition<std::invoke_result_t<UF, T>> {
            FlatSuperposition<std::invoke_result_t<UF, T>> result;
            for (auto& x: branches_)
                result.branches_.insert(uf(x));
            return result;
        }

        template <std::equality_comparable T>
        template <std::regular_invocable<const T&, const T&> BF, std::equality_comparable U>
        auto FlatSuperposition<T>::apply(BF bf, const FlatSuperposition<U>& s) const
        -> FlatSuperposition<std::invoke_result_t<BF, T, U>> {
            FlatSuperposition<std::invoke_result_t<BF, T, U>> result;
            for (auto& x: branches_)
                for (auto& y: s.branches_)
                    result.branches_.insert(bf(x, y));
            return result;
        }

        template <std::equality_comparable T>
        FlatSuperposition<std::strong_ordering> FlatSuperposition<T>::compare(const FlatSuperposition& s) const {
            FlatSuperposition<std::strong_ordering> result;
            for (auto& x: branches_)
                for (auto& y: s.branches_)
                    result.branches_.insert(x <=> y);
            return result;
        }

        template <std::equality_comparable T>
        std::partial_ordering FlatSuperposition<T>::partial_compare(const FlatSuperposition& s) const {
            auto c = compare(s);
            if (c.is_unique())
                return *c;
            else
                return std::partial_ordering::unordered;
        }

        template <std::equality_comparable T>
        FlatSuperposition<T> FlatSuperposition<T>::set_difference(const FlatSuperposition<T>& y) const {
            FlatSuperposition z;
            std::set_difference(branches_.begin(), branches_.end(),
                y.branches_.begin(), y.branches_.end(), append(z.branches_));
            return z;
        }

        template <std::equality_comparable T>
        FlatSuperposition<T> FlatSuperposition<T>::set_intersection(const FlatSuperposition<T>& y) const {
            FlatSuperposition z;
            std::set_intersection(branches_.begin(), branches_.end(),
                y.branches_.begin(), y.branches_.end(), append(z.branches_));
            return z;
        }

        template <std::equality_comparable T>
        FlatSuperposition<T> FlatSuperposition<T>::set_symmetric_difference(const FlatSuperposition<T>& y) const {
            FlatSuperposition z;
            std::set_symmetric_difference(branches_.begin(), branches_.end(),
                y.branches_.begin(), y.branches_.end(), append(z.branches_));
            return z;
        }

        template <std::equality_comparable T>
        FlatSuperposition<T> FlatSuperposition<T>::set_union(const FlatSuperposition<T>& y) const {
            FlatSuperposition z;
            std::set_union(branches_.begin(), branches_.end(),
                y.branches_.begin(), y.branches_.end(), append(z.branches_));
            return z;
        }

        template <std::equality_comparable T>
        std::string FlatSuperposition<T>::str(const FormatSpec& spec) const {
            switch (branches_.size()) {
                case 0:   return "null";
                case 1:   return format_object(**this, spec);
                default:  return format_range(branches_, spec);
            }
        }

        template <std::equality_comparable T>
        const T& FlatSuperposition<T>::value() const {
            if (! is_unique())
                throw std::invalid_argument("Non-unique superposition");
            return **this;
        }

    template <std::equality_comparable T, NonIntegralNumericType W = double>
    class WeightedSuperposition {

    private:

        using branch_map = FlexibleMap<T, W>;

    public:

        using scalar_type = T;
        using weight_type = W;
        using value_type = typename branch_map::value_type;
        using iterator = typename branch_map::const_iterator;

        WeightedSuperposition() = default;
        WeightedSuperposition(const T& x): branches_{{x, W(1)}} {}
        template <typename U, typename... Args> WeightedSuperposition(U&& u, W w, Args&&... args);
        WeightedSuperposition(const WeightedSuperposition& s) = default;
        WeightedSuperposition(WeightedSuperposition&& s) noexcept;
        ~WeightedSuperposition() = default;
        WeightedSuperposition& operator=(const WeightedSuperposition& s) = default;
        WeightedSuperposition& operator=(WeightedSuperposition&& s) noexcept;

        template <std::regular_invocable<const T&> UF>
            auto apply(UF uf) const
            -> WeightedSuperposition<std::invoke_result_t<UF, T>>;
        template <std::regular_invocable<const T&, const T&> BF, std::equality_comparable U>
            auto apply(BF bf, const WeightedSuperposition<U>& s) const
            -> WeightedSuperposition<std::invoke_result_t<BF, T, U>>;

        iterator begin() const { return branches_.begin(); }
        iterator end() const { return branches_.end(); }
        WeightedChoice<T> choice() const;
        WeightedSuperposition<std::strong_ordering> compare(const WeightedSuperposition& s) const;
        std::partial_ordering partial_compare(const WeightedSuperposition& s) const;
        W mean(std::enable_if<std::constructible_from<W, T>>* = nullptr) const;
        W sd(std::enable_if<std::floating_point<W> && std::constructible_from<W, T>>* = nullptr) const;
        T min(std::enable_if<std::totally_ordered<T>>* = nullptr) const { return branches_.begin()->first; }
        T max(std::enable_if<std::totally_ordered<T>>* = nullptr) const { return std::prev(branches_.end())->first; }
        size_t size() const noexcept { return branches_.size(); }
        bool empty() const noexcept { return branches_.empty(); }
        bool is_unique() const noexcept { return branches_.size() == 1; }
        std::string str(const FormatSpec& spec1 = {}, const FormatSpec& spec2 = {}) const;
        const T& value() const;

        const T& operator*() const { return branches_.begin()->first; }
        WeightedSuperposition operator+() const { return *this; }
        WeightedSuperposition operator-() const { return apply(std::negate<T>()); }
        template <typename U> WeightedSuperposition& operator+=(const WeightedSuperposition<U>& y)
            { return *this = apply(Plus<T, U>(), y); }
        template <typename U> WeightedSuperposition& operator-=(const WeightedSuperposition<U>& y)
            { return *this = apply(Minus<T, U>(), y); }
        template <typename U> WeightedSuperposition& operator*=(const WeightedSuperposition<U>& y)
            { return *this = apply(Multiplies<T, U>(), y); }
        template <typename U> WeightedSuperposition& operator/=(const WeightedSuperposition<U>& y)
            { return *this = apply(Divides<T, U>(), y); }

        friend WeightedSuperposition operator+(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return x.apply(Plus<T>(), y); }
        friend WeightedSuperposition operator-(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return x.apply(Minus<T>(), y); }
        friend WeightedSuperposition operator*(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return x.apply(Multiplies<T>(), y); }
        friend WeightedSuperposition operator/(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return x.apply(Divides<T>(), y); }
        friend bool operator==(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return (x.empty() && y.empty()) || (x.is_unique() && y.is_unique() && *x == *y); }
        friend std::partial_ordering operator<=>(const WeightedSuperposition& x, const WeightedSuperposition& y)
            { return x.compare(y); }
        friend std::ostream& operator<<(std::ostream& out, const WeightedSuperposition& x)
            { return out << x.str(); }

    private:

        class uninit {};

        branch_map branches_;

        explicit WeightedSuperposition(uninit) {}

        void append() {}
        template <typename... Args> void append(const T& t, W w, Args&&... args);
        template <typename... Args> void append(const WeightedSuperposition& s, W w, Args&&... args);
        void normalise();

    };

        template <std::equality_comparable T, NonIntegralNumericType W>
        template <typename U, typename... Args>
        WeightedSuperposition<T, W>::WeightedSuperposition(U&& u, W w, Args&&... args) {
            append(std::forward<U>(u), w, std::forward<Args>(args)...);
            normalise();
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        WeightedSuperposition<T, W>::WeightedSuperposition(WeightedSuperposition&& s) noexcept:
        branches_(std::move(s.branches_)) {
            s.branches_.clear();
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        WeightedSuperposition<T, W>& WeightedSuperposition<T, W>::operator=(WeightedSuperposition&& s) noexcept {
            if (&s != this) {
                branches_ = std::move(s.branches_);
                s.branches_.clear();
            }
            return *this;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        template <std::regular_invocable<const T&> UF>
        auto WeightedSuperposition<T, W>::apply(UF uf) const
        -> WeightedSuperposition<std::invoke_result_t<UF, T>> {
            WeightedSuperposition<std::invoke_result_t<UF, T>> result{uninit{}};
            for (auto& [x,f]: *this)
                result.branches_[uf(x)] += f;
            return result;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        template <std::regular_invocable<const T&, const T&> BF, std::equality_comparable U>
        auto WeightedSuperposition<T, W>::apply(BF bf, const WeightedSuperposition<U>& s) const
        -> WeightedSuperposition<std::invoke_result_t<BF, T, U>> {
            WeightedSuperposition<std::invoke_result_t<BF, T, U>> result{uninit{}};
            for (auto& [x,f]: *this)
                for (auto& [y,g]: s.branches_)
                    result.branches_[bf(x, y)] += f * g;
            return result;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        WeightedChoice<T> WeightedSuperposition<T, W>::choice() const {
            WeightedChoice<T> c;
            for (auto& [x,f]: *this)
                c.add(double(f), x);
            return c;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        WeightedSuperposition<std::strong_ordering> WeightedSuperposition<T, W>::compare(const WeightedSuperposition& s) const {
            WeightedSuperposition<std::strong_ordering> result{uninit{}};
            for (auto& [x,f]: *this)
                for (auto& [y,g]: s)
                    result.branches_[x <=> y] += f * g;
            return result;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        std::partial_ordering WeightedSuperposition<T, W>::partial_compare(const WeightedSuperposition& s) const {
            auto c = compare(s);
            if (c.is_unique())
                return *c;
            else
                return std::partial_ordering::unordered;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        std::string WeightedSuperposition<T, W>::str(const FormatSpec& spec1, const FormatSpec& spec2) const {
            switch (branches_.size()) {
                case 0:   return "null";
                case 1:   return format_object(**this, spec1);
                default:  return format_map(branches_, spec1, spec2);
            }
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        const T& WeightedSuperposition<T, W>::value() const {
            if (! is_unique())
                throw std::invalid_argument("Non-unique superposition");
            return **this;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        W WeightedSuperposition<T, W>::mean(std::enable_if<std::constructible_from<W, T>>*) const {
            W m = 0;
            for (auto& [x,f]: *this)
                m += f * W(x);
            return m;
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        W WeightedSuperposition<T, W>::sd(std::enable_if<std::floating_point<W> && std::constructible_from<W, T>>*) const {
            W m1 = 0;
            W m2 = 0;
            for (auto& [t,f]: *this) {
                W x = W(t);
                W fx = f * W(x);
                m1 += fx;
                m2 += fx * x;
            }
            W v = m2 - m1 * m1;
            return std::sqrt(v);
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        template <typename... Args>
        void WeightedSuperposition<T, W>::append(const T& t, W w, Args&&... args) {
            if (w > 0)
                branches_[t] += w;
            append(std::forward<Args>(args)...);
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        template <typename... Args>
        void WeightedSuperposition<T, W>::append(const WeightedSuperposition& s, W w, Args&&... args) {
            if (w > 0)
                for (auto& [t,wt]: s.branches_)
                    branches_[t] += w * wt;
            append(std::forward<Args>(args)...);
        }

        template <std::equality_comparable T, NonIntegralNumericType W>
        void WeightedSuperposition<T, W>::normalise() {
            if (branches_.size() == 1) {
                branches_.begin()->second = 1;
            } else if (branches_.size() > 1) {
                W sum = 0;
                for (auto& [_,f]: branches_)
                    sum += f;
                W factor = W(1) / sum;
                for (auto& [_,f]: branches_)
                    f *= factor;
            }
        }

}
