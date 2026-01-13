#include <iostream>
using namespace std;

// when define a inline function, the compiler will replace the function call with the function code
// The inline keyword is optional when declaring an inline function
// The declaration and definition of an inline function must be placed in the same file (the compiler does not see the function body) 
// Member functions declared and defined simultaneously in a class are automatically converted to inline functions 
// This is a typical approach to trade space for time
// 内联函数声明的时候inline可加可不加
// 内联函数的声明和定义必须放到同一个文件（编译器看不到函数体）
// 成员函数在类中同时声明和定义，自动转为为内联函数
// 典型的空间换时间的做法
int max(int a, int b);
inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

int main()
{
    int i = 0, j = 2, k;
    k = max(i, j);
    // In fact, this function is not called, reducing the overhead of function calls,
    // but the code will become longer, which means the final exe file will be larger. 
    // Generally, we only inline small functions. 
    // Currently, C++ compilers can recognize functions and automatically inline some small functions,
    // while users can only add inline to suggest the compiler to inline
    // 实际上没有调用这个函数,减少函数调用的开销,但是代码会变长，也就是最终生成的exe变大
    // 一般来说我们只对小函数进行inline
    // 目前c++编译器能够识别函数，对一些小函数自动内联，而用户只能加上inline来建议编译器内联
    // {
    //     int a = i, b = j;
    //     k = (a > b) ? a : b;
    // }
    cout << "the Max number is:" << k << endl;
    return 0;
}