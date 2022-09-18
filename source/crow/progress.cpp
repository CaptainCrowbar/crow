#include "crow/progress.hpp"
#include "crow/format.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

namespace Crow {

    // Class ProgressBar

    ProgressBar::ProgressBar(const std::string& label, double scale, size_t length,
        const ProgressColours& colours, std::ostream& out):
    xterm_(true),
    colours_(colours),
    start_time_(),
    scale_(scale),
    bar_length_(length),
    bar_offset_(label.size() + 1),
    current_pos_(0),
    output_(&out) {

        using namespace std::chrono;

        if (scale_ <= 0)
            throw std::invalid_argument("Progress bar scale must be positive");

        if (bar_length_ == 0) {
            auto w = std::getenv("WIDTH");
            if (w != nullptr && *w != 0)
                bar_length_ = to_size(w);
            if (bar_length_ == 0)
                bar_length_ = 80;
            bar_length_ -= bar_offset_ + tail_length_ + 4;
        }

        *output_ << xterm_.beginning_of_line();
        if (! label.empty())
            *output_ << xterm_.rgb(colours_.text) << label << " ";
        *output_ << std::string(xterm_.rgb(colours_.done)) << "["
            << xterm_.rgb(colours_.todo) << std::string(bar_length_, '-')
            << xterm_.rgb(colours_.done) << "] " << std::string(tail_length_, ' ')
            << xterm_.reset() << std::flush;

        start_time_ = system_clock::now();

    }

    ProgressBar::~ProgressBar() noexcept {
        try { *output_ << xterm_.move_left(tail_length_) << xterm_.erase_right() << std::endl; }
            catch (...) {}
    }

    void ProgressBar::update(double x) {

        using namespace Literals;
        using namespace std::chrono;

        static const auto eta_format = "ETA {0:g2}{1}"_fmt;

        double y = std::clamp(x / scale_, 0.0, 1.0);
        std::string eta;
        auto now = system_clock::now();

        if (y > 0 && y < 1 && now > start_time_) {
            auto elapsed = duration_cast<duration<double>>(now - start_time_);
            auto estimate = elapsed * ((1 - y) / y);
            double count = estimate.count();
            char unit = 's';
            if (count >= 86400) {
                count /= 86400;
                unit = 'd';
            } else if (count >= 3600) {
                count /= 3600;
                unit = 'h';
            } else if (count >= 60) {
                count /= 60;
                unit = 'm';
            }
            eta = eta_format(count, unit);
        }

        eta.resize(tail_length_, ' ');
        size_t new_pos = std::max(size_t(std::llround(y * bar_length_)), current_pos_);
        size_t n_left = bar_length_ - current_pos_ + tail_length_ + 2;
        size_t n_advance = new_pos - current_pos_;
        size_t n_right = bar_length_ - new_pos + 2;

        *output_ << xterm_.move_left(int(n_left)) << xterm_.rgb(colours_.done) << std::string(n_advance, '+')
            << xterm_.move_right(int(n_right)) << xterm_.rgb(colours_.eta) << eta
            << xterm_.reset() << std::flush;

        current_pos_ = new_pos;

    }

}
