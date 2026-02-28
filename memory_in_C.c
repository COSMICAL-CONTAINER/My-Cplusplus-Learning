#include <stdio.h>
#include <stdlib.h>

void GetMemory1(char *p)
{
    p = (char *)malloc(100);
}

void GetMemory2(char **p)
{
    *p = (char *)malloc(100);
}

char *GetMemory3(void)
{
    char p[] = "hello world";
    return p;
}

// Test1: program crashes. Because GetMemory1 cannot pass dynamic memory, str in Test1 function is always NULL.
// strcpy(str, "hello world") will cause the program to crash
// Test1: 程序崩溃。因为GetMemory1并不能传递动态内存，Test1函数中的str一直都是NULL。
// strcpy(str, "hello world")将使程序崩溃
void Test1(void)
{
    char *str = NULL;
    GetMemory1(str);
    strcpy(str, "hello world");
    printf(str);
}

// Test2: can output hello world, memory management is correct
// Test2: 能够输出hello world，内存管理正确
void Test2(void)
{
    char *str = NULL;
    GetMemory2(&str);
    strcpy(str, "hello world");
    printf(str);
    free(str);
}
// Test3: may be garbled. Because GetMemory3 returns a pointer to "stack memory", the address of the pointer is not NULL,
// but its original content has been cleared, and the new content is unknown
// Test3: 可能是乱码。因为GetMemory3返回的是指向"栈内存"的指针，该指针的地址不是NULL，
// 但其原现的内容已经被清除，新内容不可知
void Test3(void)
{
    char *str = NULL;
    str = GetMemory3();
}

// Test4: tampering with the content of the dynamic memory area, the consequences are unpredictable. Very dangerous. Because after free(str), str becomes a wild pointer,
// the if(str != NULL) statement does not
// Test4: 篡改动态内存区的内容，后果难以预料。非常危险。因为free(str)之后，str成为野指针，
// if(str != NULL)语句不起作用
void Test4(void)
{
    char *str = (char *)malloc(100);
    strcpy(str, "hello");
    free(str);
    if (str != NULL)
    {
        strcpy(str, "world");
        printf(str);
    }
}

// write strcpy
// key points analysis:
// 1. const modifier: the source string parameter is modified by const to prevent modification of the source string
// 2. null pointer check: does not check the validity of the pointer, indicating that the respondent does not pay attention to the robustness of the code
// 3. return destination address: forgetting to save the original dst value will lead to
// 4. '\0' handling: the loop written as while (*dst++ = *src++); is obviously wrong and needs to check '\0'
// 实现内存拷贝函数
// 关键点分析：
// 1. const修饰：源字符串参数用const修饰，防止修改源字符串
// 2. 空指针检查：不检查指针的有效性，说明答题者不注重代码的健壮性
// 3. 返回目标地址：忘记保存原始的dst值会导致错误
// 4. '\0'处理：循环写成while (*dst++ = *src++);明显是错误的，需要检查'\0'
char *strcpy(char *dst, const char *src)
{
    // void pointer check
    // 空指针检查
    assert(dst != NULL && src != NULL);
    // save original address
    // 保存原始地址
    char *ret = dst;
    // one by one character copy
    // 逐字符拷贝
    while ((*dst++ = *src++) != '\0')
        ; 
    // return destination address
    // 返回目标地址
    return ret;
}

// when considering memory overlap, the above implementation is incorrect
// because when dst and src overlap, copying from front to back may overwrite src data that has not been copied yet
// the correct approach is to check for overlap and copy from back to front if necessary
// 考虑内存重叠的strcpy实现
// 上面的实现是不正确的，因为当dst和src重叠时，从前往后拷贝可能会覆盖掉还未拷贝的src数据
// 正确的做法是检查重叠情况，必要时从后往前拷贝
char *my_memcpy(char *dst, const char *src, size_t n)
{
    if (dst == NULL || src == NULL)
        return NULL;

    char *ret = dst;

    // check for memory overlap
    // 检查内存重叠
    if (dst > src && dst < src + n)
    {
        // src and dst overlap, copy from back to front
        // 从后往前拷贝
        dst += n - 1;
        src += n - 1;
        while (n--)
        {
            *dst-- = *src--;
        }
    }
    else
    {
        // no overlap, copy from front to back
        // 从前往后拷贝
        while (n--)
        {
            *dst++ = *src++;
        }
    }

    return ret;
}

int main()
{
    // Test1();
    Test2();
    // Test3();
    // Test4();

    char str1[20] = "Hello, World!";
    char str2[20];

    printf("str1: %s\n", str1);
    printf("Before memcpy, str2: %s\n", str2);

    my_memcpy(str2, str1, 14);

    printf("After memcpy, str2: %s\n", str2);
    
    return 0;
}

