# Enumeration Tools

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/enum.hpp"
namespace Crow;
```

This header defines preprocessor macros that help to define enumeration
types.

## Contents

* TOC
{:toc}

## Enumeration definition

```c++
#define CROW_ENUM(EnumType, IntType,
    first_value, first_name, ...)
#define CROW_ENUM_CLASS(EnumType, IntType,
    first_value, first_name, ...)
```

These define an `enum` or `enum class` type. The macro arguments are the type
name, the underlying integer type, the integer value at which to start, and a
list of enumeration constant names.

This works only at namespace scope; it can't be used to define class member
types.

Example:

```c++
CROW_ENUM_CLASS(MyEnumClass, int, 1, alpha, bravo, charlie)
```

This is equivalent to:

```c++
enum class MyEnumClass: int {
    alpha = 1,
    bravo,
    charlie
};
```

In addition to defining the enumeration itself, the macros also define the
following functions:

* `constexpr IntType count_enum_values(EnumType) noexcept`
    * Returns the number of values in the enumeration. (The argument's value
      is ignored; a function argument is used to specify the type, instead of
      a template argument, so these functions can be found by argument
      dependent lookup.)
* `constexpr EnumType min_enum_value(EnumType) noexcept`
* `constexpr EnumType max_enum_value(EnumType) noexcept`
    * Return the minimum and maximum values of the enumeration.
* `std::vector<std::string> list_enum_names(EnumType)`
* `std::vector<EnumType> list_enum_values(EnumType)`
    * Return a list of the enumeration constants or their names.
* `bool parse_enum(const std::string& str, EnumType& t)`
    * If the string matches one of the enumeration names, this sets `t` to the
      corresponding value and returns true; otherwise, it leaves `t`
      unchanged and returns false.
* `std::string to_string(EnumType t)`
* `std::ostream& operator<<(std::ostream& out, EnumType t)`
    * These convert an enumeration value to a string containing its
      unqualified name. If an argument is supplied that does not correspond
      to a named enumeration constant, they will call `std::to_string()` on
      its integer value.

## Bitmask operators

```c++
#define CROW_BITMASK_OPERATORS(EnumClass)
```

This defines the standard bitwise operators on an enumeration type, allowing
an `enum class` to be used as a set of bitmask values. The following
operators are defined:

* Unary operators: `! ~`
* Binary operators: `& | ^ &= |= ^=`

These all have their natural meaning in terms of the corresponding operations
on the underlying integer type.

This can be used on enumeration classes that are defined either in namespace
scope or as class members; in the latter case the macro must be used in
namespace scope outside the class.
