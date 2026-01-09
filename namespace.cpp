#include <iostream>
using namespace std;

// namespace is used to solve the problem of name conflicts in large projects
// all the variables and functions defined in a namespace are scoped to that namespace
// 命名空间用于解决大型项目中的命名冲突问题
// 命名空间中定义的所有变量和函数都作用于该命名空间
namespace 
{
    // null namespace only for this file
    // 空命名空间仅用于此文件
    int ns0;
}

// namespace can be nested defined
// 命名空间可以嵌套定义
namespace ns1
{
    int a;
    float ns1;
    void swap(int c, int d)
    {
        cout << "ns1 swap()" << endl;
    }
    namespace ns2
    {
        int a, b;
        // double ns3; // error : ns3 is not a variable
        void swap(int c, int d)
        {
            cout << "ns2 swap()" << endl;
        }
        namespace ns3
        {
            char ch;
            int a;
            void swap(int e, int f) 
            {
                cout << "ns3 swap()" << endl;
            }
        } // namespace ns3
        
    } // namespace ns2
    

} // namespace ns1

// can become global only for some variable or function of a namespace
// but can still be called using namespace
// 可以只让某个命名空间的某个变量或函数变成全局的,但是还可以使用命名空间调用
using ns1::ns2::ns3::ch;
using ns1::ns2::ns3::swap;

int main()
{
    ns1::a = 1;
    ns1::ns1 = 3.14;
    ns1::ns2::a = 2;
    // ns1::ns2::ns3 = 1.414; //error : ns3 is not a variable
    ns1::swap(1, 2);
    ns1::ns2::swap(1, 3);
    ch = 'a';
    ns1::ns2::ns3::ch = 'b';
    swap(1, 0);
    return 0;
}