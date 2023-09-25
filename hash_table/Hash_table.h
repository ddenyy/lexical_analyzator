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

        static const int DEFAULT_SIZE = 1000;
        const double REHASH_SIZE = 0.75;

        vector<ptr_node> arr;       // соответственно в массиве будут хранится структуры Node*
        int size;                   // сколько элементов у нас сейчас в массиве (без учета deleted)
        int buffer_size;            // размер самого массива, сколько памяти выделено под хранение нашей таблицы
        int size_all_non_nullptr;   // сколько элементов у нас сейчас в массиве (с учетом deleted)

    public:

        Hash_table();

        ~Hash_table();

        void resize();

        void rehash();

        //int hash_function(string& s, const int buffer_size);

        bool Add(Token& value);
        bool Find(Token& value);

        static const int getDefaultSize();

        const double getRehashSize();

};


#endif //YAMP_LABS_HASH_TABLE_H
