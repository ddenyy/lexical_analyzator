//
// Created by Denis Osipov on 25.09.2023.
//

#include "Hash_table.h"


Hash_table::Hash_table()
{
    buffer_size = DEFAULT_SIZE;
    size = 0;
    size_all_non_nullptr = 0;

    arr.resize(buffer_size);
    for (int i = 0; i < buffer_size; ++i) {
        arr[i] = nullptr;                             //заполняем nullptr - то есть если значение отсутствует,
    }                                                 // и никто раньше по этому адресу не обращался
}

Hash_table::~Hash_table()
{
    for(auto& item: arr)
        delete item;
    arr.clear();
}

// изменение размера хэш таблицы
void Hash_table::resize()
{
    int past_buffer_size = buffer_size;
    buffer_size *= 2;
    size_all_non_nullptr = 0;
    size = 0;

    vector<ptr_node> arr2 = vector<ptr_node> (buffer_size, nullptr);
    // по дефолту не только перекидывает значения которые были в массиве
    // но ещё и размеры массивов тоже свапает
    swap(arr, arr2);

    for (int i = 0; i < past_buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->isState())
        {
           // this->Add(arr2[i]->getValue()); // добавляем элементы в новый массив
        }

    }

    // удаление предыдущего массива
    for (int i = 0; i < past_buffer_size; ++i)
        if (arr2[i])
            delete arr2[i];

    arr2.clear();
    arr2.resize(0);
}

void Hash_table::rehash()
{
    size_all_non_nullptr = 0;
    size = 0;
    vector<ptr_node> arr2(buffer_size, nullptr);
    swap(arr, arr2);

    for (int i = 0; i < buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->isState())
            Add(arr2[i]->getValue());
    }
    // удаление предыдущего массива
    arr2.clear();
    arr2.resize(0);
}

int hash_function(string& s, int size_hash_table)
{
    int result = 0;
    const int hash_coef = 57;

}

bool Hash_table::Find(Token& value)
{
//    int indx = hash_function()
}



const int Hash_table::getDefaultSize() {
    return DEFAULT_SIZE;
}

const double Hash_table::getRehashSize() {
    return REHASH_SIZE;
}
