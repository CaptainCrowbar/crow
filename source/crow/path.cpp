#include "crow/regex.hpp"
#include "crow/guard.hpp"
#include "crow/path.hpp"
#include "crow/string.hpp"
#include "crow/time.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <system_error>

#ifdef __APPLE__
    #include <Availability.h>
#endif

#ifdef _XOPEN_SOURCE

    #include <dirent.h>
    #include <fcntl.h>
    #include <pwd.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>

    #define OS_CHAR(c) c
    #define OS_FUNCTION(f) f

#else

    #include <windows.h>

    #define OS_CHAR(c) L ## c
    #define OS_FUNCTION(f) _w ## f

#endif

using namespace std::chrono;
using namespace std::literals;

namespace Crow {

    namespace {

        #ifdef _XOPEN_SOURCE

            struct StatResult {
                struct stat st;
                bool ok;
            };

            StatResult get_stat(const std::string& file, Path::flag flags) noexcept {
                StatResult result;
                result.ok = (!! (flags & Path::flag::no_follow) ? lstat : stat)(file.data(), &result.st) == 0;
                if (! result.ok)
                    memset(&result.st, 0, sizeof(result.st));
                return result;
            }

            Path get_user_home(std::string user) {
                const char* envptr = nullptr;
                if (user.empty()) {
                    envptr = getenv("HOME");
                    if (envptr && *envptr)
                        return Path(envptr);
                }
                std::string workbuf(1024, '\0');
                passwd pwdbuf;
                passwd* pwdptr = nullptr;
                int rc = 0;
                for (;;) {
                    if (user.empty())
                        rc = getpwuid_r(geteuid(), &pwdbuf, workbuf.data(), workbuf.size(), &pwdptr);
                    else
                        rc = getpwnam_r(user.data(), &pwdbuf, workbuf.data(), workbuf.size(), &pwdptr);
                    if (rc != ERANGE)
                        break;
                    workbuf.resize(2 * workbuf.size());
                }
                if (pwdptr && pwdptr->pw_dir && *pwdptr->pw_dir)
                    return Path(pwdptr->pw_dir);
                if (user.empty()) {
                    envptr = getenv("USER");
                    if (envptr && *envptr)
                        user = envptr;
                    else if (pwdptr && pwdptr->pw_name && *pwdptr->pw_name)
                        user = pwdptr->pw_name;
                    else
                        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
                }
                #ifdef __APPLE__
                    static const Path dir("/Users");
                #else
                    static const Path dir("/home");
                #endif
                return dir / user;
            }

        #else

            uint32_t get_attributes(const std::wstring& file) noexcept {
                auto rc = GetFileAttributesW(file.data());
                return rc == INVALID_FILE_ATTRIBUTES ? 0 : rc;
            }

            WIN32_FILE_ATTRIBUTE_DATA get_attributes_ex(const std::wstring& file) noexcept {
                WIN32_FILE_ATTRIBUTE_DATA info;
                memset(&info, 0, sizeof(info));
                GetFileAttributesExW(file.data(), GetFileExInfoStandard, &info);
                return info;
            }

            std::string make_utf8(const std::wstring& wstr) {
                return to_utf8(decode_string(wstr));
            }

            std::string dumb_ascii_conversion(const std::wstring& wstr) {
                std::string ascii(wstr.size(), '\0');
                std::transform(wstr.begin(), wstr.end(), ascii.begin(),
                    [] (wchar_t w) { return w <= 127 ? char(w) : '_'; });
                return ascii;
            }

        #endif

        Path get_current_directory(bool checked) {
            #ifdef _XOPEN_SOURCE
                std::string buf(256, '\0');
                for (;;) {
                    auto rc = getcwd(buf.data(), buf.size());
                    int err = errno;
                    if (rc)
                        return rc;
                    else if (err == ERANGE)
                        buf.resize(2 * buf.size());
                    else if (checked)
                        throw std::system_error(err, std::generic_category());
                    else
                        return {};
                }
            #else
                auto size = GetCurrentDirectoryW(0, nullptr);
                std::wstring buf(size, L'\0');
                auto rc = GetCurrentDirectoryW(size, buf.data());
                auto err = GetLastError();
                if (rc > 0)
                    return buf.data();
                else if (checked)
                    throw std::system_error(err, std::system_category());
                else
                    return {};
            #endif
        }

        std::string make_utf8(const std::string& str) {
            return str;
        }

        Path::string_type make_native_string(const std::string& utf8) {
            #ifdef _XOPEN_SOURCE
                return utf8;
            #else
                return to_wstring(decode_string(utf8));
            #endif
        }

    }

    // Constructors

    #ifdef _XOPEN_SOURCE

        Path::Path(const std::string& file, flag flags):
        filename_(file) {
            make_canonical(flags);
        }

    #else

        Path::Path(const std::string& file, flag flags):
        filename_(to_wstring(decode_string(file))) {
            make_canonical(flags);
        }

        Path::Path(const std::wstring& file, flag flags):
        filename_(file) {
            make_canonical(flags);
        }

    #endif

    // Path name functions

    std::string Path::name() const {
        return make_utf8(filename_);
    }

    std::string Path::as_url() const {
        if (! is_absolute())
            throw std::invalid_argument("File URL must be an absolute path");
        std::string prefix = "file://";
        std::string suffix = name();
        #ifndef _XOPEN_SOURCE
            std::replace(suffix.begin(), suffix.end(), '\\', '/');
        #endif
        if (suffix[0] != '/')
            prefix += '/';
        return prefix + suffix;
    }

    std::vector<std::string> Path::breakdown() const {
        #ifdef _XOPEN_SOURCE
            return os_breakdown();
        #else
            auto os_parts = os_breakdown();
            std::vector<std::string> utf8_parts(os_parts.size());
            std::transform(os_parts.begin(), os_parts.end(), utf8_parts.begin(),
                [=] (auto& s) { return make_utf8(s); });
            return utf8_parts;
        #endif
    }

    std::vector<Path::string_type> Path::os_breakdown() const {
        std::vector<string_type> parts;
        if (empty())
            return parts;
        auto root = get_root(true);
        if (! root.empty())
            parts.push_back(root);
        size_t p = root.size(), q = 0, size = filename_.size();
        while (p < size) {
            q = filename_.find(native_delimiter, p);
            if (q == npos)
                q = size;
            parts.push_back(filename_.substr(p, q - p));
            p = q + 1;
        }
        return parts;
    }

    Path Path::change_ext(const std::string& new_ext) const {
        if (empty() || get_root(true).size() == filename_.size())
            throw std::invalid_argument("Can't change file extension: " + quote(name()));
        string_type prefix = filename_;
        prefix.resize(prefix.size() - get_base_ext().second.size());
        auto suffix = make_native_string(new_ext);
        if (! suffix.empty() && suffix[0] != OS_CHAR('.'))
            prefix += OS_CHAR('.');
        return prefix + suffix;
    }

    bool Path::is_absolute() const noexcept {
        #ifdef _XOPEN_SOURCE
            return filename_[0] == '/';
        #else
            static const Regex pattern(R"(
                ^ ( \\{2}\?\\ ) ?
                ( [A-Z] :\\ | \\{2,} (?= [^?\\] ))
            )", Regex::extended | Regex::icase | Regex::no_capture | Regex::optimize);
            auto ascii_name = dumb_ascii_conversion(filename_);
            return pattern(ascii_name).matched();
        #endif
    }

    bool Path::is_drive_absolute() const noexcept {
        #ifdef _XOPEN_SOURCE
            return false;
        #else
            return ! filename_.empty() && filename_[0] == L'\\' && filename_[1] != L'\\';
        #endif
    }

    bool Path::is_drive_relative() const noexcept {
        #ifdef _XOPEN_SOURCE
            return false;
        #else
            return filename_.size() >= 2 && filename_[0] >= L'A' && filename_[0] <= L'Z' && filename_[1] == L':' && filename_[2] != L'\\';
        #endif
    }

    bool Path::is_relative() const noexcept {
        return ! empty() && ! is_absolute() && ! is_drive_absolute() && ! is_drive_relative();
    }

    bool Path::is_leaf() const noexcept {
        if (filename_.empty())
            return false;
        #ifdef _XOPEN_SOURCE
            return filename_.find('/') == npos;
        #else
            return filename_.find_first_of(L":\\") == npos;
        #endif
    }

    bool Path::is_legal() const noexcept {
        if (filename_.find(character_type(0)) != npos)
            return false;
        #ifndef _XOPEN_SOURCE
            auto root = get_root(true);
            if (root.empty() && filename_[0] == L'\\')
                return false;
            if (filename_.find_first_of(L"\"*:<>?|", root.size()) != npos)
                return false;
        #endif
        return true;
    }

    bool Path::is_root() const noexcept {
        return ! filename_.empty() && get_root(false).size() == filename_.size();
    }

    Path::form Path::path_form() const noexcept {
        if (filename_.empty())          return form::empty;
        else if (is_drive_absolute())  return form::drive_absolute;
        else if (is_drive_relative())  return form::drive_relative;
        else if (is_absolute())        return form::absolute;
        else                           return form::relative;
    }

    Path Path::relative_to(const Path& base) const {
        auto base_form = base.path_form(), this_form = path_form();
        if (! (base_form == form::absolute && this_form == form::absolute)
                && ! (base_form == form::relative && this_form == form::relative))
            throw std::invalid_argument("Invalid combination of arguments to Path::relative_to()");
        auto base_vec = base.os_breakdown(), this_vec = os_breakdown();
        if (this_form == form::absolute && base_vec[0] != this_vec[0])
            return *this;
        auto cuts = std::mismatch(base_vec.begin(), base_vec.end(), this_vec.begin(), this_vec.end());
        Path prefix;
        if (base_vec.end() > cuts.first) {
            size_t len = 3 * (base_vec.end() - cuts.first) - 1;
            prefix.filename_.assign(len, OS_CHAR('.'));
            for (size_t i = 2; i < len; i += 3)
                prefix.filename_[i] = native_delimiter;
        }
        Path suffix = join(irange(cuts.second, this_vec.end()));
        Path rel_path = prefix / suffix;
        if (rel_path.empty())
            rel_path = ".";
        return rel_path;
    }

    std::pair<std::string, std::string> Path::split_leaf() const {
        auto os_parts = split_os_leaf();
        std::pair<std::string, std::string> parts;
        parts.first = make_utf8(os_parts.first);
        parts.second = make_utf8(os_parts.second);
        return parts;
    }

    std::pair<Path::string_type, Path::string_type> Path::split_os_leaf() const {
        return get_base_ext();
    }

    std::pair<Path, Path> Path::split_path() const {
        auto leaf = get_leaf();
        auto dir = filename_.substr(0, filename_.size() - leaf.size());
        return {dir, leaf};
    }

    std::pair<Path, Path> Path::split_root() const {
        auto root = get_root(true);
        auto tail = filename_.substr(root.size());
        return {root, tail};
    }

    Path Path::common(const Path& lhs, const Path& rhs) {
        auto root1 = lhs.get_root(true);
        auto root2 = rhs.get_root(true);
        if (root1 != root2)
            return {};
        size_t cut = std::mismatch(lhs.filename_.begin(), lhs.filename_.end(),
            rhs.filename_.begin(), rhs.filename_.end()).first - lhs.filename_.begin();
        if (cut == root1.size())
            return lhs.filename_.substr(0, cut);
        else if ((cut == lhs.filename_.size() || lhs.filename_[cut] == native_delimiter)
                && (cut == rhs.filename_.size() || rhs.filename_[cut] == native_delimiter))
            return lhs.filename_.substr(0, cut);
        do --cut;
            while (cut > root1.size() && lhs.filename_[cut] != native_delimiter);
        return lhs.filename_.substr(0, cut);
    }

    Path Path::join(const Path& lhs, const Path& rhs) {
        if (lhs.empty() || rhs.is_absolute())
            return rhs;
        auto result = lhs.os_name();
        auto root = lhs.get_root(true);
        if (root.size() < result.size() && result.back() != native_delimiter && ! rhs.empty())
            result += native_delimiter;
        result += rhs.os_name();
        return result;
    }

    std::ostream& operator<<(std::ostream& out, Path::form f) {
        switch (f) {
            case Path::form::empty:           out << "empty"; break;
            case Path::form::absolute:        out << "absolute"; break;
            case Path::form::relative:        out << "relative"; break;
            case Path::form::drive_absolute:  out << "drive_absolute"; break;
            case Path::form::drive_relative:  out << "drive_relative"; break;
            default:                          out << int(f); break;
        }
        return out;
    }

    // File system query functions

    Path::time_point Path::access_time([[maybe_unused]] flag flags) const noexcept {
        time_point tp;
        #ifdef __APPLE__
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_atimespec, tp);
        #elif defined(_XOPEN_SOURCE)
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_atim, tp);
        #else
            tp = get_file_time(1);
        #endif
        return tp;
    }

    Path::time_point Path::create_time([[maybe_unused]] flag flags) const noexcept {
        time_point tp;
        #ifdef __APPLE__
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_birthtimespec, tp);
        #elif defined(_XOPEN_SOURCE)
            return {};
        #else
            tp = get_file_time(0);
        #endif
        return tp;
    }

    Path::time_point Path::modify_time([[maybe_unused]] flag flags) const noexcept {
        time_point tp;
        #ifdef __APPLE__
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_mtimespec, tp);
        #elif defined(_XOPEN_SOURCE)
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_mtim, tp);
        #else
            tp = get_file_time(2);
        #endif
        return tp;
    }

    Path::time_point Path::status_time([[maybe_unused]] flag flags) const noexcept {
        time_point tp;
        #ifdef __APPLE__
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_ctimespec, tp);
        #elif defined(_XOPEN_SOURCE)
            auto st = get_stat(filename_, flags).st;
            timespec_to_timepoint(st.st_ctim, tp);
        #endif
        return tp;
    }

    Path::directory_range Path::directory(flag flags) const {
        directory_iterator it(*this, flags);
        return {it, {}};
    }

    Path::search_range Path::deep_search(flag flags) const {
        search_iterator it(*this, flags);
        return {it, {}};
    }

    bool Path::exists([[maybe_unused]] flag flags) const noexcept {
        #ifdef _XOPEN_SOURCE
            struct stat st;
            if (!! (flags & flag::no_follow))
                return lstat(c_name(), &st) == 0;
            else
                return stat(c_name(), &st) == 0;
        #else
            return get_attributes(filename_);
        #endif
    }

    Path::id_type Path::id([[maybe_unused]] flag flags) const noexcept {
        uint64_t device = 0, file = 0;
        #ifdef _XOPEN_SOURCE
            auto st = get_stat(filename_, flags).st;
            device = uint64_t(st.st_dev);
            file = uint64_t(st.st_ino);
        #else
            auto handle = CreateFileW(c_name(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS , nullptr);
            if (handle != INVALID_HANDLE_VALUE) {
                BY_HANDLE_FILE_INFORMATION info;
                memset(&info, 0, sizeof(info));
                if (GetFileInformationByHandle(handle, &info)) {
                    device = uint64_t(info.dwVolumeSerialNumber);
                    file = (uint64_t(info.nFileIndexHigh) << 32) + uint64_t(info.nFileIndexLow);
                }
                CloseHandle(handle);
            }
        #endif
        return {device, file};
    }

    bool Path::is_directory([[maybe_unused]] flag flags) const noexcept {
        #ifdef _XOPEN_SOURCE
            auto st = get_stat(filename_, flags).st;
            return S_ISDIR(st.st_mode);
        #else
            return get_attributes(filename_) & FILE_ATTRIBUTE_DIRECTORY;
        #endif
    }

    bool Path::is_file([[maybe_unused]] flag flags) const noexcept {
        #ifdef _XOPEN_SOURCE
            auto st = get_stat(filename_, flags).st;
            return S_ISREG(st.st_mode);
        #else
            auto attr = get_attributes(filename_);
            return attr && ! (attr & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY));
        #endif
    }

    bool Path::is_special([[maybe_unused]] flag flags) const noexcept {
        #ifdef _XOPEN_SOURCE
            auto rc = get_stat(filename_, flags);
            return rc.ok && ! S_ISDIR(rc.st.st_mode) && ! S_ISREG(rc.st.st_mode);
        #else
            return get_attributes(filename_) & FILE_ATTRIBUTE_DEVICE;
        #endif
    }

    bool Path::is_hidden() const noexcept {
        if (filename_.empty() || filename_ == OS_CHAR(".") || filename_ == OS_CHAR(".."))
            return false;
        #ifdef _XOPEN_SOURCE
            auto leaf = get_leaf();
            return ! leaf.empty() && leaf[0] == '.';
        #else
            static constexpr uint32_t hidden_attr = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM
                #ifdef FILE_ATTRIBUTE_INTEGRITY_STREAM
                    | FILE_ATTRIBUTE_INTEGRITY_STREAM
                #endif
                #ifdef FILE_ATTRIBUTE_NO_SCRUB_DATA
                    | FILE_ATTRIBUTE_NO_SCRUB_DATA
                #endif
                ;
            return ! is_root() && (get_attributes(filename_) & hidden_attr);
        #endif
    }

    bool Path::is_symlink() const noexcept {
        #ifdef _XOPEN_SOURCE
            auto st = get_stat(filename_, flag::no_follow).st;
            return S_ISLNK(st.st_mode);
        #else
            return false;
        #endif
    }

    Path Path::resolve() const {
        Path result;
        if (empty()) {
            result = get_current_directory(false);
        } else {
            #ifdef _XOPEN_SOURCE
                if (filename_[0] == '~') {
                    size_t pos = filename_.find('/');
                    std::string tail, user = filename_.substr(1, pos - 1);
                    if (pos != npos)
                        tail = filename_.substr(pos + 1, npos);
                    Path home = get_user_home(user);
                    if (! home.empty())
                        result = home / tail;
                }
                if (result.empty() && is_relative()) {
                    Path cwd = get_current_directory(false);
                    if (! cwd.empty())
                        result = cwd / *this;
                }
            #else
                std::wstring buf(1024, L'\0'), resolved(filename_);
                for (;;) {
                    auto rc = GetLongPathNameW(resolved.data(), &buf[0], uint32_t(buf.size()));
                    if (rc == 0)
                        break;
                    if (rc < buf.size()) {
                        resolved.assign(buf.data(), rc);
                        break;
                    }
                    buf.resize(2 * buf.size());
                }
                for (;;) {
                    auto rc = GetFullPathNameW(resolved.data(), uint32_t(buf.size()), &buf[0], nullptr);
                    if (rc == 0)
                        break;
                    if (rc < buf.size()) {
                        resolved.assign(buf.data(), rc);
                        break;
                    }
                    buf.resize(2 * buf.size());
                }
                result = resolved;
            #endif
        }
        if (result.empty())
            result = *this;
        if (! result.is_absolute())
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), name());
        return result;
    }

    Path Path::resolve_symlink() const {
        #ifdef _XOPEN_SOURCE
            std::string buf(256, '\0');
            for (;;) {
                auto rc = readlink(c_name(), &buf[0], buf.size());
                if (rc < 0) {
                    int err = errno;
                    if (err == EINVAL || err == ENOENT || err == ENOTDIR)
                        return *this;
                    else
                        throw std::system_error(err, std::generic_category(), name());
                }
                if (size_t(rc) <= buf.size() - 2) {
                    buf.resize(rc);
                    return buf;
                }
                buf.resize(2 * buf.size());
            }
        #else
            return *this;
        #endif
    }

    uint64_t Path::size(flag flags) const {
        #ifdef _XOPEN_SOURCE
            auto st = get_stat(filename_, flags).st;
            uint64_t bytes = st.st_size;
        #else
            auto info = get_attributes_ex(filename_);
            uint64_t bytes = (uint64_t(info.nFileSizeHigh) << 32) + uint64_t(info.nFileSizeLow);
        #endif
        if (!! (flags & flag::recurse))
            for (auto& child: directory())
                bytes += child.size(flag::no_follow | flag::recurse);
        return bytes;
    }

    // File system update functions

    void Path::copy_to(const Path& dst, flag flags) const {
        static constexpr size_t block_size = 16384;
        if (! exists())
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), name());
        if (*this == dst || id() == dst.id())
            throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());
        if (is_directory() && ! (flags & flag::recurse))
            throw std::system_error(std::make_error_code(std::errc::is_a_directory), name());
        if (dst.exists()) {
            if (! (flags & flag::overwrite))
                throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());
            dst.remove(flag::recurse);
        }
        if (is_symlink()) {
            resolve_symlink().make_symlink(dst);
        } else if (is_directory()) {
            dst.make_directory();
            for (auto& child: directory())
                child.copy_to(dst / child.split_path().second, flag::recurse);
        } else {
            auto in = OS_FUNCTION(fopen)(c_name(), OS_CHAR("rb"));
            int err = errno;
            if (! in)
                throw std::system_error(err, std::generic_category(), name());
            auto guard_in = on_scope_exit([=] { fclose(in); });
            auto out = OS_FUNCTION(fopen)(dst.c_name(), OS_CHAR("wb"));
            err = errno;
            if (! out)
                throw std::system_error(err, std::generic_category(), dst.name());
            auto guard_out = on_scope_exit([=] { fclose(out); });
            std::string buf(block_size, '\0');
            while (! feof(in)) {
                errno = 0;
                size_t n = fread(&buf[0], 1, buf.size(), in);
                err = errno;
                if (err)
                    throw std::system_error(err, std::generic_category(), name());
                if (n) {
                    errno = 0;
                    fwrite(buf.data(), 1, n, out);
                    err = errno;
                    if (err)
                        throw std::system_error(err, std::generic_category(), dst.name());
                }
            }
        }
    }

    void Path::create() const {
        if (! exists())
            save(std::string{});
    }

    void Path::make_directory(flag flags) const {
        static const auto mkdir_call = [] (const Path& dir) {
            #ifdef _XOPEN_SOURCE
                return mkdir(dir.c_name(), 0777);
            #else
                return _wmkdir(dir.c_name());
            #endif
        };
        if (mkdir_call(*this) == 0)
            return;
        int err = errno;
        if (err == EEXIST) {
            if (is_directory())
                return;
            if (! (flags & flag::overwrite))
                throw std::system_error(err, std::generic_category(), name());
            remove();
        } else if (err == ENOENT && !! (flags & flag::recurse) && ! empty()) {
            Path parent = split_path().first;
            if (parent == *this)
                throw std::system_error(err, std::generic_category(), name());
            parent.make_directory(flags);
        }
        if (mkdir_call(*this) != 0) {
            err = errno;
            throw std::system_error(err, std::generic_category(), name());
        }
    }

    void Path::make_symlink(const Path& linkname, flag flags) const {
        #ifdef _XOPEN_SOURCE
            if (linkname.is_symlink()) {
                try {
                    if (linkname.resolve_symlink() == *this)
                        return;
                }
                catch (const std::system_error&) {}
            }
            if (!! (flags & flag::overwrite) && linkname.exists())
                linkname.remove(flags);
            if (symlink(c_name(), linkname.c_name()) == 0)
                return;
            if (! (flags & flag::may_copy)) {
                int err = errno;
                throw std::system_error(err, std::generic_category(), linkname.name() + " -> " + name());
            }
            copy_to(linkname, flag::recurse);
        #else
            if (!! (flags & flag::may_copy))
                copy_to(linkname, flag::recurse);
            else
                throw std::system_error(std::make_error_code(std::errc::operation_not_supported),
                    "Symbolic links are not supported on Windows");
        #endif
    }

    void Path::move_to(const Path& dst, flag flags) const {
        if (! exists())
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), name());
        if (*this == dst)
            return;
        if (dst.exists() && id() != dst.id()) {
            if (! (flags & flag::overwrite))
                throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());
            dst.remove(flag::recurse);
        }
        if (OS_FUNCTION(rename)(c_name(), dst.c_name()) == 0)
            return;
        int err = errno;
        if (err != EXDEV || ! (flags & flag::may_copy))
            throw std::system_error(err, std::generic_category(), name() + " -> " + dst.name());
        copy_to(dst, flag::recurse);
        remove(flag::recurse);
    }

    void Path::remove(flag flags) const {
        if (!! (flags & flag::recurse) && is_directory() && ! is_symlink())
            for (auto child: directory())
                child.remove(flag::recurse);
        #ifdef _XOPEN_SOURCE
            int rc = std::remove(c_name());
            int err = errno;
            if (rc != 0 && err != ENOENT)
                throw std::system_error(err, std::generic_category(), name());
        #else
            bool ok;
            if (is_directory())
                ok = RemoveDirectoryW(c_name());
            else
                ok = DeleteFileW(c_name());
            int err = GetLastError();
            if (! ok && err != ERROR_FILE_NOT_FOUND)
                throw std::system_error(err, std::system_category(), name());
        #endif
    }

    void Path::set_access_time(time_point t, [[maybe_unused]] flag flags) const {
        #ifdef _XOPEN_SOURCE
            set_file_times(t, modify_time(), flags);
        #else
            set_file_time(t, 1);
        #endif
    }

    void Path::set_create_time([[maybe_unused]] time_point t, flag /*flags*/) const {
        #ifdef _XOPEN_SOURCE
            throw std::system_error(std::make_error_code(std::errc::operation_not_supported),
                "The operating system does not support modifying file creation time");
        #else
            set_file_time(t, 0);
        #endif
    }

    void Path::set_modify_time(time_point t, [[maybe_unused]] flag flags) const {
        #ifdef _XOPEN_SOURCE
            set_file_times(access_time(), t, flags);
        #else
            set_file_time(t, 2);
        #endif
    }

    // I/O functions

    void Path::load(std::string& str, size_t maxlen, flag flags) const {
        static constexpr size_t block_size = 16384;
        FILE* in = nullptr;
        auto guard = on_scope_exit([&] { if (in != nullptr && in != stdin) fclose(in); });
        if (!! (flags & flag::stdio) && (filename_.empty() || filename_ == OS_CHAR("-"))) {
            in = stdin;
        } else {
            in = OS_FUNCTION(fopen)(c_name(), OS_CHAR("rb"));
            if (! in) {
                if (! (flags & flag::may_fail))
                    throw std::system_error(std::make_error_code(std::errc::io_error), name());
                str.clear();
                return;
            }
        }
        str.clear();
        while (str.size() < maxlen) {
            size_t ofs = str.size(), n = std::min(maxlen - ofs, block_size);
            str.append(n, '\0');
            size_t rc = ::fread(&str[0] + ofs, 1, n, in);
            str.resize(ofs + rc);
            if (rc < n)
                break;
        }
    }

    void Path::save(const std::string& str, flag flags) const {
        static constexpr flag options = flag::append | flag::overwrite;
        if ((flags & options) == options)
            throw std::invalid_argument("Invalid options to Path::save()");
        FILE* out = nullptr;
        auto guard = on_scope_exit([&] { if (out != nullptr && out != stdout) fclose(out); });
        if (!! (flags & flag::stdio) && (filename_.empty() || filename_ == OS_CHAR("-"))) {
            out = stdout;
        } else {
            if (! (flags & options) && exists())
                throw std::system_error(std::make_error_code(std::errc::file_exists), name());
            out = OS_FUNCTION(fopen)(c_name(), !! (flags & flag::append) ? OS_CHAR("ab") : OS_CHAR("wb"));
            if (! out)
                throw std::system_error(std::make_error_code(std::errc::io_error), name());
        }
        size_t pos = 0;
        while (pos < str.size()) {
            errno = 0;
            pos += ::fwrite(str.data() + pos, 1, str.size() - pos, out);
            if (errno)
                throw std::system_error(std::make_error_code(std::errc::io_error), name());
        }
    }

    // Process state functions

    void Path::change_directory() const {
        if (OS_FUNCTION(chdir)(c_name()) == -1) {
            int err = errno;
            throw std::system_error(err, std::generic_category(), name());
        }
    }

    Path Path::current_directory() {
        return get_current_directory(true);
    }

    // Implementation details

    std::pair<Path::string_type, Path::string_type> Path::get_base_ext() const noexcept {
        if (filename_.empty())
            return {filename_, filename_};
        size_t start = filename_.find_last_of(native_delimiter);
        if (start != npos)
            ++start;
        #ifndef _XOPEN_SOURCE
            else if (filename_.size() >= 2 && filename_[1] == L':')
                start = 2;
        #endif
        else
            start = 0;
        size_t dot = npos;
        if (start < filename_.size()) {
                dot = filename_.find_last_of(OS_CHAR('.'));
                if (dot <= start || dot >= filename_.size() - 1)
                dot = npos;
            }
        auto base = filename_.substr(start, dot - start);
        string_type ext;
        if (dot < filename_.size())
            ext = filename_.substr(dot);
        return {base, ext};
    }

    Path::string_type Path::get_leaf() const noexcept {
        if (filename_.empty())
            return filename_;
        size_t start = filename_.find_last_of(native_delimiter);
        if (start != npos)
            ++start;
        #ifndef _XOPEN_SOURCE
            else if (filename_.size() >= 2 && filename_[1] == L':')
                start = 2;
        #endif
        else
            start = 0;
        return filename_.substr(start);
    }

    Path::string_type Path::get_root([[maybe_unused]] bool allow_drive_special) const noexcept {
        size_t pos = 0;
        if (! filename_.empty()) {
            #ifdef _XOPEN_SOURCE
                // Posix: /path
                pos = filename_.find_first_not_of('/');
            #else
                // Windows: [\\?\]drive:\path or [\\?\]\\server\path
                // if allow_drive_special: [\\?\]\path or [\\?\]drive:path
                static const Regex pattern1(R"(
                    ^ ( \\{2}\?\\ ) ?
                    ( [A-Z] :\\ | \\{2,} [^?\\]+ \\? )
                )", Regex::extended | Regex::icase | Regex::no_capture | Regex::optimize);
                static const Regex pattern2(R"(
                    ^ ( \\{2}\?\\ ) ?
                    ( [A-Z] :\\? | \\{2,} [^?\\]+ \\? | \\+ )
                )", Regex::extended | Regex::icase | Regex::no_capture | Regex::optimize);
                auto& pattern = allow_drive_special ? pattern2 : pattern1;
                auto ascii_name = dumb_ascii_conversion(filename_);
                auto match = pattern(ascii_name);
                if (match)
                    pos = match.count();
            #endif
        }
        return filename_.substr(0, pos);
    }

    void Path::make_canonical(flag flags) {
        static const string_type ss = {native_delimiter, native_delimiter};
        static const string_type sds = {native_delimiter, OS_CHAR('.'), native_delimiter};
        #ifndef _XOPEN_SOURCE
            // Replace slash delimiters
            std::replace(filename_.begin(), filename_.end(), L'/', L'\\');
        #endif
        // Trim redundant leading slashes
        size_t p = filename_.find_first_not_of(native_delimiter);
        if (p == npos)
            p = filename_.size();
        #ifdef _XOPEN_SOURCE
            if (p > 1)
                filename_.erase(0, p - 1);
        #else
            if (p > 2)
                filename_.erase(0, p - 2);
        #endif
        // Replace /./ with /
        p = 0;
        for (;;) {
            p = filename_.find(sds, p);
            if (p == npos)
                break;
            filename_.erase(p, 2);
        }
        // Replace multiple slashes with one
        #ifdef _XOPEN_SOURCE
            p = filename_.find_first_not_of('/');
        #else
            p = filename_.find_first_not_of(L"?\\");
        #endif
        while (p < filename_.size()) {
            p = filename_.find(ss, p);
            if (p == npos)
                break;
            size_t q = filename_.find_first_not_of(native_delimiter, p + 2);
            if (q == npos)
                q = filename_.size();
            filename_.erase(p + 1, q - p - 1);
            ++p;
        }
        // Trim trailing / and /.
        size_t root_size = get_root(true).size();
        size_t min_root = std::max(root_size, size_t(1));
        while (filename_.size() > min_root && (filename_.back() == native_delimiter || (filename_.back() == OS_CHAR('.')
                && filename_.end()[-2] == native_delimiter)))
            filename_.pop_back();
        #ifndef _XOPEN_SOURCE
            // Ensure a trailing slash on network paths
            if (filename_.size() >= 3 && filename_[0] == L'\\' && filename_[1] == L'\\' && filename_.find(L'\\', 2) == npos)
                filename_ += L'\\';
            // Set the drive letter to upper case
            size_t drive = npos;
            if (root_size >= 3 && filename_[root_size - 1] == L'\\')
                drive = root_size - 3;
            else if (root_size >= 2 && filename_[root_size - 1] == L':')
                drive = root_size - 2;
            if (drive != npos && filename_[drive] >= L'a' && filename_[drive] <= L'z' && filename_[drive + 1] == L':')
                filename_[drive] -= 0x20;
        #endif
        // Validate if requested
        if (!! (flags & flag::legal_name) && ! is_legal())
            throw std::invalid_argument("Invalid file name: " + quote(name()));
    }

    #if defined(__APPLE__) && __MAC_OS_X_VERSION_MAX_ALLOWED < 101300

        void Path::set_file_times(time_point atime, time_point mtime, int /*flags*/) const {
            timeval times[2];
            timepoint_to_timeval(atime, times[0]);
            timepoint_to_timeval(mtime, times[1]);
            errno = 0;
            int rc = utimes(c_name(), times);
            int err = errno;
            if (rc == -1)
                throw std::system_error(err, std::generic_category(), name());
        }

    #elif defined(_XOPEN_SOURCE)

        void Path::set_file_times(time_point atime, time_point mtime, flag flags) const {
            timespec times[2];
            timepoint_to_timespec(atime, times[0]);
            timepoint_to_timespec(mtime, times[1]);
            int utflags = !! (flags & flag::no_follow) ? AT_SYMLINK_NOFOLLOW : 0;
            errno = 0;
            int rc = utimensat(AT_FDCWD, c_name(), times, utflags);
            int err = errno;
            if (rc == -1)
                throw std::system_error(err, std::generic_category(), name());
        }

    #else

        Path::time_point Path::get_file_time(int index) const noexcept {
            auto fh = CreateFileW(c_name(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (! fh)
                return {};
            auto guard = on_scope_exit([=] { CloseHandle(fh); });
            FILETIME ft[3];
            if (! GetFileTime(fh, ft, ft + 1, ft + 2))
                return {};
            time_point tp;
            filetime_to_timepoint(ft[index], tp);
            return tp;
        }

        void Path::set_file_time(time_point t, int index) const {
            SetLastError(0);
            auto fh = CreateFileW(c_name(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            auto err = GetLastError();
            if (! fh)
                throw std::system_error(err, std::system_category(), name());
            auto guard = on_scope_exit([=] { CloseHandle(fh); });
            FILETIME ft;
            timepoint_to_filetime(t, ft);
            FILETIME* fts[3];
            std::memset(fts, 0, sizeof(fts));
            fts[index] = &ft;
            auto rc = SetFileTime(fh, fts[0], fts[1], fts[2]);
            err = GetLastError();
            if (! rc)
                throw std::system_error(err, std::system_category(), name());
        }

    #endif

    // Comparison objects

    bool Path::equal::operator()(const Path& lhs, const Path& rhs) const {
        if (cmode == cmp::icase)
            return AsciiIcaseEqual()(lhs.name(), rhs.name());
        else
            return lhs == rhs;
    }

    bool Path::less::operator()(const Path& lhs, const Path& rhs) const {
        if (cmode == cmp::icase)
            return AsciiIcaseLess()(lhs.name(), rhs.name());
        else
            return lhs < rhs;
    }

    // Directory iterator

    struct Path::directory_iterator::impl_type {
        Path current;
        Path prefix;
        string_type leaf;
        flag flags = {};
        #ifdef _XOPEN_SOURCE
            DIR* dirptr = nullptr;
            dirent entry;
            char padding[NAME_MAX + 1];
            ~impl_type() { if (dirptr) closedir(dirptr); }
        #else
            HANDLE handle = nullptr;
            WIN32_FIND_DATAW info;
            bool first;
            ~impl_type() { if (handle) FindClose(handle); }
        #endif
    };

    Path::directory_iterator::directory_iterator(const Path& dir, flag flags) {
        if (!! (flags & flag::unicode) && ! dir.is_unicode())
            return;
        #ifdef _XOPEN_SOURCE
            impl_ = std::make_shared<impl_type>();
            memset(&impl_->entry, 0, sizeof(impl_->entry));
            memset(impl_->padding, 0, sizeof(impl_->padding));
            if (dir.empty())
                impl_->dirptr = opendir(".");
            else
                impl_->dirptr = opendir(dir.c_name());
            if (! impl_->dirptr)
                impl_.reset();
        #else
            // We need to check first that the supplied file name refers to a
            // directory, because FindFirstFile() gives a false positive for
            // "file/*" when the file exists but is not a directory. There's a
            // race condition here but there doesn't seem to be anything we
            // can do about it.
            if (! dir.empty() && ! dir.is_root() && ! dir.is_directory())
                return;
            impl_ = std::make_shared<impl_type>();
            memset(&impl_->info, 0, sizeof(impl_->info));
            impl_->first = true;
            std::wstring glob = (dir / L"*").os_name();
            impl_->handle = FindFirstFileW(glob.data(), &impl_->info);
            if (! impl_->handle)
                impl_.reset();
        #endif
        if (! impl_)
            return;
        impl_->prefix = dir;
        impl_->flags = flags;
        ++*this;
    }

    const Path& Path::directory_iterator::operator*() const noexcept {
        return impl_->current;
    }

    Path::directory_iterator& Path::directory_iterator::operator++() {
        static const string_type dot1(1, OS_CHAR('.'));
        static const string_type dot2(2, OS_CHAR('.'));
        const bool skip_hidden = !! (impl_->flags & flag::no_hidden);
        while (impl_) {
            #ifdef _XOPEN_SOURCE
                dirent* entptr = nullptr;
                #ifdef __GNUC__
                    #pragma GCC diagnostic push
                    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
                #endif
                int rc = readdir_r(impl_->dirptr, &impl_->entry, &entptr);
                #ifdef __GNUC__
                    #pragma GCC diagnostic pop
                #endif
                bool ok = rc == 0 && entptr;
                if (ok)
                    impl_->leaf = impl_->entry.d_name;
            #else
                bool ok = impl_->first || FindNextFile(impl_->handle, &impl_->info);
                impl_->first = false;
                if (ok)
                    impl_->leaf = impl_->info.cFileName;
            #endif
            if (! ok) {
                impl_.reset();
                break;
            }
            if (impl_->leaf != dot1 && impl_->leaf != dot2
                    && (! (impl_->flags & flag::unicode) || is_valid_utf(impl_->leaf))) {
                impl_->current = impl_->prefix / impl_->leaf;
                if (! skip_hidden || ! impl_->current.is_hidden())
                    break;
            }
        }
        return *this;
    }

    // Deep search iterator

    struct Path::search_iterator::impl_type {
        std::vector<directory_range> stack;
        flag flagset = {};
        bool revisit = false;
    };

    Path::search_iterator::search_iterator(const Path& dir, flag flags) {
        if (! dir.is_directory(flags))
            return;
        auto range = dir.directory(flags);
        if (range.empty())
            return;
        impl_ = std::make_shared<impl_type>();
        impl_->stack.push_back(range);
        impl_->flagset = flags;
        if (!! (impl_->flagset & flag::bottom_up)) {
            for (;;) {
                range = (**this).directory(impl_->flagset);
                if (range.empty())
                    break;
                impl_->stack.push_back(range);
            }
        }
        impl_->revisit = !! (impl_->flagset & flag::bottom_up) && ! impl_->stack.empty() && (**this).is_directory();
        if (impl_->stack.empty())
            impl_.reset();
    }

    const Path& Path::search_iterator::operator*() const noexcept {
        return *impl_->stack.back().first;
    }

    Path::search_iterator& Path::search_iterator::operator++() {
        do {
            if ((**this).is_directory(impl_->flagset) && ! impl_->revisit) {
                auto range = (**this).directory(impl_->flagset);
                impl_->revisit = range.empty();
                if (! impl_->revisit)
                    impl_->stack.push_back(range);
            } else {
                ++impl_->stack.back().first;
                impl_->revisit = impl_->stack.back().empty();
                if (impl_->revisit)
                    impl_->stack.pop_back();
            }
        } while (! impl_->stack.empty() && (**this).is_directory(impl_->flagset) && impl_->revisit != !! (impl_->flagset & flag::bottom_up));
        if (impl_->stack.empty())
            impl_.reset();
        return *this;
    }

}
