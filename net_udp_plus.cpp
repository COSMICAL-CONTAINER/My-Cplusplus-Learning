#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
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
// Advanced UDP socket demo (concurrency + timeout + simple protocol)
// =========================
// Protocol (single datagram frame):
// [magic:4bytes][length:4bytes][payload:length bytes]
// magic is fixed to 0x43505031 ("CPP1"), length is in network byte order.
//
// compile:
//  g++ -std=c++17 .\net_udp_plus.cpp -o .\net_udp_plus.exe -lws2_32
//
// Usage:
//  Server: net_udp_plus.exe server <port>
//  Client: net_udp_plus.exe client <host> <port>
//
// Example:
//  Server: net_udp_plus.exe server 1234
//  Client: net_udp_plus.exe client 127.0.0.1 1234

// =========================
// 进阶版 UDP socket 示例（并发处理 + 超时 + 简单协议）
// =========================
// 协议（单个 UDP 数据报中的二进制帧）:
// [magic:4bytes][length:4bytes][payload:length bytes]
// magic 固定为 0x43505031 ("CPP1")，length 为网络字节序。
//
// 编译:
//  g++ -std=c++17 .\net_udp_plus.cpp -o .\net_udp_plus.exe -lws2_32
//
// 用法:
//  服务器: net_udp_plus.exe server <port>
//  客户端: net_udp_plus.exe client <host> <port>
//
// 示例:
//  服务器: net_udp_plus.exe server 1234
//  客户端: net_udp_plus.exe client 127.0.0.1 1234

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
bool set_socket_timeout_ms(SOCKET s, int ms)
{
    // Set recv/send timeout in milliseconds
    // 设置收发超时（毫秒）
    DWORD timeout = (DWORD)ms;
    int r1 = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    int r2 = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    return r1 == 0 && r2 == 0;
}
bool is_timeout_error()
{
    // Identify timeout-style socket errors on Windows
    // 识别 Windows 下的超时类错误
    int e = WSAGetLastError();
    return e == WSAETIMEDOUT || e == WSAEWOULDBLOCK;
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
bool is_timeout_error()
{
    // Identify timeout-style socket errors on POSIX
    // 识别 POSIX 下的超时类错误
    return errno == EAGAIN || errno == EWOULDBLOCK;
}
#endif

constexpr uint32_t kMagic = 0x43505031; // "CPP1"
constexpr uint32_t kMaxPayload = 60 * 1024;

vector<char> encode_packet(const string &payload)
{
    // Frame = magic + length + payload
    // 帧结构 = 魔数 + 长度 + 负载
    vector<char> packet(8 + payload.size());
    uint32_t magic_n = htonl(kMagic);
    uint32_t len_n = htonl((uint32_t)payload.size());
    memcpy(packet.data(), &magic_n, 4);
    memcpy(packet.data() + 4, &len_n, 4);
    if (!payload.empty())
        memcpy(packet.data() + 8, payload.data(), payload.size());
    return packet;
}

bool decode_packet(const char *data, size_t n, string &payload)
{
    // Basic protocol validation
    // 基础协议校验
    if (n < 8)
        return false;
    uint32_t magic_n = 0, len_n = 0;
    memcpy(&magic_n, data, 4);
    memcpy(&len_n, data + 4, 4);
    uint32_t magic = ntohl(magic_n);
    uint32_t len = ntohl(len_n);
    if (magic != kMagic || len > kMaxPayload)
        return false;
    if (n != 8 + len)
        return false;
    payload.assign(data + 8, data + 8 + len);
    return true;
}

struct DatagramTask
{
    vector<char> data;
    sockaddr_in client;
    socklen_t client_len;
};

int run_server(const char *port_str)
{
    socket_startup();
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

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
        return 1;
    }

    if (bind(sockfd, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
    {
        cerr << "bind failed\n";
        close_socket(sockfd);
        freeaddrinfo(res);
        socket_cleanup();
        return 1;
    }
    freeaddrinfo(res);

    // Set 1 second timeout for recvfrom to allow periodic checks and graceful shutdown
    // 1秒超时，便于循环检查与优雅退出
    set_socket_timeout_ms(sockfd, 1000);
    cout << "UDP server listening on port " << port_str << "\n";
    cout << "Protocol: [magic(4)][length(4)][payload] magic=CPP1\n";

    queue<DatagramTask> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> running{true};

    auto worker = [&]()
    {
        while (running.load())
        {
            DatagramTask task;
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [&]()
                        { return !tasks.empty() || !running.load(); });
                if (!running.load() && tasks.empty())
                    return;
                task = std::move(tasks.front());
                tasks.pop();
            }

            string req;
            string resp;
            // Application-level message dispatch
            // 应用层消息分发
            if (!decode_packet(task.data.data(), task.data.size(), req))
            {
                resp = "ERR|BAD_PROTOCOL";
            }
            else if (req == "ping")
            {
                resp = "OK|pong";
            }
            else if (req == "quit")
            {
                resp = "BYE";
            }
            else
            {
                resp = "OK|UDP_ECHO|" + req;
            }

            auto packet = encode_packet(resp);
            sendto(sockfd, packet.data(), (int)packet.size(), 0, (sockaddr *)&task.client, task.client_len);
        }
    };

    thread w1(worker);
    thread w2(worker);

    while (true)
    {
        vector<char> buf(65535);
        sockaddr_in client{};
        socklen_t len = sizeof(client);
        int n = (int)recvfrom(sockfd, buf.data(), (int)buf.size(), 0, (sockaddr *)&client, &len);
        if (n < 0)
        {
            if (is_timeout_error())
                continue;
            cerr << "recvfrom failed\n";
            break;
        }
        if (n > 0)
        {
            char host[NI_MAXHOST];
            inet_ntop(AF_INET, &client.sin_addr, host, sizeof(host));
            cout << "recv datagram from " << host << ":" << ntohs(client.sin_port) << " bytes=" << n << '\n';

            DatagramTask task;
            task.data.assign(buf.begin(), buf.begin() + n);
            task.client = client;
            task.client_len = len;

            {
                lock_guard<mutex> lock(mtx);
                tasks.push(std::move(task));
            }
            cv.notify_one();
        }
    }

    running = false;
    cv.notify_all();
    if (w1.joinable())
        w1.join();
    if (w2.joinable())
        w2.join();

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
        return 1;
    }

    // Client waits up to 3 seconds for each reply
    // 客户端每次等待回复最长 3 秒
    set_socket_timeout_ms(sockfd, 3000);

    cout << "Connected (UDP no-connect style). type message, or type quit to exit.\n";
    while (true)
    {
        string msg;
        cout << "input message to send: ";
        getline(cin, msg);

        // Encode payload into protocol frame before sending
        // 发送前把消息编码成协议帧
        auto packet = encode_packet(msg);
        int sent = (int)sendto(sockfd, packet.data(), (int)packet.size(), 0, res->ai_addr, (int)res->ai_addrlen);
        if (sent < 0)
        {
            cerr << "sendto failed\n";
            break;
        }

        vector<char> buf(65535);
        sockaddr_in from{};
        socklen_t fromlen = sizeof(from);
        int n = (int)recvfrom(sockfd, buf.data(), (int)buf.size(), 0, (sockaddr *)&from, &fromlen);
        if (n < 0)
        {
            if (is_timeout_error())
            {
                cout << "timeout waiting for server reply\n";
                continue;
            }
            cerr << "recvfrom failed\n";
            break;
        }

        string reply;
        if (!decode_packet(buf.data(), (size_t)n, reply))
        {
            cout << "reply protocol decode failed\n";
        }
        else
        {
            cout << "reply: " << reply << '\n';
        }

        if (msg == "quit")
            break;
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
