#include "crow/uri.hpp"
#include "crow/format.hpp"
#include "crow/regex.hpp"
#include <algorithm>
#include <stdexcept>
#include <stdexcept>

using namespace Crow::Literals;

namespace Crow {

    namespace {

        template <typename T, typename... Args>
        size_t hash_mix(const T& t, const Args&... args) {
            size_t h = std::hash<T>()(t);
            if constexpr (sizeof...(Args) > 0) {
                size_t h2 = hash_mix(args...);
                h ^= (h << 6) + (h >> 2) + h2 + 0x9e3779b9;
            }
            return h;
        }

    }

    Uri::Uri(const std::string& s) {
        if (! try_parse(s))
            throw std::invalid_argument("Invalid URI: {0:q}"_fmt(s));
    }

    Uri::Uri(const std::string& scheme, const std::string& user, const std::string& password, const std::string& host, uint16_t port,
            const std::string& path, const std::string& query, const std::string& fragment) {
        set_scheme(scheme);
        set_host(host);
        set_user(user);
        set_password(password);
        set_port(port);
        set_path(path);
        set_query(query);
        set_fragment(fragment);
    }

    void Uri::set_scheme(const std::string& new_scheme, bool smart) {

        static const Regex pattern("[a-z][a-z0-9.+-]*(:(//)?)?", Regex::full);

        std::string scheme_text = ascii_lowercase(new_scheme);
        auto match = pattern(scheme_text);
        if (! match)
            throw std::invalid_argument("Invalid URI scheme: {0:q}"_fmt(new_scheme));

        if (! match.matched(1))
            scheme_text += ':';
        if (smart && ! match.matched(2) && scheme_text != "mailto:")
            scheme_text += "//";

        int delta = int(scheme_text.size()) - ofs_user_;
        text_.replace(0, ofs_user_, scheme_text);
        ofs_user_ += delta;
        ofs_password_ += delta;
        ofs_host_ += delta;
        ofs_port_ += delta;
        ofs_path_ += delta;
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_user(const std::string& new_user) {

        check_nonempty();

        if (new_user.empty()) {
            clear_user();
            return;
        }

        if (! has_host())
            throw std::invalid_argument("Can't set user on URI with no host");

        auto user_text = encode(new_user);
        int delta = int(user_text.size()) - (ofs_password_ - ofs_user_);

        if (has_user()) {
            text_.replace(ofs_user_, ofs_password_ - ofs_user_, user_text);
            ofs_password_ += delta;
            ofs_host_ += delta;
        } else {
            text_.replace(ofs_user_, ofs_password_ - ofs_user_, user_text + '@');
            ++delta;
            ofs_host_ += delta;
            ofs_password_ = ofs_host_ - 1;
        }

        ofs_port_ += delta;
        ofs_path_ += delta;
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_password(const std::string& new_password) {

        check_nonempty();

        if (new_password.empty()) {
            clear_password();
            return;
        }

        if (! has_user())
            throw std::invalid_argument("Can't set password on URI with no user");

        std::string password_text = ':' + encode(new_password) + '@';
        int delta = int(password_text.size()) - (ofs_host_ - ofs_password_);
        text_.replace(ofs_password_, ofs_host_ - ofs_password_, password_text);

        ofs_host_ += delta;
        ofs_port_ += delta;
        ofs_path_ += delta;
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_host(const std::string& new_host) {

        check_nonempty();

        if (new_host.empty()) {
            clear_host();
            return;
        }

        std::string host_text = encode(new_host);
        int delta = int(host_text.size()) - (ofs_port_ - ofs_host_);
        text_.replace(ofs_host_, ofs_port_ - ofs_host_, host_text);

        ofs_port_ += delta;
        ofs_path_ += delta;
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_port(uint16_t new_port) {

        check_nonempty();

        if (new_port == 0) {
            clear_port();
            return;
        }

        if (! has_host())
            throw std::invalid_argument("Can't set port on URI with no host");

        std::string port_text = ':' + std::to_string(new_port);
        text_.replace(ofs_port_, ofs_path_ - ofs_port_, port_text);
        int delta = int(port_text.size()) - (ofs_path_ - ofs_port_);

        ofs_path_ += delta;
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_path(const std::string& new_path) {

        check_nonempty();

        if (new_path.empty()) {
            clear_path();
            return;
        }

        if (new_path.find("//") != npos)
            throw std::invalid_argument("Invalid URI path: {0:q}"_fmt(new_path));

        std::string path_text = encode(new_path, ":/");

        if (! path_text.empty() && path_text[0] != '/')
            path_text.insert(0, 1, '/');

        int delta = int(path_text.size()) - (ofs_query_ - ofs_path_);
        text_.replace(ofs_path_, ofs_query_ - ofs_path_, path_text);

        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::set_query(const std::string& new_query) {

        check_nonempty();

        if (new_query.empty()) {
            clear_query();
            return;
        }

        int delta = int(new_query.size() + 1) - (ofs_fragment_ - ofs_query_);
        std::string q = "?";
        q += new_query;
        text_.replace(ofs_query_, ofs_fragment_ - ofs_query_, q);
        ofs_fragment_ += delta;

    }

    void Uri::set_fragment(const std::string& new_fragment) {

        check_nonempty();

        if (new_fragment.empty()) {
            clear_fragment();
            return;
        }

        text_.replace(ofs_fragment_, npos, '#' + encode(new_fragment));

    }

    void Uri::clear_user() noexcept {
        if (! has_user())
            return;
        int delta = ofs_host_ - ofs_user_;
        text_.erase(ofs_user_, delta);
        ofs_host_ = ofs_password_ = ofs_user_;
        ofs_port_ -= delta;
        ofs_path_ -= delta;
        ofs_query_ -= delta;
        ofs_fragment_ -= delta;
    }

    void Uri::clear_password() noexcept {
        if (! has_password())
            return;
        text_.replace(ofs_password_, ofs_host_ - ofs_password_, "@");
        int delta = ofs_host_ - ofs_password_ - 1;
        ofs_host_ -= delta;
        ofs_port_ -= delta;
        ofs_path_ -= delta;
        ofs_query_ -= delta;
        ofs_fragment_ -= delta;
    }

    void Uri::clear_host() noexcept {
        if (! has_host())
            return;
        int delta = ofs_port_ - ofs_host_;
        text_.erase(ofs_host_, delta);
        ofs_port_ -= delta;
        ofs_path_ -= delta;
        ofs_query_ -= delta;
        ofs_fragment_ -= delta;
    }

    void Uri::clear_port() noexcept {
        if (! has_port())
            return;
        int delta = ofs_path_ - ofs_port_;
        text_.erase(ofs_port_, delta);
        ofs_path_ -= delta;
        ofs_query_ -= delta;
        ofs_fragment_ -= delta;
    }

    void Uri::clear_path() noexcept {
        if (! has_path())
            return;
        int delta = ofs_query_ - ofs_path_;
        text_.erase(ofs_path_, delta);
        ofs_query_ -= delta;
        ofs_fragment_ -= delta;
    }

    void Uri::clear_query() noexcept {
        if (! has_query())
            return;
        text_.erase(ofs_query_, ofs_fragment_ - ofs_query_);
        ofs_fragment_ = ofs_query_;
    }

    void Uri::clear_fragment() noexcept {
        if (has_fragment())
            text_.resize(ofs_fragment_);
    }

    void Uri::append_path(const std::string& new_path) {

        if (new_path.empty())
            return;

        check_nonempty();

        if (new_path[0] == '/' || ! has_path()) {
            set_path(new_path);
            return;
        }

        if (new_path.find("//") != npos)
            throw std::invalid_argument("Invalid URI path: {0:q}"_fmt(new_path));

        std::string path_text = encode(new_path, ":/");

        if (text_[ofs_query_ - 1] != '/')
            path_text.insert(0, 1, '/');

        int delta = int(path_text.size());
        text_.insert(ofs_query_, path_text);
        ofs_query_ += delta;
        ofs_fragment_ += delta;

    }

    void Uri::clear() noexcept {
        text_.clear();
        ofs_user_ = ofs_password_ = ofs_host_ = ofs_port_ = ofs_path_ = ofs_query_ = ofs_fragment_ = 0;
    }

    Uri Uri::doc() const {
        Uri u = *this;
        u.text_.resize(ofs_fragment_);
        return u;
    }

    Uri Uri::base() const {
        Uri u = *this;
        u.text_.resize(ofs_query_);
        u.ofs_fragment_ = u.ofs_query_;
        return u;
    }

    Uri Uri::parent() const {
        Uri u = base();
        if (u.ofs_query_ - u.ofs_path_ > 1) {
            int p = u.ofs_query_ - 1;
            if (u.text_[p] == '/')
                --p;
            while (p > u.ofs_path_ && u.text_[p] != '/')
                --p;
            if (u.text_[p] == '/')
                ++p;
            u.text_.resize(p);
            u.ofs_query_ = u.ofs_fragment_ = p;
        }
        return u;
    }

    Uri Uri::root() const {
        Uri u = *this;
        u.text_.resize(ofs_path_);
        u.ofs_path_ = int(u.text_.size());
        if (ofs_query_ > ofs_path_)
            u.text_ += '/';
        u.ofs_query_ = u.ofs_fragment_ = int(u.text_.size());
        return u;
    }

    size_t Uri::hash() const noexcept {
        return hash_mix(text_, ofs_user_, ofs_password_, ofs_host_, ofs_port_, ofs_path_, ofs_query_, ofs_fragment_);
    }

    std::string Uri::path_dir() const {
        int cut = find_path_cut();
        return decode(text_.substr(ofs_path_, cut - ofs_path_));
    }

    std::string Uri::path_leaf() const {
        int cut = find_path_cut();
        return decode(text_.substr(cut, ofs_query_ - cut));
    }

    bool Uri::try_parse(const std::string& s) {

        // scheme:[//][[user[:password]@]host[:port]][/path][?query][#fragment]

        static const Regex pattern(R"(
            ([a-z][a-z0-9+.-]*:(?://)?)  # scheme
            (?:
                (?:
                    ([^/:@]+)            # user
                    ([^/@]*@)            # password
                )?
                ([^/:@]+)                # host
                (:\d+)?                  # port
            )?
            (/[^#?]*)?                   # path
            (\?[^#]*)?                   # query
            (\#.*)?                      # fragment
        )", Regex::extended | Regex::full | Regex::icase);

        if (std::find_if(s.begin(), s.end(), ascii_iscntrl) != s.end())
            return false;

        auto match = pattern(s);
        if (! match)
            return false;

        text_ = std::string(s);
        ofs_user_ = int(match.count(1));
        ofs_password_ = ofs_user_ + int(match.count(2));
        ofs_host_ = ofs_password_ + int(match.count(3));
        ofs_port_ = ofs_host_ + int(match.count(4));
        ofs_path_ = ofs_port_ + int(match.count(5));
        ofs_query_ = ofs_path_ + int(match.count(6));
        ofs_fragment_ = ofs_query_ + int(match.count(7));

        return true;

    }

    std::string Uri::encode(const std::string& s, const std::string& exempt) {
        auto is_safe = [exempt] (char c) { return ascii_isalnum_w(c) || c == '-' || c == '.' || c == '~' || exempt.find(c) != npos; };
        std::string out;
        auto i = s.begin(), end = s.end();
        while (i != end) {
            auto j = std::find_if_not(i, end, is_safe);
            out.append(i, j);
            if (j == end)
                break;
            out += '%' + hex(uint8_t(*j));
            i = j + 1;
        }
        return out;
    }

    std::string Uri::decode(const std::string& s) {
        size_t i = 0, j = 0, len = s.size();
        if (len < 3)
            return std::string(s);
        std::string out;
        while (i < len) {
            j = s.find('%', i);
            if (j > len - 3) {
                out.append(s, i, npos);
                break;
            }
            out.append(s, i, j - i);
            if (ascii_isxdigit(s[j + 1]) && ascii_isxdigit(s[j + 2])) {
                out += char(to_uint8(s.substr(j + 1, 2), 16));
                i = j + 3;
            } else {
                out += '%';
                i = j + 1;
            }
        }
        return out;
    }

    std::vector<std::pair<std::string, std::string>> Uri::parse_query(const std::string& query, char delimiter) {
        std::vector<std::pair<std::string, std::string>> v;
        if (! delimiter) {
            delimiter = '&';
            size_t pos = query.find_first_of("&;");
            if (pos != npos)
                delimiter = query[pos];
        }
        auto entries = split(query, std::string{delimiter});
        for (auto& entry: entries) {
            auto kv = partition_at(entry, "=");
            v.push_back({decode(kv.first), decode(kv.second)});
        }
        return v;
    }

    void Uri::check_nonempty() const {
        if (empty())
            throw std::invalid_argument("Individual URI elements can't be set on an empty URI");
    }

    int Uri::find_path_cut() const noexcept {
        int i = ofs_query_ - 1;
        while (i >= ofs_path_ && text_[i] != '/')
            --i;
        return i + 1;
    }

    std::strong_ordering operator<(const Uri& u, const Uri& v) noexcept {
        if (u.text_ == v.text_)
            return std::strong_ordering::equal;
        else if (u.text_ < v.text_)
            return std::strong_ordering::less;
        else
            return std::strong_ordering::greater;
    }

}
