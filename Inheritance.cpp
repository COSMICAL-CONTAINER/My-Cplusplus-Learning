#include <iostream>
#include <string>
using namespace std;

// Inheritance继承
// Derivation class is a specialization of base class, while base class is an abstraction of derived class.
// 派生类是基类的具体化，而基类则是派生类的抽象

/*
describeion Inheritance and Derivation in C++
● Inheritance and Derivation are two sides of the same coin
  ➢ The process of creating a new class by retaining the characteristics of an existing class is called Inheritance
  ➢ The process of creating a new class by adding its own characteristics on the basis of an existing class is called Derivation.
● The purpose of Inheritance: to achieve design and code reuse.
● The purpose of Derivation: when new problems arise that the original program cannot solve (or
    cannot completely solve), it is necessary to modify the original program.
● The existing class being inherited is called the base class (or parent class)
● The new class derived is called the derived class (or child class)
● The base class that directly participates in the derivation of a certain class is called the direct base class
● The base class of the base class or even higher-level base class is called the indirect base class
*/

/*
继承(Inheritance)与派生(Derivation)概述
● 继承与派生是同一过程从不同的角度看
  ➢ 保持已有类的特性而构造新类的过程称为继承
  ➢ 在已有类的基础上新增自己的特性而产生新类的过程称为派生。
● 继承的目的:实现设计与代码的重用。
● 派生的目的:当新的问题出现，原有程序无法解决(或不能完全解决)时，需要对原有程序进行改造。
● 被继承的已有类称为基类(或父类)
● 派生出的新类称为派生类(或子类)
● 直接参与派生出某类的基类称为直接基类
● 基类的基类甚至更高层的基类称为间接基类
*/

class Point
{
public:
    // can be accessed by member functions (friends) and class objects, but not by external functions
    // 可以被成员函数（友元）、类对象、类外函数访问
    Point(double x, double y, string point_name):x(x),y(y),point_name(point_name){};
    ~Point(){};
    void getX() const;
    void print(ostream &out) const
    {
        out << "This is a Point:"<< point_name << endl;
        out << "(" << x << "," << y << ")" << endl;
    }

protected:
    // only member functions (friends) can access
    // 只能被成员函数（友元）访问
    void move(double x,double y){x = x;y = y;};

private:
    // string title;
    // Protected member - positioned between public and private, utilized in inheritance, and accessible only by member functions (or friends)
    // 保护成员-介于共有与私有之间，在继承中使用，只能被成员函数（友元）访问
    double x,y;
    string point_name;

};
// Divided into public, private, protected, and derived
// Regardless of the situation, the public and protected members of the base class can be accessed within the derived class
// However, if it is a private or protected derivation, the public and protected members of the base class cannot be accessed from outside the derived class
// 分为public private protected派生
// 不管怎么样派生类内部都能访问到基类的publlic和protected
// 但是如果是private和pootected派生，在派生类外部就访问不到基类的pubic和protected了
class Circle:public Point
{
public:
    Circle(double x, double y, double r, string circle_name):Point(x,y,circle_name + "_p"),radius(r),circle_name(circle_name){};
    ~Circle(){};
    void getR() const;
    // The original print function no longer meets the usage requirements. Define a function with the same name as the parent class to override the original function
    // 原来的print函数不满足使用要求了，定义一个和父类一样函数名的函数覆盖原来的函数
    void print(ostream &out) const
    {
        out << "This is a Circle:"<< circle_name << endl;
        out << "Center:";
        // this must add the domain qualifier, otherwise it will loop infinitely
        // 这里要加域限定符，要不然会无限循环
        Point::print(out);
        out << "Radius:" << radius << endl;
    }
protected:
    void enlarge(double r){radius = r;};
private:
    double radius;
    string circle_name;
};

class Cylinder:public Circle
{
public:
    Cylinder(double x, double y, double r, double h, string cylinder_name):Circle(x, y, r, cylinder_name + "_c"),height(r),cylinder_name(cylinder_name){};
    ~Cylinder(){};
    void getH() const;
    // The original `print` function no longer meets the usage requirements. Define a function with the same name as the parent class to override the original function
    // 原来的print函数不满足使用要求了，定义一个和父类一样函数名的函数覆盖原来的函数
    void print(ostream &out) const
    {
        out << "This is a Cylinder : "<< cylinder_name << endl;
        out << "The bottom Center:" << endl;
        // 这里要加域限定符，要不然会无限循环
        Circle::print(out);
        out << "Height:" << height << endl;
    }
protected:
    void raise(double h){height = h;};
private:
    double height;
    string cylinder_name;
};

/*
(1) Member functions of the base class access base class members.
(2) Member functions of derived classes access members added by the derived class itself.
(3) Accessing members of a derived class from a member function of the base class. (Cannot be accessed) 
(4) Accessing members of the base class from a member function of the derived class. (Can be accessed, only public and protected members can be accessed, private members cannot be accessed)
(5) Accessing members of the derived class from outside the derived class (derived class objects and pointers).
(6) Accessing members of the base class from outside the derived class. (Yes, only public members can be accessed, protected and private members cannot be accessed)
/*
(1)基类的成员函数访问基类成员。
(2)派生类的成员函数访问派生类自己增加的成员。
(3)基类的成员函数访问派生类的成员。             (访问不到)
(4)派生类的成员函数访问基类的成员。             (可以,只能访问public和protected不能访问private)
(5)在派生类外(派生类对象和指针)访问派生类的成员。
(6)在派生类外访问基类的成员。                   (可以，只能访问public，不能访问protected、private)
*/

int main()
{
    // First construct the point and then construct the circle
    // 先构造点然后构造圆
    // Circle c1, *pc = &c1;

    // Both call the print function of the derived class
    // However, if the print function in Circle has no parameters, it will report an error, which will also be overridden
    // You can use c1.Point:print(cout) to call it
    // You can also use using Point::print(); to make the function visible, which becomes function overloading
    // 都调用派生类的print函数
    // 但是如果Circle中print函数没有参数的话就会报错，这样也会被覆盖
    // 可以使用c1.Point:print(cout)进行调用
    // 还可以使用 using Point::print();让函数可见，就变成函数重载了

    // c1.print(cout);
    // pc->print(cout);

    Cylinder Cy1(1,1,2,3,"Cylinder1");
    Cy1.print(cout);
    return 0;
}

/*
When creating an object, the order in which the constructor functions are executed is:
  ➢ The derived class constructor first calls the base class constructor; if there are multiple base classes, they are constructed in the order they are declared in the derived class
  ➢ Then execute the derived class constructor itself (i.e., the function body of the derived class constructor)
● When a derived class object is released
  ➢ Execute the derived class destructor first
  ➢ Then execute its base class destructor, which follows the reverse order of construction
*/
/*
● 在建立一个对象时，执行构造函数的顺序是:
  ➢ 派生类构造函数先调用基类构造函数;若有多个基类，按照派生类声明时基类的次序先后构造
  ➢ 再执行派生类构造函数本身(即派生类构造函数的函数体)
● 在派生类对象释放时
  ➢ 先执行派生类析构函数
  ➢ 再执行其基类析构函数，基类析构次序与构造次序相反
*/