#pragma once

#include "crow/io-utility.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <compare>
#include <cstdlib>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace Crow {

    class Uri {

    public:

        static constexpr int lone_keys = 1;

        Uri() = default;
        explicit Uri(const std::string& s);
        Uri(const std::string& scheme, const std::string& user, const std::string& password, const std::string& host, uint16_t port = 0,
            const std::string& path = {}, const std::string& query = {}, const std::string& fragment = {});

        bool has_slashes() const noexcept { return ! empty() && text_[ofs_user_ - 1] == '/'; }
        bool has_user() const noexcept { return ! empty() && ofs_password_ > ofs_user_; }
        bool has_password() const noexcept { return ! empty() && ofs_host_ - ofs_password_ > 1; }
        bool has_host() const noexcept { return ! empty() && ofs_port_ > ofs_host_; }
        bool has_port() const noexcept { return ! empty() && ofs_path_ > ofs_port_; }
        bool has_path() const noexcept { return ! empty() && ofs_query_ - ofs_path_ > 1; }
        bool has_query() const noexcept { return ! empty() && ofs_fragment_ > ofs_query_; }
        bool has_fragment() const noexcept { return ! empty() && text_.size() > size_t(ofs_fragment_); }

        std::string scheme() const { return empty() ? std::string() : ascii_lowercase(text_.substr(0, ofs_user_ - (has_slashes() ? 3 : 1))); }
        std::string user() const { return decode(text_.substr(ofs_user_, ofs_password_ - ofs_user_)); }
        std::string password() const { return has_password() ? decode(text_.substr(ofs_password_ + 1, ofs_host_ - ofs_password_ - 2)) : std::string(); }
        std::string host() const { return decode(text_.substr(ofs_host_, ofs_port_ - ofs_host_)); }
        uint16_t port() const noexcept { return has_port() ? uint16_t(std::strtoul(text_.data() + ofs_port_ + 1, nullptr, 10)) : 0; }
        std::string path() const { return decode(text_.substr(ofs_path_, ofs_query_ - ofs_path_)); }
        std::string query() const { return has_query() ? decode(text_.substr(ofs_query_ + 1, ofs_fragment_ - ofs_query_ - 1)) : std::string(); }
        std::string fragment() const { return has_fragment() ? decode(text_.substr(ofs_fragment_ + 1, npos)) : std::string(); }

        void set_scheme(const std::string& new_scheme, bool smart = true);
        void set_user(const std::string& new_user);
        void set_password(const std::string& new_password);
        void set_host(const std::string& new_host);
        void set_port(uint16_t new_port);
        void set_path(const std::string& new_path);
        void set_query(const std::string& new_query);
        void set_fragment(const std::string& new_fragment);

        void clear_user() noexcept;
        void clear_password() noexcept;
        void clear_host() noexcept;
        void clear_port() noexcept;
        void clear_path() noexcept;
        void clear_query() noexcept;
        void clear_fragment() noexcept;

        void append_path(const std::string& new_path);
        void clear() noexcept;
        Uri doc() const;
        Uri base() const;
        Uri parent() const;
        Uri root() const;
        bool empty() const noexcept { return text_.empty(); }
        size_t hash() const noexcept;
        bool is_root() const noexcept { return ! empty() && ! has_path() && ! has_query() && ! has_fragment(); }
        std::string path_dir() const;
        std::string path_leaf() const;
        std::string str() const { return text_; }
        bool try_parse(const std::string& s);
        Uri& operator/=(const std::string& s) { append_path(s); return *this; }

        static std::string encode(const std::string& s, const std::string& exempt = {});
        static std::string decode(const std::string& s);
        template <typename R> static std::string make_query(const R& range, char delimiter = '&', int flags = 0);
        static std::vector<std::pair<std::string, std::string>> parse_query(const std::string& query, char delimiter = '\0');

        friend Uri operator/(const Uri& u, const std::string& s) { Uri v = u; v.append_path(s); return v; }
        friend bool operator==(const Uri& u, const Uri& v) noexcept { return u.text_ == v.text_; }
        friend std::strong_ordering operator<(const Uri& u, const Uri& v) noexcept;
        friend std::ostream& operator<<(std::ostream& out, const Uri& u) { return out << u.text_; }

    private:

        // scheme:[//][[user[:password]@]host[:port]][/path][?query][#fragment]

        std::string text_;      // Other fields are offsets into this string
        int ofs_user_ = 0;      // user
        int ofs_password_ = 0;  // :password@
        int ofs_host_ = 0;      // host
        int ofs_port_ = 0;      // :port
        int ofs_path_ = 0;      // /path
        int ofs_query_ = 0;     // ?query
        int ofs_fragment_ = 0;  // #fragment

        void check_nonempty() const;
        int find_path_cut() const noexcept;

    };

        template <typename R>
        std::string Uri::make_query(const R& range, char delimiter, int flags) {
            bool lone = (flags & lone_keys) != 0;
            std::string q;
            for (auto& [k,v]: range) {
                q += encode(k);
                if (! lone || ! v.empty())
                    q += '=';
                q += encode(v);
                q += delimiter;
            }
            if (! q.empty())
                q.pop_back();
            return q;
        }

    namespace Literals {

        inline Uri operator""_uri(const char* p, size_t n) { return Uri(std::string(p, n)); }

    }

}

namespace std {

    template <>
    struct hash<Crow::Uri> {
        size_t operator()(const Crow::Uri& u) const noexcept {
            return u.hash();
        }
    };

}
