#include <iostream>
using namespace std;
// Template
// Template is a tool for type parameterization in C++
// The purpose of using templates is to enable programmers to write code that is independent of specific types
// Using templates allows users to declare a general pattern for classes or functions, enabling certain data members in the class or the parameters and return values of member functions to take any type
// There are usually two forms: function templates and class templates.
// The declaration or definition of a template can only be made within the global, namespace, or class scope. It cannot be made within a local scope or a function, such as declaring or defining a template within the main function
// 模板
// 模板是C++对类型参数化的工具
// 使用模板的目的就是能够让程序员编写与类型给无关的代码
// 使用模板可以使用户为类或者函数声明一种一般模式，使得类中的某些数据成员或者成员函数的参数、返回值取得任意类型
// 通常有两种形式：函数模板（Function Template）和类模板（Class Template）。
// 模板的声明或定义只能在全局，命名空间或类范围内进行。既不能在局部范围，函数内进行，比如不能在main函数中声明或定义一个模板

// template<Template parameter (cannot be empty)> Function parameter
// template<模板参(不能为空)>函数参数
// template <typename T, typename X> T my_max(const T &a, const T &b)

template <typename T> T my_max(const T &a, const T &b)
{
    return a > b ? a : b;
}

template <typename T> void fun1(T rhs)
{
    rhs = 4;
    cout << rhs * 2 << endl;
}

template <typename T> void fun2(const T rhs)
{
    // rhs = 4;
    cout << rhs * 2 << endl;
}

template <typename T> void fun3(T& rhs)
{
    rhs = 4;
    cout << rhs * 2 << endl;
}

template <typename T> void fun4(const T& rhs)
{
    rhs = 4;
    cout << rhs * 2 << endl;
}

template <typename T> void fun5(const T* rhs)
{
    cout << *rhs * 2 << endl;
}

template <typename T> void fun6(T rhs)
{
    rhs[0] = 3;
    cout << rhs[0] * 2 << endl;
}

template <typename T> void fun7(T& rhs)
{
    rhs[0] = 3;
    cout << rhs[0] * 2 << endl;
}

template <typename T> void fun8(T rhs)
{
    cout << rhs(0) << endl;
}

int doubleX(int rhs)
{
    return rhs * 2;
}

class A
{
    public:
    A operator()()
    {
        cout << "()";
        return *this;
    }
};

int main()
{
    A a;
    a()();
    cout << my_max(1, 2) << endl;
    cout << my_max(1.3, 1.0) << endl;

    cout << my_max<int>(1, 2) << endl;
    cout << my_max<int>(1.3, 2.3) << endl;
    // When the parameters are different, the passed parameters should be displayed, otherwise an error will be reported
    // 对于参数不一样的时候要显示传参，否则报错
    // cout << my_max(1.3, 2) << endl;
    
    int x = 3;
    int & x1 = x;
    const int x2 = 3;
    const int & x3 = 3;

    // 以下T都是int
    fun1(x);
    fun1(x1);
    fun1(x2);
    fun1(x3);
    fun1(3);
    fun2(x);
    fun2(x1);
    fun2(x2);
    fun2(x3);
    fun2(3);

    fun3(x);    // int
    fun3(x1);   // int
    fun3(x2);   // const int
    fun3(x3);   // const int
    // fun3(3);    //error

    fun4(x);    // int
    fun4(x1);   // int
    fun4(x2);   // int
    fun4(x3);   // int
    fun4(3);    // int

    fun5(&x);    // int
    fun5(&x1);   // int
    fun5(&x2);   // int
    fun5(&x3);   // int

    int x[3] = {3};
    fun6(x);     //int *

    fun7(x);     //int [3]

    fun8(doubleX);//int(*)(int)
    return 0;
}