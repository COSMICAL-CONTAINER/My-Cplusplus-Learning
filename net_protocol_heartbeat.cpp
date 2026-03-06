#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>

using namespace std;

// Protocol heartbeat mechanism learning example (logic simulation)
// No real socket is used, focus on demonstrating "message format, timeout check, cleanup strategy"
// 协议心跳机制学习示例(逻辑模拟)
// 不做真实 socket，重点展示"消息格式 + 超时检查 + 清理策略"

// 1) This file simulates an application-layer heartbeat protocol without real socket/network I/O;
// 2) It defines heartbeat message formats: "PING|client_id|timestamp" and "PONG|client_id|timestamp";
// 3) It stores each client's last heartbeat time and alive status in ConnectionState;
// 4) A timeout threshold (3 seconds) is used to detect stale connections;
// 5) In each tick, c1 always sends heartbeat; c2 stops sending after tick 3;
// 6) If elapsed time since last heartbeat exceeds timeout, the client is marked disconnected;
// 7) The final summary prints which clients are still alive or disconnected;

// 1) 该文件模拟应用层心跳协议，不做真实 socket/网络收发;
// 2) 它定义了心跳消息格式："PING|client_id|timestamp" 和 "PONG|client_id|timestamp";
// 3) 它在 ConnectionState 中保存每个客户端的最近心跳时间和存活状态;
// 4) 通过超时阈值（3 秒）来检测失活连接;
// 5) 每一轮中，c1 始终发送心跳；c2 在第 3 轮后停止发送;
// 6) 如果距离最近心跳的时间超过超时阈值，则将该客户端标记为断开;
// 7) 最后的 summary 会输出哪些客户端仍在线、哪些已断开;

// Typical use cases: IM/WebSocket long connections; online game gateways; service discovery in microservices; IoT device liveness checks; financial/trading channel health detection;
// 典型应用场景：即时通信/WebSocket 长连接；在线游戏网关；微服务注册发现；物联网设备在线检测；金融/交易通道健康探测;

struct ConnectionState
{
    string id;
    chrono::steady_clock::time_point last_heartbeat;
    bool alive = true;
};

string make_ping(const string &client_id, long long ts_ms)
{
    return "PING|" + client_id + "|" + to_string(ts_ms);
}

string make_pong(const string &client_id, long long ts_ms)
{
    return "PONG|" + client_id + "|" + to_string(ts_ms);
}

int main()
{
    cout << "=== heartbeat protocol simulation ===\n";

    unordered_map<string, ConnectionState> clients;
    auto now = chrono::steady_clock::now();
    clients["c1"] = {"c1", now, true};
    clients["c2"] = {"c2", now, true};

    const auto timeout = chrono::seconds(3);

    for (int tick = 1; tick <= 8; ++tick)
    {
        auto t = chrono::steady_clock::now();
        long long ts_ms = chrono::duration_cast<chrono::milliseconds>(t.time_since_epoch()).count();

        cout << "\n[tick " << tick << "]\n";

        // c1 always sends heartbeat; c2 stops after tick 3
        // 模拟 c1 每次都发心跳，c2 从第4轮开始不再发
        string ping1 = make_ping("c1", ts_ms);
        (void)ping1;
        clients["c1"].last_heartbeat = t;
        cout << "recv " << make_ping("c1", ts_ms) << " -> " << make_pong("c1", ts_ms) << '\n';

        if (tick <= 3)
        {
            clients["c2"].last_heartbeat = t;
            cout << "recv " << make_ping("c2", ts_ms) << " -> " << make_pong("c2", ts_ms) << '\n';
        }

        for (auto &[id, state] : clients)
        {
            if (!state.alive)
                continue;
            auto elapsed = chrono::duration_cast<chrono::seconds>(t - state.last_heartbeat);
            if (elapsed > timeout)
            {
                state.alive = false;
                cout << "[cleanup] " << id << " timed out, mark as disconnected\n";
            }
            else
            {
                cout << "[alive] " << id << " elapsed=" << elapsed.count() << "s\n";
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "\nsummary:\n";
    for (const auto &[id, state] : clients)
    {
        cout << id << " -> " << (state.alive ? "alive" : "disconnected") << '\n';
    }
    return 0;
}
