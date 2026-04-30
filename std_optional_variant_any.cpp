#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <vector>

using namespace std;

// std::optional / std::variant / std::any
// 目标: 掌握三种“可选/多态/动态”值容器的使用边界

optional<int> parse_positive_int(const string &s)
{
    try
    {
        int value = stoi(s);
        if (value > 0)
            return value;
        return nullopt;
    }
    catch (...)
    {
        return nullopt;
    }
}

using Message = variant<int, string, vector<int>>;

void print_message(const Message &msg)
{
    visit([](const auto &value)
          {
              using T = decay_t<decltype(value)>;
              if constexpr (is_same_v<T, int>)
                  cout << "variant<int>: " << value << '\n';
              else if constexpr (is_same_v<T, string>)
                  cout << "variant<string>: " << value << '\n';
              else
              {
                  cout << "variant<vector<int>>:";
                  for (int x : value)
                      cout << ' ' << x;
                  cout << '\n';
              } },
          msg);
}

int main()
{
    cout << "=== std::optional demo ===\n";
    for (const string &s : {"42", "0", "-3", "abc"})
    {
        optional<int> parsed = parse_positive_int(s);
        if (parsed)
            cout << "parse '" << s << "' -> " << *parsed << '\n';
        else
            cout << "parse '" << s << "' -> invalid or non-positive\n";
    }

    cout << "\n=== std::variant demo ===\n";
    Message m1 = 7;
    Message m2 = string("hello variant");
    Message m3 = vector<int>{1, 2, 3};
    print_message(m1);
    print_message(m2);
    print_message(m3);

    cout << "\n=== std::any demo ===\n";
    any dynamic = 3.14159;
    cout << "any holds type: " << dynamic.type().name() << '\n';
    try
    {
        cout << "as double: " << any_cast<double>(dynamic) << '\n';
        cout << "as int: " << any_cast<int>(dynamic) << '\n';
    }
    catch (const bad_any_cast &e)
    {
        cout << "bad_any_cast caught: " << e.what() << '\n';
    }

    dynamic = string("dynamic string");
    cout << "as string: " << any_cast<string>(dynamic) << '\n';

    return 0;
}
