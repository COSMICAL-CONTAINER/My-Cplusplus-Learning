#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/*
Memento Pattern Notes (Overview)

Memento is a behavioral design pattern that lets you capture and restore an object's
previous state without exposing its internal implementation details.

Core idea:
1) Originator owns and changes internal state.
2) Memento stores a snapshot of originator state.
3) Caretaker keeps memento history and requests restore when needed.

When to use Memento:
1) You need undo/rollback for stateful objects.
2) You need checkpoint/restore in long workflows.
3) You want to avoid breaking encapsulation via external state copying.

Advantages:
1) Preserves encapsulation while supporting snapshots.
2) Simplifies rollback logic via history stack management.
3) Works naturally with command history in editors.

Trade-offs:
1) Frequent snapshots may consume significant memory.
2) Caretaker must manage lifecycle of many snapshots.
3) In dynamic languages, snapshot immutability can be harder to enforce.

Relationship with other patterns:
1) Commonly combined with Command to implement undo/redo.
2) Can combine with Iterator to save/restore traversal state.
3) Prototype may serve as a simpler alternative when state is easy to clone.
*/

/*
备忘录模式笔记（概览）

备忘录（Memento）是一种行为型设计模式，
它允许你在不暴露对象内部实现细节的前提下，保存并恢复对象历史状态。

核心思想：
1）原发器（Originator）负责维护与变更自身状态。
2）备忘录（Memento）负责保存状态快照。
3）负责人（Caretaker）负责保存历史并在需要时触发恢复。

适用场景：
1）需要撤销/回滚能力（如编辑器操作）。
2）需要长流程中的检查点恢复。
3）希望避免通过外部拷贝状态而破坏封装。

优点：
1）在支持快照的同时保持封装性。
2）通过历史栈管理，回滚逻辑清晰。
3）与编辑器命令历史天然契合。

缺点：
1）高频快照会带来较大内存开销。
2）负责人需要妥善管理大量快照生命周期。
3）在动态语言中，快照不可变性不易严格保证。

与其他模式关系：
1）常与命令模式结合实现 undo/redo。
2）可与迭代器结合保存/恢复遍历状态。
3）当状态简单时，原型模式可作为轻量替代。
*/

/*
Scenario Explanation:

This file uses a text editor undo scenario.
TextEditor is the Originator. Before mutating operations, it creates snapshots
(Mementos). History is the Caretaker that stores snapshots and triggers rollback.
Caretaker can read only metadata (time/name), while editor restores full state from
memento internals.
*/

/*
场景说明：

本文件使用“文本编辑器撤销”场景。
TextEditor 是原发器，在执行修改操作前创建快照（Memento）。
History 作为负责人保存快照并在撤销时回滚。
负责人只读取快照元数据（时间/名称），编辑器才拥有恢复完整状态的权限。
*/

class Memento
{
public:
    virtual ~Memento() = default;
    virtual string getName() const = 0;
    virtual string getDateTime() const = 0;
};

class TextEditor;

class EditorMemento : public Memento
{
public:
    EditorMemento(string textValue, int cursorXValue, int cursorYValue, int selectionWidthValue)
        : text(std::move(textValue)),
          cursorX(cursorXValue),
          cursorY(cursorYValue),
          selectionWidth(selectionWidthValue)
    {
        timestamp = currentTimeString();
    }

    string getName() const override
    {
        string shortText = text.substr(0, text.size() > 12 ? 12 : text.size());
        if (text.size() > 12)
        {
            shortText += "...";
        }
        return timestamp + " / Text='" + shortText + "'";
    }

    string getDateTime() const override
    {
        return timestamp;
    }

private:
    static string currentTimeString()
    {
        std::time_t now = std::time(nullptr);
        string raw = std::ctime(&now);
        if (!raw.empty() && raw.back() == '\n')
        {
            raw.pop_back();
        }
        return raw;
    }

private:
    string text;
    int cursorX = 0;
    int cursorY = 0;
    int selectionWidth = 0;
    string timestamp;

    friend class TextEditor;
};

// Originator
// 原发器
class TextEditor
{
public:
    void setText(const string &value)
    {
        text = value;
    }

    void setCursor(int x, int y)
    {
        cursorX = x;
        cursorY = y;
    }

    void setSelectionWidth(int width)
    {
        selectionWidth = width;
    }

    const string &getText() const
    {
        return text;
    }

    unique_ptr<Memento> createSnapshot() const
    {
        return make_unique<EditorMemento>(text, cursorX, cursorY, selectionWidth);
    }

    void restore(const Memento &snapshot)
    {
        const auto *realSnapshot = dynamic_cast<const EditorMemento *>(&snapshot);
        if (!realSnapshot)
        {
            return;
        }

        text = realSnapshot->text;
        cursorX = realSnapshot->cursorX;
        cursorY = realSnapshot->cursorY;
        selectionWidth = realSnapshot->selectionWidth;

        cout << "[Editor] restored -> text='" << text
             << "', cursor=(" << cursorX << "," << cursorY
             << "), selectionWidth=" << selectionWidth << "\n";
    }

    void printState(const string &prefix) const
    {
        cout << prefix
             << " text='" << text
             << "', cursor=(" << cursorX << "," << cursorY
             << "), selectionWidth=" << selectionWidth << "\n";
    }

private:
    string text;
    int cursorX = 0;
    int cursorY = 0;
    int selectionWidth = 0;
};

// Caretaker
// 负责人
class History
{
public:
    explicit History(TextEditor &originatorRef) : originator(originatorRef) {}

    void backup()
    {
        cout << "[History] backup snapshot\n";
        snapshots.push_back(originator.createSnapshot());
    }

    void undo()
    {
        if (snapshots.empty())
        {
            cout << "[History] nothing to undo\n";
            return;
        }

        unique_ptr<Memento> last = std::move(snapshots.back());
        snapshots.pop_back();

        cout << "[History] restore: " << last->getName() << "\n";
        originator.restore(*last);
    }

    void showHistory() const
    {
        cout << "[History] snapshots:\n";
        for (const auto &snapshot : snapshots)
        {
            cout << "  - " << snapshot->getName() << "\n";
        }
    }

private:
    TextEditor &originator;
    vector<unique_ptr<Memento>> snapshots;
};

int main()
{
    TextEditor editor;
    History history(editor);

    editor.setText("Hello");
    editor.setCursor(5, 0);
    editor.setSelectionWidth(0);
    editor.printState("Initial:");

    history.backup();
    editor.setText("Hello, Design Patterns");
    editor.setCursor(22, 0);
    editor.setSelectionWidth(7);
    editor.printState("After edit #1:");

    history.backup();
    editor.setText("Hello, Design Patterns! Memento in action.");
    editor.setCursor(40, 0);
    editor.setSelectionWidth(10);
    editor.printState("After edit #2:");

    cout << "\n";
    history.showHistory();

    cout << "\nUndo once:\n";
    history.undo();

    cout << "\nUndo twice:\n";
    history.undo();

    cout << "\nUndo third time (empty):\n";
    history.undo();

    return 0;
}
