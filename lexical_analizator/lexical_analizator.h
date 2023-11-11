//
// Created by Denis Osipov on 17.10.2023.
//

#ifndef YAMP_LABS_LEXICAL_ANALIZATOR_H
#define YAMP_LABS_LEXICAL_ANALIZATOR_H

#include "../main/main.h"
#include "../hash_table/Hash_table.h"
#include "../Token/Token.h"

struct position {
    // В элементе вектора с индексом i лежит количество символов в строке с номером i + 1.
    vector<int> _data;
    // Последнее прочитанное слово.
    string _last_word;
};

class lexical_analyzer {
private:
    // Поле хранит поток из которого необходимо читать токены
    std::stringstream _input;
    // Таблица со всеми прочитанными токенами
    Hash_table _tokens;
    // Текущая позиция
    position _pos;

    /**
     * Метод обновляет позицию в файле в зависимости от текущего символа
     * \param s - символ по которому обновляется позиция в файле
     * */
    void update_position(char s);
    /**
     * Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
     * \param s - символ для проверки
     * */
    bool is_separators(char s);
    /**
     * Метод проверяет, что символ является символом разделителем (односимвольной лексемой)
     * \param s - символ для проверки
     * */
    bool is_separators(string& s);
    /**
     * Метод проверяет, является ли слово ключевым (это слова int, char, return)
     * \param s - слово для проверки
     * */
    bool is_keyword(string& s);
    /**
     * Метод возвращает тип токена разделителя
     * \param s - слово для определения типа
     * */
    Token::Type_lexeme get_separator_type(string& s);
    /**
     * Метод возвращает тип ключевого слова
     * \param s - слово для определения типа
     * */
    Token::Type_lexeme get_keyword_type(string& s);
    /**
     * Метод получает следующее по тексту слово
     * */
    string get_next_word();
public:
    /**
     * Конструктор по имени файла. Файл переводится в поток stringstream
     * \param file_name - файл для чтения
     * */
    lexical_analyzer(string file_name);
    /**
     * Деструктор. В нем закрывается поток stringstream
     * */
    ~lexical_analyzer() { _input.clear(); }
    /**
     * Метод возвращает хеш-таблицу с токенами
     * */
    Hash_table tokens() const { return _tokens; }
    /**
     * Метод возвращает текущую позицию
     * */
    position position() const { return _pos; }
    /**
     * Метод возвращает следующий по тексту токен
     * */
    Token get_next_token();
    /**
     * Метод возвращает в поток последнее прочитанное слово
     * */
    void return_last_word();
    /**
     * Метод запускает лексический анализ по всему файлу. После его работы файл будет полностью
     * прочитан
     * */
    Hash_table get_all_tokens();
};




#endif //YAMP_LABS_LEXICAL_ANALIZATOR_H
