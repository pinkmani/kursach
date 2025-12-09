#include "parser.h"
#include <sstream>
#include <cctype>
#include <vector>

using namespace std;

bool parseLine(const string &line, KeyType &key, string &value)
{
    istringstream iss(line); // превращаем строку в поток
    vector<string> tokens;
    string token;

    // Читаем слова по одному
    while (iss >> token)
    {
        tokens.push_back(token);
    }

    // Допустимо 2 или 3 "слова" в строке
    if (tokens.size() != 2 && tokens.size() != 3)
    {
        return false;
    }

    size_t keyIndex = 0;
    size_t valueIndex = 1;

    // Формат сохранённого файла: "индекс ключ значение" — индекс бакета пропускаем
    if (tokens.size() == 3 && isInteger(tokens[0]))
    {
        keyIndex = 1;
        valueIndex = 2;
    }

    string key_str = tokens[keyIndex];
    value = tokens[valueIndex]; // значение - соответствующее слово

    // Определяем, что за тип у ключа
    if (isInteger(key_str))
    {
        key = stoi(key_str); // целое число
    }
    else if (isDouble(key_str))
    {
        key = stod(key_str); // дробное число
    }
    else
    {
        key = key_str; // строка
    }
    return true;
}

bool isInteger(const string &s)
{
    if (s.empty())
        return false;
    size_t start = 0;
    if (s[0] == '-' || s[0] == '+')
        start = 1; // минус или плюс в начале - нормально
    if (start >= s.size())
        return false; // но если только знак - ошибка
    for (size_t i = start; i < s.length(); ++i)
        if (!isdigit(s[i]))
            return false; // если не цифра - не целое
    return true;
}

bool isDouble(const string &s)
{
    if (s.empty())
        return false;
    istringstream iss(s);
    double d;
    iss >> d;
    // Должно прочитаться число и вся строка должна быть обработана
    return !iss.fail() && iss.eof();
}

bool isLatinWord(const string &s)
{
    for (char c : s)
    {
        if (!((c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') ||
              c == '_' || c == '-' || c == '.' ||
              c == '+' || c == '=' || c == '*' ||
              c == '/' || c == '!' || c == '?' ||
              c == '@' || c == '#' || c == '$' ||
              c == '%' || c == '&'))
        {
            return false;
        }
    }
    return true;
}

// Парсит строку как ключ (определяет тип: int, double или string)
bool parseKey(const string &input, KeyType &key)
{
    if (input.empty())
        return false;

    // Сначала пробуем как целое число
    if (isInteger(input))
    {
        key = stoi(input);
        return true;
    }
    // Затем как дробное число
    else if (isDouble(input))
    {
        key = stod(input);
        return true;
    }
    // Иначе как строка (но проверяем на латиницу)
    else
    {
        if (isLatinWord(input)) // оставляем только латиницу, чтобы ключи были сопоставимы с файлами
        {
            key = input;
            return true;
        }
    }
    return false;
}

