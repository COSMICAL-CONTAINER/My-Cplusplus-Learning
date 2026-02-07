#include <iostream>
#include <map>
#include "Person.hpp"
using namespace std;

// map is a container that stores key-value pairs, where each key is unique and maps to a value.
// It is implemented as a balanced binary search tree (usually a red-black tree), which allows for efficient insertion, deletion, and lookup operations.
// The keys in a map are ordered according to a comparison function (by default, std::less),
// which means that the elements in the map are stored in sorted order based on their keys.
// map 是一个存储键值对的容器,其中每个键都是唯一的,并映射到一个值。
// 它被实现为一个平衡的二叉搜索树（通常是红黑树）,这允许进行高效的插入、删除和查找操作。
// map 中的键根据比较函数（默认为 std::less）进行排序,
// 这意味着根据键的排序顺序,map 中的元素被存储。

int main()
{
    cout << "=== Map Demo ===" << endl;
    cout << "=== Map 详细示例 ===" << endl;

// 1. create and initialize
// 1. 创建和初始化
    map<string, int> m1 = {{"apple", 1}, {"banana", 2}, {"orange", 3}};
    map<string, int> m2;

// 2. insert operation
// 2. 插入操作
    m2["apple"] = 1;
    m2.insert({"banana", 2});
    m2.insert(make_pair("orange", 3));
    m2.emplace("grape", 4);

// 3. access and search
// 3. 访问和查找
    cout << "apple's value: " << m2["apple"] << endl;
    cout << "banana's value: " << m2.at("banana") << endl;

    auto it = m2.find("orange");
    if (it != m2.end())
    {
        cout << "found orange: " << it->second << endl;
    }

// 4. bound operation
// 4. 边界查找
    auto lower = m2.lower_bound("b");
    auto upper = m2.upper_bound("o");
    cout << "range from b to o: ";
    for (auto it = lower; it != upper; ++it)
    {
        cout << it->first << ":" << it->second << " ";
    }
    cout << endl;

// 5. forward traversal
// 5. 遍历
    cout << "map: ";
    for (const auto &pair : m2)
    {
        cout << pair.first << ":" << pair.second << " ";
    }
    cout << endl;

// 6. delete operation
// 6. 删除操作
    m2.erase("banana");
    cout << "after delete banana: ";
    for (const auto &pair : m2)
    {
        cout << pair.first << ":" << pair.second << " ";
    }
    cout << endl;

// 7. clear the map
// 7. 自定义比较函数
    map<Person, int, greater<Person>> personMap;
    personMap[Person("Alice", 25)] = 1;
    personMap[Person("Bob", 30)] = 2;
    personMap[Person("Charlie", 20)] = 3;

    cout << "sort by age: ";
    for (const auto &pair : personMap)
    {
        cout << pair.first << ":" << pair.second << " ";
    }
    cout << endl;

    return 0;
}