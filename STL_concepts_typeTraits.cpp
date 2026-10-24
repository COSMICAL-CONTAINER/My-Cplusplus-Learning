#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

// concepts + type traits 学习笔记
// 目标: 约束模板输入并做编译期类型判断

#if __cplusplus >= 202002L
template <typename T>
concept Arithmetic = is_arithmetic_v<T>;

template <Arithmetic T>
T add_two(T a, T b)
{
    return a + b;
}
#endif

template <typename T>
void print_type_traits(const T &)
{
    cout << "is_integral: " << boolalpha << is_integral_v<T> << '\n';
    cout << "is_floating_point: " << boolalpha << is_floating_point_v<T> << '\n';
    cout << "is_trivially_copyable: " << boolalpha << is_trivially_copyable_v<T> << '\n';
}

int main()
{
    cout << "=== type traits demo ===\n";
    int a = 1;
    double b = 3.14;
    vector<int> c{1, 2, 3};

    cout << "int:\n";
    print_type_traits(a);
    cout << "double:\n";
    print_type_traits(b);
    cout << "vector<int>:\n";
    print_type_traits(c);

#if __cplusplus >= 202002L
    cout << "\n=== concepts demo ===\n";
    cout << "add_two(3, 5) = " << add_two(3, 5) << '\n';
    cout << "add_two(1.2, 2.3) = " << add_two(1.2, 2.3) << '\n';
#else
    cout << "\nconcepts require C++20 or later.\n";
    cout << "concepts 需要 C++20 或以上标准。\n";
#endif

    return 0;
}
