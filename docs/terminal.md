# Terminal Control

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/terminal.hpp"
namespace Crow;
```

This header defines some utilities for ANSI/Xterm terminal control.

## Terminal control class

```c++
class Xterm;
```

Container for terminal control functions.

```c++
using Xterm::colour = std::array<int, 3>;
```

Used for setting RGB colours.

```c++
Xterm::Xterm() noexcept;
explicit Xterm::Xterm(bool tty) noexcept;
```

Constructors. Optionally, a flag can be set to indicate whether the output is
going directly to a terminal emulator or redirected to a file. If the `tty`
flag is true, terminal control codes will be generated; if not, all of the
terminal control functions will return empty strings. By default, the `tty`
flag is determined by calling `is_tty(stdout)`.

```c++
Xterm::Xterm(const Xterm& xt);
Xterm::Xterm(Xterm&& xt) noexcept;
Xterm::~Xterm() noexcept;
Xterm& Xterm::operator=(const Xterm& xt);
Xterm& Xterm::operator=(Xterm&& xt) noexcept;
```

Other life cycle functions.

```c++
bool Xterm::tty() const noexcept;
```

Returns the `tty` flag.

```c++
std::string Xterm::up() const;                 // <esc>[A
std::string Xterm::down() const;               // <esc>[B
std::string Xterm::right() const;              // <esc>[C
std::string Xterm::left() const;               // <esc>[D
std::string Xterm::move_up(int n) const;       // <esc>[<n>A
std::string Xterm::move_down(int n) const;     // <esc>[<n>B
std::string Xterm::move_right(int n) const;    // <esc>[<n>C
std::string Xterm::move_left(int n) const;     // <esc>[<n>D
std::string Xterm::beginning_of_line() const;  // <esc>[G
std::string Xterm::erase_above() const;        // <esc>[1J
std::string Xterm::erase_below() const;        // <esc>[J
std::string Xterm::erase_right() const;        // <esc>[K
std::string Xterm::erase_left() const;         // <esc>[1K
std::string Xterm::erase_line() const;         // <esc>[2K
std::string Xterm::clear() const;              // <esc>[2J
```

Cursor movement and related control sequences.

```c++
std::string Xterm::reset() const;       // <esc>[0m
std::string Xterm::bold() const;        // <esc>[1m
std::string Xterm::under() const;       // <esc>[4m
std::string Xterm::bold_off() const;    // <esc>[22m
std::string Xterm::under_off() const;   // <esc>[24m
std::string Xterm::colour_off() const;  // <esc>[39m
std::string Xterm::black() const;       // <esc>[30m
std::string Xterm::red() const;         // <esc>[31m
std::string Xterm::green() const;       // <esc>[32m
std::string Xterm::yellow() const;      // <esc>[33m
std::string Xterm::blue() const;        // <esc>[34m
std::string Xterm::magenta() const;     // <esc>[35m
std::string Xterm::cyan() const;        // <esc>[36m
std::string Xterm::white() const;       // <esc>[37m
std::string Xterm::black_bg() const;    // <esc>[40m
std::string Xterm::red_bg() const;      // <esc>[41m
std::string Xterm::green_bg() const;    // <esc>[42m
std::string Xterm::yellow_bg() const;   // <esc>[43m
std::string Xterm::blue_bg() const;     // <esc>[44m
std::string Xterm::magenta_bg() const;  // <esc>[45m
std::string Xterm::cyan_bg() const;     // <esc>[46m
std::string Xterm::white_bg() const;    // <esc>[47m
```

Colour and font control sequences.

```c++
std::string Xterm::grey(int y) const;     // <esc>[38;5;<n>m
std::string Xterm::grey_bg(int y) const;  // <esc>[48;5;<n>m
```

Greyscale foreground and background colour code generator functions. The
brightness argument is clamped to the range 0-24.

```c++
std::string Xterm::rgb(int r, int g, int b) const;     // <esc>[38;5;<n>m
std::string Xterm::rgb(colour c) const;                // <esc>[38;5;<n>m
std::string Xterm::rgb_bg(int r, int g, int b) const;  // <esc>[48;5;<n>m
std::string Xterm::rgb_bg(colour c) const;             // <esc>[48;5;<n>m
```

RGB foreground and background colour code generator functions. Each channel is
clamped to the range 0-5.

```c++
static bool Xterm::is_tty(int fd) noexcept;
static bool Xterm::is_tty(FILE* fp) noexcept;
```

Check whether an output stream is going to a terminal or is redirected. These
call `isatty()` or the local equivalent.
