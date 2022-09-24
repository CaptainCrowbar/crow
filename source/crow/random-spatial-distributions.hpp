// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-continuous-distributions.hpp"
#include "crow/random-discrete-distributions.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <cmath>
#include <numbers>
#include <type_traits>

namespace Crow {

    template <typename T, int N>
    class RandomVector {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomVector() noexcept: vec_(T(1)) {}
        constexpr explicit RandomVector(T t) noexcept: vec_(t) {}
        constexpr explicit RandomVector(const result_type& v) noexcept: vec_(v) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {
            result_type v;
            for (int i = 0; i < N; ++i)
                v[i] = UniformReal<T>(vec_[i])(rng);
            return v;
        }

        constexpr result_type apex() const noexcept { return vec_; }

    private:

        result_type vec_;

    };

    template <typename T, int N>
    class SymmetricRandomVector {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr SymmetricRandomVector() noexcept: vec_(T(1)) {}
        constexpr explicit SymmetricRandomVector(T t) noexcept: vec_(t) {}
        constexpr explicit SymmetricRandomVector(const result_type& v) noexcept: vec_(v) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {
            result_type v;
            for (int i = 0; i < N; ++i)
                v[i] = UniformReal<T>(- vec_[i], vec_[i])(rng);
            return v;
        }

        constexpr result_type apex() const noexcept { return vec_; }

    private:

        result_type vec_;

    };

    template <typename T, int N>
    class RandomPointInSphere {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomPointInSphere() noexcept: radius_(1) {}
        constexpr explicit RandomPointInSphere(T r) noexcept: radius_(std::fabs(r)) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {

            using std::numbers::pi_v;

            result_type v;

            if constexpr (N == 1) {

                v[0] = UniformReal<T>(- radius_, radius_)(rng);

            } else if constexpr (N <= 4) {

                do {
                    for (auto& x: v)
                        x = UniformReal<T>(-1, 1)(rng);
                } while (v.r2() > 1);

                v *= radius_;

            } else {

                for (int i = 0; i < N; i += 2) {
                    T a = std::sqrt(-2 * std::log(UniformReal<T>()(rng)));
                    T b = 2 * pi_v<T> * UniformReal<T>()(rng);
                    v[i] = a * std::cos(b);
                    if (i + 1 < N)
                        v[i + 1] = a * std::sin(b);
                }

                v *= radius_ * std::pow(UniformReal<T>()(rng), T(1) / T(N)) / v.r();

            }

            return v;

        }

        constexpr T radius() const noexcept { return radius_; }

    private:

        T radius_;

    };

    template <typename T, int N>
    class RandomDirection {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomDirection() noexcept {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {

            using std::numbers::pi_v;

            result_type v;

            if constexpr (N == 1) {

                v[0] = BernoulliDistribution()(rng) ? 1 : -1;

            } else if constexpr (N == 2) {

                T phi = UniformReal<T>(0, 2 * pi_v<T>)(rng);
                v[0] = std::cos(phi);
                v[1] = std::sin(phi);

            } else if constexpr (N == 3) {

                T phi = UniformReal<T>(0, 2 * pi_v<T>)(rng);
                v[2] = UniformReal<T>(-1, 1)(rng);
                T r = std::sqrt(1 - v[2] * v[2]);
                v[0] = r * std::cos(phi);
                v[1] = r * std::sin(phi);

            } else {

                for (int i = 0; i < N; i += 2) {
                    T a = std::sqrt(-2 * std::log(UniformReal<T>()(rng)));
                    T b = 2 * pi_v<T> * UniformReal<T>()(rng);
                    v[i] = a * std::cos(b);
                    if (i + 1 < N)
                        v[i + 1] = a * std::sin(b);
                }

                v /= v.r();

            }

            return v;

        }

    };

}
