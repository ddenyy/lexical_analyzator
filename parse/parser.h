#ifndef PARSER_H
#define PARSER_H

#include "../lexical_analizator/lexical_analizator.h"
#include "../Token/Token.h"
#include "../parse_tree/parse_tree.h"

class parser {
private:
    // Лексический анализатор для получения токенов из файла
    lexical_analyzer _tokens;
    // Дерево разбора
    parse_tree _parse_tree;

    /**
     * Метод выведет сообщение, если лексический анализатор не распознал токен
     * */
    void check_lexical_error(Token& curr);
    /**
     * Метод разбирает продукцию Begin
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool begin(parse_tree& begin_tree);
    /**
     * Метод разбирает продукцию Description
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool description(parse_tree& description_tree);
    /**
     * Метод разбирает продукцию Descr
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool small_descriptions(parse_tree& small_description_tree);
    /**
     * Метод разбирает продукцию Var list
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool var_list(parse_tree& var_list_tree);
    /**
     * Метод разбирает продукцию Operators
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool operators(parse_tree& operators_tree);
    /**
     * Метод разбирает продукцию Op
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool small_operators(parse_tree& small_operators_tree);
    /**
     * Метод разбирает продукцию Num expr
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool num_expr(parse_tree& num_expr_tree);
    /**
     * Метод разбирает продукцию Simple num expr
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool simple_num_expr(parse_tree& simple_num_expr_tree);
    /**
     * Метод разбирает продукцию String expr
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool string_expr(parse_tree& string_expr_tree);
    /**
     * Метод разбирает продукцию End
     * \return true если продукция корректно разобрана, false если нашлись ошибки
     * */
    bool end(parse_tree& end_tree);

public:
    /**
     * Метод печатает дерево в переданный поток вывода
     * \param out - поток для печати дерева
     * */
    void print_parse_tree(std::ostream& out) { _parse_tree.print(out); }
    /**
     * Метод возвращает дерево разбора
     * */
    parse_tree get_parse_tree() const { return _parse_tree; }
    /**
     * Метод возвращает хеш-таблицу с токенами
     * */
    Hash_table get_hash_table() { return _tokens.tokens(); }
    /**
     * Конструктор класса parser
     * \param file_name - файл для синтаксического анализа
     * */
    parser(const char* file_name) : _tokens(file_name), _parse_tree() {}
    /**
     * Метод запускает синтаксический анализ.
     * \return true если нет ошибок, false если есть ошибки
     * */
    bool run();
};


#endif // PARSER_H
