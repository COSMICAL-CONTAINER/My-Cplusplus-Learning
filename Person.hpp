#include <iostream>
using namespace std;

// Person class, used to store the name and age of a person
// Person类，用于存储人的姓名和年龄
struct Person
{
    string name;
    int age;
    Person(const string &n, int a) : name(n), age(a) {}
    
    // overload the less than operator
    // 重载小于运算符
    bool operator<(const Person &other) const
    {
        return age < other.age;
    }
    bool operator>(const Person& other) const {
        return age > other.age;
    }
    
    // overload the output operator
    // 重载输出运算符
    friend ostream &operator<<(ostream &os, const Person &p)
    {
        return os << p.name << "(" << p.age << ")";
    }
};