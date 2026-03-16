#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Adapter Pattern Notes (Overview)

Adapter is a structural design pattern that allows objects with incompatible interfaces
to collaborate. Instead of changing existing client code or third-party services, you add
an adapter layer that translates calls, data formats, or parameter conventions between
both sides.

When to use Adapter:
1) You want to reuse an existing class, but its interface does not match your client code.
2) You need to integrate legacy/third-party modules that cannot be modified directly.
3) You need a migration layer while replacing one service implementation with another.

Advantages:
1) Separates conversion logic from core business logic (SRP).
2) Client code can remain unchanged when adding new adapters (OCP).
3) Improves integration flexibility across incompatible components.

Trade-offs:
1) Adds extra types and indirection, increasing structural complexity.
2) For small systems, direct service modification might be simpler.

Relationship with other patterns:
1) Adapter changes interface; Decorator keeps interface and adds behavior.
2) Adapter usually wraps one object; Facade typically wraps a subsystem.
3) Adapter gives a different interface; Proxy keeps the same interface.
4) Bridge is planned upfront for variation; Adapter is often applied later for compatibility.
*/

/*
适配器模式笔记（概览）

适配器（Adapter）是一种结构型设计模式，用于让“接口不兼容”的对象能够协作。
核心做法不是改客户端或第三方代码，而是新增一层适配器，负责在两边之间做调用
转换、数据转换或参数映射。

适用场景：
1）想复用某个已有类，但它的接口与现有代码不匹配。
2）需要接入不能直接修改的遗留系统或第三方库。
3）在新旧实现切换期间，需要一层兼容过渡。

优点：
1）把转换逻辑与业务逻辑分离，符合单一职责原则（SRP）。
2）可通过新增适配器扩展兼容对象，减少改动客户端（OCP）。
3）提升跨模块整合能力。

缺点：
1）会增加类型数量与调用层次，结构复杂度上升。
2）在小型场景下，直接改服务类可能更简单。

与其他模式关系：
1）适配器会改变接口；装饰器不改接口，只增强功能。
2）适配器通常封装单个对象；外观通常面向整个子系统。
3）适配器提供“不同接口”；代理通常保持“相同接口”。
4）桥接多在设计前期规划；适配器多在后期做兼容改造。
*/

/*
Scenario Explanation:

This file demonstrates Adapter with two concrete scenarios.
The object adapter scenario (RoundHole + SquarePegAdapter) shows interface conversion by
wrapping an incompatible object. The class adapter scenario (XML provider + JSON analyzer)
shows adaptation via multiple inheritance in C++. In both cases, client-side logic keeps
its expected interface while adapter code handles conversion details.
*/

/*
场景说明：

本文件通过两个具体场景讲解适配器模式。
对象适配器场景（RoundHole + SquarePegAdapter）展示了通过“封装不兼容对象”来
完成接口转换；类适配器场景（XML 提供方 + JSON 分析器）展示了 C++ 多重继承下
的适配方式。两种场景都体现了：客户端保持原有接口不变，转换细节由适配器承担。
*/

// ============================================================
// Object Adapter demo: RoundHole + RoundPeg + SquarePegAdapter
// 对象适配器示例：圆孔 + 圆钉 + 方钉适配器
// ============================================================

class RoundPeg
{
public:
    explicit RoundPeg(double radiusValue) : radius(radiusValue) {}

    virtual ~RoundPeg() = default;

    virtual double getRadius() const
    {
        return radius;
    }

private:
    double radius = 0.0;
};

class RoundHole
{
public:
    explicit RoundHole(double radiusValue) : radius(radiusValue) {}

    double getRadius() const
    {
        return radius;
    }

    bool fits(const RoundPeg &peg) const
    {
        return peg.getRadius() <= radius;
    }

private:
    double radius = 0.0;
};

// Incompatible service.
// 不兼容的服务类（方钉接口与圆钉不兼容）。
class SquarePeg
{
public:
    explicit SquarePeg(double widthValue) : width(widthValue) {}

    double getWidth() const
    {
        return width;
    }

private:
    double width = 0.0;
};

// Object adapter: wraps SquarePeg and exposes RoundPeg interface.
// 对象适配器：封装 SquarePeg，对外暴露 RoundPeg 接口。
class SquarePegAdapter : public RoundPeg
{
public:
    explicit SquarePegAdapter(const SquarePeg &peg) : RoundPeg(0.0), squarePeg(peg) {}

    double getRadius() const override
    {
        // Minimum enclosing circle radius for a square: width * sqrt(2) / 2
        // 方形最小外接圆半径：width * sqrt(2) / 2
        return squarePeg.getWidth() * std::sqrt(2.0) / 2.0;
    }

private:
    const SquarePeg &squarePeg;
};

// ============================================================
// Class Adapter demo: multiple inheritance in C++
// 类适配器示例：C++ 多重继承
// ============================================================

class StockXmlProvider
{
public:
    string fetchXml() const
    {
        return "<stock><symbol>MSFT</symbol><price>421.35</price></stock>";
    }
};

class JsonAnalyzer
{
public:
    string analyzeJson(const string &jsonText) const
    {
        return "Analyzer accepted JSON payload: " + jsonText;
    }
};

// Class adapter uses multiple inheritance to adapt XML provider to analyzer flow.
// 类适配器通过多重继承，把 XML 提供方适配到 JSON 分析流程。
class XmlToJsonClassAdapter : public StockXmlProvider, public JsonAnalyzer
{
public:
    string analyzeFromXml() const
    {
        string xml = fetchXml();
        string json = convertXmlToJson(xml);
        return analyzeJson(json);
    }

private:
    string convertXmlToJson(const string &xml) const
    {
        // Demo-only conversion for learning purpose.
        // 教学演示：这里是简化转换，不是完整 XML/JSON 解析。
        (void)xml;
        return "{\"symbol\":\"MSFT\",\"price\":421.35}";
    }
};

int main()
{
    cout << "[Object Adapter] Round hole and square peg\n";
    RoundHole hole(5.0);
    RoundPeg roundPeg(5.0);
    SquarePeg smallSquarePeg(5.0);
    SquarePeg largeSquarePeg(10.0);

    cout << "Round peg fits hole: " << (hole.fits(roundPeg) ? "true" : "false") << "\n";

    SquarePegAdapter smallAdapter(smallSquarePeg);
    SquarePegAdapter largeAdapter(largeSquarePeg);

    cout << "Small square peg fits via adapter: " << (hole.fits(smallAdapter) ? "true" : "false") << "\n";
    cout << "Large square peg fits via adapter: " << (hole.fits(largeAdapter) ? "true" : "false") << "\n";

    cout << "\n[Class Adapter] XML provider to JSON analyzer\n";
    XmlToJsonClassAdapter classAdapter;
    cout << classAdapter.analyzeFromXml() << "\n";

    return 0;
}
