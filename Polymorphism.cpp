#include <iostream>
using namespace std;

// Dynamic Polymorphism - Virtual Functions
// Constructors can never be virtual functions, because the derived class is constructed first, then the base class, and so on
// Destructor functions are generally written as virtual functions
// 动态多态性-虚函数
// 构造函数永远不能是虚函数，因为先构造派生类才有基类,先有
// 析构函数一般要写成虚函数

// Point *p = new Circle;
// delete p;

// If the destructor of Point is not virtual, only the point will be destructed, and the circle will never be destructed, resulting in a memory leak
// Therefore, destructors are often written as virtual functions
// 如果Point的析构函数不是虚函数，则只会析构点，永远不会析构圆，造成内存泄露
// 所以析构函数往往要写成虚函数

class Point
{
public:
    Point(){};
    Point(double x, double y, string point_name) : x(x), y(y), point_name(point_name){};
    ~Point(){};
    virtual const Point* getme() const{cout << "get Point" << endl; return this;};
    virtual const void showType()const{cout << "Type Point" << endl;};
    // Base class with virtual functions, and the same-named functions in derived classes are automatically marked as virtual
    // 基类加上虚函数，派生类的同名函数自动加上virtual
    virtual void print(ostream &out) const
    {
        out << "This is a Point:" << point_name << endl;
        out << "(" << x << "," << y << ")" << endl;
    }

protected:
    void move(double x, double y)
    {
        x = x;
        y = y;
    };

private:
    double x, y;
    string point_name;
};

class Circle : public Point
{
public:
    Circle(double x, double y, double r, string circle_name) : Point(x, y, circle_name + "_p"), radius(r), circle_name(circle_name){};
    ~Circle(){};
    const Circle* getme() const{cout << "get Circle" << endl; return this;};
    const void showType()const{cout << "Type Circle" << endl;};
    void getR() const;
    void print(ostream &out) const
    {
        out << "This is a Circle:" << circle_name << endl;
        out << "Center:";
        Point::print(out);
        out << "Radius:" << radius << endl;
    }

protected:
    void enlarge(double r) { radius = r; };

private:
    double radius;
    string circle_name;
};

class Cylinder : public Circle
{
public:
    Cylinder(double x, double y, double r, double h, string cylinder_name) : Circle(x, y, r, cylinder_name + "_c"), height(r), cylinder_name(cylinder_name){};
    ~Cylinder(){};
    void getH() const;
    void print(ostream &out) const
    {
        out << "This is a Cylinder : " << cylinder_name << endl;
        out << "The bottom Center:" << endl;
        Circle::print(out);
        out << "Height:" << height << endl;
    }

protected:
    void raise(double h) { height = h; };

private:
    double height;
    string cylinder_name;
};

class Square : public Point
{
public:
    Square(double x, double y, double z, string square_name) : x(x), y(y),square_name(square_name){};
    ~Square(){};
    const Square* getme() const{cout << "get Square" << endl; return this;};
    const void showType()const{cout << "Type Square" << endl;};
    void getH() const;
    void print(ostream &out) const
    {
        out << "This is a Square : "<< square_name << endl;
        out << "len_x: "<< x << "  len_y: " << y << endl;
        out << "Height:" << height << endl;
    }

protected:
    void raise(double h) { height = h; };

private:
    double x,y;
    double height;
    string square_name;
};

// Virtual functions are only for member functions of a class, and ordinary functions cannot be declared as virtual
// In the base class, the virtual keyword is required when declaring member functions with virtual, but it is not necessary when defining them outside the class
// The function name, function type, number of function parameters, and parameter types are all the same as the virtual function of the base class, and are redefined according to the needs of the derived class
// It is possible to return a pointer to a base class from a base class and a pointer to a derived class from a derived class, as long as type casting is applied
// Both the entry and exit of the function are in the base class, and the function body is also in the base class
// virtual函数只针对类的成员函数，普通函数不能声明虚函数
// 在基类用virtual声明成员函数要加上virtual关键字，在类外部定义则不需要加上virtual关键字了
// 函数名，函数类型，函数参数个数和类型全部与基类的虚函数相同，并根据派生类的需要重新定义
// 基类返回基类指针，派生类返回派生类指针是可以的，只要能强转
// 函数的入口出口都在基类, 函数体在基类

// Destructors should generally use virtual functions, while constructors cannot be marked as virtual functions
// When providing default values for virtual functions, try to keep them consistent
// 析构函数一般要使用虚函数，构造函数不能标为虚函数
// 虚函数给缺省值的时候尽量一样

// Pure virtual function, an interface reserved by the base class for derived classes
// 纯虚函数，基类给派生类留的接口

// virtual double area() const = 0;  // No need to define the function body 不用定义函数体

// This class is an abstract base class, which cannot define objects itself. It can only be used as a base class for other classes, and pointers to abstract base classes can be defined
// 这个类就是抽象基类，不能自己定义对象，只能给别的类作为基类，可以定义指向抽象基类的指针

int main()
{
    Circle cone(1, 2, 3, "cone");
    Square sone(3, 4, 5, "sone");
    Point *point;

    // The same pointer, the same calling form, calling functions of different derived classes
    // 同一个指针，相同的调用形式，调用不同的派生类的函数
    point = &cone;
    point->print(cout);

    point = &sone;
    point->print(cout);

    point->getme()->showType();

    return 0;
}
