#include <iostream>
#include <memory>
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

// 智能指针示例
class SmartPointerExample
{
private:
    string name;

public:
    SmartPointerExample(const string &n) : name(n)
    {
        cout << "create object: " << name << endl;
        // cout << "创建对象: " << name << endl;
    }

    ~SmartPointerExample()
    {
        cout << "destroy object: " << name << endl;
        // cout << "销毁对象: " << name << endl;
    }

    void doSomething()
    {
        cout << name << " is working..." << endl;
        // cout << name << " 正在工作..." << endl;
    }
};

// 循环引用示例
class CircularReference
{
public:
    shared_ptr<CircularReference> other;

    CircularReference()
    {
        cout << "CircularReference create" << endl;
        // cout << "CircularReference 创建" << endl;
    }

    ~CircularReference()
    {
        cout << "CircularReference destory" << endl;
        // cout << "CircularReference 销毁" << endl;
    }
};

void smartPointerDemo()
{
    cout << "\n=== Smart Pointer Demo ===" << endl;
    // cout << "\n=== 智能指针示例 ===" << endl;

    // unique_ptr - alone ownership
    // unique_ptr - 独占所有权
    {
        unique_ptr<SmartPointerExample> ptr1(new SmartPointerExample("UniquePtr"));
        ptr1->doSomething();
        // when ptr1 goes out of scope, memory is automatically released
        // ptr1离开作用域时自动释放
    }

    // shared_ptr - shared ownership
    // shared_ptr - 共享所有权
    {
        shared_ptr<SmartPointerExample> ptr1 =
            make_shared<SmartPointerExample>("SharedPtr1");
        {
            // reference count +1
            // 引用计数+1
            shared_ptr<SmartPointerExample> ptr2 = ptr1;
            cout << "reference count: " << ptr1.use_count() << endl;
            // cout << "引用计数: " << ptr1.use_count() << endl;
        }
        // when ptr2 goes out of scope, reference count -1
        // ptr2离开作用域，引用计数-1

        cout << "reference count: " << ptr1.use_count() << endl;
        // cout << "引用计数: " << ptr1.use_count() << endl;
    }
    // when ptr1 goes out of scope, reference count is 0, memory is automatically released
    // ptr1离开作用域，引用计数变为0，自动释放

    // weak_ptr - weak reference, does not increase reference count
    // weak_ptr - 弱引用，不增加引用计数
    {
        shared_ptr<SmartPointerExample> ptr1 =
            make_shared<SmartPointerExample>("SharedPtr");
        weak_ptr<SmartPointerExample> weakPtr = ptr1;

        cout << "shared_ptr reference count: " << ptr1.use_count() << endl;
        // cout << "shared_ptr引用计数: " << ptr1.use_count() << endl;

        cout << "weak_ptr reference count: " << weakPtr.use_count() << endl;
        // cout << "weak_ptr引用计数: " << weakPtr.use_count() << endl;

        // shared_ptr is the owner (like a dog owner holding the leash)
        // weak_ptr is a passerby (observer)
        // .lock() tries to hold the leash
        // shared_ptr 是持有者（比如牵着狗的主人）
        // weakPtr 是一个路人（观察者）
        // .lock() 是尝试去牵绳子
        if (auto sharedPtr = weakPtr.lock())
        {
            // when entering here, .lock() succeeded! Returned a valid sharedPtr. The object is still alive!
            // now sharedPtr is also one of the new owners, the object will definitely not be destroyed here.
            // 进入这里，说明 .lock() 成功了！返回了一个有效的 sharedPtr。说明对象还活着！
            // 此时 sharedPtr 也是新的持有者之一，对象绝对不会在这里被销毁。
            cout << "obj is still alive, I can safely use it" << endl;
            // cout << "对象还活着，我可以安全地使用它" << endl;
        }
        else
        {
            // when entering here, .lock() returned nullptr.
            // indicates the object has been destructed (dead).
            // 进入这里，说明 .lock() 返回了 nullptr。
            // 说明对象已经析构了（死透了）。
            cout << "obj is gone, do not access it" << endl;
            cout << "对象已经没了，别访问了" << endl;
        }
    }
}

void memoryLeakDemo()
{
    cout << "\n=== memory Leak Demo ===" << endl;

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

    // situation 3: circular reference
    // 情况3：循环引用
    auto obj1 = make_shared<CircularReference>();
    auto obj2 = make_shared<CircularReference>();
    obj1->other = obj2;
    obj2->other = obj1;
    // both objects will not be destroyed due to circular reference
    // 两个对象都不会被销毁，因为引用计数永远不会为0
}

void sizeofDemoFunc(char Funcstr[100])
{
    // sizeof(Funcstr) = 8: the function parameter is a character array name, i.e., the address of the first element of the array, size is the size of the pointer
    // sizeof(Funcstr) = 8: 函数的参数为字符数组名，即数组首元素的地址，大小为指针的大小
    cout << "sizeof(Funcstr): " << sizeof(Funcstr) << endl;
}

void sizeofDemo()
{
    cout << "\n=== sizeof Demo ===" << endl;

    cout << "sizeof(char): " << sizeof(char) << " byte(s)" << endl;
    cout << "sizeof(int): " << sizeof(int) << " byte(s)" << endl;
    cout << "sizeof(float): " << sizeof(float) << " byte(s)" << endl;
    cout << "sizeof(double): " << sizeof(double) << " byte(s)" << endl;
    cout << "sizeof(void*): " << sizeof(void *) << " byte(s)" << endl;

    struct MyStruct
    {
        char a;   // 4 byte
        int b;    // 4 bytes
        double c; // 8 bytes
    };

    cout << "sizeof(MyStruct): " << sizeof(MyStruct) << " byte(s)" << endl;

    char str[] = "hello";
    char *p1 = str;
    int n = 10;

    // sizeof(str) = 6: calculates the size of the array in memory, including the terminating '\0'
    // sizeof(str) = 6: 计算的是数组的所占内存的大小，包括末尾的'\0'
    cout << "sizeof(str): " << sizeof(str) << endl;

    // sizeof(p) = 8: p is a pointer variable, size is 8 bytes on a 64-bit system
    // sizeof(p) = 8: p 为指针变量，64位系统下大小为8 bytes
    cout << "sizeof(p1): " << sizeof(p1) << endl;

    // sizeof(n) = 4: n is an integer variable, occupies 4 bytes of memory
    // sizeof(n) = 4: n 是整型变量，占用内存空间4个字节
    cout << "sizeof(n): " << sizeof(n) << endl;

    sizeofDemoFunc(str);

    // sizeof(p) = 8: p is a pointer pointing to the memory allocated by malloc of size 100 bytes, sizeof(p) is the size of the pointer, not the size of the memory it points to
    // sizeof(p) = 8: p 指向malloc分配的大小为100 byte的内存的起始地址，sizeof(p)为指针的大小，而不是它指向内存的大小
    void *p2 = malloc(100);
    cout << "sizeof(p2): " << sizeof(p2) << endl;
}

// RAII(Resource Acquisition Is Initialization)(资源获取即初始化)
// RAII is said to bind the lifecycle of resources to the lifecycle of an object
// create (constructor): when you create an object, acquire resources in the constructor (e.g., allocate memory with new, open files, lock).
// destroy (destructor): when the object is destroyed, release resources in the destructor (e.g., free memory with delete, close files, unlock).
// RAII说白了就是把资源的生命周期，绑定到一个对象的生命周期上
// 生（构造函数）：当你创建一个对象时，在构造函数里获取资源（比如申请内存 new、打开文件、加锁）。
// 死（析构函数）：当对象销毁时，在析构函数里释放资源（比如释放内存 delete、关闭文件、解锁）。
class RAIIExample
{
private:
    int *data;

public:
    RAIIExample() : data(nullptr)
    {
        data = new int[10];
        cout << "RAII: 资源获取" << endl;
    }

    ~RAIIExample()
    {
        if (data)
        {
            delete[] data;
            cout << "RAII: 资源释放" << endl;
        }
    }

    // 禁用拷贝
    RAIIExample(const RAIIExample &) = delete;
    RAIIExample &operator=(const RAIIExample &) = delete;

    // 允许移动
    RAIIExample(RAIIExample &&other) noexcept : data(other.data)
    {
        other.data = nullptr;
        cout << "RAII: 移动构造" << endl;
    }

    RAIIExample &operator=(RAIIExample &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            other.data = nullptr;
            cout << "RAII: 移动赋值" << endl;
        }
        return *this;
    }
};

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

    sizeofDemo();

    smartPointerDemo();

    return 0;
}