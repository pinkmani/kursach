#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <list>
#include <string>
#include <variant>
#include <iostream>

// Ключ может быть одним из трёх типов
using KeyType = std::variant<int, double, std::string>;

// Пара "ключ-значение"
struct KeyValuePair
{
    KeyType key;
    std::string value;
};

// Хеш-таблица
class HashTable
{
private:
    static const int INITIAL_SIZE = 11; // начальный размер таблицы
    std::vector<std::list<KeyValuePair>> table; // массив списков - каждый элемент массива это список пар
    int count = 0; // сколько всего элементов добавлено

    // Основная функция: вычисляет индекс в таблице по ключу
    int getHashIndex(const KeyType &key, int size) const;

public:
    // Конструктор: создаём таблицу с 11 пустыми списками
    HashTable();

    // Проверка на пустоту
    bool isEmpty() const;

    // Получить доступ к данным
    const std::vector<std::list<KeyValuePair>> &getTableData() const;

    // Вычисляет индекс в таблице по ключу (для текущего размера)
    int getHashIndex(const KeyType &key) const;

    // Увеличиваем таблицу, когда становится слишком много элементов
    void rehash();

    // Добавление пары "ключ-значение"
    void insert(const KeyType &key, const std::string &value);

    // Поиск значения по ключу
    bool search(const KeyType &key, std::string &value) const;

    // Удаление по ключу
    bool remove(const KeyType &key);

    // Вывод всей таблицы на экран или в файл
    void printTable(std::ostream &out) const;
};

#endif // HASH_TABLE_H

