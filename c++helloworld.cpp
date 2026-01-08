#include <iostream>
#include <string>
using namespace std;

// use class to define a structure
// 使用类来定义一个结构体
class MatterClass
{
    public:
    // member variables
    // 成员变量
    double quality;
    double length;
    double width;
    double height;

    // constructor and destructor
    // 构造函数和析构函数
    MatterClass(){quality = 0; length = 0; width = 0; height = 0;}
    ~MatterClass(){quality = 0; length = 0; width = 0; height = 0;}

    // member functions
    // 成员函数
    void set_quality(double quality_){quality = quality_;}
    void set_length(double length_){length = length_;}
    void set_width(double width_){width = width_;}
    void set_height(double height_){height = height_;}
};

// inheritance
// public继承
class StudentClass : public MatterClass
{
    public:
    // member variables
    // 成员变量
    int student_id;
    int student_age;
    string student_name;

    // constructor and destructor
    // 构造函数和析构函数
    StudentClass(){student_id = 2023000; student_age = 0; student_name = "NULL";}
    ~StudentClass(){student_id = 0; student_age = 0; student_name = "";}

    // member functions
    // 成员函数
    void set_id(int id){student_id = id;}
    void set_age(int age){student_age = age;}
    void set_name(string name){student_name = name;}

    // overload operator <<
    // 重载输出运算符 <<
    friend ostream &operator<<(ostream &out, StudentClass &s)
    {
        out << "stu_id:" << s.student_id << " stu_age:" << s.student_age << " stu_name:" << s.student_name;
        return out;
    }
};

int main()
{
    // create object
    // 创建对象
    StudentClass(Tom);

    // use cout to print
    // 使用 cout 打印
    cout << "Hello World!" << endl;

    // access member variables
    // 访问成员变量
    cout << Tom.student_id << endl;
    cout << Tom.student_name << endl;
    cout << Tom << endl;

    // access member functions
    // 访问成员函数
    Tom.set_id(2023001);
    Tom.set_age(20);
    Tom.set_name("Tom");

    // print again
    // 再次打印
    cout << Tom.student_id << endl;
    cout << Tom.student_name << endl;
    cout << Tom << endl;
    
    // delete object
    // 删除对象
    delete &Tom;
    return 0;
}