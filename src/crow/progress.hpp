#pragma once

#include "crow/terminal.hpp"
#include "crow/types.hpp"
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>

namespace Crow {

    struct ProgressColours {
        static constexpr Xterm::colour default_text  = {{5, 5, 1}};
        static constexpr Xterm::colour default_todo  = {{1, 1, 5}};
        static constexpr Xterm::colour default_done  = {{1, 5, 1}};
        static constexpr Xterm::colour default_eta   = {{5, 3, 1}};
        Xterm::colour text  = default_text;
        Xterm::colour todo  = default_todo;
        Xterm::colour done  = default_done;
        Xterm::colour eta   = default_eta;
    };

    class ProgressBar {

    public:

        explicit ProgressBar(const std::string& label, double scale = 1, size_t length = 0,
            const ProgressColours& colours = {}, std::ostream& out = std::cout);
        ~ProgressBar() noexcept;

        ProgressBar(const ProgressBar&) = delete;
        ProgressBar(ProgressBar&&) = delete;
        ProgressBar& operator=(const ProgressBar&) = delete;
        ProgressBar& operator=(ProgressBar&&) = delete;

        void update(double x);
        void operator()(double x) { update(x); }

    private:

        static constexpr size_t tail_length_ = 10;

        Xterm xterm_;
        ProgressColours colours_;
        std::chrono::system_clock::time_point start_time_;
        double scale_;
        size_t bar_length_;
        size_t bar_offset_;
        size_t current_pos_;
        std::ostream* output_;

    };

}
