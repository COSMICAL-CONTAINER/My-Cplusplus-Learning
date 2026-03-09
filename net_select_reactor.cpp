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
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
using SOCKET = int;
#endif

using namespace std;

// 1) This file demonstrates a select-based Reactor server and a tiny client in one executable;
// 2) Server mode: one thread manages listen socket + all clients, then echoes "[reactor] " + request;
// 3) Client mode: connect to server, send one message, read one response, then exit;
// 4) This solves the "server seems no response" confusion: server waits for external client traffic;
// 5) Build (Windows/MinGW): g++ net_select_reactor.cpp -o net_select_reactor.exe -lws2_32;
// 6) Run server: net_select_reactor.exe server 1234;
// 7) Run client: net_select_reactor.exe client 127.0.0.1 1234 "hello reactor";

// 1) 该文件在一个可执行程序里同时演示 select Reactor 服务端和一个简易客户端;
// 2) 服务端模式: 单线程管理监听 socket 与多个客户端, 并回显 "[reactor] " + 请求内容;
// 3) 客户端模式: 连接服务端, 发送一条消息, 读取一条响应后退出;
// 4) 这样可以解决“服务端看起来没反应”的困惑: 服务端本质上是在等待外部客户端流量;
// 5) 编译命令 (Windows/MinGW): g++ net_select_reactor.cpp -o net_select_reactor.exe -lws2_32;
// 6) 启动服务端: net_select_reactor.exe server 1234;
// 7) 启动客户端: net_select_reactor.exe client 127.0.0.1 1234 "hello reactor";

#if defined(_WIN32) || defined(_WIN64)
void socket_startup()
{
    WSADATA wsa{};
    WSAStartup(MAKEWORD(2, 2), &wsa);
}
void socket_cleanup() { WSACleanup(); }
void close_socket(SOCKET s) { closesocket(s); }
#else
void socket_startup() {}
void socket_cleanup() {}
void close_socket(SOCKET s) { close(s); }
#endif

void print_usage(const char *prog)
{
    cout << "usage:\n";
    cout << "  " << prog << " server <port>\n";
    cout << "  " << prog << " client <host> <port> <message>\n";
    cout << "examples:\n";
    cout << "  " << prog << " server 1234\n";
    cout << "  " << prog << " client 127.0.0.1 1234 \"hello reactor\"\n";
}

int run_server(const char *port_str)
{
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        cerr << "socket failed\n";
        return 1;
    }

    int yes = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)stoi(port_str));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        cerr << "bind failed\n";
        close_socket(listen_sock);
        return 1;
    }

    if (listen(listen_sock, 8) == SOCKET_ERROR)
    {
        cerr << "listen failed\n";
        close_socket(listen_sock);
        return 1;
    }

    cout << "reactor server listening on port " << port_str << '\n';
    cout << "echo protocol: plain text, server replies with [reactor] prefix\n";

    vector<SOCKET> clients;

    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(listen_sock, &readfds);
        SOCKET max_fd = listen_sock;

        for (SOCKET c : clients)
        {
            FD_SET(c, &readfds);
            if (c > max_fd)
                max_fd = c;
        }

        timeval tv{};
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ready = select((int)max_fd + 1, &readfds, nullptr, nullptr, &tv);
        if (ready < 0)
        {
            cerr << "select failed\n";
            break;
        }

        if (ready == 0)
        {
            cout << "[idle] waiting for new connection or client data...\n";
            continue;
        }

        if (FD_ISSET(listen_sock, &readfds))
        {
            sockaddr_in client_addr{};
            socklen_t len = sizeof(client_addr);
            SOCKET c = accept(listen_sock, (sockaddr *)&client_addr, &len);
            if (c != INVALID_SOCKET)
            {
                clients.push_back(c);
                cout << "new client, total=" << clients.size() << '\n';
            }
        }

        for (size_t i = 0; i < clients.size();)
        {
            SOCKET c = clients[i];
            if (!FD_ISSET(c, &readfds))
            {
                ++i;
                continue;
            }

            char buf[1024]{};
            int n = recv(c, buf, sizeof(buf) - 1, 0);
            if (n <= 0)
            {
                close_socket(c);
                clients.erase(clients.begin() + (long long)i);
                cout << "client disconnected, total=" << clients.size() << '\n';
                continue;
            }

            string req(buf, buf + n);
            cout << "recv: " << req << '\n';
            string resp = "[reactor] " + req;
            send(c, resp.c_str(), (int)resp.size(), 0);
            ++i;
        }
    }

    for (SOCKET c : clients)
        close_socket(c);
    close_socket(listen_sock);
    return 0;
}

int run_client(const char *host, const char *port_str, const char *message)
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short)stoi(port_str));
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0)
    {
        cerr << "invalid host address\n";
        close_socket(s);
        return 1;
    }

    if (connect(s, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        cerr << "connect failed\n";
        close_socket(s);
        return 1;
    }

    string req = message;
    if (send(s, req.c_str(), (int)req.size(), 0) <= 0)
    {
        cerr << "send failed\n";
        close_socket(s);
        return 1;
    }

    char buf[1024]{};
    int n = recv(s, buf, sizeof(buf) - 1, 0);
    if (n <= 0)
    {
        cerr << "recv failed\n";
        close_socket(s);
        return 1;
    }

    cout << "client recv: " << string(buf, buf + n) << '\n';
    close_socket(s);
    return 0;
}

int main(int argc, char **argv)
{
    socket_startup();

    int rc = 0;
    if (argc >= 2 && string(argv[1]) == "server")
    {
        const char *port_str = (argc >= 3 ? argv[2] : "9010");
        rc = run_server(port_str);
    }
    else if (argc >= 2 && string(argv[1]) == "client")
    {
        if (argc < 5)
        {
            print_usage(argv[0]);
            socket_cleanup();
            return 1;
        }
        rc = run_client(argv[2], argv[3], argv[4]);
    }
    else
    {
        print_usage(argv[0]);
        socket_cleanup();
        return 1;
    }

    socket_cleanup();
    return rc;
}
