#include <iostream>
using namespace std;

class Complex
{
public:
    // Default constructor: no return value (void is not allowed), no need to pass any value when calling
    // If a class does not have a constructor, the system will automatically provide this default function
    // Constructors's name is same as class name but no return value
    // 缺省构造函数：没有返回值(void也不行)，调用不需要传入任何值，如果一个类没有构造函数则系统会自动提供这个缺省函数
    // 构造函数与类名一样且不能有返回值
    // 一般构造函数在类有指针、引用、const类型变量来说都自己写构造函数
    Complex(){real = 0; ima = 0;}
    // Complex(double, double = 0);

    // initializer list is efficient than assignment with equal sign
    // 更高效的初始化列表，因为没有用等号赋值
    Complex(double r, double i):real(r),ima(i){};

    // explicit can prevent implicit calls to this constructor
    // only one argument constructor can be used for type conversion
    // explicit 加了这个关键词可以限制这个构造函数用于类型转化,不能显示的调用
    // 只能one.add(Complex(3.2))来隐式调用
    /* explicit */ Complex(double);

    // copy constructor: the compiler will automatically insert it
    // must use reference &, if not, it will be value passing, which will continue to call the copy constructor, leading to infinite recursion
    // principle: used together with default constructor
    // 拷贝构造函数-编译器会自动插入
    // 必须要加引用&，如果不加的话会出现值传递，会继续调用拷贝构造函数，永远无限循环
    // 原则：与缺省构造函数一起使用
    Complex(const Complex &);

    // Delegate constructor
    // 委托构造函数
    Complex(Complex &a):Complex(a.real, a.ima){};

    // Destructor: no return value, no parameters, only system can call it, only one destructor per class, cannot be overloaded
    // system will also automatically generate it, generally used to release space free or delete
    // 析构函数只有系统能够调用，用户并不能调用，所以析构函数没有参数, 一个类析构函数是唯一的，不能重载
    // 系统也会自动生成， 一般来说析构函数用来释放空间 free 或 delete
    ~Complex(){};

    // member functions generally set as set and get functions
    // 成员函数一般设置为set 和 get 函数
    void setReal(double new_real){real = new_real;};
    void setIma(double new_ima){real = new_ima;};

    // Adding const to a constant function definition indicates that no values within the function will change (by any means)
    // Constructor and destructor cannot use const functions
    // Ordinary functions can call constant functions
    // 常函数定义加上const，表示这个函数里面不会改变任何值(任何手段)
    // 构造函数和析构函数都不能使用const函数
    // 普通函数可以调用常函数
    double getReal()const{return real; /* setReal(1.2); 这也是不可以的，常函数只能调用常函数 */};
    double getIma()const{return ima;};

    // Ordinary functions can be overloaded
    // 常函数也可以重载
    // double getIma(){return ima;};

    // The following is not allowed to call, will have a forced conversion, actually a.getIma() = 6 can also change
    // 下面也是不可以调用的，会有强转，其实a.getIma() = 6也能改变
    // double& Complex::getIma()const{return ima;}
    // Ordinary functions returning references must return const references
    // 常函数返回引用必须返回常引用
    // const double& Complex::getIma()const{return ima;}

    // Static member functions belong to the class itself rather than to any object of the class
    // Static member functions can be called without creating an instance of the class
    // Through changes in power, it can be observed that static functions can only call static functions, but ordinary functions can call static member functions
    // 静态成员函数没有this指针，所以不能访问类内的非静态成员成员(数据和函数)
    // 静态成员可以具有public和private属性，静态成员函数没有const属性 
    // 通过权力的变化，可以发现静态函数只能调用静态函数，但是普通函数可以调用静态成员函数
    // static double getReal() const;
    // static double setReal(double); 
    // 使用Complex::getReal()调用函数

    void print(); 
    Complex &add(const Complex &b);
private:
    // const variables must be initialized when declared, cannot be assigned later
    // use mutable to change the access permission of data members of constant objects
    // 常变量只能访问常成员函数,不能访问非常成员函数
    // 使用mutable关键字改变常对象数据成员的访问权限
    // mutable double real;
    double real;

    // static data members belong to the class itself rather than to any object of the class
    // can be used to record the number of objects created from the class
    // 所有的对象都共享一个数据成员（所有的对象共用一个地址空间），算是属于类的数据成员 
    // 可以使用Complex::real来访问或者用对象来访问
    //inline static double real;

    double ima;
    // reference type and const variables must be initialized when declared, cannot be assigned later
    // 引用和常变量只能放初始化列表里初始化，必须一开始就初始化
    // double &z; const int a;

    // cannot contain itself, will infinitely call the constructor
    // 不能自己包含自己，会无穷调用构造函数
    // Complex inComplex;

    // but can be static member, because static members are not constructed with the object
    // 但是静态成员不会分配空间，构造函数不去构造它，就不会有问题
    static Complex inComplex;

    // or can be pointer type, because pointer type only allocates space for the pointer itself
    // 指针类型也可以
    // Complex* inComplex;
};


// Complex::Complex(double r, double i = 0)
// {
//     real = r;
//     ima = i;
// }

// initializer list and class member initialization
// 初始化列表+类初值初始化
// Complex::Complex(double r, double i):real(r)
// {
//     ima = i;
// }

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

// Complex::Complex(const Complex &a)
// {
//     real = a.real;
//     ima = a.ima;
//     // shallow copy, just copy the pointer address
//     // 浅拷贝，只是把指针地址拷贝了一下
//     ptr = a.ptr   
// }
// Complex::Complex(const Complex &a)
// {
//     real = a.real;
//     ima = a.ima;
//     // deep copy, copy the value pointed to by the pointer
//     // 深拷贝，把指针指向的值也拷贝了一下
//     ptr = new double(*a.ptr)
// }

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

int main()
{
    // run default constructor function? no, this is treated as function declaration
    // 调用缺省构造函数?不对，把a当成函数声明了;
    Complex a(void);
    // a.print(); //error: a is not an Complex object
    
    // This is the correct way to call the default constructor
    // 正确调用缺省构造函数的三种方法

    // 1.Call a general constructor function
    // 1.调用一般构造函数
    Complex num1(1, 2);
    Complex num2(3, 4);
    num1.print();
    num2.print();

    // 2.Call the default constructor function
    // 2.调用缺省构造函数
    Complex complex_array1[5];
    complex_array1[0].print();
    // Complex complex_array2[2](0.1) // error: not allowed
    Complex complex_array2[2] = {Complex(1,2), Complex(3,4)};
    complex_array2[0].print();
    complex_array2[1].print();

    // 3.Call the copy constructor function
    // 3.调用拷贝构造函数
    Complex num3(num2);
    num3.print();
    Complex num4 = num1;
    num4.print();
    Complex num5 = getComplex(num1);
    num5.print();

    // A constructor function can be used as a type conversion, 
    // and it is necessary to have this function
    // 构造函数可以当类型转换来用, 一定要有这个函数才行 
    Complex num6;
    num6 = 3.2;
    // this step is actually completed in two steps
    // First call Complex(double);
    // It is equivalent to converting a double to a Complex object, similar to int a = 3.2; and then calling the copy constructor
    // 这一步其实分两步完成的
    // 先调用Complex(double); 
    // 相当于把一个double强转成Complex对象 类似与int a = 3.2; 然后调用拷贝构造函数

    num6.print();
    // If it is constructed as a constant, the constructor must be annotated with const to complete the conversion.
    // 如果是常数来构造，构造函数就必须加上const才能完成转换。
    num6.add(2.3);
    num6.print();

    // Here, we need to destruct Complex, following the principle of last-in-first-out, because local variables are placed in the stack area
    // If there are global variables, they are constructed before the main function, so they are the last to be destructed
    // 到这里需要析构Complex，遵循先进后出原则，因为局部变量放在栈区
    // 如果有全局变量，一开始在主函数前就先构造全局变量了，所以是最后一个析构的
    /*
        void creat()
        {
            Complex three(3);
        }

        Complex one(1);
        int main()
        {
            Complex two(2);
            creat();
            Complex four(4);
            return 0;
        }
        // Structure      1   构造1
        // Construction   2   构造2
        // Construction   3   构造3
        // Deconstruction 3   析构3
        // Construction   4   构造4
        // Deconstruction 4   析构4
        // Deconstruction 2   析构2
        // Deconstruction 1   析构1
    */
    return 0;
}