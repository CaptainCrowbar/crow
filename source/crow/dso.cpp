#include "crow/dso.hpp"
#include "crow/string.hpp"
#include <system_error>
#include <utility>

#ifdef _XOPEN_SOURCE
    #include <dlfcn.h>
#else
    #include <windows.h>
#endif

namespace Crow {

    namespace {

        #ifdef _XOPEN_SOURCE

            using native_flag_type = int;
            using native_handle_type = void*;

            std::string dl_error() {
                auto cptr = dlerror();
                return cptr ? std::string(cptr) : std::string();
            }

        #else

            using native_flag_type = uint32_t;
            using native_handle_type = HMODULE;

        #endif

        #define TRANSLATE_FLAG(from, to) if ((flags & Dso::from) != 0) native |= to

        native_flag_type translate_flags(Dso::flag_type flags) noexcept {

            native_flag_type native = 0;

            #ifdef _XOPEN_SOURCE

                TRANSLATE_FLAG(global,  RTLD_GLOBAL);
                TRANSLATE_FLAG(lazy,    RTLD_LAZY);
                TRANSLATE_FLAG(local,   RTLD_LOCAL);
                TRANSLATE_FLAG(now,     RTLD_NOW);

                #ifdef __APPLE__

                    TRANSLATE_FLAG(first,     RTLD_FIRST);
                    TRANSLATE_FLAG(nodelete,  RTLD_NODELETE);
                    TRANSLATE_FLAG(noload,    RTLD_NOLOAD);

                #endif

            #else

                TRANSLATE_FLAG(alter_search,        LOAD_WITH_ALTERED_SEARCH_PATH);
                TRANSLATE_FLAG(datafile,            LOAD_LIBRARY_AS_DATAFILE);
                TRANSLATE_FLAG(datafile_exclusive,  LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE);
                TRANSLATE_FLAG(ignore_authz,        LOAD_IGNORE_CODE_AUTHZ_LEVEL);
                TRANSLATE_FLAG(image_resource,      LOAD_LIBRARY_AS_IMAGE_RESOURCE);
                TRANSLATE_FLAG(search_application,  LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
                TRANSLATE_FLAG(search_default,      LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
                TRANSLATE_FLAG(search_dll,          LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
                TRANSLATE_FLAG(search_system,       LOAD_LIBRARY_SEARCH_SYSTEM32);
                TRANSLATE_FLAG(search_user,         LOAD_LIBRARY_SEARCH_USER_DIRS);

            #endif

            return native;

        }

    }

    Dso::Dso(Dso&& dso) noexcept:
    file_(std::move(dso.file_)),
    handle_(std::exchange(dso.handle_, nullptr)) {}

    Dso& Dso::operator=(Dso&& dso) noexcept {
        if (&dso != this) {
            reset();
            file_ = std::move(dso.file_);
            handle_ = std::exchange(dso.handle_, nullptr);
        }
        return *this;
    }

    Dso Dso::search(const std::vector<std::string>& names, flag_type flags) {
        return do_search(names, flags);
    }

    bool Dso::load_library(const Path& file, flag_type flags, bool check) {

        reset();
        file_ = {};
        auto nflags = translate_flags(flags);

        #ifdef _XOPEN_SOURCE

            const char* c_file = file.empty() ? nullptr : file.c_name();
            handle_ = dlopen(c_file, nflags);

            if (check && handle_ == nullptr) {
                std::string msg = dl_error();
                if (! file.empty())
                    msg = quote(file.name()) + ": " + msg;
                throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), msg);
            }

        #else

            if (file.empty()) {
                handle_ = GetModuleHandle(nullptr);
            } else {
                auto wname = file.os_name();
                handle_ = LoadLibraryExW(wname.data(), nullptr, nflags);
            }

            if (check && handle_ == nullptr) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category(), file.name());
            }

        #endif

        file_ = std::move(file);

        return handle_ != nullptr;

    }

    Dso::symbol_type Dso::load_symbol(const std::string& name, bool check) {

        symbol_type sym;

        #ifdef _XOPEN_SOURCE

            sym = dlsym(native_handle_type(handle_), name.data());
            if (check && ! sym)
                throw std::system_error(std::make_error_code(std::errc::function_not_supported), quote(name) + ": " + dl_error());

        #else

            sym = reinterpret_cast<symbol_type>(GetProcAddress(native_handle_type(handle_), name.data()));
            if (check && ! sym) {
                int err = GetLastError();
                throw std::system_error(err, std::system_category(), name);
            }

        #endif

        return sym;

    }

    void Dso::reset() noexcept {
        if (handle_ != nullptr) {
            #ifdef _XOPEN_SOURCE
                dlclose(native_handle_type(handle_));
            #else
                FreeLibrary(native_handle_type(handle_));
            #endif
            handle_ = nullptr;
        }
        file_ = {};
    }

    Dso Dso::do_search(const std::vector<std::string>& names, flag_type flags) {

        #ifdef _XOPEN_SOURCE
            static constexpr const char* delimiters = "/";
            #if defined(__APPLE__)
                static constexpr const char* suffix = ".dylib";
            #else
                static constexpr const char* suffix = ".so";
            #endif
        #else
            static constexpr const char* delimiters = "/\\";
            static constexpr const char* suffix = ".dll";
        #endif

        Dso dso;
        auto nflags = translate_flags(flags);

        for (auto& name: names) {
            if (dso.load_library(name, nflags, false))
                return dso;
            if (name.find_first_of(delimiters) != npos)
                continue;
            std::string libname = name + suffix;
            if (dso.load_library(libname, nflags, false))
                return dso;
            if (dso.load_library("lib" + libname, nflags, false))
                return dso;
        }

        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), join(names, "|"));

    }

}
