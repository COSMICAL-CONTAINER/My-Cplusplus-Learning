#include <iostream>
using namespace std;

// Proxy class, hide the original class, use new class to call
// 代理类，隐藏原始类，使用新的类来调用

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
    private:
    double real;
    double ima;
};


Complex::Complex(double num)
{
    real = num;
    ima = num;
}

Complex::Complex(const Complex &a)
{
    real = a.real;
    ima = a.ima;
    cout << "constructor Function Three" << endl;
}

void Complex::print()   
{
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

// interface class(proxy class), use new class to call, hide the original class!!!
// 接口类(代理类)，使用新的类来调用,将原来的类全部隐藏！！！
class Interface
{
    public:
    Interface(){pr = new Complex();};
    ~Interface(){delete pr;};

    void print(){pr->print();};

    private:
    Complex * pr;
};


int main()
{
    Interface a;
    a.print();

    return 0;
}