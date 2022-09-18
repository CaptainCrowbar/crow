#pragma once

#include "crow/format-type.hpp"
#include "crow/types.hpp"
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>

namespace Crow {

    namespace Detail {

        template <typename T, typename... Args>
        void print_helper(std::string& str, const T& t, const Args&... args) {
            str += format_object(t);
            if constexpr (sizeof...(Args) > 0) {
                str += ' ';
                print_helper(str, args...);
            }
        }

        inline void check_lf(std::string& str) {
            if (str.empty() || str.back() != '\n')
                str += '\n';
        }

    }

    template <typename... Args>
    std::string prints(const Args&... args) {
        std::string str;
        Detail::print_helper(str, args...);
        return str;
    }

    template <typename... Args>
    void printct(std::ostream& out, const std::string& pattern, const Args&... args) {
        auto str = Formatter(pattern)(args...);
        Detail::check_lf(str);
        out.write(str.data(), str.size());
    }

    template <typename... Args>
    void printct(FILE* out, const std::string& pattern, const Args&... args) {
        auto str = Formatter(pattern)(args...);
        Detail::check_lf(str);
        std::fwrite(str.data(), 1, str.size(), out);
    }

    template <typename... Args>
    void printc(const std::string& pattern, const Args&... args) {
        printct(std::cout, pattern, args...);
    }

    template <typename... Args>
    void printpt(std::ostream& out, const Args&... args) {
        auto str = prints(args...);
        Detail::check_lf(str);
        out.write(str.data(), str.size());
    }

    template <typename... Args>
    void printpt(FILE* out, const Args&... args) {
        auto str = prints(args...);
        Detail::check_lf(str);
        std::fwrite(str.data(), 1, str.size(), out);
    }

    template <typename... Args>
    void printp(const Args&... args) {
        printpt(std::cout, args...);
    }

    inline std::string prints() { return {}; }
    inline void printc() { std::cout << '\n'; }
    inline void printct(std::ostream& out) { out.put('\n'); }
    inline void printct(FILE* out) { std::fwrite("\n", 1, 1, out); }
    inline void printp() { std::cout << '\n'; }
    inline void printpt(std::ostream& out) { out.put('\n'); }
    inline void printpt(FILE* out) { std::fwrite("\n", 1, 1, out); }
    template <typename... Args> void printcft(std::ostream& out, const std::string& pattern, const Args&... args)
        { printct(out, pattern, args...); out.flush(); }
    template <typename... Args> void printcft(FILE* out, const std::string& pattern, const Args&... args)
        { printct(out, pattern, args...); std::fflush(out); }
    template <typename... Args> void printcf(const std::string& pattern, const Args&... args)
        { printcft(stdout, pattern, args...); }
    template <typename... Args> void printpft(std::ostream& out, const Args&... args) { printpt(out, args...); out.flush(); }
    template <typename... Args> void printpft(FILE* out, const Args&... args) { printpt(out, args...); std::fflush(out); }
    template <typename... Args> void printpf(const Args&... args) { printpft(stdout, args...); }
    inline void printcft(std::ostream& out) { printct(out); out.flush(); }
    inline void printcft(FILE* out) { printct(out); std::fflush(out); }
    inline void printcf() { printcft(stdout); }
    inline void printpft(std::ostream& out) { printpt(out); out.flush(); }
    inline void printpft(FILE* out) { printpt(out); std::fflush(out); }
    inline void printpf() { printpft(stdout); }

}
