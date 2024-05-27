// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-discrete-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/hash.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/uuid.hpp"
#include <algorithm>
#include <array>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T>
        concept DistributionBoundedBelow =
            RandomDistributionType<T>
            && requires(T t) {
                { t.min() } -> std::convertible_to<typename T::result_type>;
            };

        template <typename T>
        concept DistributionBoundedAbove =
            RandomDistributionType<T>
            && requires(T t) {
                { t.max() } -> std::convertible_to<typename T::result_type>;
            };

    }

    template <typename Base>
    requires (ArithmeticType<typename Base::result_type>)
    class ConstrainedDistribution {

    public:

        using base_distribution = Base;
        using result_type = typename Base::result_type;

        ConstrainedDistribution(const Base& dist, result_type min, result_type max);
        template <typename... Args> ConstrainedDistribution(result_type min, result_type max, Args&&... args);

        template <RandomEngineType RNG> result_type operator()(RNG& rng) const;

        result_type min() const noexcept { return min_; }
        result_type max() const noexcept { return max_; }

    private:

        Base dist_;
        result_type min_ = std::numeric_limits<result_type>::lowest();
        result_type max_ = std::numeric_limits<result_type>::max();

        void check_range();

    };

        template <typename Base>
        requires (ArithmeticType<typename Base::result_type>)
        ConstrainedDistribution<Base>::ConstrainedDistribution(const Base& dist, result_type min, result_type max):
        dist_(dist), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        requires (ArithmeticType<typename Base::result_type>)
        template <typename... Args>
        ConstrainedDistribution<Base>::ConstrainedDistribution(result_type min, result_type max, Args&&... args):
        dist_(std::forward<Args>(args)...), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        requires (ArithmeticType<typename Base::result_type>)
        template <RandomEngineType RNG>
        typename ConstrainedDistribution<Base>::result_type
        ConstrainedDistribution<Base>::operator()(RNG& rng) const {
            result_type x;
            do x = dist_(rng);
                while (x < min_ || x > max_);
            return x;
        }

        template <typename Base>
        requires (ArithmeticType<typename Base::result_type>)
        void ConstrainedDistribution<Base>::check_range() {
            if constexpr (Detail::DistributionBoundedBelow<Base>)
                min_ = std::max(min_, dist_.min());
            if constexpr (Detail::DistributionBoundedAbove<Base>)
                max_ = std::min(max_, dist_.max());
            if (min_ > max_)
                throw std::invalid_argument("Constrained distribution has no possible values");
        }

    template <typename Base>
    requires (RegularHashable<typename Base::result_type>)
    class UniqueDistribution:
    public Base {

    public:

        using base_distribution = Base;
        using result_type = typename Base::result_type;

        explicit UniqueDistribution(const Base& dist): Base(dist) {}
        template <typename... Args> UniqueDistribution(Args&&... args): Base(std::forward<Args>(args)...) {}

        template <RandomEngineType RNG> result_type operator()(RNG& rng) {
            result_type t;
            do t = static_cast<Base&>(*this)(rng);
                while (used_.contains(t));
            used_.insert(t);
            return t;
        }

        void reset() noexcept { used_.clear(); }

    private:

        std::unordered_set<result_type> used_;

    };

    template <typename T> class UniqueChoice;

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

        template <RandomEngineType RNG>
        const T& operator()(RNG& rng) const {
            if (vec_.empty())
                throw std::length_error("Random choice pool is empty");
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

        template <typename Range>
        RandomChoice& add(const Range& range) {
            using std::begin;
            using std::end;
            vec_.insert(vec_.end(), begin(range), end(range));
            return *this;
        }

        bool empty() const noexcept { return vec_.empty(); }
        size_t size() const noexcept { return vec_.size(); }

    private:

        friend class UniqueChoice<T>;

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
    class UniqueChoice {

    public:

        using result_type = T;

        UniqueChoice() = default;
        UniqueChoice(std::initializer_list<T> list): vec_(list) {}
        explicit UniqueChoice(const RandomChoice<T>& choice): vec_(choice.vec_) {}

        template <typename Range>
        explicit UniqueChoice(const Range& range) {
            using std::begin;
            using std::end;
            vec_.assign(begin(range), end(range));
        }

        template <RandomEngineType RNG>
        T operator()(RNG& rng) {
            if (! ready_) {
                pool_.resize(vec_.size());
                std::iota(pool_.begin(), pool_.end(), size_t(0));
                ready_ = true;
            }
            if (pool_.empty())
                throw std::length_error("Random choice pool is empty");
            UniformInteger<size_t> dist(pool_.size());
            auto pool_index = dist(rng);
            auto vec_index = pool_[pool_index];
            if (pool_index + 1 < pool_.size())
                std::swap(pool_[pool_index], pool_.back());
            pool_.pop_back();
            return vec_[vec_index];
        }

        template <typename... Args>
        UniqueChoice& add(const T& t, const Args&... args) {
            vec_.push_back(t);
            ready_ = false;
            if constexpr (sizeof...(Args) > 0)
                add(args...);
            return *this;
        }

        template <typename Range>
        UniqueChoice& add(const Range& range) {
            using std::begin;
            using std::end;
            vec_.insert(vec_.end(), begin(range), end(range));
            ready_ = false;
            return *this;
        }

        bool empty() const noexcept { return vec_.empty(); }
        size_t size() const noexcept { return vec_.size(); }
        bool pool_empty() const noexcept { return ready_ ? pool_.empty() : vec_.empty(); }
        size_t pool_size() const noexcept { return ready_ ? pool_.size() : vec_.size(); }
        void reset() noexcept { ready_ = false; }

    private:

        std::vector<T> vec_;
        std::vector<size_t> pool_;
        bool ready_ = false;

    };

    template <typename Range>
    UniqueChoice<RangeValue<Range>> unique_choice(const Range& range) {
        return UniqueChoice<RangeValue<Range>>(range);
    }

    template <typename T>
    UniqueChoice<T> unique_choice(std::initializer_list<T> list) {
        return UniqueChoice<T>(list);
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

        template <RandomEngineType RNG>
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

        template <RandomEngineType RNG>
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
