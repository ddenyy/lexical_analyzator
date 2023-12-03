//
// Created by Denis Osipov on 25.09.2023.
//
#pragma once
#include "../main/main.h"
#include "../Node/Node.h"
#include "../Token/Token.h"
#ifndef YAMP_LABS_HASH_TABLE_H
#define YAMP_LABS_HASH_TABLE_H

using ptr_node = Node*;

class Hash_table {
    private:

        // стандартный размер таблицы
        static const int DEFAULT_SIZE = 7;

        // коэффициент расширения таблицы (насколько сильны мы её заполнили)
        const double REHASH_SIZE = 0.75;

        vector<ptr_node> arr;       // соответственно в массиве будут хранится структуры Node*
        int size;                   // сколько элементов у нас сейчас в массиве (без учета deleted)


    public:

        // Конструктор по умолчанию
        Hash_table();

        // деструктор
        ~Hash_table() = default;

        size_t hash_function(string s, size_t size_hash_table);

        void resize();

        void rehash();

        bool Add(Token value);
        Token Find(string& value);

        int getSize();

        double getRehashSize();

        void print_table();

        void set_type(string& text, string& var_type);

    vector<pair<size_t, Token>> to_array() const;
};


#endif //YAMP_LABS_HASH_TABLE_H
