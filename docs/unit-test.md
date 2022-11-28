# Unit Testing

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/unit-test.hpp"
namespace Crow::UnitTest;
```

## Contents

* TOC
{:toc}

## Files

Running the unit tests requires a user-created test wrapper in your project;
this is called `unit-test.cpp` by default, but can be named anything. A shell
script (`rs-update-tests`) is supplied that can be used to automatically
generate `unit-test.cpp` (if you follow some test naming conventions), but
this is not necessary. Writing `unit-test.cpp` by hand is simple, and an
example is supplied that explains what is needed.

The `rs-update-tests` script searches your CMake file for any source files
named `*-test.cpp`, then searches inside those files for functions with the
form `void test_*()`. All of those functions are assumed to be required test
functions, and will be run in the order in which the test files appear in
your CMake file.

The other script, `rs-bump-version`, finds the local `CMakeLists.txt` (of the
project you run it from), increments the patch number in the version string,
and regenerates the local `version.hpp`.

## Test utilities

```c++
template <typename Range> std::string format_range(const Range& r);
template <typename Range> std::string format_map(const Range& r);
```

These format a range (such as a vector) as `"[a,b,c,...]"`, or a range of
pairs (such as a map) as `"{a:1,b:2,c:3,...}"`.

```c++
template <typename T> std::string type_name();
```

Returns a human readable type name, derived from `std::type_info::name()`.
MSVC already supplies human readable names, so those are simply passed through
unchanged. On GCC, and GCC-like compilers such as Clang and Xcode, this will
attempt to demangle the name by calling `abi::__cxa_demangle()`; if this fails
it will give up and return the original name.

```c++
template <typename T> class Counted {
    Counted();
    Counted(const T& t);
    Counted(const Counted& c);
    Counted(Counted&& c) noexcept;
    ~Counted() noexcept;
    Counted& operator=(const Counted& c);
    Counted& operator=(Counted&& c) noexcept;
    Counted& operator=(const T& t);
    T& operator*() noexcept;
    const T& operator*() const noexcept;
    T* operator->() noexcept;
    const T* operator->() const noexcept;
    const T& get() const noexcept;
    void set(const T& t);
    static int count() noexcept;
    static void reset() noexcept;
};
bool operator==(const Counted<T>& a, const Counted<T>& b);
bool operator!=(const Counted<T>& a, const Counted<T>& b);
bool operator<(const Counted<T>& a, const Counted<T>& b);
bool operator>(const Counted<T>& a, const Counted<T>& b);
bool operator<=(const Counted<T>& a, const Counted<T>& b);
bool operator>=(const Counted<T>& a, const Counted<T>& b);
std::ostream& operator<<(std::ostream& out, const Counted<T>& c);
```

A type wrapper with population tracking. This is intended for testing code
that allocates and deallocates objects(e.g. containers or smart pointers);
the population count makes it easy to tell whether constructor and destructor
counts are matched.

The `*` and `->` operators, and the `get()` function, return references to the
contained value; `set()` changes the value. The `count()` function indicates
how many `Counted<T>` objects are currently in existence(this may be negative
if the destructor is called too many times and the process survives). The
`reset()` function sets the counter back to zero (so testing can continue
even if some objects have been leaked by earlier tests).

The comparison and output operators simply call the corresponding operators on
`T`.

```c++
void begin_tests(int argc, char** argv);
int end_tests();
```

Call these at the beginning and end of main(). Command line arguments passed
to `begin_tests()` can be used to control which tests are run. The
`end_tests()` function returns 0 on success, 1 if any tests failed; this value
should be returned from `main()`.

The unit test program should look like this:

```c++
#include "crow/unit-test.hpp"

int main(int argc, char** argv) {

    // Call begin_tests() at the start of your main() function. If any
    // command line arguments are passed, these are interpreted as regular
    // expressions, and only tests whose names match one of the expressions
    // will be run.

    Crow::UnitTest::begin_tests(argc, argv);

    // Call individual test functions using the UNIT_TEST() macro. The
    // argument is the name of the test function, minus the "test_" prefix.

    UNIT_TEST(example) // runs test_example()

    // Call end_tests() at the end of your main() function.

    return Crow::UnitTest::end_tests();

}
```

## Test macros

The `TRY(), TEST(),` and `REQUIRE()` macros will always evaluate their
argument exactly once. All of the other macros may evaluate any of their
arguments any number of times (including zero).

```c++
#define UNIT_TEST(name)
```

Call this from `main()` for each test. The argument is the name of the test
function, without the `test_` prefix.

```c++
#define FAIL_TEST(message)
```

Explicitly fail a test. This is normally only used internally, but can be
called explicitly if you choose.

```c++
#define TRY(expr)
```

Evaluate the expression, and fail if it throws an exception.

```c++
#define TEST(expr)
#define REQUIRE(expr)
```

Evaluate the expression, which is expected to return a `bool` or something
explicitly convertible to `bool`, and fail if the result is false or an
exception is thrown. `REQUIRE()` differs from `TEST()` in that it returns
immediately from the current test function on failure; this is intended for
situations where a test failure indicates that further testing would be
pointless or impossible.

```c++
#define TEST_EQUAL(lhs, rhs)
#define TEST_EQUAL_RANGES(lhs, rhs)
```

Evaluate the two expressions, and fail if they not equal, or if either of them
throws an exception.

```c++
#define TEST_IN_RANGE(expr, min, max)
```

Evaluate the expressions, and fail if the result is outside the range(i.e. if
`expr<min` or `expr>max`).

```c++
#define TEST_NEAR(lhs, rhs, epsilon)
```

Evaluate the expressions, which are expected to return `double` or something
implicitly convertible to `double`, and fail if the two values are not within
`epsilon` of each other.

```c++
#define TEST_MATCH(expr, pattern)
#define TEST_MATCH_ICASE(expr, pattern)
```

Evaluate the expression, which is expected to return a `std::string` or
something implicitly convertible to `std::string`, and fail if the expression
does not match the regex pattern, or throws an exception. The second version
does a case insensitive match.

```c++
#define TEST_THROW(expr, extype)
#define TEST_THROW_MESSAGE(expr, extype, message)
#define TEST_THROW_MATCH(expr, extype, pattern)
```

Evaluate the expression, and fail if it does not throw an exception, or throws
one of the wrong type. The second and third versions also check that the
error message matches a string (exactly) or a regular expression.

```c++
#define TEST_TYPE(lhs, rhs)
```

The two expressions are expected to be type expressions; fail if they are
different types.
