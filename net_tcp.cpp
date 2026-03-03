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
// Basic TCP socket demo
// =========================
// Goal:
// 1) Learn the basic socket/bind/listen/accept/recv/send call flow
// 2) Understand TCP is connection-oriented (connect first, then send/recv)
// 3) Keep the code short for easy comparison with the plus version
//
// compile:
//  g++ -std=c++17 .\net_tcp.cpp -o .\net_tcp.exe -lws2_32
//
// Usage:
//  Server: net_tcp.exe server <port>
//  Client: net_tcp.exe client <host> <port>
//
// Example:
//  Server: net_tcp.exe server 1234
//  Client: net_tcp.exe client 127.0.0.1 1234

// =========================
// 基础版 TCP socket 示例
// =========================
// 目标:
// 1) 学会最底层的 socket / bind / listen / accept / recv / send 调用流程
// 2) 理解 TCP 是面向连接的（先连接，再收发）
// 3) 保持代码短小，便于与 plus 版本对照学习
//
// 编译:
//  g++ -std=c++17 .\net_tcp.cpp -o .\net_tcp.exe -lws2_32
//
// 用法:
//  服务器: net_tcp.exe server <port>
//  客户端: net_tcp.exe client <host> <port>
//
// 示例:
//  服务器: net_tcp.exe server 1234
//  客户端: net_tcp.exe client 127.0.0.1 1234

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
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // bind to all interfaces

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
        socket_cleanup();
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

    cout << "[TCP basic] server listening on port " << port_str << "\n";
    cout << "tip: this basic version handles one message per connection.\n";

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

        char host[NI_MAXHOST]{};
        inet_ntop(AF_INET, &client_addr.sin_addr, host, sizeof(host));
        cout << "accept from " << host << ":" << ntohs(client_addr.sin_port) << "\n";

        // Read only once to keep the basic request-response flow simple
        // 这里只读一次消息，帮助初学者理解"请求-响应"最小流程
        vector<char> buf(1024);
        int n = (int)recv(client, buf.data(), (int)buf.size() - 1, 0);
        if (n > 0)
        {
            buf[n] = '\0';
            cout << "recv: " << buf.data() << "\n";

            string reply = string("ECHO: ") + buf.data();
            send(client, reply.c_str(), (int)reply.size(), 0);
        }

        close_socket(client);
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
        socket_cleanup();
        return 1;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
    {
        cerr << "connect() failed\n";
        close_socket(sock);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }
    freeaddrinfo(res);

    string msg;
    cout << "input message to send: ";
    getline(cin, msg);

    send(sock, msg.c_str(), (int)msg.size(), 0);

    vector<char> buf(1024);
    int n = (int)recv(sock, buf.data(), (int)buf.size() - 1, 0);
    if (n > 0)
    {
        buf[n] = '\0';
        cout << "reply: " << buf.data() << '\n';
    }

    close_socket(sock);
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
