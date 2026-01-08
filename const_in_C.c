#include <stdio.h>

// #define GLOBAL

// If GLOBAL is defined, const_int and const_int_ptr are global variables
#ifdef GLOBAL
const int const_int = 0;
const int *const_int_ptr = &const_int;
#endif 

int main()
{
    // If GLOBAL is not defined, const_int and const_int_ptr are local variables
    #ifndef GLOBAL
        const int const_int = 0;
        const int *const_int_ptr = &const_int;
    #endif

    // Print the value before modification
    printf("before:\n");
    printf("const_int:\t%d\n", const_int);
    printf("&const_int:\t%p\n", &const_int);
    printf("const_int_ptr:\t%p\n", const_int_ptr);
    printf("*(int *)const_int_ptr: \t%d\n", *(int *)const_int_ptr);
    printf("*const_int_ptr: \t%d\n", *const_int_ptr);

    // Modify the value using pointer casting
    // use int * to cast away constness and change the value
    *(int *)const_int_ptr = 1;

    // Print the value after modification
    // if GLOBAL is defined, const_int remains 0 or it just errors
    // if GLOBAL is not defined, const_int becomes 1
    printf("after:\n");
    printf("const_int:\t%d\n", const_int);
    printf("&const_int:\t%p\n", &const_int);
    printf("const_int_ptr:\t%p\n", const_int_ptr);
    printf("*(int *)const_int_ptr: \t%d\n", *(int *)const_int_ptr);
    printf("*const_int_ptr: \t%d\n", *const_int_ptr);

    return 0;
}