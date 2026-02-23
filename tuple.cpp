#include <iostream>
#include <tuple>
#include <string>

using namespace std;

// tuple is a fixed-size collection of heterogeneous values. It is defined in <tuple> and provides a way to store multiple values of different types together. Tuples are often used for returning multiple values from a function, or for grouping related data together without defining a custom struct or class.
// 元组是一个固定大小的异构值集合。它在 <tuple> 中定义，提供了一种将不同类型的多个值一起存储的方法。元组常用于从函数返回多个值，或在不定义自定义结构体或类的情况下将相关数据组合在一起。

int main()
{
// 1. Create tuples
// 1. 创建元组
    auto t1 = std::make_tuple(1, "hello", 3.14);                       // deduced types
    std::tuple<int, std::string, double> t2(1, "world", 2.71);         // explicit types

// 2. Access elements by index with std::get
// 2. 使用 std::get 按索引访问元素
    int first = std::get<0>(t1);
    std::string second = std::get<1>(t1);
    double third = std::get<2>(t1);
    cout << "std::get: " << first << ", " << second << ", " << third << "\n";

// 3. Structured bindings (C++17) — convenient unpacking
// 3. 结构化绑定（C++17）——方便的解包语法
    auto [x, y, z] = t1; // x:int, y:const char*, z:double (deduced)
    cout << "structured binding: " << x << ", " << y << ", " << z << "\n";

// 4. Use std::tie to unpack into existing variables (can use std::ignore)
// 4. 使用 std::tie 将元组解包到已有变量（可用 std::ignore 忽略某些元素）
    int a;
    string b;
    double c;
    std::tie(a, b, c) = t1;
    cout << "std::tie: " << a << ", " << b << ", " << c << "\n";

    // ignore example: only extract first and third
    // ignore 示例：只提取第一个和第三个元素
    int only1; double only3;
    std::tie(only1, std::ignore, only3) = t1;
    cout << "tie with ignore: " << only1 << ", " << only3 << "\n";

// 5. Tuple utilities: size and element type
// 5. 元组工具：size 与元素类型
    cout << "tuple_size(t1) = " << std::tuple_size<decltype(t1)>::value << "\n";
    using SecondType = std::tuple_element<1, decltype(t1)>::type;
    static_assert(std::is_same<SecondType, const char*>::value || std::is_same<SecondType, std::string>::value, "second type check");

// 6. Concatenate tuples with tuple_cat
// 6. 使用 tuple_cat 拼接元组
    auto t3 = std::tuple_cat(t1, std::make_tuple(std::string("extra"), 42));
    cout << "tuple_cat size = " << std::tuple_size<decltype(t3)>::value << "\n";

// 7. Apply a callable to tuple elements using std::apply (C++17)
// 7. 使用 std::apply 对元组元素调用可调用对象（C++17）
    auto printer = [](auto&&... elems){ ((cout << elems << " "), ...); cout << '\n'; };
    std::apply(printer, t3); // prints all elements in t3

// 8. Compare tuples lexicographically using the built-in operators
// 8. 元组支持逐元素的字典序比较（内建比较操作符）
    auto a_t = std::make_tuple(1, 2);
    auto b_t = std::make_tuple(1, 3);
    cout << "a_t < b_t: " << (a_t < b_t) << "\n";

// 9. Swapping tuples
// 9. 交换元组
    auto s1 = std::make_tuple(10, "s1");
    auto s2 = std::make_tuple(20, "s2");
    cout << "before swap: "; std::apply(printer, s1); std::apply(printer, s2);
    s1.swap(s2);
    cout << "after swap: "; std::apply(printer, s1); std::apply(printer, s2);

// 10. Printing a tuple generically: using std::apply and a fold expression
// 10. 通用打印元组：使用 std::apply 与折叠表达式
    cout << "print t1: "; std::apply(printer, t1);

    return 0;
}
