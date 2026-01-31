#include <iostream>
#include <vector>
#include <algorithm>  // for std::sort, std::for_each
#include <functional> // for std::function

using namespace std;

// Lambda Expressions in C++
// C++ 中的 Lambda 表达式
// [capture list](parameter list) mutable -> return type { function body }
// [捕获列表](参数列表) mutable -> 返回类型 { 函数体 }

// 1. Basic Syntax Demo
// 1. 基本语法示例
void basicDemo()
{
    cout << "=== Basic Syntax ===" << endl;

    // A simple lambda with no capture, no params
    // 一个无捕获、无参数的最简 lambda
    auto sayHello = []()
    {
        cout << "Hello Lambda!" << endl;
    };

    sayHello(); // call it like a function 就像普通函数一样调用它

    // Lambda with parameters and return type
    // 带参数和返回值的 lambda
    // "-> int" is optional, compiler can deduce it usually
    // "-> int" 是可选的，编译器通常能自动推导
    auto add = [](int a, int b) -> int
    {
        return a + b;
    };

    cout << "1 + 2 = " << add(1, 2) << endl;
}

// 2. Capture Clause Demo (The most important part)
// 2. 捕获列表示例（最重要的部分）

// [ ] : empty, capture nothing
// [=] : capture all local variables by value (copy),
//       note: by default, value-captured variables are read-only.
//       If you want to modify the copy (e.g., x++), you must add the mutable keyword after the parameter list
// [&] : capture all local variables by reference, meaning changes inside the Lambda affect the outside variables
// [x, &y] : capture x by value, y by reference
// [this] : capture the this pointer by value (for member functions)

// [ ] : 空，什么都不捕获
// [=] : 按值捕获所有局部变量（拷贝）
//       注意：默认情况下，拷贝进来的变量是只读的（read-only）。
//       如果你想修改这个拷贝（比如 x++），必须在参数后加 mutable 关键字
// [&] : 按引用捕获所有局部变量，也就是你在 Lambda 里修改变量，外面的变量真的会变
// [x, &y] : x 按值捕获，y 按引用捕获
// [this] : 按值捕获 this 指针（用于成员函数）
void captureDemo()
{
    cout << "\n=== Capture List Demo ===" << endl;

    int x = 100;
    int y = 200;

    // Scenario A: Capture by Value [=] or [x]
    // 场景 A: 按值捕获 (拷贝一份)
    // 'mutable' is required if you want to modify value-captured variables inside
    // 如果想在内部修改按值捕获的变量，必须加 'mutable' 关键字
    auto captureByValue = [x]() mutable
    {
        x++; // this modifies the COPY inside lambda, not the outer x
             // 这里修改的是 lambda 内部的拷贝，不是外面的 x
        cout << "Inside captureByValue: x = " << x << endl;
    };

    captureByValue();
    cout << "After captureByValue: x = " << x << " (Unchanged)" << endl;
    // cout << "调用后: x = " << x << " (外部变量未变)" << endl;

    // Scenario B: Capture by Reference [&] or [&x]
    // 场景 B: 按引用捕获 (操作本体)
    auto captureByRef = [&x]()
    {
        x++; // this modifies the actual outer x
             // 直接修改外面的 x
        cout << "Inside captureByRef: x = " << x << endl;
    };

    captureByRef();
    cout << "After captureByRef: x = " << x << " (Changed!)" << endl;
    // cout << "调用后: x = " << x << " (变了!)" << endl;

    // Scenario C: Mixed Capture
    // 场景 C: 混合捕获
    // capture y by ref, everything else (x) by value
    // y 按引用，其他的 (x) 按值
    auto mixedCapture = [=, &y]()
    {
        y++; // changes outer y
        // x++; // allowed if mutable is present, but changes copy
        cout << "Mixed: x=" << x << ", y=" << y << endl;
    };
    mixedCapture();
}

// 3. Using Lambda with STL
// 3. 在 STL 中使用 Lambda
void stlDemo()
{
    cout << "\n=== STL Integration ===" << endl;

    vector<int> nums = {4, 1, 8, 5, 3};

    // Sort descending using lambda
    // 使用 lambda 进行降序排序
    sort(nums.begin(), nums.end(), [](int a, int b)
         {
             return a > b; // logic: is a greater than b?
         });

    // For_each with lambda
    // 使用 for_each 遍历打印
    cout << "Sorted numbers: ";
    for_each(nums.begin(), nums.end(), [](int n)
             { cout << n << " "; });
    cout << endl;
}

// 4. std::function (Storing lambdas)
// 4. std::function (存储 Lambda)
// If you want to pass lambda as a parameter explicitly
// 如果你想把 lambda 显式地作为参数传递
void runCallback(function<void(int)> func, int value)
{
    func(value);
}

void functionTypeDemo()
{
    cout << "\n=== std::function Type ===" << endl;

    // std::function<return_type(param_types)>
    // 定义一个可以存储 lambda 的对象
    function<int(int, int)> operation;

    // assign lambda to it
    // 把 lambda 赋值给它
    operation = [](int a, int b)
    { return a * b; };

    cout << "5 * 6 = " << operation(5, 6) << endl;

    // Pass lambda to another function
    // 把 lambda 传给另一个函数
    runCallback([](int v)
                { cout << "Callback received: " << v << endl; }, 999);
}

int main()
{
    basicDemo();
    captureDemo();
    stlDemo();
    functionTypeDemo();
    return 0;
}