#pragma once

#include "crow/types.hpp"
#include <array>
#include <string>

namespace Crow {

    class Xterm {

    public:

        using colour = std::array<int, 3>;

        Xterm() noexcept: tty_(is_tty(1)) {}
        explicit Xterm(bool tty) noexcept: tty_(tty) {}

        bool tty() const noexcept { return tty_; }

        std::string up() const                 { if (! tty_) return {}; static const std::string s = "\x1b[A"; return s; }
        std::string down() const               { if (! tty_) return {}; static const std::string s = "\x1b[B"; return s; }
        std::string right() const              { if (! tty_) return {}; static const std::string s = "\x1b[C"; return s; }
        std::string left() const               { if (! tty_) return {}; static const std::string s = "\x1b[D"; return s; }
        std::string move_up(int n) const       { if (! tty_) return {}; return "\x1b[" + std::to_string(n) + 'A'; }
        std::string move_down(int n) const     { if (! tty_) return {}; return "\x1b[" + std::to_string(n) + 'B'; }
        std::string move_right(int n) const    { if (! tty_) return {}; return "\x1b[" + std::to_string(n) + 'C'; }
        std::string move_left(int n) const     { if (! tty_) return {}; return "\x1b[" + std::to_string(n) + 'D'; }
        std::string beginning_of_line() const  { if (! tty_) return {}; static const std::string s = "\x1b[G"; return s; }
        std::string erase_above() const        { if (! tty_) return {}; static const std::string s = "\x1b[1J"; return s; }
        std::string erase_below() const        { if (! tty_) return {}; static const std::string s = "\x1b[J"; return s; }
        std::string erase_right() const        { if (! tty_) return {}; static const std::string s = "\x1b[K"; return s; }
        std::string erase_left() const         { if (! tty_) return {}; static const std::string s = "\x1b[1K"; return s; }
        std::string erase_line() const         { if (! tty_) return {}; static const std::string s = "\x1b[2K"; return s; }
        std::string clear() const              { if (! tty_) return {}; static const std::string s = "\x1b[2J"; return s; }
        std::string reset() const              { if (! tty_) return {}; static const std::string s = "\x1b[0m"; return s; }
        std::string bold() const               { if (! tty_) return {}; static const std::string s = "\x1b[1m"; return s; }
        std::string under() const              { if (! tty_) return {}; static const std::string s = "\x1b[4m"; return s; }
        std::string bold_off() const           { if (! tty_) return {}; static const std::string s = "\x1b[22m"; return s; }
        std::string under_off() const          { if (! tty_) return {}; static const std::string s = "\x1b[24m"; return s; }
        std::string colour_off() const         { if (! tty_) return {}; static const std::string s = "\x1b[39m"; return s; }
        std::string black() const              { if (! tty_) return {}; static const std::string s = "\x1b[30m"; return s; }
        std::string red() const                { if (! tty_) return {}; static const std::string s = "\x1b[31m"; return s; }
        std::string green() const              { if (! tty_) return {}; static const std::string s = "\x1b[32m"; return s; }
        std::string yellow() const             { if (! tty_) return {}; static const std::string s = "\x1b[33m"; return s; }
        std::string blue() const               { if (! tty_) return {}; static const std::string s = "\x1b[34m"; return s; }
        std::string magenta() const            { if (! tty_) return {}; static const std::string s = "\x1b[35m"; return s; }
        std::string cyan() const               { if (! tty_) return {}; static const std::string s = "\x1b[36m"; return s; }
        std::string white() const              { if (! tty_) return {}; static const std::string s = "\x1b[37m"; return s; }
        std::string black_bg() const           { if (! tty_) return {}; static const std::string s = "\x1b[40m"; return s; }
        std::string red_bg() const             { if (! tty_) return {}; static const std::string s = "\x1b[41m"; return s; }
        std::string green_bg() const           { if (! tty_) return {}; static const std::string s = "\x1b[42m"; return s; }
        std::string yellow_bg() const          { if (! tty_) return {}; static const std::string s = "\x1b[43m"; return s; }
        std::string blue_bg() const            { if (! tty_) return {}; static const std::string s = "\x1b[44m"; return s; }
        std::string magenta_bg() const         { if (! tty_) return {}; static const std::string s = "\x1b[45m"; return s; }
        std::string cyan_bg() const            { if (! tty_) return {}; static const std::string s = "\x1b[46m"; return s; }
        std::string white_bg() const           { if (! tty_) return {}; static const std::string s = "\x1b[47m"; return s; }

        std::string grey(int y) const;
        std::string grey_bg(int y) const;
        std::string rgb(int r, int g, int b) const;
        std::string rgb_bg(int r, int g, int b) const;
        std::string rgb(colour c) const { return rgb(c[0], c[1], c[2]); }
        std::string rgb_bg(colour c) const { return rgb_bg(c[0], c[1], c[2]); }

        static bool is_tty(int fd) noexcept;
        static bool is_tty(FILE* fp) noexcept;

    private:

        bool tty_;

        static int grey_index(int y) noexcept;
        static int rgb_index(int r, int g, int b) noexcept;

    };

}
