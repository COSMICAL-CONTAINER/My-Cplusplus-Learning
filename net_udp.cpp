#include <iostream>
#include <string>
#include <vector>
#include <cstring>

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
// Basic UDP socket demo
// =========================
// Goal:
// 1) Understand UDP is connectionless (no accept/connect required)
// 2) Use recvfrom/sendto to read peer address and reply
// 3) Keep logic minimal for comparison with the plus version
//
// compile:
//  g++ -std=c++17 .\net_udp.cpp -o .\net_udp.exe -lws2_32
//
// Usage:
//  Server: net_udp.exe server <port>
//  Client: net_udp.exe client <host> <port>
//
// Example:
//  Server: net_udp.exe server 1234
//  Client: net_udp.exe client 127.0.0.1 1234

// =========================
// 基础版 UDP socket 示例
// =========================
// 目标:
// 1) 理解 UDP 是无连接通信（无需 accept/connect）
// 2) 使用 recvfrom/sendto 获取并回复对端地址
// 3) 保持逻辑最小化，方便和 plus 版本做对比
//
// 编译:
//  g++ -std=c++17 .\net_udp.cpp -o .\net_udp.exe -lws2_32
//
// 用法:
//  服务器: net_udp.exe server <port>
//  客户端: net_udp.exe client <host> <port>
//
// 示例:
//  服务器: net_udp.exe server 1234
//  客户端: net_udp.exe client 127.0.0.1 1234

#if defined(_WIN32) || defined(_WIN64)
void socket_startup()
{
    // On Windows, Winsock must be initialized before socket APIs
    // Windows 下必须先初始化 Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}
void socket_cleanup()
{
    // Release Winsock resources before exit
    // 程序结束前释放 Winsock 资源
    WSACleanup();
}
void close_socket(SOCKET s) { closesocket(s); }
#else
void socket_startup() {}
void socket_cleanup() {}
void close_socket(SOCKET s) { close(s); }
#endif

int run_server(const char *port_str)
{
    socket_startup();

    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE;    // bind to all interfaces

    if (getaddrinfo(nullptr, port_str, &hints, &res) != 0)
    {
        cerr << "getaddrinfo failed\n";
        return 1;
    }

    SOCKET sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == INVALID_SOCKET)
    {
        cerr << "socket() failed\n";
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }

    if (bind(sockfd, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
    {
        cerr << "bind() failed\n";
        close_socket(sockfd);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }
    freeaddrinfo(res);

    cout << "[UDP basic] server listening on port " << port_str << "\n";
    cout << "tip: UDP keeps packet boundaries naturally.\n";
    cout << "tip: this basic version handles one datagram per request.\n";

    while (true)
    {
        vector<char> buf(2048);
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);

        // recvfrom 会同时告诉我们“谁发来的”，这是 UDP 常见写法
        // recvfrom also gives sender address, which is the typical UDP style
        int n = (int)recvfrom(sockfd, buf.data(), (int)buf.size() - 1, 0, (sockaddr *)&client_addr, &len);
        if (n <= 0)
        {
            continue;
        }

        buf[n] = '\0';
        char host[NI_MAXHOST]{};
        inet_ntop(AF_INET, &client_addr.sin_addr, host, sizeof(host));
        cout << "recv from " << host << ":" << ntohs(client_addr.sin_port)
             << " -> " << buf.data() << '\n';

        // 直接回发给刚才的来源地址（无连接状态）
        // send reply back to the sender address (connectionless)
        string reply = string("UDP ECHO: ") + buf.data();
        sendto(sockfd, reply.c_str(), (int)reply.size(), 0, (sockaddr *)&client_addr, len);
    }

    close_socket(sockfd);
    socket_cleanup();
    return 0;
}

int run_client(const char *host, const char *port_str)
{
    socket_startup();

    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host, port_str, &hints, &res) != 0)
    {
        cerr << "getaddrinfo failed\n";
        return 1;
    }

    SOCKET sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == INVALID_SOCKET)
    {
        cerr << "socket() failed\n";
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }

    string msg;
    cout << "input message to send: ";
    getline(cin, msg);

    // UDP 不保证送达，此处仅示范最小发送流程
    // UDP does not guarantee delivery; this shows minimal send flow
    int sent = (int)sendto(sockfd, msg.c_str(), (int)msg.size(), 0, res->ai_addr, (int)res->ai_addrlen);
    if (sent < 0)
    {
        cerr << "sendto failed\n";
        close_socket(sockfd);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }

    vector<char> buf(2048);
    sockaddr_in from{};
    socklen_t fromlen = sizeof(from);
    int n = (int)recvfrom(sockfd, buf.data(), (int)buf.size() - 1, 0, (sockaddr *)&from, &fromlen);
    if (n > 0)
    {
        buf[n] = '\0';
        cout << "reply: " << buf.data() << '\n';
    }

    close_socket(sockfd);
    freeaddrinfo(res);
    socket_cleanup();
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "Usage:\n"
             << "  server: " << argv[0] << " server <port>\n"
             << "  client: " << argv[0] << " client <host> <port>\n";
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

    cerr << "unknown mode\n";
    return 1;
}
