#include <iostream>
#include <vector>
using namespace std;

// auto and decltype can be used to deduce types in C++17
// auto 和 decltype 可以在C++17中用来推导类型

int main()
{
    cout << "=== auto and decltype Demo ===" << endl;
    cout << "=== 类型推导示例 ===" << endl;

// 1. the use of auto and decltype in C++17
// 1. auto关键字的使用
    // int
    auto i = 42;
    // double
    auto d = 3.14;
    // const char*
    auto str = "hello";
    // std::initializer_list<int>
    auto vec = {1, 2, 3, 4, 5};

    // lambda type
    auto lambda = [](int x){ return x * 2; };
    cout << "auto i = " << i << ", type: " << typeid(i).name() << endl;
    cout << "auto d = " << d << ", type: " << typeid(d).name() << endl;
    cout << "auto str = " << str << ", type: " << typeid(str).name() << endl;

// 2. decltype is used to query the type of an expression at compile time
// 2. decltype 是被用来查询一个表达式的类型
    int x = 42;
    // int
    decltype(x) y = x;
    // int& (attenion ())
    decltype((x)) z = x;
    // int
    decltype(x + y) result = 0;
    // int& (C++14)
    decltype(auto) ref = x;

    cout << "decltype(x) y = " << y << endl;
    cout << "decltype((x)) z = " << z << endl;
    cout << "decltype(x + y) result = " << result << endl;

// 3. decltype is used in templates to deduce return types
// 3. decltype在模板中使用以便推导返回类型
    vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

// 4. function return type deduction
// 4. 函数返回类型推导
    auto add = [](int a, int b) -> int
    { return a + b; };
    cout << "add(3, 4) = " << add(3, 4) << endl;

    return 0;
}