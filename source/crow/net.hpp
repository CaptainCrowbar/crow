#pragma once

#include "crow/channel.hpp"
#include "crow/io-utility.hpp"
#include "crow/types.hpp"
#include <atomic>
#include <cstring>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#ifdef _XOPEN_SOURCE
    #include <netinet/in.h>
    #include <sys/socket.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

namespace Crow {

    namespace Detail {

        #ifdef _XOPEN_SOURCE

            inline void net_init() noexcept {}

        #else

            void net_init();

        #endif

    }

    // Forward declarations

    class IPv4;
    class IPv6;
    class Socket;
    class SocketAddress;
    class SocketSet;
    class TcpClient;
    class TcpServer;
    class UdpClient;

    // Basic definitions

    #ifdef _XOPEN_SOURCE

        using NativeSocket = int;

    #else

        using NativeSocket = uintptr_t;

    #endif

    static constexpr auto no_socket = NativeSocket(-1);

    // IP address classes

    class IPv4 {
    public:
        static constexpr size_t size = 4;
        IPv4() noexcept { udata_ = 0; }
        explicit IPv4(uint32_t addr) noexcept;
        IPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) noexcept: bytes_{a,b,c,d} {}
        explicit IPv4(const std::string& s);
        uint8_t operator[](unsigned i) const noexcept { return i < size ? bytes_[i] : 0; }
        uint8_t* data() noexcept { return bytes_; }
        const uint8_t* data() const noexcept { return bytes_; }
        size_t hash() const noexcept { return std::hash<uint32_t>()(value()); }
        uint32_t net() const noexcept { return udata_; }
        uint32_t& net() noexcept { return udata_; }
        std::string str() const;
        uint32_t value() const noexcept;
        static IPv4 any() noexcept { return {}; }
        static IPv4 broadcast() noexcept { return IPv4(0xffffffff); }
        static IPv4 localhost() noexcept { return IPv4(0x7f000001); }
        static IPv4 from_sin(const void* ptr) noexcept;
    private:
        union {
            uint32_t udata_;
            uint8_t bytes_[size];
        };
    };

    static_assert(sizeof(IPv4) == IPv4::size, "Unexpected size for IPv4 class");

    inline bool operator==(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() == rhs.value(); }
    inline bool operator!=(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() != rhs.value(); }
    inline bool operator<(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() < rhs.value(); }
    inline bool operator>(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() > rhs.value(); }
    inline bool operator<=(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() <= rhs.value(); }
    inline bool operator>=(IPv4 lhs, IPv4 rhs) noexcept { return lhs.value() >= rhs.value(); }
    inline std::ostream& operator<<(std::ostream& out, IPv4 ip) { return out << ip.str(); }

    class IPv6 {
    public:
        static constexpr size_t size = 16;
        IPv6() noexcept { std::memset(bytes_, 0, sizeof(bytes_)); }
        IPv6(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
            uint8_t i, uint8_t j, uint8_t k, uint8_t l, uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept:
            bytes_{a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p} {}
        explicit IPv6(const std::string& s);
        uint8_t operator[](unsigned i) const noexcept { return i < size ? reinterpret_cast<const uint8_t*>(this)[i] : 0; }
        uint8_t* data() noexcept { return bytes_; }
        const uint8_t* data() const noexcept { return bytes_; }
        size_t hash() const noexcept;
        std::string str() const;
        static IPv6 any() noexcept { return {}; }
        static IPv6 localhost() noexcept { return IPv6(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1); }
        static IPv6 from_sin(const void* ptr) noexcept;
    private:
        union {
            uint8_t bytes_[size];
            std::aligned_storage_t<size> align_;
        };
    };

    static_assert(sizeof(IPv6) == IPv6::size, "Unexpected size for IPv6 class");

    inline bool operator==(IPv6 lhs, IPv6 rhs) noexcept { return std::memcmp(&lhs, &rhs, IPv6::size) == 0; }
    inline bool operator!=(IPv6 lhs, IPv6 rhs) noexcept { return ! (lhs == rhs); }
    inline bool operator<(IPv6 lhs, IPv6 rhs) noexcept { return std::memcmp(&lhs, &rhs, IPv6::size) < 0; }
    inline bool operator>(IPv6 lhs, IPv6 rhs) noexcept { return rhs < lhs; }
    inline bool operator<=(IPv6 lhs, IPv6 rhs) noexcept { return ! (rhs < lhs); }
    inline bool operator>=(IPv6 lhs, IPv6 rhs) noexcept { return ! (lhs < rhs); }
    inline std::ostream& operator<<(std::ostream& out, IPv6 ip) { return out << ip.str(); }

    class SocketAddress {
    private:
        union sa_union_type {
            sockaddr base;
            sockaddr_in inet4;
            sockaddr_in6 inet6;
        };
        sa_union_type sa_;
        size_t current_size_ = 0;
    public:
        static constexpr size_t max_size = sizeof(sa_union_type);
        SocketAddress() noexcept { std::memset(&sa_, 0, sizeof(sa_union_type)); }
        SocketAddress(IPv4 ip, uint16_t port = 0) noexcept;
        SocketAddress(IPv6 ip, uint16_t port = 0, uint32_t flow = 0, uint32_t scope = 0) noexcept;
        SocketAddress(const void* ptr, size_t n) noexcept;
        explicit SocketAddress(const std::string& s);
        explicit operator bool() const noexcept;
        uint8_t* data() noexcept { return reinterpret_cast<uint8_t*>(&sa_); }
        const uint8_t* data() const noexcept { return reinterpret_cast<const uint8_t*>(&sa_); }
        uint16_t family() const noexcept { return current_size_ < sizeof(sockaddr) ? 0 : sa_.base.sa_family; }
        uint32_t flow() const noexcept;
        size_t hash() const noexcept;
        IPv4 ipv4() const noexcept;
        IPv6 ipv6() const noexcept;
        sockaddr* native() noexcept { return &sa_.base; }
        const sockaddr* native() const noexcept { return &sa_.base; }
        uint16_t port() const noexcept;
        uint32_t scope() const noexcept;
        void set_family(uint16_t f);
        void set_size(size_t n);
        size_t size() const noexcept { return current_size_; }
        std::string str() const;
    };

    bool operator==(const SocketAddress& lhs, const SocketAddress& rhs) noexcept;
    inline bool operator!=(const SocketAddress& lhs, const SocketAddress& rhs) noexcept { return ! (lhs == rhs); }
    bool operator<(const SocketAddress& lhs, const SocketAddress& rhs) noexcept;
    inline bool operator>(const SocketAddress& lhs, const SocketAddress& rhs) noexcept { return rhs < lhs; }
    inline bool operator<=(const SocketAddress& lhs, const SocketAddress& rhs) noexcept { return ! (rhs < lhs); }
    inline bool operator>=(const SocketAddress& lhs, const SocketAddress& rhs) noexcept { return ! (lhs < rhs); }
    inline std::ostream& operator<<(std::ostream& out, const SocketAddress& sa) { return out << sa.str(); }

    // IP address literals

    namespace Literals {

        IPv4 operator""_ip4(const char* p, size_t n);
        IPv6 operator""_ip6(const char* p, size_t n);
        SocketAddress operator""_sa(const char* p, size_t n);

    }

    // DNS query functions

    namespace Dns {

        SocketAddress host_to_ip(const std::string& name, int family = 0);
        std::vector<SocketAddress> host_to_ips(const std::string& name, int family = 0);
        std::string ip_to_host(const SocketAddress& addr);

    };

    // TCP/IP socket classes

    class Socket:
    public StreamChannel {
    public:
        Socket() = default;
        explicit Socket(NativeSocket s): sock_(s) {}
        Socket(int domain, int type, int protocol = 0);
        ~Socket() noexcept override { do_close(); }
        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;
        Socket& operator=(const Socket&) = delete;
        Socket& operator=(Socket&&) = delete;
        void close() noexcept override { do_close(); }
        bool is_closed() const noexcept override { return sock_ == no_socket; }
        size_t read(void* dst, size_t maxlen) override { return do_read(dst, maxlen, nullptr); }
        SocketAddress local() const;
        SocketAddress remote() const;
        NativeSocket native() const noexcept { return sock_; }
        size_t read_from(void* dst, size_t maxlen, SocketAddress& from) { return do_read(dst, maxlen, &from); }
        void set_blocking(bool flag);
        bool write(std::string_view s) { return do_write(s.data(), s.size(), nullptr); }
        bool write(const void* src, size_t len) { return do_write(src, len, nullptr); }
        bool write_to(std::string_view s, const SocketAddress& to) { return do_write(s.data(), s.size(), &to); }
        bool write_to(const void* src, size_t len, const SocketAddress& to) { return do_write(src, len, &to); }
    protected:
        native_handle get_handle() const noexcept override { return reinterpret_cast<native_handle>(sock_); }
        void do_close() noexcept;
        bool do_wait_for(duration t) override;
    private:
        NativeSocket sock_ = no_socket;
        size_t do_read(void* dst, size_t maxlen, SocketAddress* from);
        bool do_write(const void* src, size_t len, const SocketAddress* to);
    };

    class TcpClient:
    public Socket {
    public:
        TcpClient() = default;
        explicit TcpClient(NativeSocket s) noexcept: Socket(s) {}
        explicit TcpClient(const SocketAddress& remote, const SocketAddress& local = {});
        template <typename... Args> explicit TcpClient(const Args&... args): TcpClient(SocketAddress{args...}) {}
        TcpClient(const TcpClient&) = delete;
        TcpClient(TcpClient&&) = delete;
        TcpClient& operator=(const TcpClient&) = delete;
        TcpClient& operator=(TcpClient&&) = delete;
        void set_nagle(bool flag);
    };

    class TcpServer:
    public MessageChannel<std::unique_ptr<TcpClient>> {
    public:
        TcpServer() = default;
        explicit TcpServer(NativeSocket s) noexcept: sock_(s) {}
        explicit TcpServer(const SocketAddress& local);
        template <typename... Args> explicit TcpServer(const Args&... args): TcpServer(SocketAddress{args...}) {}
        TcpServer(const TcpServer&) = delete;
        TcpServer(TcpServer&&) = delete;
        TcpServer& operator=(const TcpServer&) = delete;
        TcpServer& operator=(TcpServer&&) = delete;
        void close() noexcept override { sock_.close(); }
        bool is_closed() const noexcept override { return sock_.is_closed(); }
        bool read(std::unique_ptr<TcpClient>& t) override;
        SocketAddress local() const { return sock_.local(); }
        NativeSocket native() const noexcept { return sock_.native(); }
    protected:
        bool do_wait_for(duration t) override { return sock_.wait_for(t); }
    private:
        Socket sock_;
    };

    class UdpClient:
    public Socket {
    public:
        UdpClient() = default;
        explicit UdpClient(NativeSocket s) noexcept: Socket(s) {}
        explicit UdpClient(const SocketAddress& remote, const SocketAddress& local = {});
        template <typename... Args> explicit UdpClient(const Args&... args): UdpClient(SocketAddress{args...}) {}
        UdpClient(const UdpClient&) = delete;
        UdpClient(UdpClient&&) = delete;
        UdpClient& operator=(const UdpClient&) = delete;
        UdpClient& operator=(UdpClient&&) = delete;
    };

    class SocketSet:
    public MessageChannel<Channel*> {
    public:
        SocketSet() = default;
        SocketSet(const SocketSet&) = delete;
        SocketSet(SocketSet&&) = delete;
        SocketSet& operator=(const SocketSet&) = delete;
        SocketSet& operator=(SocketSet&&) = delete;
        void close() noexcept override { open_ = false; }
        bool is_closed() const noexcept override { return ! open_; }
        bool read(Channel*& t) override;
        void clear() noexcept;
        bool empty() const noexcept { return channels_.empty(); }
        void erase(Socket& s) noexcept { do_erase(s); }
        void erase(TcpServer& s) noexcept { do_erase(s); }
        void insert(Socket& s) { do_insert(s, s.native()); }
        void insert(TcpServer& s) { do_insert(s, s.native()); }
        size_t size() const noexcept { return channels_.size(); }
    protected:
        bool do_wait_for(duration t) override;
    private:
        friend class Socket;
        friend class TcpServer;
        std::vector<Channel*> channels_;
        std::vector<NativeSocket> natives_;
        Channel* current_ {nullptr};
        std::atomic<bool> open_ {true};
        void do_erase(Channel& c) noexcept;
        void do_insert(Channel& c, NativeSocket s);
        static int do_select(NativeSocket* sockets, size_t n, duration t = {}, size_t* index = nullptr);
            // +1 = ready, 0 = timeout, -1 = socket closed
    };

}

namespace std {

    template <>
    struct hash<Crow::IPv4> {
        size_t operator()(const Crow::IPv4& t) const noexcept {
            return t.hash();
        }
    };

    template <>
    struct hash<Crow::IPv6> {
        size_t operator()(const Crow::IPv6& t) const noexcept {
            return t.hash();
        }
    };

    template <>
    struct hash<Crow::SocketAddress> {
        size_t operator()(const Crow::SocketAddress& t) const noexcept {
            return t.hash();
        }
    };

}
