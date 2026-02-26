#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <functional>
#include <utility>
using namespace std;

// ==================== function template examples ====================
// ==================== 函数模板示例 ====================

// 1. Basic function template
// 1. 基本函数模板
template <typename T>
T myMax(T a, T b)
{
    return (a > b) ? a : b;
}

// 2. Function template with multiple parameters
// 2. 多参数函数模板
template <typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b)
{
    return a + b;
}

// 3. Function template overloading
// 3. 函数模板重载
template <typename T>
void print(const T &value)
{
    cout << "usually version: " << value << endl;
}

template <typename T>
void print(const vector<T> &vec)
{
    cout << "vector version: ";
    for (const auto &elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl;
}

// 4. Function template specialization
// 4. 函数模板特化
template <>
void print(const string &str)
{
    cout << "string version: \"" << str << "\"" << endl;
}

// 5. Non-type template parameters
// 5. 非类型模板参数
template <typename T, size_t N>
T *myBegin(T (&array)[N])
{
    return array;
}
template <typename T, size_t N>
T *myEnd(T (&array)[N])
{
    return array + N;
}

// 6. Default template parameters
// 6. 默认模板参数
template <typename T = int>
T getDefault()
{
    return T{};
}

void functionTemplateDemo()
{
    // Basic function template
    // 基本函数模板
    cout << "myMax(3, 5) = " << myMax(3, 5) << endl;
    cout << "myMax(3.14, 2.71) = " << myMax(3.14, 2.71) << endl;
    cout << "myMax('a', 'b') = " << myMax('a', 'b') << endl;

    // Function template with multiple parameters
    // 多参数函数模板
    cout << "add(3, 4.5) = " << add(3, 4.5) << endl;
    cout << "add(3.14, 2) = " << add(3.14, 2) << endl;

    // Function template overloading
    // 函数模板重载
    print(42);
    print(3.14);
    print(string("Hello"));

    vector<int> vec = {1, 2, 3, 4, 5};
    print(vec);

    // Non-type template parameters
    // 非类型模板参数
    int arr[] = {1, 2, 3, 4, 5};
    cout << "array elements: ";
    for (auto it = myBegin(arr); it != myEnd(arr); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

    // Default template parameters
    // 默认模板参数
    cout << "default int: " << getDefault() << endl;
    cout << "default double: " << getDefault<double>() << endl;
}

// ==================== Class Template Examples ====================
// ==================== 类模板示例 ====================
// 1. Basic class template
// 1. 基本类模板
template <typename T>
class Stack
{
private:
    vector<T> data;

public:
    void push(const T &value)
    {
        data.push_back(value);
    }

    T pop()
    {
        if (data.empty())
        {
            throw runtime_error("Stack is empty");
        }
        T value = data.back();
        data.pop_back();
        return value;
    }

    bool empty() const
    {
        return data.empty();
    }

    size_t size() const
    {
        return data.size();
    }

    T &top()
    {
        if (data.empty())
        {
            throw runtime_error("Stack is empty");
        }
        return data.back();
    }
};

// 2. Multiple-parameter class template
// 2. 多参数类模板
template <typename T, typename U>
class Pair
{
private:
    T first;
    U second;

public:
    Pair(const T &f, const U &s) : first(f), second(s) {}

    T getFirst() const { return first; }
    U getSecond() const { return second; }

    void setFirst(const T &f) { first = f; }
    void setSecond(const U &s) { second = s; }

    void print() const
    {
        cout << "(" << first << ", " << second << ")" << endl;
    }
};

// 3. Class template specialization
// 3. 类模板特化
template <typename T>
class Container
{
public:
    void print(const T &value)
    {
        cout << "General Container: " << value << endl;
    }
};

// Full specialization
// 全特化
template <>
class Container<string>
{
public:
    void print(const string &value)
    {
        cout << "String specialization Container: \"" << value << "\"" << endl;
    }
};

// Partial specialization
// 偏特化
template <typename T>
class Container<T *>
{
public:
    void print(T *value)
    {
        cout << "Pointer specialization Container: ";
        if (value)
            cout << *value;
        else
            cout << "nullptr";
        cout << endl;
    }
};

// 4. Member function template
// 4. 成员函数模板
template <typename T>
class SmartPtr
{
private:
    T *ptr;

public:
    SmartPtr(T *p = nullptr) : ptr(p) {}

    ~SmartPtr()
    {
        delete ptr;
    }

    // Member function template
    // 成员函数模板
    template <typename U>
    SmartPtr(const SmartPtr<U> &other) : ptr(other.get()) {}

    template <typename U>
    SmartPtr &operator=(const SmartPtr<U> &other)
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.get();
        }
        return *this;
    }

    T *get() const { return ptr; }
    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }
};

// 5. Inheritance and templates
// 5. 继承和模板
template <typename T>
class Base
{
public:
    virtual void process(const T &value)
    {
        cout << "Base process: " << value << endl;
    }

    virtual ~Base() = default;
};
template <typename T>
class Derived : public Base<T>
{
public:
    void process(const T &value) override
    {
        cout << "Derived process: " << value << endl;
    }
};

void classTemplateDemo()
{
    cout << "=== Class Template Examples ===" << endl;
    cout << "=== 类模板示例 ===" << endl;

    // Basic class template
    // 基本类模板
    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    intStack.push(3);

    cout << "Stack's size: " << intStack.size() << endl;
    cout << "Stack's top: " << intStack.top() << endl;

    while (!intStack.empty())
    {
        cout << "pop: " << intStack.pop() << endl;
    }

    // multiple-parameter class template
    // 多参数类模板
    Pair<int, string> pair(1, "Hello");
    pair.print();

    Pair<double, char> pair2(3.14, 'A');
    pair2.print();

    // class template specialization
    // 类模板特化
    Container<int> c1;
    c1.print(42);

    Container<string> c2;
    c2.print("Hello");

    int x = 100;
    Container<int *> c3;
    c3.print(&x);

    // member function template
    // 成员函数模板
    SmartPtr<int> ptr1(new int(42));
    SmartPtr<double> ptr2(new double(3.14));

    // use member function template to convert between different SmartPtr types
    // 使用成员函数模板
    SmartPtr<int> ptr3 = ptr1;
    cout << "ptr3 value: " << *ptr3 << endl;

    // Inheritance and templates
    // 继承和模板
    Base<int> base;
    Derived<int> derived;

    base.process(42);
    derived.process(42);
}

// ==================== Template Specialization Examples ====================
// ==================== 模板特化示例 ====================
// 1. Function template specialization
// 1. 函数模板特化
template <typename T>
bool isEqual(const T &a, const T &b)
{
    return a == b;
}

// 1. all specialization for double, using an epsilon for comparison
// 1. 全特化
template <>
bool isEqual(const double &a, const double &b)
{
    const double epsilon = 1e-9;
    return abs(a - b) < epsilon;
}

// 2. Class template specialization
// 2. 类模板特化
template <typename T>
class TypeInfo
{
public:
    static string getName()
    {
        return "Unknown";
    }

    static size_t getSize()
    {
        return sizeof(T);
    }
};

// 3. Full specialization
// 3. 全特化
template <>
class TypeInfo<int>
{
public:
    static string getName()
    {
        return "int";
    }

    static size_t getSize()
    {
        return sizeof(int);
    }
};
template <>
class TypeInfo<string>
{
public:
    static string getName()
    {
        return "string";
    }

    static size_t getSize()
    {
        return sizeof(string);
    }
};

// 4. Partial specialization
// 4. 偏特化
template <typename T>
class Array
{
private:
    T *data;
    size_t size;

public:
    Array(size_t n) : size(n)
    {
        data = new T[n];
    }

    ~Array()
    {
        delete[] data;
    }

    T &operator[](size_t index)
    {
        return data[index];
    }

    void fill(const T &value)
    {
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = value;
        }
    }
};

// 5. Partial specialization for pointer types
// 5. 偏特化：指针类型
template <typename T>
class Array<T *>
{
private:
    T **data;
    size_t size;

public:
    Array(size_t n) : size(n)
    {
        data = new T *[n];
        for (size_t i = 0; i < n; ++i)
        {
            data[i] = nullptr;
        }
    }

    ~Array()
    {
        for (size_t i = 0; i < size; ++i)
        {
            delete data[i];
        }
        delete[] data;
    }

    T *&operator[](size_t index)
    {
        return data[index];
    }

    void fill(T *value)
    {
        for (size_t i = 0; i < size; ++i)
        {
            delete data[i];
            data[i] = value ? new T(*value) : nullptr;
        }
    }
};

void templateSpecializationDemo()
{
    cout << endl;
    cout << "=== Template Specialization Examples ===" << endl;
    cout << "=== 模板特化示例 ===" << endl;

    // function template specialization
    // 函数模板特化
    cout << "isEqual(1, 1): " << isEqual(1, 1) << endl;
    cout << "isEqual(1.0, 1.0): " << isEqual(1.0, 1.0) << endl;
    cout << "isEqual(1.0, 1.000000001): " << isEqual(1.0, 1.000000001) << endl;

    // class template specialization
    // 类模板特化
    cout << "TypeInfo<int>::getName(): " << TypeInfo<int>::getName() << endl;
    cout << "TypeInfo<int>::getSize(): " << TypeInfo<int>::getSize() << endl;
    cout << "TypeInfo<string>::getName(): " << TypeInfo<string>::getName()
         << endl;
    cout << "TypeInfo<string>::getSize(): " << TypeInfo<string>::getSize()
         << endl;

    // partial specialization
    // 偏特化
    Array<int> intArray(5);
    intArray.fill(42);
    cout << "intArray[0]: " << intArray[0] << endl;

    Array<int *> ptrArray(3);
    int value = 100;
    ptrArray.fill(&value);
    cout << "*ptrArray[0]: " << *ptrArray[0] << endl;
}

// ==================== Variadic Template Examples ====================
// ==================== 可变参数模板示例 ====================
// 1. Basic variadic template
// 1. 基本可变参数模板
template <typename... Args>
void printArgs(Args... args)
{
    cout << "the number of arguments is: " << sizeof...(args) << endl;
}

// 2. Recursive expansion
// 2. 递归展开
template <typename T>
void printRecursive(T t)
{
    cout << t << endl;
}
template <typename T, typename... Args>
void printRecursive(T t, Args... args)
{
    cout << t << " ";
    printRecursive(args...);
}

// 2. Recursive expansion with base case ( C++17 )
// 3. 折叠表达式 (C++17)
template <typename... Args>
auto sum(Args... args)
{
    return (... + args);
}
template <typename... Args>
void printFold(Args... args)
{
    (cout << ... << args) << endl;
}

// 4. Perfect forwarding
// 4. 完美转发
template <typename... Args>
class Tuple
{
private:
    tuple<Args...> data;

public:
    Tuple(Args &&...args) : data(forward<Args>(args)...) {}

    template <size_t I>
    auto get() -> decltype(get<I>(data))
    {
        return std::get<I>(data);
    }

    void print() const
    {
        printTuple(data, make_index_sequence<sizeof...(Args)>{});
    }

private:
    template <size_t... I>
    void printTuple(const tuple<Args...> &t, index_sequence<I...>) const
    {
        using expander = int[];
        (void)expander{0, ((cout << (I == 0 ? "" : ", ") << std::get<I>(t)), 0)...};
        cout << endl;
    }
};

// 5. Type traits
// 5. 类型特征
template <typename... Args>
struct TypeList
{
    static constexpr size_t size = sizeof...(Args);
};
template <typename T, typename... Args>
struct Contains
{
    static constexpr bool value = (is_same_v<T, Args> || ...);
};
void variadicTemplateDemo()
{
    cout << endl;
    cout << "=== Variadic Template Examples ===" << endl;
    cout << "=== 可变参数模板示例 ===" << endl;

    // Basic variadic template
    // 基本可变参数模板
    printArgs(1, 2, 3, "hello", 3.14);

    // Recursive expansion
    // 递归展开
    cout << "recurisive print: ";
    printRecursive(1, "hello", 3.14, 'A');

    // Fold expression
    // 折叠表达式
    cout << "sum: " << sum(1, 2, 3, 4, 5) << endl;
    cout << "fold print: ";
    printFold(1, " ", "hello", " ", 3.14);

    // Perfect forwarding
    // 完美转发
    Tuple<int, string, double> t(42, "Hello", 3.14);
    t.print();

    // Type traits
    // 类型特征
    cout << "TypeList size: " << TypeList<int, string, double>::size << endl;
    cout << "include int: " << Contains<int, string, double, int>::value << endl;
    cout << "include char: " << Contains<char, string, double, int>::value << endl;
}

// ==================== Template Metaprogramming Examples ====================
// ==================== 模板元编程示例 ====================
// 1. Compile-time computation
// 1. 编译期计算
template <size_t N>
struct Factorial
{
    static constexpr size_t value = N * Factorial<N - 1>::value;
};
template <>
struct Factorial<0>
{
    static constexpr size_t value = 1;
};
// 2. Type traits
// 2. 类型特征
template <typename T>
struct IsPointer
{
    static constexpr bool value = false;
};
template <typename T>
struct IsPointer<T *>
{
    static constexpr bool value = true;
};

// 3. Conditional types
// 3. 条件类型
template <bool Condition, typename T, typename F>
struct Conditional
{
    using type = T;
};
template <typename T, typename F>
struct Conditional<false, T, F>
{
    using type = F;
};

// 4. SFINAE (Substitution Failure Is Not An Error)
// Use detection idiom to avoid ambiguous overloads
// 4. SFINAE (替换失败不是错误)
// 使用检测惯用法避免歧义重载
template <typename, typename = void>
struct has_size_trait : std::false_type {};

template <typename T>
struct has_size_trait<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template <typename, typename = void>
struct has_length_trait : std::false_type {};

template <typename T>
struct has_length_trait<T, std::void_t<decltype(std::declval<T>().length())>> : std::true_type {};

template <typename T>
void hasSize(const T &t)
{
    if constexpr (has_size_trait<T>::value)
    {
        cout << "with size() method" << endl;
    }
    else if constexpr (has_length_trait<T>::value)
    {
        cout << "with length() method" << endl;
    }
    else
    {
        cout << "use sizeof" << endl;
    }
}
void templateMetaprogrammingDemo()
{
    cout << endl;
    cout << "=== Template Metaprogramming Examples ===" << endl;
    cout << "=== 模板元编程示例 ===" << endl;

    // Compile-time computation
    // 编译期计算
    cout << "5!: " << Factorial<5>::value << endl;
    cout << "10!: " << Factorial<10>::value << endl;

    // Type traits
    // 类型特征
    cout << "IsPointer<int>::value: " << IsPointer<int>::value << endl;
    cout << "IsPointer<int*>::value: " << IsPointer<int *>::value << endl;

    // Conditional types
    // 条件类型
    using IntOrDouble = Conditional<true, int, double>::type;
    cout << "Conditional types: " << sizeof(IntOrDouble) << endl;

    // SFINAE
    vector<int> vec = {1, 2, 3};
    string str = "hello";
    int x = 42;

    // call the hasSize function with different types to demonstrate SFINAE
    // 调用第一个重载
    hasSize(vec); 
    // call the hasSize function with a string to demonstrate SFINAE
    // 调用第二个重载
    hasSize(str);
    // call the hasSize function with an int to demonstrate SFINAE
    // 调用第三个重载
    hasSize(x);
}
// ==================== Concepts and Constraints (C++20) ====================
// ==================== 概念和约束 (C++20) ====================
#ifdef __cpp_concepts
// Concept definition
// 概念定义
template <typename T>
concept Integral = is_integral_v<T>;
template <typename T>
concept FloatingPoint = is_floating_point_v<T>;
template <typename T>
concept Arithmetic = Integral<T> || FloatingPoint<T>;
// use concepts to constrain template parameters
// 使用概念的函数模板
template <Arithmetic T>
T add(T a, T b)
{
    return a + b;
}
// use concepts to constrain class templates
// 约束的类模板
template <Arithmetic T>
class Calculator
{
public:
    T add(T a, T b) { return a + b; }
    T subtract(T a, T b) { return a - b; }
    T multiply(T a, T b) { return a * b; }
    T divide(T a, T b) { return a / b; }
};
void conceptsDemo()
{
    cout << endl;
    cout << "=== Concepts and Constraints (C++20) ===" << endl;
    cout << "=== 概念和约束示例 (C++20) ===" << endl;

    Calculator<int> intCalc;
    cout << "int计算: " << intCalc.add(5, 3) << endl;

    Calculator<double> doubleCalc;
    cout << "double计算: " << doubleCalc.multiply(2.5, 3.0) << endl;
}
#endif
int main()
{
    functionTemplateDemo();
    classTemplateDemo();
    templateSpecializationDemo();
    variadicTemplateDemo();
    templateMetaprogrammingDemo();

#ifdef __cpp_concepts
    conceptsDemo();
#endif

    return 0;
}