#ifndef LEXICAL_ITEM_H
#define LEXICAL_ITEM_H

#include <iostream>
#include <string>

/**
 * Класс интерфейс лексического элемента. Лексический элемент - это одна лексическая единица (токен
 * или терминал). Объявляет внутри себя виртуальный метод class_name() для получения имени класса,
 * который лежит в интерфейсе.
 * */
class lexical_item {
public:
    virtual std::string class_name() const = 0;
};

#endif // LEXICAL_ITEM_H
