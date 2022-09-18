# Topological Order

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/topological-order.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Exceptions

```c++
class TopologicalOrderError: public std::runtime_error;
    class TopologicalOrderCycle: public TopologicalOrderError;
    class TopologicalOrderEmpty: public TopologicalOrderError;
```

Exceptions reporting errors in topological order operations.

## Topological order container

```c++
template <typename T, typename Compare = std::less<T>> class TopologicalOrder;
```

A container that sorts its elements according to a caller-supplied topological
order, given a set of _(before,after)_ relations. The comparison object is
used only to sort elements within a subset; it does not participate in the
topological relation.

```c++
using compare_type = Compare;
using value_type = T;
```

Member types.

```c++
TopologicalOrder::TopologicalOrder();
explicit TopologicalOrder::TopologicalOrder(Compare c);
TopologicalOrder::TopologicalOrder(const TopologicalOrder& obj);
TopologicalOrder::TopologicalOrder(TopologicalOrder&& obj) noexcept;
TopologicalOrder::~TopologicalOrder() noexcept;
TopologicalOrder& TopologicalOrder::operator=(const TopologicalOrder& obj);
TopologicalOrder& TopologicalOrder::operator=(TopologicalOrder&& obj) noexcept;
```

Life cycle functions.

```c++
void TopologicalOrder::clear() noexcept;
```

Removes all elements from the container.

```c++
Compare TopologicalOrder::comp() const;
```

Returns a copy of the comparison predicate.

```c++
bool TopologicalOrder::contains(const T& t) const;
```

True if the element is in the container.

```c++
bool TopologicalOrder::empty() const noexcept;
```

True if the container is empty.

```c++
bool TopologicalOrder::erase(const T& t);
```

Erase a specific element from the container. This will do nothing if the
element was not present.

```c++
T TopologicalOrder::front() const;
std::vector<T> TopologicalOrder::front_set() const;
T TopologicalOrder::back() const;
std::vector<T> TopologicalOrder::back_set() const;
```

Query the current front set (elements that never appear on the right of an
ordering relation) or back set (elements that never appear on the left), or
the first element in each set. The elements in each returned list will be
ordered according to the comparison predicate. An element that has no
ordering relations with any other element will appear in both sets. The
`front()` and `back()` functions will throw `TopologicalOrderEmpty` if the
container is empty, or `TopologicalOrderCycle` if a cycle is detected; the
set functions will throw if a cycle is detected but will simply return an
empty set if the container is empty. Not all cycles will be detected by these
functions (a cycle that involves only interior elements, while still leaving
well defined front and back sets, will not be detected here).

```c++
template <typename... Args> void TopologicalOrder::insert(const T& t, const Args&... args);
template <typename Range> void TopologicalOrder::insert_n(const Range& r);
```

Insert one or more elements. If only one element is inserted, it has no
implied ordering relations with any other elements. If multiple elements are
listed, an ordering relation is implied between each element and all elements
after it in the list. In all insert functions, implied relations between an
element and itself are ignored.

```c++
template <typename Range> void TopologicalOrder::insert_1n(const T& t1, const Range& r2);
void TopologicalOrder::insert_1n(const T& t1, std::initializer_list<T> r2);
template <typename Range> void TopologicalOrder::insert_n1(const Range& r1, const T& t2);
void TopologicalOrder::insert_n1(std::initializer_list<T> r1, const T& t2);
template <typename Range1, typename Range2> void TopologicalOrder::insert_mn(const Range1& r1, const Range2& r2);
void TopologicalOrder::insert_mn(std::initializer_list<T> r1, std::initializer_list<T> r2);
```

Insert one or more pairs of elements, with ordering relations between the left
and right arguments. The `insert_1n()` function creates an ordering relation
between the first argument and every element of the second range;
`insert_n1()` creates an ordering relation between every element of the first
range and the second element; `insert_mn()` creates an ordering relation
between every pair in the Cartesian product of the two ranges.

```c++
bool TopologicalOrder::is_front(const T& t) const;
bool TopologicalOrder::is_back(const T& t) const;
```

True if the give element is part of the current front or back set.

```c++
T TopologicalOrder::pop_front();
std::vector<T> TopologicalOrder::pop_front_set();
T TopologicalOrder::pop_back();
std::vector<T> TopologicalOrder::pop_back_set();
```

Remove one or all elements of the current front or back set. These will throw
`TopologicalOrderCycle` or `TopologicalOrderEmpty` under the same
circumstances as the corresponding front or back element or set functions.

```c++
size_t TopologicalOrder::size() const noexcept;
```

Returns the number of elements in the container.
