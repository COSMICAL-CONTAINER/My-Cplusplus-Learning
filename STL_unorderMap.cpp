#include <iostream>
#include <unordered_map>
using namespace std;

// unordered_map is a container that stores key-value pairs, where each key is unique and maps to a value.
// It is implemented as a hash table, which allows for average-case constant time complexity for insertions, deletions, and lookups.
// However, the worst-case time complexity can be linear in the number of elements if many elements hash to the same bucket.
// The unordered_map container is part of the C++ Standard Library and is defined in the <unordered_map> header file.
// unordered_map 是 map 容器的哈希表实现, 存储键值对, 其中每个键都是唯一的.
// 它是 C++ 标准库的一部分, 定义在 <unordered_map> 头文件中。
// unordered_map 是一个无序的关联容器, 基于哈希表实现, 提供平均常数时间复杂度的插入、删除和查找操作,
// 但在最坏情况下可能退化为线性时间复杂度。

int main() 
{
    cout << "=== UnorderedMap Demo ===" << endl;
    cout << "=== UnorderedMap 详细示例 ===" << endl;
    
    unordered_map<string, int> um = {{"apple", 1}, {"banana", 2}, {"orange", 3}};

// 1. base operation
// 1. 基本操作
    // 判断是否为空    // check if empty
    // 元素数量        // number of elements
    // 桶数量          // number of buckets
    // 负载因子        // load factor
    cout << "empty: " << um.empty() << endl;
    cout << "size: " << um.size() << endl;
    cout << "bucket count: " << um.bucket_count() << endl;
    cout << "load factor: " << um.load_factor() << endl;
    cout << "max load factor: " << um.max_load_factor() << endl;
    
// 2. insert and access
// 2. 插入和访问
    um["grape"] = 4;
    cout << "value of grape: " << um["grape"] << endl;
    
// 3. iteration
// 3. 遍历
    cout << "contents of unordered_map: ";
    for (const auto& pair : um)
    {
        cout << pair.first << ":" << pair.second << " ";
    }
    cout << endl;
    
// 4. bucket operations
// 4. 桶操作
    for (size_t i = 0; i < um.bucket_count(); ++i)
    {
        if (um.bucket_size(i) > 0)
        {
            cout << "bucket " << i << " contains: ";
            for (auto it = um.begin(i); it != um.end(i); ++it)
            {
                cout << it->first << " ";
            }
            cout << endl;
        }
    }
    
// 5. rehash
// 5. 重新哈希
    um.rehash(20);
    cout << "bucket count after rehash: " << um.bucket_count() << endl;

    return 0;
}