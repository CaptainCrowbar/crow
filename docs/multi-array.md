# Multi-Dimensional Array

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/multi-array.hpp"
namespace Crow;
```

## Multi-dimensional array class

```c++
template <typename T, int N> class MultiArray;
```

This class represents an `N`-dimensional array, indexed by a fixed-size vector
of integers. The element type `T` must be copyable and movable; `N` must be a
positive integer.

Several of the constructors and member functions can accept a set of
coordinates expressed either as a position vector, or as an explicit list of
integer arguments, Although the integer-list versions of these functions are
listed here as variadic templates, they are actually defined only if the
number of arguments is correct and the arguments are all `int` or convertible
to `int`.

```c++
using MultiArray::iterator = [forward iterator];
using MultiArray::const_iterator = [forward iterator];
```

Iterators. The default iteration order visits every element once, with the
first index (axis 0) varying fastest, and the last index (axis `N-1`) varying
slowest.

Iterators have two additional member functions: `move(axis,distance)` moves
the iterator along the given axis by the given number of cells; `pos()`
returns the position vector corresponding to the iterator's current position.
For `move()`, behaviour is undefined if `axis<0` or `axis>=N`.

Iterators can be considered to exist in an infinite space (bounded in practise
by the range of an `int`); moving an iterator outside the bounds of the array
using `move()` is safe. Behaviour is undefined if any of `operator*()`,
`operator++()`, `pos()`, or the comparison operators are called on an
off-the-map iterator.

```c++
using MultiArray::position = Vector<int, N>;
using MultiArray::value_type = T;
```

Other member types.

```c++
static constexpr int MultiArray::dim = N;
```

Member constants.

```c++
MultiArray::MultiArray() noexcept;
explicit MultiArray::MultiArray(const position& shape);
explicit MultiArray::MultiArray(const position& shape, const T& t);
template <typename... Args> explicit MultiArray::MultiArray(Args... args);
```

The default constructor creates an empty array (all dimensions zero). The
other constructors create an array with the given set of dimensions.
Optionally, a value to fill the array with can be supplied; otherwise, the
array is uninitialised. Behaviour is undefined if any element of `shape` is
negative.

```c++
MultiArray::MultiArray(const MultiArray& a);
MultiArray::MultiArray(MultiArray&& a) noexcept;
MultiArray::~MultiArray() noexcept;
MultiArray& MultiArray::operator=(const MultiArray& a);
MultiArray& MultiArray::operator=(MultiArray&& a) noexcept;
```

Other life cycle functions.

```c++
T& MultiArray::operator[](const position& p) noexcept;
const T& MultiArray::operator[](const position& p) const noexcept;
template <typename... Args>
    T& MultiArray::operator()(Args... args) noexcept;
template <typename... Args>
    const T& MultiArray::operator()(Args... args) const noexcept;
T& MultiArray::ref(const position& p) noexcept;
template <typename... Args>
    T& MultiArray::ref(Args... args) noexcept;
const T& MultiArray::get(const position& p) const noexcept;
template <typename... Args>
    const T& MultiArray::get(Args... args) const noexcept;
```

Access the element at the given position. Behaviour is undefined if any
element of the position is out of bounds (i.e. if `contains(p)` is false).

```c++
iterator MultiArray::begin() noexcept;
const_iterator MultiArray::begin() const noexcept;
iterator MultiArray::end() noexcept;
const_iterator MultiArray::end() const noexcept;
```

Iterators at the beginning and past the end of the default iteration order.
The `begin()` functions return the same iterator as `locate(0,0,...)`.

```c++
void MultiArray::clear() noexcept;
```

Resets the array to an empty array (equivalent to `reset({0,0,...})`).

```c++
bool MultiArray::contains(const position& p) const noexcept;
template <typename... Args>
    bool MultiArray::contains(Args... args) const noexcept;
```

True if the position is within the array (none of the coordinates are out of
bounds).

```c++
T* MultiArray::data() noexcept;
const T* MultiArray::data() const noexcept;
```

These return a pointer to the array's internal data.

```c++
bool MultiArray::empty() const noexcept;
```

True if the array contains no elements (i.e. if any element of `shape()` is
zero).

```c++
void MultiArray::fill(const T& t);
```

Sets all elements of the array to the same value.

```c++
iterator MultiArray::locate(const position& p) noexcept;
const_iterator MultiArray::locate(const position& p) const noexcept;
template <typename... Args>
    iterator MultiArray::locate(Args... args) noexcept;
template <typename... Args>
    const_iterator MultiArray::locate(Args... args) const noexcept;
```

Return an iterator pointing to the element at the given position. Behaviour is
undefined if any element of the position is out of bounds (i.e. if
`contains(p)` is false).

```c++
void MultiArray::reset(const position& shape);
void MultiArray::reset(const position& shape, const T& t);
template <typename... Args> void MultiArray::reset(Args... args);
```

Replace the array with a new one, discarding the existing contents.
Optionally, a value to fill the array with can be supplied; otherwise, the new
array is uninitialised. Behaviour is undefined if any element of `shape` is
negative.

```c++
position MultiArray::shape() const noexcept;
```

Returns the dimensions of the array.

```c++
size_t MultiArray::size() const noexcept;
```

Returns the number of elements in the array (equal to the product of the
elements in `shape()`).

```c++
void MultiArray::swap(MultiArray& a) noexcept;
void swap(MultiArray& a, MultiArray& b) noexcept;
```

Swap two arrays.

```c++
bool operator==(const MultiArray& a, const MultiArray& b) noexcept;
bool operator!=(const MultiArray& a, const MultiArray& b) noexcept;
```

Comparison operators. These first check that the two shapes are the same, then
perform elementwise comparison.
