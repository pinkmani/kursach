#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include "hash_table.h"

// Загрузка данных из файла
void loadFromFile(HashTable &ht, const std::string &filename);

// Добавляет .txt к имени файла, если его нет
std::string addTxtExtension(const std::string &filename);

// Сохранение таблицы в файл
void exportToFile(const HashTable &ht, const std::string &filename);

#endif // FILE_OPERATIONS_H

