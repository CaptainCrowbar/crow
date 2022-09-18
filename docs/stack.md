# Stack

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/stack.hpp"
namespace Crow;
```

## Stack class

```c++
template <typename T> class Stack {
    using iterator = [random access iterator];
    using const_iterator = [random access iterator];
    using value_type = T;
    Stack();
    ~Stack() noexcept;
    Stack(Stack&& s) noexcept;
    Stack& operator=(Stack&& s) noexcept;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    void clear() noexcept;
    template <typename... Args> void emplace(Args&&... args);
    bool empty() const noexcept;
    void pop() noexcept;
    void push(const T& t);
    void push(T&& t);
    size_t size() const noexcept;
    T& top() noexcept;
    const T& top() const noexcept;
};
```

This is a simple LIFO container, essentially the same as `std::stack` apart
from the added guarantee that its elements are destroyed in reverse order of
insertion. Behaviour is undefined if `pop()` or `top()` is called on an empty
stack.
