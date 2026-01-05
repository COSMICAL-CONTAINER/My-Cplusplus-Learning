#include <iostream>
#include <string>
using namespace std;

// use class to define a structure
class MatterClass
{
    public:
    // member variables
    double quality;
    double length;
    double width;
    double height;

    // constructor and destructor
    MatterClass(){quality = 0; length = 0; width = 0; height = 0;}
    ~MatterClass(){quality = 0; length = 0; width = 0; height = 0;}

    // member functions
    void set_quality(double quality_){quality = quality_;}
    void set_length(double length_){length = length_;}
    void set_width(double width_){width = width_;}
    void set_height(double height_){height = height_;}
};

// inheritance
class StudentClass : public MatterClass
{
    public:
    // member variables
    int student_id;
    int student_age;
    string student_name;

    // constructor and destructor
    StudentClass(){student_id = 2023000; student_age = 0; student_name = "NULL";}
    ~StudentClass(){student_id = 0; student_age = 0; student_name = "";}

    // member functions
    void set_id(int id){student_id = id;}
    void set_age(int age){student_age = age;}
    void set_name(string name){student_name = name;}

    // overload operator <<
    friend ostream &operator<<(ostream &out, StudentClass &s)
    {
        out << "stu_id:" << s.student_id << " stu_age:" << s.student_age << " stu_name:" << s.student_name;
        return out;
    }
};

int main()
{
    // create object
    StudentClass(Tom);

    // use cout to print
    cout << "Hello World!" << endl;

    // access member variables
    cout << Tom.student_id << endl;
    cout << Tom.student_name << endl;
    cout << Tom << endl;

    // access member functions
    Tom.set_id(2023001);
    Tom.set_age(20);
    Tom.set_name("Tom");

    // print again
    cout << Tom.student_id << endl;
    cout << Tom.student_name << endl;
    cout << Tom << endl;
    
    // delete object
    delete &Tom;

    // reference vs pointer
    const int const_int = 0;
    const int *const_int_ptr = &const_int;
    cout << "before:" << const_int << endl;
    *(int *)const_int_ptr = 1;
    cout << "after:" << const_int << endl;
    cout << "after:" << *const_int_ptr << endl;

    return 0;
}