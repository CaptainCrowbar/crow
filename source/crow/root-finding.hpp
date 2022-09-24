#pragma once

#include "crow/maths.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <cstdlib>
#include <memory>
#include <type_traits>

namespace Crow {

    template <typename T>
    class RootFinder {
    public:
        static_assert(std::is_floating_point_v<T>);
        virtual ~RootFinder() noexcept {}
        T solve(T y = 0, T x = 0) { return do_solve(y, x, x + T(1)); }
        T solve(T y, T x1, T x2) { return do_solve(y, x1, x2); }
        T epsilon() const noexcept { return epsilon_; }
        void set_epsilon(T e) noexcept { epsilon_ = e; }
        int limit() const noexcept { return limit_; }
        void set_limit(int n) noexcept { limit_ = n; }
        T error() const noexcept { return error_; }
        int count() const noexcept { return count_; }
    protected:
        RootFinder() = default;
        virtual T do_solve(T y, T x1, T x2) = 0;
        void set_error(T er) noexcept { error_ = er; }
        void set_count(int n) noexcept { count_ = n; }
    private:
        T epsilon_ = (sizeof(T) < sizeof(double) ? T(1e-4) : T(1e-10));
        int limit_ = 100;
        T error_ = 0;
        int count_ = 0;
    };

    template <typename T, typename F, typename DF>
    class NewtonRaphson:
    public RootFinder<T> {
    public:
        NewtonRaphson(F f, DF df): RootFinder<T>(), f_(f), df_(df) {}
    protected:
        T do_solve(T y, T x1, T x2) override;
    private:
        F f_;
        DF df_;
    };

        template <typename T, typename F, typename DF>
        T NewtonRaphson<T, F, DF>::do_solve(T y, T x1, T /*x2*/) {
            this->set_count(0);
            this->set_error(0);
            for (int i = 0; i < this->limit(); ++i) {
                this->set_count(i);
                T y1 = this->f_(x1) - y;
                this->set_error(std::abs(y1));
                if (this->error() <= this->epsilon())
                    break;
                T slope = this->df_(x1);
                if (slope == T(0))
                    x1 += T(100) * this->epsilon();
                else
                    x1 -= y1 / slope;
            }
            return x1;
        }

    template <typename T, typename F, typename DF>
    std::unique_ptr<RootFinder<T>> newton_raphson(F f, DF df) {
        return std::make_unique<NewtonRaphson<T, F, DF>>(f, df);
    }

}
