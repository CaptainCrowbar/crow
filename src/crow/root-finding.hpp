#pragma once

#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/types.hpp"
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <memory>
#include <stdexcept>

namespace Crow {

    // Root finder base class

    template <std::floating_point T>
    class RootFinder {

    public:

        virtual ~RootFinder() noexcept {}

        T solve(T y = 0, T x = 0) {
            auto r = do_solve(y, x, x + T(1));
            check();
            return r;
        }

        T solve(T y, T x1, T x2) {
            auto r = do_solve(y, x1, x2);
            check();
            return r;
        }

        T epsilon() const noexcept { return epsilon_; }
        void set_epsilon(T e) noexcept { epsilon_ = e; }
        int limit() const noexcept { return limit_; }
        void set_limit(int n) noexcept { limit_ = n; }
        bool strict() const noexcept { return strict_; }
        void set_strict(bool b = true) noexcept { strict_ = b; }
        T error() const noexcept { return error_; }
        int count() const noexcept { return count_; }

    protected:

        RootFinder() = default;

        virtual T do_solve(T y, T x1, T x2) = 0;
        void reset() noexcept { error_ = 0; count_ = 0; }
        void set_error(T err) noexcept { error_ = std::abs(err); }
        void increment() noexcept { ++count_; }

    private:

        T epsilon_ = (sizeof(T) < sizeof(double) ? T(1e-4) : T(1e-10));
        int limit_ = 100;
        bool strict_ = false;
        T error_ = 0;
        int count_ = 0;

        void check() const {
            if (strict_ && count_ >= limit_ && error_ > epsilon_)
                throw std::range_error("Root finder failed to converge");
        }

    };

    // Bisection algorithm

    template <std::floating_point T, std::invocable<T> F>
    requires requires (T t, F f) {
        { f(t) } -> std::convertible_to<T>;
    }
    class Bisection:
    public RootFinder<T> {

    public:

        explicit Bisection(F f): RootFinder<T>(), f_(f) {}

    protected:

        T do_solve(T y, T x1, T x2) override {

            this->reset();

            auto y1 = f_(x1) - y;
            this->set_error(y1);
            if (this->error() <= this->epsilon())
                return x1;
            if (x1 == x2)
                throw std::domain_error(fmt("Invalid arguments for bisection: {0}, {1}", x1, x2));

            auto y2 = f_(x2) - y;
            this->set_error(y2);
            if (this->error() <= this->epsilon())
                return x2;

            bool s1 = std::signbit(y1);
            bool s2 = std::signbit(y2);
            if (s1 == s2)
                throw std::domain_error(fmt("Invalid arguments for bisection: {0}, {1}", x1, x2));

            while (this->count() < this->limit()) {

                this->increment();

                auto x3 = (x1 + x2) / 2;
                auto y3 = f_(x3) - y;
                this->set_error(y3);

                if (this->error() <= this->epsilon())
                    return x3;

                if (std::signbit(y3) == s1)
                    x1 = x3;
                else
                    x2 = x3;

            }

            return (x1 + x2) / 2;

        }

    private:

        F f_;

    };

    template <std::floating_point T, std::invocable<T> F>
    requires requires (T t, F f) {
        { f(t) } -> std::convertible_to<T>;
    }
    std::unique_ptr<RootFinder<T>> bisection(F f) {
        return std::make_unique<Bisection<T, F>>(f);
    }

    // FalsePosition algorithm

    template <std::floating_point T, std::invocable<T> F>
    requires requires (T t, F f) {
        { f(t) } -> std::convertible_to<T>;
    }
    class FalsePosition:
    public RootFinder<T> {

    public:

        explicit FalsePosition(F f): RootFinder<T>(), f_(f) {}

    protected:

        T do_solve(T y, T x1, T x2) override {

            this->reset();

            auto y1 = f_(x1) - y;
            this->set_error(y1);
            if (this->error() <= this->epsilon())
                return x1;
            if (x1 == x2)
                throw std::domain_error(fmt("Invalid arguments for false position: {0}, {1}", x1, x2));

            auto y2 = f_(x2) - y;
            this->set_error(y2);
            if (this->error() <= this->epsilon())
                return x2;

            bool s1 = std::signbit(y1);
            bool s2 = std::signbit(y2);
            if (s1 == s2)
                throw std::domain_error(fmt("Invalid arguments for false position: {0}, {1}", x1, x2));

            while (this->count() < this->limit()) {

                this->increment();

                auto x3 = x2 - y2 * (x2 - x1) / (y2 - y1);
                auto y3 = f_(x3) - y;
                this->set_error(y3);

                if (this->error() <= this->epsilon())
                    return x3;

                if (std::signbit(y3) == s1)
                    x1 = x3;
                else
                    x2 = x3;

            }

            return (x1 + x2) / 2;

        }

    private:

        F f_;

    };

    template <std::floating_point T, std::invocable<T> F>
    requires requires (T t, F f) {
        { f(t) } -> std::convertible_to<T>;
    }
    std::unique_ptr<RootFinder<T>> false_position(F f) {
        return std::make_unique<FalsePosition<T, F>>(f);
    }

    // Newton-Raphson algorithm

    template <typename T, typename F, typename DF>
    concept NewtonRaphsonArgumentTypes =
        std::floating_point<T>
        && requires (T t, F f, DF df) {
            { f(t) } -> std::convertible_to<T>;
            { df(t) } -> std::convertible_to<T>;
        };

    template <typename T, typename F, typename DF>
    requires (NewtonRaphsonArgumentTypes<T, F, DF>)
    class NewtonRaphson:
    public RootFinder<T> {

    public:

        NewtonRaphson(F f, DF df): RootFinder<T>(), f_(f), df_(df) {}

    protected:

        T do_solve(T y, T x1, T /*x2*/) override {

            this->reset();

            while (this->count() < this->limit()) {

                this->increment();

                T y1 = this->f_(x1) - y;
                this->set_error(y1);

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

    private:

        F f_;
        DF df_;

    };

    template <typename T, typename F, typename DF>
    requires (NewtonRaphsonArgumentTypes<T, F, DF>)
    std::unique_ptr<RootFinder<T>> newton_raphson(F f, DF df) {
        return std::make_unique<NewtonRaphson<T, F, DF>>(f, df);
    }

}
