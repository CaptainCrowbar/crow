# File System Path

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/path.hpp"
namespace Crow;
```

## Contents

* TOC
{:toc}

## Path class

```c++
class Path;
```

The `Path` class holds a file system path as a string. Internally, the file
name is stored as a `std::string` on Unix, or a `std::wstring` on Windows,
but `Path` objects on any system can be constructed from a UTF-8 string. The
class provides a set of file system operations, such as file renaming and
deletion, directory search, and so on.

File handling in Unicode always has the problem that the actual file names on
most systems are not guaranteed to be valid Unicode. On most Unix systems, a
file name is an arbitrary sequence of bytes, with no restrictions other than
excluding the null and slash characters; while file names on systems such as
Linux are usually expected to be encoded in UTF-8 when they contain non-ASCII
characters, this is not enforced in any way. On Microsoft Windows, using the
NTFS file system, file names are nominally UTF-16 (with a short list of
disallowed characters), but not all of the wide character API functions that
deal with files check for valid encoding; it's not hard to create a file whose
name is an arbitrary sequence of 16-bit integers. The HFS+ file system
formerly used by Apple on macOS and iOS appears to be the only widely used
file system that actually does enforce valid Unicode names at the file system
level, although it then proceeds to complicate matters by using a proprietary
normalization scheme that does not match any of the four standard ones, and in
any case the new APFS file system does not appear to enforce any kind of
encoding.

To deal with this situation, the default behaviour of `Path` when constructed
from a native file name string (`std::string` on Unix, `std::wstring` on
Windows) is to copy the string verbatim, without checking for or correcting
any UTF encoding. When converting from UTF-8 to UTF-16, an exception will be
thrown if the name is not valid UTF-8.

The examples in the documentation below mostly use Unix-style file names for
simplicity; the equivalent code on Windows will make the obvious substitutions
of backslashes for slashes, and drive roots such as `"C:\"` for `"/"`.

The term "leaf name" is used here to mean the name of an individual file
within a directory, with no directory path prefix (for example, the leaf name
of `"/foo/bar/hello.txt"`, or `"C:\foo\bar\hello.txt"`, is `"hello.txt"`).

Exceptions will be thrown under the following circumstances:

* On Windows, any function that accepts a file name as a `std::string` will
  throw `std::invalid_argument` if the string is not valid UTF-8. This does
  not apply to `std::wstring` arguments, which can accept invalid UTF-16.
* Any function not marked `noexcept` may throw `std::bad_alloc` if memory
  allocation fails.
* Exception behaviour is only documented for individual functions where it
  varies from the above rules.

## Member types

```c++
using Path::character_type = [char on Unix, wchar_t on Windows];
using Path::string_type = std::basic_string<character_type>;
```

Native OS string types.

```c++
class Path::directory_iterator;
class Path::search_iterator;
typename Path::directory_range;
typename Path::search_range;
```

Directory search iterators, and range types containing a pair of iterators,
returned by the directory search functions.

```c++
using Path::id_type = std::pair<uint64_t, uint64_t>;
using Path::time_point = std::chrono::system_clock::time_point;
```

Other member types.

```c++
enum class Path::cmp: int {
    cased,
    icase,
    native
};
class Path::equal {
    equal() noexcept;
    explicit equal(cmp mode) noexcept;
    bool operator()(const Path& lhs, const Path& rhs) const;
};
class Path::less {
    less() noexcept;
    explicit less(cmp mode) noexcept;
    bool operator()(const Path& lhs, const Path& rhs) const;
};
```

Path comparison objects. These have equality and less-than semantics; the
constructor argument indicates whether to do a case insensitive comparison.
`Path::cmp::native` is the default, and will be equal to one of the other two
options, reflecting the native OS convention.

Case insensitivity here applies only to ASCII characters; the semantics of
Unicode case sensitivity are too complicated to emulate here,

```c++
enum class Path::flag_type: int {
    no_flags = 0,
    append,        // If the file already exists, append to it instead of overwriting
    bottom_up,     // Search a directory tree in bottom up order instead of top down
    legal_name,    // Fail if the file name is illegal for the operating system
    may_copy,      // Fall back on copying files if the original operation is not possible
    may_fail,      // Return a default value instead of throwing if the operation fails
    no_backtrack,  // Don't search upward through directories
    no_follow,     // Don't follow symlinks
    no_hidden,     // Ignore hidden files
    overwrite,     // Replace the file if it already exists
    recurse,       // Perform directory operations recursively
    stdio,         // If the path is empty or "-", use standard input or output instead
    unicode,       // Ignore files whose names are not valid UTF
};
using enum Path::flag_type;
```

Bitmask flags controlling the behaviour of some of the path functions. More
details of the resulting behaviour are described with the relevant functions.
As a general rule, flags that are not relevant to a specific function are
ignored.

```c++
enum class Path::form: int {
    empty,
    absolute,
    drive_absolute,
    drive_relative,
    relative
};
using enum Path::form;
std::ostream& operator<<(std::ostream& out, Path::form f);
```

These are the possible formats for a path; any path will have one of these
forms, which can be queried using `path_form()` and related functions such as
`is_absolute()`.

The concepts of a "drive absolute" path (e.g. `"\foo"` with no leading drive
letter), or a "drive relative" path (e.g. `"C:foo"` with no backslash after
the drive letter), are specific to Windows; on Unix, `path_form()` will never
return one of those values. and `is_drive_absolute()` and
`is_drive_relative()` are always false.

## Constants

```c++
static constexpr char Path::delimiter = ['/' on Unix, '\\' on Windows];
static constexpr Path::character_type Path::native_delimiter;
```

The native path delimiter character.

```c++
static constexpr bool Path::native_case;
```

Indicates whether path names are case sensitive by default on the host OS.

## Life cycle functions

```c++
Path::Path() noexcept;
```

Default constructor, equivalent to constructing from an empty string.

```c++
Path::Path(const std::string& file, flag_type flags = no_flags);
Path::Path(const char* file, flag_type flags = no_flags);
Path::Path(const std::wstring& file, flag_type flags = no_flags);
Path::Path(const wchar_t* file, flag_type flags = no_flags);
```

Constructors (and implicit conversions) from a string.
The constructors that take a wide string are only defined on Windows.

If the argument is a native string (`std::string` on Unix, `std::wstring` on
Windows), the name will simply be copied verbatim with no UTF encoding
checks. On Windows, the constructor from an 8-bit string will throw
`std::invalid_argument` if the name is not valid UTF-8.

If the `legal_name` flag is used, the constructor will throw
`std::invalid_argument` if a file name whose format is illegal for the
operating system is supplied. Note that this will not reject a name solely
because it contains invalid UTF, since this is legal on most systems.

On construction, paths are brought into a canonical form according to the
following rules:

* On Windows, replace any slash delimiters with backslashes
* Trim redundant leading slashes
* Replace `/./` with `/` throughout
* Replace redundant multiple slashes with one
* Trim trailing `/` and `/.`
* On Windows, ensure a trailing slash on network paths
* On Windows, convert the drive letter to upper case

```c++
Path::~Path() noexcept;
Path::Path(const Path& p);
Path::Path(Path&& p) noexcept;
Path& Path::operator=(const Path& p);
Path& Path::operator=(Path&& p) noexcept;
```

Other life cycle functions.

## Path name functions

These operate on the path purely as a string, making no contact with the
actual file system.

```c++
std::string Path::name() const;
std::string Path::str() const;
std::ostream& operator<<(std::ostream& out, const Path& p);
```

These return the full path as UTF-8. On Windows, they will throw
`std::invalid_argument` if the actual file name contains invalid UTF-16.

```c++
Path::string_type Path::os_name() const;
const Path::character_type* Path::c_name() const noexcept;
```

These return the full path in its native form, with no conversion.

```c++
std::string Path::as_url() const;
```

Returns the file name as a UTF-8 `"file://..."` URL. This will throw
`std::invalid_argument` if the path is not absolute, or if the file name
contains invalid UTF.

```c++
std::vector<std::string> Path::breakdown() const;
std::vector<string_type> Path::os_breakdown() const;
```

Break the path down into its directory and file elements. If the path is
absolute, the first element will be the root path.

```c++
Path Path::change_ext(const std::string& new_ext) const;
```

Replace the file extension with a new one. The new extension can be supplied
with or without a leading dot, or can be an empty string to remove the
extension. This will throw `std::invalid_argument` if the path is empty or is
a root path, or (on Windows) if the new extension contains invalid UTF-8.

```c++
bool Path::is_empty() const noexcept;
bool Path::is_absolute() const noexcept;
bool Path::is_relative() const noexcept;
bool Path::is_drive_absolute() const noexcept;
bool Path::is_drive_relative() const noexcept;
Path::form Path::path_form() const noexcept;
```

Query which syntactic form the path has. Exactly one of the first five
functions will always be true for any path. On Unix, `is_drive_absolute()` and
`is_drive_relative()` will always be false.

```c++
size_t Path::hash() const noexcept;
struct std::hash<Path>;
```

Hash of the file name. This returns the same value as `std::hash` of the
`os_name()`.

```c++
bool Path::is_leaf() const noexcept;
```

True if the path is a leaf name (one with no directory prefix).

```c++
bool Path::is_legal() const noexcept;
```

True if the path is legal for this operating system. This does not check for
valid UTF, only for the OS-specific file naming rules.

```c++
bool Path::is_root() const noexcept;
```

True if the path is the root of a logical file system. This is a purely
syntactic check and does not attempt to check against actual mount points. On
Unix this is true only for `"/"` and no other paths. On Windows, this checks
for a drive letter (e.g. `"C:\"`) or a network path (e.g. `"\\server\"`).

```c++
bool Path::is_unicode() const noexcept;
```

True if the name is valid UTF.

```c++
Path Path::relative_to(const Path& base, flag_type flags = no_flags) const;
```

Returns a relative path locating the current path relative to the given base
path (which is assumed, without checking, to be a directory). The current and
base paths must be either both absolute or both relative; if they are
relative, they are assumed to be relative to the same root. If the
`no_backtrack` flag is set, paths that are not within the base directory will
be returned as absolute paths, instead of using `".."` elements to backtrack
up the directory tree.

On Windows, if the two paths are absolute and have different drive prefixes,
the original absolute path will be returned unchanged, regardless of the
`no_backtrack` flag.

This will throw `std::invalid_argument` if one of the two paths is absolute
and the other relative, or if both paths are relative, the current path is
not below the base directory, and the `no_backtrack` flag is set.

```c++
std::pair<std::string, std::string> Path::split_leaf() const;
std::pair<Path::string_type, Path::string_type> Path::split_os_leaf() const;
```

Splits the path's leaf name into a base and extension; the extension begins
with the last dot, provided this is not the first or last character of the
leaf name. The part of the path before the leaf name (i.e. up to the last
path delimiter) is not included in the return value. If the path is a root
path, both strings will be empty. For example, `"/foo/bar/hello.txt"` will
return `{"hello",".txt"}`.

```c++
std::pair<Path, Path> Path::split_path() const;
```

Splits the path into a directory path and a leaf name. If the path is a root
path, the leaf name will be empty. For example, `"/foo/bar/hello.txt"` will
return `{"/foo/bar","hello.txt"}`.

```c++
std::pair<Path, Path> Path::split_root() const;
```

Splits the path into a root path and a path relative to the root. For example,
`"/foo/bar/hello.txt"` will return `{"/","foo/bar/hello.txt"}`.

```c++
static Path Path::common(const Path& lhs, const Path& rhs);
template <typename Range> static Path Path::common(const Range& files);
```

Return the longest common prefix of two or more paths. The return value will
be a complete path (i.e. it will not split a path element in half if two path
elements happen to share a substring). The returned path will be empty if the
argument paths are a mixture of absolute and relative paths, or (on Windows)
if they are absolute but do not all have the same root. For example,
`common("/a/b/c/d/e/f","/a/b/c/x/y/z")` will return `"/a/b/c"`.

```c++
static Path Path::join(const Path& lhs, const Path& rhs);
template <typename Range> static Path Path::join(const Range& files);
Path Path::operator/(const Path& lhs, const Path& rhs);
Path& Path::operator/=(Path& lhs, const Path& rhs);
```

Join two paths. This will discard the LHS and return the RHS unchanged if the
LHS is empty or the RHS is absolute; otherwise, the resulting path is obtained
by assuming the RHS is relative to the LHS.

## Comparison operators

```c++
std::strong_ordering operator<=>(const Path& lhs, const Path& rhs) noexcept;
bool operator==(const Path& lhs, const Path& rhs) noexcept;
bool operator!=(const Path& lhs, const Path& rhs) noexcept;
bool operator<(const Path& lhs, const Path& rhs) noexcept;
bool operator>(const Path& lhs, const Path& rhs) noexcept;
bool operator<=(const Path& lhs, const Path& rhs) noexcept;
bool operator>=(const Path& lhs, const Path& rhs) noexcept;
```

Comparison operators. These perform lexicographical comparison by code unit,
not by code point, because the path string may not be a valid encoding. On
Windows this means that paths that contain characters outside the BMP will
not necessarily sort in Unicode order.

These operators always perform case sensitive comparison regardless of the
operating system. You can use the `Path::equal` and `Path::less` function
objects if you need case insensitive comparison, but please note that even
with these, no attempt is made to exactly emulate the native file name
sorting conventions.

## File system query functions

These require read-only access to the file system. Any function not marked
`noexcept` can throw `std::system_error` if the underlying system API fails,
in addition to any other documented exceptions.

```c++
Path::time_point Path::access_time(flag_type flags = no_flags) const noexcept;
Path::time_point Path::create_time(flag_type flags = no_flags) const noexcept;
Path::time_point Path::modify_time(flag_type flags = no_flags) const noexcept;
Path::time_point Path::status_time(flag_type flags = no_flags) const noexcept;
```

Query the file's time metadata; see also the corresponding update functions
below. The four possible time properties are detailed below, along with which
operations are possible on each operating system. The query functions will
return the epoch if the file does not exist or is not accessible. If the
`no_follow` flag is set, and the path refers to a symlink, these will
operate (if possible) on the symlink rather than the target file.

| Property     | Interpretation              | Posix    | Linux          | Mac         | Windows        |
| --------     | --------------              | -----    | ----------     | ---         | -------        |
| Access time  | File was last read          | `atime`  | Read/write     | Read/write  | Read/write     |
| Create time  | File was created            |          | Not supported  | Read only   | Read/write     |
| Modify time  | File was last modified      | `mtime`  | Read/write     | Read/write  | Read/write     |
| Status time  | Metadata was last modified  | `ctime`  | Read only      | Read only   | Not supported  |

```c++
Path::directory_range Path::directory(flag_type flags = no_flags) const;
Path::search_range Path::deep_search(flag_type flags = no_flags) const;
```

Return an iterator range over the files within a directory, either the
immediate directory members (`directory()`) or a recursive search of all
child directories (`deep_search()`).

The range will be empty if the path does not exist or is not a directory. The
order in which files are returned is unspecified. If the directory's contents
are changed while iteration is in progress, it is unspecified whether the
changes will be visible to the caller. If the `no_hidden` flag is set, hidden
files will not be included. If the `unicode` flag is set, files whose names
are not valid UTF will not be included (this test is applied to the whole
path, so a directory whose own name is invalid will appear to be empty).

By default, `deep_search()` visits files in top down order (a directory is
seen before its contents); if the `bottom_up` flag is set, bottom up order is
used instead (a directory's contents are seen before the directory itself).
Deep search iterators otherwise take the same flags, and follow the same
rules, as directory iterator.

```c++
bool Path::exists(flag_type flags = no_flags) const noexcept;
```

Query whether a file exists. This may give a false negative if the file exists
but is not accessible to the calling process. The `no_follow` flag prevents
this function from following symbolic links.

```c++
Path::id_type Path::id(flag_type flags = no_flags) const noexcept;
```

Returns a unique file identifier, intended to identify the file even if it is
referred to by different paths. It will return a null ID if the file does not
exist or the caller does not have permission to query its properties. This is
based on the file's device and inode numbers (or the equivalent on Windows),
but completely reliable file identification cannot be guaranteed in the
presence of shenanigans like parallel NFS mounts. The `no_follow` flag
prevents this function from following symbolic links.

```c++
bool Path::is_directory(flag_type flags = no_flags) const noexcept;
bool Path::is_file(flag_type flags = no_flags) const noexcept;
bool Path::is_special(flag_type flags = no_flags) const noexcept;
```

Query the file type. Exactly one of these will be true if `exists()` is true.
These may give a false negative if the file exists but is not accessible to
the calling process. The `no_follow` flag prevents this function from
following symbolic links.

```c++
bool Path::is_hidden() const noexcept;
```

True if the file is normally hidden. On Unix this is based on the file name (a
file is hidden if its name starts with a dot), and the file's existence is not
checked; on Windows this is a metadata property, and will return false if the
file does not exist or is not accessible to the caller.

```c++
bool Path::is_symlink() const noexcept;
```

True if the file is a symbolic link. I haven't tried to support the
complicated symlink-like concepts on NTFS; this simply always returns false
on Windows.

```c++
Path Path::resolve() const;
```

This resolves a relative path to an absolute one. If the path is empty, this
will return the current directory. On Unix, if the path is absolute, it will
be returned unchanged, but on Windows any short filenames in the original
path will be resolved to long names even if the path was absolute.

```c++
Path Path::resolve_symlink() const;
```

Returns the file pointed to by a symlink. This will just return the original
path unchanged if it is not a symlink (on Windows this will always happen).

```c++
uint64_t Path::size(flag_type flags = no_flags) const;
```

Returns the size of a file in bytes. If the `no_follow` flag is set, and the
path refers to a symlink, this will return the size of the symlink itself
instead of the target file. If the `recurse` flag is set, and the path refers
to a directory, this will recursively determine the total size of the
directory and everything in it (symlinks below the outermost directory will
never be followed regardless of the flags).

## File system update functions

These require write access to the file system. Any of these functions can
throw `std::system_error` if the underlying system API fails, in addition to
any other documented exceptions.

```c++
void Path::copy_to(const Path& dst, flag_type flags = no_flags) const;
```

Copy a file from the current path to the destination path. If the `overwrite`
flag is set, this will replace any existing file at the destination. If the
`recurse` flag is set, and the path refers to a directory, it will be copied
recursively.

This will throw `std::system_error` if the source file does not exist, if the
source and destination are the same file, if the destination already exists
and `overwrite` was not set, or if the source is a directory and `recurse`
was not set.

```c++
void Path::create() const;
```

If the file does not exist, an empty file with default permissions is
created.

```c++
void Path::make_directory(flag_type flags = no_flags) const;
```

Create a directory, with default permissions, at the current path. If the
`overwrite` flag is set, and the path exists but is not a directory, it will
be replaced. If the `recurse` flag is set, any missing parent directories
will also be created (like `mkdir -p`).

This will do nothing if the directory already exists. It will throw
`std::system_error` if the path exists but is not a directory, and the
`overwrite` flag was not set.

```c++
void Path::make_symlink(const Path& linkname, flag_type flags = no_flags) const;
```

Create a symlink at `linkname`, pointing to the current path. The existence or
validity of the target is not checked. If the `may_copy` flag is set, this
will copy the file (recursively) if a symlink can't be created. If the
`overwrite` flag is set, any existing file at `linkname` will be replaced.

This will do nothing if the link already exists and points to the correct
file. It will throw `std::system_error` if a file already exists at
`linkname`, it is not a symlink already pointing to the target path, and the
`overwrite` flag was not set. On Windows this will always fail unless the
`may_copy` flag is set.

```c++
void Path::move_to(const Path& dst, flag_type flags = no_flags) const;
```

Move a file from the current path to the destination path. If the `may_copy`
flag is set, this will attempt to copy the file and then delete the original
if the move fails (this may leave two copies of a file if the operation fails
partway through). If the `overwrite` flag is set, this will replace any
existing file at the destination.

This will do nothing if the source and destination are the same path. It will
throw `std::system_error` if the source file does not exist, or if the
destination already exists and `overwrite` was not set.

```c++
void Path::remove(flag_type flags = no_flags) const;
```

Delete a file. This will do nothing if the file already does not exist. If the
`recurse` flag is set, this will behave like like `rm -rf` and delete
directories recursively.

This will throw `std::system_error` if the path refers to a non-empty
directory and the `recurse` flag is not used, if the caller does not have
permission to remove the file, or in some circumstances, if the file is in
use by another process.

```c++
void Path::set_access_time(flag_type flags = no_flags) const;
void Path::set_access_time(Path::time_point t,
    flag_type flags = no_flags) const;
void Path::set_create_time(flag_type flags = no_flags) const;
void Path::set_create_time(Path::time_point t,
    flag_type flags = no_flags) const;
void Path::set_modify_time(flag_type flags = no_flags) const;
void Path::set_modify_time(Path::time_point t,
    flag_type flags = no_flags) const;
```

Modify the file's time metadata. Refer to the corresponding query functions
(above) for details of the interpretation of the various time properties.
There is no `set_status_time()` function because no known operating system
supports explicitly setting the file metadata modification time.

The functions that do not take a time argument will set it to the current
time. If the `no_follow` flag is set, and the path refers to a symlink, these
will operate (if possible) on the symlink rather than the target file.

These will throw `std::system_error` if the file does not exist, if the
calling process does not have permission to modify the requested time field,
or if that field is not defined, or not modifiable, on this operating
system.

## I/O functions

```c++
void Path::load(std::string& str, size_t maxlen = npos,
    flag_type flags = no_flags) const;
```

Read the contents of a file into a string (erasing its former contents).
Optionally, a maximum number of bytes can be specified. If the `may_fail` flag
is set, this will return an empty string if the file does not exist or a read
error occurs. If the `stdio` flag is set, this will read from standard input
if the path is an empty string or `"-"`.

If the `may_fail` flag is not set, this will throw `std::system_error` if the
file does not exist or an I/O error occurs.

```c++
void Path::save(const std::string& str, flag_type flags = no_flags) const;
```

Writes the contents of a string to a file. If the `append` flag is set and the
file already exists, the new data will be appended to it; otherwise, if the
`overwrite` flag is set, it will be overwritten. If the `stdio` flag is set,
this will write to standard output if the path is an empty string or `"-"`.

This will throw `std::system_error` if the file already exists and neither
`append` nor `overwrite` are set, or if an I/O error occurs. It will throw
`std::invalid_argument` if both `append` and `overwrite` are set.

## Process state functions

```c++
void Path::change_directory() const;
static Path Path::current_directory();
```

Query or set the calling process's current working directory. Remember that
the CWD concept is process-global, so neither of these functions is thread
safe. These will throw `std::system_error` if the underlying system API
fails.
