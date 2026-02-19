#include <iostream>
#include <vector>
using namespace std;

// move semantics is a feature of C++11 that allows objects to be moved rather than copied.
// 移动语义是C++11的一个特性，它允许对象被移动而不是复制。

class Resource
{
private:
    int *data;
    size_t size;

public:
    // Constructor: allocate elements
    // 构造函数: 分配元素
    Resource(size_t s) : size(s)
    {
        data = new int[size];
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = i;
        }
        cout << "Constructor: allocate " << size << " elements" << endl;
    }

    // Copy constructor: copy elements
    // 拷贝构造函数: 复制元素
    Resource(const Resource &other) : size(other.size)
    {
        data = new int[size];
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = other.data[i];
        }
        cout << "Copy constructor: copy " << size << " elements" << endl;
    }

    // Move constructor: transfer ownership
    // 移动构造函数: 转移所有权
    Resource(Resource &&other) noexcept : data(other.data),
                                          size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
        cout << "Move constructor: transfer " << size << " elements" << endl;
    }

    // Copy assignment: copy elements
    // 拷贝赋值: 复制元素
    Resource &operator=(const Resource &other)
    {
        if (this != &other)
        {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i)
            {
                data[i] = other.data[i];
            }
            cout << "Copy assignment: copy " << size << " elements" << endl;
        }
        return *this;
    }

    // Move assignment: transfer ownership
    // 移动赋值: 转移所有权
    Resource &operator=(Resource &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "Move assignment: transfer " << size << " elements" << endl;
        }
        return *this;
    }

    // Destructor: release resource
    // 析构函数: 释放资源
    ~Resource()
    {

        delete[] data;
        cout << "Destructor: release resource" << endl;
    }

    // get data pointer
    // 获取数据
    int *getData() const { return data; }
    // Get size
    // 获取大小
    size_t getSize() const { return size; }
};

// Perfect forwarding example
// 完美转发示例
// Forward declaration for process (needed for template)
// process的前向声明（模板需要）
void process(int &x);
void process(int &&x);
template <typename T>
void wrapper(T &&arg)
{
    cout << "wrapper调用，参数类型: " << typeid(arg).name() << endl;
    // Use std::forward to preserve value category
    // 使用std::forward保持参数的值类型
    process(std::forward<T>(arg));
}

// Definition of process overloads
void process(int &x)
{
    // Handle lvalue reference
    // 处理左值引用
    cout << "process(int &x) called" << endl;
    cout << "处理左值引用: " << x << endl;
}

void process(int &&x)
{
    // Handle rvalue reference
    // 处理右值引用
    cout << "process(int &&x) called" << endl;
    cout << "处理右值引用: " << x << endl;
}

// What is lvalue and rvalue?
// 什么是左值和右值？
// lvalue: An object that has a persistent address in memory (can appear on the left side of =).
//         Example: x = 1;  // x is an lvalue
// 左值：在内存中有持久地址的对象（可以出现在=左边）。
//      例：x = 1;  // x是左值
//
// rvalue: A temporary value or literal that does not have a persistent address (usually appears on the right side of =).
//         Example: y = x + 1;  // x+1 is an rvalue
// 右值：临时值或字面量，没有持久地址（通常出现在=右边）。
//      例：y = x + 1;  // x+1是右值
//
// Note: Variables like x can be both lvalue (when assigned) and rvalue (when used in expressions).
// 注意：变量x既可以作为左值（赋值时），也可以作为右值（表达式中使用时）。

// Why do we need & and && in function parameters?
// 为什么函数参数要有&和&&？
//
// int&  : Accepts only lvalues (existing variables).
// int&& : Accepts only rvalues (temporaries, literals, std::move results).
// int&  ：只接受左值（已有变量）。
// int&& ：只接受右值（临时对象、字面量、std::move的结果）。
//
// This allows us to write different code for lvalues and rvalues (e.g., copy for lvalue, move for rvalue).
// 这样我们可以针对左值和右值写不同的代码（如左值拷贝，右值移动）。

// What is perfect forwarding?
// 什么是完美转发？
//
// Perfect forwarding allows a function template to forward its arguments to another function,
// preserving whether each argument is an lvalue or rvalue.
// 完美转发允许函数模板把参数"原样"转发给另一个函数，保留每个参数的左值/右值属性。
//
// This is done using std::forward<T>(arg).
// 这通过std::forward<T>(arg)实现。

// Why is perfect forwarding useful?
// 完美转发有什么用？
//
// - It enables writing generic wrapper functions that do not lose efficiency.
// - It avoids unnecessary copies or moves.
// - It is essential for factory functions, emplace-style container methods, etc.
// - 让你写出高效的泛型包装函数。
// - 避免不必要的拷贝或移动。
// - 工厂函数、容器的emplace等场景必不可少。

// Example application: std::make_unique, std::make_shared, std::vector::emplace_back
// 应用举例：std::make_unique、std::make_shared、std::vector::emplace_back等

int main()
{
    cout << "=== move semantics Demo ===" << endl;
    cout << "=== 移动语义示例 ===" << endl;

// 1. base move semantics
// 1. 基本移动语义
    Resource r1(5);

    cout << endl;
    // Call move constructor
    // 调用移动构造函数
    Resource r2 = std::move(r1); 

    cout << endl;
    Resource r3(3);
    // Call move assignment operator
    // 调用移动赋值运算符
    r3 = std::move(r2);

// 2. using move semantics in containers
// 2. 在容器中使用移动语义
    // Temporary object, use move semantics
    // 临时对象，使用移动语义
    cout << endl;
    vector<Resource> resources;
    resources.push_back(Resource(10));

// 2. perfect forwarding
// 3. 完美转发
    cout << endl;
    int x = 42;
    // lvalue
    // 左值
    wrapper(x);
    
    // rvalue
    // 右值
    wrapper(100);

// 4. std::move usage
// 4. std::move的使用
    string str1 = "Hello";
    // str1 becomes empty
    // str1变为空字符串
    string str2 = std::move(str1);
    
    cout << "str1: '" << str1 << "'" << endl;
    cout << "str2: '" << str2 << "'" << endl;

    return 0;
}