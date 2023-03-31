# Crow Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/crow)_

## Overview

This is my unified utility library. Everything here is in `namespace Crow`.
The `CMakeLists.txt` includes an `install` target to copy the headers and
static library into `/usr/local/include` and `/usr/local/bin` or the
equivalent.

Other libraries required:

* All operating systems
    * [PCRE2](http://www.pcre.org/) - Required by the Regex module and anything that uses it
    * [Libcurl](https://curl.se) - Required by the Web Client module
    * [Sqlite3](https://www.sqlite.org/index.html) - Required by the Sqlite module
* Operating system specific
    * Linux
        * `crypto` - Required for cryptographic hash functions
    * Windows
        * `ws2_32` - Required for networking

## Index

* Basic types
    * [crow/enum](enum.html) - Enumeration types
    * [crow/guard](guard.html) - Scope guards
    * [crow/iterator](iterator.html) - Iterator types and algorithms
    * [crow/meta](meta.html) - Metaprogramming utilities
    * [crow/types](types.html) - Basic types
    * [crow/uuid](uuid.html) - UUID
* Algorithms
    * [crow/algorithm](algorithm.html) - Algorithms
    * [crow/hash](hash.html) - Hash functions
    * [crow/topological-order](topological-order.html) - Topological order
* Command line
    * [crow/options](options.html) - Command line options
* Containers
    * [crow/bounded-array](bounded-array.html) - Bounded array on the stack
    * [crow/cache](cache.html) - LRU cache
    * [crow/compact-array](compact-array.html) - Compact array optimized for small size
    * [crow/flexible-map](flexible-map.html) - Maps and sets with flexible implementation
    * [crow/index-table](index-table.html) - Indexed table with multiple keys
    * [crow/linear-map](linear-map.html) - Linear interpolated map
    * [crow/mirror-map](mirror-map.html) - Map with two-way lookup
    * [crow/multi-array](multi-array.html) - Multi-dimensional array
    * [crow/spatial-index](spatial-index.html) - Spatial index
    * [crow/stack](stack.html) - Stack
    * [crow/superposition](superposition.html) - Superposition of states
* Database
    * [crow/sqlite](sqlite.html) - Sqlite
* Graphics
    * [crow/colour](colour.html) - Colour
    * [crow/colour-space](colour-space.html) - Colour space
    * [crow/font](font.html) - Fonts
    * [crow/geometry](geometry.html) - Geometric primitives
    * [crow/image](image.html) - Image
* I/O
    * [crow/log](log.html) - Logging
    * [crow/path](path.html) - File path
    * [crow/progress](progress.html) - Progress bar
    * [crow/stdio](stdio.html) - Standard I/O
    * [crow/terminal](terminal.html) - Terminal control
* Intervals
    * [crow/interval](interval.html) - Interval class
    * [crow/interval-map](interval-map.html) - Interval map class
    * [crow/interval-set](interval-set.html) - Interval set class
* Linear algebra
    * [crow/matrix](matrix.html) - Matrix
    * [crow/quaternion](quaternion.html) - Quaternion
    * [crow/transform](transform.html) - Transforms
    * [crow/vector](vector.html) - Vector
* Mapping
    * [crow/hexmap](hexmap.html) - Hex mapping
    * [crow/projection](projection.html) - Map projections
* Multithreading
    * [crow/channel](channel.html) - Channel-based message dispatch
    * [crow/named-mutex](named-mutex.html) - Named mutex
    * [crow/thread](thread.html) - Thread
    * [crow/thread-pool](thread-pool.html) - Thread pool
* Networking
    * [crow/http](http.html) - HTTP status codes
    * [crow/net](net.html) - TCP/IP networking
    * [crow/uri](uri.html) - URI
    * [crow/web-client](web-client.html) - Web client
* Numerics
    * [crow/approx](approx.html) - Approximate values
    * [crow/binary](binary.html) - Bitwise operations
    * [crow/constants](constants.html) - Constants
    * [crow/fixed-binary](fixed-binary.html) - Fixed-size binary numbers
    * [crow/log-scale](log-scale.html) - Log scale floating point
    * [crow/maths](maths.html) - Mathematical utilities
    * [crow/mp-integer](mp-integer.html) - Multiple precision integers
    * [crow/prime](prime.html) - Prime numbers
    * [crow/probability](probability.html) - Probability
    * [crow/rational](rational.html) - Rational numbers
    * [crow/root-finding](root-finding.html) - Root finding
    * [crow/statistics](statistics.html) - Statistics
* Random generation
    * [crow/dice](dice.html) - Dice
    * [crow/markov](markov.html) - Markov text generator
    * [crow/noise](noise.html) - Pseudo-random noise
    * [crow/random](random.html) - Random number generators
    * [crow/text-gen](text-gen.html) - Text generators
* Science
    * [crow/formula](formula.html) - Useful formulae
    * [crow/spectrum](spectrum.html) - Stellar spectral class
* System facilities
    * [crow/dso](dso.html) - Dynamic libraries
    * [crow/process](process.html) - Process control
    * [crow/signal](signal.html) - Signal handling
* Text manipulation
    * [crow/encoding](encoding.html) - Encoding conversion
    * [crow/english](english.html) - English language tools
    * [crow/format](format.html) - Formatting functions
    * [crow/markup](markup.html) - HTML/XML markup
    * [crow/name](name.html) - Name formatting
    * [crow/regex](regex.html) - Regular expressions
    * [crow/string](string.html) - String functions
    * [crow/table](table.html) - Table layout
    * [crow/unicode](unicode.html) - Unicode functions
* Time and date
    * [crow/benchmark](benchmark.html) - Benchmarking
    * [crow/time](time.html) - Time and date
* Unit testing
    * [crow/unit-test](unit-test.html) - Unit test
* Library version
    * [crow/version](version.html) - Version
