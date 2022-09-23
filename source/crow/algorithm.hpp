#pragma once

#include "crow/binary.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <map>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Crow {

    // Arithmetic algorithms

    template <typename T>
    constexpr T binomial(T a, T b) noexcept {
        static_assert(std::is_arithmetic_v<T>);
        if (b < 0 || b > a)
            return 0;
        if (b == 0 || b == a)
            return 1;
        if (b > a / 2)
            b = a - b;
        T n = 1;
        T d = 1;
        while (b > 0) {
            n *= a;
            d *= b;
            a -= 1;
            b -= 1;
        }
        return n / d;
    }

    template <typename T, typename U, typename BinaryFunction>
    constexpr T integer_power(T x, U y, BinaryFunction f, T unit = T(1)) noexcept {
        static_assert(std::is_integral_v<U>);
        using U2 = std::make_unsigned_t<U>;
        U2 y2 = U2(y);
        U2 mask = std::bit_floor(y2);
        T z = unit;
        while (mask != 0) {
            z = f(z, z);
            if ((y2 & mask) != 0)
                z = f(z, x);
            mask >>= 1;
        }
        return z;
    }

    template <typename T, typename U>
    constexpr T integer_power(T x, U y) noexcept {
        return integer_power(x, y, std::multiplies<T>());
    }

    // Container algorithms

    template <typename Container>
    void unique_in(Container& con) {
        con.erase(std::unique(con.begin(), con.end()), con.end());
    }

    template <typename Container, typename BinaryPredicate>
    void unique_in(Container& con, BinaryPredicate eq) {
        con.erase(std::unique(con.begin(), con.end(), eq), con.end());
    }

    template <typename Container>
    void sort_unique_in(Container& con) {
        std::sort(con.begin(), con.end());
        unique_in(con);
    }

    template <typename Container, typename BinaryPredicate>
    void sort_unique_in(Container& con, BinaryPredicate cmp) {
        using T = std::decay_t<decltype(*con.begin())>;
        std::sort(con.begin(), con.end(), cmp);
        unique_in(con, [cmp] (const T& a, const T& b) { return ! cmp(a, b); });
    }

    // Myers difference algorithm
    // http://xmailserver.org/diff2.pdf

    template <typename RandomAccessRange>
    struct DiffEntry {
        using iterator = RangeIterator<const RandomAccessRange>;
        using subrange = Irange<iterator>;
        subrange del;
        subrange ins;
    };

    template <typename RandomAccessRange> using DiffList = std::vector<DiffEntry<RandomAccessRange>>;

    namespace Detail {

        struct DiffHelper {

        public:

            using match_function = std::function<bool(ptrdiff_t, ptrdiff_t)>;
            using insert_function = std::function<void(ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t)>;

            DiffHelper(ptrdiff_t l_size, ptrdiff_t r_size, match_function match, insert_function insert):
            fwd_path_(2 * (l_size + r_size + 1)),
            rev_path_(2 * (l_size + r_size + 1)),
            fwd_(fwd_path_.begin() + l_size + r_size + 1),
            rev_(rev_path_.begin() + l_size + r_size + 1),
            l_changes_(l_size, false),
            r_changes_(r_size, false),
            l_size_(l_size),
            r_size_(r_size),
            match_(match),
            insert_(insert) {}

            void resolve() {
                longest_common(0, l_size_, 0, r_size_);
                collect_diffs();
            }

        private:

            using offset_list = std::vector<ptrdiff_t>;
            using offset_iterator = offset_list::iterator;

            offset_list fwd_path_;
            offset_list rev_path_;
            offset_iterator fwd_;
            offset_iterator rev_;
            std::vector<bool> l_changes_;
            std::vector<bool> r_changes_;
            ptrdiff_t l_size_;
            ptrdiff_t r_size_;
            match_function match_;
            insert_function insert_;

            void collect_diffs() {
                ptrdiff_t lpos = 0, rpos = 0;
                for (; lpos != l_size_ && rpos != r_size_; ++lpos, ++rpos) {
                    ptrdiff_t lstart = lpos, rstart = rpos;
                    for (; lpos != l_size_ && l_changes_[lpos]; ++lpos) {}
                    for (; rpos != r_size_ && r_changes_[rpos]; ++rpos) {}
                    if (lstart != lpos || rstart != rpos)
                        insert_(lstart, lpos, rstart, rpos);
                }
                if (lpos != l_size_ || rpos != r_size_)
                    insert_(lpos, l_size_, rpos, r_size_);
            }

            void longest_common(ptrdiff_t l1, ptrdiff_t l2, ptrdiff_t r1, ptrdiff_t r2) {
                for (; l1 < l2 && r1 < r2 && match_(l1, r1); ++l1, ++r1) {}
                for (; l1 < l2 && r1 < r2 && match_(l2 - 1, r2 - 1); --l2, --r2) {}
                if (l1 == l2) {
                    std::fill(r_changes_.begin() + r1, r_changes_.begin() + r2, true);
                    return;
                }
                if (r1 == r2) {
                    std::fill(l_changes_.begin() + l1, l_changes_.begin() + l2, true);
                    return;
                }
                fwd_[1] = l1;
                rev_[-1] = l2;
                ptrdiff_t d1 = l1 - r1, d2 = l2 - r2, delta = d2 - d1;
                for (ptrdiff_t d = 0;; ++d) {
                    for (ptrdiff_t k = - d; k <= d; k = k + 2) {
                        ptrdiff_t i = k == - d ? fwd_[k + 1] : fwd_[k - 1] + 1;
                        if (k < d)
                            i = std::max(i, fwd_[k + 1]);
                        for (ptrdiff_t j = i - k - d1; i < l2 && j < r2 && match_(i, j); ++i, ++j) {}
                        fwd_[k] = i;
                        if ((delta & 1) && k > delta - d && k < delta + d && rev_[k - delta] <= fwd_[k]) {
                            longest_common(l1, fwd_[k], r1, fwd_[k] - k - d1);
                            longest_common(fwd_[k], l2, fwd_[k] - k - d1, r2);
                            return;
                        }
                    }
                    for (ptrdiff_t k = - d; k <= d; k = k + 2) {
                        ptrdiff_t i = k == d ? rev_[k - 1] : rev_[k + 1] - 1;
                        if (k > - d)
                            i = std::min(i, rev_[k - 1]);
                        for (ptrdiff_t j = i - k - d2; i > l1 && j > r1 && match_(i - 1, j - 1); --i, --j) {}
                        rev_[k] = i;
                        if (! (delta & 1) && k >= - d - delta && k <= d - delta && rev_[k] <= fwd_[k + delta]) {
                            longest_common(l1, fwd_[k + delta], r1, fwd_[k + delta] - k - d2);
                            longest_common(fwd_[k + delta], l2, fwd_[k + delta] - k - d2, r2);
                            return;
                        }
                    }
                }
            }

        };

    }

    template <typename RandomAccessRange, typename EqualityPredicate>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs, const RandomAccessRange& rhs, EqualityPredicate eq) {

        using std::begin;
        using std::end;

        auto l_begin = begin(lhs);
        auto l_end = end(lhs);
        auto l_size = std::distance(l_begin, l_end);
        auto r_begin = begin(rhs);
        auto r_end = end(rhs);
        auto r_size = std::distance(r_begin, r_end);

        DiffList<RandomAccessRange> diffs;

        Detail::DiffHelper dh(l_size, r_size,
            [eq,l_begin,r_begin] (ptrdiff_t i, ptrdiff_t j) { return eq(l_begin[i], r_begin[j]); },
            [&diffs,l_begin,r_begin] (ptrdiff_t lpos1, ptrdiff_t lpos2, ptrdiff_t rpos1, ptrdiff_t rpos2) {
                diffs.push_back({{l_begin + lpos1, l_begin + lpos2}, {r_begin + rpos1, r_begin + rpos2}});
            });

        dh.resolve();

        return diffs;

    }

    template <typename RandomAccessRange>
    DiffList<RandomAccessRange> diff(const RandomAccessRange& lhs, const RandomAccessRange& rhs) {
        return diff(lhs, rhs, std::equal_to<RangeValue<RandomAccessRange>>());
    }

    // Edit distance (Levenshtein distance)

    template <typename ForwardRange1, typename ForwardRange2, typename T>
    T edit_distance(const ForwardRange1& range1, const ForwardRange2& range2, T ins, T del, T sub) {

        static_assert(std::is_arithmetic_v<T>);

        using std::begin;
        using std::end;

        const ptrdiff_t n1 = std::distance(begin(range1), end(range1));
        const ptrdiff_t n2 = std::distance(begin(range2), end(range2));

        std::vector<T> buf1(n2 + 1);
        std::vector<T> buf2(n2 + 1);

        for (ptrdiff_t j = 1; j <= n2; ++j)
            buf1[j] = buf1[j - 1] + ins;

        auto p = begin(range1);

        for (ptrdiff_t i = 1; i <= n1; ++i, ++p) {
            buf2[0] = del * T(i);
            auto q = begin(range2);
            for (ptrdiff_t j = 1; j <= n2; ++j, ++q)
                buf2[j] = std::min({buf2[j - 1] + ins, buf1[j] + del, buf1[j - 1] + (*p == *q ? T(0) : sub)});
            buf1.swap(buf2);
        }

        return buf1[n2];

    }

    template <typename ForwardRange1, typename ForwardRange2>
    int edit_distance(const ForwardRange1& range1, const ForwardRange2& range2) {
        return edit_distance(range1, range2, 1, 1, 1);
    }

    // Hash table comparison

    template <typename T, typename Hash1, typename Hash2, typename Eq, typename Cmp>
    int hash_compare(const std::unordered_set<T, Hash1, Eq>& a, const std::unordered_set<T, Hash2, Eq>& b, Cmp cmp) {
        if (a.empty() || b.empty())
            return int(b.empty()) - int(a.empty());
        std::optional<T> a_min;   // Least value in a but not in b
        std::optional<T> b_min;   // Least value in b but not in a
        std::optional<T> ab_max;  // Greatest value common to both
        for (auto& key: a) {
            if (b.count(key) == 0) {
                if (! a_min || cmp(key, *a_min))
                    a_min = key;
            } else if (! ab_max || cmp(*ab_max, key)) {
                ab_max = key;
            }
        }
        for (auto& key: b)
            if (a.count(key) == 0 && (! b_min || cmp(key, *b_min)))
                b_min = key;
        if (a_min && b_min)
            return cmp(*a_min, *b_min) ? -1 : 1;
        else if (a_min)
            return cmp(*a_min, *ab_max) ? -1 : 1;
        else if (b_min)
            return cmp(*ab_max, *b_min) ? -1 : 1;
        else
            return 0;
    }

    template <typename T, typename Hash1, typename Hash2, typename Eq>
    int hash_compare(const std::unordered_set<T, Hash1, Eq>& a, const std::unordered_set<T, Hash2, Eq>& b) {
        return hash_compare(a, b, std::less<T>());
    }

    // Interpolation

    enum Interpolate: int {
        log_x = 1,
        log_y = 2,
    };

    template <typename T>
    T interpolate(T x1, T y1, T x2, T y2, T x, int flags = 0) noexcept {
        static_assert(std::is_floating_point_v<T>);
        if ((flags & Interpolate::log_x) != 0) {
            x1 = std::log(x1);
            x2 = std::log(x2);
            x = std::log(x);
        }
        if ((flags & Interpolate::log_y) != 0) {
            y1 = std::log(y1);
            y2 = std::log(y2);
        }
        T y = y1 + (y2 - y1) * ((x - x1) / (x2 - x1));
        if ((flags & Interpolate::log_y) != 0)
            y = std::exp(y);
        return y;
    }

    template <typename T, int Flags = 0>
    class InterpolatedMapBase {

    protected:

        static_assert(std::is_floating_point_v<T>);

        static constexpr bool x_log = (Flags & Interpolate::log_x) != 0;
        static constexpr bool y_log = (Flags & Interpolate::log_y) != 0;

        static T x_in(T x) noexcept { if constexpr (x_log) return std::log(x); else return x; }
        static T y_in(T y) noexcept { if constexpr (y_log) return std::log(y); else return y; }
        static T y_out(T y) noexcept { if constexpr (y_log) return std::exp(y); else return y; }

    };

    template <typename T, int Flags = 0>
    class InterpolatedMap:
    public InterpolatedMapBase<T, Flags> {

    public:

        InterpolatedMap() = default;
        InterpolatedMap(std::initializer_list<std::pair<const T, T>> list)
            { for (auto& [x,y]: list) insert(x, y); }
        explicit InterpolatedMap(const std::vector<std::pair<T, T>> points)
            { for (auto& [x,y]: points) insert(x, y); }

        InterpolatedMap& insert(T x, T y);
        T operator()(T x) const noexcept;

    private:

        std::map<T, T> map_;

        void init();

    };

        template <typename T, int Flags>
        InterpolatedMap<T, Flags>& InterpolatedMap<T, Flags>::insert(T x, T y) {
            x = this->x_in(x);
            y = this->y_in(y);
            map_[x] = y;
            return *this;
        }

        template <typename T, int Flags>
        T InterpolatedMap<T, Flags>::operator()(T x) const noexcept {

            if (map_.empty())
                return 0;

            if (map_.size() == 1)
                return this->y_out(map_.begin()->second);

            x = this->x_in(x);
            auto i = map_.lower_bound(x);

            if (i == map_.end())
                --i;
            else if (i->first == x)
                return this->y_out(i->second);

            auto j = i;
            if (i == map_.begin())
                ++j;
            else
                --i;

            T y = i->second + (j->second - i->second) * ((x - i->first) / (j->first - i->first));
            y = this->y_out(y);

            return y;

        }

    template <typename T, int Flags = 0>
    class CubicSplineMap:
    public InterpolatedMapBase<T, Flags> {

    public:

        CubicSplineMap() = default;
        CubicSplineMap(std::initializer_list<std::pair<T, T>> list): points_(list) { init(); }
        explicit CubicSplineMap(const std::vector<std::pair<T, T>> points): points_(points) { init(); }

        T operator()(T x) const noexcept;

    private:

        std::vector<std::pair<T, T>> points_;
        std::vector<T> deriv2_;
        int count_ = 0;

        void init();
        T px(ptrdiff_t i) const noexcept { return points_[i].first; }
        T py(ptrdiff_t i) const noexcept { return points_[i].second; }

    };

        template <typename T, int Flags>
        T CubicSplineMap<T, Flags>::operator()(T x) const noexcept {

            x = this->x_in(x);

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
            y = this->y_out(y);

            return y;

        }

        template <typename T, int Flags>
        void CubicSplineMap<T, Flags>::init() {

            count_ = int(points_.size());
            if (count_ < 4)
                throw std::invalid_argument("Not enough points for cubic spline (requires 4)");

            std::sort(points_.begin(), points_.end());
            unique_in(points_);
            auto it = std::adjacent_find(points_.begin(), points_.end(),
                [] (auto& a, auto& b) { return a.first == b.first; });
            if (it != points_.end())
                throw std::invalid_argument("Degenerate points in cubic spline");

            if constexpr (Flags != 0) {
                for (auto& [x,y]: points_) {
                    if constexpr (this->x_log)
                        if (x <= 0)
                            throw std::invalid_argument("Invalid X value in logarithmic cubic spline");
                    if constexpr (this->y_log)
                        if (y <= 0)
                            throw std::invalid_argument("Invalid Y value in logarithmic cubic spline");
                    x = this->x_in(x);
                    y = this->y_in(y);
                }
            }

            deriv2_.resize(points_.size(), 0);
            std::vector<T> d2_offset(count_ - 1, 0);

            for (int i = 1; i < count_ - 1; ++i) {
                T dx_prev = px(i) - px(i - 1);
                T dx_next = px(i + 1) - px(i);
                T dy_prev = py(i) - py(i - 1);
                T dy_next = py(i + 1) - py(i);
                T delta_d = dy_next / dx_next - dy_prev / dx_prev;
                T divisor = dx_prev * (deriv2_[i - 1] + 2) + dx_next * 2;
                deriv2_[i] = - dx_next / divisor;
                d2_offset[i] = (delta_d * 6 - dx_prev * d2_offset[i - 1]) / divisor;
            }

            for (int i = count_ - 2; i >= 0; --i)
                deriv2_[i] = deriv2_[i] * deriv2_[i + 1] + d2_offset[i];

        }

    // Numerical algorithms

    template <typename T>
    class PrecisionSum {

    public:

        static_assert(std::is_floating_point_v<T>);

        using value_type = T;

        PrecisionSum& add(T t) {
            size_t i = 0;
            for (T p: partials_) {
                if (std::abs(t) < std::abs(p))
                    std::swap(t, p);
                T sum = t + p;
                p -= sum - t;
                t = sum;
                if (p != T())
                    partials_[i++] = p;
            }
            partials_.erase(partials_.begin() + i, partials_.end());
            partials_.push_back(t);
            return *this;
        }

        PrecisionSum& operator()(T t) { return add(t); }
        void clear() noexcept { partials_.clear(); }
        T get() const { return std::accumulate(partials_.begin(), partials_.end(), T()); }
        operator T() const { return get(); }

    private:

        std::vector<T> partials_;

    };

    template <typename SinglePassRange>
    auto precision_sum(const SinglePassRange& range) {
        using std::begin;
        using T = std::decay_t<decltype(*begin(range))>;
        static_assert(std::is_floating_point_v<T>);
        PrecisionSum<T> sum;
        for (auto x: range)
            sum(x);
        return sum;
    }

    namespace Detail {

        template <typename T>
        class LineIntegralIterator:
        public ForwardIterator<LineIntegralIterator<T>, const T> {

        public:

            LineIntegralIterator() = default;

            template <typename F> LineIntegralIterator(T x1, T x2, int k, F f):
            function_(f), start_x_(x1), delta_x_((x2 - x1) / k), prev_y_(f(x1)), area_element_(0), index_(0) {
                ++*this;
            }

            explicit LineIntegralIterator(int k) { index_ = k + 1; }

            const T& operator*() const noexcept { return area_element_; }
            LineIntegralIterator& operator++() {
                ++index_;
                T x = start_x_ + delta_x_ * index_;
                T y = function_(x);
                area_element_ = delta_x_ * (prev_y_ + y) / 2;
                prev_y_ = y;
                return *this;
            }
            bool operator==(const LineIntegralIterator& rhs) const noexcept { return index_ == rhs.index_; }

        private:

            std::function<T(T)> function_;
            T start_x_ = 0;
            T delta_x_ = 0;
            T prev_y_ = 0;
            T area_element_ = 0;
            int index_ = 0;

        };

        template <typename T, int N>
        class VolumeIntegralIterator:
        public ForwardIterator<VolumeIntegralIterator<T, N>, const T> {

        public:

            using vector_type = Vector<T, N>;

            VolumeIntegralIterator() = default;

            template <typename F> VolumeIntegralIterator(vector_type x1, vector_type x2, int k, F f):
                function_(f),
                cache_(),
                start_x_(x1),
                delta_x_((x2 - x1) / T(k)),
                index_(vector_type::null()),
                n_edge_(k),
                volume_factor_(std::ldexp(std::accumulate(delta_x_.begin(), delta_x_.end(), T(1), std::multiplies<T>()), - N)),
                volume_element_(get_volume()),
                done_(false) {}

            const T& operator*() const noexcept {
                return volume_element_;
            }

            VolumeIntegralIterator& operator++() {
                next_index();
                volume_element_ = get_volume();
                return *this;
            }

            bool operator==(const VolumeIntegralIterator& rhs) const noexcept {
                return done_ == rhs.done_ && (done_ || index_ == rhs.index_);
            }

        private:

            static constexpr int points = 1 << N;

            std::function<T(vector_type)> function_;
            std::deque<std::pair<vector_type, T>> cache_;
            vector_type start_x_;
            vector_type delta_x_;
            vector_type index_;
            int n_edge_;
            T volume_factor_;
            T volume_element_;
            bool done_ = true;

            void next_index() {
                int i = 0;
                while (i < N) {
                    index_[i] += 1;
                    if (index_[i] < n_edge_)
                        break;
                    index_[i++] = 0;
                }
                done_ = i == N;
            }

            T get_value(vector_type x) {
                static constexpr int cache_size = 2 * points;
                for (auto& xy: cache_)
                    if (xy.first == x)
                        return xy.second;
                T y = function_(x);
                cache_.push_back({x, y});
                if (cache_.size() > cache_size)
                    cache_.pop_front();
                return y;
            }

            T get_volume() {
                auto corner = start_x_ + delta_x_ * index_;
                std::array<T, points> y;
                for (int i = 0; i < points; ++i) {
                    auto x = corner;
                    for (int j = 0; j < N; ++j)
                        x[j] += delta_x_[j] * ((i >> j) & 1);
                    y[i] = get_value(x);
                }
                return volume_factor_ * precision_sum(y);
            }

        };

    }

    template <typename T, typename F>
    T line_integral(T x1, T x2, int k, F f) {
        static_assert(std::is_floating_point_v<T>);
        static_assert(std::is_convertible_v<F, std::function<T(T)>>);
        using iterator = Detail::LineIntegralIterator<T>;
        iterator i(x1, x2, k, f);
        iterator j(k);
        return precision_sum(irange(i, j));
    }

    template <typename T, int N, typename F>
    T volume_integral(Vector<T, N> x1, Vector<T, N> x2, int k, F f) {
        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);
        static_assert(std::is_convertible_v<F, std::function<T(Vector<T, N>)>>);
        using iterator = Detail::VolumeIntegralIterator<T, N>;
        iterator i(x1, x2, k, f);
        iterator j;
        return precision_sum(irange(i, j));
    }

    // Range algorithms

    template <typename ForwardRange, typename UnaryFunction, typename Compare>
    auto find_optimum(ForwardRange& range, UnaryFunction f, Compare cmp) {
        using std::begin;
        using std::end;
        auto i = begin(range);
        auto j = end(range);
        if (i == j)
            return j;
        auto opt = i;
        auto current = f(*i);
        for (++i; i != j; ++i) {
            auto value = f(*i);
            if (cmp(value, current)) {
                opt = i;
                current = value;
            }
        }
        return opt;
    }

    template <typename ForwardRange, typename UnaryFunction>
    auto find_optimum(ForwardRange& range, UnaryFunction f) {
        using std::begin;
        using T = std::decay_t<decltype(f(*begin(range)))>;
        return find_optimum(range, f, std::greater<T>());
    }

    // Subsets

    namespace Detail {

        class Bitset {
        public:
            Bitset() = default;
            explicit Bitset(size_t bits): words_((bits + 63) / 64, 0) {}
            void operator++() noexcept {
                for (auto& word: words_)
                    if (++word != 0)
                        break;
            }
            bool operator[](size_t index) const noexcept {
                return ((words_[index / 64] >> (index % 64)) & size_t(1)) == 1;
            }
        private:
            std::vector<uint64_t> words_; // little endian
        };

    }

    template <typename RandomAccessContainer>
    std::vector<RandomAccessContainer> subsets(const RandomAccessContainer& con) {

        std::vector<RandomAccessContainer> subs;
        RandomAccessContainer sub;
        Detail::Bitset bits(con.size());
        size_t n = con.size();

        for (;;) {
            sub.clear();
            for (size_t i = 0; i < n; ++i)
                if (bits[i])
                    sub.insert(sub.end(), con[i]);
            subs.push_back(sub);
            if (sub.size() == n)
                break;
            ++bits;
        }

        return subs;

    }

    template <typename RandomAccessContainer>
    std::vector<RandomAccessContainer> subsets(const RandomAccessContainer& con, int k) {

        std::vector<RandomAccessContainer> subs;
        RandomAccessContainer sub(k, {});
        std::vector<int> indices(k);
        std::iota(indices.begin(), indices.end(), 0);
        int n = int(con.size());

        for (;;) {

            std::transform(indices.begin(), indices.end(), sub.begin(),
                [&con] (int i) { return con[i]; });
            subs.push_back(sub);
            int stop = n;
            int i;

            for (i = k - 1; i >= 0; --i, --stop) {
                ++indices[i];
                if (indices[i] < stop)
                    break;
            }

            if (i < 0)
                break;

            for (++i; i < k; ++i)
                indices[i] = indices[i - 1] + 1;

        }

        return subs;

    }


}
