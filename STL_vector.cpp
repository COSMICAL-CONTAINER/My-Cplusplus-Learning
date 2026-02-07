#include <iostream>
#include <vector>
using namespace std;

// vector is a dynamic array that can resize itself automatically when elements are added or removed.
// vector 是一个动态数组，当元素被添加或删除时，它能够自动调整大小。

int main()
{
    cout << "=== Vector Demo ===" << endl;
    cout << "=== Vector 详细示例 ===" << endl;
// 1. create and initialize
// 1. 创建和初始化
    vector<int> vec1 = {1, 2, 3, 4, 5};

    // five elements with value 10
    // 5个10
    vector<int> vec2(5, 10);

    // use vec1's elements to initialize
    // 用vec1初始化
    vector<int> vec3(vec1.begin(), vec1.end());

    // void vector;
    // 空vector
    vector<int> vec4;

// 2. capacity and size
// 2. 容量管理
    cout << "vec1.size(): " << vec1.size() << ", vec1.capacity(): " << vec1.capacity() << endl;
    // preallocate space, reduce memory allocation times
    // 预分配空间, 减少内存分配次数
    vec1.reserve(20);

    cout << "after reserve capacity: " << vec1.capacity() << endl;

// 3. element access
// 3. 元素访问
    // use index or at(), use front() and back() to get the first and last element
    // when use at(), it will do boundary check
    // 用下标或at()访问, 用front()和back()获取第一个和最后一个元素
    // 其中用at()访问会做越界检查
    cout << "the first element: "<< vec1.front() << endl;
    cout << "the last element: " << vec1.back() << endl;
    cout << "the 3rd element: " << vec1[2] << endl;
    cout << "the 3rd element(safe): " << vec1.at(2) << endl;

// 4. insert operation
// 4. 插入操作
    vec1.push_back(6);
    vec1.insert(vec1.begin() + 2, 100);
    // insert 3 elements at the beginning
    // 在开头插入3个0
    vec1.insert(vec1.begin(), 3, 0);

    cout << "after insert: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 5. delete operation
// 5. 删除操作
    vec1.pop_back();
    vec1.erase(vec1.begin() + 2);
    // delete the first 3 elements
    // 删除前3个元素
    vec1.erase(vec1.begin(), vec1.begin() + 3);

    cout << "after erase: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 6. resize operation
// 6. 调整大小
    // resize to 10 elements, new elements are filled with 999
    // 调整到10个元素，新元素用999填充
    vec1.resize(10, 999);
    cout << "after resize: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 7. clear and swap
// 7. 清空和交换
    vector<int> temp = {100, 200, 300};
    vec1.swap(temp);
    cout << "after swap: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;
}