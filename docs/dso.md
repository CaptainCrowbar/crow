# Dynamic Libraries

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/dso.hpp"
namespace Crow;
```

## DSO class ##

```c++
class Dso;
```

This class loads a dynamic library when constructed. Symbols from the library
can be loaded at run time.

```c++
enum class Dso::flag_type;
using enum flag_type;
```

The type used for bitmask flags.

| Flag                  | System   | Equivalent                             | Behaviour                                               |
| ----                  | ------   | ----------                             | ---------                                               |
| `none`                | All      | 0                                      | None                                                    |
| `lazy`                | Posix    | `RTLD_LAZY`                            | Relocations may be delayed                              |
| `now`                 | Posix    | `RTLD_NOW`                             | Relocations are performed immediately                   |
| `global`              | Posix    | `RTLD_GLOBAL`                          | Symbols are available when relocating other libraries   |
| `local`               | Posix    | `RTLD_LOCAL`                           | Symbols are not visible                                 |
| `noload`              | Apple    | `RTLD_NOLOAD`                          | Do not load, succeed only if already loaded             |
| `nodelete`            | Apple    | `RTLD_NODELETE`                        | Never unload                                            |
| `first`               | Apple    | `RTLD_FIRST`                           | Search only this library for symbols                    |
| `ignore_authz`        | Windows  | `LOAD_IGNORE_CODE_AUTHZ_LEVEL`         | Do not check software restriction policies              |
| `datafile`            | Windows  | `LOAD_LIBRARY_AS_DATAFILE`             | Map address space as data, do not execute               |
| `datafile_exclusive`  | Windows  | `LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE`   | Map address space as data, with exclusive write access  |
| `image_resource`      | Windows  | `LOAD_LIBRARY_AS_IMAGE_RESOURCE`       | Map address space as image, do not execute              |
| `search_application`  | Windows  | `LOAD_LIBRARY_SEARCH_APPLICATION_DIR`  | Search application directory only                       |
| `search_default`      | Windows  | `LOAD_LIBRARY_SEARCH_DEFAULT_DIRS`     | Search application, system, and user-added directories  |
| `search_dll`          | Windows  | `LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR`     | Search DLL directory for its dependencies               |
| `search_system32`     | Windows  | `LOAD_LIBRARY_SEARCH_SYSTEM32`         | Search system directory only                            |
| `search_user`         | Windows  | `LOAD_LIBRARY_SEARCH_USER_DIRS`        | Search user-added directories only                      |
| `altered_search`      | Windows  | `LOAD_WITH_ALTERED_SEARCH_PATH`        | Use alternative standard search path                    |

Flags passed to the constructor and related functions, controlling the way the
library is loaded. Flags not relevant to the target system will be ignored.

```c++
static constexpr Dso::flag_type Dso::default_flags = Dso::global | Dso::now;
```

The default flags.

```c++
Dso::Dso() noexcept;
explicit Dso::Dso(const Path& file, flag_type flags = default_flags);
Dso::~Dso() noexcept;
Dso::Dso(Dso&&) noexcept;
Dso& Dso::operator=(Dso&&) noexcept;
```

Life cycle functions. A `Dso` object is movable but not copyable.

The default constructor does not load anything; calling `symbol()` on a
default constructed `Dso` will always fail. The second constructor attempts
to load the named library. The search strategy depends on the format of the
file name, and may vary slightly between systems; it may also be modified by
some of the flags.

| File name format  | Search strategy                                   |
| ----------------  | ---------------                                   |
| Empty string      | The executable for the calling process is loaded  |
| Unqualified name  | The standard dynamic library search path is used  |
| Absolute path     | Only that exact file is checked                   |
| Relative path     | Behaviour is implementation defined               |

The constructor will throw `std::system_error` if the library is not found,.

```c++
explicit Dso::operator bool() const noexcept;
```

True if the object holds a handle on a valid library. This is false for
default constructed and moved-from objects.

```c++
Path Dso::file() const;
```

Returns the file name, as supplied to the constructor. If the name passed to
the constructor was not an absolute path, this does not attempt to determine
the actual path to the loaded library.

```c++
void* Dso::native_handle() const noexcept;
```

Returns the native operating system handle on the library. On Unix this is a
void pointer that can be passed to `dlsym()` and related functions. On Windows
this can be cast to an `HMODULE`. This will be null if `bool(*this)` is false.

```c++
template <typename Sym> Sym Dso::symbol(const std::string& name);
template <typename Sym>
    bool Dso::symbol(const std::string& name, Sym& sym) noexcept;
```

Load a symbol from the library. The first version takes a symbol type
explicitly, and returns the symbol, or throws `std::system_error` if the
symbol is not found. The second version returns the symbol by reference,
returning a flag indicating whether or not the symbol was successfully
loaded, and does not throw anything.

It is up to the caller to ensure that the symbol type supplied is compatible
with that of the actual symbol in the library; there is no way to check this
through the standard dynamic library API on any supported system. If the
actual type is a function pointer (the usual case), the requested symbol type
may be a compatible `std::function` instantiation instead of a plain function
pointer. Behaviour is undefined if the types do not match.

```c++
static Dso Dso::search(const std::vector<std::string>& names,
    flag_type flags = 0);
template <typename... Args> static Dso Dso::search(Args... args);
```

Search for a library that may have any of several names. Any number of names
may be passed, either as variadic arguments compatible with string views, or
a vector of strings; either can optionally be followed by a bitmask of flags.
If a name is an unqualified file name, in addition to the exact name
supplied, it will also be tried with the standard dynamic library suffix for
the target system (`".dll"`, `".dylib"`, or `".so"`), and with or without a
`"lib"` prefix. A `std::system_error` exception will be thrown if none of
these work.

```c++
static Dso Dso::self(flag_type flags = default_flags);
```

Obtains a handle on the executable of the current process (equivalent to
`Dso("")`).
