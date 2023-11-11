//
// Created by Denis Osipov on 11.11.2023.
//

#include "parse_tree.h"
#include "parse_tree.h"
#include <memory>

tree_node::tree_node(const tree_node& v) {
    _children = std::vector<std::shared_ptr<tree_node>>(v._children);
    _rpn = v._rpn;
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
        out << temp.get_text() << '\n';
        temp.print_type_token(temp);
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

void parse_tree::push_product_item(std::vector<std::shared_ptr<tree_node>>& children, type_product product) {
    // В этой функции просто для каждой продукции необходимо прописать, какие токены и терминалы она
    // добавляет
    if (product == FUNCTION) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::BEGIN}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::END}));
    } else if (product == BEGIN) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::FUNCTION_NAME}));
        children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "("}));
        children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, ")"}));
        children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "{"}));
    } else if (product == END) {
        children.push_back(std::make_shared<tree_node>(Token{Token::RETURN, "return"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
        children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, "}"}));
    } else if (product == FUNCTION_NAME) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == DESCRIPTION_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
    } else if (product == DESCRIPTION_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCR}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::DESCRIPTION}));
    } else if (product == DESCR) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::TYPE}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
    } else if (product == TYPE_1) {
        children.push_back(std::make_shared<tree_node>(Token{Token::INT, "int"}));
    } else if (product == TYPE_2) {
        children.push_back(std::make_shared<tree_node>(Token{Token::FLOAT, "float"}));
    } else if (product == VAR_LIST_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == VAR_LIST_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(Token{Token::COMMA, ","}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::VAR_LIST}));
    } else if (product == OPERATORS_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
    } else if (product == OPERATORS_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OP}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::OPERATORS}));
    } else if (product == OP_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(Token{Token::EQUALS, "="}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
    } else if (product == OP_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
        children.push_back(std::make_shared<tree_node>(Token{Token::EQUALS, "="}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::STRING_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SEMICOLON, ";"}));
    } else if (product == NUM_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
    } else if (product == NUM_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SUM, "+"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
    } else if (product == NUM_EXPR_3) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::MINUS, "-"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
    } else if (product == SIMPLE_NUM_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::ID}));
    } else if (product == SIMPLE_NUM_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::CONST}));
    } else if (product == SIMPLE_NUM_EXPR_3) {
        children.push_back(std::make_shared<tree_node>(Token{Token::LEFT_BRACKET, "("}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::NUM_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::RIGHT_BRACKET, ")"}));
    } else if (product == STRING_EXPR_1) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_STRING_EXPR}));
    } else if (product == STRING_EXPR_2) {
        children.push_back(std::make_shared<tree_node>(terminal{terminal::SIMPLE_STRING_EXPR}));
        children.push_back(std::make_shared<tree_node>(Token{Token::SUM, "+"}));
        children.push_back(std::make_shared<tree_node>(terminal{terminal::STRING_EXPR}));
    }
}
