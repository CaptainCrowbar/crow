#include "crow/terminal.hpp"
#include "crow/string.hpp"
#include <algorithm>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace Crow {

    // Class Xterm

    std::string Xterm::grey(int y) const {
        if (! tty_)
            return {};
        else if (y <= 0)
            return black();
        else
            return "\x1b[38;5;" + std::to_string(grey_index(y)) + "m";
    }

    std::string Xterm::grey_bg(int y) const {
        if (! tty_)
            return {};
        else if (y <= 0)
            return black();
        else
            return "\x1b[48;5;" + std::to_string(grey_index(y)) + "m";
    }

    std::string Xterm::rgb(int r, int g, int b) const {
        if (tty_)
            return "\x1b[38;5;" + std::to_string(rgb_index(r, g, b)) + "m";
        else
            return {};
    }

    std::string Xterm::rgb_bg(int r, int g, int b) const {
        if (tty_)
            return "\x1b[48;5;" + std::to_string(rgb_index(r, g, b)) + "m";
        else
            return {};
    }

    int Xterm::grey_index(int y) noexcept {
        return std::clamp(y, 1, 24) + 231;
    }

    int Xterm::rgb_index(int r, int g, int b) noexcept {
        r = std::clamp(r, 0, 5);
        g = std::clamp(g, 0, 5);
        b = std::clamp(b, 0, 5);
        return 36 * r + 6 * g + b + 16;
    }

    bool Xterm::is_tty(int fd) noexcept {
        #ifdef _WIN32
            return _isatty(fd) != 0;
        #else
            return isatty(fd) != 0;
        #endif
    }

    bool Xterm::is_tty(FILE* fp) noexcept {
        int fd =
            #ifdef _WIN32
                _fileno(fp);
            #else
                fileno(fp);
            #endif
        return is_tty(fd);
    }

}
