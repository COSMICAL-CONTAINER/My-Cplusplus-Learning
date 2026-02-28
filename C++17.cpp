#include <iostream>
#include <tuple>
#include <map>

// contents: C++17 features demo
// 1. Structured bindings
// 2. Compiler conditionals (if constexpr)
// 3. Parameter pack expansion / fold expressions
// 4. std::optional (optional values)
// 5. std::variant (type-safe union)
// 6. std::any (type-erased container)
// 7. std::string_view (non-owning string view)
// 8. std::filesystem (filesystem utilities)
// 9. Parallel algorithms (execution policies)
// 10. Inline variables (inline globals and template inline variables)

// 目录: C++17特性演示
// 1. 结构化绑定
// 2. 编译器条件判断(if constexpr)
// 3. 参数包展开 / 折叠表达式
// 4. std::optional(可能包含值)
// 5. std::variant(类型安全的联合体)
// 6. std::any(任意类型容器 / 类型擦除)
// 7. std::string_view(非拥有的字符串视图)
// 8. std::filesystem(文件系统工具)
// 9. STL 并行算法(execution 策略)
// 10. 内联变量(内联全局与模板内联变量)

// ======================begin======================
// 1. structured bindings
// 1. 结构化绑定
// Custom type Point
// 自定义类型 点
struct Point
{
    int x, y;
};

// add tuple interface to Point
// 为Point添加tuple接口
namespace std
{
    template <>
    struct tuple_size<Point> : integral_constant<size_t, 2>
    {
    };
    template <>
    struct tuple_element<0, Point>
    {
        using type = int;
    };
    template <>
    struct tuple_element<1, Point>
    {
        using type = int;
    };
}

template <size_t I>
int get(const Point &p)
{
    if constexpr (I == 0)
        return p.x;
    else if constexpr (I == 1)
        return p.y;
}
void structuredBindingsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Structured Bindings Demo ===" << std::endl;
    std::cout << "=== 结构化绑定演示 ===" << std::endl;

    // tuple decomposition
    // 元组解构
    auto [x, y, z] = std::make_tuple(1, "hello", 3.14);
    std::cout << x << ", " << y << ", " << z << std::endl;
    // array decomposition
    // 数组解构
    int arr[3] = {1, 2, 3};
    auto [a, b, c] = arr;
    // map decomposition
    // 哈希表解构
    std::map<std::string, int> m{{"one", 1}, {"two", 2}};
    for (const auto &[key, value] : m)
    {
        std::cout << key << ": " << value << std::endl;
    }

    // custom type decomposition (requires tuple_size and tuple_element specialization)
    // 自定义类型解构(需要tuple_size和tuple_element特化)
    Point p{10, 20};
    auto [px, py] = p;
}

// 2. compiler conditionals
// 2. 编译器条件判断
template <typename T>
auto get_value(T t)
{
    if constexpr (std::is_integral_v<T>)
    {
        return t * 2;
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        return t + 0.5;
    }
    else
    {
        return t;
    }
}
void compilerConditionalsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Compiler Conditionals Demo ===" << std::endl;
    std::cout << "=== 编译器条件判断演示 ===" << std::endl;

    int result1 = get_value(5);                            // 10
    double result2 = get_value(3.14);                      // 3.64
    std::string result3 = get_value(std::string("hello")); // "hello"
    std::cout << result1 << ", " << result2 << ", " << result3 << std::endl;
}

// 3. parameter pack expansion simplification
// 3. 参数包展开的简化
// resursive expansion (C++11)
// 递归展开(C++11)
template <typename T>
T sum(T t) { return t; }
template <typename T, typename... Args>
T sum(T t, Args... args)
{
    return t + sum(args...);
}
// fold expression (C++17)
// 折叠表达式(C++17)
template <typename... Args>
auto sum(Args... args)
{
    return (... + args);    // left fold - 左折叠
    // return (args + ...); // right fold - 右折叠
}
// other fold operations
// 其他折叠操作
template <typename... Args>
bool all_true(Args... args)
{
    return (... && args); // 逻辑与
}
template <typename... Args>
bool any_true(Args... args)
{
    return (... || args); // 逻辑或
}

void parameterPackExpansionSimplificationDemo()
{
    std::cout << std::endl;
    std::cout << "=== Parameter Pack Expansion Simplification Demo ===" << std::endl;
    std::cout << "=== 参数包展开的简化演示 ===" << std::endl;

    int result = sum(1, 2, 3, 4, 5); // 15
    std::cout << "Sum result: " << result << std::endl;
    bool all = all_true(true, true, false); // false
    bool any = any_true(true, false, false); // true
    std::cout << "All true: " << all << std::endl;
    std::cout << "Any true: " << any << std::endl;
}

// 4. std::optional
// 4. 可能包含值的容器
#include <optional>
std::optional<int> divide(int a, int b) 
{
    if (b == 0) {
        return std::nullopt; // no value - 无值
    }
    return a / b; // have value - 有值
}

void optionalDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::optional Demo ===" << std::endl;
    std::cout << "=== 可能包含值的容器演示 ===" << std::endl;

    auto result = divide(10, 2);
    if (result.has_value()) 
    {
        std::cout << "Result: " << result.value() << std::endl;
    } 
    else 
    {
        std::cout << "Division by zero!" << std::endl;
    }
    // use value_or to provide a default value
    // 使用value_or提供默认值
    int value = result.value_or(-1);
    // use operator* or operator. to access the value
    // 使用operator*或者operator.访问值
    if (result) 
    {
        std::cout << "Result using operator*: " << *result << std::endl;
        std::cout << "Result using operator.: " << result.value() << std::endl;
    }
}

// 5. std::variant
// 5. 类型安全的联合体
#include <variant>
#include <string>
void variantDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::variant Demo ===" << std::endl;
    std::cout << "=== 类型安全的联合体演示 ===" << std::endl;

    std::variant<int, double, std::string> v;
    v = 42;                    // save int
    v = 3.14;                  // save double
    v = std::string("hello");  // save string

    // access value - 访问值
    // must access the currently held type, otherwise std::bad_variant_access is thrown
    // 必须一一对应类型访问，否则会抛出std::bad_variant_access异常
    std::cout << std::get<std::string>(v) << std::endl; // direct access - 直接访问
    std::cout << std::get<2>(v) << std::endl;           // index accedss - 按索引访问

    // safe access - 安全访问
    try 
    {
        std::cout << std::get<double>(v) << std::endl;
    } 
    catch (const std::bad_variant_access& e) 
    {
        std::cout << "Wrong type!" << std::endl;
    }
    // 使用std::visit
    std::visit([](const auto& value) {
        std::cout << "Value: " << value << std::endl;
    }, v);

    v = 42; 

    // 检查当前类型
    if (std::holds_alternative<int>(v)) 
    {
        std::cout << "Contains int" << std::endl;
    }
}

// 6. std::any
// 6. 任意类型容器 类型擦除的容器
#include <any>
void anyDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::any Demo ===" << std::endl;
    std::cout << "=== 任意类型容器演示 ===" << std::endl;

    std::any a = 42;
    a = 3.14;
    a = std::string("hello");
    // access value - 访问值
    try
    {
        int value = std::any_cast<int>(a);
        std::cout << value << std::endl;
    }
    catch (const std::bad_any_cast& e)
    {
        // because a currently holds a string, trying to cast to int will throw an exception
        // 因为a当前持有一个字符串，尝试转换为int会抛出异常
        std::cout << "Wrong type!" << std::endl;
    }

    a = 42;

    // check type - 检查类型
    if (a.type() == typeid(int))
    {
        std::cout << "Contains int" << std::endl;
    }
    // use any_cast with pointer - 使用any_cast的指针版本
    if (int* ptr = std::any_cast<int>(&a))
    {
        std::cout << "Value: " << *ptr << std::endl;
    }
}

// 7. std::string_view
// 7. 字符串视图 非拥有字符串的轻量级视图
#include <string_view>

// function that takes a string_view parameter
// 函数参数优化
void process_string(std::string_view sv) 
{
    // do not need to copy the string, can directly use the view
    // 不需要拷贝，直接使用
    std::cout << sv << std::endl;
}

void stringViewDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::string_view Demo ===" << std::endl;
    std::cout << "=== 字符串视图演示 ===" << std::endl;

    std::string_view sv = "Hello, World!";

    // can use string_view without owning the string - no copy needed
    // 不拷贝，直接引用
    std::cout << sv.substr(0, 5) << std::endl; // "Hello"

    // interoperability with string
    // 与string的互操作
    std::string str = "Hello, World!";
    std::string_view view = str; // from string to create a view - 从string创建视图

    // can pass string_view to functions that take string_view, no need to convert
    // 可以接受多种字符串类型
    process_string("literal");
    process_string(str);
    process_string(sv);
}

// 8. std::filesystem
// 8. 文件系统库
#include <filesystem>

void filesystemDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::filesystem Demo ===" << std::endl;
    std::cout << "=== 文件系统库演示 ===" << std::endl;

    namespace fs = std::filesystem;
    
    // file path operations - 文件路径操作
    fs::path p = "path/to/file.txt";
    std::cout << p.filename() << std::endl;     // "file.txt"
    std::cout << p.extension() << std::endl;    // ".txt"
    std::cout << p.parent_path() << std::endl;  // "path/to"

    // this exe file path
    p = "";

    // file operations - 文件操作
    if (fs::exists(p))
    {
        std::cout << "File exists" << std::endl;
        std::cout << "Size: " << fs::file_size(p) << std::endl;
    }
    // directory operations - 目录操作
    for (const auto& entry : fs::directory_iterator("."))
    {
        // check if it's a regular file or a directory - 判断是文件还是目录
        if (entry.is_regular_file())
        {
            std::cout << "File: " << entry.path() << std::endl;
        }
        else if (entry.is_directory())
        {
            std::cout << "Dir: " << entry.path() << std::endl;
        }
    }
    // creat a directory and copy a file - 创建目录和复制文件
    fs::create_directories("path/to/new/dir");
    // fs::copy_file("source.txt", "dest.txt");
}

// 9. Parallel algorithms in STL
// 9. STL算法的并行版本
#include <execution>
#include <algorithm>
#include <vector>

void parallelAlgorithmsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Parallel Algorithms Demo ===" << std::endl;
    std::cout << "=== STL算法的并行版本演示 ===" << std::endl;

    std::vector<int> vec(1000000);
    std::iota(vec.begin(), vec.end(), 0);

    // parallel sort - 并行排序
    std::sort(std::execution::par, vec.begin(), vec.end());

    // parallel find - 并行查找
    auto it = std::find(std::execution::par, vec.begin(), vec.end(), 42);

    // parallel transform - 并行变换
    std::transform(std::execution::par, vec.begin(), vec.end(), vec.begin(),
                [](int x) { return x * 2; });
    // parallel reduce - 并行归约
    int sum = std::reduce(std::execution::par, vec.begin(), vec.end());
}


// 10. inline variables
// 10. 内联变量 
// template inline variable
// 模板内联变量
template<typename T>
inline T default_value = T{};
template<>
inline int default_value<int> = 0;
template<>
inline std::string default_value<std::string> = "default";

inline int global_value = 42;
inline const std::string global_string = "hello";
void inlineVariablesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Inline Variables Demo ===" << std::endl;
    std::cout << "=== 内联变量演示 ===" << std::endl;
    std::cout << "global_value: " << global_value << std::endl;
    std::cout << "global_string: " << global_string << std::endl;
    std::cout << "default_value<int>: " << default_value<int> << std::endl;
    std::cout << "default_value<std::string>: " << default_value<std::string> << std::endl;
}

int main()
{
    std::cout << "=== C++17 Features Demo ===" << std::endl;
    std::cout << "=== C++17 新特性演示 ===" << std::endl;

    structuredBindingsDemo();
    compilerConditionalsDemo();
    parameterPackExpansionSimplificationDemo();
    optionalDemo();
    variantDemo();
    anyDemo();
    stringViewDemo();
    filesystemDemo();
    parallelAlgorithmsDemo();
    inlineVariablesDemo();

    return 0;
}