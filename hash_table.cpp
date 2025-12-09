#include "hash_table.h"
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>

using namespace std;

// Константы для хеш-функций
const unsigned int GOLDEN_RATIO_32 = 2654435761U;
const unsigned long MIXING_CONSTANT_DOUBLE = 0x8e9d5abcUL;
const unsigned int POLYNOMIAL_BASE = 31;

HashTable::HashTable() : table(INITIAL_SIZE) {}

bool HashTable::isEmpty() const
{
    return count == 0;
}

const vector<list<KeyValuePair>> &HashTable::getTableData() const
{
    return table;
}

int HashTable::getHashIndex(const KeyType &key, int size) const
{
    if (holds_alternative<int>(key))
    {
        // Если ключ — целое число
        int k = get<int>(key);
        unsigned int hash = static_cast<unsigned int>(k);
        return (hash * GOLDEN_RATIO_32) % size; // умножаем на золотое число и берём остаток
    }
    else if (holds_alternative<double>(key))
    {
        // Если ключ - дробное число
        double k = get<double>(key);
        if (isnan(k) || isinf(k))
            return 0; // если это не число или бесконечность - не хешируем

        // Берём битовое представление числа как целое
        uint64_t bits;
        memcpy(&bits, &k, sizeof(bits));
        bits ^= bits >> 32; // XOR с верхними битами — чтобы лучше перемешать
        return (bits * MIXING_CONSTANT_DOUBLE) % size;
    }
    else if (holds_alternative<string>(key))
    {
        // Если ключ = строка
        const string &k = get<string>(key);
        unsigned int hash = 0;
        for (char c : k)
        {
            // Полиномиальное хеширование: hash = hash * 31 + буква
            hash = hash * POLYNOMIAL_BASE + static_cast<unsigned char>(c);
        }
        return hash % size;
    }
    return 0;
}

int HashTable::getHashIndex(const KeyType &key) const
{
    return getHashIndex(key, table.size());
}

void HashTable::rehash()
{
    int newSize = table.size() * 2 + 1; // делаем примерно в 2 раза больше
    vector<list<KeyValuePair>> newTable(newSize);

    // Переносим все элементы в новую таблицу
    for (auto &bucket : table)
    {
        for (auto &pair : bucket)
        {
            int newIndex = getHashIndex(pair.key, newSize);
            newTable[newIndex].push_back(move(pair)); // переносим пару
        }
    }

    table = move(newTable); // заменяем старую таблицу на новую
    cout << "Рехеш: размер увеличен до " << newSize << "\n";
}

void HashTable::insert(const KeyType &key, const string &value)
{
    // Если таблица заполнена больше чем на 70% - увеличиваем её
    if (count >= 0.7 * table.size())
    {
        rehash();
    }

    int index = getHashIndex(key); // вычисляем, в какой бакет попадёт ключ
    auto &bucket = table[index];   // получаем ссылку на список в этом бакете

    // Сначала проверяем, нет ли уже такого ключа
    for (auto &pair : bucket)
    {
        if (pair.key == key)
        {
            pair.value = value; // если есть - просто обновляем значение
            return;
        }
    }

    // Если ключа нет - добавляем новую пару в конец списка
    bucket.push_back({key, value});
    count++;
}

bool HashTable::search(const KeyType &key, string &value) const
{
    int index = getHashIndex(key);
    const auto &bucket = table[index];

    // Ищем в списке по ключу
    for (const auto &pair : bucket)
    {
        if (pair.key == key)
        {
            value = pair.value; // нашли - записываем значение
            return true;
        }
    }
    return false; // не нашли
}

bool HashTable::remove(const KeyType &key)
{
    int index = getHashIndex(key);
    auto &bucket = table[index];

    // Проходим по списку, чтобы можно было удалить элемент
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
        if (it->key == key)
        {
            bucket.erase(it); // удаляем из списка
            count--;
            return true;
        }
    }
    return false;
}

void HashTable::printTable(ostream &out) const
{
    out << "\n=== ХЕШ ТАБЛИЦА ===\n";
    for (size_t i = 0; i < table.size(); ++i)
    {
        out << "[" << i << "]: "; // номер бакета
        bool first = true;
        for (const auto &pair : table[i])
        {
            if (!first)
                out << ", ";
            // Выводим ключ (visit подбирает правильный тип)
            visit([&](const auto &k)
                  { out << k; }, pair.key);
            out << " => " << pair.value; // ключ => значение
            first = false;
        }
        out << "\n"; // переходим к следующему бакету
    }
    out << "==================\n\n";
}

