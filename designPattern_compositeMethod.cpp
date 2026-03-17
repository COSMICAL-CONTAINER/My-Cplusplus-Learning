#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

/*
Composite Pattern Notes (Overview)

Composite is a structural design pattern that lets you compose objects into tree
structures and then work with individual objects and object groups uniformly through a
common interface. It is useful when your domain naturally forms part-whole hierarchies.

Core idea:
1) Define a common Component interface for both simple and complex nodes.
2) Leaf nodes implement real atomic behavior.
3) Composite nodes store child components and delegate work recursively.

When to use Composite:
1) You need to model recursive tree-like structures.
2) Client code should treat single objects and grouped objects uniformly.
3) You want polymorphic + recursive processing over hierarchical data.

Advantages:
1) Simplifies client code via uniform interface.
2) Works naturally with recursion for deep structures.
3) Supports OCP: new leaf/composite types can be added with minimal client changes.

Trade-offs:
1) A generic component interface may become too broad for very different node types.
2) Design can become harder when you need strict constraints on valid child types.

Relationship with other patterns:
1) Often combined with Iterator to traverse trees.
2) Can combine with Visitor for operations over whole trees.
3) Can combine with Builder to construct complex trees step by step.
4) Composite and Decorator look similar structurally but have different intent.
*/

/*
组合模式笔记（概览）

组合（Composite）是一种结构型设计模式，用于把对象组织成树状结构，
并通过统一接口让客户端以相同方式处理“单个对象”和“对象组合”。
当业务模型天然存在“部分-整体”的层次关系时，这个模式非常有用。

核心思想：
1）定义统一的组件接口（Component），供叶子和容器共同实现。
2）叶子节点负责原子行为。
3）组合节点持有子组件，并递归委派工作。

适用场景：
1）需要表达递归的树状对象结构。
2）希望客户端无差别处理简单对象与复杂组合对象。
3）希望用多态 + 递归处理层级数据。

优点：
1）统一接口，简化客户端代码。
2）天然适配深层递归结构。
3）符合开闭原则（OCP），可较容易新增叶子/组合类型。

缺点：
1）当节点差异很大时，通用接口可能被迫过度抽象。
2）若要严格限制子节点类型，设计复杂度会上升。

与其他模式关系：
1）常与迭代器结合用于遍历对象树。
2）可与访问者结合，对整棵树执行操作。
3）可与生成器结合，分步骤构建复杂树结构。
4）组合与装饰在结构上相似，但设计意图不同。
*/

/*
Scenario Explanation:

This file uses an order pricing scenario with Products and Boxes.
A Product is a leaf node with a direct price. A Box is a composite node that can contain
Products and smaller Boxes. Both expose the same interface (price), so client code can
calculate totals without caring whether a node is a single item or a nested container.
The total is computed recursively over the object tree.
*/

/*
场景说明：

本文件使用“订单计价（产品 + 盒子）”场景讲解组合模式。
Product 是叶子节点，直接返回自身价格；Box 是组合节点，可包含多个 Product
以及更小的 Box。两者对外都暴露同一个接口（price），客户端无需区分当前对象
是单件商品还是嵌套容器，直接统一调用即可。总价通过递归遍历整棵对象树计算。
*/

// Component interface shared by leaf and composite.
// 叶子与组合共享的组件接口。
class OrderItem
{
public:
    virtual ~OrderItem() = default;
    virtual double price() const = 0;
    virtual string description() const = 0;
};

// Leaf: a basic product with fixed price.
// 叶子节点：基础商品，价格固定。
class Product : public OrderItem
{
public:
    Product(string itemName, double itemPrice)
        : name(std::move(itemName)), unitPrice(itemPrice) {}

    double price() const override
    {
        return unitPrice;
    }

    string description() const override
    {
        return "Product(" + name + ", $" + to_string(unitPrice) + ")";
    }

private:
    string name;
    double unitPrice = 0.0;
};

// Composite: a box that can hold products and other boxes.
// 组合节点：盒子，可包含商品和其他盒子。
class Box : public OrderItem
{
public:
    explicit Box(string boxName) : name(std::move(boxName)) {}

    void add(unique_ptr<OrderItem> child)
    {
        children.push_back(std::move(child));
    }

    double price() const override
    {
        const double childrenTotal = std::accumulate(
            children.begin(), children.end(), 0.0,
            [](double acc, const unique_ptr<OrderItem> &item)
            {
                return acc + item->price();
            });

        // Optional packaging fee for each box.
        // 每个盒子的附加包装费（示例用途）。
        return childrenTotal + packageFee;
    }

    string description() const override
    {
        return "Box(" + name + ", children=" + to_string(children.size()) + ")";
    }

private:
    string name;
    vector<unique_ptr<OrderItem>> children;
    double packageFee = 1.0;
};

void printOrderSummary(const OrderItem &item)
{
    cout << item.description() << "\n";
    cout << "Total price: $" << item.price() << "\n";
}

int main()
{
    // Build a tree-like order:
    // RootBox
    // |- Product(Laptop)
    // |- AccessoryBox
    //    |- Product(Mouse)
    //    |- Product(Keyboard)
    // |- GiftBox
    //    |- Product(StickerPack)

    auto rootBox = make_unique<Box>("RootBox");
    rootBox->add(make_unique<Product>("Laptop", 999.0));

    auto accessoryBox = make_unique<Box>("AccessoryBox");
    accessoryBox->add(make_unique<Product>("Mouse", 39.0));
    accessoryBox->add(make_unique<Product>("Keyboard", 89.0));

    auto giftBox = make_unique<Box>("GiftBox");
    giftBox->add(make_unique<Product>("StickerPack", 5.0));

    rootBox->add(std::move(accessoryBox));
    rootBox->add(std::move(giftBox));

    cout << "Composite order pricing demo:\n";
    printOrderSummary(*rootBox);

    // Client can treat leaf and composite uniformly.
    // 客户端可统一处理叶子与组合节点。
    Product singleItem("USB Cable", 12.5);
    cout << "\nSingle item pricing:\n";
    printOrderSummary(singleItem);

    return 0;
}
