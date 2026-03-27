#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Mediator Pattern Notes (Overview)

Mediator is a behavioral design pattern that reduces chaotic dependencies among objects by
centralizing their communication in a mediator object. Instead of components calling each
other directly, components notify the mediator, and the mediator coordinates the workflow.

Core idea:
1) Components keep a reference to mediator interface only.
2) Components emit events to mediator.
3) Mediator decides how other components should react.

When to use Mediator:
1) Components are tightly coupled through many direct dependencies.
2) You want reusable UI/domain components independent of each other.
3) You want to change collaboration rules without editing component classes.

Advantages:
1) Centralizes interaction logic (SRP-friendly collaboration layer).
2) Reduces direct coupling among components.
3) Supports OCP by introducing new mediator implementations.
4) Improves component reusability across contexts.

Trade-offs:
1) Mediator may grow into a god object if overloaded.
2) Too much centralization can hide flow and increase debugging effort.

Relationship with other patterns:
1) Facade simplifies subsystem usage; Mediator coordinates peer collaboration.
2) Can be implemented with Observer-style event subscription.
3) Compared with CoR: CoR forwards request along chain, Mediator routes based on global context.
*/

/*
中介者模式笔记（概览）

中介者（Mediator）是一种行为型设计模式，
它通过“集中化通信”来减少对象之间混乱的相互依赖。
组件不再彼此直接调用，而是把事件通知给中介者，由中介者统一协调后续动作。

核心思想：
1）组件只依赖中介者接口。
2）组件发生事件时通知中介者。
3）中介者决定其他组件如何响应。

适用场景：
1）组件之间直接依赖过多，耦合严重。
2）希望组件可在不同场景中复用。
3）希望不改组件代码也能调整协作规则。

优点：
1）交互逻辑集中管理，职责更清晰。
2）降低组件间直接耦合。
3）可通过新增中介者扩展协作规则，符合 OCP。
4）组件更容易复用到其他场景。

缺点：
1）中介者若持续膨胀，可能演化为上帝对象。
2）过度中心化可能让流程追踪更复杂。

与其他模式关系：
1）外观是简化子系统入口；中介者是协调同级对象协作。
2）中介者可结合观察者实现事件分发。
3）与责任链对比：责任链是顺序传递；中介者是基于全局上下文路由。
*/

/*
Scenario Explanation:

This file uses an authentication dialog scenario (login/register).
UI components (checkbox, textboxes, buttons) do not communicate with each other directly.
They notify AuthenticationDialogMediator, which applies interaction rules:
- switch between login/register mode
- validate fields
- perform login/register flow
This keeps components generic and reusable while collaboration logic stays centralized.
*/

/*
场景说明：

本文件使用“认证对话框（登录/注册）”场景。
UI 组件（复选框、文本框、按钮）彼此不直接通信，
而是把事件通知给 AuthenticationDialogMediator。
中介者集中处理协作规则：
- 切换登录/注册模式
- 校验输入字段
- 执行登录或注册流程
这样组件保持通用与可复用，复杂交互逻辑集中在中介者中。
*/

class Component;

class Mediator
{
public:
    virtual ~Mediator() = default;
    virtual void notify(Component *sender, const string &event) = 0;
};

class Component
{
public:
    virtual ~Component() = default;

    void setMediator(Mediator *m)
    {
        mediator = m;
    }

protected:
    Mediator *mediator = nullptr;
};

class TextBox : public Component
{
public:
    explicit TextBox(string labelText) : label(std::move(labelText)) {}

    void setValue(const string &v)
    {
        value = v;
        if (mediator)
        {
            mediator->notify(this, "change");
        }
    }

    const string &getValue() const
    {
        return value;
    }

    void setVisible(bool visible)
    {
        isVisible = visible;
    }

    bool visible() const
    {
        return isVisible;
    }

    const string &name() const
    {
        return label;
    }

private:
    string label;
    string value;
    bool isVisible = true;
};

class CheckBox : public Component
{
public:
    explicit CheckBox(string labelText) : label(std::move(labelText)) {}

    void setChecked(bool checked)
    {
        isChecked = checked;
        if (mediator)
        {
            mediator->notify(this, "check");
        }
    }

    bool checked() const
    {
        return isChecked;
    }

private:
    string label;
    bool isChecked = true; // true = login mode, false = register mode
};

class Button : public Component
{
public:
    explicit Button(string labelText) : label(std::move(labelText)) {}

    void click()
    {
        if (mediator)
        {
            mediator->notify(this, "click");
        }
    }

private:
    string label;
};

class AuthenticationDialogMediator : public Mediator
{
public:
    AuthenticationDialogMediator(
        CheckBox &modeSwitch,
        TextBox &loginUser,
        TextBox &loginPwd,
        TextBox &registerUser,
        TextBox &registerPwd,
        TextBox &registerEmail,
        Button &ok,
        Button &cancel)
        : modeCheck(modeSwitch),
          loginUsername(loginUser),
          loginPassword(loginPwd),
          registrationUsername(registerUser),
          registrationPassword(registerPwd),
          registrationEmail(registerEmail),
          okButton(ok),
          cancelButton(cancel)
    {
        bindAll();
        applyMode();
    }

    void notify(Component *sender, const string &event) override
    {
        if (sender == &modeCheck && event == "check")
        {
            applyMode();
            return;
        }

        if (sender == &okButton && event == "click")
        {
            onSubmit();
            return;
        }

        if (sender == &cancelButton && event == "click")
        {
            cout << "[Dialog] cancel pressed\n";
            return;
        }
    }

private:
    void bindAll()
    {
        modeCheck.setMediator(this);
        loginUsername.setMediator(this);
        loginPassword.setMediator(this);
        registrationUsername.setMediator(this);
        registrationPassword.setMediator(this);
        registrationEmail.setMediator(this);
        okButton.setMediator(this);
        cancelButton.setMediator(this);
    }

    void applyMode()
    {
        const bool loginMode = modeCheck.checked();
        cout << "[Dialog] mode = " << (loginMode ? "Login" : "Register") << "\n";

        loginUsername.setVisible(loginMode);
        loginPassword.setVisible(loginMode);

        registrationUsername.setVisible(!loginMode);
        registrationPassword.setVisible(!loginMode);
        registrationEmail.setVisible(!loginMode);
    }

    void onSubmit()
    {
        if (modeCheck.checked())
        {
            // Login mode
            if (loginUsername.getValue().empty() || loginPassword.getValue().empty())
            {
                cout << "[Dialog] login failed: missing username/password\n";
                return;
            }

            if (loginUsername.getValue() == "alice" && loginPassword.getValue() == "123456")
            {
                cout << "[Dialog] login success for user 'alice'\n";
            }
            else
            {
                cout << "[Dialog] login failed: invalid credentials\n";
            }
        }
        else
        {
            // Register mode
            if (registrationUsername.getValue().empty() ||
                registrationPassword.getValue().empty() ||
                registrationEmail.getValue().empty())
            {
                cout << "[Dialog] register failed: missing required fields\n";
                return;
            }

            cout << "[Dialog] register success for user '" << registrationUsername.getValue()
                 << "', email='" << registrationEmail.getValue() << "'\n";
        }
    }

private:
    CheckBox &modeCheck;
    TextBox &loginUsername;
    TextBox &loginPassword;
    TextBox &registrationUsername;
    TextBox &registrationPassword;
    TextBox &registrationEmail;
    Button &okButton;
    Button &cancelButton;
};

int main()
{
    CheckBox loginOrRegister("Login/Register");

    TextBox loginUser("LoginUsername");
    TextBox loginPwd("LoginPassword");

    TextBox registerUser("RegisterUsername");
    TextBox registerPwd("RegisterPassword");
    TextBox registerEmail("RegisterEmail");

    Button ok("OK");
    Button cancel("Cancel");

    AuthenticationDialogMediator dialog(
        loginOrRegister,
        loginUser,
        loginPwd,
        registerUser,
        registerPwd,
        registerEmail,
        ok,
        cancel);

    cout << "\n--- Login flow ---\n";
    loginOrRegister.setChecked(true);
    loginUser.setValue("alice");
    loginPwd.setValue("123456");
    ok.click();

    cout << "\n--- Register flow ---\n";
    loginOrRegister.setChecked(false);
    registerUser.setValue("bob");
    registerPwd.setValue("abc123");
    registerEmail.setValue("bob@example.com");
    ok.click();

    cout << "\n--- Invalid login flow ---\n";
    loginOrRegister.setChecked(true);
    loginUser.setValue("eve");
    loginPwd.setValue("wrong");
    ok.click();

    return 0;
}
