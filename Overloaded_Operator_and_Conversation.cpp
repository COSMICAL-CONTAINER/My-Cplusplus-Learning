#include <iostream>
#include <math.h>
using namespace std;

// Symbol Overloading and Type Conversion
// 符号重载与类型转换

// If the operator function is defined as a member of a class, the first parameter of the function is implicitly set to the current object through the this pointer
// If the operator function is defined as a non-member of the class, it is generally necessary to define it as a friend function of the class
// Operator functions cannot specify default arguments, except for the function call operator () where at least one parameter of the overloaded operator function is of a class type, that is,
// We cannot redefine operator functions for built-in types, such as redefining the addition of two real numbers as 1 + 1 = 3
// Only existing operators can be overloaded, and new operators cannot be defined
// Overloaded operators do not change the precedence and associativity of operations
// The functions implemented by overloaded operators should be as closely related to their original meanings as possible
// 如果运算符函数定义为类的成员，则函数的第一个参数通过this指针默认为当前对象
// 如果运算符函数定义为类的非成员，则一般需要定义为类的友员函数
// 运算符函数不能指定默认实参，但函数调用运算符()除外重载的运算符函数的参数至少有一个为类类型，也就是
// 我们不能对内置类型重新定义运算符函数，如不能重新定义两个实数相加 1 + 1 = 3
// 只能重载已有的运算符，不能自己定义新的运算符
// 重载运算符没有改变运算优先级和结合律
// 重载运算符实现的功能尽量与本身的意义相关

class Complex
{
    public:
    Complex(){real = 0; ima = 0;}
    Complex(double r, double i):real(r),ima(i){};
    Complex(double);
    Complex(const Complex &);

    Complex(Complex &a):Complex(a.real, a.ima){};
    ~Complex(){};

    void setReal(double new_real){real = new_real;};
    void setIma(double new_ima){real = new_ima;};
    double getReal()const{return real;};
    double getIma()const{return ima;};
    void print(); 
    Complex &add(const Complex &b);

    // Overload various operators
    // 重载各种运算符
    // Complex operator+(const Complex &a, const Complex &b);
    Complex operator+(const Complex &b);
    Complex operator-(const Complex &b);
    Complex& operator+=(const Complex &addedComplex);
    Complex& operator-=(const Complex &subbedComplex);
    Complex& operator=(const Complex &complexObj);
    bool operator==(const Complex &complexObj);
    bool operator!=(const Complex &complexObj);

    Complex& operator++();
    Complex operator++(int);

    // overload [] operators
    // 重载[]
    // private:string title; 
    // char& operator[](int n){return title[n]};

    // Overload *-> symbol
    // Note:
    // 1.-> must be a member function; * can be a non-member function
    // 2. The meaning of * can be changed, but the meaning of -> cannot be changed; it must refer to obtaining a member
    // 3. The return value of -> is a pointer to an object or an object itself (with overloading of ->)
    // 重载 *-> 符号
    // 注意:
    // 1.->必须是成员函数，;*可以是非成员函数
    // 2.可以改变*的含义，但->的含义不可改变，一定是获取成员
    // 3.->的返回值为对象的指针或对象(含有->的重载)
    Complex& operator*(){return (*this);};
    Complex* operator->(){return this;};

    // overload () function object
    // 重载() 函数对象
    double operator()(const Complex &a){return sqrt(pow(a.real, 2) + pow(a.ima, 2));};
    // Complex One(3, 4);
    // Complex modulus();
    // cout << modulus(One) << endl;

    // overload type conversion operator, usually written as member function
    // 类型转换运算符 一般写成成员函数
    operator double() const {return real;}
    // Complex One(3, 4);
    // cout << double(One) << endl;
    /*
    struct A;
    struct B{ 
        operator A()const{};
    };

    struct A
    {
        A(const B&){}
    };

    void f(const A&){}
    int main()
    {
        B b;
        A a = f(b);
    }
    // The above code will generate an error, with two type conversion functions appearing
    // Try not to overload type conversion operators unless necessary
    // 上述代码会报错，出现两个类型转换函数
    // 除非必要，尽量不要重载类型转换运算符
*/
    // Overload input and output
    // 重载输入输出

    // void operator<<(ostream &out);
    // If written like this, when called, it will be Complex a; a << std::cout;
    // Therefore, it is generally not written as a member function
    // 如果这样写到时候调用就是Complex a;  a << std::cout;
    // 所以一般不写成成员函数
    friend ostream& operator>>(ostream &in, const Complex &complexIn);

    private:
    double real;
    double ima;
};

Complex::Complex(double num)
{
    real = num;
    ima = num;
}

// Copy constructor
// 拷贝构造函数
Complex::Complex(const Complex &a)
{
    real = a.real;
    ima = a.ima;
    cout << "constructor Function Three" << endl;
}

void Complex::print()   
{
    // Advantages: Facilitates program maintenance  Disadvantages: Increases the overhead of function calls
    // 优点：便于程序维护  缺点：增加了函数调用的开销
    cout<<"real:" << getReal() << "  ima:"<< getIma() << endl;
}

Complex & Complex::add(const Complex &b)
{
    real += b.real;
    ima += b.ima;
    return (*this);
}

Complex getComplex(const Complex a)
{
    return a;
}

Complex Complex::operator+(const Complex &b)
{
    Complex summation;
    summation.real = real + b.real;
    summation.ima = ima + b.ima;
    return summation;
}

Complex Complex::operator-(const Complex &b)
{
    Complex sub;
    sub.real = real - b.real;
    sub.ima = ima - b.ima;
    return sub;
}

Complex& Complex::operator+=(const Complex &addedComplex)
{
    real += addedComplex.real;
    ima += addedComplex.ima;
    return (*this);
}

Complex& Complex::operator-=(const Complex &subbedComplex)
{
    real -= subbedComplex.real;
    ima -= subbedComplex.ima;
    return (*this);
}

// If it's not a member function, write it like this, and remember to add a friend function, because private member variables cannot be accessed from outside
// 如果不是成员函数的话就这样写,记得加上友元函数，因为私有成员变量外部不能访问
// Complex& operator+=(Complex &one, const Complex &addedComplex)
// {
//     one.real += addedComplex.real;
//     one.ima += addedComplex.ima;
//     return one;
// }

Complex& Complex::operator=(const Complex &complexObj)
{
    real = complexObj.real;
    ima = complexObj.ima;
    return (*this);
}

bool Complex::operator==(const Complex &complexObj)
{
    if (real == complexObj.real && ima == complexObj.ima)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Complex::operator!=(const Complex &complexObj)
{
    if (real == complexObj.real && ima == complexObj.ima)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// 1. why return object instead of reference?
// Because the post increment creates a temporary object that will be destroyed when the function ends
// If you return a reference, the temporary object will be destroyed after the function ends, and the reference will point to invalid memory
// 2. Why const before the post increment?
// Prevents calling the post increment operator twice in a row (such as i++++)
// Consistent with built-in type behavior
// The first return is the old value, not the original object, and calling two post increments results in only one increment
// 3. Performance considerations:
// When dealing with user-defined types, it is better to use the pre-increment operator
// Because it does not create a temporary object, thus avoiding the additional overhead caused by construction and destruction
// 1. 为什么后置返回对象，而不是引用？
// 因为后置为了返回旧值创建了一个临时对象，在函数结束的时候这个对象就会被销毁
// 如果返回引用，那么临时对象被销毁后，引用就指向了无效的内存
// 2. 为什么后置前面要加const？
// 防止连续两次调用后置++重载符（如i++++）
// 与内置类型行为保持一致
// 第一次返回的是旧值，而不是原对象，调用两次后置++，结果只累加了一次
// 3. 性能考虑：
// 处理用户的自定义类型时，最好使用前置++
// 因为它不会创建临时对象，进而不会带来构造和析构而造成的格外开销
Complex &Complex::operator++()
{
    real++;
    return (*this);
}

Complex Complex::operator++(int)
{
    Complex temp = *this;
    real++;
    return temp;
}

// If the return value is void, you cannot write cout << a << b;
// Remember to add references. Streams do not have copy constructors, and a computer only has one screen and one keyboard
// 如果返回值为void就不能这样写cout << a << b;
// 一定要记住加上引用，流没有拷贝构造函数，一台电脑就只有一个屏幕一个键盘
ostream& operator<<(ostream &out, const Complex &complexOut)
{
    out << "real:" << complexOut.getReal() << "  ima:"<< complexOut.getIma();
    return out;
}

ostream& operator>>(ostream &in, const Complex &complexIn)
{
    cout << "Please input a Complex: " << endl;
    cout << "Input real: ";
    in >> complexIn.real;
    cout << "Input ima: ";
    in >> complexIn.ima;
    return in;
}

int main()
{
    Complex c1(1,2);
    Complex c2(3,4);

    c1.print();
    c2.print();

    cout << c1 + c2 << endl;;

    cout << c1++ << endl;
    cout << ++c2 << endl;

    return 0;
}
