#include <iostream>
#include <vector>
#include <map>

// contents: C++11 features demo
// 1. Initializer list (initializer_list)
// 2. Range-based for loop (range-based for)
// 3. nullptr vs NULL
// 4. Type alias (using vs typedef)
// 5. Delegating constructors
// 6. Inheriting constructors
// 7. enum class (scoped and strongly typed enums)
// 8. Defaulted and deleted functions
// 9. Variadic templates
// 10. std::atomic and atomic operations
// 11. std::regex
// 目录： C++11特性演示
// 1. 初始化列表 (initializer_list)
// 2. 范围 for 循环 (range-based for)
// 3. nullptr 与 NULL 的区别
// 4. 类型别名(using 与 typedef)
// 5. 委托构造函数
// 6. 继承构造函数
// 7. enum class(作用域与强类型枚举)
// 8. 默认与删除函数
// 9. 变长参数模板
// 10. std::atomic 与原子操作
// 11. 正则表达式(std::regex)

// ======================begin======================
// 1. Initializer list
// 1. 初始化列表(initializer_list)
void initializerListDemo()
{
    std::cout << std::endl;
    std::cout << "=== Initializer list Demo ===" << std::endl;
    std::cout << "=== 初始化列表 示例 ===" << std::endl;
    int x{42};
    double pi{3.14};
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> m = {{"apple", 1}, {"banana", 2}};
    std::cout << "init list: ";
    for (const auto &elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;
    class Container
    {
    public:
        Container(std::initializer_list<int> list)
        {
            for (auto v : list)
                data_.push_back(v);
        }

    private:
        std::vector<int> data_;
    };
    Container c{1, 2, 3, 4, 5};
}

// 2. Range-based for loop
// 2. 范围 for 循环
void rangeForDemo()
{
    std::cout << std::endl;
    std::cout << "=== Range-based for Demo ===" << std::endl;
    std::cout << "=== 范围 for 示例 ===" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "range for loop: ";
    for (const auto x : vec)
        std::cout << x << " ";
    std::cout << std::endl;
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
    for (const auto &p : m)
        std::cout << p.first << ":" << p.second << " ";
    std::cout << std::endl;
    for (auto &x : vec)
        x *= 2;
    class MyContainer
    {
    public:
        int *begin() { return data_; }
        int *end() { return data_ + size_; }

    private:
        int data_[5] = {1, 2, 3, 4, 5};
        int size_ = 5;
    };
    MyContainer mc;
    for (int x : mc)
        std::cout << x << " ";
    std::cout << std::endl;
}

// 3. nullptr vs NULL
// 3.nullptr 与 NULL 的区别
void f(int)
{
    std::cout << "f(int) called" << std::endl;
}

void f(char *)
{
    std::cout << "f(char*) called" << std::endl;
}

void nullptrDemo()
{
    std::cout << std::endl;
    std::cout << "=== nullptr vs NULL Demo ===" << std::endl;
    std::cout << "=== nullptr 与 NULL ===" << std::endl;
    int *ptr1 = nullptr;
    int *ptr2 = NULL;
    if (ptr1 == nullptr)
        std::cout << "ptr is nullptr" << std::endl;
    if (ptr2 == NULL)
        std::cout << "ptr2 is NULL" << std::endl;
    f(nullptr); // calls f(char*)
}

// 4. Type alias (using vs typedef)
// 4. 类型别名(using 与 typedef)
template <typename T>
using Ptr = T *; // alias template
void typeAliasDemo()
{
    std::cout << std::endl;
    std::cout << "=== Type alias Demo ===" << std::endl;
    std::cout << "=== 类型别名 示例 ===" << std::endl;

    using IntVector = std::vector<int>;
    typedef std::vector<int> IntVectorOld;
    IntVector numbers = {1, 2, 3};
    IntVectorOld numbers2 = {4, 5, 6};
    Ptr<int> ptr3 = numbers.data();
    (void)ptr3;
    typedef int *IntPtr;
    IntPtr ptr4 = numbers2.data();
    (void)ptr4;
}

// 5. Delegating constructors
// 5. 委托构造函数
void delegatingConstructorsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Delegating constructors Demo ===" << std::endl;
    std::cout << "=== 委托构造函数 示例 ===" << std::endl;

    class Point
    {
        int x, y;

    public:
        Point() : Point(0, 0) {}
        Point(int x) : Point(x, 0) {}
        Point(int x, int y) : x(x), y(y) {}
        void print() const { std::cout << "Point(" << x << ", " << y << ")" << std::endl; }
    };
    Point p1, p2(5), p3(3, 4);
    p1.print();
    p2.print();
    p3.print();
}

// 6. Inheriting constructors
// 6. 继承构造函数
void inheritingConstructorsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Inheriting constructors Demo ===" << std::endl;
    std::cout << "=== 继承构造函数 示例 ===" << std::endl;

    class Base
    {
    public:
        Base(int x) { std::cout << "Base(" << x << ")" << std::endl; }
    };
    class Derived : public Base
    {
    public:
        using Base::Base;
    };
    Derived d(42);
}

// 7. enum class (scoped and strongly typed enums)
// 7. enum class(作用域与强类型枚举)
void enumClassDemo()
{
    std::cout << std::endl;
    std::cout << "=== enum class Demo ===" << std::endl;
    std::cout << "=== enum class 示例 ===" << std::endl;

    enum Color_old
    {
        Red,
        Green,
        Blue
    };
    Color_old color_old = Red;
    int temp = color_old;
    (void)temp;
    enum class Color
    {
        Red,
        Green,
        Blue
    };
    Color color = Color::Red;
    temp = static_cast<int>(color);
    (void)temp;
}

// 8. Defaulted and deleted functions
// 8. 默认与删除函数
void defaultDeleteDemo()
{
    std::cout << std::endl;
    std::cout << "=== Defaulted & Deleted Demo ===" << std::endl;
    std::cout << "=== 默认与删除函数 示例 ===" << std::endl;
    class MyClass
    {
    public:
        MyClass() = default;
        MyClass(const MyClass &) = default;
        MyClass &operator=(const MyClass &) = default;
        MyClass(int) = delete;
        void func() = delete;
    };
    MyClass obj1;
    MyClass obj2(obj1);
    (void)obj2;
}

// 9. Variadic templates
// 9. 变长参数模板
// 9. Variadic templates
// 9. 变长参数模板
template<typename... Args>
void func(Args... args) 
{
    // process parameter pack
    // 处理参数包
}

// Recursive expansion for printing
// 递归展开用于打印
template<typename T>
void print(T t) 
{
    std::cout << t << std::endl;
}
template<typename T, typename... Args>
void print(T t, Args... args) 
{
    std::cout << t << " ";
    print(args...);
}

void variadicTemplatesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Variadic templates Demo ===" << std::endl;
    std::cout << "=== 变长参数模板 示例 ===" << std::endl;

    print(1, "hello", 3.14);
}

// 10. std::atomic
// 10. std::atomic 与原子操作
#include <atomic>
void atomicDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::atomic Demo ===" << std::endl;
    std::cout << "=== 原子操作 示例 ===" << std::endl;

    std::atomic<int> counter{0};
    ++counter;
    --counter;
    (void)counter;
}

// 11. std::regex
// 11. 正则表达式(std::regex)
#include <regex>
void regexDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::regex Demo ===" << std::endl;
    std::cout << "=== 正则表达式 示例 ===" << std::endl;

    std::string text = "Hello, my email is test@example.com";
    std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::smatch match;
    if (std::regex_search(text, match, email_pattern))
        std::cout << "Found email: " << match[0] << std::endl;
    std::string result = std::regex_replace(text, email_pattern, "[EMAIL]");
    (void)result;
}

int main()
{
    std::cout << "=== C++11 features Demo ===" << std::endl;
    std::cout << "=== C++11 特性示例 ===" << std::endl;

    initializerListDemo();
    rangeForDemo();
    nullptrDemo();
    typeAliasDemo();
    delegatingConstructorsDemo();
    inheritingConstructorsDemo();
    enumClassDemo();
    defaultDeleteDemo();
    variadicTemplatesDemo();
    atomicDemo();
    regexDemo();

    return 0;
}
