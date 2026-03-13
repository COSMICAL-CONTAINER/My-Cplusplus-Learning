#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Prototype Pattern Notes (Overview)

Prototype is a creational design pattern that lets you copy existing objects without
making client code depend on their concrete classes. Instead of constructing a new object
from the outside (which may require class knowledge and access to internal state), cloning
is delegated to the object itself through a common clone interface.

When to use Prototype:
1) You need to duplicate objects while keeping client code independent from concrete types.
2) Object initialization is expensive, and reusing preconfigured instances is beneficial.
3) The differences between object variants are mostly in state/configuration, not behavior.
4) You want a prototype registry (name -> prototype) to quickly produce configured copies.

Advantages:
1) Clone objects without tight coupling to concrete classes.
2) Reuse prebuilt prototypes and avoid repeated heavy initialization.
3) Convenient for creating complex configured objects.
4) Offers an alternative to subclass explosion for many configurations.

Trade-offs:
1) Deep copy can be hard when objects have circular references or external resources.
2) Clone logic must be carefully maintained when object structure changes.

Relationship with other patterns:
1) Prototype can be an alternative path after Factory Method in evolving designs.
2) Abstract Factory can be implemented with factory methods that clone registered prototypes.
3) Works well with Composite/Decorator when copying complex object structures.
4) Compared with Factory Method: Prototype is clone-based; Factory Method is inheritance-based creation.
*/

/*
原型模式笔记（概览）

原型（Prototype）是一种创建型设计模式：通过“克隆已有对象”来创建新对象，
从而避免客户端依赖具体类。与“从外部 new + 手动拷贝字段”相比，原型模式把
复制责任交给对象自己（通过统一的 clone 接口），客户端只面向抽象接口编程。

适用场景：
1）需要复制对象，同时希望客户端不依赖对象的具体类。
2）对象初始化成本较高，希望复用预配置对象。
3）对象差异主要体现在状态配置，而不是行为本身。
4）希望使用“原型注册表（名称 -> 原型）”快速生成对象副本。

优点：
1）克隆时无需耦合具体类。
2）可复用预生成原型，减少重复初始化开销。
3）更方便创建复杂配置对象。
4）可在很多配置场景下减少子类数量。

缺点：
1）当对象包含循环引用、外部资源时，深拷贝实现复杂。
2）对象结构变动时，克隆逻辑也要同步维护。

与其他模式关系：
1）很多系统先用工厂方法，复杂后可转向原型方案。
2）抽象工厂也可结合原型注册表来产出产品对象。
3）复制复杂组合结构（如组合/装饰）时，原型很有价值。
4）与工厂方法对比：原型以“克隆”为核心，工厂方法以“继承创建”为核心。
*/

/*
Scenario Explanation:

This file explains Prototype with a shape cloning scenario.
Client code works with the Shape interface and clones objects without depending on
concrete classes (Circle/Rectangle). A registry stores preconfigured prototypes by name
and returns cloned copies on demand. This approach is useful when initialization is
expensive or when many object variants differ mostly by configuration.
*/

/*
场景说明：

本文件用“图形克隆”场景讲解原型模式。
客户端只依赖 Shape 抽象接口，通过 clone() 复制对象，而不依赖具体类
（Circle、Rectangle）。注册表按名称保存预配置原型，并在需要时返回克隆体。
当初始化成本高或对象变体主要体现在配置差异时，这种方式非常实用。
*/

// Prototype interface: every shape supports clone.
// 原型接口：所有图形都支持克隆。
class Shape
{
public:
    virtual ~Shape() = default;
    virtual unique_ptr<Shape> clone() const = 0;
    virtual string describe() const = 0;

    void setPosition(int xPos, int yPos)
    {
        x = xPos;
        y = yPos;
    }

    void setColor(const string &newColor)
    {
        color = newColor;
    }

protected:
    int x = 0;
    int y = 0;
    string color = "white";
};

// Concrete prototype: Circle.
// 具体原型：圆形。
class Circle : public Shape
{
public:
    Circle() = default;

    Circle(int radiusValue)
    {
        radius = radiusValue;
    }

    // Copy constructor clones all state including base fields.
    // 拷贝构造：复制当前对象全部状态（含父类字段）。
    Circle(const Circle &other)
    {
        x = other.x;
        y = other.y;
        color = other.color;
        radius = other.radius;
    }

    unique_ptr<Shape> clone() const override
    {
        return make_unique<Circle>(*this);
    }

    string describe() const override
    {
        return "Circle[x=" + to_string(x) +
               ", y=" + to_string(y) +
               ", color=" + color +
               ", radius=" + to_string(radius) + "]";
    }

private:
    int radius = 0;
};

// Concrete prototype: Rectangle.
// 具体原型：矩形。
class Rectangle : public Shape
{
public:
    Rectangle() = default;

    Rectangle(int widthValue, int heightValue)
    {
        width = widthValue;
        height = heightValue;
    }

    Rectangle(const Rectangle &other)
    {
        x = other.x;
        y = other.y;
        color = other.color;
        width = other.width;
        height = other.height;
    }

    unique_ptr<Shape> clone() const override
    {
        return make_unique<Rectangle>(*this);
    }

    string describe() const override
    {
        return "Rectangle[x=" + to_string(x) +
               ", y=" + to_string(y) +
               ", color=" + color +
               ", width=" + to_string(width) +
               ", height=" + to_string(height) + "]";
    }

private:
    int width = 0;
    int height = 0;
};

// Prototype registry: stores preconfigured prototypes.
// 原型注册表：保存预配置原型，按名称克隆。
class ShapePrototypeRegistry
{
public:
    void registerPrototype(const string &name, unique_ptr<Shape> prototype)
    {
        prototypes[name] = std::move(prototype);
    }

    unique_ptr<Shape> create(const string &name) const
    {
        auto it = prototypes.find(name);
        if (it == prototypes.end())
        {
            return nullptr;
        }
        return it->second->clone();
    }

private:
    unordered_map<string, unique_ptr<Shape>> prototypes;
};

int main()
{
    ShapePrototypeRegistry registry;

    // Prepare preconfigured prototypes once.
    // 预生成原型：初始化一次，后续按需克隆。
    auto redCirclePrototype = make_unique<Circle>(20);
    redCirclePrototype->setPosition(10, 10);
    redCirclePrototype->setColor("red");
    registry.registerPrototype("redCircle", std::move(redCirclePrototype));

    auto blueRectanglePrototype = make_unique<Rectangle>(30, 15);
    blueRectanglePrototype->setPosition(5, 8);
    blueRectanglePrototype->setColor("blue");
    registry.registerPrototype("blueRectangle", std::move(blueRectanglePrototype));

    // Client clones without knowing concrete class names.
    // 客户端无需知道具体类型，只通过注册表名获取克隆体。
    vector<unique_ptr<Shape>> copiedShapes;
    copiedShapes.push_back(registry.create("redCircle"));
    copiedShapes.push_back(registry.create("redCircle"));
    copiedShapes.push_back(registry.create("blueRectangle"));

    cout << "Prototype clone results:\n";
    for (const auto &shape : copiedShapes)
    {
        if (shape)
        {
            cout << "- " << shape->describe() << "\n";
        }
    }

    return 0;
}
