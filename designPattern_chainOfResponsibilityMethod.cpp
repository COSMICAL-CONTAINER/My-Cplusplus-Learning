#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Chain of Responsibility Pattern Notes (Overview)

Chain of Responsibility (CoR) is a behavioral design pattern that passes a request along
a chain of handlers. Each handler decides whether to process the request, pass it to the
next handler, or stop the chain.

Core idea:
1) Define a common Handler interface.
2) Each handler keeps a reference to the next one.
3) Handlers run in sequence and can terminate propagation.

When to use CoR:
1) A request may require multiple checks in a configurable order.
2) Different requests may be handled by different handlers.
3) You want to add/remove/reorder processing steps without changing client code.

Advantages:
1) Decouples sender from concrete receivers (SRP-friendly structure).
2) Supports runtime composition of processing pipelines.
3) Follows OCP by adding new handlers with minimal impact.

Trade-offs:
1) Some requests may pass through many handlers and add overhead.
2) Requests might end up unhandled if chain is misconfigured.
3) Debugging can be harder when flow depends on chain order.

Relationship with other patterns:
1) Similar recursive composition shape to Decorator, but CoR can stop propagation.
2) Often combined with Composite for event bubbling through object trees.
3) Can be combined with Command where request objects encapsulate operations.
*/

/*
责任链模式笔记（概览）

责任链（Chain of Responsibility, CoR）是一种行为型设计模式，
它把请求沿着“处理者链”依次传递。每个处理者都可以选择：
处理请求、继续传递请求、或中断链路。

核心思想：
1）定义统一处理者接口。
2）每个处理者保存下一个处理者引用。
3）处理者按顺序执行，并可随时终止传递。

适用场景：
1）一个请求需要按可配置顺序经过多个检查步骤。
2）不同请求可能由不同处理者处理。
3）希望在不改客户端代码的前提下增删改处理步骤。

优点：
1）发送者与接收者解耦，结构更清晰。
2）支持运行时组装处理流水线。
3）符合开闭原则（OCP），新增处理者成本低。

缺点：
1）请求可能经过较长链路，带来一定开销。
2）链配置不当时，部分请求可能无人处理。
3）当顺序影响结果时，调试复杂度上升。

与其他模式关系：
1）与装饰结构相似，但责任链可中断传递。
2）常与组合模式结合，实现对象树中的事件冒泡。
3）可与命令模式结合，请求对象可封装操作。
*/

/*
Scenario Explanation:

This file uses an online ordering security pipeline scenario.
An incoming order request passes through multiple handlers: rate limit check,
authentication, payload sanitization, role authorization, and cache lookup.
Any handler can stop processing on failure; otherwise it forwards to the next handler.
This keeps each rule isolated and lets the chain be reordered or extended easily.
*/

/*
场景说明：

本文件使用“在线订购系统安全检查流水线”场景。
订单请求依次经过多个处理者：限流检查、身份认证、数据清洗、权限校验、缓存检查。
任意一步失败都可立即中断链路；若通过则继续传递给下一处理者。
这样每条规则都能独立维护，且链路可按需重排或扩展。
*/

struct OrderRequest
{
    string username;
    string password;
    string ip;
    string payload;
    bool requiresAdmin = false;
};

// Handler interface
// 处理者接口
class Handler
{
public:
    virtual ~Handler() = default;
    virtual Handler *setNext(Handler *next) = 0;
    virtual bool handle(OrderRequest &request) = 0;
};

// Base handler with default pass-through behavior.
// 基础处理者：默认继续传递。
class BaseHandler : public Handler
{
public:
    Handler *setNext(Handler *next) override
    {
        nextHandler = next;
        return next;
    }

    bool handle(OrderRequest &request) override
    {
        if (nextHandler)
        {
            return nextHandler->handle(request);
        }
        return true;
    }

protected:
    Handler *nextHandler = nullptr;
};

class RateLimitHandler : public BaseHandler
{
public:
    bool handle(OrderRequest &request) override
    {
        int &count = failedAttemptsByIp[request.ip];
        if (count >= maxAttempts)
        {
            cout << "[RateLimit] blocked IP: " << request.ip << "\n";
            return false;
        }

        return BaseHandler::handle(request);
    }

    void recordFailure(const string &ip)
    {
        failedAttemptsByIp[ip]++;
    }

private:
    unordered_map<string, int> failedAttemptsByIp;
    int maxAttempts = 3;
};

class AuthHandler : public BaseHandler
{
public:
    explicit AuthHandler(RateLimitHandler &limiter) : rateLimiter(limiter) {}

    bool handle(OrderRequest &request) override
    {
        if (request.username != "alice" || request.password != "123456")
        {
            cout << "[Auth] failed for user: " << request.username << "\n";
            rateLimiter.recordFailure(request.ip);
            return false;
        }

        cout << "[Auth] passed\n";
        return BaseHandler::handle(request);
    }

private:
    RateLimitHandler &rateLimiter;
};

class SanitizationHandler : public BaseHandler
{
public:
    bool handle(OrderRequest &request) override
    {
        // Demo sanitization: reject suspicious marker.
        // 示例清洗：拒绝可疑标记。
        if (request.payload.find("<script>") != string::npos)
        {
            cout << "[Sanitize] payload rejected\n";
            return false;
        }

        cout << "[Sanitize] passed\n";
        return BaseHandler::handle(request);
    }
};

class RoleCheckHandler : public BaseHandler
{
public:
    bool handle(OrderRequest &request) override
    {
        if (request.requiresAdmin && request.username != "alice")
        {
            cout << "[Role] admin privilege required\n";
            return false;
        }

        cout << "[Role] passed\n";
        return BaseHandler::handle(request);
    }
};

class CacheHandler : public BaseHandler
{
public:
    bool handle(OrderRequest &request) override
    {
        auto it = responseCache.find(request.payload);
        if (it != responseCache.end())
        {
            cout << "[Cache] hit -> " << it->second << "\n";
            return true;
        }

        cout << "[Cache] miss\n";
        bool ok = BaseHandler::handle(request);
        if (ok)
        {
            responseCache[request.payload] = "OrderResult{cached}";
        }
        return ok;
    }

private:
    unordered_map<string, string> responseCache;
};

class OrderServiceHandler : public BaseHandler
{
public:
    bool handle(OrderRequest &request) override
    {
        cout << "[OrderService] order created for user " << request.username
             << " with payload: " << request.payload << "\n";
        return true;
    }
};

void runRequest(Handler &chain, OrderRequest request, const string &title)
{
    cout << "\n=== " << title << " ===\n";
    bool ok = chain.handle(request);
    cout << (ok ? "Result: ACCEPTED" : "Result: REJECTED") << "\n";
}

int main()
{
    RateLimitHandler rateLimit;
    AuthHandler auth(rateLimit);
    SanitizationHandler sanitize;
    RoleCheckHandler role;
    CacheHandler cache;
    OrderServiceHandler service;

    // Chain order can be changed at runtime.
    // 链顺序可按需配置。
    rateLimit.setNext(&auth)->setNext(&sanitize)->setNext(&role)->setNext(&cache)->setNext(&service);

    OrderRequest good{"alice", "123456", "10.0.0.8", "create-order:book", false};
    OrderRequest badAuth{"bob", "wrong", "10.0.0.9", "create-order:pen", false};
    OrderRequest badPayload{"alice", "123456", "10.0.0.8", "create-order:<script>", false};

    runRequest(rateLimit, good, "Request #1 (valid)");
    runRequest(rateLimit, good, "Request #2 (same payload, cache expected)");
    runRequest(rateLimit, badAuth, "Request #3 (auth failed)");
    runRequest(rateLimit, badPayload, "Request #4 (sanitization failed)");

    return 0;
}
