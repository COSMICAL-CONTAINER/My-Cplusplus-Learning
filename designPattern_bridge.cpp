#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Bridge Pattern Notes (Overview)

Bridge is a structural design pattern that splits a large class (or a closely related set
of classes) into two independent hierarchies: Abstraction and Implementation. This avoids
combinatorial class explosion when a system evolves along multiple dimensions.

Typical problem:
If one hierarchy (for example, RemoteControl variants) must also vary by another dimension
(for example, Device platforms), pure inheritance quickly creates too many subclasses.
Bridge solves this by replacing inheritance across dimensions with composition: abstraction
holds a reference to implementation and delegates low-level work to it.

When to use Bridge:
1) A class changes for more than one independent reason.
2) You need independent extension of high-level logic and platform details.
3) You want to switch implementations at runtime.

Advantages:
1) Decouples abstraction from implementation.
2) Supports OCP on both sides (new abstractions and implementations independently).
3) Improves maintainability by separating high-level logic and low-level details.
4) Lets client depend on stable abstraction only.

Trade-offs:
1) Introduces additional indirection and classes.
2) Can feel over-engineered for very small/simple codebases.

Relationship with other patterns:
1) Bridge is usually designed upfront; Adapter is often added later for compatibility.
2) Strategy/State have similar composition shape but different intent.
3) Can combine with Abstract Factory to create valid abstraction-implementation pairs.
4) Can combine with Builder where Director controls abstraction-level workflow.
*/

/*
桥接模式笔记（概览）

桥接（Bridge）是一种结构型设计模式，它把一个大类（或一组紧密相关类）拆成两条
可独立演化的层次结构：抽象部分（Abstraction）和实现部分（Implementation）。
这样可以避免“多个维度同时用继承扩展”导致的组合类爆炸。

典型问题：
如果一个类既要扩展“控制逻辑维度”（如不同遥控器），又要扩展“平台维度”（如不同设备），
单纯靠继承会快速产生大量子类。桥接通过组合替代跨维度继承：抽象层持有实现层引用，
把底层工作委派给实现层完成。

适用场景：
1）一个类在多个独立维度上变化。
2）希望高层逻辑和平台实现能独立扩展。
3）希望在运行时切换实现对象。

优点：
1）解耦抽象与实现。
2）两侧都符合开闭原则（OCP），可独立扩展。
3）高层逻辑与底层细节分离，维护性更好。
4）客户端仅依赖稳定抽象。

缺点：
1）会增加间接层和类数量。
2）在非常小的系统中可能显得复杂。

与其他模式关系：
1）桥接多在前期设计；适配器多在后期兼容改造。
2）与策略/状态在结构上相似，但意图不同。
3）可结合抽象工厂，封装合法的“抽象-实现”组合。
4）可结合生成器，由 Director 组织抽象层流程。
*/

/*
Scenario Explanation:

This Bridge example uses "RemoteControl + Device" as the teaching scenario:
RemoteControl is the abstraction hierarchy, and Device is the implementation hierarchy.
The base remote declares a reference member that points to a device object. All remotes
interact with devices through the common Device interface, so one remote abstraction can
work with different concrete devices (for example, TV and Radio).

You can develop new remote subclasses independently from device classes.
For example, a basic remote supports power, volume, and channel controls; then you can
extend it with an advanced remote that adds features such as mute (or in real products,
extra battery modules and touch screen controls).
*/

/*
场景说明：

这里用“遥控器 + 设备”来讲桥接模式：遥控器是抽象层，设备是实现层。
遥控器基类声明了一个指向设备对象的引用成员变量。所有遥控器通过通用设备接口
与设备进行交互，使得同一个遥控器可以支持不同类型的设备（例如 TV、Radio）。

你可以开发独立于设备类的遥控器子类，而无需改动设备层代码。
例如，基础遥控器只提供开关机、音量和频道控制；在此基础上可扩展高级遥控器，
新增静音等功能（现实中也可类比额外电池、触摸屏等扩展）。
*/

// Implementation hierarchy: device platform API.
// 实现层：设备平台接口。
class Device
{
public:
    virtual ~Device() = default;

    virtual bool isEnabled() const = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual int getVolume() const = 0;
    virtual void setVolume(int value) = 0;

    virtual int getChannel() const = 0;
    virtual void setChannel(int value) = 0;

    virtual string name() const = 0;
};

class TV : public Device
{
public:
    bool isEnabled() const override { return powerOn; }

    void enable() override { powerOn = true; }

    void disable() override { powerOn = false; }

    int getVolume() const override { return volume; }

    void setVolume(int value) override
    {
        volume = std::clamp(value, 0, 100);
    }

    int getChannel() const override { return channel; }

    void setChannel(int value) override
    {
        channel = std::max(1, value);
    }

    string name() const override { return "TV"; }

private:
    bool powerOn = false;
    int volume = 30;
    int channel = 1;
};

class Radio : public Device
{
public:
    bool isEnabled() const override { return powerOn; }

    void enable() override { powerOn = true; }

    void disable() override { powerOn = false; }

    int getVolume() const override { return volume; }

    void setVolume(int value) override
    {
        volume = std::clamp(value, 0, 100);
    }

    int getChannel() const override { return station; }

    void setChannel(int value) override
    {
        station = std::max(1, value);
    }

    string name() const override { return "Radio"; }

private:
    bool powerOn = false;
    int volume = 20;
    int station = 88;
};

// Abstraction hierarchy: remote control logic.
// 抽象层：遥控器逻辑。
class RemoteControl
{
public:
    explicit RemoteControl(shared_ptr<Device> boundDevice) : device(std::move(boundDevice)) {}
    virtual ~RemoteControl() = default;

    virtual void togglePower()
    {
        if (device->isEnabled())
        {
            device->disable();
        }
        else
        {
            device->enable();
        }
    }

    virtual void volumeDown()
    {
        device->setVolume(device->getVolume() - 10);
    }

    virtual void volumeUp()
    {
        device->setVolume(device->getVolume() + 10);
    }

    virtual void channelDown()
    {
        device->setChannel(device->getChannel() - 1);
    }

    virtual void channelUp()
    {
        device->setChannel(device->getChannel() + 1);
    }

    virtual string describeState() const
    {
        return "Remote->" + device->name() +
               " [power=" + string(device->isEnabled() ? "on" : "off") +
               ", volume=" + to_string(device->getVolume()) +
               ", channel/station=" + to_string(device->getChannel()) + "]";
    }

protected:
    shared_ptr<Device> device;
};

// Refined abstraction: advanced remote behavior.
// 精确抽象：高级遥控器行为。
class AdvancedRemoteControl : public RemoteControl
{
public:
    explicit AdvancedRemoteControl(shared_ptr<Device> boundDevice)
        : RemoteControl(std::move(boundDevice)) {}

    void mute()
    {
        device->setVolume(0);
    }
};

int main()
{
    shared_ptr<Device> tv = make_shared<TV>();
    RemoteControl basicTvRemote(tv);

    basicTvRemote.togglePower();
    basicTvRemote.volumeUp();
    basicTvRemote.channelUp();
    cout << basicTvRemote.describeState() << "\n";

    shared_ptr<Device> radio = make_shared<Radio>();
    AdvancedRemoteControl advancedRadioRemote(radio);

    advancedRadioRemote.togglePower();
    advancedRadioRemote.channelUp();
    advancedRadioRemote.channelUp();
    advancedRadioRemote.mute();
    cout << advancedRadioRemote.describeState() << "\n";

    // Runtime switch of implementation with same abstraction.
    // 运行时切换实现：同一抽象可绑定不同实现。
    basicTvRemote = RemoteControl(radio);
    basicTvRemote.volumeUp();
    cout << basicTvRemote.describeState() << "\n";

    return 0;
}
