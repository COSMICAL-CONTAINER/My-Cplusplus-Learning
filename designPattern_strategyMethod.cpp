// ============================================================
// Strategy Pattern Notes
// ============================================================
// Intent:
// Strategy is a behavioral design pattern that defines a family of algorithms,
// encapsulates each one, and makes them interchangeable at runtime.
//
// Core idea:
// 1) Context keeps a reference to Strategy interface.
// 2) ConcreteStrategy classes provide different algorithm variants.
// 3) Client selects or replaces strategy based on current need.
//
// When to use:
// 1) Multiple algorithm variants for the same task.
// 2) Frequent algorithm switching at runtime.
// 3) Want to remove large if/switch blocks from context.
//
// Advantages:
// 1) Open/Closed: add new algorithm without modifying context.
// 2) Isolates algorithm details from business flow.
// 3) Replaces inheritance-heavy design with composition.
//
// Trade-offs:
// 1) Increases number of classes.
// 2) Client needs to understand strategy differences.
//
// Relation to State:
// Both use composition and delegation.
// Strategy focuses on interchangeable algorithms;
// State focuses on behavior changes driven by internal state transitions.
//
// ============================================================
// 策略模式笔记
// ============================================================
// 意图：
// 策略模式是一种行为型设计模式，用于定义一组可互换的算法，
// 并在运行时按需切换。
//
// 核心思想：
// 1）上下文持有策略接口引用。
// 2）具体策略类实现不同算法变体。
// 3）客户端根据场景选择或替换策略。
//
// 适用场景：
// 1）同一任务存在多种实现算法。
// 2）需要在运行时切换算法。
// 3）希望消除上下文中的大量 if/switch 分支。
//
// 优点：
// 1）符合开闭原则，新增算法不改上下文。
// 2）业务流程与算法细节解耦。
// 3）用组合替代继承，结构更灵活。
//
// 缺点：
// 1）会增加类的数量。
// 2）客户端需要了解各策略差异以便选择。
//
// 与状态模式关系：
// 两者都用组合与委派。
// 策略强调“可替换算法”；状态强调“状态驱动行为变化”。

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/*
Scenario Explanation:

We model a map navigation app.
RoutePlanner (Context) does not implement routing details itself.
It delegates route generation to a RouteStrategy.

Strategies in this demo:
1) DrivingRouteStrategy
2) WalkingRouteStrategy
3) PublicTransitRouteStrategy
*/

/*
场景说明：

我们模拟一个地图导航应用。
RoutePlanner（上下文）不直接实现路径算法，
而是把“生成路线”委派给 RouteStrategy。

本示例包含三种策略：
1）DrivingRouteStrategy（驾车）
2）WalkingRouteStrategy（步行）
3）PublicTransitRouteStrategy（公交）
*/

class RouteStrategy
{
public:
    virtual ~RouteStrategy() = default;
    virtual vector<string> buildRoute(const string &from, const string &to) const = 0;
    virtual const char *name() const = 0;
};

class DrivingRouteStrategy : public RouteStrategy
{
public:
    vector<string> buildRoute(const string &from, const string &to) const override
    {
        return {
            "Start at " + from,
            "Enter urban expressway",
            "Take Exit 5 toward city center",
            "Arrive at " + to};
    }

    const char *name() const override
    {
        return "Driving";
    }
};

class WalkingRouteStrategy : public RouteStrategy
{
public:
    vector<string> buildRoute(const string &from, const string &to) const override
    {
        return {
            "Start at " + from,
            "Walk through Riverside Park",
            "Use pedestrian overpass",
            "Arrive at " + to};
    }

    const char *name() const override
    {
        return "Walking";
    }
};

class PublicTransitRouteStrategy : public RouteStrategy
{
public:
    vector<string> buildRoute(const string &from, const string &to) const override
    {
        return {
            "Start at " + from,
            "Take Metro Line 2 for 4 stops",
            "Transfer to Bus 301",
            "Arrive at " + to};
    }

    const char *name() const override
    {
        return "PublicTransit";
    }
};

class RoutePlanner
{
public:
    explicit RoutePlanner(unique_ptr<RouteStrategy> strategyValue = nullptr)
        : strategy(std::move(strategyValue)) {}

    void setStrategy(unique_ptr<RouteStrategy> strategyValue)
    {
        strategy = std::move(strategyValue);
    }

    void showRoute(const string &from, const string &to) const
    {
        if (!strategy)
        {
            cout << "[RoutePlanner] strategy is not set\n";
            return;
        }

        cout << "[RoutePlanner] strategy = " << strategy->name() << "\n";
        vector<string> path = strategy->buildRoute(from, to);
        for (size_t i = 0; i < path.size(); ++i)
        {
            cout << "  " << (i + 1) << ". " << path[i] << "\n";
        }
    }

private:
    unique_ptr<RouteStrategy> strategy;
};

int main()
{
    const string from = "Railway Station";
    const string to = "City Museum";

    RoutePlanner planner(make_unique<DrivingRouteStrategy>());
    planner.showRoute(from, to);

    cout << "\n--- switch strategy to walking ---\n";
    planner.setStrategy(make_unique<WalkingRouteStrategy>());
    planner.showRoute(from, to);

    cout << "\n--- switch strategy to public transit ---\n";
    planner.setStrategy(make_unique<PublicTransitRouteStrategy>());
    planner.showRoute(from, to);

    return 0;
}