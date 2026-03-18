#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/*
Decorator Pattern Notes (Overview)

Decorator is a structural design pattern that lets you attach new behavior to objects
by placing them inside special wrapper objects that implement the same interface.
Instead of creating many subclasses for every feature combination, you can compose
behavior dynamically at runtime by stacking decorators.

Core idea:
1) Define a common Component interface.
2) Keep a Concrete Component with baseline behavior.
3) Create a Base Decorator that stores a wrapped component reference.
4) Create Concrete Decorators that add behavior before/after delegation.

When to use Decorator:
1) You need to add/remove responsibilities at runtime.
2) Inheritance-based feature combinations are exploding in number.
3) You want to keep client code working against one stable interface.

Advantages:
1) Extends behavior without creating many subclasses.
2) Supports runtime composition of multiple features.
3) Follows SRP by splitting independent concerns into decorators.
4) Keeps open for extension (OCP) through new decorators.

Trade-offs:
1) Deep wrapper stacks can make debugging harder.
2) Removing a specific middle decorator at runtime can be tricky.
3) Initialization/configuration code may become verbose.

Relationship with other patterns:
1) Adapter changes interface; Decorator keeps interface and adds behavior.
2) Proxy keeps same interface for access control/lifecycle; Decorator adds responsibilities.
3) Composite and Decorator both use recursive composition but with different intent.
4) Often combined with Composite to enhance selected nodes in a tree.
*/

/*
装饰模式笔记（概览）

装饰（Decorator）是一种结构型设计模式，它通过“包装对象”的方式为对象动态添加
额外行为。装饰器与被包装对象实现相同接口，因此客户端通常无须感知自己操作的是
原始对象还是经过多层装饰后的对象。

核心思想：
1）定义统一组件接口（Component）。
2）保留一个基础具体组件（Concrete Component）。
3）定义装饰基类，内部持有被包装组件引用。
4）具体装饰在委派前后附加行为。

适用场景：
1）需要在运行时动态叠加或移除职责。
2）使用继承扩展导致组合子类数量爆炸。
3）希望客户端始终只依赖稳定接口。

优点：
1）无需大量子类即可扩展行为。
2）支持多种功能按需组合。
3）符合单一职责原则（SRP），不同职责可拆分为独立装饰。
4）符合开闭原则（OCP），新增行为可通过新增装饰类完成。

缺点：
1）装饰层级很深时，调试和排错会更复杂。
2）运行时精确移除某一层装饰不太容易。
3）对象装配代码可能变得冗长。

与其他模式关系：
1）适配器会改接口；装饰器不改接口，只增强行为。
2）代理通常用于访问控制/生命周期管理；装饰用于增强职责。
3）组合与装饰都基于递归组合，但设计意图不同。
4）装饰常与组合配合，对树中部分节点做行为增强。
*/

/*
Scenario Explanation:

This file uses a notification library scenario.
Notifier is the base component that sends email notifications. Decorators such as
SmsDecorator, WeChatDecorator, and QQDecorator wrap a notifier and add extra channels.
Client code can build different notification stacks at runtime (for example,
QQ -> WeChat -> SMS -> Email) without changing existing notifier classes.
*/

/*
场景说明：

本文件使用“通知库”场景讲解装饰模式。
Notifier 是基础组件，提供邮件通知；SmsDecorator、WeChatDecorator、
QQDecorator 等装饰器在包装 Notifier 后为其叠加新的通知渠道。
客户端可在运行时自由组装通知栈（例如 QQ -> 微信 -> 短信 -> 邮件），
而无需修改已有通知类。
*/

// Component interface shared by concrete component and decorators.
// 组件接口：基础组件与装饰器都实现同一接口。
class INotifier
{
public:
    virtual ~INotifier() = default;
    virtual void send(const string &message) = 0;
    virtual string channels() const = 0;
};

// Concrete component: baseline behavior (email notification).
// 具体组件：基础行为（邮件通知）。
class EmailNotifier : public INotifier
{
public:
    void send(const string &message) override
    {
        cout << "[Email] " << message << "\n";
    }

    string channels() const override
    {
        return "Email";
    }
};

// Base decorator: forwards calls to wrapped component.
// 装饰基类：将调用委派给被包装组件。
class NotifierDecorator : public INotifier
{
public:
    explicit NotifierDecorator(unique_ptr<INotifier> wrappedNotifier)
        : wrapped(std::move(wrappedNotifier)) {}

    void send(const string &message) override
    {
        wrapped->send(message);
    }

    string channels() const override
    {
        return wrapped->channels();
    }

protected:
    unique_ptr<INotifier> wrapped;
};

// Concrete decorator: add SMS channel.
// 具体装饰：增加短信渠道。
class SmsDecorator : public NotifierDecorator
{
public:
    using NotifierDecorator::NotifierDecorator;

    void send(const string &message) override
    {
        NotifierDecorator::send(message);
        cout << "[SMS] " << message << "\n";
    }

    string channels() const override
    {
        return NotifierDecorator::channels() + " + SMS";
    }
};

// Concrete decorator: add WeChat channel.
// 具体装饰：增加微信渠道。
class WeChatDecorator : public NotifierDecorator
{
public:
    using NotifierDecorator::NotifierDecorator;

    void send(const string &message) override
    {
        NotifierDecorator::send(message);
        cout << "[WeChat] " << message << "\n";
    }

    string channels() const override
    {
        return NotifierDecorator::channels() + " + WeChat";
    }
};

// Concrete decorator: add QQ channel.
// 具体装饰：增加 QQ 渠道。
class QQDecorator : public NotifierDecorator
{
public:
    using NotifierDecorator::NotifierDecorator;

    void send(const string &message) override
    {
        NotifierDecorator::send(message);
        cout << "[QQ] " << message << "\n";
    }

    string channels() const override
    {
        return NotifierDecorator::channels() + " + QQ";
    }
};

int main()
{
    // Basic notifier (email only).
    // 基础通知器（仅邮件）。
    unique_ptr<INotifier> basicNotifier = make_unique<EmailNotifier>();
    cout << "Channels: " << basicNotifier->channels() << "\n";
    basicNotifier->send("Server is up.");

    cout << "\n";

    // Decorated notifier stack built at runtime.
    // 运行时组装多层装饰。
    unique_ptr<INotifier> emergencyNotifier = make_unique<QQDecorator>(
        make_unique<WeChatDecorator>(
            make_unique<SmsDecorator>(
                make_unique<EmailNotifier>())));

    cout << "Channels: " << emergencyNotifier->channels() << "\n";
    emergencyNotifier->send("Critical alert: Database unreachable!");

    return 0;
}
