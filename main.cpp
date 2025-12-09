#include <iostream>
#include <string>
#include <windows.h>
#include "hash_table.h"
#include "parser.h"
#include "file_operations.h"

using namespace std;

// ОСНОВНАЯ ПРОГРАММА
int main()
{
    // Включаем UTF-8 для корректного отображения кириллицы
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    HashTable ht;        // создаём таблицу
    bool running = true; // флаг для основного цикла
    string input;        // переменная для ввода пользователя

    // Основной цикл программы - меню
    while (running)
    {
        system("cls"); // очищаем экран

        // Выводим меню
        cout << "=== ХЕШ ТАБЛИЦА ===\n";
        cout << "Программа хранит пары \"ключ значение\" в хеш-таблице.\n";
        cout << "Ключ может быть числом (int/double) или словом на латинице.\n";
        cout << "Значение — одно слово на латинице. Данные можно вводить вручную\n";
        cout << "или загружать из текстового файла формата: ключ пробел значение (по строкам).\n\n";
        cout << "Выберите режим:\n";
        cout << "1. Ручной ввод данных\n";
        cout << "2. Загрузка из файла\n";
        cout << "3. Поиск по ключу\n";
        cout << "4. Удаление по ключу\n";
        cout << "5. Вывести таблицу\n";
        cout << "6. Сохранить в файл\n";
        cout << "0. Выход\n\n";

        if (ht.isEmpty())
        {
            cout << "Таблица пуста. Добавьте данные.\n\n";
        }

        cout << "Введите номер режима: ";
        getline(cin, input);

        // Ручной ввод
        if (input == "1")
        {
            system("cls");
            cout << "=== РЕЖИМ: РУЧНОЙ ВВОД ===\n";
            cout << "Формат: ключ значение (значение — одно слово)\n";
            cout << "Чтобы выйти, введите 0\n\n";

            bool inputting = true;
            while (inputting)
            {
                cout << "> ";
                getline(cin, input);

                if (input == "0")
                {
                    inputting = false;
                }
                else if (!input.empty())
                {
                    KeyType key;
                    string value;
                    if (parseLine(input, key, value))
                    {
                        // Проверяем, что если ключ - строка, то он на латинице
                        if (holds_alternative<string>(key))
                        {
                            string key_str = get<string>(key);
                            if (!isLatinWord(key_str))
                            {
                                cout << "Ошибка: ключ должен быть на латинице (a-z, A-Z, 0-9, '_', '-', '.')\n";
                                continue;
                            }
                        }

                        // Проверяем, что значение тоже только из латинских символов
                        if (!isLatinWord(value))
                        {
                            cout << "Ошибка: значение должно содержать только латинские буквы, цифры, '_', '-', '.'\n";
                            continue;
                        }

                        ht.insert(key, value);
                        cout << "Добавлено: ";
                        visit([](const auto &k)
                              { cout << k; }, key);
                        cout << " => " << value << "\n";
                    }
                    else
                    {
                        cout << "Ошибка: ожидается ровно два слова: 'ключ значение'\n";
                    }
                }
            }
        }

        // Загрузка из файла
        else if (input == "2")
        {
            system("cls");
            cout << "=== РЕЖИМ: ЗАГРУЗКА ИЗ ФАЙЛА ===\n";
            cout << "Формат каждой строки: ключ значение \n";
            cout << "Имя файла должно быть на латинице (например: data.txt)\n";
            cout << "Введите имя файла (.txt добавится автоматически): ";
            getline(cin, input);

            if (!input.empty())
            {
                // Проверка: только латиница, цифры и разрешённые символы
                bool valid = true;
                for (char c : input)
                {
                    if (!((c >= 'a' && c <= 'z') ||
                          (c >= 'A' && c <= 'Z') ||
                          (c >= '0' && c <= '9') ||
                          c == '_' || c == '-' || c == '.'))
                    {
                        valid = false;
                        break;
                    }
                }

                if (!valid)
                {
                    cout << "Ошибка: имя файла может содержать только латинские буквы, цифры, '_', '-', '.'\n";
                }
                else
                {
                    string filename = addTxtExtension(input);
                    loadFromFile(ht, filename);
                }
            }
            else
            {
                cout << "Имя файла не указано.\n";
            }
            cout << "Нажмите Enter для возврата...";
            getline(cin, input);
        }

        // Поиск
        else if (input == "3")
        {
            if (ht.isEmpty())
            {
                cout << "Таблица пуста. Сначала добавьте данные.\n";
                cout << "Нажмите Enter...";
                getline(cin, input);
            }
            else
            {
                system("cls");
                cout << "=== РЕЖИМ: ПОИСК ПО КЛЮЧУ ===\n";
                cout << "Введите ключ для поиска (0 — выход): \n";

                bool searching = true;
                while (searching)
                {
                    cout << "> ";
                    getline(cin, input);
                    if (input == "0")
                    {
                        searching = false;
                    }
                    else if (!input.empty())
                    {
                        // Парсим ключ (определяем тип: int, double или string) — важно для совпадения с типом, сохранённым в таблице
                        KeyType key;
                        if (!parseKey(input, key))
                        {
                            cout << "Ошибка: неверный формат ключа\n";
                            continue;
                        }

                        string result;
                        if (ht.search(key, result))
                        {
                            cout << "Найдено: ";
                            visit([](const auto &k)
                                  { cout << k; }, key);
                            cout << " => " << result << "\n";
                        }
                        else
                        {
                            cout << "Ключ не найден.\n";
                        }
                    }
                }
            }
        }

        // Удаление
        else if (input == "4")
        {
            if (ht.isEmpty())
            {
                cout << "Таблица пуста. Нечего удалять.\n";
                cout << "Нажмите Enter...";
                getline(cin, input);
            }
            else
            {
                system("cls");
                cout << "=== РЕЖИМ: УДАЛЕНИЕ ПО КЛЮЧУ ===\n";
                cout << "Введите ключ для удаления (0 — выход): \n";

                bool removing = true;
                while (removing)
                {
                    cout << "> ";
                    getline(cin, input);
                    if (input == "0")
                    {
                        removing = false;
                    }
                    else if (!input.empty())
                    {
                        // Парсим ключ (определяем тип: int, double или string)
                        KeyType key;
                        if (!parseKey(input, key))
                        {
                            cout << "Ошибка: неверный формат ключа\n";
                            continue;
                        }

                        if (ht.remove(key))
                        {
                            cout << "Удалено.\n";
                        }
                        else
                        {
                            cout << "Ключ не найден.\n";
                        }
                    }
                }
            }
        }

        // Вывод таблицы
        else if (input == "5")
        {
            if (ht.isEmpty())
            {
                cout << "Таблица пуста. Добавьте данные.\n";
                cout << "Нажмите Enter...";
                getline(cin, input);
            }
            else
            {
                system("cls");
                cout << "=== РЕЖИМ: ВЫВОД ТАБЛИЦЫ ===\n";
                ht.printTable(cout);
                cout << "Нажмите Enter для возврата...";
                getline(cin, input);
            }
        }

        // Сохранение в файл
        else if (input == "6")
        {
            if (ht.isEmpty())
            {
                cout << "Таблица пуста. Нечего сохранять.\n";
                cout << "Нажмите Enter...";
                getline(cin, input);
            }
            else
            {
                system("cls");
                cout << "=== РЕЖИМ: СОХРАНЕНИЕ В ФАЙЛ ===\n";
                cout << "Имя файла должно быть на латинице\n";
                cout << "Введите имя файла: ";
                getline(cin, input);

                if (input.empty())
                {
                    cout << "Имя файла не указано.\n";
                }
                else
                {
                    // Проверка имени файла
                    bool valid = true;
                    for (char c : input)
                    {
                        if (!((c >= 'a' && c <= 'z') ||
                              (c >= 'A' && c <= 'Z') ||
                              (c >= '0' && c <= '9') ||
                              c == '_' || c == '-' || c == '.'))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid)
                    {
                        cout << "Ошибка: имя файла может содержать только латинские буквы, цифры, а также символы '_', '-', '.'\n";
                    }
                    else
                    {
                        string filename = addTxtExtension(input);
                        exportToFile(ht, filename);
                        cout << "Данные сохранены в '" << filename << "'.\n";
                    }
                }
                cout << "Нажмите Enter для возврата...";
                getline(cin, input);
            }
        }

        // Выход
        else if (input == "0")
        {
            cout << "\nВыход из программы.\n";
            running = false;
        }

        // Неверный ввод
        else
        {
            cout << "\nВведите число от 0 до 6.\n";
            cout << "Нажмите Enter...";
            getline(cin, input);
        }
    }

    return 0;
}
