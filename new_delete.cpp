#include <iostream>
#include <cstdlib> // for malloc/free
#include <memory>  // for smart pointers (unique_ptr)
using namespace std;

// ==========================================
// Helper Functions to Demonstrate Bad Habits
// 辅助函数：用于演示导致野指针的错误习惯
// ==========================================

// Case 2: Returning address of local variable
// 原因2：返回局部变量的指针
int* createLocalPtr() {
    int x = 10; // local variable, stored on stack
                // 局部变量，存储在栈上
    
    return &x;  // Error: x is destroyed when function ends
                // 错误：函数结束时 x 会被销毁，返回的指针指向无效内存
}

// Case 4: Deleting pointer passed as parameter
// 原因4：函数参数指针被释放
void deleteParam(int* ptr) {
    if (ptr != nullptr) {
        delete ptr; // Memory released here
                    // 内存主要在这里被释放
    }
}

int main()
{
    cout << "=== 1. Basic Allocation & Deallocation ===" << endl;
    
    // malloc only allocates memory, does not initialize
    // malloc只负责分配空间，不能初始化
    int* intPtr1 = (int *)malloc(sizeof(int));
    int* intPtr2 = (int *)malloc(sizeof(int) * 10);

    int* intPtr3 = new int;
    
    // the space allocated is initialized to 4, not 4 integers
    // 分配空间后初始化为4，而不是4个整形
    int* intPtr4 = new int(4);
    
    // c++11 new method, initialize to 0
    // c++11 新方法，初始化为0
    int* intPtr5 = new int(0);
    int* intPtr6 = new int(); // same as above (0)
    
    // malloc array, 10 int type all are 0 (note: new int[10] does not strictly guarantee 0 init without (), better use new int[10]())
    // 分配数组
    int* intPtr7 = new int[10];

    // use free() to release memory allocated by malloc
    // 用free()来释放malloc申请的内存
    free(intPtr1);
    free(intPtr2);

    // use delete to release memory allocated by new
    // 用delete来释放new申请的内存
    // don't mix use delete and free
    // 不要混用delete和free
    delete intPtr3;
    delete intPtr4;
    delete intPtr5;
    delete intPtr6;

    // delete array, must add [] to indicate it's an array
    // if not, only the first element might be destroyed (undefined behavior)
    // 销毁整个数组,如果不加就会导致未定义行为（通常只销毁第一个）
    delete [] intPtr7; 

    cout << "=== 2. Dangling Pointer (Wild Pointer) Examples ===" << endl;
    cout << "=== 2. 野指针示例 ===" << endl;

    // ---------------------------------------------------------
    // Cause 1: Pointer not set to null after delete
    // 原因1：释放后没有置空指针
    // ---------------------------------------------------------
    int* p1 = new int(100);
    delete p1; 
    // NOW p1 is a Dangling Pointer!
    // 此时 p1 成为野指针！
    // It still holds the address of the memory that was just freed.
    // 它仍然保存着刚刚被释放的内存地址。
    
    // cout << *p1 << endl; // DANGEROUS: Undefined behavior (Program crash or data corruption)
                            // 危险：未定义行为（程序崩溃或数据损坏）

    // Solution: Set to nullptr immediately
    // 解决方法：立即置为 nullptr
    p1 = nullptr; 


    // ---------------------------------------------------------
    // Cause 2: Returning pointer to local variable
    // 原因2：返回局部变量的指针
    // ---------------------------------------------------------
    int* p2 = createLocalPtr();
    // p2 is a Dangling Pointer immediately. The variable 'x' inside the function is dead.
    // p2 立即成为野指针。函数内部的变量 'x' 已经消亡。
    // cout << *p2 << endl; // Undefined behavior


    // ---------------------------------------------------------
    // Cause 3 & 4: Pointer released in function but caller doesn't know
    // 原因3 & 4：释放内存后没有调整指针 / 函数参数指针被释放
    // ---------------------------------------------------------
    int* p3 = new int(50);
    deleteParam(p3); // p3 is deleted inside this function
                     // p3 在函数内部被 delete 了
    
    // Back in main, p3 is now a Dangling Pointer
    // 回到 main 函数，p3 现在是野指针
    // delete p3; // Double Free Error! (Crashes program)
                  // 二次释放错误！（会导致程序崩溃）
    
    p3 = nullptr; // Manually fix local pointer if you know it's gone (hard to track)
                  // 手动置空（但在复杂的代码中很难追踪）


    cout << "=== 3. How to Avoid Dangling Pointers ===" << endl;
    cout << "=== 3. 如何避免野指针 ===" << endl;

    // Method 1: Always set to nullptr after delete
    // 方法1：在释放内存后将指针置为 nullptr
    int* safePtr = new int(20);
    delete safePtr;
    safePtr = nullptr; // Safe now
    
    if (safePtr != nullptr) {
        // This block will not execute, preventing crash
        // 这个代码块不会执行，防止崩溃
        cout << *safePtr << endl;
    }

    // Method 2: Avoid returning local variable addresses (See createLocalPtr above)
    // 方法2：避免返回局部变量的指针（见上面的 createLocalPtr）

    // Method 3: Use Smart Pointers (Best Practice)
    // 方法3：使用智能指针（最佳实践）
    {
        // unique_ptr automatically manages memory
        // unique_ptr 自动管理内存
        std::unique_ptr<int> smartPtr = std::make_unique<int>(99);
        
        cout << "Smart Pointer value: " << *smartPtr << endl;
        
        // No need to call delete. 
        // When smartPtr goes out of scope (at the closing brace), memory is released automatically.
        // 不需要调用 delete。
        // 当 smartPtr 离开作用域（遇到右花括号）时，内存会自动释放。
    } 
    // smartPtr is destroyed here, no chance of dangling pointer.
    // smartPtr 在这里销毁，没有产生野指针的机会。

    return 0;
}