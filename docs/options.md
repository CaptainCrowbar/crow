# Command Line Options

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/options.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Options Class

```c++
class Options;
```

The `Options` class in this module handles parsing of command line options,
and automatic generation of help messages for the user. It provides some
commonly expected command line features:

* Long option names prefixed with two hyphens, e.g. `--long-option`.
* Single letter abbreviations, e.g. `-x`.
* Combinations of abbreviated options, e.g. `-abc` is equivalent to
  `-a -b -c`.
* Options can be followed by arguments, delimited by a space or an equals
  sign, e.g. `--name value` or `--name=value`.
* Options can have default arguments.
* Options may take multiple arguments, e.g. `--option arg1 arg2 arg3`.
* Boolean options can be prefixed with `--no-` to invert them.
* An option's arguments can be checked against a regular expression.
* Unattached arguments can be implicitly assigned to options.
* Sets of mutually exclusive options can be specified.
* An argument consisting only of two hyphens (`"--"`) marks the end of
  explicitly named options; any text following it is read as unattached
  arguments, even if it looks like an option.

A program will normally construct an `Options` object, and use multiple calls
to `add()` to construct the option specification, before calling `parse()` to
parse the actual command line arguments.

### Example

```c++
int main(int argc, char** argv) {
    std::string alpha, omega;
    int number = 42;
    Options opt("My Program", "1.0", "Goes ding when there's stuff.");
    opt.add(alpha, "alpha", 'a', "The most important option");
    opt.add(omega, "omega", 0, "The least important option");
    opt.add(number, "number", 'n', "How many roads to walk down");
    if (! opt.parse(argc, argv))
        return 0;
    // ... main code ...
}
```

If this program is invoked with the `--help` option (or `-h`), it will display
the following information on standard output:

    My Program 1.0

    Goes ding when there's stuff.

    Options:

        --alpha, -a <arg>   = The most important option
        --omega <arg>       = The least important option
        --number, -n <int>  = How many roads to walk down (default 42)
        --help, -h          = Show usage information
        --version, -v       = Show version information

### Member types

```c++
enum class Options::flag_type;
    Options::none = 0;
    Options::anon;
    Options::no_default;
    Options::random;
    Options::required;
    Options::dir_exists;
    Options::file_exists;
    Options::not_exists;
    Options::parent_exists;
using enum Options::flag_type;
```

These are bitmasks that can be used in the flags argument of `Options::add()`.

* The `anon` flag indicates that the option name can be left out on the
  command line; the first unclaimed argument (not yet assigned to any other
  option) will be assigned to it. If this is a container-valued option, all
  remaining unclaimed arguments will be assigned to it.
* The `no_default` flag suppresses the display of the default value in the
  help text.
* The `random` flag will generate a random value (0-max for integers, 0-1 for
  floating point) using `std::random_device` if a value is not supplied.
* The `required` flag indicates that this option must be supplied (this does
  not apply if the user selects the `--help` or `--version` options).
* The `dir_exists` and `file_exists` flags indicate that the argument must be
  a path (absolute or relative) to an existing directory or non-directory
  file, respectively. If both flags are supplied, then either a directory or
  file is acceptable.
* The `not_exists` flag indicates that the argument must not be the path to an
  existing file or directory.
* The `parent_exists` flag indicates that the argument must be a syntactically
  valid path that may or may not exist, but its parent directory must exist.
  This has no effect if combined with `dir_exists` or `file_exists`, but it
  may be combined with `not_exists`.

```c++
class Options::setup_error: public std::logic_error;
class Options::user_error: public std::runtime_error;
```

Exceptions thrown by `Options` member functions if anything goes wrong. The
`setup_error` exception indicates that the programmer has made an error in
configuring the options; `user_error` indicates that an invalid set of
arguments were supplied on the command line.

### Life cycle functions

```c++
Options::Options(const std::string& app, const std::string& version,
    const std::string& description, const std::string& extra = {});
```

Constructor. The arguments are the application name, a version number
(which can be an arbitrary string, and may be empty), a description of the
app (to be printed at the top of the help text), and optionally some further
information (to be printed at the bottom of the help text).

This will throw `setup_error` if `app` or `description` is empty (or contains
only whitespace characters).

```c++
Options::Options();
Options::Options(const Options& opts);
Options::Options(Options&& opts) noexcept;
Options::~Options() noexcept;
Options& Options::operator=(const Options& opts);
Options& Options::operator=(Options&& opts) noexcept;
```

Other life cycle functions.

### Configuration functions

```c++
template <typename T> Options& Options::add(T& var,
    const std::string& name, char abbrev, const std::string& description,
    flag_type flags = flag_type::none, const std::string& group = {},
    const std::string& pattern = {});
```

Adds an option to the configuration. The arguments are:

* `var` -- The variable into which the value supplied on the command line will
  be written. The allowed types are described below.
* `name` -- The long form of the option name. This can be supplied here with or
  without the `"--"` prefix; either way, the full `--name` will be checked
  for on the command line.
* `abbrev` -- A single character abbreviation, which can be supplied on the
  command line as e.g. `-a`. This is optional; set it to zero if you don't
  want to allow an abbreviated form of this option. Any printable ASCII
  character except the space and hyphen may be used here.
* `description` -- Description of what the option does. This will be reproduced
  in the help text.
* `flags` -- A bitmask consisting of zero or more of the `flag_type`
  enumerations.
* `group` -- Set this to a non-empty string to create a group of mutually
  exclusive options. A user error will be raised if more than one option from
  the same group is supplied.
* `pattern` -- Set this to a non-empty string to supply a regular expression
  that any arguments attached to this option must match. This is only usable
  with string-valued options. Prefix the pattern with `"(?i)"` for case
  insensitive matching.

The variable into which the parsed value is written must be one of the
following types:

* `bool` -- This is a special case. A boolean option is not followed by any
  associated arguments. The boolean variable supplied here will simply be set
  to true if the option is present.
* `std::string` -- This does no argument checking (unless a pattern is
  supplied), but simply copies an argument string from the command line.
* Any type implicitly or explicitly constructible from a string or string
  view.
* Any standard arithmetic type -- The argument supplied on the command line
  will be parsed as an integer or floating point value, including range
  checking, throwing `user_error` if an invalid value is passed.
* An enumeration type -- The argument passed on the command line must match
  one of the type's enumeration values. This will only work with enumerations
  defined using the `CROW_DEFINE_ENUM[_CLASS]()` macros; behaviour is
  undefined if any other enumeration type is used.
* A container of any of the above types. The type can be any STL compatible
  container (except `std::basic_string`) that accepts insertion of one of
  these types. Any of the standard sequential containers (`vector`, `deque`,
  `list`) will work, as will `set` and other set-like containers, but `map`
  and other map-like containers will not. Using a container as the output
  variable implies that the option can accept multiple arguments; all
  arguments following the option name, up to the next option, are added to
  the container.

The initial value of the variable is used as a default if the option is not
present on the command line. Behaviour is undefined if the variable's value is
changed after `add()` has been called on it (other than by `parse()`).

The `Options` class will automatically add the `--help` and `--version`
options, after all caller-supplied options, with the abbreviations `-h` and
`-v` if those have not been used by any previous options.

The `add()` function will throw `setup_error` under any of the following
conditions:

* The option name has less than two characters (not counting any leading
  hyphens).
* The name or abbreviation contains any whitespace or control characters.
* The abbreviation is not an ASCII character.
* The name or abbreviation has already been used by another option.
* The description string is empty (or contains only whitespace).
* The `anon` or `required` flag is used with a boolean option.
* The `random` flag is used with a non-arithmetic type, or a non-zero default
  value.
* The `not_exists` flag is combined with `dir_exists` or `file_exists`.
* Any of the file-related flags are used with an argument type other than
  `std::string`, `Path`, or a container of one of those.
* Any anonymous options appear after an anonymous, container-valued option
  (which will have already swallowed up any remaining unattached arguments).
* A required option is in a mutual exclusion group.
* A pattern or a file-related flag is supplied for a variable of boolean,
  arithmetic, or enumeration type.
* The pattern is not a valid PCRE2 regular expression.
* You try to create the `--help` or `--version` options explicitly.

If more than one of these error conditions exists for the same option, it is
unspecified which one will be reported.

Default values are not checked against regex patterns or fie system
requirements.

Behaviour is undefined if `add()` is called after `parse()`.

```c++
void Options::auto_help() noexcept;
```

If this is set, an empty argument list will be interpreted as a request for
help (equivalent to `--help`).

```c++
void Options::set_colour(bool b) noexcept;
```

This can be used to control whether or not the help output is colourised. By
default, if help is requested, the `parse()` function will call `isatty(1)` to
determine whether standard output has been redirected, and will emit colour
codes only if it believes it is writing to a terminal. This function overrides
the automatic detection.

### Command line parsing functions

```c++
bool Options::parse(int argc, char** argv,
    std::ostream& out = std::cout);
bool Options::parse(std::vector<std::string> args,
    std::ostream& out = std::cout);
```

After the options have been configured, call one of these functions to parse
the actual command line arguments. The first version takes the standard
arguments from `main()` directly; the second takes a vector of argument
strings (not including the command name). The output stream argument
indicates where to write any help or version information requested by the
user, defaulting to standard output.

The return value is true if the command line arguments have been successfully
parsed and the program can continue processing. If `parse()` returns false,
then help or version information has been written to the output stream, and
the caller should stop processing and exit here (probably by returning from
`main()`).

The `parse()` functions will throw `user_error` under any of the following
conditions:

* A long or short option name is supplied that is not in the configuration.
* The same option appears more than once, but is not container-valued.
* A required option is missing.
* More than one option from the same mutual exclusion group is supplied.
* The argument supplied for a numeric option can't be parsed as the correct
  data type.
* The argument supplied for a numeric option is out of range for its data
  type.
* The argument supplied for an enumeration-valued option is not one of the
  valid enumeration values.
* The argument supplied for a string option does not match the pattern
  specified for it.
* The argument supplied for a file option does not meet the flag condition, or
  is not a valid file path for this operating system.
* There are unclaimed arguments left over after all options have been
  satisfied.

(If more than one of these error conditions exists for the same argument, it
is unspecified which one will be reported.)

```c++
bool Options::found(const std::string& name) const;
```

True if the named option was found on the command line (the leading `"--"` is
optional). This will always return false if the name does not match any of
the configured options.
