//
// Created by Denis Osipov on 11.11.2023.
//

#include "parse_tree.h"
#include "parse_tree.h"
#include <memory>

tree_node::tree_node(const tree_node& v) {
    _children = std::vector<std::shared_ptr<tree_node>>(v._children);

    if (v._value == nullptr) {
        _value = nullptr;
        return;
    }
    if (v._value->class_name() == "terminal") {
        _value = std::dynamic_pointer_cast<terminal>(v._value);
    }
    if (v._value->class_name() == "token") {
        _value = std::dynamic_pointer_cast<Token>(v._value);
    }
}

void parse_tree::print (std::ostream& out, const ptree_node& pos, size_t level) {
    // Не нужно отслеживать pos == nullptr так как корень всегда инициализирован, а если у
    // какого-то элемента дерева нет детей, то мы просто не заходим в цикл по его детям

    // Сначала выводим текущий сдвиг
    for (size_t i = 0; i < level; i++) {
        out << '\t';
    }
    // Далее выводим текущий корень поддерева (на него указывает pos)
    if (pos->_value->class_name() == "terminal") {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        out << temp << '\n';
    } else if (pos->_value->class_name() == "token") {
        Token temp = *std::dynamic_pointer_cast<Token>(pos->_value);
        out << temp.print_type_token(temp) << '\n';
    }
    // После этого печатаем все поддеревья, которые являются дочерними к корню pos
    for (auto& item : pos->_children) {
        print(out, item, level + 1);
    }
}

void parse_tree::insert_tree(const terminal& to_add, const parse_tree& tree, ptree_node& pos) {
    // Если мы пришли в пустое поддерево, то просто выходим из него
    // (по идее такая ситуация невозможно, но проверка была вставлена для безопасности)
    if (! pos) {
        return;
    }
    // Если это корень содержит терминал и у него нет детей, то в него возможна вставка
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        // Проверяем, нужный ли это терминал
        if (temp.type() == to_add.type()) {
            // Если нужный, то текущий корень очищаем и вставляем в него дерево
            pos = nullptr;
            copy_tree(pos, tree._root);
            return;
        }
    }
    // Если в текущее поддерево вставить невозможно, то пытаемся вставить в дочерние поддеревья
    for (auto& item : pos->_children) {
        insert_tree(to_add, tree, item);
    }
}

void parse_tree::add_tree(const terminal& to_add, const parse_tree& tree, const ptree_node& pos) {
    // Если мы пришли в пустое поддерево, то просто выходим из него
    // (по идее такая ситуация невозможно, но проверка была вставлена для безопасности)
    if (! pos) {
        return;
    }
    // Если это корень содержит терминал и у него нет детей, то к нему возможно добавление
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        if (temp.type() == to_add.type()) {
            // Если нужный, то к текущему дереву добавляем ребенка, в которого копируем дерево
            pos->_children.push_back(std::make_shared<tree_node>());
            copy_tree(*pos->_children.begin(), tree._root);
            return;
        }
    }
    // Если к текущему поддереву добавить невозможно, то пытаемся добавить к дочерним поддеревьям
    for (const auto& item : pos->_children) {
        add_tree(to_add, tree, item);
    }
}

void parse_tree::copy_tree(ptree_node& to, const ptree_node& from) {
    // Заменяем корни поддеревьев
    to = std::make_shared<tree_node>(*from);
    // Далее осуществляем копирование дочерний поддеревьев
    for (size_t i = 0; i < from->_children.size(); i++) {
        copy_tree(to->_children[i], from->_children[i]);
    }
}

bool parse_tree::add_token(const terminal& to_add, const Token& v, ptree_node& pos) {
    // Если мы пришли в пустое поддерево, то просто выходим из него
    // (по идее такая ситуация невозможно, но проверка была вставлена для безопасности)
    if (! pos) {
        return false;
    }
    // Если это корень содержит терминал и у него нет детей, то к нему возможно добавление
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        if (temp.type() == to_add.type()) {
            // Если это нужный, то к его детям добавляем переданный токен
            pos->_children.push_back(std::make_shared<tree_node>(v));
            return true;
        }
    }
    // Если к текущему поддереву добавить невозможно, то пытаемся добавить к дочерним поддеревьям
    for (auto& item : pos->_children) {
        if (add_token(to_add, v, item)) {
            return true;
        }
    }
    return false;
}

bool parse_tree::add_product(const terminal& to_add, type_product product, ptree_node& pos) {
    // Если мы пришли в пустое поддерево, то просто выходим из него
    // (по идее такая ситуация невозможно, но проверка была вставлена для безопасности)
    if (! pos) {
        return false;
    }
    // Если это корень содержит терминал и у него нет детей, то к нему возможно добавление
    if (pos->_value->class_name() == "terminal" && pos->_children.empty()) {
        terminal temp = *std::dynamic_pointer_cast<terminal>(pos->_value);
        if (temp.type() == to_add.type()) {
            // Если это нужный, то к его детям добавляем терминалы и токены, соответсвующее
            // переданной продукции
            push_product_item(pos->_children, product);
            return true;
        }
    }
    // Если к текущему поддереву добавить невозможно, то пытаемся добавить к дочерним поддеревьям
    for (auto& item : pos->_children) {
        if (add_product(to_add, product, item)) {
            return true;
        }
    }
    return false;
}

void parse_tree::push_product_item(vector<shared_ptr<tree_node>>& children, type_product product) {
    // для каждой продукции необходимо прописать, какие токены и терминалы она
    // добавляет
    if (product == FUNCTION) {
        push_product_item_function(children, product);
    } else if (product == BEGIN) {
        push_product_item_begin(children,product);
    } else if (product == END) {
        push_product_item_end(children,product);
    } else if (product == FUNCTION_NAME) {
        push_product_item_function_name(children,product);
    } else if (product == DESCRIPTION_1) {
        push_product_item_description_1(children,product);
    } else if (product == DESCRIPTION_2) {
        push_product_item_description_2(children,product);
    } else if (product == DESCR) {
        push_product_item_descr(children,product);
    } else if (product == TYPE_1) {
        push_product_item_type_1(children,product);
    } else if (product == TYPE_2) {
        push_product_item_type_2(children, product);
    } else if (product == VAR_LIST_1) {
        push_product_item_var_list_1(children,product);
    } else if (product == VAR_LIST_2) {
        push_product_item_var_list_2(children,product);
    } else if (product == OPERATORS_1) {
        push_product_item_var_operators_1(children, product);
    } else if (product == OPERATORS_2) {
        push_product_item_var_operators_2(children,product);
    } else if (product == OP_1) {
        push_product_item_var_op_1(children,product);
    } else if (product == EXPR_1) {
        push_product_item_expr_1(children, product);
    } else if (product == EXPR_2) {
        push_product_item_expr_2(children, product);
    } else if (product == EXPR_3) {
        push_product_item_expr_3(children, product);
    } else if (product == TERM_1) {
        push_product_item_term_1(children, product);
    } else if (product == TERM_2) {
        push_product_item_term_2(children, product);
    } else if (product == TERM_3) {
        push_product_item_term_3(children, product);
    } else if (product == SIMPLE_EXPR_1) {
        push_product_item_simple_expr_1(children, product);
    } else if (product == SIMPLE_EXPR_2) {
        push_product_item_simple_expr_2(children, product);
    } else if (product == SIMPLE_EXPR_3) {
        push_product_item_simple_expr_3(children, product);
    }
}

void parse_tree::push_product_item_function(vector<shared_ptr<tree_node>>& children, type_product product)
{
    children.push_back(std::make_shared<tree_node>(terminal{terminal::BEGIN}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::END}));
}

void parse_tree::push_product_item_begin(vector<shared_ptr<tree_node>>& children, type_product product)
{
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::FUNCTION_NAME}));
    children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "("}));
    children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, ")"}));
    children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "{"}));
}

void parse_tree::push_product_item_end(vector<shared_ptr<tree_node>>& children, type_product product)
{
    children.push_back(std::make_shared<tree_node>(Token{Token::RETURN, "return"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
    children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, "}"}));
}

void parse_tree::push_product_item_function_name(vector<shared_ptr<tree_node>>& children, type_product product)
{
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
}

void parse_tree::push_product_item_description_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
}

void parse_tree::push_product_item_description_2(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
}

void parse_tree::push_product_item_descr(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
    children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
}

void parse_tree::push_product_item_type_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(Token{Token::INT, "int"}));
}

void parse_tree::push_product_item_type_2(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(Token{Token::FLOAT, "float"}));
}

void parse_tree::push_product_item_var_list_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
}
void parse_tree::push_product_item_var_list_2(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    children.push_back(std::make_shared<tree_node>(Token{Token::COMMA, ","}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
}

void parse_tree::push_product_item_var_operators_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
}

void parse_tree::push_product_item_var_operators_2(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
}

void parse_tree::push_product_item_var_op_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    children.push_back(std::make_shared<tree_node>(Token{Token::EQUALS, "="}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::EXPR})); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
}

void parse_tree::push_product_item_expr_1(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
}

void parse_tree::push_product_item_expr_2(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
    children.push_back(std::make_shared<tree_node>(Token{Token::SUM, "+"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::EXPR}));
}

void parse_tree::push_product_item_expr_3(vector<shared_ptr<tree_node>>& children, type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
    children.push_back(std::make_shared<tree_node>(Token{Token::MINUS, "-"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::EXPR}));
}

void parse_tree::push_product_item_term_1(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_EXPR}));
    children.push_back(std::make_shared<tree_node>(Token{Token::MULTIPLY, "*"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
}

void parse_tree::push_product_item_term_2(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_EXPR}));
    children.push_back(std::make_shared<tree_node>(Token{Token::DIVIDE, "/"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
}

void parse_tree::push_product_item_term_3(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_EXPR}));
    children.push_back(std::make_shared<tree_node>(Token{Token::MOD, "%"}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::TERM}));
}

void parse_tree::push_product_item_simple_expr_1(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
}

void parse_tree::push_product_item_simple_expr_2(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(terminal{terminal::CONST}));
}

void parse_tree::push_product_item_simple_expr_3(vector<shared_ptr<tree_node>> &children, parse_tree::type_product product) {
    children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "("}));
    children.push_back(std::make_shared<tree_node>(terminal{terminal::EXPR}));
    children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, ")"}));
}
