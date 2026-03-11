#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Abstract Factory Pattern Notes (Overview)

Abstract Factory is a creational design pattern that provides an interface for creating
families of related products without specifying their concrete classes. The key constraint
is compatibility inside one product variant: products created by the same concrete factory
must match in style or platform (for example, Modern furniture set or Victorian furniture
set). Client code depends only on abstract product and factory interfaces, so switching
from one variant to another usually requires changing only initialization code.

When to use Abstract Factory:
1) The system must work with multiple families of related objects.
2) You need to guarantee consistent combinations of products.
3) You want to avoid direct dependency on concrete classes.
4) You expect new product variants to be added over time.

Advantages:
1) Ensures compatibility among products produced by the same factory.
2) Decouples client code from concrete products and concrete factories.
3) Supports SRP by centralizing product creation logic.
4) Supports OCP when introducing a new variant (add a factory + products).

Trade-offs:
1) Adds many interfaces/classes and can increase structural complexity.
2) Adding a new product type (for every variant) is costly because all factories change.

Relationship with other patterns:
1) Often evolves from Factory Method in early design.
2) Usually implemented as a set of Factory Methods.
3) Compared with Builder: Builder focuses on step-by-step construction of one complex
   object; Abstract Factory focuses on creating a related product family.
4) Can be combined with Bridge to hide variant-specific implementation mapping.
*/

/*
抽象工厂模式笔记（概览）

抽象工厂（Abstract Factory）是一种创建型设计模式，用于在不指定具体类的前提下，
创建“一整套相互关联”的对象。它的核心约束是“同一变体内的产品必须相互匹配”：
同一个具体工厂创建出来的对象应该风格一致、平台一致（比如现代风格家具一整套，
或维多利亚风格家具一整套）。客户端只依赖抽象工厂和抽象产品接口，因此通常只需
修改初始化阶段的工厂选择逻辑，就可以切换整套产品变体。

适用场景：
1）系统需要处理多个“相关产品族”。
2）需要强约束：同一批产品必须风格一致、可协作。
3）希望客户端不直接依赖具体类。
4）未来会持续新增“产品变体”。

优点：
1）保证同一工厂生成的产品彼此匹配。
2）降低客户端与具体产品/工厂的耦合。
3）符合单一职责原则（SRP），创建逻辑集中管理。
4）符合开闭原则（OCP），新增变体通常只需扩展类而非改客户端。

缺点：
1）接口与类数量会上升，结构复杂度更高。
2）新增“产品种类”成本较高，因为所有具体工厂都要补齐新方法。

与其他模式的关系：
1）很多项目先用工厂方法，后续再演进到抽象工厂。
2）抽象工厂通常由一组工厂方法构成。
3）与生成器对比：生成器关注“分步骤构造一个复杂对象”，抽象工厂关注“生成一族相关对象”。
4）可与桥接模式组合，用于封装“抽象与实现的特定映射关系”。
*/

/*
Scenario Explanation:

This file uses a furniture store scenario to explain Abstract Factory.
The abstract factory (FurnitureFactory) creates a whole product family: Chair, Sofa,
and CoffeeTable. Concrete factories (ModernFurnitureFactory and VictorianFurnitureFactory)
guarantee style consistency across products. Client code only depends on factory/product
interfaces, so switching style variants happens mainly in initialization code.
*/

/*
场景说明：

本文件用“家具商店”场景讲解抽象工厂模式。
抽象工厂（FurnitureFactory）用于创建一整套产品族：Chair、Sofa、CoffeeTable。
具体工厂（ModernFurnitureFactory、VictorianFurnitureFactory）保证同一批次产品
风格一致。客户端只依赖工厂/产品抽象接口，因此切换风格变体主要发生在初始化阶段。
*/

// Abstract product: Chair in a furniture family.
// 抽象产品：家具族中的椅子接口。
class Chair
{
public:
    virtual ~Chair() = default;
    virtual string sitOn() const = 0;
    virtual string styleName() const = 0;
};

// Abstract product: Sofa in a furniture family.
// 抽象产品：家具族中的沙发接口。
class Sofa
{
public:
    virtual ~Sofa() = default;
    virtual string lieOn() const = 0;
    virtual string styleName() const = 0;
};

// Abstract product: Coffee table in a furniture family.
// 抽象产品：家具族中的咖啡桌接口。
class CoffeeTable
{
public:
    virtual ~CoffeeTable() = default;
    virtual string placeItems() const = 0;
    virtual string styleName() const = 0;
};

// Concrete product variants: Modern style.
// 具体产品变体：现代风格。
class ModernChair : public Chair
{
public:
    string sitOn() const override
    {
        return "Sit on a modern chair.";
    }

    string styleName() const override
    {
        return "Modern";
    }
};

class ModernSofa : public Sofa
{
public:
    string lieOn() const override
    {
        return "Lie on a modern sofa.";
    }

    string styleName() const override
    {
        return "Modern";
    }
};

class ModernCoffeeTable : public CoffeeTable
{
public:
    string placeItems() const override
    {
        return "Place coffee on a modern coffee table.";
    }

    string styleName() const override
    {
        return "Modern";
    }
};

// Concrete product variants: Victorian style.
// 具体产品变体：维多利亚风格。
class VictorianChair : public Chair
{
public:
    string sitOn() const override
    {
        return "Sit on a victorian chair.";
    }

    string styleName() const override
    {
        return "Victorian";
    }
};

class VictorianSofa : public Sofa
{
public:
    string lieOn() const override
    {
        return "Lie on a victorian sofa.";
    }

    string styleName() const override
    {
        return "Victorian";
    }
};

class VictorianCoffeeTable : public CoffeeTable
{
public:
    string placeItems() const override
    {
        return "Place coffee on a victorian coffee table.";
    }

    string styleName() const override
    {
        return "Victorian";
    }
};

// Abstract factory: create one full furniture family.
// 抽象工厂：用于创建同一风格的一整套家具。
class FurnitureFactory
{
public:
    virtual ~FurnitureFactory() = default;
    virtual unique_ptr<Chair> createChair() const = 0;
    virtual unique_ptr<Sofa> createSofa() const = 0;
    virtual unique_ptr<CoffeeTable> createCoffeeTable() const = 0;
};

// Concrete factory: Modern family.
// 具体工厂：现代风格产品族。
class ModernFurnitureFactory : public FurnitureFactory
{
public:
    unique_ptr<Chair> createChair() const override
    {
        return make_unique<ModernChair>();
    }

    unique_ptr<Sofa> createSofa() const override
    {
        return make_unique<ModernSofa>();
    }

    unique_ptr<CoffeeTable> createCoffeeTable() const override
    {
        return make_unique<ModernCoffeeTable>();
    }
};

// Concrete factory: Victorian family.
// 具体工厂：维多利亚风格产品族。
class VictorianFurnitureFactory : public FurnitureFactory
{
public:
    unique_ptr<Chair> createChair() const override
    {
        return make_unique<VictorianChair>();
    }

    unique_ptr<Sofa> createSofa() const override
    {
        return make_unique<VictorianSofa>();
    }

    unique_ptr<CoffeeTable> createCoffeeTable() const override
    {
        return make_unique<VictorianCoffeeTable>();
    }
};

// Client code only sees abstract factory and abstract products.
// 客户端只依赖抽象工厂和抽象产品接口。
void showFurnitureSet(const FurnitureFactory &factory)
{
    unique_ptr<Chair> chair = factory.createChair();
    unique_ptr<Sofa> sofa = factory.createSofa();
    unique_ptr<CoffeeTable> table = factory.createCoffeeTable();

    cout << "Chair: " << chair->sitOn() << "\n";
    cout << "Sofa: " << sofa->lieOn() << "\n";
    cout << "Table: " << table->placeItems() << "\n";

    const bool sameStyle = (chair->styleName() == sofa->styleName()) &&
                           (sofa->styleName() == table->styleName());
    cout << "Style compatibility check: "
         << (sameStyle ? "PASS" : "FAIL")
         << " (" << chair->styleName() << ")\n";
}

int main()
{
    cout << "Client: Build Modern furniture family.\n";
    ModernFurnitureFactory modernFactory;
    showFurnitureSet(modernFactory);

    cout << "\nClient: Build Victorian furniture family.\n";
    VictorianFurnitureFactory victorianFactory;
    showFurnitureSet(victorianFactory);

    return 0;
}