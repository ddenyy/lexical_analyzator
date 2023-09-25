//
// Created by Denis Osipov on 25.09.2023.
//
#pragma once
#include "../main/main.h"
#include "../Token/Token.h"
#include "../Node/Node.h"

#ifndef YAMP_LABS_NODE_H
#define YAMP_LABS_NODE_H


class Node {
private:
        Token value;
        // если state == false значит эл-т был удален
        bool state;
public:
        // конструктор по умолчанию
        Node() : value(Token()), state(true) {};

        // Конструктор по ключу, изначально элемент не удалён
        Node(Token& _value): value(_value), state(true) {};

        // Конструктор по ключу и состоянию (удалён ли узел)
        Node(const Token &value, bool state) : value(value), state(state) {};

    Token &getValue() {
        return value;
    }

    void setValue(const Token &value) {
        Node::value = value;
    }

    bool isState()  {
        return state;
    }

    void setState(bool state) {
        Node::state = state;
    }
};


#endif //YAMP_LABS_NODE_H
