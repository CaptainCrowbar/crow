// This header is not part of the public interface;
// include "crow/random.hpp" instead.

#pragma once

#include "crow/random-continuous-distributions.hpp"
#include "crow/random-discrete-distributions.hpp"
#include "crow/random-engines.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

namespace Crow {

    template <RangeType R, RandomEngineType RNG>
    std::vector<RangeValue<R>> sample(const R& range, RNG& rng, size_t k) {

        using V = RangeValue<R>;

        using std::begin;
        using std::end;

        auto rb = begin(range);
        auto re = end(range);
        auto n = size_t(re - rb);

        if (k >= n)
            return std::vector(rb, re);

        std::vector<V> out(k);
        size_t i = 0;
        auto r = rb;
        auto o = out.begin();

        for (; i < k; ++i, ++o, ++r)
            *o = *r;

        UniformReal<double> unit;
        UniformInteger<size_t> index_k(k);
        double w = std::exp(std::log(unit(rng)) / k);

        for (;;) {

            auto delta = size_t(std::floor(std::log(unit(rng)) / std::log(1 - w))) + 1;
            i += delta;

            if (i >= n)
                break;

            std::advance(r, delta);
            auto j = index_k(rng);
            out[j] = *r;
            w *= std::exp(std::log(unit(rng)) / k);

        }

        return out;

    }

    template <MutableRandomAccessRangeType R, RandomEngineType RNG>
    void shuffle(R& range, RNG& rng) {

        using std::begin;
        using std::end;

        auto rb = begin(range);
        auto re = end(range);
        auto n = size_t(re - rb);

        for (size_t i = 0; i < n - 1; ++i) {
            UniformInteger<size_t> dist(i, n - 1);
            size_t j = dist(rng);
            if (i != j)
                std::swap(rb[i], rb[j]);
        }

    }

}
