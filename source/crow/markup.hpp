#pragma once

#include "crow/types.hpp"
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

namespace Crow {

    class Xtag {
    public:
        Xtag() = default;
        explicit Xtag(const std::string& element): Xtag(element, std::cout) {}
        Xtag(const std::string& element, std::string& out);
        Xtag(const std::string& element, std::ostream& out);
        Xtag(const Xtag&) = delete;
        Xtag(Xtag&& t) noexcept;
        virtual ~Xtag() noexcept { try { stop(); } catch (...) {} }
        Xtag& operator=(const Xtag&) = delete;
        Xtag& operator=(Xtag&& t) noexcept;
        std::string tag() const { return tag_; }
        void next(const std::string& element = {});
        template <typename Out, typename... Args> static void nest(Out& out, const Args&... args) { do_nest(out, args...); }
    protected:
        void start(const std::string& element);
        void stop(bool close = true);
    private:
        std::string element_;
        std::string tag_;
        std::function<void(const std::string&)> write_;
        bool lf_ = false;
        template <typename Out, typename... Args> static void do_nest(Out& out, const std::string& arg, const Args&... args);
        static void do_nest(std::string& out, const std::string& arg) { out += arg; }
        static void do_nest(std::ostream& out, const std::string& arg) { out << arg; }
    };

        template <typename Out, typename... Args>
        void Xtag::do_nest(Out& out, const std::string& arg, const Args&... args) {
            Xtag tag(arg, out);
            do_nest(out, args...);
        }

    class Htag:
    public Xtag {
    public:
        Htag() = default;
        explicit Htag(const std::string& element): Htag(element, std::cout) {}
        Htag(const std::string& element, std::string& out): Xtag(element, out) { init(); }
        Htag(const std::string& element, std::ostream& out): Xtag(element, out) { init(); }
        Htag(const Htag&) = delete;
        Htag(Htag&& t) = default;
        Htag& operator=(const Htag&) = delete;
        Htag& operator=(Htag&& t) = default;
    private:
        void init();
    };

}
