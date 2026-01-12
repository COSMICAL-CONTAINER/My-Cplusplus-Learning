#include <iostream>
using namespace std;

// overloaded function with default arguments
// 函数重载：函数名相同，参数类型或个数不同

// this is a normal overloaded function 
// 缺省参数函数的申明
int max(int =1, int =2, int =3);
int max(int x = 1, int y = 2, int z = 3)
{
    return x;
}

// notes: only the trailing parameters can be given default values
// max(,2) can't be used, because the first parameter is missing
// 注意：缺省参数只能在函数声明或定义中出现一次，不能同时出现在声明和定义中
// max(,2)这种函数调用是不行的，所以第一个参数给了缺省值，第二个没给，相当于没用，会报错
// 所以给缺省值一定要从最后面开始给
// int max(int =1, int , int );  //错误

// default arguments can only be declared once
// 缺省参数也可以声明两次
// int max(int , int , int);
// int max(int =1, int =2, int);

// when have overloaded functions with default arguments
// 多个匹配函数，编译器不知道是哪个函数会报错
// int max(int =1, int =2, int =3);
// int max(int, int);
// max(2, 1);

// we must make sure the best match is unique for overloaded function calls
// 对重载函数调用，一定要保证最好匹配的唯一性
// int max(double x, double y);
// int max(int x, int y);
// max(1.0, 2.0);  // 可以的
// max(1, 2.0);    // 每个都匹配50%，肯定会报错

int main()
{
    // x
    max(1);
    // x y
    max(2, 3);
    // x y z
    max(1, 3, 5);
    return 0;
}