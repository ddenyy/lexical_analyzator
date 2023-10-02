//
// Created by Denis Osipov on 25.09.2023.
//
#pragma once
#include "../main/main.h"
#include "../Node/Node.h"
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
        ~Hash_table();

        int hash_function(string s, int size_hash_table);

        void resize();

        void rehash();

        bool Add(Token& value);
        bool Find(Token& value);

        const int getSize();

        const double getRehashSize();

};


#endif //YAMP_LABS_HASH_TABLE_H
