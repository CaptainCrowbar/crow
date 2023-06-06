#include "crow/path.hpp"
#include "crow/binary.hpp"
#include "crow/guard.hpp"
#include "crow/regex.hpp"
#include "crow/string.hpp"
#include "crow/time.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <stdio.h>
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

using namespace Crow::Literals;
using namespace std::chrono;
using namespace std::literals;

namespace Crow {

    namespace {

        #ifdef _XOPEN_SOURCE

            struct StatResult {
                struct stat st;
                bool ok;
            };

            StatResult get_stat(const std::string& file, Path::flag_type flags) noexcept {
                StatResult result;
                result.ok = (has_bit(flags, Path::no_follow) ? lstat : stat)(file.data(), &result.st) == 0;
                if (! result.ok)
                    memset(&result.st, 0, sizeof(result.st));
                return result;
            }

            Path get_current_directory(bool checked) {

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

            }

            std::string get_root(std::string_view filename, bool /*allow_drive_special*/) noexcept {
                size_t pos = 0;
                if (! filename.empty())
                    pos = filename.find_first_not_of('/');
                return std::string(filename, 0, pos);
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

            bool legal_file_name(std::string_view filename) noexcept {
                return filename.find(Path::os_char(0)) == npos;
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

            Path get_current_directory(bool checked) {

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

            }

            std::wstring get_root(std::wstring_view filename, bool allow_drive_special) noexcept {

                // [\\?\]drive:\path or [\\?\]\\server\path
                // if allow_drive_special: [\\?\]\path or [\\?\]drive:path

                if (filename.empty())
                    return {};

                static constexpr auto regex_flags = Regex::extended | Regex::icase | Regex::no_capture;

                static const Regex pattern1(R"(
                    ^ ( \\{2}\?\\ ) ?
                    ( [A-Z] :\\ | \\{2,} [^?\\]+ \\? )
                )", regex_flags);

                static const Regex pattern2(R"(
                    ^ ( \\{2}\?\\ ) ?
                    ( [A-Z] :\\? | \\{2,} [^?\\]+ \\? | \\+ )
                )", regex_flags);

                auto& pattern = allow_drive_special ? pattern2 : pattern1;
                auto ascii_name = simple_ascii_conversion(filename);
                auto match = pattern(ascii_name);
                size_t pos = 0;

                if (match)
                    pos = match.count();

                return std::wstring(filename, 0, pos);

            }

            std::string make_utf8(std::wstring_view wstr) {
                return to_utf8(decode_string(wstr));
            }

            std::string simple_ascii_conversion(std::wstring_view wstr) {
                std::string ascii(wstr.size(), '\0');
                std::transform(wstr.begin(), wstr.end(), ascii.begin(),
                    [] (wchar_t w) { return w <= 127 ? char(w) : '_'; });
                return ascii;
            }

            bool legal_file_name(std::wstring_view filename) noexcept {
                if (filename.find(os_char(0)) != npos)
                    return false;
                auto root = get_root(filename, true);
                if (root.empty() && filename[0] == L'\\')
                    return false;
                else
                    return filename.find_first_of(L"\"*:<>?|", root.size()) == npos;
            }

        #endif

        std::string make_utf8(std::string_view str) {
            return std::string(str);
        }

        Path::os_string make_native_string(std::string_view utf8) {
            #ifdef _XOPEN_SOURCE
                return std::string(utf8);
            #else
                return to_wstring(decode_string(utf8));
            #endif
        }

    }

    // Constructors

    #ifdef _XOPEN_SOURCE

        Path::Path(std::string_view file, flag_type flags):
        filename_(file) {
            make_canonical(flags);
        }

    #else

        Path::Path(std::string_view file, flag_type flags):
        filename_(to_wstring(decode_string(file))) {
            make_canonical(flags);
        }

        Path::Path(std::wstring_view file, flag_type flags):
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

        #ifdef _WIN32
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

    std::vector<Path::os_string> Path::os_breakdown() const {

        std::vector<os_string> parts;

        if (is_empty())
            return parts;

        auto root = get_root(filename_, true);

        if (! root.empty())
            parts.push_back(root);

        size_t p = root.size(), q = 0, size = filename_.size();

        while (p < size) {
            q = filename_.find(os_delimiter, p);
            if (q == npos)
                q = size;
            parts.push_back(filename_.substr(p, q - p));
            p = q + 1;
        }

        return parts;

    }

    Path Path::change_ext(std::string_view new_ext) const {

        if (is_empty() || get_root(filename_, true).size() == filename_.size())
            throw std::invalid_argument("Can't change file extension: " + quote(name()));

        os_string prefix = filename_;
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
            )", Regex::extended | Regex::icase | Regex::no_capture);

            auto ascii_name = simple_ascii_conversion(filename_);

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
        return ! is_empty() && ! is_absolute() && ! is_drive_absolute() && ! is_drive_relative();
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
        return legal_file_name(filename_);
    }

    bool Path::is_root() const noexcept {
        return ! filename_.empty() && get_root(filename_, false).size() == filename_.size();
    }

    Path::form Path::path_form() const noexcept {
        if (filename_.empty())         return form::empty;
        else if (is_drive_absolute())  return form::drive_absolute;
        else if (is_drive_relative())  return form::drive_relative;
        else if (is_absolute())        return form::absolute;
        else                           return form::relative;
    }

    Path Path::relative_to(const Path& base, flag_type flags) const {

        auto base_form = base.path_form();
        auto this_form = path_form();

        if (! (base_form == form::absolute && this_form == form::absolute)
                && ! (base_form == form::relative && this_form == form::relative))
            throw std::invalid_argument("Invalid combination of arguments to Path::relative_to()");

        auto base_vec = base.os_breakdown(), this_vec = os_breakdown();

        if (this_form == form::absolute && base_vec[0] != this_vec[0])
            return *this;

        Path prefix;
        auto cuts = std::mismatch(base_vec.begin(), base_vec.end(), this_vec.begin(), this_vec.end());

        if (base_vec.end() > cuts.first) {
            if (! has_bit(flags, no_backtrack)) {
                size_t len = 3 * (base_vec.end() - cuts.first) - 1;
                prefix.filename_.assign(len, OS_CHAR('.'));
                for (size_t i = 2; i < len; i += 3)
                    prefix.filename_[i] = os_delimiter;
            } else if (base_form == form::relative) {
                throw std::invalid_argument("Invalid combination of arguments to Path::relative_to()");
            } else {
                return *this;
            }
        }

        Path suffix = join(irange(cuts.second, this_vec.end()));
        Path rel_path = prefix / suffix;

        if (rel_path.is_empty())
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

    std::pair<Path::os_string, Path::os_string> Path::split_os_leaf() const {
        return get_base_ext();
    }

    std::pair<Path, Path> Path::split_path() const {
        auto leaf = get_leaf();
        auto dir = filename_.substr(0, filename_.size() - leaf.size());
        return {dir, leaf};
    }

    std::pair<Path, Path> Path::split_root() const {
        auto root = get_root(filename_, true);
        auto tail = filename_.substr(root.size());
        return {root, tail};
    }

    Path Path::common(const Path& lhs, const Path& rhs) {

        auto root1 = get_root(lhs.filename_, true);
        auto root2 = get_root(rhs.filename_, true);

        if (root1 != root2)
            return {};

        size_t cut = std::mismatch(lhs.filename_.begin(), lhs.filename_.end(),
            rhs.filename_.begin(), rhs.filename_.end()).first - lhs.filename_.begin();

        if (cut == root1.size())
            return lhs.filename_.substr(0, cut);
        else if ((cut == lhs.filename_.size() || lhs.filename_[cut] == os_delimiter)
                && (cut == rhs.filename_.size() || rhs.filename_[cut] == os_delimiter))
            return lhs.filename_.substr(0, cut);

        do --cut;
            while (cut > root1.size() && lhs.filename_[cut] != os_delimiter);

        return lhs.filename_.substr(0, cut);

    }

    Path Path::join(const Path& lhs, const Path& rhs) {
        if (lhs.is_empty() || rhs.is_absolute())
            return rhs;
        auto result = lhs.os_name();
        auto root = get_root(lhs.filename_, true);
        if (root.size() < result.size() && result.back() != os_delimiter && ! rhs.is_empty())
            result += os_delimiter;
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

    std::ostream& operator<<(std::ostream& out, Path::kind k) {
        switch (k) {
            case Path::kind::none:       out << "none"; break;
            case Path::kind::directory:  out << "directory"; break;
            case Path::kind::file:       out << "file"; break;
            case Path::kind::special:    out << "special"; break;
            case Path::kind::symlink:    out << "symlink"; break;
            default:                     out << int(k); break;
        }
        return out;
    }

    std::strong_ordering operator<=>(const Path& lhs, const Path& rhs) noexcept {
        if (lhs.filename_ == rhs.filename_)
            return std::strong_ordering::equal;
        else if (lhs.filename_ < rhs.filename_)
            return std::strong_ordering::less;
        else
            return std::strong_ordering::greater;
    }

    // File system query functions

    Path::directory_range Path::directory(flag_type flags) const {
        directory_iterator it(*this, flags);
        return {it, {}};
    }

    Path::search_range Path::deep_search(flag_type flags) const {
        search_iterator it(*this, flags);
        return {it, {}};
    }

    bool Path::exists([[maybe_unused]] flag_type flags) const noexcept {
        #ifdef _XOPEN_SOURCE
            return get_stat(filename_, flags).ok;
        #else
            return get_attributes(filename_);
        #endif
    }

    Path::id_type Path::id([[maybe_unused]] flag_type flags) const noexcept {

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

    Path::kind Path::file_kind([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef _XOPEN_SOURCE

            auto [st,ok] = get_stat(filename_, flags);
            if (! ok)
                return kind::none;
            else if (has_bit(flags, no_follow) && S_ISLNK(st.st_mode))
                return kind::symlink;
            else if (S_ISDIR(st.st_mode))
                return kind::directory;
            else if (S_ISREG(st.st_mode))
                return kind::file;
            else
                return kind::special;

        #else

            auto attr = get_attributes(filename_);
            if (attr == 0)
                return kind::none;
            else if (has_bit(attr, FILE_ATTRIBUTE_DIRECTORY))
                return kind::directory;
            else if (has_bit(attr, FILE_ATTRIBUTE_DEVICE))
                return kind::special;
            else
                return kind::file;

        #endif

    }

    bool Path::is_directory([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef _XOPEN_SOURCE

            auto st = get_stat(filename_, flags).st;
            return S_ISDIR(st.st_mode);

        #else

            return get_attributes(filename_) & FILE_ATTRIBUTE_DIRECTORY;

        #endif

    }

    bool Path::is_file([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef _XOPEN_SOURCE

            auto st = get_stat(filename_, flags).st;
            return S_ISREG(st.st_mode);

        #else

            auto attr = get_attributes(filename_);
            return attr && ! (attr & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY));

        #endif

    }

    bool Path::is_special([[maybe_unused]] flag_type flags) const noexcept {

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

            return ! is_root() && has_bit(get_attributes(filename_), hidden_attr);

        #endif

    }

    bool Path::is_symlink() const noexcept {

        #ifdef _XOPEN_SOURCE

            auto st = get_stat(filename_, no_follow).st;
            return S_ISLNK(st.st_mode);

        #else

            return false;

        #endif

    }

    Path Path::resolve() const {

        Path result;
        if (is_empty()) {

            result = get_current_directory(false);

        } else {

            #ifdef _XOPEN_SOURCE

                if (filename_[0] == '~') {
                    size_t pos = filename_.find('/');
                    std::string tail, user = filename_.substr(1, pos - 1);
                    if (pos != npos)
                        tail = filename_.substr(pos + 1, npos);
                    Path home = get_user_home(user);
                    if (! home.is_empty())
                        result = home / tail;
                }

                if (result.is_empty() && is_relative()) {
                    Path cwd = get_current_directory(false);
                    if (! cwd.is_empty())
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

        if (result.is_empty())
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

    uint64_t Path::size(flag_type flags) const {

        #ifdef _XOPEN_SOURCE

            auto st = get_stat(filename_, flags).st;
            uint64_t bytes = st.st_size;

        #else

            auto info = get_attributes_ex(filename_);
            uint64_t bytes = (uint64_t(info.nFileSizeHigh) << 32) + uint64_t(info.nFileSizeLow);

        #endif

        if (has_bit(flags, recurse))
            for (auto& child: directory())
                bytes += child.size(no_follow | recurse);

        return bytes;

    }

    // File system update functions

    void Path::copy_to(const Path& dst, flag_type flags) const {

        static constexpr size_t block_size = 16384;

        if (! exists())
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), name());

        if (*this == dst || id() == dst.id())
            throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());

        if (is_directory() && ! has_bit(flags, recurse))
            throw std::system_error(std::make_error_code(std::errc::is_a_directory), name());

        if (dst.exists()) {
            if (! has_bit(flags, overwrite))
                throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());
            dst.remove(recurse);
        }

        if (is_symlink()) {

            resolve_symlink().make_symlink(dst);

        } else if (is_directory()) {

            dst.make_directory();

            for (auto& child: directory())
                child.copy_to(dst / child.leaf(), recurse);

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

    void Path::make_directory(flag_type flags) const {

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
            if (! has_bit(flags, overwrite))
                throw std::system_error(err, std::generic_category(), name());
            remove();

        } else if (err == ENOENT && has_bit(flags, recurse) && ! is_empty()) {

            Path p = parent();
            if (p == *this)
                throw std::system_error(err, std::generic_category(), name());
            p.make_directory(flags);

        }

        if (mkdir_call(*this) != 0) {
            err = errno;
            throw std::system_error(err, std::generic_category(), name());
        }

    }

    void Path::make_symlink(const Path& linkname, flag_type flags) const {

        #ifdef _XOPEN_SOURCE

            if (linkname.is_symlink()) {
                try {
                    if (linkname.resolve_symlink() == *this)
                        return;
                }
                catch (const std::system_error&) {}
            }

            if (has_bit(flags, overwrite) && linkname.exists())
                linkname.remove(flags);

            if (symlink(c_name(), linkname.c_name()) == 0)
                return;

            if (! has_bit(flags, may_copy)) {
                int err = errno;
                throw std::system_error(err, std::generic_category(), linkname.name() + " -> " + name());
            }

            copy_to(linkname, recurse);

        #else

            if (has_bit(flags, may_copy))
                copy_to(linkname, recurse);
            else
                throw std::system_error(std::make_error_code(std::errc::operation_not_supported),
                    "Symbolic links are not supported on Windows");

        #endif

    }

    void Path::move_to(const Path& dst, flag_type flags) const {

        if (! exists())
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), name());

        if (*this == dst)
            return;

        if (dst.exists() && id() != dst.id()) {
            if (! has_bit(flags, overwrite))
                throw std::system_error(std::make_error_code(std::errc::file_exists), dst.name());
            dst.remove(recurse);
        }

        if (OS_FUNCTION(rename)(c_name(), dst.c_name()) == 0)
            return;

        int err = errno;

        if (err != EXDEV || ! has_bit(flags, may_copy))
            throw std::system_error(err, std::generic_category(), name() + " -> " + dst.name());

        copy_to(dst, recurse);
        remove(recurse);

    }

    void Path::remove(flag_type flags) const {

        if (has_bit(flags, recurse) && is_directory() && ! is_symlink())
            for (auto child: directory())
                child.remove(recurse);

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

    // File system query/update functions

    Path::time_point Path::access_time([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef __APPLE__

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_atimespec, tp);
            return tp;

        #elif defined(_XOPEN_SOURCE)

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_atim, tp);
            return tp;

        #else

            return get_file_time(1);

        #endif

    }

    Path::time_point Path::create_time([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef __APPLE__

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_birthtimespec, tp);
            return tp;

        #elif defined(_XOPEN_SOURCE)

            return {};

        #else

            return get_file_time(0);

        #endif

    }

    Path::time_point Path::modify_time([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef __APPLE__

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_mtimespec, tp);
            return tp;

        #elif defined(_XOPEN_SOURCE)

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_mtim, tp);
            return tp;

        #else

            return get_file_time(2);

        #endif

    }

    Path::time_point Path::status_time([[maybe_unused]] flag_type flags) const noexcept {

        #ifdef __APPLE__

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_ctimespec, tp);
            return tp;

        #elif defined(_XOPEN_SOURCE)

            auto st = get_stat(filename_, flags).st;
            time_point tp;
            timespec_to_timepoint(st.st_ctim, tp);
            return tp;

        #else

            return {};

        #endif

    }

    void Path::set_access_time(time_point t, [[maybe_unused]] flag_type flags) const {
        #ifdef _XOPEN_SOURCE
            set_file_times(t, modify_time(), flags);
        #else
            set_file_time(t, 1);
        #endif
    }

    void Path::set_create_time([[maybe_unused]] time_point t, flag_type /*flags*/) const {
        #ifdef _XOPEN_SOURCE
            throw std::system_error(std::make_error_code(std::errc::operation_not_supported),
                "The operating system does not support modifying file creation time");
        #else
            set_file_time(t, 0);
        #endif
    }

    void Path::set_modify_time(time_point t, [[maybe_unused]] flag_type flags) const {
        #ifdef _XOPEN_SOURCE
            set_file_times(access_time(), t, flags);
        #else
            set_file_time(t, 2);
        #endif
    }

    void Path::set_status_time(time_point /*t*/, flag_type /*flags*/) const {
        throw std::system_error(std::make_error_code(std::errc::operation_not_supported),
            "The operating system does not support modifying file status time");
    }

    // I/O functions

    std::string Path::load(size_t maxlen, flag_type flags) const {
        std::string content;
        load(content, maxlen, flags & ~ append);
        return content;
    }

    void Path::load(std::string& content, size_t maxlen, flag_type flags) const {

        static constexpr size_t block_size = 16384;

        FILE* in = nullptr;

        auto guard = on_scope_exit([&] {
            if (in != nullptr && in != stdin)
                fclose(in);
        });

        if (has_bit(flags, stdio) && (filename_.empty() || filename_ == OS_CHAR("-"))) {
            in = stdin;
        } else {
            in = OS_FUNCTION(fopen)(c_name(), OS_CHAR("rb"));
            if (! in) {
                if (! has_bit(flags, may_fail))
                    throw std::system_error(std::make_error_code(std::errc::io_error), name());
                content.clear();
                return;
            }
        }

        if (! has_bit(flags, append))
            content.clear();
        else if (maxlen != npos)
            maxlen += content.size();

        while (content.size() < maxlen) {
            size_t ofs = content.size();
            size_t n = std::min(maxlen - ofs, block_size);
            content.append(n, '\0');
            size_t rc = ::fread(&content[0] + ofs, 1, n, in);
            content.resize(ofs + rc);
            if (rc < n)
                break;
        }

    }

    void Path::save(std::string_view content, flag_type flags) const {

        static constexpr flag_type options = append | overwrite;

        if (has_bits(flags, options))
            throw std::invalid_argument("Invalid options to Path::save()");

        FILE* out = nullptr;
        auto guard = on_scope_exit([&] { if (out != nullptr && out != stdout) fclose(out); });

        if (has_bit(flags, stdio) && (filename_.empty() || filename_ == OS_CHAR("-"))) {
            out = stdout;
        } else {
            if (! has_bit(flags, options) && exists())
                throw std::system_error(std::make_error_code(std::errc::file_exists), name());
            out = OS_FUNCTION(fopen)(c_name(), has_bit(flags, append) ? OS_CHAR("ab") : OS_CHAR("wb"));
            if (! out)
                throw std::system_error(std::make_error_code(std::errc::io_error), name());
        }

        size_t pos = 0;

        while (pos < content.size()) {
            errno = 0;
            pos += ::fwrite(content.data() + pos, 1, content.size() - pos, out);
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

    std::pair<Path::os_string, Path::os_string> Path::get_base_ext() const noexcept {

        if (filename_.empty())
            return {filename_, filename_};

        size_t start = filename_.find_last_of(os_delimiter);

        if (start != npos)
            ++start;

        #ifdef _WIN32
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
        os_string ext;

        if (dot < filename_.size())
            ext = filename_.substr(dot);

        return {base, ext};

    }

    Path::os_string Path::get_leaf() const noexcept {

        if (filename_.empty())
            return filename_;

        size_t start = filename_.find_last_of(os_delimiter);

        if (start != npos)
            ++start;

        #ifdef _WIN32
            else if (filename_.size() >= 2 && filename_[1] == L':')
                start = 2;
        #endif

        else
            start = 0;

        return filename_.substr(start);

    }

    void Path::make_canonical(flag_type flags) {

        static const os_string ss = {os_delimiter, os_delimiter};
        static const os_string sds = {os_delimiter, OS_CHAR('.'), os_delimiter};

        // Check legality

        if (has_bit(flags, legal_name) && ! is_legal())
            throw std::invalid_argument("Invalid file name: " + quote(name()));

        if (has_bit(flags, unicode) && ! is_unicode())
            throw std::invalid_argument("Invalid Unicode file name: " + quote(name()));

        // Replace slash delimiters

        #ifdef _WIN32
            std::replace(filename_.begin(), filename_.end(), L'/', L'\\');
        #endif

        // Trim redundant leading slashes

        size_t p = filename_.find_first_not_of(os_delimiter);

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
            size_t q = filename_.find_first_not_of(os_delimiter, p + 2);
            if (q == npos)
                q = filename_.size();
            filename_.erase(p + 1, q - p - 1);
            ++p;
        }

        // Trim trailing / and /.

        size_t root_size = get_root(filename_, true).size();
        size_t min_root = std::max(root_size, 1_uz);

        while (filename_.size() > min_root && (filename_.back() == os_delimiter || (filename_.back() == OS_CHAR('.')
                && filename_.end()[-2] == os_delimiter)))
            filename_.pop_back();

        #ifdef _WIN32

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

        void Path::set_file_times(time_point atime, time_point mtime, flag_type flags) const {
            timespec times[2];
            timepoint_to_timespec(atime, times[0]);
            timepoint_to_timespec(mtime, times[1]);
            int utflags = has_bit(flags, no_follow) ? AT_SYMLINK_NOFOLLOW : 0;
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
        os_string leaf;
        flag_type flags = no_flags;

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

    Path::directory_iterator::directory_iterator(const Path& dir, flag_type flags) {

        if (has_bit(flags, unicode) && ! dir.is_unicode())
            return;

        #ifdef _XOPEN_SOURCE

            impl_ = std::make_shared<impl_type>();
            memset(&impl_->entry, 0, sizeof(impl_->entry));
            memset(impl_->padding, 0, sizeof(impl_->padding));

            if (dir.is_empty())
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

        static const os_string dot1(1, OS_CHAR('.'));
        static const os_string dot2(2, OS_CHAR('.'));

        const bool skip_hidden = has_bit(impl_->flags, no_hidden);

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
                    && (! has_bit(impl_->flags, unicode) || is_valid_utf(impl_->leaf))) {
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
        flag_type flagset = no_flags;
        bool revisit = false;
    };

    Path::search_iterator::search_iterator(const Path& dir, flag_type flags) {

        if (! dir.is_directory(flags))
            return;

        auto range = dir.directory(flags);

        if (range.empty())
            return;

        impl_ = std::make_shared<impl_type>();
        impl_->stack.push_back(range);
        impl_->flagset = flags;

        if (has_bit(impl_->flagset, bottom_up)) {

            for (;;) {
                range = (**this).directory(impl_->flagset);
                if (range.empty())
                    break;
                impl_->stack.push_back(range);
            }

        }

        impl_->revisit = has_bit(impl_->flagset, bottom_up) && ! impl_->stack.empty() && (**this).is_directory();

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

        } while (! impl_->stack.empty()
            && (**this).is_directory(impl_->flagset)
            && impl_->revisit != has_bit(impl_->flagset, bottom_up));

        if (impl_->stack.empty())
            impl_.reset();

        return *this;

    }

}
