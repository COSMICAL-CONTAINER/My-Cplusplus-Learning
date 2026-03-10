#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Factory Method Pattern Notes

Factory Method is a creational design pattern that defines a method in a base creator
class for creating objects, while allowing subclasses to decide which concrete product
class to instantiate. The key idea is to replace direct constructor calls in client-facing
business logic with calls to a factory method, so object creation and object usage are
decoupled. This lets the same high-level logic work with different product types through
one shared product interface.

When to use Factory Method:
1) You cannot predict the exact concrete product types in advance.
2) You want to extend product families without modifying existing client logic.
3) You want framework/library users to customize internal components by subclassing.
4) You want to centralize creation logic (including caching/pooling/reuse) in one place.

Advantages:
1) Reduces tight coupling between creator/client code and concrete products.
2) Supports Single Responsibility Principle by isolating creation logic.
3) Supports Open/Closed Principle by adding new products via new creator subclasses.
4) Improves maintainability in scenarios where product types evolve frequently.

Trade-offs:
1) Introduces more classes/subclasses and increases structural complexity.
2) May feel over-engineered for very small systems with stable product types.
3) Requires a well-designed common product interface.

Relationship with other patterns:
1) Often appears early in design and can evolve toward Abstract Factory, Builder, or Prototype.
2) Abstract Factory is commonly implemented as a set of Factory Methods.
3) Factory Method can be a special step inside Template Method-based workflows.
4) Compared with Prototype: Factory Method relies on inheritance; Prototype relies on cloning.
*/

/*
工厂方法模式笔记

工厂方法（Factory Method）是一种创建型设计模式：它在创建者基类中定义一个创建对象的方法，
并允许子类决定实际实例化哪一种具体产品。核心思想是：在业务逻辑中不直接 `new` 具体类，
而是改为调用工厂方法，从而把“对象创建”和“对象使用”分离。这样一来，同一套上层逻辑可以
通过统一产品接口与不同具体产品协作。

适用场景：
1）无法提前确定对象的确切类型及其依赖关系。
2）希望新增产品类型时不改动现有客户端逻辑。
3）希望框架/库使用者通过继承来自定义内部组件。
4）希望把创建逻辑（含缓存、对象池、复用）集中管理。

优点：
1）降低创建者/客户端与具体产品之间的耦合。
2）符合单一职责原则（SRP），创建逻辑集中更易维护。
3）符合开闭原则（OCP），通过新增子类扩展而非修改旧代码。
4）在产品类型经常变化的系统中可维护性更好。

缺点：
1）会引入更多类和子类，增加系统结构复杂度。
2）对于产品类型稳定且规模很小的系统，可能显得过度设计。
3）要求先设计好稳定、合理的产品抽象接口。

与其他模式的关系：
1）常在设计初期使用，后续可演进为抽象工厂、生成器或原型模式。
2）抽象工厂通常由一组工厂方法构成。
3）工厂方法可作为模板方法流程中的一个步骤。
4）与原型对比：工厂方法主要依赖继承，原型模式主要依赖克隆。
*/

/*
Scenario Explanation:

This file explains Factory Method using a logistics scenario.
The creator base class (Logistics) defines createTransport(), while concrete creators
(RoadLogistics and SeaLogistics) decide which product to instantiate (TruckTransport or
ShipTransport). Client code only depends on the creator abstraction and the product
interface, so new transport types can be introduced with minimal client changes.
*/

/*
场景说明：

本文件用“物流运输”场景讲解工厂方法模式。
创建者基类（Logistics）定义 createTransport()，具体创建者（RoadLogistics、
SeaLogistics）决定实例化哪种产品（TruckTransport、ShipTransport）。
客户端只依赖创建者抽象和产品接口，因此后续新增运输类型时，基本不需要改动
客户端逻辑。
*/

// Product interface: all concrete transports share the same behavior.
// 产品接口：所有具体运输方式都遵循同一套行为。
class Transport
{
public:
    virtual ~Transport() = default;
    virtual string deliver() const = 0;
};

// Concrete product: land transport by truck.
// 具体产品：卡车（陆路运输）。
class TruckTransport : public Transport
{
public:
    string deliver() const override
    {
        return "Deliver by land in a truck.";
    }
};

// Concrete product: sea transport by ship.
// 具体产品：轮船（海路运输）。
class ShipTransport : public Transport
{
public:
    string deliver() const override
    {
        return "Deliver by sea in a ship.";
    }
};

// Creator(Logistics): declares the factory method and contains core business logic.
// 创建者（物流）：声明工厂方法,并包含依赖产品接口的核心业务逻辑。
class Logistics
{
public:
    virtual ~Logistics() = default;

    // Factory Method: subclasses return different concrete products.
    // 工厂方法：子类返回不同的具体产品。
    virtual unique_ptr<Transport> createTransport() const = 0;

    // Business logic that works with the abstract product interface.
    // 业务逻辑只依赖抽象产品接口,不依赖具体实现。
    string planDelivery() const
    {
        unique_ptr<Transport> transport = createTransport();
        return "Logistics plan: " + transport->deliver();
    }
};

// Concrete creator: creates truck transport.
// 具体创建者：创建卡车运输对象。
class RoadLogistics : public Logistics
{
public:
    unique_ptr<Transport> createTransport() const override
    {
        return make_unique<TruckTransport>();
    }
};

// Concrete creator: creates ship transport.
// 具体创建者：创建轮船运输对象。
class SeaLogistics : public Logistics
{
public:
    unique_ptr<Transport> createTransport() const override
    {
        return make_unique<ShipTransport>();
    }
};

// Client code depends on the creator abstraction.
// 客户端代码只依赖创建者抽象类型。
void runLogisticsScenario(const Logistics &logistics)
{
    cout << "Client: creator type is hidden, factory method still works.\n";
    cout << logistics.planDelivery() << "\n";
}

int main()
{
    cout << "App: Road logistics selected.\n";
    RoadLogistics roadLogistics;
    runLogisticsScenario(roadLogistics);

    cout << "\nApp: Sea logistics selected.\n";
    SeaLogistics seaLogistics;
    runLogisticsScenario(seaLogistics);

    return 0;
}