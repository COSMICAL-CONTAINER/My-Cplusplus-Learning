#include <iostream>
#include <exception> // standard exception base class
#include <stdexcept> // standard exception classes like runtime_error
#include <string>

using namespace std;

// 1. Custom Exception Class
// 1. 自定义异常类
// Inherits from std::exception is the best practice
// 继承自 std::exception 是最佳实践
class MyCustomException : public exception
{
private:
    string message;

public:
    MyCustomException(const string &msg) : message(msg) {}

    // override what() method to return error message
    // 重写 what() 方法返回错误信息
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

// Helper class to demonstrate Stack Unwinding
// 用于演示栈展开（Stack Unwinding）的辅助类
class Resource
{
public:
    string name;
    Resource(const string &n) : name(n)
    {
        cout << "Resource [" << name << "] constructed." << endl;
        // cout << "资源 [" << name << "] 已构造。" << endl;
    }

    ~Resource()
    {
        cout << "Resource [" << name << "] destroyed (Stack Unwinding)." << endl;
        // cout << "资源 [" << name << "] 已销毁 (栈展开)。" << endl;
    }
};

// 2. Demo: Throwing basic types (not recommended but possible)
// 2. 演示：抛出基本类型（不推荐但在语法上是允许的）
void basicExceptionDemo(int x)
{
    cout << "\n--- Basic Exception Demo ---" << endl;
    try
    {
        if (x == 0)
        {
            // throw integer
            // 抛出整数
            throw 404;
        }
        else if (x < 0)
        {
            // throw string literal
            // 抛出字符串字面量
            throw "Negative number error";
        }
        cout << "Calculation result: " << 100 / x << endl;
    }
    catch (int e)
    {
        cout << "Caught int exception: error code " << e << endl;
        // cout << "捕获整数异常：错误码 " << e << endl;
    }
    catch (const char *e)
    {
        cout << "Caught string exception: " << e << endl;
        // cout << "捕获字符串异常：" << e << endl;
    }
    catch (...)
    {
        // Catch-all handler (ellipses)
        // 捕获所有其他类型的异常
        cout << "Caught unknown exception" << endl;
        // cout << "捕获未知异常" << endl;
    }
}

// 3. Demo: Standard Exceptions and Stack Unwinding
// 3. 演示：标准异常与栈展开
// This demonstrates RAII: destructors are called even if exception occurs
// 这演示了 RAII：即使发生异常，析构函数也会被调用
void stackUnwindingDemo()
{
    cout << "\n--- Stack Unwinding & Std Exception ---" << endl;
    try
    {
        // Object created on stack
        // 在栈上创建对象
        Resource res("NetworkConnection");

        cout << "Processing data..." << endl;

        // Simulate an error
        // 模拟一个错误
        // logic_error, runtime_error, out_of_range are common
        // logic_error, runtime_error, out_of_range 是常见的标准异常
        throw runtime_error("Connection timed out");

        // This line will NOT be executed
        // 这行代码不会被执行
        cout << "Task finished." << endl;
    }
    catch (const exception &e)
    {
        // Polymorphism: catching base class reference
        // 多态：通过基类引用捕获
        cout << "Caught standard exception: " << e.what() << endl;
        // cout << "捕获标准异常：" << e.what() << endl;
    }
    // notice: Resource destructor is called BEFORE catch block finishes (or enters)
    // 注意：Resource 的析构函数会在异常处理流程中自动调用
}

// 4. Demo: Custom Exception and Rethrowing
// 4. 演示：自定义异常与重新抛出
void innerFunction()
{
    try
    {
        throw MyCustomException("Database error in inner function");
    }
    catch (const MyCustomException &e)
    {
        cout << "Inner catch: " << e.what() << endl;
        // cout << "内部捕获：" << e.what() << endl;

        // Log the error and rethrow it to outer layer
        // 记录错误并将其重新抛出给外层
        cout << "Rethrowing exception..." << endl;
        // cout << "重新抛出异常..." << endl;
        throw; // throw without arguments rethrows the current exception
    }
}

void rethrowDemo()
{
    cout << "\n--- Custom Exception & Rethrow Demo ---" << endl;
    try
    {
        innerFunction();
    }
    catch (const MyCustomException &e)
    {
        cout << "Outer catch: Handled rethrown exception -> " << e.what() << endl;
        // cout << "外部捕获：处理重新抛出的异常 -> " << e.what() << endl;
    }
}

// 5. Demo: Constructor failure
// 5. 演示：构造函数失败
class DangerObject
{
public:
    DangerObject()
    {
        cout << "DangerObject constructor started..." << endl;
        // If constructor throws, destructor is NOT called for this object
        // 如果构造函数抛出异常，该对象的析构函数不会被调用
        throw runtime_error("Failed to init");
    }
    ~DangerObject()
    {
        // This will not run
        cout << "DangerObject destructor" << endl;
    }
};

void constructorExceptionDemo()
{
    cout << "\n--- Constructor Failure Demo ---" << endl;
    try
    {
        DangerObject obj;
    }
    catch (const exception &e)
    {
        cout << "Caught exception from constructor: " << e.what() << endl;
        // cout << "捕获构造函数异常：" << e.what() << endl;
    }
}

int main()
{
    // Test basic types (int, string)
    // 测试基本类型 (int, string)
    basicExceptionDemo(0);
    basicExceptionDemo(-5);

    // Test RAII and Stack Unwinding (Crucial C++ concept)
    // 测试 RAII 和栈展开 (C++核心概念)
    stackUnwindingDemo();

    // Test Custom Exceptions and Rethrow logic
    // 测试自定义异常和重新抛出逻辑
    rethrowDemo();

    // Test Exception in Constructor
    // 测试构造函数中的异常
    constructorExceptionDemo();

    return 0;
}