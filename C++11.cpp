#include <iostream>
#include <vector>
#include <map>
#include <atomic>
#include <regex>

using namespace std;

// 3. nullptr vs NULL
// 3. nullptr 与 NULL 的区别
void f(int)
{ 
    cout << "f(int) called" << endl; 
}

void f(char *)
{
    cout << "f(char*) called" << endl; 
}

// 4. Type alias: using vs typedef
// 4. 类型别名：using 与 typedef 的区别
template<typename T>
using Ptr = T*; // alias template

// 9. Variadic templates
// 9. 变长参数模板
template<typename... Args>
void func(Args... args) {
    // process parameter pack
    // 处理参数包
}

// Recursive expansion for printing
// 递归展开用于打印
template<typename T>
void print(T t) {
    cout << t << endl;
}
template<typename T, typename... Args>
void print(T t, Args... args) {
    cout << t << " ";
    print(args...);
}

int main()
{
    cout << "=== C++11 features Demo ===" << endl;
    cout << "=== C++11 特性示例 ===" << endl;

// 1. 初始化列表
// 1. Initializer list
    // base types - 基础类型
    int x{42};
    double pi{3.14};

    // arrays and containers - 数组与容器
    int arr[] = {1, 2, 3, 4, 5};

    vector<int> vec = {1, 2, 3, 4, 5};
    map<string, int> m = {{"apple", 1}, {"banana", 2}};

    cout << "init list: ";
    for (const auto &elem : vec)
        cout << elem << " ";
    cout << endl;

    // user-defined class - 自定义类
    // initializer_list can also be used in user-defined classes to allow initialization with brace-enclosed lists.
    // initializer_list 也可以用于用户定义的类，使其支持用花括号括起来的列表进行初始化。
    class Container {
    public:
        Container(std::initializer_list<int> list)
        {
            for (auto x : list)
            {
                data_.push_back(x);
            }
        }
    private:
        vector<int> data_;
    };
    Container c{1, 2, 3, 4, 5};

// 2. 范围for循环
// 2. Range-based for loop
    cout << "range for loop: ";

    // value (copy of elements)
    // 按值遍历(元素的副本)
    for (const auto x: vec)
    {
        cout << x << " ";
    }
    cout << endl;

    // reference (avoid copying)
    // 按引用遍历(避免拷贝)
    for (const auto &pair : m)
    {
        cout << pair.first << ":" << pair.second << " ";
    }
    cout << endl;

    // modify elements (non-const reference)
    // 修改元素(非常量引用)
    for (auto &x : vec) 
    {
        x *= 2;
    }

    // user-defined container - 用户定义的容器
    class MyContainer
    {
    public:
        int* begin() { return data_; }
        int* end() { return data_ + size_; }
        
    private:
        int data_[5] = {1, 2, 3, 4, 5};
        int size_ = 5;
    };
    MyContainer mc;
    for (int x : mc) 
    {
        cout << x << " ";
    }
    cout << endl;

// 3. nullptr vs NULL
// 3. nullptr 与 NULL 的区别
    int *ptr1 = nullptr;
    int *ptr2 = NULL;

    // In C++98 NULL is 0, which can be assigned to int, but nullptr cannot be assigned to int
    // 在C++98中NULL其实是0，可以赋值给int，但nullptr不能赋值给int
    int n = NULL; // warning: ISO C++11 does not allow conversion from string literal to 'char *' [-Wwritable-strings]

    // nullptr is a keyword representing a pointer of type std::nullptr_t (type-safe)
    // NULL is typically defined as 0 or ((void*)0), not type-safe
    // nullptr 是C++11引入的关键字，类型为std::nullptr_t，类型安全
    // NULL 通常是0或((void*)0)，不是类型安全的

    if (ptr1 == nullptr)
    {
        cout << "ptr is nullptr" << endl;
    }
    if (ptr2 == NULL)
    {
        cout << "ptr2 is NULL" << endl;
    }

    // The following will not compile:
    // error: cannot convert nullptr to int
    // 下面这句不能编译通过：
    // 错误，nullptr不能赋值给int

    // int n2 = nullptr; 

    // Directly call f(NULL) and f(nullptr) to show the difference
    // 直接调用f(NULL)和f(nullptr)以演示区别

    // f(NULL) is ambiguous in C++11 if both f(int) and f(char*) exist
    // C++11中如果有f(int)和f(char*)，f(NULL)会二义性报错（NULL既可转int也可转指针）
    // 但在某些编译器下NULL为0，优先匹配int，老代码可能无警告
    // f(nullptr)始终只会匹配指针重载，类型安全

#if 0
    // cancel the comment below to see the ambiguity error
    // 取消注释下行可观察二义性报错
    f(NULL); // ambiguous in C++11
#endif
    f(nullptr); // always calls f(char*)

    // Recommendation: use nullptr instead of NULL for pointer overload resolution to avoid ambiguity
    // 推荐：用nullptr替代NULL做指针重载选择，避免二义性

// 4. Type alias: using vs typedef
// 4. 类型别名：using 与 typedef 的区别
    // using is the modern way (C++11), typedef is the old way (C/C++)
    // using 是C++11推荐的现代写法，typedef是传统C/C++写法

    using IntVector = vector<int>;           // using alias
    typedef vector<int> IntVectorOld;        // typedef alias

    IntVector numbers = {1, 2, 3};
    IntVectorOld numbers2 = {4, 5, 6};

    // using can be used for templates (alias templates), typedef cannot
    // using 可用于模板类型别名(别名模板)，typedef不行
    Ptr<int> ptr3 = numbers.data(); // int*

    // typedef cannot do this:
    // typedef T* Ptr2<T>; // 错误，typedef不能用于模板
    typedef int* IntPtr;
    IntPtr ptr4 = numbers2.data();  // int*

// 5. Delegating constructors
// 5. 委托构造函数
    class Point
    {
    private:
        int x, y;

    public:
    // Delegating constructors allow one constructor to call another constructor in the same class.
    // This helps avoid code duplication and centralizes initialization logic.
    // 委托构造函数允许一个构造函数调用同一个类中的另一个构造函数。
    // 这样可以避免重复代码，让初始化逻辑集中管理。
        Point() : Point(0, 0) {}
        Point(int x) : Point(x, 0) {}

        Point(int x, int y) : x(x), y(y) {}

        void print() const
        {
            cout << "Point(" << x << ", " << y << ")" << endl;
        }
    };

    Point p1, p2(5), p3(3, 4);
    p1.print();
    p2.print();
    p3.print();

// 6. Inheriting constructors
// 6. 继承构造函数
    class Base
    {
    public:
        Base(int x) { cout << "Base(" << x << ")" << endl; }
    };

    class Derived : public Base
    {
    public:
    // using Base::Base; means inheriting all constructors from the Base class.
    // This allows Derived to be constructed just like Base.
    // However, destructors are NOT inherited. Derived must define its own destructor if needed.
    // using Base::Base; 表示继承Base类的所有构造函数。
    // 这样Derived可以像Base一样被构造。
    // 但析构函数不会被继承，如果需要特殊析构逻辑，Derived必须自己定义析构函数。
        using Base::Base;
    };

    // Call inherited constructor
    // 调用继承的构造函数
    Derived d(42);

// 7. enum class (scoped and strongly typed enums)
// 7. enum class（作用域和强类型枚举）
    // old enum in C - 传统C语言枚举
    enum Color_old { Red, Green, Blue };
    Color_old color_old = Red;
    // it can be implicitly converted to int, which can lead to bugs if not careful
    // 它可以隐式转换为int，如果不小心可能导致错误
    int temp = color_old;

    // new enum in C++ - C++的强类型枚举
    enum class Color { Red, Green, Blue };
    // need to explicitly convert to int
    // 需要显式转换
    Color color = Color::Red;
    // int x = c; // error, cannot implicitly convert
    temp = static_cast<int>(color);

// 8. Defaulted and deleted functions
// 8. 默认和删除函数
    class MyClass {
    public:
        // Explicitly request the compiler to generate default functions (C++11 feature)
        // 显式要求编译器生成默认函数（C++11特性）
        MyClass() = default;
        MyClass(const MyClass&) = default;
        MyClass& operator=(const MyClass&) = default;
        
        // Delete functions (C++11 feature)
        // 删除函数（C++11特性）
        MyClass(int) = delete;
        void func() = delete;
    };
    MyClass obj1;        // OK
    MyClass obj2(obj1);  // OK
    // MyClass obj3(42); // Error: constructor is deleted
    // obj1.func();      // Error: function is deleted

// 9. Variadic templates
// 9. 变长参数模板
    print(1, "hello", 3.14); // Output: 1 hello 3.14

// 10. std::atomic and atomic operations
// 10. std::atomic与原子操作
    std::atomic<int> counter{0};
    // atomic increment - 原子自增
    ++counter;
    // atomic decrement - 原子自减
    --counter;
    // Atomic operation functions
    // 原子操作函数
    // int old_value = counter.fetch_add(5); // atomic addition
    // bool success = counter.compare_exchange_strong(old_value, new_value); // atomic compare and swap

// 11. Regular expressions (std::regex)
// 11. 正则表达式（std::regex）
    std::string text = "Hello, my email is test@example.com";
    // Match email
    // 匹配邮箱
    std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::smatch match;
    if (std::regex_search(text, match, email_pattern)) {
        std::cout << "Found email: " << match[0] << std::endl;
    }
    // Replace
    // 替换
    std::string result = std::regex_replace(text, email_pattern, "[EMAIL]");

// 12. other C++11 features (not covered here): auto, decltype, lambda, constexpr, static_assert, move semantics, smart pointers, etc.
// 12. 其他C++11特性（这里不展开）：auto, decltype, lambda, constexpr, static_assert, move semantics, smart pointers等
    // ... (see other files for demos of these features)
    // ...（其他文件中有这些特性的示例）
    return 0;
}
