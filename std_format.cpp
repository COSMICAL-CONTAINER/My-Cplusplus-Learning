#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#if __has_include(<format>)
#include <format>
#endif

using namespace std;

// std::format learning notes
// Automatically demonstrate iostream method if std::format is not fully supported by the compiler/library
// std::format 学习笔记
// 若编译器/标准库未完整支持 std::format，则自动演示 iostream 方式

int main()
{
    string name = "Alice";
    double score = 95.2367;
    int rank = 3;

    cout << "=== formatting demo ===\n";

#if __has_include(<format>) && __cplusplus >= 202002L
    cout << std::format("name: {}, score: {:.2f}, rank: {}\n", name, score, rank);
    cout << std::format("align -> |{:>10}|{:<10}|\n", "right", "left");
    cout << std::format("hex: {:#x}, binary-ish(not native): {}\n", 255, 255);
#else
    cout << "std::format is unavailable in current toolchain, fallback to iostream.\n";
    cout << "当前工具链不支持 std::format，使用 iostream 作为回退示例。\n";

    ostringstream oss;
    oss << "name: " << name << ", score: " << fixed << setprecision(2) << score << ", rank: " << rank;
    cout << oss.str() << '\n';
    cout << "align -> |" << setw(10) << right << "right" << "|" << setw(10) << left << "left" << "|\n";
#endif

    return 0;
}
