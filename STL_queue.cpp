#include <iostream>
#include <queue>
using namespace std;

// queue is a container that follows the First In First Out (FIFO) principle.
// The first element added to the queue will be the first one to be removed.
// The queue is a container adapter, which means that it is implemented on top of another container, usually deque.
// The queue class template is defined in the <queue> header file.
// queue 是遵循先进先出（FIFO）原则的容器。
// 首先添加到队列中的元素将是第一个被移除的元素。
// 队列是一个容器适配器，这意味着它是在另一个容器之上实现的，通常是 deque。
// queue 类模板在 <queue> 头文件中定义。

int main()
{
    cout << "=== Queue Demo ===" << endl;
    cout << "=== Queue 详细示例 ===" << endl;

// 1. push and pop
// 1. 入队和出队
    queue<int> q;
    q.push(1);
    q.push(2);
    q.pop();
    q.push(3);

// 2. size and elements
// 2. 队列大小和元素
    cout << "queue size: " << q.size() << endl;

    cout << "queue elements: ";
    // q.empty() checks if the queue is empty.
    // q.empty() 检查队列是否为空。
    while (!q.empty()) 
    {
        // q.front() returns a reference to the front element.
        // q.front() 返回对前端元素的引用。
        cout << q.front() << " ";
        // q.pop() removes the front element.
        // q.pop() 移除前端元素。
        q.pop();
    }
    cout << endl;
}