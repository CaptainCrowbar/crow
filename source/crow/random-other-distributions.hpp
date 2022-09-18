// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-discrete-distributions.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/uuid.hpp"
#include <algorithm>
#include <array>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T, typename = void> struct HasMinMethod: std::false_type {};
        template <typename T> struct HasMinMethod<T, std::void_t<decltype(std::declval<const T&>().min())>>: std::true_type {};
        template <typename T> constexpr bool has_min_method = HasMinMethod<T>::value;

        template <typename T, typename = void> struct HasMaxMethod: std::false_type {};
        template <typename T> struct HasMaxMethod<T, std::void_t<decltype(std::declval<const T&>().min())>>: std::true_type {};
        template <typename T> constexpr bool has_max_method = HasMaxMethod<T>::value;

    }

    template <typename Base>
    class ConstrainedDistribution {

    public:

        using base_distribution = Base;
        using result_type = typename Base::result_type;

        ConstrainedDistribution(const Base& dist, result_type min, result_type max);
        template <typename... Args> ConstrainedDistribution(result_type min, result_type max, Args&&... args);

        template <typename RNG> result_type operator()(RNG& rng) const;

        result_type min() const noexcept { return min_; }
        result_type max() const noexcept { return max_; }

    private:

        Base dist_;
        result_type min_ = std::numeric_limits<result_type>::lowest();
        result_type max_ = std::numeric_limits<result_type>::max();

        void check_range();

    };

        template <typename Base>
        ConstrainedDistribution<Base>::ConstrainedDistribution(const Base& dist, result_type min, result_type max):
        dist_(dist), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        template <typename... Args>
        ConstrainedDistribution<Base>::ConstrainedDistribution(result_type min, result_type max, Args&&... args):
        dist_(std::forward<Args>(args)...), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        template <typename RNG>
        typename ConstrainedDistribution<Base>::result_type
        ConstrainedDistribution<Base>::operator()(RNG& rng) const {
            result_type x;
            do x = dist_(rng);
                while (x < min_ || x > max_);
            return x;
        }

        template <typename Base>
        void ConstrainedDistribution<Base>::check_range() {
            if constexpr (Detail::has_min_method<Base>)
                min_ = std::max(min_, dist_.min());
            if constexpr (Detail::has_max_method<Base>)
                max_ = std::min(max_, dist_.max());
            if (min_ > max_)
                throw std::invalid_argument("Constrained distribution has no possible values");
        }

    template <typename T>
    class RandomChoice {

    public:

        using result_type = T;

        RandomChoice() = default;
        RandomChoice(std::initializer_list<T> list): vec_(list) {}

        template <typename Range>
        explicit RandomChoice(const Range& range) {
            using std::begin;
            using std::end;
            vec_.assign(begin(range), end(range));
        }

        template <typename RNG>
        const T& operator()(RNG& rng) const {
            UniformInteger<size_t> dist(vec_.size());
            size_t index = dist(rng);
            return vec_[index];
        }

        template <typename... Args>
        RandomChoice& add(const T& t, const Args&... args) {
            vec_.push_back(t);
            if constexpr (sizeof...(Args) > 0)
                add(args...);
            return *this;
        }

        bool empty() const noexcept { return vec_.empty(); }
        size_t size() const noexcept { return vec_.size(); }

    private:

        std::vector<T> vec_;

    };

    template <typename Range>
    RandomChoice<RangeValue<Range>> random_choice(const Range& range) {
        return RandomChoice<RangeValue<Range>>(range);
    }

    template <typename T>
    RandomChoice<T> random_choice(std::initializer_list<T> list) {
        return RandomChoice<T>(list);
    }

    template <typename T>
    class WeightedChoice {

    private:

        struct group {
            double weight = 0;
            std::vector<T> values;
            group() = default;
            template <typename... Args> group(double w, const T& t, const Args&... args): weight(w), values{t, args...} {}
        };

    public:

        using result_type = T;

        WeightedChoice() = default;

        WeightedChoice(std::initializer_list<group> list) {
            for (auto& g: list)
                add_group(g);
        }

        template <typename RNG>
        const T& operator()(RNG& rng) const {
            double x = dist_(rng);
            auto it = table_.upper_bound(x);
            return it->second;
        }

        template <typename... Args>
        WeightedChoice& add(double w, const T& t, const Args&... args) {
            group g(w, t, args...);
            add_group(g);
            return *this;
        }

        bool empty() const noexcept { return table_.empty(); }
        double total_weight() const noexcept { return dist_.max(); }

    private:

        std::map<double, T> table_; // Cumulative => value
        UniformReal<double> dist_{0};

        void add_group(const group& g) {
            if (g.weight <= 0 || g.values.empty())
                return;
            T last = g.values.back();
            auto most = irange(g.values.begin(), g.values.end() - 1);
            double cw = dist_.max();
            double iw = g.weight / g.values.size();
            for (auto& t: most) {
                cw += iw;
                table_[cw] = t;
            }
            cw = dist_.max() + g.weight;
            table_[cw] = last;
            dist_ = UniformReal<double>(cw);
        }

    };

    class RandomUuid {
    public:

        using result_type = Uuid;

        template <typename RNG>
        Uuid operator()(RNG& rng) const {
            std::array<uint64_t, 2> array;
            UniformInteger<uint64_t> make64(0, ~ uint64_t(0));
            for (auto& a: array)
                a = make64(rng);
            Uuid u(array.data(), 16);
            u[6] = (u[6] & 0x0f) | 0x40;
            u[8] = (u[8] & 0x3f) | 0x80;
            return u;
        }

    };

}
