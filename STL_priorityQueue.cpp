#include <iostream>
#include <queue>
#include "person.hpp"
using namespace std;

// priority_queue is a container that follows the First In First Out (FIFO) principle.
// The first element added to the priority_queue will be the first one to be removed.
// The priority_queue is a container adapter, which means that it is implemented on top of another container, usually vector or deque.
// The priority_queue class template is defined in the <queue> header file.
// priority_queue 是一个遵循先进先出（FIFO）原则的容器。
// priority_queue 是一个容器适配器，它是在另一个容器（通常是 vector 或 deque）之上实现的。
// priority_queue 类模板定义在 <queue> 头文件中。

int main()
{
    cout << "=== Priority Queue Demo ===" << endl;
    cout << "=== Priority Queue 详细示例 ===" << endl;

// 1. Basic priority queue
// 1. 基本优先级队列
    priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(2);

// 2. size and elements
// 2. 大小和元素
    cout << "priority_queue size: " << pq.size() << endl;

    cout << "priority_queue elements: ";
    // pq.empty() checks if the priority_queue is empty.
    // pq.empty() 检查优先级队列是否为空。
    while (!pq.empty()) 
    {
        // pq.top() returns a reference to the top element, which is the largest element in the priority_queue by default.
        // pq.top() 返回对优先级队列中最大元素的引用（默认情况下）。
        cout << pq.top() << " ";
        // pq.pop() removes the top element from the priority_queue.
        // pq.pop() 从优先级队列中移除顶部元素。
        pq.pop();
    }
    cout << endl;
    
// 3. Custom priority queue
// 3. 自定义优先级队列
    priority_queue<Person, vector<Person>, greater<Person>> personPq;
    personPq.push(Person("Alice", 25));
    personPq.push(Person("Bob", 30));
    personPq.push(Person("Charlie", 20));
    
    cout << "priority queue sorted by age in ascending order: ";
    cout << "priority_queue 通过年龄升序排序结果: ";
    while (!personPq.empty()) 
    {
        cout << personPq.top() << " ";
        personPq.pop();
    }
    cout << endl;
}