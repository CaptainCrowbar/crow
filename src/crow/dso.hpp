#pragma once

#include "crow/enum.hpp"
#include "crow/path.hpp"
#include "crow/types.hpp"
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

namespace Crow {

    class Dso {

    public:

        enum class flag_type: uint32_t {

            none                = 0,
            global              = 1u << 0,   // [Posix]    Symbols are available when relocating other libraries
            lazy                = 1u << 1,   // [Posix]    Relocations may be delayed
            local               = 1u << 2,   // [Posix]    Symbols are not visible
            now                 = 1u << 3,   // [Posix]    Relocations are performed immediately
            first               = 1u << 4,   // [Apple]    Search only this library for symbols
            nodelete            = 1u << 5,   // [Apple]    Never unload
            noload              = 1u << 6,   // [Apple]    Do not load, succeed only if already loaded
            alter_search        = 1u << 7,   // [Windows]  Use alternative standard search path
            datafile            = 1u << 8,   // [Windows]  Map address space as data, do not execute
            datafile_exclusive  = 1u << 9,   // [Windows]  Map address space as data, with exclusive write access
            ignore_authz        = 1u << 10,  // [Windows]  Do not check software restriction policies
            image_resource      = 1u << 11,  // [Windows]  Map address space as image, do not execute
            search_application  = 1u << 12,  // [Windows]  Search application directory only
            search_default      = 1u << 13,  // [Windows]  Search application, system, and user-added directories
            search_dll          = 1u << 14,  // [Windows]  Search DLL directory for its dependencies
            search_system       = 1u << 15,  // [Windows]  Search system directory only
            search_user         = 1u << 16,  // [Windows]  Search user-added directories only

        };

        using enum flag_type;

        static constexpr flag_type default_flags = flag_type(uint32_t(flag_type::global) | uint32_t(flag_type::now));

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

    CROW_ENUM_BITMASK_OPERATORS(Dso::flag_type)

}
