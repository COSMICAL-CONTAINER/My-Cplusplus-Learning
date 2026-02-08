#include <iostream>
#include <set>
using namespace std;

// set is a container that stores unique elements in a specific order.
// It is implemented as a binary search tree (usually a red-black tree).
// The elements in a set are always sorted, and the set does not allow duplicate elements.
// The time complexity for insert, delete, and search operations is O(log n).
// The set container is part of the Standard Template Library (STL) in C++.
// The set container is defined in the <set> header file.
// set 是一个容器，用于存储唯一元素，并且按照特定顺序排列。
// 它通常实现为二叉搜索树（通常是红黑树）。
// set 中的元素总是排序的，并且不允许重复元素。
// 插入、删除和搜索操作的时间复杂度为 O(log n)。
// set 容器是 C++ 标准模板库（STL）的一部分。
// set 容器定义在 <set> 头文件中。

int main()
{
    cout << "=== Set Demo ===" << endl;
    cout << "=== Set 详细示例 ===" << endl;

    set<int> s1 = {3, 1, 4, 1, 5, 9, 2, 6};
    // 自动去重和排序
    // auto deduplicate and sort
    cout << "set s1: ";
    for (const auto &elem : s1)
        cout << elem << " ";
    cout << endl;

// 1. insert operation
// 1. 插入操作
    auto result = s1.insert(7);
    if (result.second)
    {
        cout << "7 inserted successfully" << endl;
    }

    result = s1.insert(3);
    if (!result.second)
    {
        cout << "3 has already existed, insertion failed" << endl;
    }

// 2. search operation
// 2. 查找操作
    auto it = s1.find(5);
    if (it != s1.end())
    {
        cout << "found 5 at position: " << distance(s1.begin(), it) << endl;
    }

// 3. range search
// 3. 范围查找
    auto lower = s1.lower_bound(3);
    auto upper = s1.upper_bound(6);
    cout << "the range from 3 to 6: ";
    for (auto it = lower; it != upper; ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

// 4. delete operation
// 4. 删除操作
    s1.erase(4);
    cout << "after deleting 4: ";
    for (const auto &elem : s1)
        cout << elem << " ";
    cout << endl;

// 5. set operations
// 5. 集合操作
    set<int> s2 = {5, 6, 7, 8, 9, 10};
    set<int> intersection, union_set, difference;

    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                     inserter(intersection, intersection.begin()));
    cout << "intersection: ";
    // cout << "交集";
    for (const auto &elem : intersection)
        cout << elem << " ";
    cout << endl;

    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
              inserter(union_set, union_set.begin()));
    cout << "union: ";
    // cout << "并集";
    for (const auto &elem : union_set)
        cout << elem << " ";
    cout << endl;
}