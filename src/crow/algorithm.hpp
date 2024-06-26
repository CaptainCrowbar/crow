#pragma once

#include "crow/binary.hpp"
#include "crow/enum.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iterator>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Crow {

    // Container algorithms

    template <AssociativeContainerType C,
        std::convertible_to<typename C::key_type> K,
        std::convertible_to<typename C::mapped_type> T>
    typename C::mapped_type lookup(const C& map, const K& key, const T& def) {
        auto it = map.find(key);
        if (it == map.end())
            return def;
        else
            return it->second;
    }

    template <AssociativeContainerType C,
        std::convertible_to<typename C::key_type> K>
    typename C::mapped_type lookup(const C& map, const K& key) {
        using T = typename C::mapped_type;
        return lookup(map, key, T());
    }

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

    template <RandomAccessRangeType Range>
    struct DiffEntry {
        using iterator = RangeIterator<const Range>;
        using subrange = Irange<iterator>;
        subrange del;
        subrange ins;
    };

    template <RandomAccessRangeType Range> using DiffList = std::vector<DiffEntry<Range>>;

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

    template <RandomAccessRangeType Range,
        std::equivalence_relation<RangeValue<Range>, RangeValue<Range>> ER>
    DiffList<Range> diff(const Range& lhs, const Range& rhs, ER eq) {

        using std::begin;
        using std::end;

        auto l_begin = begin(lhs);
        auto l_end = end(lhs);
        auto l_size = std::distance(l_begin, l_end);
        auto r_begin = begin(rhs);
        auto r_end = end(rhs);
        auto r_size = std::distance(r_begin, r_end);

        DiffList<Range> diffs;

        Detail::DiffHelper dh(l_size, r_size,
            [eq,l_begin,r_begin] (ptrdiff_t i, ptrdiff_t j) { return eq(l_begin[i], r_begin[j]); },
            [&diffs,l_begin,r_begin] (ptrdiff_t lpos1, ptrdiff_t lpos2, ptrdiff_t rpos1, ptrdiff_t rpos2) {
                diffs.push_back({{l_begin + lpos1, l_begin + lpos2}, {r_begin + rpos1, r_begin + rpos2}});
            });

        dh.resolve();

        return diffs;

    }

    template <RandomAccessRangeType Range>
    DiffList<Range> diff(const Range& lhs, const Range& rhs) {
        return diff(lhs, rhs, std::equal_to<RangeValue<Range>>());
    }

    // Edit distance

    template <ArithmeticType T = double>
    class Levenshtein {
    public:
        Levenshtein() = default;
        Levenshtein(T ins, T del, T sub);
        template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
            T operator()(const Range1& a, const Range2& b) const;
    private:
        T ins_ = 1;
        T del_ = 1;
        T sub_ = 1;
    };

    template <ArithmeticType T>
    Levenshtein<T>::Levenshtein(T ins, T del, T sub):
    ins_(ins), del_(del), sub_(sub) {
        if (ins < 0 || del < 0 || sub < 0)
            throw std::invalid_argument("Invalid costs for Levenshtein distance");
    }

    template <ArithmeticType T>
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
    T Levenshtein<T>::operator()(const Range1& a, const Range2& b) const {

        using std::begin;
        using std::end;

        const int m = int(std::distance(begin(a), end(a)));
        const int n = int(std::distance(begin(b), end(b)));

        std::vector<T> buf1(n + 1, 0);
        std::vector<T> buf2(n + 1, 0);

        for (int j = 0; j < n; ++j)
            buf1[j + 1] = buf1[j] + ins_;

        for (int i = 0; i < m; ++i) {

            buf2[0] = del_ * (i + 1);

            for (int j = 0; j < n; ++j) {
                T inscost = buf2[j] + ins_;
                T delcost = buf1[j + 1] + del_;
                T subcost = buf1[j];
                if (a[i] != b[j])
                    subcost += sub_;
                buf2[j + 1] = std::min({inscost, delcost, subcost});
            }

            buf1.swap(buf2);

        }

        return buf1[n];

    }

    // Zhao and Sahni BMC Bioinformatics 2019, 20(Suppl 11):277
    // https://doi.org/10.1186/s12859-019-2819-0
    // Zhao and Sahni BMC Bioinformatics 2020, 21(Suppl 1):4
    // https://doi.org/10.1186/s12859-019-3184-8

    template <ArithmeticType T = double>
    class DamerauLevenshtein {
    public:
        DamerauLevenshtein() = default;
        DamerauLevenshtein(T ins, T del, T sub, T exch);
        template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
            T operator()(const Range1& a, const Range2& b) const;
    private:
        T ins_ = 1;
        T del_ = 1;
        T sub_ = 1;
        T exch_ = 1;
    };

    template <ArithmeticType T>
    DamerauLevenshtein<T>::DamerauLevenshtein(T ins, T del, T sub, T exch):
    ins_(ins), del_(del), sub_(sub), exch_(exch) {
        if (ins < 0 || del < 0 || sub < 0 || 2 * exch < ins + del)
            throw std::invalid_argument("Invalid costs for Damerau-Levenshtein distance");
    }

    template <ArithmeticType T>
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
    T DamerauLevenshtein<T>::operator()(const Range1& a, const Range2& b) const {

        using AT = std::decay_t<decltype(*begin(a))>;
        using BT = std::decay_t<decltype(*begin(b))>;

        static_assert(std::is_same_v<AT, BT>);

        using std::begin;
        using std::end;

        const int m = int(std::distance(begin(a), end(a)));
        const int n = int(std::distance(begin(b), end(b)));
        const T maxcost = (m + n) * std::max({ins_, del_, sub_, exch_}) + 1;

        std::unordered_map<AT, int> lastrow;
        std::vector<T> fr(n + 2, maxcost);
        std::vector<T> r1(n + 2, maxcost);
        std::vector<T> r(n + 2);

        for (auto& c: a)
            lastrow[c] = -1;
        for (auto& c: b)
            lastrow[c] = -1;
        r[0] = maxcost;
        for (int i = 0; i <= n; ++i)
            r[i + 1] = i;

        for (int i = 0; i < m; ++i) {

            r.swap(r1);
            int lastcol = -1;
            T lastval = r[1];
            r[1] = i + 1;
            T t = maxcost;

            for (int j = 0; j < n; ++j) {

                T inscost = r[j + 1] + ins_;
                T delcost = r1[j + 2] + del_;
                T subcost = r1[j + 1];

                if (a[i] != b[j])
                    subcost += sub_;

                T cost = std::min({subcost, inscost, delcost});

                if (a[i] == b[j]) {
                    lastcol = j + 1;
                    fr[j + 2] = r1[j];
                    t = lastval;
                } else {
                    int k = lastrow[b[j]];
                    int l = lastcol;
                    if (j == l) {
                        T exchcost = fr[j + 2] + exch_ + i - k;
                        cost = std::min(cost, exchcost);
                    } else if (i == k) {
                        T exchcost = t + exch_ + j - l;
                        cost = std::min(cost, exchcost);
                    }
                }

                lastval = r[j + 2];
                r[j + 2] = cost;

            }

            lastrow[a[i]] = i + 1;

        }

        return r[n + 1];

    }

    // https://stackoverflow.com/questions/19123506/jaro-winkler-distance-algorithm-in-c-sharp
    // https://gist.github.com/ronnieoverby/2aa19724199df4ec8af6

    // prefix     = Prefix length to be checked by Winkler modification (default 4)
    // threshold  = Apply Winkler modification only if distance is within threshold (default 0.3)
    // scale      = Scale factor applied to Winkler modification (default 0.1)

    template <std::floating_point T = double>
    class JaroWinkler {
    public:
        JaroWinkler() = default;
        JaroWinkler(size_t prefix, T threshold, T scale);
        template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
            T operator()(const Range1& a, const Range2& b) const;
    private:
        size_t prefix_ = 4;
        T threshold_ = 0.3f;
        T scale_ = 0.1f;
    };

    template <std::floating_point T>
    JaroWinkler<T>::JaroWinkler(size_t prefix, T threshold, T scale):
    prefix_(prefix), threshold_(threshold), scale_(scale) {
        if (prefix < 0)
            throw std::invalid_argument("Invalid prefix length for Jaro-Winkler distance");
        if (threshold < 0 || threshold > 1)
            throw std::invalid_argument("Invalid distance threshold for Jaro-Winkler distance");
        if (scale < 0 || scale * prefix > 1)
            throw std::invalid_argument("Invalid scale factor for Jaro-Winkler distance");
    }

    template <std::floating_point T>
    template <RandomAccessRangeType Range1, RandomAccessRangeType Range2>
    T JaroWinkler<T>::operator()(const Range1& a, const Range2& b) const {

        using std::begin;
        using std::end;

        using AT = std::decay_t<decltype(*begin(a))>;
        using BT = std::decay_t<decltype(*begin(b))>;

        static_assert(std::is_same_v<AT, BT>);

        auto m = size_t(std::distance(begin(a), end(a)));
        auto n = size_t(std::distance(begin(b), end(b)));

        if (m == 0)
            return n == 0 ? 0 : 1;

        size_t max_mn = std::max(m, n);
        size_t search_range = max_mn >= 2 ? max_mn / 2 - 1 : 0;

        std::vector<bool> matched1(m, false);
        std::vector<bool> matched2(n, false);

        size_t num_common = 0;

        for (size_t i = 0; i < m; ++i) {
            size_t start = i > search_range ? i - search_range : 0;
            size_t end = std::min(i + search_range + 1, n);
            for (size_t j = start; j < end; ++j) {
                if (! matched2[j] && a[i] == b[j]) {
                    matched1[i] = matched2[j] = true;
                    ++num_common;
                    break;
                }
            }
        }

        if (num_common == 0)
            return 1;

        size_t num_half_transposed = 0;

        for (size_t i = 0, j = 0; i < m; ++i) {
            if (! matched1[i])
                continue;
            while (! matched2[j])
                ++j;
            if (a[i] != b[j])
                ++num_half_transposed;
            ++j;
        }

        size_t num_transposed = num_half_transposed / 2;
        T num_common_t = T(num_common);
        T distance = T(1) - (num_common_t / m + num_common_t / n + (num_common - num_transposed) / num_common_t) / T(3);

        if (distance >= threshold_)
            return distance;

        size_t maxlen = std::min({prefix_, m, n});
        size_t pos = 0;
        while (pos < maxlen && a[pos] == b[pos])
            ++pos;
        if (pos != 0)
            distance -= scale_ * pos * distance;

        return distance;

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

    // Numerical algorithms

    template <std::floating_point T>
    class PrecisionSum {

    public:

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

    template <InputRangeType Range>
    requires std::floating_point<RangeValue<Range>>
    auto precision_sum(const Range& range) {
        PrecisionSum<RangeValue<Range>> sum;
        for (auto x: range)
            sum(x);
        return sum;
    }

    namespace Detail {

        template <std::floating_point T>
        class LineIntegralIterator:
        public ForwardIterator<LineIntegralIterator<T>, const T> {

        public:

            LineIntegralIterator() = default;

            template <std::invocable<T> F>
            requires std::convertible_to<std::invoke_result_t<F, T>, T>
            LineIntegralIterator(T x1, T x2, int k, F f):
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

        template <std::floating_point T, int N>
        class VolumeIntegralIterator:
        public ForwardIterator<VolumeIntegralIterator<T, N>, const T> {

        public:

            using vector_type = Vector<T, N>;

            VolumeIntegralIterator() = default;

            template <std::invocable<vector_type> F>
            requires std::convertible_to<std::invoke_result_t<F, vector_type>, T>
            VolumeIntegralIterator(vector_type x1, vector_type x2, int k, F f):
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

    template <std::floating_point T, std::invocable<T> F>
    requires std::convertible_to<std::invoke_result_t<F, T>, T>
    T line_integral(T x1, T x2, int k, F f) {
        static_assert(std::is_floating_point_v<T>);
        static_assert(std::is_convertible_v<F, std::function<T(T)>>);
        using iterator = Detail::LineIntegralIterator<T>;
        iterator i(x1, x2, k, f);
        iterator j(k);
        return precision_sum(irange(i, j));
    }

    template <std::floating_point T, int N, std::invocable<Vector<T, N>> F>
    requires std::convertible_to<std::invoke_result_t<F, Vector<T, N>>, T>
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

    template <ForwardRangeType Range>
    class CartesianPowerIterator:
    public ForwardIterator<CartesianPowerIterator<Range>, std::vector<RangeValue<Range>>> {

    public:

        CartesianPowerIterator() = default;

        CartesianPowerIterator(const Range& range, size_t k):
        range_(&range), index_(0) {
            using std::begin;
            if (k > 0) {
                iterators_.resize(k, begin(range));
                elements_.resize(k, *iterators_[0]);
            }
        }

        const std::vector<RangeValue<Range>>& operator*() const noexcept {
            return elements_;
        }

        CartesianPowerIterator& operator++() {
            using std::begin;
            using std::end;
            ++index_;
            for (int i = int(iterators_.size()) - 1; i >= 0; --i) {
                ++iterators_[i];
                if (iterators_[i] != end(*range_)) {
                    elements_[i] = *iterators_[i];
                    return *this;
                }
                iterators_[i] = begin(*range_);
                elements_[i] = *iterators_[i];
            }
            index_ = npos;
            return *this;
        }

        bool operator==(const CartesianPowerIterator& i) const noexcept {
            return index_ == i.index_;
        }

    private:

        using iterator_type = RangeIterator<const Range>;
        using iterator_vector = std::vector<iterator_type>;
        using element_type = RangeValue<Range>;
        using element_vector = std::vector<element_type>;

        iterator_vector iterators_;
        element_vector elements_;
        const Range* range_ = nullptr;
        size_t index_ = npos;

    };

    template <ForwardRangeType Range>
    Irange<CartesianPowerIterator<Range>> cartesian_power(const Range& range, size_t k) {
        return {{range, k}, {}};
    }

    template <ForwardRangeType Range,
        std::invocable<RangeValue<Range>> UnaryFunction,
        std::strict_weak_order<
            std::invoke_result_t<UnaryFunction, RangeValue<Range>>,
            std::invoke_result_t<UnaryFunction, RangeValue<Range>>
        > Compare>
    auto find_optimum(Range& range, UnaryFunction f, Compare cmp) {
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

    template <ForwardRangeType Range,
        std::invocable<RangeValue<Range>> UnaryFunction>
    auto find_optimum(Range& range, UnaryFunction f) {
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
