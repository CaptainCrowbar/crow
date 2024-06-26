#pragma once

#include "crow/string.hpp"
#include "crow/types.hpp"
#include "crow/unicode.hpp"
#include <concepts>
#include <ostream>
#include <string>
#include <string_view>

namespace Crow {

    class FormatSpec {

    public:

        FormatSpec() = default;
        FormatSpec(std::string_view str);
        FormatSpec(const std::string& str): FormatSpec(std::string_view(str)) {}
        FormatSpec(const char* str): FormatSpec(std::string_view(str)) {}
        FormatSpec(char m, const std::string& o, int p = -1);
        FormatSpec(int p): prec_(p) {}

        bool empty() const noexcept { return mode_ == 0 && opts_.empty() && prec_ == -1; }

        char mode() const noexcept { return mode_; }
        char lcmode() const noexcept { return ascii_tolower(mode_); }
        char find_mode(std::string_view chars) const noexcept;
        void default_mode(char m);
        void set_mode(char m);
        void exclude_mode(std::string_view chars) const;

        bool option(char c) const noexcept { return opts_.find(c) != npos; }
        std::string options() const { return opts_; }
        char find_option(std::string_view chars) const noexcept;
        void no_option(std::string_view chars) noexcept;
        void exclude_option(std::string_view chars) const;

        int prec() const noexcept { return prec_; }
        bool has_prec() const noexcept { return prec_ >= 0; }
        void default_prec(int p);
        void set_prec(int p);

        std::string str() const;

    private:

        char mode_ = '\0';
        std::string opts_;
        int prec_ = -1;

    };

    class Formatted {
    public:
        virtual ~Formatted() noexcept {}
        virtual std::string str(const FormatSpec& spec = {}) const = 0;
    };

    inline std::string to_string(const Formatted& obj, const FormatSpec& spec = {}) { return obj.str(spec); }
    inline std::ostream& operator<<(std::ostream& out, const Formatted& obj) { return out << obj.str(FormatSpec()); }

    namespace Detail {

        template <typename T>
        concept ExtendedStrMethodType = requires (T t, FormatSpec fs) {
            { t.str(fs) } -> std::convertible_to<std::string>;
        };

    }

}
