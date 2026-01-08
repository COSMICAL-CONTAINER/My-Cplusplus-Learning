#include <iostream>
using namespace std;

// if we need to pass a variable but not change it, use const reference

// #define GLOBAL

// If GLOBAL is defined, const_int and const_int_ptr are global variables
#ifdef GLOBAL
const int const_int = 0;
const int *const_int_ptr = &const_int;
#endif 

int main()
{
    // comst qualifier
    // If GLOBAL is not defined, const_int and const_int_ptr are local variables
    #ifndef GLOBAL
        const int const_int = 0;
        const int *const_int_ptr = &const_int;
    #endif

    // print before modification
    cout << "try to change const variable through cast" << endl;
    cout << "before:" << endl;
    cout << "const_int:\t" << const_int << endl;
    cout << "&const_int:\t" << &const_int << endl;
    cout << "const_int_ptr:\t" << const_int_ptr << endl;
    cout << "*(int *)const_int_ptr: \t" << *(int *)const_int_ptr << endl;
    cout << "*const_int_ptr: \t" << *const_int_ptr << endl;

    // apply pointer casting to modify the value
    cout << "\ndo *((int*)(const_int_ptr)) = 1\n" << endl;
    *((int*)(const_int_ptr)) = 1;

    // error static_cast cannot cast away constness
    // *(static_cast<int *>(const_int_ptr)) = 1; 

    // print after modification
    // in c++, const_int still 0, cannot be changed
    cout << "after:" << endl;
    cout << "const_int:\t" << const_int << endl;
    cout << "&const_int:\t" << &const_int << endl;
    cout << "const_int_ptr:\t" << const_int_ptr << endl;
    // but through the pointer, the value is changed to 1
    cout << "*(int *)const_int_ptr: \t" << *(int *)const_int_ptr << endl;
    cout << "*const_int_ptr: \t" << *const_int_ptr << endl;

    // normal variable and const Pointer
    int i, j;
    // i is modifiable, but the value pointed to by intPtr1 is not modifiable
    // *intPtr1 is read-only
    // *intPtr1 = 42; // error
    const int* intPtr1 = &i;
    // intPtr2 is same as intPtr1
    int const* intPtr2 = &i;
    intPtr1 = &j;     // ok

    // when a pointer is declared as const, it must be initialized at the time of declaration
    // int *const intPtr0; // error
    // intPtr3 is modifiable, but the pointer intPtr2 itself is not modifiable
    // intPtr3 = &j; // error
    int *const intPtr3 = &i;
    *intPtr3 = 42;   // ok

    // const Pointer to const variable
    // both the pointer and the value pointed to are not modifiable
    // *intPtr4 = 43; // error
    // intPtr4 = &j;  // error
    const int *const intPtr4 = &i;

    // const variable and const Pointer
    const int k = 0;
    // int *intPtr4 = &k;          // error
    // int *const intPtr5 = &k;    // error
    const int *intPtr6 = &k;       // ok
    int const *intPtr7 = &k;       // ok
    const int *const intPtr8 = &k; // ok
    int const *const intPtr9 = &k; // ok

    // const variable and Reference
    const int b = 0;
    const int &a = b;
    // error: assignment of read-only location ‘a’
    // a = 3;

    // error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // int &c = b;

    cout << "\ntry to change const variable through reference cast" << endl;
    cout << "b: " << b << endl;
    // use cast to change the value of const variable
    int &c = (int &)b;
    // try to change the value of b through c
    c = 1;
    cout << "\ndo \nint &c = (int &)b;\nc = 1;\n" << endl;
    // b is still 0
    cout << "b: " << b << endl;
    cout << "c: " << c << endl;

    i = 43;
    const int &r1 = i;
    const int &r2 = 42;
    const int &r3 = r1 * 2;
    i = 100;
    // i is changed, r1 reflects the change
    cout << i << endl;
    cout << r1 << endl;
    // r2 and r3 are not changed
    cout << r2 << endl;
    cout << r3 << endl;

    double d_i = 42.5;
    // reference and the referenced are different types, must use const
    // following example is casted, a temporary variable is created
    const int &dr1 = d_i;
    d_i = 43.5;
    cout << d_i << endl;
    // still the old values 42.5
    cout << dr1 << endl;

    // const in function parameters
    int int_a = 1;
    double double_b = 1.23;
    int multi1(const int &x);
    int multi2(const int x);

    cout << multi1(int_a) << endl;
    cout << multi1(double_b) << endl;
    cout << multi1(1) << endl;

    cout << multi2(int_a) << endl;
    cout << multi2(double_b) << endl;
    cout << multi2(1) << endl;

    return 0;
}

int multi1(const int &x)
{
    return 2 * x;
}

int multi2(const int x)
{
    // error: const variable 'x' cannot be assigned
    // x = 2 * x;
    return x;
}

void swap(const int *x, const int *y)
{
    int z;
    z = *x;
    // error
    // *x = *y;
    // *y = z;
}

int *fun(int *const f)
{
    // error
    // f = malloc();
    return f;
}
