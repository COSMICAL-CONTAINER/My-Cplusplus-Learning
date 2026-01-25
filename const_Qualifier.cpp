#include <iostream>
using namespace std;

// if we need to pass a variable but not change it, use const reference
// 如果我们需要定义一个不能被修改的变量，可以使用const

// #define GLOBAL

// If GLOBAL is defined, const_int and const_int_ptr are global variables
// 如果GLOBAL被定义，const_int和const_int_ptr是全局变量
#ifdef GLOBAL
const int const_int = 0;
const int *const_int_ptr = &const_int;
#endif 

int main()
{
    // const qualifier
    // If GLOBAL is not defined, const_int and const_int_ptr are local variables
    // 如果GLOBAL没有被定义，const_int和const_int_ptr是局部变量
    #ifndef GLOBAL
        const int const_int = 0;
        const int *const_int_ptr = &const_int;
    #endif

    // print before modification
    // 打印修改前的值
    cout << "try to change const variable through cast" << endl;
    cout << "before:" << endl;
    cout << "const_int:\t" << const_int << endl;
    cout << "&const_int:\t" << &const_int << endl;
    cout << "const_int_ptr:\t" << const_int_ptr << endl;
    cout << "*(int *)const_int_ptr: \t" << *(int *)const_int_ptr << endl;
    cout << "*const_int_ptr: \t" << *const_int_ptr << endl;

    // apply pointer casting to modify the value
    // 通过指针强制类型转换来修改值
    cout << "\ndo *((int*)(const_int_ptr)) = 1\n" << endl;
    *((int*)(const_int_ptr)) = 1;

    // error: static_cast cannot cast away constness
    // 错误: static_cast不能去掉const属性
    // *(static_cast<int *>(const_int_ptr)) = 1; 

    // print after modification
    // in c++, const_int still 0, cannot be changed
    // 打印修改后的值
    // 在c++中，const_int仍然为0，不能被修改
    cout << "after:" << endl;
    cout << "const_int:\t" << const_int << endl;
    cout << "&const_int:\t" << &const_int << endl;
    cout << "const_int_ptr:\t" << const_int_ptr << endl;
    // but through the pointer, the value is changed to 1
    // 但是通过指针，值被修改为1
    cout << "*(int *)const_int_ptr: \t" << *(int *)const_int_ptr << endl;
    cout << "*const_int_ptr: \t" << *const_int_ptr << endl;

// 1.normal variable and const Pointer
// 1.普通变量和const指针
    int i, j;
    // i is modifiable, but the value pointed to by intPtr1 is not modifiable
    // *intPtr1 is read-only
    // i是可修改的，但是intPtr1指向的值是不可修改的
    // *intPtr1 是只读的

    // *intPtr1 = 42; // error

    const int* intPtr1 = &i;
    // intPtr2 is same as intPtr1
    // intPtr2和intPtr1相同
    int const* intPtr2 = &i;
    intPtr1 = &j;     // ok

    // when a pointer is declared as const, it must be initialized at the time of declaration
    // 当指针被声明为const时，它必须在声明时初始化

    // int *const intPtr0; // error

    // intPtr3 is modifiable, but the pointer intPtr2 itself is not modifiable
    // intPtr3是可修改的，但是指针intPtr2本身是不可修改的

    // intPtr3 = &j; // error
    int *const intPtr3 = &i;
    *intPtr3 = 42;   // ok

    // const Pointer to const variable
    // both the pointer and the value pointed to are not modifiable
    // 
    // 指针和指针指向的值都是不可修改的
    // *intPtr4 = 43; // error
    // intPtr4 = &j;  // error
    const int *const intPtr4 = &i;

// 2.const variable and const Pointer
// 2.const变量和const指针
    const int k = 0;
    // int *intPtr4 = &k;          // error
    // int *const intPtr5 = &k;    // error
    const int *intPtr6 = &k;       // ok
    int const *intPtr7 = &k;       // ok
    const int *const intPtr8 = &k; // ok
    int const *const intPtr9 = &k; // ok

// 3.const variable and Reference
// 3.常量变量和引用
    const int b = 0;
    const int &a = b;
    // error: assignment of read-only location 'a'
    // 错误: 赋值只读位置'a'
    // a = 3;

    // error: cannot bind non-const lvalue reference of type 'int&' to an rvalue of type 'int'
    // 错误: 不能将非const左值引用绑定到int类型的rvalue
    // int &c = b;

    cout << "\ntry to change const variable through reference cast" << endl;
    cout << "b: " << b << endl;
    // use cast to change the value of const variable
    // 使用强制类型转换来修改const变量的值
    int &c = (int &)b;
    // try to change the value of b through c
    // 尝试通过c来修改b的值
    c = 1;
    cout << "\ndo \nint &c = (int &)b;\nc = 1;\n" << endl;
    // b is still 0
    // b仍然是0
    cout << "b: " << b << endl;
    cout << "c: " << c << endl;

    i = 43;
    const int &r1 = i;
    const int &r2 = 42;
    const int &r3 = r1 * 2;
    i = 100;
    // i is changed, r1 reflects the change
    // i被修改了，r1反映了变化
    cout << i << endl;
    cout << r1 << endl;
    // r2 and r3 are not changed
    // r2和r3没有变化
    cout << r2 << endl;
    cout << r3 << endl;

    double d_i = 42.5;
    // reference and the referenced are different types, must use const
    // following example is casted, a temporary variable is created
    // 引用和引用的类型不同，必须使用const
    // 以下示例被强制类型转换，创建了一个临时变量
    const int &dr1 = d_i;
    d_i = 43.5;
    cout << d_i << endl;
    // still the old values 42.5
    // 仍然保留旧的值 42.5
    cout << dr1 << endl;

// 4.const in function parameters
// 4.函数参数中的const
    int int_a = 1;
    double double_b = 1.23;
    int multi1(const int &x);
    int multi2(const int x);

    cout << multi1(int_a) << endl;
    cout << multi1(double_b) << endl;
    cout << multi1(1) << endl;

    cout << multi2(int_a) << endl;
    cout << multi2(double_b) << endl;
    cout << multi2(1) << endl;

    return 0;
}

int multi1(const int &x)
{
    return 2 * x;
}

int multi2(const int x)
{
    // error: const variable 'x' cannot be assigned
    // 错误: const变量'x'不能被赋值
    // x = 2 * x;
    return x;
}

void swap(const int *x, const int *y)
{
    int z;
    z = *x;
    // error
    // *x = *y;
    // *y = z;
}

int *fun(int *const f)
{
    // error
    // f = malloc();
    return f;
}
