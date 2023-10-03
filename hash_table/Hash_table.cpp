//
// Created by Denis Osipov on 25.09.2023.
//

#include "Hash_table.h"


Hash_table::Hash_table()
{
    size = DEFAULT_SIZE;
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = nullptr;                             //заполняем nullptr - то есть если значение отсутствует,
    }                                                 // и никто раньше по этому адресу не обращался
}

Hash_table::~Hash_table()
{
    for(auto& item: arr)
        delete item;
    arr.clear();
}


int Hash_table::hash_function(string s, int size_hash_table)
{
    int hash_result = 0;
    int hash_coefficient = 57;
    for (char i : s) {
        hash_result = (hash_coefficient * hash_result + i) % size_hash_table;
    }
    hash_result = (hash_result * 2 + 1) % size_hash_table;
    return hash_result;
}


bool Hash_table::Add(Token& value) {
    // Проверяем, не нужно ли делать ре-хеширование
    if (REHASH_SIZE <= (1.0*size / arr.size())) {
        rehash();
    }

    int index = hash_function(value.getText(), arr.size());
    // Ищем место, куда можно вставить элемент.
    // Это должен быть либо пустой, либо удаленный узел

    cout << arr[index] << ' ' << '\n';

    while (arr[index] != nullptr && arr[index]->isState()) {
        // Так как хеш-таблица не допускает повторов, то выходим, как только нашли такой же элемент
        if (arr[index]->getValue().isEqual(value)) {
            return false;
        }
        index +=1;
        index %= arr.size();
    }
    size++;
    // Вставка происходить по-разному: если это пустой узел, то просто создаем новый, а если это
    // удаленный узел, то заменяем значение в нем и поднимаем флаг
    if (arr[index] == nullptr) {
        arr[index] = new Node(value, true);
    } else {
        arr[index]->setValue(value);
        arr[index]->setState(true);
    }
    return true;
}


// изменение размера хэш таблицы
void Hash_table::resize()
{
    int past_buffer_size = size;
    size = 0;

    vector<ptr_node> arr2 = vector<ptr_node> (size*2, nullptr);
    // По дефолту не только перекидывает, значения, которые были в массиве.
    // Но ещё и размеры массивов тоже меняет
    swap(arr, arr2);

    for (int i = 0; i < past_buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->isState())
        {
           this->Add(arr2[i]->getValue()); // добавляем элементы в новый массив
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
    size = 0;
    vector<ptr_node> arr2(arr);
    fill(all(arr), nullptr);
    arr.resize(arr.size()*2, nullptr);

    for(auto i: arr2)
    {
        if (i != nullptr && i->isState())
        {
            Add(i->getValue());
        }
    }

    // удаление предыдущего массива
    arr2.clear();
    arr2.resize(0);
}


bool Hash_table::Find(Token& value)
{
//    int index = hash_function()
    return true;
}


void print_table()
{

}


const int Hash_table::getSize() {
    return size;
}

const double Hash_table::getRehashSize() {
    return REHASH_SIZE;
}
