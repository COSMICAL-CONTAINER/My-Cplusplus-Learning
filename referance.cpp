#include <iostream>
using namespace std;

void swap(int *ptrA, int *ptrB)
{
    int temp = *ptrA;
    *ptrA = *ptrB;
    *ptrB = temp;
}

// use referance as function parameters binds the reference to the actual parameters,
// when the referenced object takes up a lot of memory, it is better to use.
// 将引用作为函数的形参，是将引用绑定到实参上，当被引用对象占据很大内存的时候比较好用。
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

int max(int x, int y)
{
    return (x > y ? x : y);
}

// the same pointer can't return the address of a local variable that is going to be destroyed
// 同指针一样不能返回一个将要销毁的局部变量的地址
// int& max(int x, int y)
// {
//     return (x > y ? x : y);
// }

// change to this
// 改成这样写
int& max(int &x, int &y)
{
    return (x > y ? x : y);
}

// reference to a static variable
// 静态变量的引用
int &static_return_test()
{
    static int a = 0;
    return ++a;
}

int main()
{
    int b = 0;
    // when defining a reference, it must be bound to an existing variable
    // 引用只有定义的时候才能绑定一次
    // int &num; // error
    int &a = b;
    // we can refrence the variable through the reference
    // 同一个变量可以有多个引用
    int &c = a;

    // note: don't use reference to change the type of variable
    // 不同变量类型不能使用引用
    // char &d = a;

    // same size
    // 一样的大小 
    cout << "sizeof(a):" << sizeof(a) << endl; 
    cout << "sizeof(b):" << sizeof(b) << endl;
    cout << "sizeof(c):" << sizeof(c) << endl;

    // same address
    // 一样的地址
    cout << "&b:" << &b << endl; 
    cout << "&a:" << &a << endl; 
    cout << "&c:" << &c << endl; 

    // same value
    // 一样的值
    cout << "b = " << b << endl;
    cout << "a = " << a << endl;
    cout << "c = " << c << endl;

    // change value through any of them
    // 通过其中一个改变值
    b = 1;
    cout << "b = " << b << endl;
    cout << "a = " << a << endl;
    cout << "c = " << c << endl;

    a = 2;
    cout << "b = " << b << endl;
    cout << "a = " << a << endl;
    cout << "c = " << c << endl;

    int array[4];
    int *ptr1;
    int *ptr2[4];
    int (*ptr3)[4];
    // int &r1[4]; 
    // error : no allowed to use reference array. beacause reference won't allocate space
    // 不允许使用引用数组，因为引用不会分配空间

    // if we want to define a reference to an array, we should do like this:
    // 如果想定义一个数组的引用，应该这样写：
    int (&rarray)[4] = array;

    // the pointer also can be referenced
    // 指针也有引用
    int* &rPtr = ptr1;

    // we can reference the function return value
    // 可以引用函数的返回值
    const int& max_num = max(1, 2);
    
    // reference to a static variable
    // 静态变量的引用
    cout << static_return_test() << static_return_test() << endl;

    // const reference
    // 常量引用
    const int r1 = 1;
    const int &r2 = 43;
    const int &r3 = r1 * 2;

    void FuncTest();

    FuncTest();

    return 0;
}

int fun1()
{
    cout << "fun1() called\n";
    return 0;
}

int fun2()
{
    cout << "fun2() called\n";
    return 0;
}

// function definitions for function pointer and reference tests
// 函数指针可以重新赋值，可以指向 nullptr
int (*ptrF)() = fun1;
// function reference cannot be re-bound after initialization, cannot be null
// 函数引用在初始化时绑定到某个函数，之后不能重新绑定，不能为 null
int (&rf1)() = fun1;
// reference to a function pointer: rpf is an alias to ptrF, changing rpf will modify ptrF
// 指向函数指针的引用：rpf 是 ptrF 的别名，通过 rpf 改变会修改 ptrF
int (*&rpf)() = ptrF;

void FuncTest() 
{
    cout << "ptrF() = " << ptrF() << endl;
    cout << "rf1()  = " << rf1() << endl;
    cout << "rpf()  = " << rpf() << endl;

    // function pointer can be re-assigned to another function
    // 函数指针可以重新指向另一个函数
    ptrF = fun2;
    cout << "ptrF() = " << ptrF() << endl;

    // function reference cannot be re-bound
    // 不能对函数引用重新绑定（下面这行若取消注释会导致编译错误）
    // rf1 = fun2; // error: assignment to reference

    // rpf is a reference to ptrF, assigning through rpf will modify ptrF
    // rpf 是 ptrF 的引用，通过 rpf 赋值会修改 ptrF
    rpf = fun1;
    cout << "ptrF() = " << ptrF() << endl;

    // pointer can be set to nullptr, but reference cannot
    // 指针可以为 nullptr（引用不行）
    int (*nullPtr)() = nullptr;
}