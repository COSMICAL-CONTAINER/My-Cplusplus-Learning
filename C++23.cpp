#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <queue>
#include <ranges>
#include <charconv>
#include <system_error>
#include <string_view>

#if defined(__has_include)
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

#if defined(DEMO_TRIGGER_WARNING)
#warning "C++23 demo: #warning is active because DEMO_TRIGGER_WARNING is defined"
#endif

#if defined(_MSC_VER)
#define PREPROCESSOR_BRANCH_TEXT "MSVC: #elifdef/#elifndef not supported yet"
#else
#define PREPROC_DEMO_VALUE 3
#if PREPROC_DEMO_VALUE > 10
#define PREPROCESSOR_BRANCH_TEXT "#if branch"
#elifdef __cpp_if_consteval
#define PREPROCESSOR_BRANCH_TEXT "#elifdef branch matched (__cpp_if_consteval)"
#elifndef __cpp_not_existing_demo_macro
#define PREPROCESSOR_BRANCH_TEXT "#elifndef branch matched"
#else
#define PREPROCESSOR_BRANCH_TEXT "#else branch"
#endif
#endif

#define DEMO_HAS_PRINT 0
#define DEMO_HAS_FORMAT 0
#define DEMO_HAS_EXPECTED 0
#define DEMO_HAS_GENERATOR 0
#define DEMO_HAS_MDSPAN 0
#define DEMO_HAS_STACKTRACE 0
#define DEMO_HAS_FLAT_MAP 0
#define DEMO_HAS_FLAT_SET 0

#if defined(__has_include)
#if __has_include(<print>)
#undef DEMO_HAS_PRINT
#define DEMO_HAS_PRINT 1
#include <print>
#endif
#if __has_include(<format>)
#undef DEMO_HAS_FORMAT
#define DEMO_HAS_FORMAT 1
#include <format>
#endif
#if __has_include(<expected>)
#undef DEMO_HAS_EXPECTED
#define DEMO_HAS_EXPECTED 1
#include <expected>
#endif
#if __has_include(<generator>)
#undef DEMO_HAS_GENERATOR
#define DEMO_HAS_GENERATOR 1
#include <generator>
#endif
#if __has_include(<mdspan>)
#undef DEMO_HAS_MDSPAN
#define DEMO_HAS_MDSPAN 1
#include <mdspan>
#endif
#if __has_include(<stacktrace>)
#undef DEMO_HAS_STACKTRACE
#define DEMO_HAS_STACKTRACE 1
#include <stacktrace>
#endif
#if __has_include(<flat_map>)
#undef DEMO_HAS_FLAT_MAP
#define DEMO_HAS_FLAT_MAP 1
#include <flat_map>
#endif
#if __has_include(<flat_set>)
#undef DEMO_HAS_FLAT_SET
#define DEMO_HAS_FLAT_SET 1
#include <flat_set>
#endif
#endif
#endif

// C++23 学习示例

// 1. Print with index helper
// 1. 带索引打印容器元素（学习辅助函数）
template <typename Container>
void printWithIndexDemo(const Container &c)
{
    std::cout << std::endl;
    std::cout << "=== Print With Index Demo ===\n";
    std::cout << "=== 带索引打印示例 ===\n";
    size_t idx = 0;
    for (const auto &x : c)
    {
        std::cout << "[" << idx++ << "] " << x << std::endl;
    }
}

// Local classes cannot declare member templates on some compilers,
// so keep this demo type at namespace scope.
struct DeducingThisBuffer
{
    using value_type = int;
    std::array<value_type, 4> arr{10, 20, 30, 40};

    template <typename Self>
    decltype(auto) at(this Self&& self, std::size_t idx)
    {
        return self.arr[idx];
    }
};

template <typename T>
struct InheritedCtadBase
{
    T f;
    explicit InheritedCtadBase(T func) : f(std::move(func)) {}
    void call() const { f(); }
};

template <typename T>
struct InheritedCtadDerived : InheritedCtadBase<T>
{
    using InheritedCtadBase<T>::InheritedCtadBase;
};

// 2. Deducing This demo
// 2. 显式 this 参数示例（C++23）
void deducingThisDemo()
{
    std::cout << std::endl;
    std::cout << "=== Deducing This Demo ===\n";
    std::cout << "=== 显式this参数示例 ===\n";

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    std::cout << "feature test macro: __cpp_explicit_this_parameter = "
              << __cpp_explicit_this_parameter << "\n";

    // 1) Recursive lambda: no helper std::function needed.
    auto fact = [](this auto self, int n) {
        if (n <= 1) return 1;
        return n * self(n - 1);
    };
    std::cout << "1) factorial(5) = " << fact(5) << "\n";

    // 2) One member function handles both mutable and const objects.
    DeducingThisBuffer buf;
    const DeducingThisBuffer& cbuf = buf;
    buf.at(1) = 99;
    std::cout << "2) buf.at(1) after write = " << buf.at(1)
              << ", cbuf.at(1) = " << cbuf.at(1) << "\n";

    // 3) By-value explicit object parameter for small view-like types.
    struct TinyView
    {
        const char* data{};
        std::size_t size{};

        const char* begin(this TinyView self)
        {
            return self.data;
        }

        const char* end(this TinyView self)
        {
            return self.data + self.size;
        }
    };

    TinyView view{"C++23", 5};
    std::cout << "3) TinyView by-value traversal: ";
    for (const char* p = view.begin(); p != view.end(); ++p)
    {
        std::cout << *p;
    }
    std::cout << "\n";

    // 4) explicit object parameter can model lvalue/rvalue behavior directly.
    struct QualifierDemo
    {
        void call(this QualifierDemo&)
        {
            std::cout << "4) called on lvalue object\n";
        }

        void call(this QualifierDemo&&)
        {
            std::cout << "4) called on rvalue object\n";
        }
    };

    QualifierDemo q;
    q.call();
    QualifierDemo{}.call();
#else
    std::cout << "Compiler does not support Deducing This yet.\n";
    std::cout << "Need: __cpp_explicit_this_parameter >= 202110L\n";
    std::cout << "Try newer GCC/Clang/MSVC with C++23 mode.\n";
#endif
}

// 3. Multidimensional subscript demo
// 3. 多元 operator[]（C++23）
void multidimensionalSubscriptDemo()
{
    
    std::cout << std::endl;
    std::cout << "=== Multidimensional Subscript Demo ===\n";
    std::cout << "=== 多元下标运算符示例 ===\n";

    struct Matrix
    {
        using value_type = int;
        std::array<value_type, 16> data{};

        value_type& operator[](int row, int col) { return data[row * 4 + col]; }
    };

    Matrix m;
    m[2, 3] = 4;
    std::cout << "m[2, 3] = " << m[2, 3] << std::endl;
}

// 4. size_t literal suffix demo
// 4. size_t 字面量后缀示例（C++23）
void sizeTSuffixDemo()
{
    std::cout << std::endl;
    std::cout << "=== size_t Literal Suffix Demo ===\n";
    std::cout << "=== size_t字面量后缀示例 ===\n";

#if defined(__cpp_size_t_suffix) && __cpp_size_t_suffix >= 202011L
    std::cout << "feature test macro: __cpp_size_t_suffix = "
              << __cpp_size_t_suffix << "\n";

    std::vector<int> nums{1, 2, 3, 4, 5};
    int sum = 0;
    for (auto i = 0uz; i < nums.size(); ++i)
    {
        sum += nums[i];
    }

    auto signed_size = 0z;
    std::cout << "sum with 0uz loop index = " << sum << "\n";
    std::cout << "0z is signed? " << std::boolalpha
              << std::is_signed_v<decltype(signed_size)> << "\n";
#else
    std::cout << "Compiler does not support size_t suffix yet.\n";
    std::cout << "Need: __cpp_size_t_suffix >= 202011L\n";
#endif
}

// 5. Extended floating-point types demo
// 5. 扩展浮点类型示例（C++23）
void extendedFloatingPointDemo()
{
    std::cout << std::endl;
    std::cout << "=== Extended Floating-point Demo ===\n";
    std::cout << "=== 扩展浮点类型示例 ===\n";

#ifdef __STDCPP_FLOAT16_T__
    std::cout << "__STDCPP_FLOAT16_T__ is available.\n";
    std::float16_t f16 = static_cast<std::float16_t>(1.5f);
    float back_to_float = static_cast<float>(f16);
    std::cout << "float16_t sample (cast to float): " << back_to_float << "\n";
#else
    std::cout << "float16_t is not available on this compiler/target.\n";
#endif

#ifdef __STDCPP_BFLOAT16_T__
    std::cout << "__STDCPP_BFLOAT16_T__ is available.\n";
#else
    std::cout << "bfloat16_t is not available on this compiler/target.\n";
#endif

#ifdef __STDCPP_FLOAT128_T__
    std::cout << "__STDCPP_FLOAT128_T__ is available.\n";
#else
    std::cout << "float128_t is not available on this compiler/target.\n";
#endif
}

// 6. Portable assumptions demo
// 6. 假定表达式示例（C++23）
void assumeAttributeDemo()
{
    std::cout << std::endl;
    std::cout << "=== Assume Attribute Demo ===\n";
    std::cout << "=== 假定表达式示例 ===\n";

#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(assume) >= 202207L
    std::array<int, 8> data{1, 2, 3, 4, 5, 6, 7, 8};
    [[assume(data.size() % 4 == 0)]];

    int sum = 0;
    for (int x : data)
    {
        sum += x;
    }
    std::cout << "assume attribute is supported; sum = " << sum << "\n";
#else
    std::cout << "[[assume(expr)]] not supported by this compiler yet.\n";
#endif
#else
    std::cout << "Cannot query attributes on this compiler.\n";
#endif
}

// 7. Static operator demo
// 7. 静态 operator() / operator[] 示例（C++23）
void staticOperatorDemo()
{
    std::cout << std::endl;
    std::cout << "=== Static Operator Demo ===\n";
    std::cout << "=== 静态operator示例 ===\n";

#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    struct Less
    {
        static constexpr bool operator()(int a, int b)
        {
            return a < b;
        }
    };
    std::cout << "static operator() : Less{}(2, 3) = "
              << Less{}(2, 3) << "\n";
#else
    std::cout << "static operator() is not supported.\n";
#endif

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202211L
    struct Plus
    {
        static constexpr int operator[](int a, int b, int c)
        {
            return a + b + c;
        }
    };
    std::cout << "static operator[] : Plus{}[1, 2, 3] = "
              << Plus{}[1, 2, 3] << "\n";
#else
    std::cout << "static operator[] is not supported (need 202211L level).\n";
#endif
}

consteval int constevalSquarePlusOne(int x)
{
    return x * x + 1;
}

constexpr int dualPathComputation(int x)
{
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval
    {
        return constevalSquarePlusOne(x);
    }
    else
    {
        return x * x + 1;
    }
#else
    return x * x + 1;
#endif
}

// 8. if consteval demo
// 8. if consteval 示例（C++23）
void ifConstevalDemo()
{
    std::cout << std::endl;
    std::cout << "=== if consteval Demo ===\n";
    std::cout << "=== if consteval示例 ===\n";

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    std::cout << "feature test macro: __cpp_if_consteval = "
              << __cpp_if_consteval << "\n";
#else
    std::cout << "if consteval not supported; showing fallback behavior.\n";
#endif

    constexpr int ct = dualPathComputation(5);
    int runtime_input = 7;
    int rt = dualPathComputation(runtime_input);
    std::cout << "compile-time result = " << ct << ", runtime result = " << rt << "\n";
}

// 9. auto(x) decay-copy demo
// 9. auto(x) 自动推导拷贝示例（C++23）
void autoCastDemo()
{
    std::cout << std::endl;
    std::cout << "=== auto(x) Demo ===\n";
    std::cout << "=== auto(x)示例 ===\n";

#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    std::cout << "feature test macro: __cpp_auto_cast = " << __cpp_auto_cast << "\n";

    int arr[3] = {10, 20, 30};
    auto ptr = auto(arr); // decay-copy: int[3] -> int*
    std::cout << "auto(arr) points to first element = " << *ptr << "\n";

    const std::string name = "cpp23";
    auto copied = auto(name);
    std::cout << "auto(name) makes a copy, value = " << copied << "\n";
#else
    std::cout << "auto(x) (decay-copy in language) is not supported.\n";
    std::cout << "Need: __cpp_auto_cast >= 202110L\n";
#endif
}

// 10. Final fix of range-based for loop lifetime demo
// 10. range-based for 生命期延长最终修补示例（C++23）
void rangeBasedForLifetimeDemo()
{
    std::cout << std::endl;
    std::cout << "=== Range-based for Lifetime Fix Demo ===\n";
    std::cout << "=== range-based for生命期修补示例 ===\n";

    struct Holder
    {
        std::vector<int> data{1, 2, 3, 4};
        const std::vector<int>& get() const { return data; }
    };

    auto makeHolder = []() { return Holder{}; };

#if defined(__cpp_range_based_for) && __cpp_range_based_for >= 202211L
    std::cout << "feature test macro: __cpp_range_based_for = "
              << __cpp_range_based_for << "\n";

    int sum = 0;
    for (int v : makeHolder().get())
    {
        sum += v;
    }
    std::cout << "C++23 fixed form sum (temporary subobject) = " << sum << "\n";
#else
    std::cout << "C++23 RBF lifetime fix not detected on this compiler.\n";
    std::cout << "Need: __cpp_range_based_for >= 202211L\n";

    int sum = 0;
    for (auto&& h = makeHolder(); int v : h.get())
    {
        sum += v;
    }
    std::cout << "safe fallback with init-statement sum = " << sum << "\n";
#endif
}

// 11. New preprocessor directives demo
// 11. 新预处理指令示例（#warning / #elifdef / #elifndef）
void preprocessorDirectivesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Preprocessor Directives Demo ===\n";
    std::cout << "=== 新预处理指令示例 ===\n";
    std::cout << "selected branch: " << PREPROCESSOR_BRANCH_TEXT << "\n";
    std::cout << "define DEMO_TRIGGER_WARNING to see #warning output at compile time\n";
}

// 12. Lambda without empty parentheses demo
// 12. lambda 省略空参数括号示例（C++23）
void lambdaWithoutParenthesesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Lambda Without () Demo ===\n";
    std::cout << "=== lambda省略空括号示例 ===\n";

    auto twice = [] mutable { return 2; };
    std::cout << "[] mutable { return 2; } -> " << twice() << "\n";
}

constexpr int constexprRelaxedDemoImpl(int x)
{
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    static constexpr int bias = 3;
    return x + bias;
#elif defined(__cpp_constexpr) && __cpp_constexpr >= 202110L
    return x + 3;
#else
    return x + 3;
#endif
}

// 13. constexpr extensions demo
// 13. constexpr 扩展示例（C++23）
void constexprExtensionsDemo()
{
    std::cout << std::endl;
    std::cout << "=== constexpr Extensions Demo ===\n";
    std::cout << "=== constexpr扩展示例 ===\n";

#if defined(__cpp_constexpr)
    std::cout << "feature test macro: __cpp_constexpr = " << __cpp_constexpr << "\n";
#else
    std::cout << "__cpp_constexpr macro is not available.\n";
#endif

    static_assert(1, "contextual conversion to bool demo");
    constexpr int v = constexprRelaxedDemoImpl(7);
    std::cout << "constexpr computation result = " << v << "\n";
}

// 14. Unicode escape enhancements demo
// 14. Unicode 转义增强示例（C++23）
void unicodeEscapesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Unicode Escapes Demo ===\n";
    std::cout << "=== Unicode转义增强示例 ===\n";

#if defined(__cpp_named_character_escapes) && __cpp_named_character_escapes >= 202207L
    std::string named = "\N{LATIN CAPITAL LETTER A}";
    std::cout << "named escape string: " << named << "\n";
#else
    std::cout << "named character escapes (\\N{...}) not supported.\n";
#endif

    std::string delimited = "\u{41}\u{42}\u{43}";
    std::cout << "delimited escape string: " << delimited << "\n";
}

std::string implicitMoveSource(std::string&& s)
{
    return s;
}

// 15. Simpler implicit move demo
// 15. implicit move 最终修补示例（C++23）
void implicitMoveDemo()
{
    std::cout << std::endl;
    std::cout << "=== Simpler Implicit Move Demo ===\n";
    std::cout << "=== implicit move修补示例 ===\n";

#if defined(__cpp_implicit_move) && __cpp_implicit_move >= 202207L
    std::cout << "feature test macro: __cpp_implicit_move = "
              << __cpp_implicit_move << "\n";
#else
    std::cout << "simpler implicit move macro not detected; behavior may be older rules.\n";
#endif

    std::string value = implicitMoveSource(std::string{"hello implicit move"});
    std::cout << "result = " << value << "\n";
}

// 16. Declaration order layout and inherited CTAD demo
// 16. 声明顺序布局与继承构造CTAD示例（C++23）
void layoutAndInheritedCtadDemo()
{
    std::cout << std::endl;
    std::cout << "=== Layout Order and Inherited CTAD Demo ===\n";
    std::cout << "=== 布局顺序与继承构造CTAD示例 ===\n";

    struct LayoutCheck
    {
        int a = 1;
    private:
        int b = 2;
    public:
        int c = 3;

        int getB() const { return b; }
    };

    LayoutCheck obj;
    std::cout << "declaration order addresses monotonic (a < c): " << std::boolalpha
              << (std::addressof(obj.a) < std::addressof(obj.c)) << "\n";
    std::cout << "values: a=" << obj.a << ", b=" << obj.getB() << ", c=" << obj.c << "\n";

#if defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 14)
    InheritedCtadDerived d([] { std::cout << "inherited CTAD works\n"; });
    d.call();
#else
    auto lam = [] { std::cout << "fallback explicit template argument path\n"; };
    InheritedCtadDerived<decltype(lam)> d(lam);
    d.call();
#endif
}

// 17. Standard library modules demo
// 17. 标准库模块 std/std.compat 示例（C++23）
void standardLibraryModulesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Standard Library Modules Demo ===\n";
    std::cout << "=== 标准库模块示例 ===\n";

#if defined(__cpp_lib_modules) && __cpp_lib_modules >= 202207L
    std::cout << "feature test macro: __cpp_lib_modules = "
              << __cpp_lib_modules << "\n";
#else
    std::cout << "std/std.compat module macro not detected in this build.\n";
#endif

    std::cout << "note: import std; needs module-enabled build pipeline and is not shown inside this TU.\n";
}

// 18. std::print demo
// 18. std::print / std::println 示例（C++23）
void printDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::print Demo ===\n";
    std::cout << "=== std::print示例 ===\n";

#if DEMO_HAS_PRINT && defined(__cpp_lib_print) && __cpp_lib_print >= 202207L && !defined(__MINGW32__)
    std::println("feature test macro: __cpp_lib_print = {}", __cpp_lib_print);
    std::println("point: ({}, {}, {})", 1, 2, 3);
    std::print("stdout via std::print -> value={}\n", 42);
#else
    std::cout << "std::print is unavailable or not link-ready on this toolchain/libstdc++ build.\n";
#endif
}

// 19. Formatting ranges demo
// 19. Ranges 格式化输出示例（C++23）
void formatRangesDemo()
{
    std::cout << std::endl;
    std::cout << "=== Format Ranges Demo ===\n";
    std::cout << "=== Ranges格式化示例 ===\n";

#if DEMO_HAS_FORMAT && defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    std::vector<int> v{2, 3, 4};
    std::map<int, std::string> m{{1, "one"}, {2, "two"}};
    std::cout << std::format("vector default = {}\n", v);
    std::cout << std::format("map default = {}\n", m);
    std::cout << std::format("vector no brackets = {:n}\n", v);
#else
    std::cout << "format ranges is not available (need __cpp_lib_format_ranges >= 202207L).\n";
#endif
}

// 20. std::expected demo
// 20. std::expected 示例（C++23）
void expectedDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::expected Demo ===\n";
    std::cout << "=== std::expected示例 ===\n";

#if DEMO_HAS_EXPECTED && defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto parseInt = [](std::string_view sv) -> std::expected<int, std::errc> {
        int value = 0;
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        if (ec == std::errc{} && ptr == sv.data() + sv.size())
        {
            return value;
        }
        return std::unexpected(ec == std::errc{} ? std::errc::invalid_argument : ec);
    };

    auto ok = parseInt("12345");
    auto bad = parseInt("12x");

    if (ok)
    {
        std::cout << "parseInt(\"12345\") = " << *ok << "\n";
    }
    if (!bad)
    {
        std::cout << "parseInt(\"12x\") error = " << static_cast<int>(bad.error()) << "\n";
    }
#else
    std::cout << "std::expected is not available (need __cpp_lib_expected >= 202202L).\n";
#endif
}

// 21. std::generator demo
// 21. std::generator 示例（C++23）
void generatorDemo()
{
    std::cout << std::endl;
    std::cout << "=== std::generator Demo ===\n";
    std::cout << "=== std::generator示例 ===\n";

#if DEMO_HAS_GENERATOR && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    auto squares = []() -> std::generator<int> {
        for (int i = 0; i < 5; ++i)
        {
            co_yield i * i;
        }
    };

    int sum = 0;
    for (int x : squares())
    {
        sum += x;
    }
    std::cout << "sum of first five squares = " << sum << "\n";
#else
    std::cout << "std::generator is not available (need __cpp_lib_generator >= 202207L).\n";
#endif
}

// 22. ranges::to and container ranges demo
// 22. ranges::to 与容器Ranges支持示例（C++23）
void rangesToAndContainersDemo()
{
    std::cout << std::endl;
    std::cout << "=== ranges::to and Containers Ranges Demo ===\n";
    std::cout << "=== ranges::to与容器Ranges示例 ===\n";

#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    auto v = std::views::iota(1, 10)
           | std::views::filter([](int x) { return x % 2 == 0; })
           | std::ranges::to<std::vector>();

    std::cout << "ranges::to<std::vector>() size = " << v.size() << "\n";
#else
    std::cout << "ranges::to is not available (need __cpp_lib_ranges_to_container >= 202202L).\n";
#endif

#if defined(__cpp_lib_adaptor_iterator_pair_constructor) && __cpp_lib_adaptor_iterator_pair_constructor >= 202106L
    std::vector<int> base{7, 8, 9};
    std::stack<int> st(base.begin(), base.end());
    std::queue<int> qu(base.begin(), base.end());
    std::cout << "stack top = " << st.top() << ", queue front = " << qu.front() << "\n";
#else
    std::cout << "stack/queue iterator-pair constructors are not available.\n";
#endif
}

// 23. mdspan demo
// 23. mdspan 多维数组视图示例（C++23）
void mdspanDemo()
{
    std::cout << std::endl;
    std::cout << "=== mdspan Demo ===\n";
    std::cout << "=== mdspan示例 ===\n";

#if DEMO_HAS_MDSPAN && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    std::vector<int> storage(12);
    for (int i = 0; i < 12; ++i)
    {
        storage[i] = i;
    }

    std::mdspan m(storage.data(), 3, 4);
    std::cout << "mdspan rank = " << m.rank() << ", value m[2, 3] = " << m[2, 3] << "\n";
#else
    std::cout << "mdspan is not available (need __cpp_lib_mdspan >= 202207L).\n";
#endif
}

// 24. stacktrace demo
// 24. 堆栈信息库 stacktrace 示例（C++23）
void stacktraceDemo()
{
    std::cout << std::endl;
    std::cout << "=== stacktrace Demo ===\n";
    std::cout << "=== stacktrace示例 ===\n";

#if DEMO_HAS_STACKTRACE && defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L && !defined(__MINGW32__)
    auto st = std::stacktrace::current();
    std::cout << "captured frames = " << st.size() << "\n";
    if (!st.empty())
    {
        std::cout << "top frame description = " << st[0].description() << "\n";
    }
#else
    std::cout << "stacktrace is unavailable or not link-ready on this toolchain/libstdc++ build.\n";
#endif
}

// 25. flat_map and flat_set demo
// 25. flat_map / flat_set 平坦容器示例（C++23）
void flatContainersDemo()
{
    std::cout << std::endl;
    std::cout << "=== Flat Containers Demo ===\n";
    std::cout << "=== 平坦容器示例 ===\n";

#if DEMO_HAS_FLAT_MAP && defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<int, int> fm;
    fm[2] = 20;
    fm[1] = 10;
    std::cout << "flat_map first key = " << fm.begin()->first << "\n";
#else
    std::cout << "flat_map is not available (need __cpp_lib_flat_map >= 202207L).\n";
#endif

#if DEMO_HAS_FLAT_SET && defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_set<int> fs{3, 1, 2};
    std::cout << "flat_set size = " << fs.size() << ", min = " << *fs.begin() << "\n";
#else
    std::cout << "flat_set is not available (need __cpp_lib_flat_set >= 202207L).\n";
#endif
}

int main()
{
    std::cout << "=== C++23 demo (compat style) ===\n";

#if __cplusplus >= 202302L
    std::cout << "Compiler mode is C++23 or later.\n";
#else
    std::cout << "Compiler mode is below C++23; running compatible examples.\n";
#endif

    std::vector<std::string> words{"cpp", "learning", "notes"};
    printWithIndexDemo(words);

    deducingThisDemo();
    multidimensionalSubscriptDemo();
    sizeTSuffixDemo();
    extendedFloatingPointDemo();
    assumeAttributeDemo();
    staticOperatorDemo();
    ifConstevalDemo();
    autoCastDemo();
    rangeBasedForLifetimeDemo();
    preprocessorDirectivesDemo();
    lambdaWithoutParenthesesDemo();
    constexprExtensionsDemo();
    unicodeEscapesDemo();
    implicitMoveDemo();
    layoutAndInheritedCtadDemo();
    standardLibraryModulesDemo();
    printDemo();
    formatRangesDemo();
    expectedDemo();
    generatorDemo();
    rangesToAndContainersDemo();
    mdspanDemo();
    stacktraceDemo();
    flatContainersDemo();

    return 0;
}
