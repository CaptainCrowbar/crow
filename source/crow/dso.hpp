#pragma once

#include "crow/path.hpp"
#include "crow/types.hpp"
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

namespace Crow {

    class Dso {

    public:

        using flag_type = uint32_t;

        enum: flag_type {

            global              = flag_type(1) << 0,   // [Posix]    Symbols are available when relocating other libraries
            lazy                = flag_type(1) << 1,   // [Posix]    Relocations may be delayed
            local               = flag_type(1) << 2,   // [Posix]    Symbols are not visible
            now                 = flag_type(1) << 3,   // [Posix]    Relocations are performed immediately
            first               = flag_type(1) << 4,   // [Apple]    Search only this library for symbols
            nodelete            = flag_type(1) << 5,   // [Apple]    Never unload
            noload              = flag_type(1) << 6,   // [Apple]    Do not load, succeed only if already loaded
            alter_search        = flag_type(1) << 7,   // [Windows]  Use alternative standard search path
            datafile            = flag_type(1) << 8,   // [Windows]  Map address space as data, do not execute
            datafile_exclusive  = flag_type(1) << 9,   // [Windows]  Map address space as data, with exclusive write access
            ignore_authz        = flag_type(1) << 10,  // [Windows]  Do not check software restriction policies
            image_resource      = flag_type(1) << 11,  // [Windows]  Map address space as image, do not execute
            search_application  = flag_type(1) << 12,  // [Windows]  Search application directory only
            search_default      = flag_type(1) << 13,  // [Windows]  Search application, system, and user-added directories
            search_dll          = flag_type(1) << 14,  // [Windows]  Search DLL directory for its dependencies
            search_system       = flag_type(1) << 15,  // [Windows]  Search system directory only
            search_user         = flag_type(1) << 16,  // [Windows]  Search user-added directories only

        };

        static constexpr flag_type default_flags = global | now;

        Dso() = default;
        explicit Dso(const Path& file, flag_type flags = default_flags) { load_library(file, flags, true); }
        ~Dso() noexcept { reset(); }
        Dso(const Dso& dso) = delete;
        Dso(Dso&& dso) noexcept;
        Dso& operator=(const Dso& dso) = delete;
        Dso& operator=(Dso&& dso) noexcept;

        explicit operator bool() const noexcept { return bool(handle_); }
        Path file() const { return file_; }
        void* native_handle() const noexcept { return handle_; }
        template <typename Sym> Sym symbol(const std::string& name);
        template <typename Sym> bool symbol(const std::string& name, Sym& sym) noexcept;

        static Dso search(const std::vector<std::string>& names, flag_type flags = default_flags);
        template <typename... Args> static Dso search(Args... args);
        static Dso self(flag_type flags = default_flags) { return Dso(Path(), flags); }

    private:

        #ifdef _XOPEN_SOURCE
            using symbol_type = void*;
        #else
            using symbol_type = void (*)();
        #endif

        template <typename T, bool = std::is_function<T>::value> struct sym_type;
        template <typename T> struct sym_type<T, true> { using ptr = T*; };
        template <typename T> struct sym_type<T*, false> { using ptr = T*; };
        template <typename R, typename... Args> struct sym_type<std::function<R(Args...)>, false>: sym_type<R(Args...)> {};

        Path file_;
        void* handle_ = nullptr;

        bool load_library(const Path& file, flag_type flags, bool check);
        symbol_type load_symbol(const std::string& name, bool check);
        void reset() noexcept;

        template <typename Sym> Sym convert_symbol(symbol_type s) noexcept;
        static Dso do_search(const std::vector<std::string>& names, flag_type flags = default_flags);
        template <typename... Args> static Dso do_search(std::vector<std::string>& names, const std::string& next, Args... args);

    };

        template <typename Sym>
        Sym Dso::symbol(const std::string& name) {
            return convert_symbol<Sym>(load_symbol(name, true));
        }

        template <typename Sym>
        bool Dso::symbol(const std::string& name, Sym& sym) noexcept {
            auto raw_symbol = load_symbol(name, false);
            if (! raw_symbol)
                return false;
            sym = convert_symbol<Sym>(raw_symbol);
            return true;
        }

        template <typename... Args>
        Dso Dso::search(Args... args) {
            std::vector<std::string> names;
            return do_search(names, args...);
        }

        template <typename Sym>
        Sym Dso::convert_symbol(symbol_type s) noexcept {
            using SP = typename sym_type<Sym>::ptr;
            auto sp = reinterpret_cast<SP>(s);
            return Sym(sp);
        }

        template <typename... Args>
        Dso Dso::do_search(std::vector<std::string>& names, const std::string& next, Args... args) {
            names.emplace_back(next);
            return do_search(names, args...);
        }

}
