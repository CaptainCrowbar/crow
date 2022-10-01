#include "crow/net.hpp"
#include "crow/guard.hpp"
#include "crow/hash.hpp"
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <set>
#include <stdexcept>
#include <system_error>
#include <thread>

#ifdef _XOPEN_SOURCE
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/tcp.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <unistd.h>
#endif

using namespace std::chrono;
using namespace std::literals;

namespace Crow {

    namespace {

        #ifdef _XOPEN_SOURCE

            using SocketFlag = int;
            using SocketIop = int;
            using SocketSendRecv = ssize_t;

            constexpr int e_again = EAGAIN;
            constexpr int e_badf = EBADF;
            constexpr int eai_again = EAI_AGAIN;
            constexpr int eai_noname = EAI_NONAME;
            constexpr int eai_overflow = EAI_OVERFLOW;
            constexpr int eai_system = EAI_SYSTEM;

            void clear_error() noexcept { errno = 0; }
            int close_socket(NativeSocket s) noexcept { return ::close(s); }
            int get_error() noexcept { return errno; }
            std::string system_string_to_utf8(const std::string& str) { return str; }
            constexpr auto ioctl_socket = &::ioctl;

        #else

            using SocketFlag = char;
            using SocketIop = unsigned long;
            using SocketSendRecv = int;
            using sa_family_t = unsigned short;

            constexpr int e_again = WSAEWOULDBLOCK;
            constexpr int e_badf = WSAENOTSOCK;
            constexpr int eai_again = WSATRY_AGAIN;
            constexpr int eai_noname = WSAHOST_NOT_FOUND;
            constexpr int eai_overflow = ERROR_INSUFFICIENT_BUFFER;
            constexpr int eai_system = -1; // No equivalent on Windows

            void clear_error() noexcept { WSASetLastError(0); }
            int close_socket(NativeSocket s) noexcept { return ::closesocket(s); }
            int get_error() noexcept { return WSAGetLastError(); }
            int ioctl_socket(NativeSocket s, long r, SocketIop* p) noexcept { return ::ioctlsocket(s, r, p); }

            std::string system_string_to_utf8(const std::wstring& str) {
                int len = WideCharToMultiByte(CP_UTF8, 0, str.data(), int(str.size()), nullptr, 0, nullptr, nullptr);
                std::string utf8(len, 0);
                WideCharToMultiByte(CP_UTF8, 0, str.data(), int(str.size()), utf8.data(), len, nullptr, nullptr);
                utf8.pop_back();
                return utf8;
            }

        #endif

        #ifdef _MSC_VER

            using dns_name_size = uint32_t;
            using socket_iosize = int;

        #else

            using dns_name_size = socklen_t;
            using socket_iosize = socklen_t;

        #endif

        template <typename T>
        struct NetResult {
            T res = 0;
            int err = 0;
            const NetResult& fail_if(T t, const char* f) const {
                if (res == t)
                    throw std::system_error(err, std::system_category(), f);
                return *this;
            }
        };

        template <typename T>
        NetResult<T> net_call(T t) noexcept {
            auto e = get_error();
            return {t, e};
        }

        void control_blocking(NativeSocket sock, bool flag) {
            auto mode = SocketIop(! flag);
            clear_error();
            net_call(ioctl_socket(sock, FIONBIO, &mode)).fail_if(-1, "ioctl()");
        }

        void control_nagle(NativeSocket sock, bool flag) {
            auto mode = SocketFlag(! flag);
            clear_error();
            net_call(::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &mode, sizeof(mode))).fail_if(-1, "setsockopt()");
        }

        std::string hex_bytes(const void* ptr, size_t len) {
            static constexpr const char* xdigits = "0123456789abcdef";
            std::string s(2 * len, '\0');
            auto bptr = static_cast<const uint8_t*>(ptr);
            for (size_t i = 0, j = 0; i < len; ++i, j += 2) {
                s[j] = xdigits[bptr[i] >> 4];
                s[j + 1] = xdigits[bptr[i] & 15];
            }
            return s;
        }

        void null_term(std::string& s) noexcept {
            size_t pos = s.find('\0');
            if (pos != std::string::npos)
                s.resize(pos);
        }

        uint16_t parse_port(const std::string& s) {
            char* endptr = nullptr;
            errno = 0;
            auto n = std::strtoull(s.data(), &endptr, 10);
            if (errno != 0 || endptr != s.data() + s.size() || n > 65535)
                throw std::invalid_argument("Invalid port number: " + s);
            return uint16_t(n);
        }

        class AddrinfoCategory:
        public std::error_category {
        public:
            AddrinfoCategory() {}
            std::string message(int ev) const override { return system_string_to_utf8(gai_strerror(ev)); }
            const char* name() const noexcept override { return "Addrinfo"; }
        };

        const std::error_category& addrinfo_category() noexcept {
            static const AddrinfoCategory cat;
            return cat;
        }

    }

    namespace Detail {

        int mem_compare(const void* lhs, size_t n1, const void* rhs, size_t n2) noexcept {
            int rc = std::memcmp(lhs, rhs, std::min(n1, n2));
            return rc < 0 ? -1 : rc > 0 ? 1 : n1 < n2 ? -1 : n1 > n2 ? 1 : 0;
        }

        #ifndef _XOPEN_SOURCE

            void net_init() {
                static const auto init = [] () -> int {
                    WSADATA data;
                    std::memset(&data, 0, sizeof(data));
                    int rc = WSAStartup(0x202, &data);
                    if (rc != 0)
                        throw std::system_error(rc, std::system_category(), "WSAStartup()");
                    return 0;
                }();
                (void)init;
            }

        #endif

    }

    // Class IPv4

    IPv4::IPv4(uint32_t addr) noexcept:
    udata_(htonl(addr)) {}

    IPv4::IPv4(const std::string& s) {
        Detail::net_init();
        clear_error();
        auto rc = net_call(inet_pton(AF_INET, s.data(), bytes_)).fail_if(-1, "inet_pton()");
        if (rc.res == 0)
            throw std::invalid_argument("Invalid IPv4 address: " + s);
    }

    std::string IPv4::str() const {
        Detail::net_init();
        auto vbytes = const_cast<uint8_t*>(bytes_); // Windows brain damage
        std::string s(16, '\0');
        for (;;) {
            clear_error();
            auto rc = net_call(inet_ntop(AF_INET, vbytes, &s[0], socklen_t(s.size())));
            if (rc.res)
                break;
            else if (rc.err != ENOSPC)
                rc.fail_if(nullptr, "inet_ntop()");
            s.resize(s.size() + 16, '\0');
        }
        null_term(s);
        return s;
    }

    uint32_t IPv4::value() const noexcept {
        return ntohl(udata_);
    }

    IPv4 IPv4::from_sin(const void* ptr) noexcept {
        IPv4 ip;
        if (ptr)
            std::memcpy(ip.bytes_, ptr, sizeof(IPv4));
        return ip;
    }

    // Class IPv6

    IPv6::IPv6(const std::string& s) {
        Detail::net_init();
        clear_error();
        auto rc = net_call(inet_pton(AF_INET6, s.data(), bytes_)).fail_if(-1, "inet_pton()");
        if (rc.res == 0)
            throw std::invalid_argument("Invalid IPv6 address: " + s);
    }

    size_t IPv6::hash() const noexcept {
        return BernsteinHash()(bytes_, size);
    }

    std::string IPv6::str() const {
        Detail::net_init();
        auto vbytes = const_cast<uint8_t*>(bytes_); // Windows brain damage
        std::string s(40, '\0');
        for (;;) {
            clear_error();
            auto rc = net_call(inet_ntop(AF_INET6, vbytes, &s[0], socklen_t(s.size())));
            if (rc.res)
                break;
            else if (rc.err != ENOSPC)
                rc.fail_if(nullptr, "inet_ntop()");
            s.resize(s.size() + 40, '\0');
        }
        null_term(s);
        return s;
    }

    IPv6 IPv6::from_sin(const void* ptr) noexcept {
        IPv6 ip;
        if (ptr)
            std::memcpy(ip.bytes_, ptr, sizeof(IPv6));
        return ip;
    }

    // Class SocketAddress

    SocketAddress::SocketAddress(IPv4 ip, uint16_t port) noexcept:
    SocketAddress() {
        sa_.inet4.sin_family = AF_INET;
        sa_.inet4.sin_port = htons(port);
        sa_.inet4.sin_addr.s_addr = ip.net();
        current_size_ = sizeof(sockaddr_in);
    }

    SocketAddress::SocketAddress(IPv6 ip, uint16_t port, uint32_t flow, uint32_t scope) noexcept:
    SocketAddress() {
        std::memset(&sa_, 0, sizeof(sa_union_type));
        sa_.inet6.sin6_family = AF_INET6;
        sa_.inet6.sin6_port = htons(port);
        sa_.inet6.sin6_flowinfo = htonl(flow);
        std::memcpy(sa_.inet6.sin6_addr.s6_addr, ip.data(), ip.size);
        sa_.inet6.sin6_scope_id = htonl(scope);
        current_size_ = sizeof(sockaddr_in6);
    }

    SocketAddress::SocketAddress(const void* ptr, size_t n) noexcept:
    SocketAddress() {
        current_size_ = std::min(n, max_size);
        std::memcpy(&sa_, ptr, current_size_);
    }

    SocketAddress::SocketAddress(const std::string& s):
    SocketAddress() {
        if (s[0] >= '0' && s[0] <= '9') {
            size_t colon = s.find(':');
            if (colon != std::string::npos && s.find(':', colon + 1) != std::string::npos) {
                *this = IPv6(s);
                return;
            }
            IPv4 ip(s.substr(0, colon));
            uint16_t port = 0;
            if (colon != std::string::npos)
                port = parse_port(s.substr(colon + 1, std::string::npos));
            *this = SocketAddress(ip, port);
            return;
        }
        if (s[0] == '[') {
            size_t close = s.find(']');
            if (close != std::string::npos) {
                IPv6 ip(s.substr(1, close - 1));
                uint16_t port = 0;
                if (close + 2 < s.size())
                    port = parse_port(s.substr(close + 2, std::string::npos));
                *this = SocketAddress(ip, port);
                return;
            }
        }
        throw std::invalid_argument("Invalid socket address: " + s);
    }

    SocketAddress::operator bool() const noexcept {
        switch (family()) {
            case 0:         return false;
            case AF_INET:   return ipv4() != IPv4() || port() != 0;
            case AF_INET6:  return ipv6() != IPv6() || port() != 0;
            default:        break;
        }
        for (size_t i = sizeof(sa_.base); i < current_size_; ++i)
            if (data()[i])
                return true;
        return false;
    }

    uint32_t SocketAddress::flow() const noexcept {
        return family() == AF_INET6 ? ntohl(sa_.inet6.sin6_flowinfo) : 0;
    }

    size_t SocketAddress::hash() const noexcept {
        return BernsteinHash()(&sa_, current_size_);
    }

    IPv4 SocketAddress::ipv4() const noexcept {
        return family() == AF_INET ? IPv4(ntohl(sa_.inet4.sin_addr.s_addr)) : IPv4();
    }

    IPv6 SocketAddress::ipv6() const noexcept {
        return family() == AF_INET6 ? IPv6::from_sin(&sa_.inet6.sin6_addr) : IPv6();
    }

    uint16_t SocketAddress::port() const noexcept {
        switch (family()) {
            case AF_INET:   return ntohs(sa_.inet4.sin_port);
            case AF_INET6:  return ntohs(sa_.inet6.sin6_port);
            default:        return 0;
        }
    }

    uint32_t SocketAddress::scope() const noexcept {
        return family() == AF_INET6 ? ntohl(sa_.inet6.sin6_scope_id) : 0;
    }

    void SocketAddress::set_family(uint16_t f) {
        if (current_size_ < sizeof(sockaddr))
            current_size_ = sizeof(sockaddr);
        sa_.base.sa_family = sa_family_t(f);
    }

    void SocketAddress::set_size(size_t n) {
        if (n > max_size)
            throw std::length_error("SocketAddress size is too big");
        current_size_ = n;
    }

    std::string SocketAddress::str() const {
        if (*this == SocketAddress())
            return "null";
        else if (family() == AF_INET)
            return ipv4().str() + ':' + std::to_string(port());
        else if (family() == AF_INET6)
            return '[' + ipv6().str() + "]:" + std::to_string(port());
        else
            return hex_bytes(native(), size());
    }

    bool operator==(const SocketAddress& lhs, const SocketAddress& rhs) noexcept {
        return Detail::mem_compare(lhs.native(), lhs.size(), rhs.native(), rhs.size()) == 0;
    }

    bool operator<(const SocketAddress& lhs, const SocketAddress& rhs) noexcept {
        return Detail::mem_compare(lhs.native(), lhs.size(), rhs.native(), rhs.size()) < 0;
    }

    // IP address literals

    namespace Literals {

        IPv4 operator""_ip4(const char* p, size_t n) {
            if (! p || ! n)
                return {};
            std::string s(p, n);
            return IPv4(s);
        }

        IPv6 operator""_ip6(const char* p, size_t n) {
            if (! p || ! n)
                return {};
            std::string s(p, n);
            return IPv6(s);
        }

        SocketAddress operator""_sa(const char* p, size_t n) {
            if (! p || ! n)
                return {};
            std::string s(p, n);
            return SocketAddress(s);
        }

    }

    // DNS query functions

    namespace Dns {

        SocketAddress host_to_ip(const std::string& name, int family) {
            Detail::net_init();
            addrinfo hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.ai_family = family;
            addrinfo* info = nullptr;
            auto guard = on_scope_exit([&] { if (info) freeaddrinfo(info); });
            clear_error();
            int rc = getaddrinfo(name.data(), nullptr, &hints, &info);
            int err = get_error();
            switch (rc) {
                case 0:           return SocketAddress(info->ai_addr, info->ai_addrlen);
                case eai_again:   return {};
                case eai_noname:  return {};
                case eai_system:  throw std::system_error(err, std::generic_category(), "getaddrinfo()");
                default:          throw std::system_error(rc, addrinfo_category(), "getaddrinfo()");
            }
        }

        std::vector<SocketAddress> host_to_ips(const std::string& name, int family) {
            Detail::net_init();
            addrinfo hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.ai_family = family;
            addrinfo* info = nullptr;
            auto guard = on_scope_exit([&] { if (info) freeaddrinfo(info); });
            clear_error();
            int rc = getaddrinfo(name.data(), nullptr, &hints, &info);
            int err = get_error();
            switch (rc) {
                case 0:           break;
                case eai_again:   return {};
                case eai_noname:  return {};
                case eai_system:  throw std::system_error(err, std::generic_category(), "getaddrinfo()");
                default:          throw std::system_error(rc, addrinfo_category(), "getaddrinfo()");
            }
            std::vector<SocketAddress> addrs;
            std::set<SocketAddress> seen;
            for (auto ai = info; ai; ai = ai->ai_next) {
                SocketAddress sa(ai->ai_addr, ai->ai_addrlen);
                if (sa && seen.insert(sa).second)
                    addrs.push_back(sa);
            }
            return addrs;
        }

        std::string ip_to_host(const SocketAddress& addr) {
            Detail::net_init();
            std::string name(100, '\0');
            for (;;) {
                clear_error();
                int rc = getnameinfo(addr.native(), socklen_t(addr.size()), &name[0], dns_name_size(name.size()), nullptr, 0, 0);
                int err = get_error();
                switch (rc) {
                    case 0:             null_term(name); return name;
                    case eai_again:     return {};
                    case eai_noname:    return {};
                    case eai_overflow:  break;
                    case eai_system:    throw std::system_error(err, std::generic_category(), "getnameinfo()");
                    default:            throw std::system_error(rc, addrinfo_category(), "getnameinfo()");
                }
                name.resize(2 * name.size());
            }
        }

    }

    // Class Socket

    Socket::Socket(int domain, int type, int protocol) {
        Detail::net_init();
        clear_error();
        auto rc = net_call(::socket(domain, type, protocol)).fail_if(no_socket, "socket()");
        sock_ = rc.res;
    }

    SocketAddress Socket::local() const {
        if (sock_ == no_socket)
            return {};
        SocketAddress sa;
        socklen_t len = SocketAddress::max_size;
        clear_error();
        net_call(::getsockname(sock_, sa.native(), &len)).fail_if(-1, "getpeername()");
        sa.set_size(len);
        return sa;
    }

    SocketAddress Socket::remote() const {
        if (sock_ == no_socket)
            return {};
        SocketAddress sa;
        socklen_t len = SocketAddress::max_size;
        clear_error();
        net_call(::getpeername(sock_, sa.native(), &len)).fail_if(-1, "getpeername()");
        sa.set_size(len);
        return sa;
    }

    void Socket::set_blocking(bool flag) {
        control_blocking(sock_, flag);
    }

    void Socket::do_close() noexcept {
        if (sock_ != no_socket) {
            close_socket(sock_);
            sock_ = no_socket;
        }
    }

    bool Socket::do_wait_for(duration t) {
        return sock_ == no_socket || SocketSet::do_select(&sock_, 1, t);
    }

    size_t Socket::do_read(void* dst, size_t maxlen, SocketAddress* from) {
        if (! dst || ! maxlen || sock_ == no_socket || SocketSet::do_select(&sock_, 1) != 1)
            return 0;
        auto cdst = static_cast<char*>(dst);
        NetResult<SocketSendRecv> rc;
        clear_error();
        if (from) {
            socklen_t addrlen = SocketAddress::max_size;
            rc = net_call(::recvfrom(native(), cdst, socket_iosize(maxlen), 0, from->native(), &addrlen)).fail_if(-1, "recvfrom()");
            if (rc.res > 0)
                from->set_size(addrlen);
        } else {
            rc = net_call(::recv(native(), cdst, socket_iosize(maxlen), 0)).fail_if(-1, "recv()");
        }
        if (rc.res == 0)
            do_close();
        return size_t(rc.res);
    }

    bool Socket::do_write(const void* src, size_t len, const SocketAddress* to) {
        if (! src || sock_ == no_socket)
            return false;
        auto csrc = static_cast<const char*>(src);
        int flags = 0;
        #ifdef MSG_NOSIGNAL
            flags |= MSG_NOSIGNAL;
        #endif
        size_t written = 0;
        NetResult<SocketSendRecv> rc;
        clear_error();
        while (written < len) {
            if (to)
                rc = net_call(::sendto(native(), csrc, socket_iosize(len), flags, to->native(), socket_iosize(to->size())));
            else
                rc = net_call(::send(native(), csrc, socket_iosize(len), flags));
            if (rc.res == -1 && rc.err == e_again)
                std::this_thread::sleep_for(10us);
            else
                rc.fail_if(-1, to ? "sendto()" : "send()");
            written += size_t(rc.res);
        }
        return true;
    }

    // Class TcpClient

    TcpClient::TcpClient(const SocketAddress& remote, const SocketAddress& local):
    Socket(PF_INET, SOCK_STREAM) {
        if (local) {
            clear_error();
            net_call(::bind(native(), local.native(), socket_iosize(local.size()))).fail_if(-1, "bind()");
        }
        clear_error();
        net_call(::connect(native(), remote.native(), socket_iosize(remote.size()))).fail_if(-1, "connect()");
        control_blocking(native(), false);
        control_nagle(native(), false);
    }

    void TcpClient::set_nagle(bool flag) {
        control_nagle(native(), flag);
    }

    // Class TcpServer

    TcpServer::TcpServer(const SocketAddress& local):
    sock_(PF_INET, SOCK_STREAM) {
        static constexpr int backlog = 10;
        SocketFlag on = 1;
        ::setsockopt(sock_.native(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (local) {
            clear_error();
            net_call(::bind(sock_.native(), local.native(), socket_iosize(local.size()))).fail_if(-1, "bind()");
        }
        control_blocking(native(), false);
        clear_error();
        net_call(::listen(sock_.native(), backlog)).fail_if(-1, "listen()");
    }

    bool TcpServer::read(std::unique_ptr<TcpClient>& t) {
        if (sock_.native() == no_socket)
            return false;
        auto s = sock_.native();
        if (SocketSet::do_select(&s, 1) != 1)
            return false;
        clear_error();
        auto rc = net_call(::accept(sock_.native(), nullptr, nullptr)).fail_if(no_socket, "socket()");
        t = std::make_unique<TcpClient>(rc.res);
        control_blocking(t->native(), false);
        control_nagle(t->native(), false);
        return true;
    }

    // Class UdpClient

    UdpClient::UdpClient(const SocketAddress& remote, const SocketAddress& local):
    Socket(PF_INET, SOCK_DGRAM) {
        if (local) {
            clear_error();
            net_call(::bind(native(), local.native(), socket_iosize(local.size()))).fail_if(-1, "bind()");
        }
        if (remote) {
            clear_error();
            net_call(::connect(native(), remote.native(), socket_iosize(remote.size()))).fail_if(-1, "connect()");
        }
        control_blocking(native(), false);
    }

    // Class SocketSet

    bool SocketSet::read(Channel*& t) {
        if (! open_ || channels_.empty())
            return false;
        if (current_) {
            t = current_;
            current_ = nullptr;
            return true;
        }
        size_t index = std::string::npos;
        do_select(natives_.data(), natives_.size(), {}, &index);
        if (index == std::string::npos)
            return false;
        t = channels_[index];
        return true;
    }

    void SocketSet::clear() noexcept {
        channels_.clear();
        natives_.clear();
        current_ = nullptr;
    }

    bool SocketSet::do_wait_for(duration t) {
        if (! open_ || current_)
            return true;
        size_t index = std::string::npos;
        int rc = do_select(natives_.data(), natives_.size(), t, &index);
        if (rc && index < channels_.size())
            current_ = channels_[index];
        return rc;
    }

    void SocketSet::do_erase(Channel& c) noexcept {
        auto it = std::find(channels_.begin(), channels_.end(), &c);
        if (it != channels_.end()) {
            natives_.erase(natives_.begin() + (it - channels_.begin()));
            channels_.erase(it);
            if (current_ == &c)
                current_ = nullptr;
        }
    }

    void SocketSet::do_insert(Channel& c, NativeSocket s) {
        channels_.push_back(&c);
        natives_.push_back(s);
    }

    int SocketSet::do_select(NativeSocket* sockets, size_t n, duration t, size_t* index) {
        Detail::net_init();
        if (index)
            *index = std::string::npos;
        fd_set rfds;
        FD_ZERO(&rfds);
        int last_socket = -1;
        for (size_t i = 0; i < n; ++i) {
            if (sockets[i] != no_socket) {
                FD_SET(sockets[i], &rfds);
                last_socket = std::max(last_socket, int(sockets[i]));
            }
        }
        fd_set efds = rfds;
        timeval tv = {0, 0};
        if (t > duration())
            duration_to_timeval(t, tv);
        clear_error();
        auto rc = net_call(::select(last_socket + 1, &rfds, nullptr, &efds, &tv));
        if (rc.res == 0)
            return 0;
        else if (rc.res == -1 && rc.err == e_badf)
            return -1;
        rc.fail_if(-1, "select()");
        size_t pos = std::string::npos;
        for (size_t i = 0; i < n && pos == std::string::npos; ++i)
            if (sockets[i] != no_socket && (FD_ISSET(sockets[i], &rfds) || FD_ISSET(sockets[i], &efds)))
                pos = i;
        if (pos == std::string::npos)
            return 0;
        if (index)
            *index = pos;
        return 1;
    }

}
