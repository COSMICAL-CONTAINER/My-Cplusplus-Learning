#include <iostream>
#include <memory>
#include <stack>
#include <string>

using namespace std;

/*
Command Pattern Notes (Overview)

Command is a behavioral design pattern that turns a request into an object.
A command object encapsulates all information needed to perform an action:
receiver reference, operation intent, and request data. This enables request
parameterization, delayed execution, queueing, logging, and undo/redo support.

Core idea:
1) Define a command interface with execute() (and optionally undo()).
2) Concrete commands store receiver + parameters.
3) Invoker triggers commands via the interface (no direct business call).
4) Receiver performs real business logic.

When to use Command:
1) You want to decouple UI/event triggers from business operations.
2) You need runtime-configurable actions for buttons/menu/shortcuts.
3) You need operation history, undo/redo, delayed execution, or queueing.

Advantages:
1) Decouples invoker and receiver (SRP-friendly structure).
2) Supports OCP by adding new commands without changing invoker.
3) Natural support for undo/history and macro commands.
4) Works well with logging and operation replay.

Trade-offs:
1) Adds extra command classes and object wiring complexity.
2) For very simple operations, this may feel over-engineered.

Relationship with other patterns:
1) Can combine with Memento for robust undo snapshots.
2) Can combine with CoR where handlers execute command objects.
3) Strategy chooses an algorithm; Command represents an executable request.
*/

/*
命令模式笔记（概览）

命令（Command）是一种行为型设计模式，它把“请求”封装为对象。
命令对象会携带执行该请求所需的全部信息：接收者、动作意图、请求参数。
这使得你可以参数化调用方、延迟执行、入队、记录日志，并实现撤销/重做。

核心思想：
1）定义命令接口 execute()（可选 undo()）。
2）具体命令持有接收者和参数。
3）调用者只通过命令接口触发行为，不直接调用业务对象。
4）接收者负责真正业务实现。

适用场景：
1）希望解耦 UI 触发逻辑与业务逻辑。
2）需要让按钮/菜单/快捷键在运行时绑定不同动作。
3）需要历史记录、撤销重做、延迟执行或任务队列。

优点：
1）调用方与接收者解耦，结构更清晰。
2）符合开闭原则（OCP），新增命令不改调用方。
3）天然支持撤销、历史记录、宏命令。
4）便于日志记录和操作重放。

缺点：
1）会引入更多命令类和装配代码。
2）对于极简单操作可能显得过度设计。

与其他模式关系：
1）可与备忘录模式配合实现更稳健的撤销。
2）可与责任链结合，把请求封装为命令对象沿链传递。
3）策略是“选择算法”，命令是“封装可执行请求”。
*/

/*
Scenario Explanation:

This file uses a text editor toolbar scenario.
Buttons and shortcuts are invokers; editor is the receiver. Commands such as
CopyCommand, CutCommand, and PasteCommand encapsulate editor operations and carry
execution context. Stateful commands save backup before mutation, and command history
stores executed commands to support undo.
*/

/*
场景说明：

本文件使用“文本编辑器工具栏”场景。
按钮和快捷键属于调用者（Invoker），编辑器是接收者（Receiver）。
CopyCommand、CutCommand、PasteCommand 等命令封装具体编辑动作与上下文数据。
会修改状态的命令在执行前保存备份，执行后写入历史栈，从而支持撤销（undo）。
*/

class Editor
{
public:
    explicit Editor(string initialText) : text(std::move(initialText)) {}

    const string &getText() const
    {
        return text;
    }

    void setText(const string &value)
    {
        text = value;
    }

    string getSelection() const
    {
        return text;
    }

    void deleteSelection()
    {
        text.clear();
    }

    void replaceSelection(const string &value)
    {
        text = value;
    }

private:
    string text;
};

class Application;

// Command interface
// 命令接口
class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual bool execute() = 0;
    virtual void undo() = 0;
};

// Base command with backup support.
// 基础命令：提供状态备份与撤销。
class EditorCommand : public ICommand
{
public:
    EditorCommand(Application &applicationRef, Editor &editorRef)
        : app(applicationRef), editor(editorRef) {}

    void saveBackup();

    void undo() override
    {
        editor.setText(backup);
    }

protected:
    Application &app;
    Editor &editor;
    string backup;
};

class CommandHistory
{
public:
    void push(unique_ptr<ICommand> command)
    {
        history.push(std::move(command));
    }

    unique_ptr<ICommand> pop()
    {
        if (history.empty())
        {
            return nullptr;
        }

        unique_ptr<ICommand> top = std::move(history.top());
        history.pop();
        return top;
    }

private:
    stack<unique_ptr<ICommand>> history;
};

class Application
{
public:
    explicit Application(Editor editorRef) : activeEditor(std::move(editorRef)) {}

    Editor &editor()
    {
        return activeEditor;
    }

    const string &clipboardData() const
    {
        return clipboard;
    }

    void setClipboard(const string &value)
    {
        clipboard = value;
    }

    void executeCommand(unique_ptr<ICommand> command)
    {
        if (command->execute())
        {
            commandHistory.push(std::move(command));
        }
    }

    void undo()
    {
        unique_ptr<ICommand> command = commandHistory.pop();
        if (command)
        {
            command->undo();
            cout << "[App] undo completed\n";
        }
        else
        {
            cout << "[App] nothing to undo\n";
        }
    }

private:
    string clipboard;
    Editor activeEditor;
    CommandHistory commandHistory;
};

void EditorCommand::saveBackup()
{
    backup = editor.getText();
}

class CopyCommand : public EditorCommand
{
public:
    using EditorCommand::EditorCommand;

    bool execute() override
    {
        app.setClipboard(editor.getSelection());
        cout << "[Copy] clipboard = " << app.clipboardData() << "\n";
        return false;
    }
};

class CutCommand : public EditorCommand
{
public:
    using EditorCommand::EditorCommand;

    bool execute() override
    {
        saveBackup();
        app.setClipboard(editor.getSelection());
        editor.deleteSelection();
        cout << "[Cut] clipboard = " << app.clipboardData()
             << ", editor = '" << editor.getText() << "'\n";
        return true;
    }
};

class PasteCommand : public EditorCommand
{
public:
    using EditorCommand::EditorCommand;

    bool execute() override
    {
        saveBackup();
        editor.replaceSelection(app.clipboardData());
        cout << "[Paste] editor = '" << editor.getText() << "'\n";
        return true;
    }
};

// Invoker-like button
// 调用者（按钮）
class Button
{
public:
    explicit Button(string buttonText) : text(std::move(buttonText)) {}

    void setOnClick(unique_ptr<ICommand> command)
    {
        onClickCommand = std::move(command);
    }

    unique_ptr<ICommand> releaseCommand()
    {
        return std::move(onClickCommand);
    }

    const string &label() const
    {
        return text;
    }

private:
    string text;
    unique_ptr<ICommand> onClickCommand;
};

int main()
{
    Application app(Editor("Design Patterns"));

    cout << "Initial editor text: '" << app.editor().getText() << "'\n\n";

    // Simulate button bindings.
    // 模拟按钮绑定命令。
    Button copyButton("Copy");
    copyButton.setOnClick(make_unique<CopyCommand>(app, app.editor()));

    Button cutButton("Cut");
    cutButton.setOnClick(make_unique<CutCommand>(app, app.editor()));

    Button pasteButton("Paste");
    pasteButton.setOnClick(make_unique<PasteCommand>(app, app.editor()));

    // Click copy.
    app.executeCommand(copyButton.releaseCommand());

    // Rebind cut command each time (common in demo code).
    // 示例中每次重新绑定一次命令。
    cutButton.setOnClick(make_unique<CutCommand>(app, app.editor()));
    app.executeCommand(cutButton.releaseCommand());

    pasteButton.setOnClick(make_unique<PasteCommand>(app, app.editor()));
    app.executeCommand(pasteButton.releaseCommand());

    cout << "\nBefore undo, editor text: '" << app.editor().getText() << "'\n";
    app.undo();
    cout << "After  undo, editor text: '" << app.editor().getText() << "'\n";

    return 0;
}
