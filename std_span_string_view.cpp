#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#if __cplusplus >= 202002L
#include <span>
#endif

using namespace std;

// std::span / std::string_view
// 核心思想: 非拥有视图（不负责资源生命周期）

void print_sv(string_view sv)
{
    cout << "string_view(len=" << sv.size() << "): " << sv << '\n';
}

#if __cplusplus >= 202002L
int sum_span(span<const int> values)
{
    int total = 0;
    for (int x : values)
        total += x;
    return total;
}
#endif

int main()
{
    cout << "=== std::string_view demo ===\n";
    string text = "hello string_view";
    print_sv(text);
    print_sv(string_view(text).substr(6, 11));

    cout << "\n[warning] string_view does not own memory\n";
    cout << "[注意] string_view 不拥有内存，原字符串失效后它也会失效\n";

#if __cplusplus >= 202002L
    cout << "\n=== std::span demo ===\n";
    int arr[] = {1, 2, 3, 4, 5};
    vector<int> vec = {10, 20, 30};

    span<const int> s1(arr);
    span<const int> s2(vec.data(), vec.size());

    cout << "sum(arr span) = " << sum_span(s1) << '\n';
    cout << "sum(vec span) = " << sum_span(s2) << '\n';
    cout << "subspan(arr[1..3]) sum = " << sum_span(s1.subspan(1, 3)) << '\n';
#else
    cout << "\nstd::span requires C++20 or later.\n";
    cout << "std::span 需要 C++20 及以上标准。\n";
#endif

    return 0;
}
