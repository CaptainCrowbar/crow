# Table Layout

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/table.hpp"
namespace Crow;
```

This header defines a class for laying out tables in plain text or Markdown.

## Table layout class

```c++
class Table: public Formatted;
```

Table formatting class.

```c++
Table::Table();
Table::Table(const Table& t);
Table::Table(Table&& t) noexcept;
Table::~Table() noexcept override;
Table& Table::operator=(const Table& t);
Table& Table::operator=(Table&& t) noexcept;
```

Life cycle functions.

```c++
template <typename T> Table& Table::operator<<(const T& t);
```

Adds one cell to the current row of the table. If a set of formatting specs
has been supplied, the cell will be formatted according to its column's spec,
or default formatted if there is no format spec for this column.

If the argument is the single character `'\n'`, the current line will be
terminated.

This will throw `std::invalid_argument` if the cell is a string containing
invalid UTF-8.

```c++
template <typename... Args> Table& Table::add(const Args&... args);
template <typename T> Table& Table::add_v(const std::vector<T>& args);
```

Adds a line of cells to the table. If a set of formatting specs has been
supplied, each cell will be formatted according to its column's spec (if there
are more cells in a row than the format set, the extra cells are default
formatted). Rows need not all have the same length.

This will throw `std::invalid_argument` if any cell contains invalid UTF-8.

```c++
template <typename... Args> Table& Table::format(const Args&... args);
template <typename T> Table& Table::format_v(const std::vector<T>& args);
```

Sets up an array of formatting specs for the table's columns. The size of the
format set need not match the actual number of columns in the table. If this
is called multiple times, a new format set replaces the old one.

```c++
template <typename... Args> Table& Table::header(const Args&... args);
template <typename T> Table& Table::header_v(const std::vector<T>& args);
```

Sets the headers for the table's columns. If there are fewer headers supplied
than columns in the longest row, the extra columns will have blank headers.
Headers may have line breaks in them, producing multiple header lines. If
this is called multiple times, only the last set of headers will actually be
used.

This will throw `std::invalid_argument` if any header contains invalid UTF-8.

```c++
bool Table::has_header() const noexcept;
```

True if a header row is present.

```c++
size_t Table::columns() const noexcept;
size_t Table::rows() const noexcept;
```

The number of columns (in the longest row, including the header row if
present) or rows (not counting the headers) in the table.

```c++
std::string Table::str(const FormatSpec& spec = {}) const override;
```

Formats the table as a multiline string. Formatting modes:

* `T` - Plain text (default)
* `B` - Box art
* `M` - Markdown

Cell widths are measured, for alignment purposes, using `utf_width()`. In
plain text mode, columns are separated with two spaces, and empty cells are
represented by `"--"` (`".."` in headers). If headers are present, they are
separated from the data cells by a row of `"="` signs in each column.

This will throw `std::invalid_argument` if the format spec is invalid.

Example:

```c++
Table tab;
tab.header("X-ray\n°C", "Yankee", "Zulu");
tab.format("", "n8", "e2");
tab.add("Alpha", "Bravo", "Charlie");
tab.add(123, 456, 789);
tab.add("Something");
```

Result in plain text:

```
X-ray      Yankee    Zulu
°C         ..        ..
=========  ========  =======
Alpha      Bravo     Charlie
123        00000456  7.9e2
Something  --        --
```

Result in Markdown:

```
| X-ray<br>°C  | Yankee    | Zulu     |
| -----------  | --------  | -------  |
| Alpha        | Bravo     | Charlie  |
| 123          | 00000456  | 7.9e2    |
| Something    |           |          |
```
