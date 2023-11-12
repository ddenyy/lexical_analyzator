#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../lexical_item/lexical_item.h"
#include "../Token/Token.h"
#include "../lexical_item/terminal.h"
#include <iostream>
#include <utility>
#include <vector>

/*
 * Класс дерева разбора
 * Дерево хранит в себе токены и терминалы в последовательности, соответствующей левому разбору строки
 * */

using ptr_lexi = std::shared_ptr<lexical_item>;
class parse_tree;

/**
 * Класс узла дерева разбора
 * \param _value - ссылка на интерфейс lexical_item
 * \param _children - вектор детей узла
 * */
class tree_node {
public:
    /**
     * Конструктор по умолчанию
     * Вектор с детьми будет создан пустым
     * */
    tree_node() : _value{nullptr}, _children{std::vector<std::shared_ptr<tree_node>>()} {}

    /**
     * Конструктор по терминалу
     * \param v - терминал, которым будет инициализировано поле _value
     * Вектор с детьми будет создан пустым
     * */
    tree_node(const terminal& v) :
            _value{std::static_pointer_cast<lexical_item>(std::make_shared<terminal>(v))},
            _children{std::vector<std::shared_ptr<tree_node>>()} {};

    /**
     * Конструктор по токену
     * \param v - токен, которым будет инициализировано поле _value
     * Вектор с детьми будет создан пустым
     * */
    tree_node(const Token& v) :
            _value{std::dynamic_pointer_cast<lexical_item>(std::make_shared<Token>(v))},
            _children{std::vector<std::shared_ptr<tree_node>>()} {};

    /**
     * Конструктор по ссылке на интерфейс lexical_item
     * \param v - ссылке на интерфейс, которой будет инициализировано поле _value
     * Вектор с детьми будет создан пустым
     * */
    tree_node(ptr_lexi v) :
            _value{std::move(v)},
            _children{std::vector<std::shared_ptr<tree_node>>()} {};

    /**
     * Конструктор копирования
     * Вектор детей будет скопирован
     * \param v - узел для копирования
     * */
    tree_node(const tree_node& v);

    /**
     * Конструктор по умолчанию
     * */
    ~tree_node() = default;

    // Класс дерева объявлен дружеским для доступа к полям узла
    friend class parse_tree;

private:
    ptr_lexi _value;
    std::vector<std::shared_ptr<tree_node>> _children;
};

using ptree_node = shared_ptr<tree_node>;

class parse_tree {
public:
    // Типы продукций в грамматике языка
    enum type_product {
        FUNCTION,
        BEGIN,
        END,
        FUNCTION_NAME,
        DESCRIPTION_1, DESCRIPTION_2,
        DESCR,
        TYPE_1, TYPE_2,
        VAR_LIST_1, VAR_LIST_2,
        OPERATORS_1, OPERATORS_2,
        OP_1, EXPR_1, EXPR_2, EXPR_3,
        TERM_1, TERM_2, TERM_3,
        SIMPLE_EXPR_1, SIMPLE_EXPR_2, SIMPLE_EXPR_3,
    };


    //По умолчанию корень дерева инициализируется терминалом FUNCTION
    parse_tree() : _root(make_shared<tree_node>(tree_node{terminal{terminal::FUNCTION}})) {}
    /**
     * Конструктор по заданному терминалу
     * \param root - терминал, которым будет инициализирован корень дерева
     * */
    parse_tree(const terminal& root) : _root(make_shared<tree_node>(tree_node{root})) {}
    /**
     * Конструктор копирования
     * \param v - дерево для копирования
     * */
    parse_tree(const parse_tree& v) { copy_tree(_root, v._root); }

    ~parse_tree() = default;

    /**
     * Метод чистит дерево, не трогая корень дерева
     * */
    void clear() { _root->_children.clear(); }

    void print(std::ostream& out) { print(out,_root, 0); }
    /**
     * Метод вставки дерева в поддерево, корень которого содержит переданный терминал. Вставка
     * осуществляется в самый левый терминал без детей.
     * Вставка возможна только к терминалам, потому что токены считаются листами дерева, вставка к
     * ни невозможна.
     * \param to_add - терминал без детей, на место которого будет вставлен корен вставляемого дерева
     * \param tree - дерево, которое надо вставить
     * */
    void insert_tree(const terminal& to_add, const parse_tree& tree) {
        insert_tree(to_add, tree, _root); }
    /**
     * Метод добавления дерева в поддерево, корень которого содержит переданный терминал. Добавление
     * осуществляется к самому левому терминалу без детей.
     * to_add - терминал без детей, к детям которого будет добавлено дерево
     * tree - дерево, которое надо добавить
     * */
    void add_tree(const terminal& to_add, const parse_tree& tree) {
        add_tree(to_add, tree, _root); }
    /**
     * Метод добавления токена к заданному терминалу без детей. Добавление осуществляется к самому
     * левому терминалу без детей.
     * \param to_add - терминал без детей, к которому будет добавлен токен
     * \param v - токен для добавления
     * */
    void add_token(const terminal& to_add, const Token& v) {
        add_token(to_add, v, _root); }
    /**
     * Метод добавления продукции к заданному терминалу без детей. Добавление осуществляется к
     * самому левому терминалу без детей.
     * \param to_add - терминал без детей, к которому будет добавлен токен
     * \param v - продукция для добавления
     * */
    void add_product(const terminal& to_add, const type_product& product) {
        add_product(to_add, product, _root); }

    // Класс семантического анализа объявлен дружественным, чтобы легче реализовать обход дерева
    friend class semantic;
private:
    ptree_node _root;

    /*
     * Все методы редактирования дерева являются рекурсивными, поэтому для каждого приватного метода
     * редактирования была создана его публичная обертка, не являющаяся рекурсивной
     * */

    /**
     * Метод печати дерева. Дерево будет напечатано в виде файлового каталога
     * \param out - поток std::ostream в который необходимо напечатать дерево
     * \param pos - указатель на корень поддерева, которое будет напечатано
     * \param level - уровень сдвига
     * */
    void print (std::ostream& out, const ptree_node& pos, size_t level);
    /**
     * Метод вставки дерева в поддерево, корень которого содержит переданный терминал. Вставка
     * осуществляется в самый левый терминал без детей.
     * Вставка возможна только к терминалам, потому что токены считаются листами дерева, вставка к
     * ним невозможна.
     * \param to_add - терминал без детей, на место которого будет вставлен корен вставляемого дерева
     * \param ree - дерево, которое надо вставить
     * \param pos - указатель на корень поддерева, в которое пытаемся вставить
     * */
    void insert_tree(const terminal& to_add, const parse_tree& tree, ptree_node& pos);
    /**
     * Метод добавления дерева в поддерево, корень которого содержит переданный терминал. Добавление
     * осуществляется к самому левому терминалу без детей.
     * \param to_add - терминал без детей, к детям которого будет добавлено дерево
     * \param tree - дерево, которое надо добавить
     * \param pos - указатель на корень поддерева, к которому пытаемся добавить
     * */
    void add_tree(const terminal& to_add, const parse_tree& tree, const ptree_node& pos);
    /**
     * Метод копирует поддерево с корнем from в поддерево с корнем to, при этом корень поддерева to
     * становится равным корню поддерева from
     * \param to - поддерево, в которое осуществляется копирование
     * \param from - поддерево, из которого осуществляется копирование
     * */
    void copy_tree(ptree_node& to, const ptree_node& from);
    /**
     * Метод добавления терминала к заданному терминалу без детей. Добавление осуществляется к
     * самому левому терминалу без детей.
     * \param to_add - терминал без детей, к которому будет добавлен токен
     * \param v - терминал для добавления
     * \param pos - указатель на корень поддерева, к которому пытаемся добавить
     * \return true если удалось добавить и false если не удалось
     * */
    bool add_token(const terminal& to_add, const Token& v, ptree_node& pos);
    /**
     * Метод добавления токена к заданному терминалу без детей. Добавление осуществляется к самому
     * левому терминалу без детей.
     * \param to_add - терминал без детей, к которому будет добавлен токен
     * \param v - токен для добавления
     * \param pos - указатель на корень поддерева, к которому пытаемся добавить
     * \return true если удалось добавить и false если не удалось
     * */
    bool add_product(const terminal& to_add, type_product product, ptree_node& pos);
    /**
     * Метод добавляет в конец переданного вектора узлы, инициализированные всеми терминалами и
     * токенами, в которые переходим переданная продукция. Терминалы и токены добавляются в порядке,
     * соответствующему их порядку в продукции
     * \param children - вектор, в который будут добавлены териналы и токены
     * \param product - продукция
     * */
    void push_product_item(std::vector<std::shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_function(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_begin(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_end(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_function_name(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_description_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_description_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_descr(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_type_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_type_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_list_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_list_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_operators_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_operators_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_op_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_3(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_term_1(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);

    void push_product_item_term_2(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);

    void push_product_item_term_3(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);

    void push_product_item_simple_expr_1(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);

    void push_product_item_simple_expr_2(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);

    void push_product_item_simple_expr_3(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product);
};


#endif // PARSE_TREE_H
