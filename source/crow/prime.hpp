#pragma once

#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Crow {

    template <typename T>
    class PrimeGenerator {
    public:
        PrimeGenerator(const PrimeGenerator& pg);
        PrimeGenerator& operator=(const PrimeGenerator& pg);
        PrimeGenerator() = default;
        PrimeGenerator(PrimeGenerator&& pg) = default;
        PrimeGenerator& operator=(PrimeGenerator&& pg) = default;
        T operator()() { next(); return get(); }
        const T& get() const noexcept { return current_; }
        void next();
    private:
        std::unordered_map<T, T> multiples_;
        std::unique_ptr<PrimeGenerator> recurse_;
        T current_ = 1;
        T factor_ = 0;
        T square_ = 0;
    };

        template <typename T>
        PrimeGenerator<T>::PrimeGenerator(const PrimeGenerator& pg):
        multiples_(pg.multiples_),
        recurse_(),
        current_(pg.current_),
        factor_(pg.factor_),
        square_(pg.square_) {
            if (pg.recurse_)
                recurse_ = std::make_unique<PrimeGenerator>(*pg.recurse_);
        }

        template <typename T>
        PrimeGenerator<T>& PrimeGenerator<T>::operator=(const PrimeGenerator& pg) {
            auto copy(pg);
            std::swap(copy, *this);
            return *this;
        }

        template <typename T>
        void PrimeGenerator<T>::next() {
            if (current_ < 3) {
                ++current_;
                return;
            }
            if (current_ < 7) {
                current_ += 2;
                return;
            }
            if (! recurse_) {
                multiples_.clear();
                recurse_ = std::make_unique<PrimeGenerator>();
                recurse_->next();
                recurse_->next();
                factor_ = recurse_->get();
                square_ = factor_ * factor_;
            }
            for (;;) {
                current_ += 2;
                if (current_ == square_) {
                    T delta = 2 * factor_;
                    T next = square_ + delta;
                    multiples_.insert({next, delta});
                    recurse_->next();
                    factor_ = recurse_->get();
                    square_ = factor_ * factor_;
                } else {
                    auto it = multiples_.find(current_);
                    if (it == multiples_.end())
                        return;
                    T delta = it->second;
                    T next = it->first + delta;
                    while (multiples_.count(next))
                        next += delta;
                    multiples_.erase(it);
                    multiples_.insert({next, delta});
                }
            }
        }

    template <typename T>
    class PrimeIterator:
    public ForwardIterator<PrimeIterator<T>, const T> {
    public:
        PrimeIterator() { gen_.next(); }
        explicit PrimeIterator(bool init) { if (init) gen_.next(); }
        const T& operator*() const noexcept { return gen_.get(); }
        PrimeIterator& operator++() { gen_.next(); return *this; }
        bool operator==(const PrimeIterator& rhs) const noexcept { return gen_.get() == rhs.gen_.get(); }
    private:
        PrimeGenerator<T> gen_;
    };

    template <typename T>
    auto prime_numbers() {
        using PI = PrimeIterator<T>;
        return irange(PI(true), PI(false));
    }

    template <typename T>
    bool is_prime(T n) {
        if (n < 2)
            return false;
        for (PrimeIterator<T> it; *it * *it <= n; ++it)
            if (n % *it == 0)
                return false;
        return true;
    }

    template <typename T>
    T next_prime(T n) {
        if (n <= 2)
            return 2;
        if (n % 2 == 0)
            ++n;
        while (! is_prime(n))
            n += 2;
        return n;
    }

    template <typename T>
    T prev_prime(T n) {
        if (n < 2)
            return 0;
        if (n == 2)
            return 2;
        if (n % 2 == 0)
            --n;
        while (n >= 2 && ! is_prime(n))
            n -= 2;
        return n;
    }

    template <typename T>
    std::vector<T> prime_factors(T n) {
        std::vector<T> factors;
        PrimeIterator<T> it;
        while (n > 1 && *it * *it <= n) {
            if (n % *it == 0) {
                n /= *it;
                factors.push_back(*it);
            } else {
                ++it;
            }
        }
        if (n > 1)
            factors.push_back(n);
        return factors;
    }

    template <typename T>
    std::map<T, T> prime_factors_map(T n) {
        std::map<T, T> factors;
        PrimeIterator<T> it;
        while (n > 1 && *it * *it <= n) {
            if (n % *it == 0) {
                n /= *it;
                ++factors[*it];
            } else {
                ++it;
            }
        }
        if (n > 1)
            ++factors[n];
        return factors;
    }

    template <typename T>
    std::vector<T> prime_list(T m, T n) {
        std::vector<T> v;
        if (n < 2) {
            // do nothing
        } else if (m <= 2) {
            auto z = size_t(n);
            std::vector<bool> sieve(z + 1, true);
            sieve[0] = sieve[1] = false;
            sieve[2] = true;
            for (size_t i = 4; i <= z; i += 2)
                sieve[i] = false;
            for (size_t i = 3; 2 * i <= z; i += 2)
                if (sieve[i])
                    for (size_t j = 2 * i; j <= z; j += i)
                        sieve[j] = false;
            for (size_t i = 0; i <= z; ++i)
                if (sieve[i])
                    v.push_back(T(i));
        } else {
            for (T t = m; t <= n; ++t)
                if (is_prime(t))
                    v.push_back(t);
        }
        return v;
    }

    template <typename T>
    std::vector<T> prime_list(T n) {
        return prime_list(T(2), n);
    }

}
