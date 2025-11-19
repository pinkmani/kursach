    #include <iostream>
    #include <vector>
    #include <cstring>
    #include <sstream>
    #include <variant>
    #include <cmath>
    #include <cstdint>

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
        if (s[0] == '-' || s[0] == '+')
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

    int hashInt(int key, int tableSize)
    {
        return (abs(key * 2654435761) % tableSize);
    }

    int hashString(const string& key, int tableSize)
    {
        unsigned int hash = 0;
        for(char symbol: key)
        {
            hash = hash * 31 + symbol;
        }
        return hash % tableSize;
    }

    int hashDouble(double key, int tableSize)
    {
        if(isnan(key) || isinf(key)) //isnan - not a number; isinf - infinity
        {
            return 0;
        }

        uint64_t bits;
        memcpy(&bits, &key, sizeof(bits)); // Копируем битовое представление double как 64-битное целое

        bits ^= bits >> 32; // Перемешиваем старшие и младшие 32 бита, чтобы все биты влияли на хеш
        return (bits * 0x8e9d5abcUL) % tableSize; // Умножаем на константу для лучшего перемешивания
    }