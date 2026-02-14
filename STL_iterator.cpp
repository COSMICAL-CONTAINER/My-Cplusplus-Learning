#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

// iterators are objects that point to elements in a container and allow us to traverse the container and modify its elements.
// 迭代器是指向容器中元素的对象，允许我们遍历容器、修改容器的值。
// iterators provide a common interface for different types of containers, such as arrays, vectors, lists, etc.
// 迭代器分为正向迭代器、反向迭代器、常量迭代器等。
// iterators also have siutions where they can become invalid, such as when the container is modified (e.g., inserting or deleting elements) or when the container is destroyed.
// 迭代器也有失效的情况，例如当容器被修改（例如插入或删除元素）或当容器被销毁时。
// iterators are defined in the <iterator> header.
// 迭代器定义在 <iterator> 头文件中。

int main() 
{
    cout << "=== STL iterator demo ===" << endl;
    cout << "=== 迭代器使用技巧详细示例 ===" << endl;
    
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const vector<int>& const_vec = vec;  // const reference to the vector

// 1. different types of iterators
// 1. 不同类型的迭代器
    cout << "positional iterator: " << endl;
    cout << "正向迭代器: " << endl;
    // auto it = vec.begin();  // C++11 auto type deduction
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
    
    cout << "constant iterator: " << endl;
    cout << "常量迭代器: " << endl;
    // const_iterator is used to read elements without modifying them.
    // const_iterator 用于读取元素而不修改它们。

    // if we try to modify the element through const_iterator, it will cause a compile-time error.
    // 如果我们尝试通过 const_iterator 修改元素，将会导致编译时错误。
    // vector<int>::const_iterator it = const_vec.cbegin();
    // *it = 100;  // error: cannot modify through const_iterator
    // it++;       // valid: can move the iterator, but cannot modify the element
    for (vector<int>::const_iterator it = const_vec.cbegin(); it != const_vec.cend(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
    
    cout << "reverse iterator: " << endl;
    cout << "反向迭代器: " << endl;
    // reverse_iterator is used to iterate through a container in reverse order.
    // reverse_iterator 用于以相反的顺序迭代容器。
    // also can use --positional iterator to traverse the container in reverse order.
    // 也可以用--正向迭代器来反向遍历容器。
    // for (auto it = vec.end() - 1; it != vec.begin(); --it)
    // {
    //     // end() points to one past the last element, so we need to dereference it = vec.end() - 1
    //     // end() 指向最后一个元素的下一个位置，所以需要定义 it = vec.end() - 1
    //     cout << *it << " ";
    // }
    for (vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) 
    {
        cout << *it << " ";
    }
    cout << endl;

// 2. iterator operations
// 2. 迭代器操作
    auto it = vec.begin();
    // advance moves the iterator forward by a specified number of positions.
    // advance 将迭代器向前移动指定数量的位置。
    advance(it, 3);
    cout << "Element after moving forward 3 positions: " << *it << endl;
    
    // distance calculates the number of elements between two iterators.
    // distance 计算两个迭代器之间的元素数量。
    auto dist = distance(vec.begin(), it);
    cout << "Distance from the beginning: " << dist << endl;

// 3. iterator invalidation example
// 3. 迭代器失效示例
    cout << "\nIterator invalidation example:" << endl;
    cout << "迭代器失效示例:" << endl;
    vector<int> vec2 = {1, 2, 3, 4, 5};
    auto it2 = vec2.begin() + 2;
    cout << "Initial iterator points to: " << *it2 << endl;

    // Inserting an element into the vector may cause reallocation, which can invalidate all iterators pointing to the vector.
    // 向vector插入元素可能会导致重新分配内存，这可能会使所有指向vector的迭代器失效。
    vec2.insert(vec2.begin(), 0);                                    // insertion operation
    cout << "Iterator points to after insertion: " << *it2 << endl;  // may be invalidated!
    
    // Correct approach
    // 正确的做法
    vector<int> vec3 = {1, 2, 3, 4, 5};
    for (auto it3 = vec3.begin(); it3 != vec3.end(); )
    {
        if (*it3 % 2 == 0)
        {
            // erase returns the next valid iterator
            // erase 返回下一个有效的迭代器
            it3 = vec3.erase(it3);
        }
        else 
        {
            ++it3;
        }
    }
    cout << "After removing even numbers: ";
    for (const auto& elem : vec3)
    {
        cout << elem << " ";
    }
    cout << endl;

// 4. iterator adapters
// 4. 迭代器适配器
    vector<int> vec4 = {1, 2, 3, 4, 5};
    vector<int> result;
    cout << "use back_inserter: ";
    // back_inserter inserts elements at the end of the container.
    // back_inserter 在容器的末尾插入元素。
    copy(vec4.begin(), vec4.end(), back_inserter(result));
    for (const auto& elem : result)
    {
        cout << elem << " ";
    }
    cout << endl;

// 5. stream iterators
// 5. 流迭代器
    cout << "use istream_iterator and ostream_iterator: " << endl;
    vector<int> inputVec;
    copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(inputVec));
    cout << "The numbers you entered are: ";
    for (const auto& elem : inputVec)
    {
        cout << elem << " ";
    }
    cout << endl;

    cout << "use ostream_iterator to output: ";
    copy(vec4.begin(), vec4.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}