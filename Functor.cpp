#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
using namespace std;

// Functor is a function object that can be called like a function, but is implemented by overloading the function call operator (operator()).
// 仿函数(Functor)是一种行为类似函数的对象，可以像函数一样被调用，但它是通过重载函数调用运算符（operator()）来实现的。
// STL Functor are defined in the <functional> header.
// STL 仿函数定义在 <functional> 头文件中。

// standard library functor
// 标准库仿函数
// mathematical functors:
// 算术仿函数:
//     plus<T>, minus<T>, multiplies<T>, divides<T>, modulus<T>, negate<T>
// relational functors:
// 关系仿函数:
//     equal_to<T>, not_equal_to<T>, greater<T>, less<T>, greater_equal<T>, less_equal<T>
// logical functors:
// 逻辑仿函数:
//     logical_and<T>, logical_or<T>, logical_not<T>

// one-argument functor
// 一元仿函数
class GreaterThan
{
private:
    int threshold;

public:
    GreaterThan(int t) : threshold(t) {}

    bool operator()(int x) const
    {
        return x > threshold;
    }
};

// two-argument functor
// 二元仿函数
class Add
{
public:
    int operator()(int a, int b) const
    {
        return a + b;
    }
};

int main()
{
    cout << "=== STL functor demo ===" << endl;
    cout << "=== 标准库仿函数详细示例 ===" << endl;

// 1. mathematical functors
// 1. 算术仿函数
    cout << "\n[1] mathematical functors:" << endl;
    cout << "[1] 算术仿函数:" << endl;
    cout << "plus(3, 7) = " << plus<int>()(3, 7) << endl;
    cout << "minus(10, 4) = " << minus<int>()(10, 4) << endl;
    cout << "multiplies(6, 5) = " << multiplies<int>()(6, 5) << endl;
    cout << "divides(20, 4) = " << divides<int>()(20, 4) << endl;
    cout << "modulus(22, 6) = " << modulus<int>()(22, 6) << endl;
    cout << "negate(9) = " << negate<int>()(9) << endl;

// 2. relational functors
// 2. 关系仿函数
    cout << "\n[2] relational functors:" << endl;
    cout << "[2] 关系仿函数:" << endl;
    cout << "equal_to(5, 5) = " << equal_to<int>()(5, 5) << endl;
    cout << "not_equal_to(5, 3) = " << not_equal_to<int>()(5, 3) << endl;
    cout << "greater(9, 2) = " << greater<int>()(9, 2) << endl;
    cout << "less(3, 8) = " << less<int>()(3, 8) << endl;
    cout << "greater_equal(5, 5) = " << greater_equal<int>()(5, 5) << endl;
    cout << "less_equal(4, 6) = " << less_equal<int>()(4, 6) << endl;

    vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    sort(vec.begin(), vec.end(), greater<int>());
    cout << "sort desc with greater<int>: ";
    for (const auto& elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl;

// 3. logical functors
// 3. 逻辑仿函数
    cout << "\n[3] logical functors:" << endl;
    cout << "[3] 逻辑仿函数:" << endl;
    bool a = true;
    bool b = false;
    cout << "logical_and(true, false) = " << logical_and<bool>()(a, b) << endl;
    cout << "logical_or(true, false) = " << logical_or<bool>()(a, b) << endl;
    cout << "logical_not(true) = " << logical_not<bool>()(a) << endl;

// 4. custom functors with algorithms
// 4. 自定义仿函数与算法配合使用
    cout << "\n[4] custom functors:" << endl;
    cout << "[4] 自定义仿函数:" << endl;
    GreaterThan gt(4);
    auto it = find_if(vec.begin(), vec.end(), gt);
    if (it != vec.end())
    {
        cout << "first element > 4: " << *it << endl;
    }
    Add add;
    cout << "Add(5, 3) = " << add(5, 3) << endl;

    // accumulate can use functors as binary operation
    // accumulate 可使用仿函数作为二元操作
    int sum = accumulate(vec.begin(), vec.end(), 0, plus<int>());
    cout << "sum by plus<int> = " << sum << endl;

    return 0;
}
