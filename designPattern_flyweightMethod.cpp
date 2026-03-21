#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Flyweight Pattern Notes (Overview)

Flyweight is a structural design pattern that reduces memory usage by sharing common,
immutable state among many similar objects. Instead of storing all fields inside every
object, you split state into:
- intrinsic state: shared, stable, reusable
- extrinsic state: context-specific, passed from outside

When to use Flyweight:
1) Your program creates huge numbers of similar objects.
2) Memory usage becomes a practical bottleneck.
3) Objects contain large repeated state that can be shared.

Advantages:
1) Saves significant memory when many objects share repeated data.
2) Keeps common data centralized and reusable.

Trade-offs:
1) Increases complexity by splitting object state.
2) May add runtime overhead because extrinsic state is passed repeatedly.
3) Requires careful design to keep flyweights immutable and safe to share.

Relationship with other patterns:
1) Flyweight can optimize Composite trees by sharing leaf data.
2) Facade represents a subsystem with one object; Flyweight represents many objects with shared state.
3) Compared with Singleton: Singleton has one instance; Flyweight has many shared instances by key.
*/

/*
享元模式笔记（概览）

享元（Flyweight）是一种结构型设计模式，核心目标是节省内存。
它通过拆分对象状态并共享“可复用的共同部分”来减少重复数据存储。
通常会把状态分为：
- 内在状态：稳定且可共享，存储在享元对象中
- 外在状态：随情景变化，由客户端或上下文在调用时传入

适用场景：
1）程序会创建大量相似对象。
2）内存占用成为瓶颈。
3）对象中存在大量可抽取、可共享的重复数据。

优点：
1）在相似对象很多时能显著节省内存。
2）公共数据集中管理，复用性高。

缺点：
1）需要拆分状态，代码复杂度会提高。
2）外在状态需要反复传参，可能有一定运行时开销。
3）需要保证享元对象不可变或至少共享安全。

与其他模式关系：
1）可用于优化组合模式中的共享叶节点。
2）外观是“一个对象代表整个子系统”，享元是“多个对象共享部分状态”。
3）与单例不同：单例通常只有一个实例，享元按键可有多个共享实例。
*/

/*
Scenario Explanation:

This file uses a forest rendering scenario.
Each Tree has unique position (x, y) as extrinsic state, while TreeType keeps shared
intrinsic state (name, color, texture). TreeFactory caches TreeType objects by key.
When planting many trees, Forest stores lightweight Tree contexts and reuses a small
set of TreeType flyweights, reducing memory footprint significantly.
*/

/*
场景说明：

本文件使用“森林渲染”场景讲解享元模式。
每棵树的坐标（x, y）属于外在状态；树种信息（名称、颜色、纹理）属于内在状态，
由 TreeType 保存。TreeFactory 按键缓存并复用 TreeType。这样在种植大量树木时，
Forest 只需保存轻量的 Tree 情景对象，并共享少量树种享元，从而显著降低内存占用。
*/

// Flyweight: shared intrinsic state.
// 享元对象：保存可共享的内在状态。
class TreeType
{
public:
    TreeType(string treeName, string treeColor, string treeTexture)
        : name(std::move(treeName)), color(std::move(treeColor)), texture(std::move(treeTexture)) {}

    void draw(int x, int y) const
    {
        cout << "Draw Tree[name=" << name
             << ", color=" << color
             << ", texture=" << texture
             << "] at (" << x << ", " << y << ")\n";
    }

    string key() const
    {
        return name + "|" + color + "|" + texture;
    }

private:
    string name;
    string color;
    string texture;
};

// Flyweight factory: cache and reuse TreeType objects.
// 享元工厂：缓存并复用 TreeType。
class TreeFactory
{
public:
    shared_ptr<TreeType> getTreeType(const string &name, const string &color, const string &texture)
    {
        const string typeKey = name + "|" + color + "|" + texture;
        auto it = cache.find(typeKey);
        if (it != cache.end())
        {
            return it->second;
        }

        auto type = make_shared<TreeType>(name, color, texture);
        cache[typeKey] = type;
        return type;
    }

    size_t flyweightCount() const
    {
        return cache.size();
    }

private:
    unordered_map<string, shared_ptr<TreeType>> cache;
};

// Context: stores extrinsic state + reference to flyweight.
// 情景对象：保存外在状态，并引用享元。
class Tree
{
public:
    Tree(int posX, int posY, shared_ptr<TreeType> sharedType)
        : x(posX), y(posY), type(std::move(sharedType)) {}

    void draw() const
    {
        type->draw(x, y);
    }

private:
    int x = 0;
    int y = 0;
    shared_ptr<TreeType> type;
};

class Forest
{
public:
    void plantTree(int x, int y, const string &name, const string &color, const string &texture)
    {
        shared_ptr<TreeType> type = factory.getTreeType(name, color, texture);
        trees.push_back(make_unique<Tree>(x, y, type));
    }

    void draw() const
    {
        for (const auto &tree : trees)
        {
            tree->draw();
        }
    }

    size_t treeCount() const
    {
        return trees.size();
    }

    size_t treeTypeCount() const
    {
        return factory.flyweightCount();
    }

private:
    vector<unique_ptr<Tree>> trees;
    TreeFactory factory;
};

int main()
{
    Forest forest;

    // Plant many trees with repeated intrinsic states.
    // 种植大量树木，重复使用树种内在状态。
    forest.plantTree(10, 20, "Oak", "Green", "RoughBark");
    forest.plantTree(15, 25, "Oak", "Green", "RoughBark");
    forest.plantTree(20, 30, "Oak", "Green", "RoughBark");

    forest.plantTree(40, 50, "Pine", "DarkGreen", "NeedleTexture");
    forest.plantTree(60, 70, "Pine", "DarkGreen", "NeedleTexture");

    forest.plantTree(80, 90, "Cherry", "Pink", "SmoothBark");

    cout << "Forest tree count: " << forest.treeCount() << "\n";
    cout << "Shared tree type (flyweight) count: " << forest.treeTypeCount() << "\n\n";

    forest.draw();

    return 0;
}
