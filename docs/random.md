# Random Number Generators

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/random.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class LogMode: int {
    natural,
    common
};
```

Used in some of the logarithmic distributions to indicate whether natural
(base e) or common (base 10) logs are intended.

## Concepts

```c++
template <typename T> concept RandomEngineType;
```

A simplified concept to match random engine types. This tests for an unsigned
integral `result_type`, a call operator that returns `result_type`, and
`min()` and `max()` functions.

```c++
template <typename T> concept RandomDistributionType;
template <typename T, typename RT> concept SpecificDistributionType;
```

`RandomDistributionType` matches a random distribution class with
`result_type` and `operator()(RNG&)`. `SpecificDistributionType` is a
refinement of `RandomDistributionType` that also requires `result_type` to be
convertible to `RT`.

## Primitive random engines

### Linear congruential generators

```c++
constexpr uint32_t lcg32(uint32_t x) noexcept;
constexpr uint64_t lcg64(uint64_t x) noexcept;
```

Good LCG transformations for 32 and 64 bit integers, from Pierre L'Ecuyer (1999),
["Tables of Linear Congruential Generators of Different Sizes and Good Lattice Structure"](http://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf).

```c++
class Lcg32 {
    using result_type = uint32_t;
    constexpr Lcg32() noexcept; // seed(0)
    explicit constexpr Lcg32(uint32_t s) noexcept;
    constexpr uint32_t operator()() noexcept;
    constexpr bool operator==(const Lcg32& rhs) const noexcept;
    constexpr bool operator!=(const Lcg32& rhs) const noexcept;
    constexpr void seed(uint32_t s) noexcept;
    static constexpr uint32_t min() noexcept;
    static constexpr uint32_t max() noexcept;
};
```

```c++
class Lcg64 {
    using result_type = uint64_t;
    constexpr Lcg64() noexcept; // seed(0)
    explicit constexpr Lcg64(uint64_t s) noexcept;
    uint64_t constexpr operator()() noexcept;
    bool constexpr operator==(const Lcg64& rhs) const noexcept;
    bool constexpr operator!=(const Lcg64& rhs) const noexcept;
    void constexpr seed(uint64_t s) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

### PCG generator

```c++
class Pcg64 {
    using result_type = uint64_t;
    constexpr Pcg64() noexcept; // seed(0,0)
    constexpr explicit Pcg64(uint64_t s) noexcept;
    constexpr explicit Pcg64(uint64_t hi, uint64_t lo) noexcept;
    constexpr uint64_t operator()() noexcept;
    constexpr void advance(int64_t offset) noexcept;
    constexpr void seed(uint64_t s) noexcept;
    constexpr void seed(uint64_t hi, uint64_t lo) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

[PCG generator](http://www.pcg-random.org/) by Melissa O'Neill.

### Xoshiro generator

```c++
class Xoshiro {
    using result_type = uint64_t;
    constexpr Xoshiro() noexcept; // seed(0,0,0,0)
    constexpr explicit Xoshiro(uint64_t s) noexcept;
    constexpr Xoshiro(uint64_t s, uint64_t t) noexcept;
    constexpr Xoshiro(uint64_t s, uint64_t t,
        uint64_t u, uint64_t v) noexcept;
    constexpr uint64_t operator()() noexcept;
    constexpr void seed(uint64_t s = 0) noexcept;
    constexpr void seed(uint64_t s, uint64_t t) noexcept;
    constexpr void seed(uint64_t s, uint64_t t,
        uint64_t u, uint64_t v) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

[Xoshiro256** generator](http://xoshiro.di.unimi.it/) by David Blackman and
Sebastiano Vigna.

### Default generator

```c++
using StdRng = Pcg64;
```

A good generator to use as a default when we have a function that calls an RNG
but we don't want to make it a template.

## Standard distributions

Many of these duplicate distributions from the standard library, to allow
consistently reproducible results without depending on the library
implementation.

The template argument types are usually not restricted by concepts, to allow
their use with non-standard arithmetic types.

In addition to the standard properties of a random distribution class, many of
these also provide functions that return statistical properties of the
distribution.

The `pdf()`, `cdf()`, and `ccdf()` functions represent the probability
distribution function, cumulative distribution function, and complementary
CDF. For discrete distributions, `pdf(x)` is the probably of a result exactly
equal to `x`, `cdf(x)` is the probability of a result less than or equal to
`x`, and `ccdf(x)` is the probability of a result greater than or equal to `x`
(`cdf+ccdf-pdf=1`). For continuous distributions, `pdf(x)` is the probably
density at `x`, `cdf(x)` is the probability of a result less than `x`, and
`ccdf(x)` is the probability of a result greater than `x` (`cdf+ccdf=1`).

The `quantile()` and `cquantile()` (complementary quantile) functions are only
defined for continuous distributions, and are the inverse of `cdf()` and
`ccdf()` respectively. Behaviour is undefined if the argument to one of the
quantile functions is less than 0 or greater than 1; it may or may not be
defined for exactly 0 or 1, depending on how the distribution is bounded. For
some of the distributions, the quantile functions have no analytical form and
are evaluated by a numerical approximation, usually reliable to at least 6
significant figures.

### Discrete distributions

#### Uniform integer distribution

```c++
template <std::integral T> class UniformInteger {
    using result_type = T;
    constexpr UniformInteger() noexcept;
        // range=[0,1]
    constexpr explicit UniformInteger(T r) noexcept;
        // range=[0,r-1]; UB if r<1
    constexpr UniformInteger(T a, T b) noexcept;
        // range=[a,b]; UB if a>b
    template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
    constexpr Ratio<T> mean() const noexcept;
    constexpr Ratio<T> variance() const noexcept;
    double sd() const noexcept;
    double pdf(T x) const noexcept;
    double cdf(T x) const noexcept;
    double ccdf(T x) const noexcept;
};
```

#### Bernoulli distribution

```c++
class BernoulliDistribution {
    using result_type = bool;
    constexpr BernoulliDistribution() noexcept;
        // p=1/2
    constexpr explicit BernoulliDistribution(double p) noexcept;
        // UB if p<0 or p>1
    template <typename T>
        constexpr explicit BernoulliDistribution(Ratio<T> p) noexcept;
        // UB if p<0 or p>1
    template <RandomEngineType RNG>
        constexpr bool operator()(RNG& rng) const noexcept;
    constexpr double p() const noexcept;
};
```

#### Discrete normal distribution

```c++
template <std::integral T> class DiscreteNormal {
    using result_type = T;
    DiscreteNormal() noexcept; // mean=0,sd=1
    DiscreteNormal(double mean, double sd) noexcept;
    template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept;
    double mean() const noexcept;
    double sd() const noexcept;
};
```

#### Poisson distribution

```c++
template <std::integral T> class PoissonDistribution {
    using result_type = T;
    PoissonDistribution() noexcept;
        // lambda=1
    explicit PoissonDistribution(double lambda) noexcept;
        // UB if lambda<=0
    template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept;
    constexpr double mean() const noexcept;
    constexpr double variance() const noexcept;
    double sd() const noexcept;
    double pdf(T x) const noexcept;
    double cdf(T x) const noexcept;
    double ccdf(T x) const noexcept;
};
```

### Continuous distributions

#### Uniform real distribution

```c++
template <std::floating_point T> class UniformReal {
    using result_type = T;
    constexpr UniformReal() noexcept;
        // range=[0,1]
    constexpr explicit UniformReal(T r) noexcept;
        // range=[0,r); UB if r<=0
    constexpr UniformReal(T a, T b) noexcept;
        // range=[a,b); UB if a>b
    template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
    constexpr T mean() const noexcept;
    constexpr T variance() const noexcept;
    T sd() const noexcept;
    constexpr T pdf(T x) const noexcept;
    constexpr T cdf(T x) const noexcept;
    constexpr T ccdf(T x) const noexcept;
    constexpr T quantile(T p) const noexcept;
    constexpr T cquantile(T q) const noexcept;
};
```

#### Normal distribution

```c++
template <std::floating_point T> class NormalDistribution {
    using result_type = T;
    NormalDistribution() noexcept; // mean=0, sd=1
    NormalDistribution(T mean, T sd) noexcept;
    template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept;
    constexpr T mean() const noexcept;
    constexpr T sd() const noexcept;
    constexpr T variance() const noexcept;
    T pdf(T x) const noexcept;
    T cdf(T x) const noexcept;
    T ccdf(T x) const noexcept;
    T quantile(T p) const noexcept;
    T cquantile(T q) const noexcept;
};
```

#### Log uniform distribution

```c++
template <std::floating_point T> class LogUniform {
    using result_type = T;
    constexpr LogUniform() noexcept; // range=[0,e)
    constexpr LogUniform(T min, T max) noexcept; // UB if min>max
    template <RandomEngineType RNG>
        constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
};
```

#### Log normal distribution

```c++
template <std::floating_point T> class LogNormal {
    using result_type = T;
    LogNormal() noexcept; // m=0, s=1
    LogNormal(T m, T s, LogMode mode = LogMode::natural) noexcept;
    template <RandomEngineType RNG>
        T operator()(RNG& rng) const noexcept;
    T m() const noexcept;
    T s() const noexcept;
    T median() const noexcept;
    T s_factor() const noexcept; // pow(base,s)
};
```

## Distribution adapters

### Constrained distribution

```c++
template <typename Base>
requires (ArithmeticType<typename Base::result_type>)
class ConstrainedDistribution {
    using base_distribution = Base;
    using result_type = typename Base::result_type;
    ConstrainedDistribution(const Base& dist,
        result_type min, result_type max);
    template <typename... Args>
        ConstrainedDistribution(result_type min, result_type max,
            Args&&... args);
    template <RandomEngineType RNG>
        result_type operator()(RNG& rng) const;
    result_type min() const noexcept;
    result_type max() const noexcept;
};
```

Distribution with resampling from a constrained range.

The first constructor takes an underlying distribution (which is copied) and
the permitted range; the second takes the permitted range and a set of
arguments that will be passed to the base distribution's constructor (the
different argument order between the two constructors is to ensure unambiguous
overload resolution). The constructors will throw `std::invalid_argument` if
`min>max` or if the intersection of `[min,max]` and the range of the base
distribution is empty.

## Non-arithmetic discrete distributions

### Uniform random choice

```c++
template <typename T> class RandomChoice {
    using result_type = T;
    RandomChoice();
    RandomChoice(std::initializer_list<T> list);
    template <typename Range>
        explicit RandomChoice(const Range& range);
    template <RandomEngineType RNG>
        const T& operator()(RNG& rng) const;
    template <typename... Args>
        RandomChoice& add(const Args&... args);
    bool empty() const noexcept;
    size_t size() const noexcept;
};
```

Selects a random item from a set of values. Behaviour is undefined if the
function call operator is called on an empty distribution.

```c++
template <typename Range>
    RandomChoice<TL::RangeValue<Range>> random_choice(const Range& range);
```

Convenience function to construct a `RandomChoice` object.

### Weighted random choice

```c++
template <typename T> class WeightedChoice {
    using result_type = T;
    WeightedChoice();
    WeightedChoice(std::initializer_list<...> list);
    template <RandomEngineType RNG>
        const T& operator()(RNG& rng) const;
    template <typename... Args>
        WeightedChoice& add(double w, const Args&... args);
    bool empty() const noexcept;
    double total_weight() const noexcept;
};
```

Selects a random item from a weighted set of values.

The `add()` function takes a weight and a number of values; the weight is
divided evenly between the values. A call to `add()` is ignored if the weight
is less than or equal to zero. The constructor argument list is a list of
nested initializer lists, each corresponding to the arguments to a call to
`add()`.

Behaviour is undefined if the function call operator is called on an empty
distribution.

### Random UUID

```c++
class RandomUuid {
    using result_type = Uuid;
    RandomUuid();
    template <RandomEngineType RNG>
        Uuid operator()(RNG& rng) const;
};
```

Generates a random version 4 UUID.

## Spatial distributions

### Random vectors

```c++
template <std::floating_point T, int N> class RandomVector {
    using result_type = Graphics::Core::Vector<T, N>;
    using scalar_type = T;
    static constexpr int dim = N;
    constexpr RandomVector() noexcept;
        // apex=(1,1,...)
    constexpr explicit RandomVector(T t) noexcept;
        // apex=(t,t,...)
    constexpr explicit RandomVector(const result_type& apex) noexcept;
    template <RandomEngineType RNG>
        result_type operator()(RNG& rng) const;
    constexpr result_type apex() const noexcept;
};
template <std::floating_point T, int N> class SymmetricRandomVector {
    using result_type = Graphics::Core::Vector<T, N>;
    using scalar_type = T;
    static constexpr int dim = N;
    constexpr SymmetricRandomVector() noexcept;
        // apex=(1,1,...)
    constexpr explicit SymmetricRandomVector(T t) noexcept;
        // apex=(t,t,...)
    constexpr explicit
        SymmetricRandomVector(const result_type& apex) noexcept;
    template <RandomEngineType RNG>
        result_type operator()(RNG& rng) const;
    constexpr result_type apex() const noexcept;
};
```

These generate a random point in a rectangular box. `RandomVector` generates a
point in the box whose corners are `(origin,apex)`, while
`SymmetricRandomVector` generates one in the box whose corners are `
(-apex,apex)`.

### Random point in a sphere

```c++
template <std::floating_point T, int N> class RandomPointInSphere {
    using result_type = Graphics::Core::Vector<T, N>;
    using scalar_type = T;
    static constexpr int dim = N;
    constexpr RandomPointInSphere() noexcept;
    constexpr explicit RandomPointInSphere(T r) noexcept;
    template <RandomEngineType RNG>
        result_type operator()(RNG& rng) const;
    constexpr T radius() const noexcept;
};
```

Generates a random point in a sphere of the given radius, centred on the
origin in `N` dimensions. Behaviour is undefined if `r<0`.

### Random direction

```c++
template <std::floating_point T, int N> class RandomDirection {
    using result_type = Graphics::Core::Vector<T, N>;
    using scalar_type = T;
    static constexpr int dim = N;
    constexpr RandomDirection() noexcept;
    template <RandomEngineType RNG>
        result_type operator()(RNG& rng) const;
};
```

Generates a random unit vector, uniformly distributed in `N` dimensions. The
vector's length may not be exactly 1 due to floating point rounding.
