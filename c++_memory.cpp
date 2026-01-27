#include <iostream>

using namespace std;

// memory partitioning in C++
// 内存分区示例

// global variables - global/static area
// 全局变量 - 全局/静态区
int globalVar = 100;

// static variables - global/static area
// 静态变量 - 全局/静态区
static int staticVar = 200;

void memoryPartitionDemo()
{
    // local variables - stack area
    // 局部变量 - 栈区
    int localVar = 10;

    // local static variables - global/static area
    // 局部静态变量 - 全局/静态区
    static int localStaticVar = 300;

    // string literals - constant area
    // 字符串常量 - 常量区
    const char *str = "Hello";

    cout << "global variables address: " << &globalVar << endl;
    cout << "static variables address: " << &staticVar << endl;
    cout << "local variables address: " << &localVar << endl;
    cout << "local static variables address: " << &localStaticVar << endl;
    cout << "string literals address: " << (void *)str << endl;
}

// memory leak demonstration
// 内存泄漏示例
class MemoryLeakExample
{
private:
    int *data;

public:
    MemoryLeakExample()
    {
        // mallocate memory
        // 分配内存
        data = new int[10];
        cout << "constructor: allocated memory" << endl;
        // cout << "构造函数: 分配内存" << endl;
    }

    // error: no destructor, leads to memory leak
    // 错误: 没有析构函数，会导致内存泄漏
    // ~MemoryLeakExample() {
    //     delete[] data;
    //     cout << "destructor: released memory" << endl;
    //     cout << "析构函数: 释放内存" << endl;
    // }
};

// demonstrate correct memory management
// 正确的内存管理
class CorrectMemoryExample
{
private:
    int *data;

public:
    CorrectMemoryExample()
    {
        data = new int[10];
        cout << "constructor: allocated memory" << endl;
        // cout << "构造函数：分配内存" << endl;
    }

    ~CorrectMemoryExample()
    {
        delete[] data;
        cout << "destructor: released memory" << endl;
        // cout << "析构函数：释放内存" << endl;
    }

    // 拷贝构造函数
    // copy constructor
    CorrectMemoryExample(const CorrectMemoryExample &other)
    {
        data = new int[10];
        for (int i = 0; i < 10; i++)
        {
            data[i] = other.data[i];
        }
        cout << "copy constructor" << endl;
        // cout << "拷贝构造函数" << endl;
    }

    // 赋值运算符
    // assignment operator
    CorrectMemoryExample &operator=(const CorrectMemoryExample &other)
    {
        if (this != &other)
        {
            delete[] data;
            data = new int[10];
            for (int i = 0; i < 10; i++)
            {
                data[i] = other.data[i];
            }
        }
        cout << "assignment operator" << endl;
        // cout << "赋值运算符" << endl;
        return *this;
    }
};

void memoryLeakDemo()
{
    cout << "=== memory Leak Demo ===" << endl;

    // situation 1: pointer reassignment without releasing memory
    // 情况1：指针指向改变，未释放内存
    int *ptr = new int(10);
    // memort leak! The original memory is not released
    // 内存泄漏！原来的内存没有释放
    ptr = new int(20);
    // only the second allocated memory is released
    // 只释放了第二次分配的内存
    delete ptr;

    // situation 2: exception leading to unreleased memory
    // 情况2：异常导致内存未释放
    try
    {
        int *ptr2 = new int[1000];
        throw runtime_error("error occurred");
        // this code will not be executed
        // 这行代码不会执行
        delete[] ptr2;
    }
    catch (const exception &e)
    {
        cout << "catch error: " << e.what() << endl;
        // memory leak! The allocated memory is not released
        // 内存泄漏！
    }
}

int main()
{
    cout << "=== memory Partition Demo ===" << endl;
    // cout << "=== 内存分区示例 ===" << endl;
    memoryPartitionDemo();

    cout << "\n=== correct memory manage Demo ===" << endl;
    // cout << "\n=== 正确的内存管理示例 ===" << endl;
    {
        CorrectMemoryExample obj1;
        // call copy constructor
        // 调用拷贝构造函数
        CorrectMemoryExample obj2 = obj1;
        // call assignment operator
        // 调用赋值运算符
        obj1 = obj2;
    } 
    // auto call destructor here
    // 自动调用析构函数

    memoryLeakDemo();

    return 0;
}