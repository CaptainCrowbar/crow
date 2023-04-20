#include "crow/markup.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <unordered_set>
#include <utility>

namespace Crow {

    // Class Xtag

    Xtag::Xtag(const std::string& element, std::string& out):
    element_(element),
    tag_(),
    write_([&out] (const std::string& s) { out += s; }),
    lf_(false)  {
        start(element);
    }

    Xtag::Xtag(const std::string& element, std::ostream& out):
    element_(element),
    tag_(),
    write_([&out] (const std::string& s) { out << s; }),
    lf_(false)  {
        start(element);
    }

    Xtag::Xtag(Xtag&& t) noexcept:
    element_(std::exchange(t.element_, std::string())),
    tag_(std::exchange(t.tag_, std::string())),
    write_(std::move(t.write_)),
    lf_(std::exchange(t.lf_, false)) {}

    Xtag& Xtag::operator=(Xtag&& t) noexcept {
        if (&t != this) {
            stop();
            element_ = std::exchange(t.element_, std::string());
            tag_ = std::exchange(t.tag_, std::string());
            write_ = std::move(t.write_);
            lf_ = std::exchange(t.lf_, false);
        }
        return *this;
    }

    void Xtag::next(const std::string& element) {
        auto arg = element.empty() ? element_ : element;
        stop();
        start(arg);
    }

    void Xtag::start(const std::string& element) {
        static const std::string trim_chars = std::string(ascii_whitespace) + "<>";
        tag_.clear();
        lf_ = false;
        std::string content = trim_right(element, "\n");
        size_t n_lines = element.size() - content.size();
        content = trim(content, trim_chars);
        if (content.empty())
            return;
        auto cut = std::find_if_not(content.begin(), content.end(), ascii_isalnum_w);
        std::string opening = '<' + content + '>';
        write_(opening);
        if (n_lines >= 2)
            write_("\n");
        lf_ = n_lines >= 1;
        if (content.back() == '/')
            stop();
        else
            tag_.assign(content.begin(), cut);
    }

    void Xtag::stop(bool close) {
        if (write_) {
            if (close && ! tag_.empty())
                write_("</" + tag_ + ">");
            if (lf_)
                write_("\n");
            tag_.clear();
            lf_ = false;
        }
    }

    // Class Htag

    void Htag::init() {
        // https://www.w3.org/TR/html5/syntax.html#void-elements
        static const std::unordered_set<std::string> void_tags =
            { "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr" };
        if (void_tags.count(ascii_lowercase(tag())))
            stop(false);
    }

}
