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

## Enumeration definitions

```c++
#define CROW_ENUM_UNSCOPED(EnumName, IntType, ...)
#define CROW_ENUM_SCOPED(EnumName, IntType, ...)
```

These define an `enum` or `enum class` type. The macro arguments are the type
name, the underlying integer type, and a list of enumeration constants in the
usual format.

This works only at namespace scope; it can't be used to define class member
types.

Example:

```c++
CROW_ENUM_CLASS(MyEnumClass, int, alpha = 1, bravo, charlie)
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

```c++
std::vector<std::pair<EnumName, std::string>> enum_value_name_vector(EnumName);
std::unordered_map<EnumName, std::string> enum_value_name_map(EnumName);
std::unordered_map<std::string, EnumName> enum_name_value_map(EnumName);
```

These return a collection of paired enumeration values and their unqualified
names as strings, in various forms. For all of these functions, the argument
is ignored and only exists to allow the correct function to be resolved via
argument dependent lookup. If an enumeration contains duplicate values, the
name associated with an ambiguous value in `enum_value_name_map()` is the
first name associated with that value in the original definition.

```c++
std::string to_string(EnumName value);
std::ostream& operator<<(std::ostream& out, EnumName x);
```

Convert an enumeration value to a string containing the unqualified name. If
the argument is not a named enumeration value, the decimal integer value will
be returned.

```c++
bool enum_value(const std::string& name, EnumName& value);
```

Convert an unqualified name to an enumeration value. If the name is not one of
the enumeration names, this will leave the second argument unchanged and
return false.


## Bitmask enumerations

```c++
#define CROW_ENUM_BITMASK(EnumName, IntType, ...)
```

This defines a scoped enumeration in the same way as `CROW_ENUM_SCOPED()`, and
also defines the standard bitwise operators on the type, allowing an
enumeration to be used as a set of bitmask values. The following operators
are defined:

* Unary operators: `! ~`
* Binary operators: `& | ^ &= |= ^=`

These all have their natural meaning in terms of the corresponding operations
on the underlying integer type.

```c++
#define CROW_ENUM_BITMASK_OPERATORS(EnumName)
```

This defines the same operators on an existing enumeration type. If the
enumeration is a class member type, the macro must be used in namespace scope
outside the class.
