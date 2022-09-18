# Progress Bar

_[Crow Library by Ross Smith](index.html)_

```c++
#include "crow/progress.hpp"
namespace Crow;
```

## Progress bar

```c++
struct ProgressColours {
    static constexpr Xterm::colour default_text  = {{5, 5, 1}};  // Yellow
    static constexpr Xterm::colour default_todo  = {{1, 1, 5}};  // Blue
    static constexpr Xterm::colour default_done  = {{1, 5, 1}};  // Green
    static constexpr Xterm::colour default_eta   = {{5, 3, 1}};  // Orange
    Xterm::colour text  = default_text;  // Prefix text on the left
    Xterm::colour todo  = default_todo;  // Uncompleted part of the bar
    Xterm::colour done  = default_done;  // Completed part of the bar
    Xterm::colour eta   = default_eta;   // ETA text on the right
};
```

Colours used for the progress bar.

```c++
class ProgressBar {
    explicit ProgressBar(const std::string& label, double scale = 1,
        size_t length = 0, const ProgressColours& colours = {},
        std::ostream& out = std::cout);
    ~ProgressBar() noexcept;
    void operator()(double x);
};
```

This class draws a progress bar on the terminal, and updates it every time the
function call operator is called.

Constructor arguments are the label to print on the left of the progress bar,
the value corresponding to the full length of the bar (defaulting to 1), the
length of the bar in character cells, and the output stream to write it to
(standard output by default). If the length is zero, the terminal width will
be used (assumed to be 80 characters if the value is not available). If an
explicit length is supplied, it is trusted to work with the available
terminal dimensions; it is up to the caller to ensure that the resulting
output will not be too long (allow for at least 16 character cells in
addition to the label and bar lengths).

The constructor will throw `std::invalid_argument` if `scale<=0`.

The function call operator updates the progress bar to reflect the value
passed to it, and adds an estimated remaining time on the right. The value is
clamped to the unit range. Progress will never go backwards; a call with a
value less than the previous value will not update the bar, although it will
be taken into account in estimating the run time.
