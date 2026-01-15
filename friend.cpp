#include <iostream>
using namespace std;

// declare a class
// 声明一个类
class Complex;

// define another class
// but in theory, two classes should be put in two .h files, but here for demonstration convenience
// 定义另一个类，理论上两个类要放在两个.h里面，但这里为了方便演示
class Display
{
    // this class is not friend of Complex class
    // it means private members of Complex class cannot be accessed by Display class
    // 这个类的私有成员不能被Complex类访问
public:
    void display(const Complex &a)
    {
        cout << a.real << " + " << a.ima << "i" << endl;
    }
};

class Complex
{
    // frined class declaration, make Display class as friend of Complex class
    // 把别的类当朋友，只能单向，也就是别的类可以直接访问这个类的私有成员
    friend class Display;

    // friend function declaration, make display function of Display class as friend of Complex class
    // 把别的类的某一个函数当初朋友，只能那个函数可以访问我这个类的私有成员
    friend void Display::display(const Complex &a);

    // friend function declaration must be in the class
    // put function as friend of the class
    // 友元函数必须在类里面声明
    // 把函数当成类的朋友
    friend Complex sum(const Complex &one, const Complex &two);

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
    private:
    double real;
    double ima;
};

Complex::Complex(double num)
{
    real = num;
    ima = num;
}

// copy constructor
// 拷贝构造函数
Complex::Complex(const Complex &a)
{
    real = a.real;
    ima = a.ima;
    cout << "constructor Function Three" << endl;
}

void Complex::print()   
{
    // advantage: easy to maintain  disadvantage: increase function call overhead
    // 优点：便于程序维护  缺点：增加了函数调用的开销
    cout<<"real:" << getReal() << "\nima:"<< getIma() << endl;
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

Complex sum(const Complex &one, const Complex &two)
{
    Complex summation;
    // friend function can directly access private members
    // 友元函数可以直接访问private属性的成员
    summation.real = one.real + two.real;
    summation.real = one.real + two.real;

    // summation.setReal(one.getReal() + two.getReal());
    // summation.setIma(one.getIma() + two.getIma());
    return summation;
}

int main()
{
    
    return 0;
}