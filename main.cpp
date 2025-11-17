#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <variant>

using namespace std;
using KeyType = variant<int, double, string>;

bool parseLine(const string &line, KeyType &key, string &value);
bool isInteger(const string &s);
bool isDouble(const string &s);

struct KeyValuePair
{
    KeyType key;
    string value;
};

int main()
{
    string line;
    cout << "Введите строку в формате 'ключ значение'" << endl;
    getline(cin, line);

    return 0;
}

bool parseLine(const string &line, KeyType &key, string &value)
{
    istringstream iss(line);
    vector<string> tokens;
    string token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() < 2)
    {
        return false;
    }

    string key_str = tokens[0];
    value = tokens[1];

    for (size_t i = 1; i < tokens.size(); ++i)
    {
        if (i > 1)
        {
            value += " ";
        }
        value += tokens[i];
    }
}

bool isInteger(const string &s)
{
    if (s.empty())
    {
        return false;
    }

    size_t start = 0;
    if (s[0] == '-' || s[1] == '+')
    {
        start = 1;
    }

    if (start >= s.size())
    {
        return false;
    }
    for (size_t i = start; i < s.length(); ++i)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

bool isDouble(const string &s)
{
    if (s.empty())
        return false;

    istringstream iss(s);
    double value;
    iss >> value;

    // Число валидно, если:
    // - Оно успешно прочитано (!fail)
    // - И вся строка consumed (eof)
    return !iss.fail() && iss.eof();
}