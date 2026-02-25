#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

// std::function is a type-erased wrapper for callable objects. It can hold function pointers, lambdas, bind expressions, and function objects.
// std::bind creates a new callable by binding some arguments to a callable, producing partial application. Use placeholders (_1, _2, ...) to leave slots.
// Prefer lambdas in new code for clarity; std::bind can be useful for quick partial application or adapting APIs.
// std::function 是一个类型擦除的可调用对象包装器。它可以保存函数指针、lambda、bind 表达式以及函数对象。
// std::bind 通过绑定部分参数来创建新的可调用对象，形成部分应用。使用占位符(_1、_2 等)来留下参数位置。
// 在新代码中优先使用 lambda 以提高可读性；std::bind 在快速实现部分应用或适配接口时仍然有用。

// Example functions and classes used in examples
// 示例中使用的函数和类
int add2(int a, int b) { return a + b; }
int add3(int a, int b, int c) { return a + b + c; }

struct Printer
{
    void operator()(int x) const { cout << "Printer: " << x << '\n'; }
};

class MyClass
{
public:
    int multiply(int a, int b) { return a * b; }
    void printPair(int x, int y) { cout << "pair: " << x << ", " << y << '\n'; }
};

int main()
{
// 1. std::function can store function pointers
// 1. std::function 可以存储函数指针
    function<int(int, int)> fptr = add2;
    cout << "fptr(2,3) = " << fptr(2, 3) << '\n';

// 2. std::function can store lambdas and function objects
// 2. std::function 可以存储 lambda 和函数对象
    function<void(int)> fprinter = [](int x) { 
        cout << "lambda: " << x << '\n'; 
    };
    fprinter(10);
    fprinter = Printer(); // assign a function object
    fprinter(20);

// 3. std::bind basic usage: bind free function
// 3. std::bind 基本用法：绑定自由函数
    auto bound_add = std::bind(add3, 5, std::placeholders::_1, std::placeholders::_2); // first arg fixed to 5
    cout << "bound_add(2,3) => add3(5,2,3) = " << bound_add(2, 3) << '\n';

// 4. bind member function to object instance
// 4. 绑定成员函数到对象实例
    MyClass obj;
    auto bound_mul = std::bind(&MyClass::multiply, &obj, std::placeholders::_1, std::placeholders::_2);
    cout << "bound_mul(4,5) = " << bound_mul(4, 5) << '\n';

// 5. placeholders can be reordered
// 5. 占位符可以重新排序参数
    auto reorder = std::bind(add3, std::placeholders::_2, std::placeholders::_1, 100);
    cout << "reorder(1,2) => add3(2,1,100) = " << reorder(1, 2) << '\n';

// 6. bind with references: use std::ref to bind by reference
// 6. 绑定引用：使用 std::ref 来按引用绑定参数
    int v = 10;
    auto inc_and_add = [](int &r, int x){ 
        r += x;
        return r; 
    };
    auto bound_ref = std::bind(inc_and_add, std::ref(v), std::placeholders::_1);
    cout << "v before = " << v << ", bound_ref(5) => " << bound_ref(5) << ", v after = " << v << '\n';

// 7. use std::function to store bind result
// 7. 使用 std::function 保存 bind 的结果
    function<int(int, int)> fb = bound_mul; // compatible signature
    cout << "fb(3,7) = " << fb(3, 7) << '\n';

// 8. adapt API: use bind to convert member function into unary callable
// 8. 适配接口：用 bind 将成员函数转换为一元可调用对象
    auto print_bound = std::bind(&MyClass::printPair, &obj, std::placeholders::_1, 999);
    print_bound(42); // calls obj.printPair(42, 999)

// 9. combine with STL: sort using bound comparator or lambda
// 9. 与 STL 结合：使用绑定的比较器或 lambda 进行排序
    vector<pair<int, int>> vec{{3, 1}, {2, 4}, {3, 0}};
    // sort by first, then second using lambda
    // 使用 lambda 按照第一个元素排序，如果相同则按照第二个元素排序
    sort(vec.begin(), vec.end(), [](const auto &a, const auto &b)
         { return a.first < b.first || (a.first == b.first && a.second < b.second); });
    cout << "sorted pairs:";
    for (auto &p : vec)
    {
        cout << " (" << p.first << "," << p.second << ")";
    }
    cout << '\n';

    // recommendation: prefer lambdas for clarity; use bind when needed
    // 建议：优先使用 lambda 以提高可读性；在需要时使用 bind

    return 0;
}