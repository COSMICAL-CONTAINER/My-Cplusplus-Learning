#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>
#include <tuple>
#include <string>
#include <vector>
#include <limits>

// contents: C++14 features demo
// 1. Binary literals & digit separators
// 2. Generic lambdas (auto parameters)
// 3. Variable templates
// 4. Return type deduction for functions
// 5. std::make_unique convenience
// 6. std::index_sequence and tuple printing
// 7. Relaxed constexpr (loops allowed in constexpr)
// 8. std::exchange utility
// 9. Aggregate initialization (derived aggregates compatibility)
// 10. Attributes: [[deprecated]], [[nodiscard]], [[maybe_unused]]
// 11. Variable template specialization
// 目录: C++14特性演示
// 1. 二进制字面量与数字分隔符
// 2. 通用 lambda（参数使用 auto）
// 3. 变量模板
// 4. 函数返回类型推导（auto 返回类型）
// 5. std::make_unique（便捷工厂函数）
// 6. std::index_sequence 与元组打印
// 7. 更宽松的 constexpr（constexpr 中允许循环）
// 8. std::exchange 工具函数
// 9. 聚合初始化（含基类的兼容写法）
// 10. 属性：[[deprecated]]、[[nodiscard]]、[[maybe_unused]]
// 11. 变量模板特化

// ======================begin======================
// 1. Binary literals & digit separators
// 1. 二进制字面量与数字分隔符
void binaryLiteralDemo()
{
    std::cout << std::endl;
    std::cout << "=== Binary literals & digit separators ===" << std::endl;
    std::cout << "=== 二进制字面量与数字分隔符 ===" << std::endl;

    int binary1 = 0b1010;        // 10
    int large_binary = 0b1111'0000'1010'0101;
    int million = 1'000'000;
    double pi = 3.141'592'653'59;
    int hex = 0xFF'FF'FF'FF;
    std::cout << "binary1=" << binary1 << ", large=" << large_binary << ", million=" << million << "\n";
}

// 2. Generic lambdas (auto parameters)
// 2. 通用 lambda(参数使用 auto)
void genericLambdaDemo()
{
    std::cout << std::endl;
    std::cout << "=== Generic lambdas ===" << std::endl;
    std::cout << "=== 通用 lambda ===" << std::endl;

    auto lambda = [](auto x) { return x * 2; };
    auto gl = [](auto a, auto b) { return a + b; };
    std::cout << "generic lambda int: " << gl(2,3) << ", string: " << gl(std::string("a"), std::string("b")) << std::endl;
}

// 3. Variable template example
// 3. 变量模板示例
template<typename T>
constexpr T pi_v = T(3.14159265358979323846);

void variableTemplateDemo()
{
    std::cout << std::endl;
    std::cout << "=== Variable templates ===" << std::endl;
    std::cout << "=== 变量模板 ===" << std::endl;

    float f = pi_v<float>;
    double dval = pi_v<double>;
    std::cout << "pi_v<double> = " << pi_v<double> << std::endl;
}

// 4. Return type deduction for functions
// 4. 函数返回类型推导(auto 返回类型)
void returnDeductionDemo()
{
    std::cout << std::endl;
    std::cout << "=== Return type deduction ===" << std::endl;
    std::cout << "=== 返回类型推导 ===" << std::endl;

    auto make_pair_auto = [](int x, int y) { return std::make_pair(x, y); };
    auto p = make_pair_auto(3,4);
    std::cout << "make_pair_auto: (" << p.first << ", " << p.second << ")\n";
}

// 5. std::make_unique convenience
// 5. std::make_unique(便捷工厂函数)
void makeUniqueDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::make_unique ===" << std::endl;
    std::cout << "=== make_unique 示例 ===" << std::endl;

    std::unique_ptr<int> ptr1(new int(42));
    auto ptr2 = std::make_unique<int>(42);
    auto ptr3 = std::make_unique<std::vector<int>>(10, 5);
    auto arr_ptr = std::make_unique<int[]>(10);
    std::cout << "make_unique -> " << *ptr1 << std::endl;
}

// 6. std::index_sequence and tuple printing
// 6. std::index_sequence 与元组打印
// Print tuple helper using index_sequence and pack expansion (C++14)
// 打印元组的辅助函数，使用 index_sequence 和参数包展开(C++14)
template<typename Tuple, size_t... I>
void print_tuple_impl(const Tuple &t, std::index_sequence<I...>)
{
    // Use initializer list trick to expand the pack and evaluate expressions in order
    // 使用初始化列表技巧展开参数包，并按顺序计算表达式
    using expander = int[];
    (void)expander{0, ( (std::cout << std::get<I>(t) << (I + 1 < sizeof...(I) ? ", " : "")), 0)... };
    std::cout << '\n';
}
template<typename... Args>
void print_tuple(const std::tuple<Args...> &t)
{
    print_tuple_impl(t, std::index_sequence_for<Args...>{});
}
void indexSequenceDemo()
{
    std::cout << std::endl;
    std::cout << "=== index_sequence / tuple printing ===" << std::endl;
    std::cout << "=== index_sequence / 元组打印 ===" << std::endl;

    auto t = std::make_tuple(1, std::string("hello"), 3.14);
    std::cout << "tuple: ";
    print_tuple(t);
}

// 7. Relaxed constexpr (C++14 allows loops in constexpr)
// 7. 更宽松的 constexpr(C++14 在 constexpr 中允许循环)
// Constexpr factorial using a loop (allowed in C++14)
// Constexpr 阶乘函数，使用循环实现(C++14 允许在 constexpr 中使用循环)
constexpr int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i)
        result *= i;
    return result;
}

int add(int a, int b) { return a + b; }
void relaxedConstexprDemo()
{
    std::cout << std::endl;
    std::cout << "=== Relaxed constexpr ===" << std::endl;
    std::cout << "=== 更宽松的 constexpr ===" << std::endl;

    static_assert(factorial(5) == 120, "factorial(5) should be 120");
    std::cout << "factorial(6) = " << factorial(6) << std::endl;
}

// 8. std::exchange utility
// 8. std::exchange 用法(返回旧值并赋新值)
void exchangeDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::exchange ===" << std::endl;
    std::cout << "=== std::exchange 示例 ===" << std::endl;

    int val = 10;
    int old = std::exchange(val, 20);
    std::cout << "exchange old= " << old << ", new= " << val << std::endl;
}

// 9. Aggregate initialization (derived aggregates compatibility)
// 9. 聚合初始化(含基类的兼容写法)
void aggregateInitDemo()
{
    std::cout << std::endl;
    std::cout << "=== Aggregate initialization ===" << std::endl;
    std::cout << "=== 聚合初始化示例 ===" << std::endl;

    struct Base { int x; };
    struct Derived : Base { int y; };
    Derived der; der.x = 1; der.y = 2;
    std::cout << "Derived.x=" << der.x << ", y=" << der.y << std::endl;
}

// 10. Attributes - [[deprecated]] example
// 10. 属性 - [[deprecated]] 示例
// [[deprecated]] - 弃用警告
[[deprecated("Use new_function instead")]]
void old_function() {}

void attributesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Attributes ([[deprecated]]) ===" << std::endl;
    std::cout << "=== 属性示例 ===" << std::endl;

    enum [[deprecated("Use new_enum instead")]] old_enum { value };
    old_function(); // should warn
    (void)value;    // silence unused warning
}

// 11. Variable template specialization
// 11. 变量模板特化示例
template<typename T>
constexpr T max_value = std::numeric_limits<T>::max();

template<>
constexpr int max_value<int> = 1000; // specialize for integer type
void specializationDemo()
{
    std::cout << std::endl;
    std::cout << "=== Variable template specialization ===" << std::endl;
    std::cout << "=== 变量模板特化 ===" << std::endl;
    
    std::cout << "max_value<int> = " << max_value<int> << std::endl;
}

int main()
{
    std::cout << "=== C++14 Features Demo ===" << std::endl;
    std::cout << "=== C++14 新特性演示 ===" << std::endl;

    binaryLiteralDemo();
    genericLambdaDemo();
    returnDeductionDemo();
    variableTemplateDemo();
    makeUniqueDemo();
    indexSequenceDemo();
    relaxedConstexprDemo();
    exchangeDemo();
    aggregateInitDemo();
    attributesDemo();
    specializationDemo();

    return 0;
}
