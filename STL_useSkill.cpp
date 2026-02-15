#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <numeric>
using namespace std;

// =================== Container Performance & Complexity ===================
//
// Operation/Container   vector   list     deque    set      map      unordered_set unordered_map
// ---------------------------------------------------------------------------------------------
// Random Access         O(1)     O(n)     O(1)     O(logn)  O(logn)  O(1) avg      O(1) avg
// Insert/Erase          O(n)     O(1)     O(n)     O(logn)  O(logn)  O(1) avg      O(1) avg
// Find                  O(n)     O(n)     O(n)     O(logn)  O(logn)  O(1) avg      O(1) avg
// Sort                  O(nlogn) O(nlogn) O(nlogn) Sorted   Sorted   O(nlogn)      O(nlogn)
//
// Space Complexity:
//
// Container             Extra Space  Fragmentation  Cache Friendliness
// ---------------------------------------------------------------
// vector                Low          Low            High
// list                  High         High           Low
// deque                 Medium       Medium         Medium
// set/map               Medium       Medium         Medium
// unordered_set/map     High         Low            Medium
// ===================================================================================

// =================== 容器性能与复杂度对比 ===================
//
// 操作/容器     vector   list     deque    set      map      unordered_set unordered_map
// --------------------------------------------------------------------------------------
// 随机访问      O(1)     O(n)     O(1)     O(logn)  O(logn)  O(1)均摊      O(1)均摊
// 插入/删除     O(n)     O(1)     O(n)     O(logn)  O(logn)  O(1)均摊      O(1)均摊
// 查找          O(n)     O(n)     O(n)     O(logn)  O(logn)  O(1)均摊      O(1)均摊
// 排序          O(nlogn) O(nlogn) O(nlogn) 已排序   已排序    O(nlogn)      O(nlogn)
//
// 空间复杂度对比：
//
// 容器              额外空间 内存碎片 缓存友好性
// ---------------------------------------------
// vector             低       低       高
// list               高       高       低
// deque              中等     中等     中等
// set/map            中等     中等     中等
// unordered_set/map  高       低       中等
// =============================================================


// STL use skill
// STL 使用技巧

void print_container_selection_guide()
{
    cout << "== 容器选择指南 ==\n";

    // 选择vector的情况：
    // - 需要随机访问
    // - 元素数量相对固定
    // - 主要在末尾插入/删除
    // - 对内存效率要求高
    cout << "vector: 随机访问快；末尾push/pop快；中间插删代价高；内存连续\n";

    // 选择list的情况：
    // - 需要频繁在中间插入/删除
    // - 不需要随机访问
    // - 对内存效率要求不高
    cout << "list: 任意位置插删快(已定位迭代器)；不支持随机访问；节点额外开销\n";

    // 选择deque的情况：
    // - 需要在两端插入/删除
    // - 需要随机访问
    // - 元素数量变化较大
    cout << "deque: 两端push/pop快；支持随机访问；分段连续存储\n";

    // 选择set/map的情况：
    // - 需要自动排序
    // - 需要快速查找
    // - 元素唯一性要求
    cout << "set/map: 有序；查找/插入/删除 O(logN)；基于红黑树\n";

    // 选择unordered_set/map的情况：
    // - 不需要排序
    // - 需要最快的查找速度
    // - 对内存效率要求不高
    cout << "unordered_set/map: 无序；平均 O(1) 查找；哈希桶占用更多内存\n";
}

int main()
{
    print_container_selection_guide();

    // Performance optimization 1: reserve space in advance
    // 性能优化1：预分配空间 reserve
    vector<int> vec;
    vec.reserve(1000); // reserve space for 1000 elements
    // 预分配1000个元素的空间（减少扩容次数）
    cout << "capacity(after reserve 1000)=" << vec.capacity() << "\n";

    // Performance optimization 2: use emplace_back
    // 性能优化2：使用 emplace_back
    vector<string> vec2;
    vec2.emplace_back("hello"); // construct in place
    // 直接构造
    cout << "vec2[0]=" << vec2[0] << "\n";

    // Performance optimization 3: use move semantics
    // 性能优化3：使用移动语义 move
    vector<string> vec3;
    string str = "hello";
    vec3.push_back(move(str)); // move instead of copy
    // 移动而不是拷贝（str 的内容资源转移给 vec3）
    cout << "vec3[0]=" << vec3[0] << "\n";
    cout << "str(after move)=" << str << "\n"; // moved-from object still usable but content is unspecified
    // moved-from 对象仍可用，但内容不保证

    // Performance optimization 4: avoid frequent insert/erase in the middle of vector
    // 性能优化4：避免在 vector 中频繁中间插删
    {
        // Bad practice: erase in the middle of vector causes all subsequent elements to move
        // 不好的做法：vector 中间删除会导致后续元素整体移动
        vector<int> vec4 = {1, 2, 3, 4, 5};
        vec4.erase(vec4.begin() + 2); // erase the 3rd element
        // 删除第3个元素，后面的元素都要移动
        cout << "vector after erase: ";
        for (int x : vec4)
        {
            cout << x << ' ';
        }
        cout << "\n";
    }

    {
        // Good practice: list erase only removes one node, no element movement
        // 好的做法：list 已定位迭代器时删除只移除一个节点
        list<int> lst = {1, 2, 3, 4, 5};
        auto it = lst.begin();
        advance(it, 2);
        lst.erase(it); // erase the 3rd element
        // 只删除一个节点
        cout << "list after erase: ";
        for (int x : lst)
        {
            cout << x << ' ';
        }
        cout << "\n";
    }

    // Performance optimization 5: use STL algorithms instead of manual loops
    // 性能优化5：使用算法而不是手动循环
    vector<int> vec5 = {1, 2, 3, 4, 5};
    int sum_manual = 0;
    // Bad practice: manual sum loop
    // 不好的做法：手动循环求和
    for (int x : vec5)
    {
        sum_manual += x;
    }
    int sum_algo = accumulate(vec5.begin(), vec5.end(), 0);
    // Good practice: use accumulate algorithm
    // 好的做法：使用 accumulate 算法
    cout << "sum_manual=" << sum_manual << "\n";
    cout << "sum_algo=" << sum_algo << "\n";

    return 0;
}