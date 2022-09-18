#include "crow/net.hpp"
#include "crow/channel.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

namespace {

    static constexpr uint16_t port = 14882;

}

void test_crow_net_tcp_client_server() {

    auto t1 = std::thread([] {
        std::unique_ptr<TcpServer> server;
        std::unique_ptr<TcpClient> client;
        std::string msg;
        size_t n = 0;
        TRY(server = std::make_unique<TcpServer>(IPv4(), port));
        TEST(server->wait_for(500ms));
        TEST(server->read(client));
        REQUIRE(client);
        TEST_EQUAL(client->remote().ipv4(), IPv4::localhost());
        TEST(! client->wait_for(10ms));
        TEST(client->write("hello"));
        TEST(client->wait_for(100ms));
        TRY(n = client->append(msg));
        TEST_EQUAL(n, 7u);
        TEST_EQUAL(msg, "goodbye");
    });

    auto t2 = std::thread([] {
        std::unique_ptr<TcpClient> client;
        std::string msg;
        size_t n = 0;
        std::this_thread::sleep_for(200ms);
        TRY(client = std::make_unique<TcpClient>(IPv4::localhost(), port));
        TEST_EQUAL(client->remote().ipv4(), IPv4::localhost());
        TEST(client->wait_for(500ms));
        TRY(n = client->append(msg));
        TEST_EQUAL(n, 5u);
        TEST_EQUAL(msg, "hello");
        TEST(client->write("goodbye"));
        msg.clear();
        TEST(client->wait_for(100ms));
        TRY(n = client->append(msg));
        TEST_EQUAL(n, 0u);
        TEST_EQUAL(msg, "");
        TEST(client->wait_for(10ms));
        TEST(client->is_closed());
    });

    TRY(t1.join());
    TRY(t2.join());

}

void test_crow_net_socket_set() {

    auto t1 = std::thread([] {
        std::unique_ptr<TcpServer> server;
        std::unique_ptr<TcpClient> client1, client2;
        SocketSet set;
        std::string msg;
        std::vector<std::string> msgs;
        size_t n = 0;
        bool rc = false;
        Channel* cp = nullptr;
        TRY(server = std::make_unique<TcpServer>(IPv4(), port));
        TEST(server->wait_for(500ms));
        TEST(server->read(client1));
        REQUIRE(client1);
        TEST(client1->wait_for(10ms));
        TEST(server->wait_for(100ms));
        TEST(server->read(client2));
        REQUIRE(client2);
        TEST(client2->wait_for(10ms));
        TRY(set.insert(*client1));
        TRY(set.insert(*client2));
        for (int i = 0; i < 6; ++i) {
            TRY(rc = bool(set.wait_for(50ms)));
            if (! rc)
                continue;
            TEST(set.read(cp));
            msg.clear();
            if (cp == client1.get()) {
                TRY(n = client1->append(msg));
            } else if (cp == client2.get()) {
                TRY(n = client2->append(msg));
            } if (n) {
                msgs.push_back(msg);
            }
        }
        std::sort(msgs.begin(), msgs.end());
        TRY(msg = format_range(msgs));
        TEST_EQUAL(msg, "[alpha,bravo]");
    });

    auto t2 = std::thread([] {
        std::unique_ptr<TcpClient> client;
        std::this_thread::sleep_for(50ms);
        TRY(client = std::make_unique<TcpClient>(IPv4::localhost(), port));
        TRY(client->write("alpha"));
        std::this_thread::sleep_for(200ms);
    });

    auto t3 = std::thread([] {
        std::unique_ptr<TcpClient> client;
        std::this_thread::sleep_for(50ms);
        TRY(client = std::make_unique<TcpClient>(IPv4::localhost(), port));
        TRY(client->write("bravo"));
        std::this_thread::sleep_for(200ms);
    });

    TRY(t1.join());
    TRY(t2.join());
    TRY(t3.join());

}
