#include "file_operations.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <variant>

using namespace std;

// Загрузка данных из файла
void loadFromFile(HashTable &ht, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Не удалось открыть файл.\n";
        return;
    }

    // Сначала копим валидные строки, чтобы при любой ошибке не менять таблицу
    vector<pair<KeyType, string>> parsedData;
    string line;
    int lineNumber = 0;
    bool hasError = false;

    // Читаем файл построчно
    while (getline(file, line))
    {
        lineNumber++;

        // Удаляем пробелы в начале и конце строки
        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos)
        {
            cout << "Строка " << lineNumber << ": пустая или содержит только пробелы\n";
            hasError = true;
            continue;
        }
        size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);

        KeyType key;
        string value;

        if (!parseLine(line, key, value))
        {
            cout << "Строка " << lineNumber << ": неверный формат (ожидается 'ключ значение')\n";
            hasError = true;
        }
        else
        {
            bool validSymbols = true;

            // Проверка ключа (только если он строковый)
            if (holds_alternative<string>(key))
            {
                const string &key_str = get<string>(key);
                if (!isLatinWord(key_str))
                {
                    cout << "Строка " << lineNumber << ": ключ содержит недопустимые символы (допустимы только латинские буквы, цифры, '_', '-', '.')\n";
                    validSymbols = false;
                }
            }

            // Проверка значения
            if (!isLatinWord(value))
            {
                cout << "Строка " << lineNumber << ": значение содержит недопустимые символы (допустимы только латинские буквы, цифры, '_', '-', '.')\n";
                validSymbols = false;
            }

            if (!validSymbols)
            {
                hasError = true;
            }
            else
            {
                parsedData.push_back({key, value});
            }
        }
    }

    file.close();

    // Если были ошибки - ничего не добавляем (fail-fast, чтобы не смешивать корректные/некорректные данные)
    if (hasError)
    {
        cout << "\nФайл содержит ошибки. Ничего не будет добавлено.\n";
        cout << "Исправьте формат или выберите другой файл.\n";
        return;
    }

    // Все строки хорошие - добавляем в таблицу
    for (const auto &[key, value] : parsedData)
    {
        ht.insert(key, value);
    }

    cout << "Файл успешно загружен: " << parsedData.size() << " элементов добавлено.\n";
}

// Добавляет .txt к имени файла, если его нет
string addTxtExtension(const string &filename)
{
    if (filename.length() >= 4)
    {
        string ext = filename.substr(filename.length() - 4);
        if (ext == ".txt" || ext == ".TXT")
        {
            return filename;
        }
    }
    return filename + ".txt";
}

// Сохранение таблицы в файл
void exportToFile(const HashTable &ht, const string &filename)
{
    ofstream out(filename);
    if (!out.is_open())
    {
        cout << "Не удалось создать файл для записи.\n";
        return;
    }

    // Проходим по всем бакетам и парам
    const auto &data = ht.getTableData();
    for (size_t index = 0; index < data.size(); ++index)
    {
        const auto &bucket = data[index];
        for (const auto &pair : bucket)
        {
            // Сначала выводим индекс бакета,
            // а затем ключ и значение в формате: индекс ключ значение
            out << index << " ";
            // Выводим ключ
            visit([&](const auto &k)
                  { out << k << " "; }, pair.key);
            // Выводим значение
            out << pair.value << "\n";
        }
    }
    out.close();
}

