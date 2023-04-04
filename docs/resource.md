# Resource Handle

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/resource.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
template <typename T, typename U>
concept ResourceDeleter =
    std::is_default_constructible_v<T> && std::invocable<T, U>;
```

Concept representing a type that can be used as a deleter to free a resource.

## Resource class

```c++
template <std::regular T, ResourceDeleter<T> Del, T Null = T()>
    class Resource;
```

This is a wrapper class for resource handles (e.g. file handles). The
destructor will release the resource by calling the deleter function. The
null value represents a null resource handle; this defaults to zero or a null
pointer, but a different value can be supplied (e.g. -1 for a null Unix file
descriptor).

Behaviour is undefined if the deleter throws an exception.

```c++
using Resource::value_type = T;
using Resource::deleter_type = Del;
```

Member types.

```c++
static constexpr T Resource::null = Null;
```

Member constants.

```c++
constexpr Resource::Resource() noexcept;
constexpr explicit Resource::Resource(T t) noexcept;
Resource::~Resource() noexcept;
constexpr Resource::Resource(Resource&& r) noexcept;
Resource& Resource::operator=(Resource&& r) noexcept;
Resource& Resource::operator=(T t) noexcept;
```

Life cycle functions. `Resource` is movable but not copyable. The internal
handle in a moved-from `Resource` is reset to `Null`. The destructor will
call the deleter to release the resource if the internal handle is not equal
to `Null`. The LHS of the move assignment operator will also be released.

The assignment operator from a `T` is a synonym for `reset(t)`.

```c++
constexpr explicit Resource::operator T() const noexcept;
constexpr T Resource::get() const noexcept;
```

Return the internal handle.

```c++
constexpr explicit Resource::operator bool() const noexcept;
```

True if the handle is not equal to `Null`.

```c++
constexpr std::remove_pointer_t<T>& Resource::operator*() const noexcept;
constexpr std::remove_pointer_t<T>* Resource::operator->() const noexcept;
```

Dereference the internal handle. These are only defined if `T` is a pointer
type.

```c++
constexpr T Resource::release() noexcept;
```

Returns the resource handle and sets the object's internal handle to `Null`.

```c++
void Resource::reset() noexcept;
```

If the internal handle is not `Null`, this will call the deleter and then set
the handle to `Null`.

```c++
void Resource::reset(T t) noexcept;
```

Replaces the current internal handle. The deleter will be called on the
original handle if it is not `Null`.

```c++
constexpr void Resource::swap(Resource& r) noexcept;
constexpr void swap(Resource& a, Resource& b) noexcept;
```

Swap two resource handles.
