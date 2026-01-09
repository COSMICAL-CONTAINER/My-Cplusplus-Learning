#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
    // cin and cout example
    // cin和cout示例
    int num;
    cout << "Please enter a number: ";
    if (!(cin >> num))
    {
        // handle invalid input
        // 处理无效输入
        cerr << "Invalid number\n";
        cin.clear();
        string _discard;
        // throw away the invalid input
        // 丢弃无效输入
        getline(cin, _discard);
    }
    else
    {
        cout << "You entered: " << num << endl;
        // clear the newline left in the input buffer
        // 清除输入缓冲区中剩余的换行符
        string _discard;
        getline(cin, _discard);
    }

    // getline vs operator>>
    // getline 与 operator>>
    string name;
    cout << "Please enter your name: ";
    // getline reads the whole line (can include spaces), does not stop at first space
    // getline 读取整行（可以包含空格），不会只读到第一个空格前
    getline(cin, name);
    cout << "Hello, " << name << "!" << endl;

    cout << "Please enter your name: ";
    // operator>> reads a single word (stops at whitespace)
    // operator>> 读取一个单词（遇到空白停止）
    cin >> name;
    cout << "Hello, " << name << "!" << endl;
    // if you want to use getline again later, remember to discard the remaining newline
    // 如果后面还要用 getline，记得丢弃剩余换行
    string _discard2;
    getline(cin, _discard2);

    // string stream line;
    // 字符串流示例
    string line1 = "Hello World";
    // create string stream object
    // 创建字符串流对象
    istringstream in(line1);
    string word;
    // read word by word
    // 逐字读取
    in >> word;
    cout << word << endl;
    // read next word
    // 读取下一个单词
    in >> word;
    cout << word << endl;
    // write to string stream
    // 写入字符串流
    ostringstream out(line1);
    out << "Goodbye World";
    cout << line1 << endl;

    float f = 3.1415926;
    cout << "Default float: " << f << endl;
    cout.precision(4);
    cout << "Set precision to 4: " << f << endl;
    cout << fixed;
    cout << "Fixed float: " << f << endl;
    cout << scientific;
    cout << "Scientific float: " << f << endl;
    cout << defaultfloat;
    cout << "Default float again: " << f << endl;

    // create and write to a file using ofstream
    // 创建并写入文件示例
    string inputFile = "my.txt";
    string copyFile = "out.copy";
    ofstream ofs(inputFile);
    if (!ofs)
    {
        cerr << "Failed to open " << inputFile << " for writing\n";
    }
    else
    {
        ofs << "Hello file\n";
        ofs << "This is a test line with spaces\n";
        ofs << "123 456\n";
        ofs.close();
    }

    // read the file line by line using ifstream + getline
    // 逐行读取文件示例
    ifstream ifs(inputFile);
    if (!ifs)
    {
        cerr << "Failed to open " << inputFile << " for reading\n";
    }
    else
    {
        string line;
        cout << "Read lines:\n";
        while (getline(ifs, line))
        {
            cout << line << endl;
        }
        ifs.close();
    }

    // read the file word by word using operator>>
    // 逐字读取文件示例
    ifs.open(inputFile);
    if (!ifs)
    {
        cerr << "Failed to open " << inputFile << " for reading (word mode)\n";
    }
    else
    {
        string token;
        cout << "Read words:\n";
        while (ifs >> token)
        {
            cout << token << endl;
        }
        ifs.close();
    }

    // copy the file (binary-safe) using stream buffer
    // 使用流缓冲区复制文件（二进制安全）
    ifstream src(inputFile, ios::binary);
    ofstream dst(copyFile, ios::binary);
    if (!src || !dst)
    {
        cerr << "Failed to open files for copying\n";
    }
    else
    {
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
    return 0;
}