#pragma once

#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>

namespace Crow {

    // https://www.johndcook.com/blog/standard_deviation/
    // https://www.johndcook.com/blog/skewness_kurtosis/
    // https://www.johndcook.com/blog/running_regression/

    template <std::floating_point T = double>
    class Statistics {

    public:

        constexpr Statistics() noexcept { clear(); }

        constexpr Statistics& operator()(T x) noexcept;
        constexpr Statistics& operator()(T x, T y) noexcept;
        constexpr Statistics operator+(const Statistics& s) const noexcept;
        constexpr Statistics& operator+=(const Statistics& s) noexcept { return *this = *this + s; }

        constexpr void clear() noexcept;
        constexpr int n() const noexcept { return count_; }
        constexpr T tn() const noexcept { return T(count_); }

        constexpr T min() const noexcept { return x_min(); }
        constexpr T max() const noexcept { return x_max(); }
        constexpr T mean() const noexcept { return x_mean(); }
        constexpr T variance() const noexcept { return x_variance(); }
        T sd() const noexcept { return x_sd(); }
        constexpr T pop_variance() const noexcept { return x_pop_variance(); }
        T pop_sd() const noexcept { return x_pop_sd(); }
        T skewness() const noexcept { return x_skewness(); }
        T kurtosis() const noexcept { return x_kurtosis(); }

        constexpr T x_min() const noexcept { return check(xmin_, 1); }
        constexpr T x_max() const noexcept { return check(xmax_, 1); }
        constexpr T x_mean() const noexcept { return check(xm1_, 1); }
        constexpr T x_variance() const noexcept { return check(xm2_ / (count_ - 1), 2); }
        T x_sd() const noexcept { return std::sqrt(x_variance()); }
        constexpr T x_pop_variance() const noexcept { return check(xm2_ / count_, 1); }
        T x_pop_sd() const noexcept { return std::sqrt(x_pop_variance()); }
        T x_skewness() const noexcept { return check(std::sqrt(tn()) * xm3_ / std::pow(xm2_, T(1.5)), 2); }
        T x_kurtosis() const noexcept { return check(tn() * xm4_ / (xm2_ * xm2_) - 3, 2); }

        constexpr T y_min() const noexcept { return check(ymin_, 1); }
        constexpr T y_max() const noexcept { return check(ymax_, 1); }
        constexpr T y_mean() const noexcept { return check(ym1_, 1); }
        constexpr T y_variance() const noexcept { return check(ym2_ / (count_ - 1), 2); }
        T y_sd() const noexcept { return std::sqrt(y_variance()); }
        constexpr T y_pop_variance() const noexcept { return check(ym2_ / count_, 1); }
        T y_pop_sd() const noexcept { return std::sqrt(y_pop_variance()); }
        T y_skewness() const noexcept { return check(std::sqrt(tn()) * ym3_ / std::pow(ym2_, T(1.5)), 2); }
        T y_kurtosis() const noexcept { return check(tn() * ym4_ / (ym2_ * ym2_) - 3, 2); }

        T r() const noexcept { return check(sxy_ / (x_sd() * y_sd() * (tn() - 1)), 1); }
        T a() const noexcept { return check(sxy_ / (x_variance() * (tn() - 1)), 1); }
        T b() const noexcept { return check(ym1_ - a() * xm1_, 1); }
        T inv_a() const noexcept { return check(sxy_ / (y_variance() * (tn() - 1)), 1); }
        T inv_b() const noexcept { return check(xm1_ - inv_a() * ym1_, 1); }

    private:

        int count_;
        T xm1_, xm2_, xm3_, xm4_, xmin_, xmax_,
            ym1_, ym2_, ym3_, ym4_, ymin_, ymax_,
            sxy_;

        constexpr T check(T t, int min_n) const noexcept { return count_ < min_n ? T(0) : t; }

    };

    template <std::floating_point T>
    constexpr Statistics<T>& Statistics<T>::operator()(T x) noexcept {

        ++count_;

        xmin_ = std::min(xmin_, x);
        xmax_ = std::max(xmax_, x);

        T x_delta = x - xm1_;
        T x_delta_n = x_delta / tn();
        T x_delta_n2 = x_delta_n * x_delta_n;
        T x_term = x_delta * x_delta_n * (tn() - 1);

        xm1_ += x_delta_n;
        xm4_ += x_term * x_delta_n2 * (tn() * tn() - 3 * tn() + 3) + 6 * x_delta_n2 * xm2_ - 4 * x_delta_n * xm3_;
        xm3_ += x_term * x_delta_n * (tn() - 2) - 3 * x_delta_n * xm2_;
        xm2_ += x_term;

        return *this;

    }

    template <std::floating_point T>
    constexpr Statistics<T>& Statistics<T>::operator()(T x, T y) noexcept {

        sxy_ += (x_mean() - x ) * (y_mean() - y) * tn() / (tn() + 1);

        ++count_;

        xmin_ = std::min(xmin_, x);
        xmax_ = std::max(xmax_, x);

        T x_delta = x - xm1_;
        T x_delta_n = x_delta / tn();
        T x_delta_n2 = x_delta_n * x_delta_n;
        T x_term = x_delta * x_delta_n * (tn() - 1);

        xm1_ += x_delta_n;
        xm4_ += x_term * x_delta_n2 * (tn() * tn() - 3 * tn() + 3) + 6 * x_delta_n2 * xm2_ - 4 * x_delta_n * xm3_;
        xm3_ += x_term * x_delta_n * (tn() - 2) - 3 * x_delta_n * xm2_;
        xm2_ += x_term;

        ymin_ = std::min(ymin_, y);
        ymax_ = std::max(ymax_, y);

        T y_delta = y - ym1_;
        T y_delta_n = y_delta / tn();
        T y_delta_n2 = y_delta_n * y_delta_n;
        T y_term = y_delta * y_delta_n * (tn() - 1);

        ym1_ += y_delta_n;
        ym4_ += y_term * y_delta_n2 * (tn() * tn() - 3 * tn() + 3) + 6 * y_delta_n2 * ym2_ - 4 * y_delta_n * ym3_;
        ym3_ += y_term * y_delta_n * (tn() - 2) - 3 * y_delta_n * ym2_;
        ym2_ += y_term;

        return *this;

    }

    template <std::floating_point T>
    constexpr Statistics<T> Statistics<T>::operator+(const Statistics& s) const noexcept {

        Statistics<T> c;

        c.count_ = count_ + s.count_;
        c.xmin_ = std::min(xmin_, s.xmin_);
        c.xmax_ = std::max(xmax_, s.xmax_);
        c.ymin_ = std::min(ymin_, s.ymin_);
        c.ymax_ = std::max(ymax_, s.ymax_);

        T tn2 = tn() * tn();
        T sn2 = s.tn() * s.tn();
        T cn2 = c.tn() * c.tn();
        T cn3 = c.tn() * cn2;
        T x_delta = s.xm1_ - xm1_;
        T x_delta2 = x_delta * x_delta;
        T x_delta3 = x_delta * x_delta2;
        T x_delta4 = x_delta2 * x_delta2;
        T y_delta = s.ym1_ - ym1_;
        T y_delta2 = y_delta * y_delta;
        T y_delta3 = y_delta * y_delta2;
        T y_delta4 = y_delta2 * y_delta2;

        c.xm1_ = (tn() * xm1_ + s.tn() * s.xm1_) / c.tn();
        c.xm2_ = xm2_ + s.xm2_ + x_delta2 * tn() * s.tn() / c.tn();
        c.xm3_ = xm3_ + s.xm3_ + x_delta3 * tn() * s.tn() * (tn() - s.tn()) / cn2
            + 3 * x_delta * (tn() * s.xm2_ - s.tn() * xm2_) / c.tn();
        c.xm4_ = xm4_ + s.xm4_ + x_delta4 * tn() * s.tn() * (tn2 - tn() * s.tn() + sn2) / cn3
            + 6 * x_delta2 * (tn2 * s.xm2_ + sn2 * xm2_) / cn2
            + 4 * x_delta * (tn() * s.xm3_ - s.tn() * xm3_) / c.tn();

        c.ym1_ = (tn() * ym1_ + s.tn() * s.ym1_) / c.tn();
        c.ym2_ = ym2_ + s.ym2_ + y_delta2 * tn() * s.tn() / c.tn();
        c.ym3_ = ym3_ + s.ym3_ + y_delta3 * tn() * s.tn() * (tn() - s.tn()) / cn2
            + 3 * y_delta * (tn() * s.ym2_ - s.tn() * ym2_) / c.tn();
        c.ym4_ = ym4_ + s.ym4_ + y_delta4 * tn() * s.tn() * (tn2 - tn() * s.tn() + sn2) / cn3
            + 6 * y_delta2 * (tn2 * s.ym2_ + sn2 * ym2_) / cn2
            + 4 * y_delta * (tn() * s.ym3_ - s.tn() * ym3_) / c.tn();

        c.sxy_ = sxy_ + s.sxy_ + tn() * s.tn() * x_delta * y_delta / c.tn();

        return c;

    }

    template <std::floating_point T>
    constexpr void Statistics<T>::clear() noexcept {
        count_ = 0;
        xm1_ = xm2_ = xm3_ = xm4_ = 0;
        ym1_ = ym2_ = ym3_ = ym4_ = 0;
        sxy_ = 0;
        xmin_ = ymin_ = std::numeric_limits<T>::max();
        xmax_ = ymax_ = std::numeric_limits<T>::min();
    }

}
