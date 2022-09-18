# Metaprogramming Utilities

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/meta.hpp"
namespace Crow;
```

Metafunctions are templates that operate on types; usually they take one or
more types as arguments and "return" another type via a member type, but
non-type template arguments and return types are also possible.

Metafunctions that return a type are implemented as a class or struct template
with a name ending in `Type` and a member type named `type`, and also as an
alias template (without the `Type` suffix) evaluating directly to the result
type.

Metafunctions that return a boolean or integer value are implemented as a
template type with a `value` member constant, and as a template constant that
yields the value directly.

## Contents

* TOC
{:toc}

## Type lists ##

A typelist is an instantiation of the `Typelist<...>` template. `Nil` is a
synonym for the empty typelist. A collection of metafunctions are defined
here that operate on typelists.

Example:

    using SignedTypes = Typelist<short, int, long>;
    using UnsignedTypes = Typelist<unsigned short, unsigned int,
        unsigned long>;
    using IntegerTypes = Concat<SignedTypes, UnsignedTypes>;
    static constexpr int n_types = length_of<IntegerTypes>; // = 6

In the documentation below, the following abbreviations are used to indicate
the requirements on template parameters:

* `T` -- General type
* `TL` -- Typelist
* `SL` -- Sorted typelist
* `UF` -- Unary metafunction _(T1 → T2)_
* `UP` -- Unary predicate metafunction _(T → bool)_
* `BF` -- Binary metafunction _(T1 × T2 → T3)_
* `BP` -- Binary predicate metafunction _(T1 × T2 → bool)_
* `CP` -- Comparison predicate metafunction _(T1 × T2 → bool)_
* `N` -- Integer

Indexing into typelists is zero-based; functions that return an index will
return -1 to indicate failure. A comparison predicate is expected to have
less-than semantics, in the same way as the comparison predicates used for
ordering in the STL.

### Typelist primitives

```c++
template <typename... TS> struct Typelist {};
using Nil = Typelist<>;
```

* `template <typename... TS> struct` **`Typelist`**
* `using` **`Nil`** `= Typelist<>`

## Type-valued metafunctions

### Typelist composition and decomposition

| Name                       | Returns  | Description                                                 |
| ----                       | -------  | -----------                                                 |
| `Append[Type]<TL,T>`       | `TL`     | Insert T at the end of TL                                   |
| `Concat[Type]<TL,...>`     | `TL`     | Concatenate typelists or types; argument list may be empty  |
| `Insert[Type]<TL,T,CP>`    | `TL`     | Insert T in a sorted list                                   |
| `InsertAt[Type]<TL,T,N>`   | `TL`     | Insert T at index N in TL                                   |
| `Prefix[Type]<T,TL>`       | `TL`     | Insert T at the beginning of TL                             |
| `Repeat[Type]<T,N>`        | `TL`     | Concatenate N copies of T                                   |
| `RepeatList[Type]<TL,N>`   | `TL`     | Concatenate N copies of TL                                  |
| `Resize[Type]<TL,N,T>`     | `TL`     | Resize list to length N; use T for padding                  |
| `Skip[Type]<TL,N>`         | `TL`     | Discard the first N elements from TL                        |
| `Sublist[Type]<TL,N1,N2>`  | `TL`     | Discard N1 elements and return next N2 elements             |
| `Take[Type]<TL,N>`         | `TL`     | First N elements in TL                                      |

### Type selection

| Name                   | Returns  | Description                                              |
| ----                   | -------  | -----------                                              |
| `AtIndex[Type]<TL,N>`  | `T`      | Nth element in TL; Nil if N is out of bounds             |
| `Head[Type]<TL>`       | `T`      | First element in TL; Nil if TL is Nil                    |
| `Last[Type]<TL>`       | `T`      | Last element in TL; Nil if TL is Nil                     |
| `Max[Type]<TL,CP>`     | `T`      | Maximum element in TL according to CP; Nil if TL is Nil  |
| `Min[Type]<TL,CP>`     | `T`      | Minimum element in TL according to CP; Nil if TL is Nil  |
| `Most[Type]<TL>`       | `TL`     | All but the last element in TL; Nil if TL is Nil         |
| `Tail[Type]<TL>`       | `TL`     | All but the first element in TL; Nil if TL is Nil        |

### Typelist transformation

| Name                             | Returns  | Description                                                                   |
| ----                             | -------  | -----------                                                                   |
| `FoldLeft[Type]<TL,BF,T>`        | `T`      | Left-to-right reduce, with T as initial value                                 |
| `FoldRight[Type]<TL,BF,T>`       | `T`      | Right-to-left reduce, with T as initial value                                 |
| `FullyUnique[Type]<TL>`          | `TL`     | Drop all duplicate elements                                                   |
| `MakeSet[Type]<TL,CP>`           | `TL`     | Sort followed by Unique                                                       |
| `Map[Type]<TL,UF>`               | `TL`     | Replace each element T in TL with UF\<T\>                                     |
| `PartialReduce[Type]<TL,BP,BF>`  | `TL`     | Reduce adjacent elements matched with BP                                      |
| `Reduce[Type]<TL,BF>`            | `T`      | Left-to-right reduce, with no initial value; Nil if TL is Nil                 |
| `Remove[Type]<TL,T>`             | `TL`     | Remove all occurrences of T in TL                                             |
| `RemoveIf[Type]<TL,UP>`          | `TL`     | Remove all elements for which UP is true                                      |
| `Reverse[Type]<TL>`              | `TL`     | Reverse the order of TL                                                       |
| `Select[Type]<TL,UP>`            | `TL`     | All elements in TL for which UP is true                                       |
| `Sort[Type]<TL,CP>`              | `TL`     | Sort TL according to CP (stable sort)                                         |
| `Unique[Type]<TL>`               | `TL`     | Drop adjacent duplicate elements                                              |
| `Zip[Type]<TL1,TL2,BF>`          | `TL`     | Map each pair of elements to BF\<T1,T2\> (output has length of shorter list)  |

### Other typelist metafunctions

| Name                         | Returns  | Description                                  |
| ----                         | -------  | -----------                                  |
| `InheritTypelist[Type]<TL>`  | `type`   | Inherits from all unique types in the list   |
| `TupleToTypelist[Type]<T>`   | `TL`     | List with the same types as a tuple or pair  |
| `TypelistToTuple[Type]<TL>`  | `tuple`  | Tuple with the same types as the list        |

## Integer-valued metafunctions

| Type                 | Constant               | Returns  | Description                                                     |
| ----                 | --------               | -------  | -----------                                                     |
| `Count<TL,T>`        | `count<TL,T>`          | `int`    | Number of times T occurs in TL                                  |
| `CountIf<TL,UP>`     | `count_if<TL,UP>`      | `int`    | Number of elements in TL for which UP is true                   |
| `CountIfNot<TL,UP>`  | `count_if_not<TL,UP>`  | `int`    | Number of elements in TL for which UP is false                  |
| `Find<TL,T>`         | `find<TL,T>`           | `int`    | Index of the first T in TL; -1 if not found                     |
| `FindIf<TL,UP>`      | `find_if<TL,UP>`       | `int`    | Index of the first matching element in TL; -1 if not found      |
| `FindIfNot<TL,UP>`   | `find_if_not<TL,UP>`   | `int`    | Index of the first non-matching element in TL; -1 if not found  |
| `LengthOf<TL>`       | `length_of<TL>`        | `int`    | Number of entries in TL                                         |

## Predicate metafunctions

| Type              | Constant            | Returns  | Description                                               |
| ----              | --------            | -------  | -----------                                               |
| `AllOf<TL,UP>`    | `all_of<TL,UP>`     | `bool`   | True if all types match UP; true if TL is Nil             |
| `AnyOf<TL,UP>`    | `any_of<TL,UP>`     | `bool`   | True if at least one type matches UP; false if TL is Nil  |
| `InList<TL,T>`    | `in_list<TL,T>`     | `bool`   | True if T is in TL                                        |
| `IsEmpty<TL>`     | `is_empty<TL>`      | `bool`   | True if TL is Nil                                         |
| `IsNotEmpty<TL>`  | `is_not_empty<TL>`  | `bool`   | True if TL is a typelist and is not Nil                   |
| `IsUnique<TL>`    | `is_unique<TL>`     | `bool`   | True if TL contains no duplicates                         |
| `NoneOf<TL,UP>`   | `none_of<TL,UP>`    | `bool`   | True if no types match UP; true if TL is Nil              |
