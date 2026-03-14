#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

/*
Singleton Pattern Notes (Overview)

Singleton is a creational design pattern that ensures a class has only one instance and
provides a global access point to that instance. It is commonly used to control access to
shared resources (for example, logger, config center, connection manager), where creating
multiple instances would be incorrect or wasteful.

Core implementation idea:
1) Hide constructor from client code.
2) Expose a static access function (getInstance).
3) Keep and return the same instance on every call.

When to use Singleton:
1) Exactly one shared instance must exist in the process.
2) You need centralized, controlled global access.
3) Instance creation is expensive or state must be consistent globally.

Advantages:
1) Guarantees a single instance.
2) Provides a known global access point.
3) Supports lazy initialization in common implementations.

Trade-offs:
1) Violates SRP (lifecycle control + business logic in one class).
2) Can hide bad architecture (excessive global state coupling).
3) Testing can become harder due to global mutable state.
4) Requires thread-safe initialization in concurrent contexts.

Relationship with other patterns:
1) Facade is sometimes implemented as Singleton when one facade is sufficient.
2) Abstract Factory / Builder / Prototype can use Singleton factories or registries.
3) Different from Flyweight: Singleton is one instance; Flyweight has many shared immutable objects.
*/

/*
单例模式笔记（概览）

单例（Singleton）是一种创建型设计模式，用于保证某个类在进程中只有一个实例，
并提供一个全局访问点。它常用于访问共享资源（如日志器、配置中心、连接管理器），
当创建多个实例会导致不一致或浪费时，单例就很有价值。

核心实现思路：
1）隐藏构造函数，禁止外部直接创建。
2）提供静态访问方法（getInstance）。
3）每次调用都返回同一个实例。

适用场景：
1）系统中确实只应存在一个共享实例。
2）需要集中化、可控的全局访问入口。
3）对象初始化昂贵，或全局状态必须一致。

优点：
1）保证唯一实例。
2）提供明确的全局访问点。
3）常见实现支持延迟初始化。

缺点：
1）违反单一职责原则（生命周期管理和业务逻辑耦合）。
2）可能掩盖架构问题（全局可变状态过多）。
3）单元测试可能更困难。
4）并发场景需要确保线程安全初始化。

与其他模式关系：
1）外观模式在某些系统中可用单例承载。
2）抽象工厂/生成器/原型的工厂或注册表可做成单例。
3）与享元不同：单例强调“唯一实例”，享元强调“多个可共享的细粒度对象”。
*/

/*
Scenario Explanation:

This file explains Singleton with a database connection manager scenario.
DatabaseConnection exposes getInstance() as the only global access point, so all callers
share one connection object. The example uses C++11+ local static initialization (Meyers
Singleton) for thread-safe lazy initialization, then verifies in multi-threaded calls that
the same instance address is reused.
*/

/*
场景说明：

本文件用“数据库连接管理器”场景讲解单例模式。
DatabaseConnection 通过 getInstance() 提供唯一全局访问点，让所有调用方共享同一
连接对象。示例采用 C++11+ 的函数内静态变量（Meyers Singleton）实现线程安全的
懒加载初始化，并通过多线程调用验证同一实例地址会被重复复用。
*/

// Singleton with thread-safe lazy initialization in C++11+.
// C++11+ 线程安全懒加载单例（Meyers Singleton）。
class DatabaseConnection
{
public:
    // Global access point.
    // 全局访问点。
    static DatabaseConnection &getInstance()
    {
        static DatabaseConnection instance;
        return instance;
    }

    // Demo business method.
    // 示例业务方法。
    void query(const string &sql)
    {
        lock_guard<mutex> lock(ioMutex);
        cout << "[DatabaseConnection #" << connectionId << "] " << sql << "\n";
    }

    int id() const
    {
        return connectionId;
    }

    // Prevent copy/move to preserve singleton identity.
    // 禁止拷贝与移动，保持单例唯一性。
    DatabaseConnection(const DatabaseConnection &) = delete;
    DatabaseConnection &operator=(const DatabaseConnection &) = delete;
    DatabaseConnection(DatabaseConnection &&) = delete;
    DatabaseConnection &operator=(DatabaseConnection &&) = delete;

private:
    DatabaseConnection()
    {
        // Simulate expensive initialization.
        // 模拟昂贵初始化。
        this_thread::sleep_for(chrono::milliseconds(120));
        connectionId = 2026;
    }

    int connectionId = 0;
    mutable mutex ioMutex;
};

void workerTask(const string &workerName, const string &sql)
{
    static mutex printMutex;

    this_thread::sleep_for(chrono::milliseconds(30));
    DatabaseConnection &db = DatabaseConnection::getInstance();

    {
        lock_guard<mutex> lock(printMutex);
        cout << workerName << " gets instance address: " << &db
             << ", id=" << db.id() << "\n";
    }

    db.query(sql);
}

int main()
{
    cout << "Singleton demo (thread-safe):\n";

    thread t1(workerTask, "Worker-A", "SELECT * FROM users;");
    thread t2(workerTask, "Worker-B", "SELECT * FROM orders;");

    t1.join();
    t2.join();

    DatabaseConnection &a = DatabaseConnection::getInstance();
    DatabaseConnection &b = DatabaseConnection::getInstance();

    cout << "\nMain checks same instance: "
         << ((&a == &b) ? "YES" : "NO")
         << " (" << &a << " vs " << &b << ")\n";

    return 0;
}
