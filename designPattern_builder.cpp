#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Builder Pattern Notes (Overview)

Builder is a creational design pattern that lets you construct complex objects step by step.
Instead of putting many optional parameters into a huge constructor (telescoping constructor),
you move construction logic into a builder object. The same construction process can produce
multiple representations of a product. For example, the same step sequence can build a real
Car object or a Manual that describes that car.

When to use Builder:
1) Object construction is complex and has many optional parts.
2) You want to avoid telescoping constructors.
3) You need different product forms built by similar steps.
4) You want reusable construction recipes (often via Director).

Advantages:
1) Build objects step by step and control construction order.
2) Reuse the same construction flow with different builders.
3) Keeps product business logic separate from assembly logic (SRP).
4) Helps prevent clients from seeing partially built products.

Trade-offs:
1) Introduces extra classes and higher structural complexity.
2) Might be overkill for simple objects.

Relationship with other patterns:
1) Often considered when Factory Method is no longer enough for object complexity.
2) Abstract Factory creates related families quickly; Builder focuses on staged assembly.
3) Useful when creating recursive object structures (for example, complex trees).
4) Can be combined with Bridge: Director handles abstraction, builders handle implementation.
*/

/*
生成器模式笔记（概览）

生成器（Builder）是一种创建型设计模式，用于“分步骤”构造复杂对象。
它可以避免把大量可选参数塞进一个超级构造函数（重叠构造函数），
而是将构造过程抽取到独立的生成器对象中。相同的构造步骤序列可以产出
不同形式的结果，例如：一套步骤既可以组装出汽车，也可以产出该汽车的使用手册。

适用场景：
1）对象构造过程复杂，且有大量可选配置。
2）希望避免重叠构造函数带来的可读性问题。
3）需要通过相似步骤构造不同形式的产品。
4）希望复用固定构造流程（通常交给主管 Director）。

优点：
1）支持分步构建，并灵活控制步骤顺序。
2）同一套流程可复用于不同具体生成器。
3）符合单一职责原则（SRP），构造逻辑与业务逻辑分离。
4）可减少客户端接触“半成品对象”的风险。

缺点：
1）会新增多个类，系统结构复杂度上升。
2）对于简单对象，可能显得过度设计。

与其他模式关系：
1）当工厂方法不足以处理复杂构造时，常会考虑生成器。
2）抽象工厂强调“快速产出一族相关对象”，生成器强调“分步骤组装过程”。
3）构建复杂树状结构时，生成器很有价值。
4）可与桥接结合：Director 负责抽象流程，具体 Builder 负责实现细节。
*/

/*
Scenario Explanation:

This file explains Builder through a car production scenario.
The same build steps (car type, seats, engine, trip computer, GPS) are defined in a
common Builder interface. CarBuilder produces a real Car, while CarManualBuilder produces
a Manual. Director encapsulates reusable build recipes (for example, sports car), and
client code can also bypass Director for fully custom step control.
*/

/*
场景说明：

本文件用“造车”场景讲解生成器模式。
同一组构造步骤（车型、座位、引擎、行车电脑、GPS）定义在 Builder 接口中。
CarBuilder 产出真实汽车，CarManualBuilder 产出对应手册。Director 封装可复用
的构造流程（如运动型汽车），客户端也可以跳过 Director 直接自定义步骤顺序。
*/

// Product: the real object to build.
// 产品：要被组装的真实对象（汽车）。
class Car
{
public:
    string carType;
    int seats = 0;
    string engine;
    bool hasTripComputer = false;
    bool hasGPS = false;

    string summary() const
    {
        return "Car[Type=" + carType +
               ", Seats=" + to_string(seats) +
               ", Engine=" + engine +
               ", TripComputer=" + string(hasTripComputer ? "Yes" : "No") +
               ", GPS=" + string(hasGPS ? "Yes" : "No") + "]";
    }
};

// Product: documentation generated from the same build steps.
// 产品：由同一套步骤生成的文档对象（手册）。
class Manual
{
public:
    string carType;
    string seatsDoc;
    string engineDoc;
    string tripComputerDoc;
    string gpsDoc;

    string summary() const
    {
        return "Manual[Type=" + carType +
               ", " + seatsDoc +
               ", " + engineDoc +
               ", " + tripComputerDoc +
               ", " + gpsDoc + "]";
    }
};

// Builder interface: common construction steps.
// 生成器接口：定义通用构造步骤。
class Builder
{
public:
    virtual ~Builder() = default;
    virtual void reset() = 0;
    virtual void setCarType(const string &type) = 0;
    virtual void setSeats(int count) = 0;
    virtual void setEngine(const string &engineType) = 0;
    virtual void setTripComputer(bool enabled) = 0;
    virtual void setGPS(bool enabled) = 0;
};

// Concrete builder: builds a Car.
// 具体生成器：构建汽车对象。
class CarBuilder : public Builder
{
public:
    CarBuilder()
    {
        reset();
    }

    void reset() override
    {
        currentCar = make_unique<Car>();
    }

    void setCarType(const string &type) override
    {
        currentCar->carType = type;
    }

    void setSeats(int count) override
    {
        currentCar->seats = count;
    }

    void setEngine(const string &engineType) override
    {
        currentCar->engine = engineType;
    }

    void setTripComputer(bool enabled) override
    {
        currentCar->hasTripComputer = enabled;
    }

    void setGPS(bool enabled) override
    {
        currentCar->hasGPS = enabled;
    }

    unique_ptr<Car> getProduct()
    {
        unique_ptr<Car> result = std::move(currentCar);
        reset();
        return result;
    }

private:
    unique_ptr<Car> currentCar;
};

// Concrete builder: builds a Manual.
// 具体生成器：构建使用手册对象。
class CarManualBuilder : public Builder
{
public:
    CarManualBuilder()
    {
        reset();
    }

    void reset() override
    {
        currentManual = make_unique<Manual>();
    }

    void setCarType(const string &type) override
    {
        currentManual->carType = type;
    }

    void setSeats(int count) override
    {
        currentManual->seatsDoc = "Seats: " + to_string(count);
    }

    void setEngine(const string &engineType) override
    {
        currentManual->engineDoc = "Engine: " + engineType;
    }

    void setTripComputer(bool enabled) override
    {
        currentManual->tripComputerDoc = string("TripComputer: ") + (enabled ? "Enabled" : "Disabled");
    }

    void setGPS(bool enabled) override
    {
        currentManual->gpsDoc = string("GPS: ") + (enabled ? "Enabled" : "Disabled");
    }

    unique_ptr<Manual> getProduct()
    {
        unique_ptr<Manual> result = std::move(currentManual);
        reset();
        return result;
    }

private:
    unique_ptr<Manual> currentManual;
};

// Director: reusable construction recipes.
// 主管：封装可复用的构造流程（步骤顺序）。
class Director
{
public:
    void constructSportsCar(Builder &builder) const
    {
        builder.reset();
        builder.setCarType("SportsCar");
        builder.setSeats(2);
        builder.setEngine("V8 Twin Turbo");
        builder.setTripComputer(true);
        builder.setGPS(true);
    }

    void constructCitySUV(Builder &builder) const
    {
        builder.reset();
        builder.setCarType("CitySUV");
        builder.setSeats(5);
        builder.setEngine("2.0T Hybrid");
        builder.setTripComputer(true);
        builder.setGPS(true);
    }
};

int main()
{
    Director director;

    // Build a sports car.
    // 构建运动型汽车。
    CarBuilder carBuilder;
    director.constructSportsCar(carBuilder);
    unique_ptr<Car> sportsCar = carBuilder.getProduct();
    cout << "Built car: " << sportsCar->summary() << "\n";

    // Build a manual for the same sports car recipe.
    // 用同一套步骤构建对应手册。
    CarManualBuilder manualBuilder;
    director.constructSportsCar(manualBuilder);
    unique_ptr<Manual> sportsCarManual = manualBuilder.getProduct();
    cout << "Built manual: " << sportsCarManual->summary() << "\n";

    // Client can also control builder directly without Director.
    // 客户端也可以不经过主管，直接控制构造步骤。
    carBuilder.reset();
    carBuilder.setCarType("CustomRoadster");
    carBuilder.setSeats(2);
    carBuilder.setEngine("Electric Dual Motor");
    carBuilder.setTripComputer(true);
    carBuilder.setGPS(false);
    unique_ptr<Car> customCar = carBuilder.getProduct();
    cout << "Built custom car: " << customCar->summary() << "\n";

    return 0;
}
