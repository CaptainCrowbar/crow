#include "crow/net.hpp"
#include "crow/channel.hpp"
#include "crow/unit-test.hpp"
#include <stdexcept>

using namespace Crow;

void test_crow_net_ipv4() {

    TEST_EQUAL(sizeof(IPv4), 4u);

    IPv4 ip;

    TEST_EQUAL(ip.value(), 0u);
    TEST_EQUAL(ip.net(), 0u);
    TEST_EQUAL(ip[0], 0);
    TEST_EQUAL(ip[1], 0);
    TEST_EQUAL(ip[2], 0);
    TEST_EQUAL(ip[3], 0);
    TEST_EQUAL(ip.str(), "0.0.0.0");

    ip = IPv4(0);
    TEST_EQUAL(ip.value(), 0u);
    TEST_EQUAL(ip.net(), 0u);
    TEST_EQUAL(ip[0], 0);
    TEST_EQUAL(ip[1], 0);
    TEST_EQUAL(ip[2], 0);
    TEST_EQUAL(ip[3], 0);
    TEST_EQUAL(ip.str(), "0.0.0.0");

    ip = IPv4(0x12345678);
    TEST_EQUAL(ip.value(), 0x12345678u);
    TEST_EQUAL(ip.net(), 0x78563412u);
    TEST_EQUAL(ip[0], 0x12);
    TEST_EQUAL(ip[1], 0x34);
    TEST_EQUAL(ip[2], 0x56);
    TEST_EQUAL(ip[3], 0x78);
    TEST_EQUAL(ip.str(), "18.52.86.120");

    ip = IPv4::broadcast();
    TEST_EQUAL(ip.value(), 0xffffffffu);
    TEST_EQUAL(ip.net(), 0xffffffffu);
    TEST_EQUAL(ip[0], 0xff);
    TEST_EQUAL(ip[1], 0xff);
    TEST_EQUAL(ip[2], 0xff);
    TEST_EQUAL(ip[3], 0xff);
    TEST_EQUAL(ip.str(), "255.255.255.255");

    ip = IPv4::localhost();
    TEST_EQUAL(ip.value(), 0x7f000001u);
    TEST_EQUAL(ip.net(), 0x0100007fu);
    TEST_EQUAL(ip[0], 127);
    TEST_EQUAL(ip[1], 0);
    TEST_EQUAL(ip[2], 0);
    TEST_EQUAL(ip[3], 1);
    TEST_EQUAL(ip.str(), "127.0.0.1");

    TRY(ip = IPv4("12.34.56.78"));
    TEST_EQUAL(ip.value(), 0x0c22384eu);
    TEST_EQUAL(ip.net(), 0x4e38220cu);
    TEST_EQUAL(ip.str(), "12.34.56.78");

    TRY(ip = IPv4("255.255.255.255"));
    TEST_EQUAL(ip.value(), 0xffffffffu);
    TEST_EQUAL(ip.net(), 0xffffffffu);
    TEST_EQUAL(ip.str(), "255.255.255.255");

    TEST_THROW(ip = IPv4(""), std::invalid_argument);
    TEST_THROW(ip = IPv4("1.2.3"), std::invalid_argument);
    TEST_THROW(ip = IPv4("1.2.3.4.5"), std::invalid_argument);
    TEST_THROW(ip = IPv4("256.0.0.0"), std::invalid_argument);
    TEST_THROW(ip = IPv4("a.b.c.d"), std::invalid_argument);

}

void test_crow_net_ipv6() {

    TEST_EQUAL(sizeof(IPv6), 16u);

    IPv6 ip;

    TEST_EQUAL(ip[0], 0);
    TEST_EQUAL(ip[1], 0);
    TEST_EQUAL(ip[2], 0);
    TEST_EQUAL(ip[3], 0);
    TEST_EQUAL(ip[4], 0);
    TEST_EQUAL(ip[5], 0);
    TEST_EQUAL(ip[6], 0);
    TEST_EQUAL(ip[7], 0);
    TEST_EQUAL(ip[8], 0);
    TEST_EQUAL(ip[9], 0);
    TEST_EQUAL(ip[10], 0);
    TEST_EQUAL(ip[11], 0);
    TEST_EQUAL(ip[12], 0);
    TEST_EQUAL(ip[13], 0);
    TEST_EQUAL(ip[14], 0);
    TEST_EQUAL(ip[15], 0);
    TEST_EQUAL(ip.str(), "::");

    ip = IPv6(0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf1,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xff);
    TEST_EQUAL(ip[0], 0x12);
    TEST_EQUAL(ip[1], 0x34);
    TEST_EQUAL(ip[2], 0x56);
    TEST_EQUAL(ip[3], 0x78);
    TEST_EQUAL(ip[4], 0x9a);
    TEST_EQUAL(ip[5], 0xbc);
    TEST_EQUAL(ip[6], 0xde);
    TEST_EQUAL(ip[7], 0xf1);
    TEST_EQUAL(ip[8], 0x23);
    TEST_EQUAL(ip[9], 0x45);
    TEST_EQUAL(ip[10], 0x67);
    TEST_EQUAL(ip[11], 0x89);
    TEST_EQUAL(ip[12], 0xab);
    TEST_EQUAL(ip[13], 0xcd);
    TEST_EQUAL(ip[14], 0xef);
    TEST_EQUAL(ip[15], 0xff);
    TEST_EQUAL(ip.str(), "1234:5678:9abc:def1:2345:6789:abcd:efff");

    ip = IPv6::localhost();
    TEST_EQUAL(ip[0], 0);
    TEST_EQUAL(ip[1], 0);
    TEST_EQUAL(ip[2], 0);
    TEST_EQUAL(ip[3], 0);
    TEST_EQUAL(ip[4], 0);
    TEST_EQUAL(ip[5], 0);
    TEST_EQUAL(ip[6], 0);
    TEST_EQUAL(ip[7], 0);
    TEST_EQUAL(ip[8], 0);
    TEST_EQUAL(ip[9], 0);
    TEST_EQUAL(ip[10], 0);
    TEST_EQUAL(ip[11], 0);
    TEST_EQUAL(ip[12], 0);
    TEST_EQUAL(ip[13], 0);
    TEST_EQUAL(ip[14], 0);
    TEST_EQUAL(ip[15], 1);
    TEST_EQUAL(ip.str(), "::1");

    TRY(ip = IPv6("1234:5678:9abc:def1:2345:6789:abcd:efff"));
    TEST_EQUAL(ip.str(), "1234:5678:9abc:def1:2345:6789:abcd:efff");
    TRY(ip = IPv6("1234::abcd"));
    TEST_EQUAL(ip.str(), "1234::abcd");

    TEST_THROW(ip = IPv6(""), std::invalid_argument);
    TEST_THROW(ip = IPv6(":::"), std::invalid_argument);
    TEST_THROW(ip = IPv6("abcd::efgh"), std::invalid_argument);
    TEST_THROW(ip = IPv6("1.2.3.4"), std::invalid_argument);

}

void test_crow_net_socket_address() {

    SocketAddress sa;

    TEST_EQUAL(sa.family(), 0);
    TEST_EQUAL(sa.ipv4(), IPv4());
    TEST_EQUAL(sa.ipv6(), IPv6());
    TEST_EQUAL(sa.port(), 0);
    TEST_EQUAL(sa.size(), 0u);
    TEST_EQUAL(sa.str(), "null");

    TRY((sa = {IPv4::localhost(), 12345}));
    TEST_EQUAL(sa.family(), AF_INET);
    TEST_EQUAL(sa.ipv4(), IPv4::localhost());
    TEST_EQUAL(sa.port(), 12345);
    TEST_EQUAL(sa.size(), 16u);
    TEST_EQUAL(sa.str(), "127.0.0.1:12345");

    TRY(sa = SocketAddress("12.34.56.78"));
    TEST_EQUAL(sa.family(), AF_INET);
    TEST_EQUAL(sa.ipv4().value(), 0x0c22384eu);
    TEST_EQUAL(sa.port(), 0);
    TEST_EQUAL(sa.size(), 16u);
    TEST_EQUAL(sa.str(), "12.34.56.78:0");

    TRY(sa = SocketAddress("12.34.56.78:90"));
    TEST_EQUAL(sa.family(), AF_INET);
    TEST_EQUAL(sa.ipv4().value(), 0x0c22384eu);
    TEST_EQUAL(sa.port(), 90);
    TEST_EQUAL(sa.size(), 16u);
    TEST_EQUAL(sa.str(), "12.34.56.78:90");

    TRY((sa = {IPv6::localhost(), 12345}));
    TEST_EQUAL(sa.family(), AF_INET6);
    TEST_EQUAL(sa.ipv6(), IPv6::localhost());
    TEST_EQUAL(sa.port(), 12345);
    TEST_EQUAL(sa.size(), 28u);
    TEST_EQUAL(sa.str(), "[::1]:12345");

    TRY(sa = SocketAddress("1234:5678:9abc:def1:2345:6789:abcd:efff"));
    TEST_EQUAL(sa.family(), AF_INET6);
    TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:0");
    TEST_EQUAL(sa.port(), 0);
    TEST_EQUAL(sa.size(), 28u);

    TRY(sa = SocketAddress("[1234:5678:9abc:def1:2345:6789:abcd:efff]"));
    TEST_EQUAL(sa.family(), AF_INET6);
    TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:0");
    TEST_EQUAL(sa.port(), 0);
    TEST_EQUAL(sa.size(), 28u);

    TRY(sa = SocketAddress("[1234:5678:9abc:def1:2345:6789:abcd:efff]:12345"));
    TEST_EQUAL(sa.family(), AF_INET6);
    TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:12345");
    TEST_EQUAL(sa.port(), 12345);
    TEST_EQUAL(sa.size(), 28u);

}

void test_crow_net_ip_literals() {

    using namespace Crow::Literals;

    IPv4 ip4;
    IPv6 ip6;
    SocketAddress sa;

    TRY(ip4 = ""_ip4);             TEST_EQUAL(ip4.str(), "0.0.0.0");
    TRY(ip4 = "12.34.56.78"_ip4);  TEST_EQUAL(ip4.str(), "12.34.56.78");
    TRY(ip4 = "127.0.0.1"_ip4);    TEST_EQUAL(ip4.str(), "127.0.0.1");

    TRY(ip6 = ""_ip6);                                         TEST_EQUAL(ip6.str(), "::");
    TRY(ip6 = "::1"_ip6);                                      TEST_EQUAL(ip6.str(), "::1");
    TRY(ip6 = "1234::abcd"_ip6);                               TEST_EQUAL(ip6.str(), "1234::abcd");
    TRY(ip6 = "1234:5678:9abc:def1:2345:6789:abcd:efff"_ip6);  TEST_EQUAL(ip6.str(), "1234:5678:9abc:def1:2345:6789:abcd:efff");

    TRY(sa = ""_sa);                                                 TEST_EQUAL(sa.str(), "null");
    TRY(sa = "127.0.0.1:12345"_sa);                                  TEST_EQUAL(sa.str(), "127.0.0.1:12345");
    TRY(sa = "12.34.56.78"_sa);                                      TEST_EQUAL(sa.str(), "12.34.56.78:0");
    TRY(sa = "12.34.56.78:90"_sa);                                   TEST_EQUAL(sa.str(), "12.34.56.78:90");
    TRY(sa = "[::1]:12345"_sa);                                      TEST_EQUAL(sa.str(), "[::1]:12345");
    TRY(sa = "1234:5678:9abc:def1:2345:6789:abcd:efff"_sa);          TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:0");
    TRY(sa = "[1234:5678:9abc:def1:2345:6789:abcd:efff]"_sa);        TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:0");
    TRY(sa = "[1234:5678:9abc:def1:2345:6789:abcd:efff]:12345"_sa);  TEST_EQUAL(sa.str(), "[1234:5678:9abc:def1:2345:6789:abcd:efff]:12345");

}
