#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <atomic>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
using socklen_t = int;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
using SOCKET = int;
#endif

using namespace std;

// =========================
// Advanced TCP socket demo (concurrency + timeout + simple protocol)
// =========================
// Protocol (binary frame):
// [magic:4bytes][length:4bytes][payload:length bytes]
// magic is fixed to 0x43505031 ("CPP1"), length is in network byte order.
//
// compile:
//  g++ -std=c++17 .\net_tcp_plus.cpp -o .\net_tcp_plus.exe -lws2_32
//
// Usage:
//  server: net_tcp_plus.exe server <port>
//  client: net_tcp_plus.exe client <host> <port>
//
// Example:
//  Server: net_tcp_plus.exe server 1234
//  Client: net_tcp_plus.exe client 127.0.0.1 1234

// =========================
// 进阶版 TCP socket 示例 (并发 + 超时 + 简单协议)
// =========================
// 协议 (二进制帧):
// [magic:4bytes][length:4bytes][payload:length bytes]
// magic 固定为 0x43505031 ("CPP1")，length 为网络字节序。
//
// 编译:
//  g++ -std=c++17 .\net_tcp_plus.cpp -o .\net_tcp_plus.exe -lws2_32
//
// 用法:
//  服务器: net_tcp_plus.exe server <port>
//  客户端: net_tcp_plus.exe client <host> <port>
//
// 示例:
//  服务器: net_tcp_plus.exe server 1234
//  客户端: net_tcp_plus.exe client 127.0.0.1 1234

#if defined(_WIN32) || defined(_WIN64)
void socket_startup()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}
void socket_cleanup() { WSACleanup(); }
void close_socket(SOCKET s) { closesocket(s); }
bool set_socket_timeout_ms(SOCKET s, int ms)
{
    DWORD timeout = (DWORD)ms;
    int r1 = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    int r2 = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    return r1 == 0 && r2 == 0;
}
#else
#include <cerrno>
void socket_startup() {}
void socket_cleanup() {}
void close_socket(SOCKET s) { close(s); }
bool set_socket_timeout_ms(SOCKET s, int ms)
{
    timeval tv{};
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    int r1 = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    int r2 = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    return r1 == 0 && r2 == 0;
}
#endif

constexpr uint32_t kMagic = 0x43505031; // "CPP1"
constexpr uint32_t kMaxPayload = 64 * 1024;

bool send_all(SOCKET s, const char *data, size_t len)
{
    size_t sent_total = 0;
    while (sent_total < len)
    {
        int n = send(s, data + sent_total, (int)(len - sent_total), 0);
        if (n <= 0)
            return false;
        sent_total += (size_t)n;
    }
    return true;
}

bool recv_all(SOCKET s, char *data, size_t len)
{
    size_t got_total = 0;
    while (got_total < len)
    {
        int n = recv(s, data + got_total, (int)(len - got_total), 0);
        if (n <= 0)
            return false;
        got_total += (size_t)n;
    }
    return true;
}

bool send_packet(SOCKET s, const string &payload)
{
    uint32_t magic_n = htonl(kMagic);
    uint32_t len_n = htonl((uint32_t)payload.size());
    if (!send_all(s, (const char *)&magic_n, sizeof(magic_n)))
        return false;
    if (!send_all(s, (const char *)&len_n, sizeof(len_n)))
        return false;
    if (!payload.empty() && !send_all(s, payload.data(), payload.size()))
        return false;
    return true;
}

bool recv_packet(SOCKET s, string &payload)
{
    uint32_t magic_n = 0, len_n = 0;
    if (!recv_all(s, (char *)&magic_n, sizeof(magic_n)))
        return false;
    if (!recv_all(s, (char *)&len_n, sizeof(len_n)))
        return false;

    uint32_t magic = ntohl(magic_n);
    uint32_t len = ntohl(len_n);
    if (magic != kMagic || len > kMaxPayload)
        return false;

    payload.assign(len, '\0');
    if (len > 0 && !recv_all(s, payload.data(), len))
        return false;
    return true;
}

void handle_client(SOCKET client, sockaddr_in client_addr, atomic<int> &active_clients)
{
    // 10 seconds timeout to prevent hanging connections
    // 10秒超时，防止连接长期卡住
    set_socket_timeout_ms(client, 10000);

    char host[NI_MAXHOST]{};
    inet_ntop(AF_INET, &client_addr.sin_addr, host, sizeof(host));
    int port = ntohs(client_addr.sin_port);
    cout << "[session] start " << host << ":" << port << " active=" << active_clients.load() << "\n";

    while (true)
    {
        string request;
        if (!recv_packet(client, request))
        {
            cout << "[session] closed/timeout " << host << ":" << port << "\n";
            break;
        }

        cout << "[session] recv(" << host << ":" << port << "): " << request << "\n";
        if (request == "quit")
        {
            send_packet(client, "BYE");
            break;
        }

        string response = "OK|ECHO|" + request;
        if (!send_packet(client, response))
        {
            cout << "[session] send failed " << host << ":" << port << "\n";
            break;
        }
    }

    close_socket(client);
    --active_clients;
    cout << "[session] end " << host << ":" << port << " active=" << active_clients.load() << "\n";
}

int run_server(const char *port_str)
{
    socket_startup();
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, port_str, &hints, &res) != 0)
    {
        cerr << "getaddrinfo failed\n";
        return 1;
    }

    SOCKET listen_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listen_sock == INVALID_SOCKET)
    {
        cerr << "socket() failed\n";
        freeaddrinfo(res);
        return 1;
    }

    int yes = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

    if (bind(listen_sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
    {
        cerr << "bind() failed\n";
        close_socket(listen_sock);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }
    freeaddrinfo(res);

    if (listen(listen_sock, 5) == SOCKET_ERROR)
    {
        cerr << "listen() failed\n";
        close_socket(listen_sock);
        socket_cleanup();
        return 1;
    }
    cout << "TCP server listening on port " << port_str << "\n";
    cout << "Protocol: [magic(4)][length(4)][payload] magic=CPP1\n";

    atomic<int> active_clients{0};
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t addrlen = sizeof(client_addr);
        SOCKET client = accept(listen_sock, (sockaddr *)&client_addr, &addrlen);
        if (client == INVALID_SOCKET)
        {
            cerr << "accept() failed\n";
            break;
        }

        ++active_clients;
        // one thread per connection for learning clarity
        // 每个连接一个线程（学习版最直观）
        thread th(handle_client, client, client_addr, ref(active_clients));
        th.detach();
    }

    close_socket(listen_sock);
    socket_cleanup();
    return 0;
}

int run_client(const char *host, const char *port_str)
{
    socket_startup();
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port_str, &hints, &res) != 0)
    {
        cerr << "getaddrinfo failed\n";
        return 1;
    }

    SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET)
    {
        cerr << "socket() failed\n";
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
    {
        cerr << "connect failed\n";
        close_socket(sock);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }
    freeaddrinfo(res);
    // client 8 seconds send/recv timeout
    // 客户端8秒收发超时
    set_socket_timeout_ms(sock, 8000);

    cout << "Connected. type message, or type quit to exit.\n";
    while (true)
    {
        string msg;
        cout << "input message to send: ";
        getline(cin, msg);
        if (!send_packet(sock, msg))
        {
            cerr << "send packet failed\n";
            break;
        }

        string reply;
        if (!recv_packet(sock, reply))
        {
            cerr << "recv packet failed or timeout\n";
            break;
        }
        cout << "reply: " << reply << '\n';
        if (msg == "quit")
            break;
    }

    close_socket(sock);
    socket_cleanup();
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "Usage:\n  server: " << argv[0] << " server <port>\n  client: " << argv[0] << " client <host> <port>\n";
        return 0;
    }
    string mode = argv[1];
    if (mode == "server")
        return run_server(argv[2]);
    if (mode == "client")
    {
        if (argc < 4)
        {
            cerr << "client needs host and port\n";
            return 1;
        }
        return run_client(argv[2], argv[3]);
    }
    cerr << "unknown mode" << '\n';
    return 1;
}
