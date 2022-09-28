# Crow Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/crow)_

## Overview

This is my unified utility library.

```c++
#include "crow.hpp"
namespace Crow;
```

The CMake file includes an `install` target to copy the headers and static
library into `/usr/local/include` and `/usr/local/bin` or the equivalent.
Headers can be included individually as required, or the entire library can
be included using the full library header.

Other libraries required:

* [PCRE2](http://www.pcre.org/) - Required by the Regex module and anything that uses it
* [Sqlite3](https://www.sqlite.org/index.html) - Required by the Sqlite module

## Index

* Basic types
    * [Basic types](types.html)
    * [Enumeration types](enum.html)
    * [Iterator types and algorithms](iterator.html)
    * [Metaprogramming utilities](meta.html)
    * [Scope guards](guard.html)
    * [UUID](uuid.html)
* Algorithms
    * [Algorithms](algorithm.html)
    * [Hash functions](hash.html)
    * [Topological order](topological-order.html)
* Command line
    * [Command line options](options.html)
* Containers
    * [Bounded array on the stack](bounded-array.html)
    * [Compact array optimized for small size](compact-array.html)
    * [Indexed table with multiple keys](index-table.html)
    * [Linear interpolated map](linear-map.html)
    * [Map with two-way lookup](mirror-map.html)
    * [Multi-dimensional array](multi-array.html)
    * [Stack](stack.html)
* Database
    * [Sqlite](sqlite.html)
* Graphics
    * [Colour](colour.html)
    * [Colour space](colour-space.html)
    * [Fonts](font.html)
    * [Geometric primitives](geometry.html)
    * [Image](image.html)
* I/O
    * [Logging](log.html)
    * [File path](path.html)
    * [Progress bar](progress.html)
    * [Standard I/O](stdio.html)
    * [Terminal control](terminal.html)
* Intervals
    * [Interval class](interval.html)
    * [Interval map class](interval-map.html)
    * [Interval set class](interval-set.html)
* Linear algebra
    * [Vector](vector.html)
    * [Matrix](matrix.html)
    * [Quaternion](quaternion.html)
    * [Transforms](transform.html)
* Mapping
    * [Hex mapping](hexmap.html)
    * [Map projections](projection.html)
* Multithreading
    * [Channel-based message dispatch](channel.html)
    * [Named mutex](named-mutex.html)
    * [Thread](thread.html)
    * [Thread pool](thread-pool.html)
* Networking
    * [TCP/IP networking](net.html)
    * [URI](uri.html)
* Numerics
    * [Bitwise operations](binary.html)
    * [Constants](constants.html)
    * [Fixed-size binary numbers](fixed-binary.html)
    * [Useful formulae](formula.html)
    * [Mathematical utilities](maths.html)
    * [Multiple precision integers](mp-integer.html)
    * [Prime numbers](prime.html)
    * [Rational numbers](rational.html)
    * [Root finding](root-finding.html)
    * [Statistics](statistics.html)
* Random generation
    * [Dice](dice.html)
    * [Markov text generator](markov.html)
    * [Pseudo-random noise](noise.html)
    * [Random number generators](random.html)
    * [Text generators](text-gen.html)
* System facilities
    * [Dynamic libraries](dso.html)
    * [Process control](process.html)
    * [Signal handling](signal.html)
* Text manipulation
    * [Encoding conversion](encoding.html)
    * [English language tools](english.html)
    * [Formatting functions](format.html)
    * [HTML/XML markup](markup.html)
    * [Regular expressions](regex.html)
    * [String functions](string.html)
    * [Table layout](table.html)
    * [Unicode functions](unicode.html)
* Time and date
    * [Time and date](time.html)
* Unit testing
    * [Unit test](unit-test.html)
* Library version
    * [Version](version.html)
