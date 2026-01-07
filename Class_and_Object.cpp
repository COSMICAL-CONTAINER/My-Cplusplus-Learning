#include <iostream>
using namespace std;

struct struct_Complex
{
    public:
    double rel;
    double img;

    struct_Complex(double r, double i)
    {
        rel = r;
        img = i;
    }
    ~struct_Complex(){}
    void print()
    {
        cout<<"("<<rel<<","<<img<<")";
    }

    struct_Complex sum(struct_Complex a, struct_Complex b)
    {
        struct_Complex c(a.rel, a.img);
        c.rel += b.rel;
        c.img += b.img;
        return c;
    }

    struct_Complex &add(struct_Complex &a, struct_Complex &b)
    {
        a.rel + b.rel;
        a.img + b.img;
        return a;
    }
};


// struct all things are public by default
// class all things are private by default
class class_Complex
{
    public:
    void print(/*Complex *p = this*/) 
    {
        cout<<"(" << real << ","<< ima <<")" << endl;
    }

    // because add function will change the current object, so no const for it
    class_Complex &add(const class_Complex &b);


    double real;
    double ima;
};

// :: is the scope resolution operator
// define the add function outside the class
class_Complex & class_Complex::add(const class_Complex &b)
{
    real += b.real;
    ima += b.ima;
    return (*this);
}

int main()
{
    struct_Complex sc1(1,2);
    struct_Complex sc2(3,4);
    sc1.print();
    struct_Complex sc3 = sc1.sum(sc1, sc2);
    sc3.print();
    cout<<endl;

    class_Complex one, two;
    one.real = 1;
    one.ima = 2;

    two.real = 3;
    two.ima = 4;

    one.print();
    two.print();

    one.add(two).print();
    one.add(two).add(two).print();

    return 0;
}