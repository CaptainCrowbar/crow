# Dice

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/dice.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Dice class

```c++
class Dice;
```

This class is a random number distribution that emulates rolling multiple dice
of different kinds, and applying some limited arithmetic to the results. A
set of dice can be specified either by building it up from instances of the
class, or by parsing a string in a conventional format.

Examples:

    // These both produce the same distribution
    auto dice1 = Dice(2, 10) * 5 + Dice(3, 6) + 10;
    auto dice2 = Dice("2d10x5 + 3d6 + 10");

```c++
using Dice::result_type = Rational;
```

The result type returned by the dice generator.

```c++
Dice::Dice() noexcept;
```

Creates a null dice roller, which always yields zero.

```c++
Dice::Dice(int n, int faces = 6, const Rational& factor = 1);
```

Creates a `Dice` object that rolls `n` dice, each numbered from 1 to `faces`,
optionally multiplying the result by `factor`. This will produce a null
(always zero) dice roller if any of the arguments is zero; it will throw
`std::invalid_argument` if `n` or `faces` is negative.

```c++
explicit Dice::Dice(const std::string& str);
```

Creates a dice roller by parsing the string representation.

A dice representation consists of any number of dice groups or numeric
modifiers. A dice group can consist of an optional number of dice (1 by
default), the letter `"D"` (case insensitive), and an optional number of
faces(6 by default). For example, `"3d6"` means "roll three six-sided dice
and add the results" (and could also be written `"3D"`).

A group can be preceded or followed by an integer multiplier, delimited by
either a star or the letter `"X"` (case insensitive), and followed by a
divisor, delimited by a slash. For example, `"3d6x2/3"` means "roll 3d6 and
multiply the result by two thirds". The returned value always keeps fractions
intact, it does not round to an integer.

The string can also add or subtract constant integers or fractions. For
example, `"3d6+10"` means "roll 3d6 and add 10" (the modifier does not have
to be at the end; `"10+3d6"` is equally valid).

White space is not significant. More complicated arithmetic, such as anything
that would require parentheses, is not supported. This constructor will throw
`std::invalid_argument` if the expression is not a valid dice specification
according to the above rules. Behaviour is undefined if the expression
implies division by zero, or if any of the numbers in the expression, or any
intermediate value in the arithmetic required to evaluate it, is out of range
for an `int`.

```c++
Dice::Dice(const Dice& d);
Dice::Dice(Dice&& d) noexcept;
Dice::~Dice() noexcept;
Dice& Dice::operator=(const Dice& d);
Dice& Dice::operator=(Dice&& d) noexcept;
```

Other life cycle functions.

```c++
template <typename RNG> Rational Dice::operator()(RNG& rng) const;
```

The main generator function. The `RNG` class can be any standard conforming
random number engine.

```c++
Dice Dice::operator+() const;
Dice Dice::operator-() const;
Dice& Dice::operator+=(const Dice& b);
Dice& Dice::operator+=(const Rational& b);
Dice& Dice::operator+=(int b);
Dice& Dice::operator-=(const Dice& b);
Dice& Dice::operator-=(const Rational& b);
Dice& Dice::operator-=(int b);
Dice& Dice::operator*=(const Rational& b);
Dice& Dice::operator*=(int b);
Dice& Dice::operator/=(const Rational& b);
Dice& Dice::operator/=(int b);
Dice operator+(const Dice& a, const Dice& b);
Dice operator+(const Dice& a, const Rational& b);
Dice operator+(const Dice& a, int b);
Dice operator+(const Rational& a, const Dice& b);
Dice operator+(int a, const Dice& b);
Dice operator-(const Dice& a, const Dice& b);
Dice operator-(const Dice& a, const Rational& b);
Dice operator-(const Dice& a, int b);
Dice operator-(const Rational& a, const Dice& b);
Dice operator-(int a, const Dice& b);
Dice operator*(const Dice& a, const Rational& b);
Dice operator*(const Dice& a, int b);
Dice operator*(const Rational& a, const Dice& b);
Dice operator*(int a, const Dice& b);
Dice operator/(const Dice& a, const Rational& b);
Dice operator/(const Dice& a, int b);
```

Operations that modify or combine two sets of dice, or a set of dice and a
numeric factor. Addition and subtraction simply combine groups of dice
together, in the same way as the plus and minus operators in the string
format. Multiplication or division by a rational number multiplies or divides
the result of future rolls by that number.

Behaviour is undefined on division by zero.

```c++
Rational Dice::mean() const noexcept;
Rational Dice::variance() const noexcept;
double Dice::sd() const noexcept;
Rational Dice::min() const noexcept;
Rational Dice::max() const noexcept;
```

Statistical properties of the dice roll result distribution.

```c++
Rational Dice::pdf(const Rational& x) const;   // Pr(result=x)
Rational Dice::cdf(const Rational& x) const;   // Pr(result≤x)
Rational Dice::ccdf(const Rational& x) const;  // Pr(result≥x)
Rational Dice::interval(const Rational& x,
    const Rational y) const;                   // Pr(result∈[x,y])
```

Probabilities of given results. The `interval()` function will return zero if
`x>y`.

The `Dice` object needs to compute a probability table the first time one of
these is called. The table is shared between copies of the same object.
Computing the table can take some time for complicated distributions.

```c++
std::string Dice::str() const;
std::ostream& operator<<(std::ostream& out, const Dice& d);
```

Format the set of dice in the object, in the same format as described for the
constructor. Because the string is being reconstructed from the stored
properties of the distribution, the result may not exactly match the original
string supplied to the constructor, but will be functionally equivalent.

## IntDice class

```c++
class IntDice {
    using result_type = int;
    IntDice();
    explicit IntDice(int n, int faces = 6, int factor = 1);
    explicit IntDice(const std::string& str);
    IntDice(const IntDice& d);
    IntDice(IntDice&& d) noexcept;
    ~IntDice() noexcept;
    IntDice& operator=(const IntDice& d);
    IntDice& operator=(IntDice&& d) noexcept;
    template <typename RNG> int operator()(RNG& rng) const;
    IntDice operator+() const;
    IntDice operator-() const;
    IntDice& operator+=(const IntDice& b);
    IntDice& operator+=(int b);
    IntDice& operator-=(const IntDice& b);
    IntDice& operator-=(int b);
    IntDice& operator*=(int b);
    IntDice& operator/=(int b);
    int min() const noexcept;
    int max() const noexcept;
    std::string str() const;
};
IntDice operator+(const IntDice& a, const IntDice& b);
IntDice operator+(const IntDice& a, int b);
IntDice operator+(int a, const IntDice& b);
IntDice operator-(const IntDice& a, const IntDice& b);
IntDice operator-(const IntDice& a, int b);
IntDice operator-(int a, const IntDice& b);
IntDice operator*(const IntDice& a, int b);
IntDice operator*(int a, const IntDice& b);
IntDice operator/(const IntDice& a, int b);
std::ostream& operator<<(std::ostream& out, const IntDice& d);
```

This class works the same as `Dice`, except that the result is rounded down to
an integer.

## Literals

```c++
namespace Literals {
    Dice operator""_d3(unsigned long long n);
    Dice operator""_d4(unsigned long long n);
    Dice operator""_d6(unsigned long long n);
    Dice operator""_d8(unsigned long long n);
    Dice operator""_d10(unsigned long long n);
    Dice operator""_d12(unsigned long long n);
    Dice operator""_d20(unsigned long long n);
    Dice operator""_d30(unsigned long long n);
    Dice operator""_d100(unsigned long long n);
    Dice operator""_d1000(unsigned long long n);
    Dice operator""_dice(const char* p, size_t n);
    IntDice operator""_id3(unsigned long long n);
    IntDice operator""_id4(unsigned long long n);
    IntDice operator""_id6(unsigned long long n);
    IntDice operator""_id8(unsigned long long n);
    IntDice operator""_id10(unsigned long long n);
    IntDice operator""_id12(unsigned long long n);
    IntDice operator""_id20(unsigned long long n);
    IntDice operator""_id30(unsigned long long n);
    IntDice operator""_id100(unsigned long long n);
    IntDice operator""_id1000(unsigned long long n);
    IntDice operator""_idice(const char* p, size_t n);
}
```

Literals for some commonly used dice. For example, `3_d6` is equivalent to
`Dice(3,6)` or `Dice("3d6")`.

The `_[i]dice` literal suffix calls the string-based constructor; for example,
`"3d6"_dice` is equivalent to `Dice(3,6)` or `Dice("3d6")`.
