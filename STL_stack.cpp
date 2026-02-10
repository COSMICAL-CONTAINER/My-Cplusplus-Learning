#include <iostream>
#include <stack>
using namespace std;

// stack is a container that follows the Last In First Out (LIFO) principle.
// The last element added to the stack will be the first one to be removed.
// The stack is a container adapter, which means that it is implemented on top of another container, usually vector or deque.
// The stack class template is defined in the <stack> header file.
// stack 是一个遵循后进先出（LIFO）原则的容器。
// 最后添加到堆栈的元素将是第一个被移除的元素。
// 堆栈是一个容器适配器，这意味着它是在另一个容器（通常是vector或deque）之上实现的。
// stack 类模板定义在 <stack> 头文件中。

int main()
{
    cout << "=== Stack Demo ===" << endl;
    cout << "=== Stack 详细示例 ===" << endl;

// 1. push and pop
// 1. 入栈和出栈
    stack<int> st;
    st.push(1);
    st.push(2);
    st.pop();
    st.push(3);

// 2. stack size and elements
// 2. 栈大小和元素
    cout << "stack size: " << st.size() << endl;

    cout << "stack elements: ";
    // st.empty() checks if the stack is empty.
    // st.empty() 检查堆栈是否为空。
    while (!st.empty()) 
    {
        // st.top() returns a reference to the top element.
        // st.top() 返回对顶部元素的引用。
        cout << st.top() << " ";
        // st.pop() removes the top element.
        // st.pop() 移除顶部元素。
        st.pop();
    }
    cout << endl;

    return 0;
}