#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
    // cin and cout example
    int num;
    cout << "Please enter a number: ";
    cin >> num;
    cout << "You entered: " << num << endl;
    cin.clear(); // clear the input buffer

    // getline vs operator>>
    string name;
    cout << "Please enter your name: ";
    // enter "zhang san" will read completely
    getline(cin, name);
    cout << "Hello, " << name << "!" << endl;
    cout << "Please enter your name: ";
    // enter "zhang san" will read only "zhang"
    cin >> name;
    cout << "Hello, " << name << "!" << endl;
    // clear the input buffer
    cin.clear();

    // string stream line;
    string line1 = "Hello World";
    // create string stream object
    istringstream in(line1);
    string word;
    // read word by word
    in >> word;
    cout << word << endl;
    // read next word
    in >> word;
    cout << word << endl;
    // write to string stream
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

    // create and write to a file using ofstream
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