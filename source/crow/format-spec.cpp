#include "crow/format-spec.hpp"
#include <algorithm>
#include <stdexcept>

namespace Crow {

    // Class FormatSpec

    FormatSpec::FormatSpec(const std::string& str) {
        if (str.empty())
            return;
        if (str[0] == '*')
            mode_ = 0;
        else if (ascii_isalpha(str[0]))
            mode_ = str[0];
        else
            throw std::invalid_argument("Invalid format spec: " + quote(str));
        auto end_options = std::find_if_not(str.begin() + 1, str.end(), ascii_isalpha);
        auto end_prec = std::find_if_not(end_options, str.end(), ascii_isdigit);
        if (end_prec != str.end())
            throw std::invalid_argument("Invalid format spec: " + quote(str));
        opts_.assign(str.begin() + 1, end_options);
        if (end_options != str.end())
            prec_ = to_int(std::string(end_options, str.end()));
    }

    FormatSpec::FormatSpec(char m, const std::string& o, int p):
    mode_(m), opts_(o), prec_(p) {
        if ((m != 0 && m != '*' && ! ascii_isalpha(m))
                || std::find_if_not(o.begin(), o.end(), ascii_isalpha) != o.end()
                || p < -1)
            throw std::invalid_argument("Invalid format spec: " + quote(m + o + std::to_string(p)));
    }

    char FormatSpec::find_mode(const std::string& chars) const noexcept {
        auto pos = chars.find(mode_);
        return pos == npos ? '\0' : chars[pos];
    }

    char FormatSpec::find_option(const std::string& chars) const noexcept {
        auto pos = opts_.find_first_of(chars);
        return pos == npos ? '\0' : opts_[pos];
    }

    void FormatSpec::no_option(const std::string& chars) noexcept {
        size_t i = 0;
        while (i < opts_.size()) {
            if (chars.find(opts_[i]) == npos)
                ++i;
            else
                opts_.erase(i, 1);
        }
    }

    std::string FormatSpec::str() const {
        if (mode_ == 0)
            return {};
        std::string s = mode_ + opts_;
        if (prec_ >= 0)
            s += std::to_string(prec_);
        return s;
    }

}
