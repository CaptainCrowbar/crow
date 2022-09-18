#include "crow/net.hpp"
#include "crow/channel.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_net_dns_query() {

    static const std::string good_name = "en.cppreference.com";
    static const std::string bad_name = "no-such-host.xyz";

    std::string name;
    SocketAddress addr;
    std::vector<SocketAddress> addrs;

    TRY(addr = Dns::host_to_ip(good_name));
    TEST(addr);
    TEST(addr.family() == AF_INET || addr.family() == AF_INET6);

    TRY(addrs = Dns::host_to_ips(good_name));
    TEST(! addrs.empty());
    if (! addrs.empty())
        TEST_EQUAL(addrs[0], addr);
    // std::cout << "... " << good_name << " => " << format_range(addrs) << "\n";

    for (auto& a: addrs) {
        name.clear();
        TRY(name = Dns::ip_to_host(a));
        // std::cout << "... " << a << " => " << name << "\n";
    }

    #ifndef __linux__ // Takes too long

        TRY(addr = Dns::host_to_ip(bad_name));
        TEST(! addr);

        TRY(addrs = Dns::host_to_ips(bad_name));
        TEST(addrs.empty());

    #endif

}
