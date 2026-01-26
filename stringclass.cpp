#include <iostream>

#include <string>
using namespace std;
// string class is a class in c++ standard library, it is used to represent string
// to use string class, you need to include the header file <string> and use the namespace std
// string类是c++标准库中的一个类，用于表示字符串
// 使用string类需要包含头文件<string>并使用命名空间std

void swap(string &s1, string &s2)
{
    string temp = s1;
    s1 = s2;
    s2 = temp;
}

void sortString(string &s, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (s[j] > s[j + 1])
            {
                swap(s[j], s[j + 1]);
            }
        }
    }
}

int main()
{
    // char *pch = &'a';

    // Definition
    // 定义字符串
    string string1;
    string string2 = "china";
    string string3("china");
    string string4 = {"china"}; //c++11
    string string5{"china"};    //c++11
    char s1[] = "Hello";
    string s2(s1);
    string s3 = s1;
    string s4 = {s1};
    string s5{s1};              //c++11

    string str[3];


    // Assignment
    // 赋值
    string1 = "china";
    string2 = string1;
    string1[0] = 'C';

    // Concatenate
    // 拼接
    string1 = "The";
    string2 = "China";
    string3 = string1 + string2;
    string4 = string1 + "China";
    string5 = "The" + string2;
    string string6 = string1 + "China" + string2;

    // Compare
    // 比较
    if(string1 < string2)

    // Input and output
    // 输入输出
    // cout << string1 << endl;
    // cin >> string1;
    // cout << string1 << endl;

    cout << "Please input 3 strings: " << endl;
    cin >> str[0] >> str[1] >> str[2];
    sortString(str[0], 3);
    cout << "The sorted string is: " << endl;
    cout << str[0] << endl;
    cout << str[1] << endl;
    cout << str[2] << endl;

    return 0;
}