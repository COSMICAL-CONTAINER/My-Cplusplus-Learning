#include <iostream>
#include <vector>
#include <unordered_set>

// remember to add -std=c++2a to compile
// 编译时添加 -std=c++2a

// contents: C++20 features demo
// 1. Compile-time type constraints
// 2. Coroutines
// 3. Three-way comparison (spaceship operator)
// 4. Ranges and views
// 5. Modules (with fallback)
// 6. Constexpr expansion
// 7. Designated initializers
// 8. constinit
// 9. std::format
// 10. std::chrono (calendar/timezone)
// 目录： C++20特性演示
// 1. 编译期类型约束
// 2. 协程
// 3. 三路比较(太空船运算符)
// 4. 范围与视图
// 5. 模块(带回退实现)
// 6. constexpr 扩展
// 7. 指定初始化
// 8. constinit
// 9. std::format 字符串格式化
// 10. std::chrono(日期/时区)

// ======================begin======================
// 1. Compile-time type constraints
// 1. 编译期类型约束
#include <concepts>

// define concepts - 定义概念
template <typename T>
concept Integral = std::is_integral_v<T>;
template <typename T>
concept Addable = requires(T a, T b) 
{
    { a + b } -> std::convertible_to<T>;
};
// use concepts - 使用概念
template <Integral T>
T multiply(T a, T b)
{
    return a * b;
}
template <Addable T>
T add(T a, T b)
{
    return a + b;
}
// concepts mix - 概念组合
template <typename T>
concept Numeric = Integral<T> || std::floating_point<T>;
// requires表达式
template <typename T>
requires requires(T t) { t.size(); }
void process_container(T &container)
{
    std::cout << "Size: " << container.size() << std::endl;
}

void compiletimetypeconstraintsDemo()
{
    std::cout << std::endl;
    std::cout << "=== Compile-time type constraints demo ===" << std::endl;
    std::cout << "=== 编译期类型约束演示 ===" << std::endl;

    std::cout << multiply(2, 3) << std::endl;
    std::cout << add(2.5, 3.5) << std::endl;
    std::vector<int> vec{1, 2, 3};
    process_container(vec); // Size: 3
}

// 2. Coroutines
// 2. 协程 异步编程支持:
#include <coroutine>
#include <future>

// simple coroutine framework
// 简单的协程框架
struct Task {
    struct promise_type 
    {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
// coroutine function
// 协程函数
Task coroutine_function() 
{
    std::cout << "Coroutine started" << std::endl;
    co_await std::suspend_never{};
    std::cout << "Coroutine resumed" << std::endl;
}

void coroutineDemo() 
{
    std::cout << std::endl;
    std::cout << "=== Coroutine Demo ===" << std::endl;
    std::cout << "=== 协程示例 ===" << std::endl;

    auto task = coroutine_function();
}

// 3. three-way comparison operator / spaceship operator (<=>)
// 3. 三路比较运算符/太空船操作符(<=>)
#include <compare>

class Point 
{
public:
    int x, y;

    // auto generate all comparison operators
    // 自动生成所有比较操作符
    auto operator<=>(const Point& other) const = default;
    
    // or manually implement
    // 或者自定义实现
    /*
    std::strong_ordering operator<=>(const Point& other) const 
    {
        if (auto cmp = x <=> other.x; cmp != 0) 
            return cmp;
        return y <=> other.y;
    }
    */
};

void spaceshipoperatorDemo()
{
    std::cout << std::endl;
    std::cout << "=== Spaceship operator Demo ===" << std::endl;
    std::cout << "=== 三路比较运算符/太空船操作符(<=>) ===" << std::endl;

    Point p1{1, 2}, p2{1, 3};
    // auto generate all comparison operators means we can use <, ==, > etc. without explicitly defining them
    // 自动支持所有比较
    bool less = p1 < p2;      // true
    bool equal = p1 == p2;    // false
    bool greater = p1 > p2;   // false
    std::cout << "p1 < p2: " << less << std::endl;
    std::cout << "p1 == p2: " << equal << std::endl;
    std::cout << "p1 > p2: " << greater << std::endl;
}

// 4. Ranges 
// 范围 - 现代迭代器库:
#include <ranges>

void rangesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Ranges Demo ===" << std::endl;
    std::cout << "=== 范围示例 ===" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // 管道操作
    auto result = vec | std::views::filter([](int x) { return x % 2 == 0; })
                    | std::views::transform([](int x) { return x * x; });
    // 转换为容器
    std::vector<int> squares;
    for (int x : result) {
        squares.push_back(x);
    }
    // 或者使用ranges::to(C++23)
    // auto squares = result | std::ranges::to<std::vector>();
    // 更多视图
    auto first_5 = vec | std::views::take(5);
    auto reversed = vec | std::views::reverse;
    std::unordered_set<int> seen;
    auto unique = vec | std::views::filter([&seen](int v){ return seen.insert(v).second; });

    std::cout << "Even squares: ";
    for (int x : squares) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "First 5 elements: ";
    for (int x : first_5) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Reversed elements: ";
    for (int x : reversed) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Unique elements: ";
    for (int x : unique) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

// 5. Modules
// 5. 模块 - 现代C++模块系统：
// modules support is compiler-dependent; provide a fallback when modules are unavailable
// 模块支持依赖于编译器；当模块不可用时提供回退方案
#if defined(__cpp_modules)
module;
#include <iostream>

export module math;
export namespace math {
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
}
import math;
int modulesDemo()
{
    int result = math::add(5, 3);
    std::cout << result << std::endl;
    return 0;
}
#else
namespace math 
{
    inline int add(int a, int b) { return a + b; }
    inline int multiply(int a, int b) { return a * b; }
}
int modulesDemo()
{
    int result = math::add(5, 3);
    std::cout << result << std::endl;
    return 0;
}
#endif

// 6. constexpr expanded 
// 6. constexpr 允许更多在编译时计算的代码
// constexpr functions with dynamic allocation (C++20)
// constexpr函数中的动态分配(C++20)
constexpr int factorial(int n) 
{
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
// constexpr vector(C++20)
// Note: many standard library types (std::vector/std::string) are not
// guaranteed to be usable in a constexpr context across standard library
// implementations. Use non-constexpr helpers here for portability.
std::vector<int> create_vector() 
{
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    return v;
}
// constexpr string(C++20)
std::string create_string() 
{
    std::string s = "hello";
    s += " world";
    return s;
}

void constexprExpandedDemo() 
{
    std::cout << std::endl;
    std::cout << "=== constexpr Expanded Demo ===" << std::endl;
    std::cout << "=== constexpr 扩展示例 ===" << std::endl;

    static_assert(factorial(5) == 120, "factorial(5) should be 120");
    // create_vector/create_string are not constexpr-portable; use normal auto
    auto vec = create_vector();
    auto str = create_string();
    std::cout << "factorial(6) = " << factorial(6) << std::endl;
    std::cout << "constexpr vector size: " << vec.size() << std::endl;
    std::cout << "constexpr string: " << str << std::endl;
}

// 7. Designated Initializers
// 7. 指定初始化 结构体成员初始化时可以指定成员名称，增强可读性和灵活性

void designatedInitDemo() 
{
    std::cout << std::endl;
    std::cout << "=== Designated Initializers Demo ===" << std::endl;
    std::cout << "=== 指定初始化示例 ===" << std::endl;

    struct Point 
    {
        int x;
        int y;
        int z;
    };
    // designated initialization - 指定成员初始化
    Point p{.x = 1, .y = 2, .z = 3};
    // partial designated initialization - 部分指定成员初始化
    Point p2{.x = 1, .z = 3}; // y is default 0 - y默认为0
    std::cout << "Point p: x=" << p.x << ", y=" << p.y << ", z=" << p.z << std::endl;
    std::cout << "Point p2: x=" << p2.x << ", y=" << p2.y << ", z=" << p2.z << std::endl;
}

// 8. constinit
// 8. 编译期初始化保证
int get_runtime_value() { return 100; }
constinit int global_value = 42; // 编译期初始化
constexpr int get_compile_time_value() { return 100; }

void constinitDemo() 
{
    std::cout << std::endl;
    std::cout << "=== constinit Demo ===" << std::endl;
    std::cout << "=== constinit 示例 ===" << std::endl;

    // 错误：运行时初始化
    // constinit int runtime_value = get_runtime_value();

    // 正确用法：constinit 只能用于静态或线程存储期的变量
    static constinit int compile_time_value = get_compile_time_value();
    
    std::cout << "global_value: " << global_value << std::endl;
    std::cout << "compile_time_value: " << compile_time_value << std::endl;
}

// 9. std::format
// 9. 现在字符串格式化
#include <format>

void formatDemo() 
{
    std::cout << std::endl;
    std::cout << "=== std::format Demo ===" << std::endl;
    std::cout << "=== std::format 示例 ===" << std::endl;

    std::string message = std::format("Hello, {}!", "World");
    std::cout << message << std::endl; // "Hello, World!"

    // position-based formatting - 位置参数
    std::string result = std::format("{1} {0} {1}", "World", "Hello");
    std::cout << result << std::endl; // "Hello World Hello"

    // format specifiers - 格式说明符
    double pi = 3.14159;
    std::string formatted = std::format("Pi: {:.2f}", pi);// "Pi: 3.14"
    std::cout << formatted << std::endl;

    // alignment and padding - 对齐和填充
    // left alignment - 左对齐
    std::string left_aligned = std::format("{:<20}", "left"); // "left                "
    // center alignment - 居中对齐
    std::string centered = std::format("{:^20}", "center");   // "     center     "
    // right alignment - 右对齐
    std::string aligned = std::format("{:>10}", "right");     // "     right"
    std::cout << "Left aligned: '" << left_aligned << "'" << std::endl;
    std::cout << "Centered: '" << centered << "'" << std::endl;
    std::cout << "Right aligned: '" << aligned << "'" << std::endl;

    // color and style (if supported) - 颜色和样式(如果支持)
    std::string colored = std::format("\033[32m{}\033[0m", "green text");
    std::cout << colored << std::endl;
}

// 10. std::chrono::day
// 10. 日历和时区支持(C++20)
#include <chrono>
void chronoDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::chrono Demo ===" << std::endl;
    std::cout << "=== 日历和时区支持示例 ===" << std::endl;

    // data - 日期
    auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    std::cout << std::format("{}/{}/{}", 
        int(today.year()), 
        unsigned(today.month()), 
        unsigned(today.day())) << std::endl;

    // time - 时间点
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&time_t) << std::endl;
}

int main()
{
    std::cout << "=== C++20 Features Demo ===" << std::endl;
    std::cout << "=== C++20 新特性演示 ===" << std::endl;

    compiletimetypeconstraintsDemo();
    coroutineDemo();
    spaceshipoperatorDemo();
    rangesDemo();
    modulesDemo();
    constexprExpandedDemo();
    designatedInitDemo();
    constinitDemo();
    formatDemo();
    chronoDemo();
    
    return 0;
}