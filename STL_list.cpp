#include <iostream>
#include <list>
using namespace std;

// C++ list is a standard template library (STL) container that implements a doubly linked list.
// It supports efficient insertion and deletion operations (O(1) time complexity) but does not support random access.
// C++ list 是标准模板库(STL)中的双向链表容器, ?支持高效插入和删除操作(时间复杂度 O(1)),
// 但不支持随机访问, 适用于频繁修改数据的场景。?

int main()
{
    cout << "=== List Demo ===" << endl;
    cout << "=== List 详细示例 ===" << endl;
    
    list<int> lst = {1, 2, 3, 4, 5};
    
// 1. base operation
// 1. 基本操作
    cout << "list size: " << lst.size() << endl;
    cout << "is list empty: " << (lst.empty() ? "yes" : "no") << endl;

// 2. insertion operation
// 2. 插入操作
    lst.push_front(0);
    lst.push_back(6);
    
    auto it = lst.begin();
    // advance 3 position
    // 前进3个位置
    advance(it, 3);
    lst.insert(it, 100);

    // insert 3 elements at the current position
    // 在当前位置插入3个999
    lst.insert(it, 3, 999);
    lst.insert(it, 1);
    lst.insert(it, 2);

    cout << "after insertion: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;

// 3. delete operation
// 3. 删除操作
    lst.pop_front();
    lst.pop_back();
    // romove all elements equal to 999
    // 删除所有999
    lst.remove(999);
    
    cout << "after deleted: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;

// 4. sort and reverse
// 4. 排序和反转
    lst.sort();
    cout << "after sort: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;
    
    lst.reverse();
    cout << "after reverse: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;

// 5. unique
// 5. 去重
    // Note: unique() only removes consecutive duplicates and requires the list to be sorted first.
    // 注意: unique() 只能删除连续的重复元素且需先排序
    lst.unique();
    cout << "after unique: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;

// 6. merge
// 6. 合并
    list<int> lst2 = {10, 20, 30};
    // lst2 must be sorted
    // lst2 必须已排序
    lst.merge(lst2);
    cout << "after merge: ";
    for (const auto& elem : lst)
        cout << elem << " ";
    cout << endl;

    return 0;
}