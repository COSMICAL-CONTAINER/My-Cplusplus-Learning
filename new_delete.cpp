#include <iostream>
#include <stdlib.h>
using namespace std;

int main()
{
    // malloc only allocates memory, does not initialize
    // malloc只负责分配空间，不能初始化
    int* intPtr1 = (int *)malloc(sizeof(int));
    int* intPtr2 = (int *)malloc(sizeof(int) * 10);

    int* intPtr3 = new int;
    // the space allocated is initialized to 4, not 4 integers
    // 分配空间后初始化为4，而不是4个整形
    int* intPtr4 = new int(4);
    int* intPtr5 = new int(0);
    // c++11 new method, initialize to 0
    // c++11 新方法，初始化为0
    int* intPtr6 = new int();
    // malloc array, 10 int type all are 0
    // 分配数组,10个int型全是0
    int* intPtr7 = new int[10];

    // use free() to release memory allocated by malloc
    // 用free()来释放malloc申请的内存
    free(intPtr1);
    free(intPtr2);

    // use delete to release memory allocated by new
    // 用delete来释放new申请的内存
    // don't mix use delete and free
    // 不要混用delete和free
    // delete intPtr1;    // error: intPtr1 was allocated by malloc
    // delete [] intPtr2; // error: intPtr2 was allocated by malloc
    delete intPtr3;
    delete intPtr4;
    delete intPtr5;
    delete intPtr6;

    // delete array, must add [] to indicate it's an array
    // if not, only the first element will be destroyed
    // 销毁整个数组,如果不加就会销毁第一个数
    delete [] intPtr7; 

    // to be avoid dangling pointer, set pointer to nullptr after delete
    // 为了避免内存泄漏(占了也不用，也不给别人用)，分配在堆上的内存一定要手工释放
    // int* intPtr = new int;
    // *intPtr = 5;
    // intPtr = new int;
    // *intPtr = 15;
    
    // missing delete
    // 迷途指针
    // int* intPtr = new int;
    // *intPtr = 5;
    // delete intPtr;
    // *intPtr = 15;
    return 0;
}