#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include "lexical_item.h"

/**
 * Класс терминала. Наследуется от интерфейса lexical_item
 * \param _type - тип терминала
 * */
class terminal : public lexical_item {
public:
    // Типы терминалов
    enum type_terminal {
        FUNCTION, BEGIN, END, FUNCTION_NAME,
        DESCRIPTION, DESCR,
        TYPE, VAR_LIST,
        OPERATORS, OP,
        ID, CONST,
        TERM, EXPR,
        LET_NUM,INT_NUMBER,FLOAT_NUMBER, SIMPLE_EXPR
    };
    /**
     * Конструктор по умолчанию. Терминал инициализируется типом FUNCTION
     * */
    terminal() : _type(FUNCTION) {}
    /**
     * Конструктор по типу терминала
     * */
    terminal(type_terminal type) : _type(type) {}
    /**
     * Конструктор копирования
     * */
    terminal(const terminal& v) : _type(v._type) {}
    /**
     * Метод возвращает тип терминала
     * */
    type_terminal type() const { return _type; }

    /**
     * Реализация метода из интерфейса lexical_item
     * Метод возвращает имя класса
     * \return "terminal"
     * */
    std::string class_name() const override { return {"terminal"}; }

    friend std::ostream& operator<<(std::ostream& out, const terminal& v);

private:
    type_terminal _type;
};

#endif // TERMINAL_H
