#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "hash_table.h"

// Парсинг строки: разбиваем на ключ и значение
// Формат:
//   1) "ключ значение"            - базовый вариант
//   2) "индекс ключ значение"     - индекс бакета (целое число) в начале, используется при экспорте
bool parseLine(const std::string &line, KeyType &key, std::string &value);

// Проверяем, является ли строка целым числом
bool isInteger(const std::string &s);

// Проверяем, является ли строка дробным числом
bool isDouble(const std::string &s);

// Проверяем, что строка содержит только допустимые латинские символы
// Разрешены: a-z, A-Z, 0-9, '_', '-', '.'
bool isLatinWord(const std::string &s);

// Парсит строку как ключ (определяет тип: int, double или string)
// Используется для поиска и удаления, когда пользователь вводит только ключ
bool parseKey(const std::string &input, KeyType &key);

#endif // PARSER_H

