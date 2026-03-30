// ============================================================
// Observer Pattern Notes (Event-Subscriber / Listener)
// ============================================================
// Intent:
// Observer is a behavioral pattern that defines a subscription mechanism.
// When publisher state changes, all interested subscribers are notified.
//
// Keywords:
// Observer, Subscriber, Listener, Event-Subscriber, Publisher, Subject
//
// Problem:
// If clients poll state repeatedly, they waste time/resources.
// If publisher broadcasts blindly to everyone, it causes noisy notifications.
//
// Solution:
// 1) Publisher maintains subscriber lists.
// 2) Subscribers can subscribe/unsubscribe dynamically.
// 3) Publisher notifies only subscribers of the matching event.
//
// Advantages:
// 1) Open/Closed Principle: add new subscribers without modifying publisher.
// 2) Runtime dynamic relationships.
// 3) Decouples sender and receivers through interfaces.
//
// Trade-offs:
// 1) Notification order is usually unspecified.
// 2) Too many subscribers may impact performance.
// 3) Event chaining may make debugging harder.
//
// Relation to other patterns:
// - Mediator centralizes communication; Observer creates distributed subscriptions.
// - Command can be combined with Observer for event-driven undo/redo actions.
//
// ============================================================
// 观察者模式笔记（事件订阅者 / 监听者）
// ============================================================
// 意图：
// 观察者是一种行为型模式，定义订阅机制。
// 当发布者状态变化时，自动通知所有感兴趣的订阅者。
//
// 关键词：
// Observer、Subscriber、Listener、Publisher、Subject
//
// 问题：
// 如果客户端频繁轮询状态，会浪费大量时间与资源。
// 如果发布者无差别群发通知，又会造成噪声和骚扰。
//
// 解决方案：
// 1）发布者维护订阅列表。
// 2）订阅者可动态订阅与取消订阅。
// 3）按事件类型精准通知，避免无关消息。
//
// 优点：
// 1）符合开闭原则：新增订阅者无需改发布者核心逻辑。
// 2）支持运行时动态连接关系。
// 3）通过接口解耦发送方与接收方。
//
// 缺点：
// 1）通知顺序通常不保证。
// 2）订阅者过多时有性能压力。
// 3）事件链较长时调试复杂度上升。
//
// 与其他模式关系：
// - 中介者强调中心化协调；观察者强调分布式订阅。
// - 可与命令模式结合实现事件驱动的撤销/重做。

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Scenario Explanation:

We model a text editor with two events: open and save.
Different services subscribe to specific events only:
1) LoggingListener subscribes to open events.
2) EmailAlertsListener subscribes to save events.

This demonstrates precise notifications and runtime subscribe/unsubscribe.
*/

/*
场景说明：

我们用“文本编辑器”作为示例，定义两类事件：open 和 save。
不同服务只订阅自己关心的事件：
1）LoggingListener 订阅 open 事件。
2）EmailAlertsListener 订阅 save 事件。

该示例展示了按事件精准通知，以及运行时动态订阅/取消订阅。
*/

class EventListener
{
public:
    virtual ~EventListener() = default;
    virtual void update(const string &eventType, const string &data) = 0;
};

class EventManager
{
public:
    int subscribe(const string &eventType, EventListener *listener)
    {
        int id = nextId++;
        listeners[eventType].push_back({id, listener});
        return id;
    }

    void unsubscribe(const string &eventType, int subscriptionId)
    {
        auto it = listeners.find(eventType);
        if (it == listeners.end())
        {
            return;
        }

        auto &bucket = it->second;
        bucket.erase(
            remove_if(bucket.begin(), bucket.end(),
                      [subscriptionId](const Subscription &entry)
                      {
                          return entry.id == subscriptionId;
                      }),
            bucket.end());
    }

    void notify(const string &eventType, const string &data)
    {
        auto it = listeners.find(eventType);
        if (it == listeners.end())
        {
            return;
        }

        for (const auto &entry : it->second)
        {
            if (entry.listener != nullptr)
            {
                entry.listener->update(eventType, data);
            }
        }
    }

private:
    struct Subscription
    {
        int id;
        EventListener *listener;
    };

    unordered_map<string, vector<Subscription>> listeners;
    int nextId = 1;
};

class Editor
{
public:
    EventManager events;

    void openFile(const string &path)
    {
        currentFile = path;
        cout << "[Editor] open file: " << currentFile << "\n";
        events.notify("open", currentFile);
    }

    void saveFile()
    {
        if (currentFile.empty())
        {
            cout << "[Editor] no file is opened, save skipped\n";
            return;
        }

        cout << "[Editor] save file: " << currentFile << "\n";
        events.notify("save", currentFile);
    }

private:
    string currentFile;
};

class LoggingListener : public EventListener
{
public:
    LoggingListener(string logFilePathValue, string messageTemplateValue)
        : logFilePath(std::move(logFilePathValue)),
          messageTemplate(std::move(messageTemplateValue)) {}

    void update(const string &eventType, const string &data) override
    {
        string message = messageTemplate;
        replaceToken(message, "%event%", eventType);
        replaceToken(message, "%file%", data);

        cout << "[LoggingListener -> " << logFilePath << "] " << message << "\n";
    }

private:
    static void replaceToken(string &target, const string &token, const string &value)
    {
        size_t pos = target.find(token);
        while (pos != string::npos)
        {
            target.replace(pos, token.size(), value);
            pos = target.find(token, pos + value.size());
        }
    }

private:
    string logFilePath;
    string messageTemplate;
};

class EmailAlertsListener : public EventListener
{
public:
    EmailAlertsListener(string emailValue, string messageTemplateValue)
        : email(std::move(emailValue)),
          messageTemplate(std::move(messageTemplateValue)) {}

    void update(const string &eventType, const string &data) override
    {
        string message = messageTemplate;
        replaceToken(message, "%event%", eventType);
        replaceToken(message, "%file%", data);

        cout << "[EmailListener -> " << email << "] " << message << "\n";
    }

private:
    static void replaceToken(string &target, const string &token, const string &value)
    {
        size_t pos = target.find(token);
        while (pos != string::npos)
        {
            target.replace(pos, token.size(), value);
            pos = target.find(token, pos + value.size());
        }
    }

private:
    string email;
    string messageTemplate;
};

int main()
{
    Editor editor;

    LoggingListener logger(
        "app.log",
        "event=%event%, someone opened file %file%");
    EmailAlertsListener emailAlerts(
        "admin@example.com",
        "event=%event%, someone modified and saved file %file%");

    int openLoggerSubscriptionId = editor.events.subscribe("open", &logger);
    int saveEmailSubscriptionId = editor.events.subscribe("save", &emailAlerts);

    editor.openFile("design_pattern_notes.md");
    editor.saveFile();

    cout << "\n--- unsubscribe email listener from save ---\n";
    editor.events.unsubscribe("save", saveEmailSubscriptionId);
    editor.saveFile();

    cout << "\n--- unsubscribe logger from open ---\n";
    editor.events.unsubscribe("open", openLoggerSubscriptionId);
    editor.openFile("observer_pattern_summary.md");

    return 0;
}