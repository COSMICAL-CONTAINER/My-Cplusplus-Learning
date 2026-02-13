#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
using namespace std;

// STL algorithms are a collection of functions that operate on ranges of elements, such as arrays or containers.
// STL算法是一组函数，作用于元素范围，如数组或容器。
// STL algorithms can be used to perform a variety of operations on data, such as sorting, searching, and modifying elements.
// STL算法可用于对数据进行各种操作，如排序、搜索和修改元素。
// STL algorithms are defined in the <algorithm> header.
// STL算法在<algorithm>头文件中定义。

int main() 
{
    cout << "=== STL algorithm demo ===" << endl;
    cout << "=== STL算法详细示例 ===" << endl;
    
    vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

// 1. Find algorithms
// 1. 查找算法
    // find finds the first occurrence of the value in the range.
    // find 在范围内查找值的第一次出现。
    auto it = find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) 
    {
        cout << "find 5 position: " << distance(vec.begin(), it) << endl;
    }
    
    // count counts the number of occurrences of the value in the range.
    // count 计算范围内值的出现次数。
    auto count5 = count(vec.begin(), vec.end(), 5);
    cout << "count of 5: " << count5 << endl;
    
    // find_if finds the first element that satisfies the given predicate.
    // find_if 查找第一个满足给定谓词的元素。
    auto it2 = find_if(vec.begin(), vec.end(), [](int x) { return x > 5; });
    if (it2 != vec.end()) 
    {
        cout << "the first element greater than 5: " << *it2 << endl;
    }

// 2. Sort algorithms
// 2. 排序算法
    sort(vec.begin(), vec.end());
    cout << "after sort: ";
    for (const auto& elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl;

// 3. Unique algorithms
// 3. 去重
    // unique removes consecutive duplicate elements, so the vector must be sorted first to remove all duplicates.
    // unique 删除连续的重复元素，因此必须在删除所有重复元素之前对vector进行排序。
    // use erase removes the elements that are not unique.
    // 用 erase 删除不是唯一的元素。
    auto last = unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
    cout << "after unique: ";
    for (const auto& elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl;
    
// 4. Binary search algorithms
// 4. 二分查找算法
    // binary_search checks if the value exists in the sorted sequence.
    // binary_search 检查值是否存在于已排序的序列中。
    // Note: The sequence must be sorted before using binary_search, otherwise the result is undefined.
    // 注意：使用binary_search之前必须对序列进行排序，否则结果未定义。
    if (binary_search(vec.begin(), vec.end(), 5)) 
    {
        cout << "5 is exist in the sorted sequence" << endl;
    }
    
    // lower_bound returns an iterator pointing to the first element that is not less than (i.e. greater or equal to) the value.
    // lower_bound 返回指向第一个不小于（即大于或等于）值的元素的迭代器。
    // upper_bound returns an iterator pointing to the first element that is greater than the value.
    // upper_bound 返回指向第一个大于值的元素的迭代器。
    auto lower = lower_bound(vec.begin(), vec.end(), 5);
    auto upper = upper_bound(vec.begin(), vec.end(), 5);
    // distance calculates the number of elements between two iterators.
    // distance 计算两个迭代器之间的元素数量。
    cout << "the range of 5 is: [" << distance(vec.begin(), lower) 
         << ", " << distance(vec.begin(), upper) << ")" << endl;

// 5. transform algorithms
// 5. 变换算法
    vector<int> vec2(vec.size());
    // transform applies a function to each element in the input range and stores the result in the output range.
    // transform 将函数应用于输入范围内的每个元素，并将结果存储在输出范围内。
    transform(vec.begin(), vec.end(), vec2.begin(), 
              [](int x) { return x * 2; });
    cout << "each element multiplied by 2: ";
    for (const auto& elem : vec2)
    {
        cout << elem << " ";
    }
    cout << endl;

// 6. condition algorithms
// 6. 条件算法
    // all_of checks if all elements in the range satisfy the given predicate.
    // all_of 检查范围内所有元素是否满足给定条件。
    auto allPositive = all_of(vec.begin(), vec.end(), 
                              [](int x) { return x > 0; });
    cout << "all elements are positive: " << (allPositive ? "yes" : "no") << endl;
    
    // any_of checks if any element in the range satisfies the given predicate.
    // any_of 检查范围内是否有任何元素满足给定条件。
    auto anyEven = any_of(vec.begin(), vec.end(), 
                          [](int x) { return x % 2 == 0; });
    cout << "any element is even: " << (anyEven ? "yes" : "no") << endl;
    
    // none_of checks if no element in the range satisfies the given predicate.
    // none_of 检查范围内是否有任何元素满足给定条件。
    auto noneNegative = none_of(vec.begin(), vec.end(), 
                                [](int x) { return x < 0; });
    cout << "no element is negative: " << (noneNegative ? "yes" : "no") << endl;
    
// 7. 移除算法
// 7. Remove algorithms
    vector<int> vec3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // remove_if removes elements that satisfy the given predicate from the range.
    // remove_if 从范围内删除满足给定条件的元素。
    auto newEnd = remove_if(vec3.begin(), vec3.end(), 
                           [](int x) { return x % 2 == 0; });
    vec3.erase(newEnd, vec3.end());
    cout << "after removing even numbers: ";
    for (const auto& elem : vec3)
    {
        cout << elem << " ";
    }
    cout << endl;

// 8. 数值算法
// 8. Numeric algorithms
    // accumulate calculates the sum of the elements in the range, starting with an initial value.
    // accumulate 计算范围内元素的和，从初始值开始。
    int sum = accumulate(vec.begin(), vec.end(), 0);
    cout << "sum: " << sum << endl;
    // multiplies is a function object that multiplies two values.
    // multiplies 是一个函数对象，用于乘以两个值。
    int product = accumulate(vec.begin(), vec.end(), 1, multiplies<int>());
    cout << "product: " << product << endl;
    
    vector<int> vec4 = {1, 2, 3, 4, 5};
    // partial_sum calculates the partial sums of the elements in the range and stores them in the output range.
    // partial_sum 计算范围内元素的部分和，并将它们存储在输出范围内。
    // Prefix Sum
    // 前缀和
    vector<int> partial_sums(vec4.size());
    partial_sum(vec4.begin(), vec4.end(), partial_sums.begin());
    cout << "partial sums: ";
    for (const auto& elem : partial_sums)
    {
        cout << elem << " ";
    }
    cout << endl;

// 9. Copy algorithms
// 9. 复制算法
    vector<int> vec5(5);
    // copy copies elements from the input range to the output range.
    // copy 将元素从输入范围复制到输出范围。
    copy(vec4.begin(), vec4.end(), vec5.begin());
    cout << "after copy: ";
    for (const auto& elem : vec5)
    {
        cout << elem << " ";
    }
    cout << endl;

// 10. fill algorithms
// 10. 填充算法
    // fill assigns the given value to all elements in the range.
    // fill 将给定值分配给范围内的所有元素。
    fill(vec5.begin(), vec5.end(), 999);
    cout << "after fill: ";
    for (const auto& elem : vec5)
    {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}