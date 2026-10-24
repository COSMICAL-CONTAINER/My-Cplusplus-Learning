#include <iostream>
#include <vector>
#include <algorithm>
#if __cplusplus >= 202002L
#include <ranges>
#endif

using namespace std;

// STL ranges 学习笔记
// 目标: 体会 filter/transform 链式写法 + 惰性求值特性

int main()
{
    vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

#if __cplusplus >= 202002L
    cout << "=== ranges pipeline ===\n";
    auto pipeline = nums
                    | views::filter([](int x)
                                    { return x % 2 == 0; })
                    | views::transform([](int x)
                                       { return x * x; });

    cout << "even squares:";
    for (int x : pipeline)
        cout << ' ' << x;
    cout << '\n';

    cout << "\n=== classic algorithm comparison ===\n";
    vector<int> temp;
    copy_if(nums.begin(), nums.end(), back_inserter(temp), [](int x)
            { return x % 2 == 0; });
    transform(temp.begin(), temp.end(), temp.begin(), [](int x)
              { return x * x; });

    cout << "even squares(classic):";
    for (int x : temp)
        cout << ' ' << x;
    cout << '\n';
#else
    cout << "STL ranges requires C++20 or later.\n";
    cout << "STL ranges 需要 C++20 及以上标准。\n";
#endif

    return 0;
}
