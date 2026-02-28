#include <iostream>
using namespace std;

//               ---  Student class     ---
// Human class ---                        --- Student leader category class
//              --- Cadre category class ---
// It can be seen that to construct a student cadre class, both a student class and a cadre class need to be constructed, and at this point, both classes will construct a human class.
// We only need one human, so we need to add 'virtual' to indicate a virtual base class

//       ---学生类---
// 人类 ---        --- 学生干部类
//       ---干部类---
// 可以看到要构造学生干部类，需要构造学生类和干部类，此时两个类都会构造人类，
// 我们只需要一个人类，故需要加上virtual表示虚基类

class People
{
public:
    People(){};
    People(string name, int age):name(name), age(age) {};
    ~People(){};
protected:
    string name;
    int age;
};

class Student:virtual public People
{
public:
    Student(){};
    Student(string name, int age, string number):People(name, age), number(number) {};
    ~Student(){};
protected:
    string number;
};

class Leader:virtual public People
{
public:
    Leader(){};
    Leader(string name, int age, string post):People(name, age), post(post) {};
    ~Leader(){};
protected:
    // position
    // 职务
    string post;
};

class SLeader:public Student,public Leader
{
public:
    SLeader(){};
    SLeader(string name, string number, string post, int age):Student(name, age, number), Leader(name, age, post){};
    ~SLeader(){};

    void show()
    {
        cout << "name:" << name << " age:" << age << " number:" << number << " post:" << post << endl;
    }
};

int main()
{
    SLeader SLone("Lisi", "12345", "League Branch Secretary", 21);
    SLone.show();
    return 0;
}