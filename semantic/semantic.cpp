#include "./semantic.h"

void semantic::run() {
    feel_begin();
    feel_description(_tree._root->_children[1]);
    feel_operators(_tree._root->_children[2]);
    feel_end();
    _tree._root->_rpn =_tree._root->_children[0]->_rpn +_tree._root->_children[1]->_rpn +_tree._root->_children[2]->_rpn +_tree._root->_children[3]->_rpn;
}

void semantic::feel_begin() {
    // in Begin
    auto curr = _tree._root->_children[0];
    // in Type
    curr = curr->_children[0];
    auto type = std::dynamic_pointer_cast<Token>(curr->_children[0]->_value);
    if (type->get_type() == Token::INT) {
        curr->_rpn = "int";
    } else if (type->get_type() == Token::FLOAT) {
        curr->_rpn = "float";
    }
    // in Function name
    curr = _tree._root->_children[0]->_children[1];
    auto name = std::dynamic_pointer_cast<Token>(curr->_children[0]->_children[0]->_value);
    std::string name_text = name->get_text();
    std::string type_text = type->get_text();
    _table.set_type(name_text, type_text);
    curr->_rpn = name->get_text();
    // собираем атрибут Begin
    // in Begin
    curr = _tree._root->_children[0];
    curr->_rpn = curr->_children[0]->_rpn + " " + curr->_children[1]->_rpn;
    curr->_rpn += " 2 FUNC\n";
}

void semantic::feel_end() {
    // in End
    auto curr = _tree._root->_children[3];
    auto var = std::dynamic_pointer_cast<Token>(curr->_children[1]->_children[0]->_value);
    curr->_rpn = var->get_text() + " return\n";

    curr = _tree._root->_children[0];
    // in Type
    curr = curr->_children[0];
    auto type = std::dynamic_pointer_cast<Token>(curr->_children[0]->_value);
    std::string var_text = var->get_text();
    if (type->get_text() != _table.Find(var_text).var_type()) {
        std::cout << "Different type: " << var_text << " must be " << type->get_text() << '\n';
        _is_not_error = false;
    }
}

void semantic::feel_description(std::shared_ptr<tree_node>& curr_description) {
    // Descr всегда есть, поэтому сначала разбираем его
    feel_descr(curr_description->_children[0]);
    curr_description->_rpn = curr_description->_children[0]->_rpn;
    // Проверяем, если дальше нет Description, то на этом все
    if (curr_description->_children.size() == 1) {
        return;
    }
    feel_description(curr_description->_children[1]);
    curr_description->_rpn += curr_description->_children[1]->_rpn;
}

void semantic::feel_descr(std::shared_ptr<tree_node>& curr_descr) {
    // Распознаем тип переменных в объявлении
    auto type = std::dynamic_pointer_cast<Token>(curr_descr->_children[0]->_children[0]->_value);
    // Вызываем разбор var list и получаем число переменных в нем
    std::string var_type = type->get_text();
    int cnt_var = feel_var_list(curr_descr->_children[1], var_type);
    // Собираем Descr
    curr_descr->_rpn = curr_descr->_children[1]->_rpn;
    curr_descr->_rpn += " " + type->get_text();
    curr_descr->_rpn += " " + std::to_string(cnt_var + 1);
    curr_descr->_rpn += " DECLARATION\n";
}

int semantic::feel_var_list(std::shared_ptr<tree_node>& var_list, std::string& var_type) {
    // Число переменных
    int cnt_var = 0;
    // Если дальше есть еще var list, то сначала разбираем его
    if (var_list->_children.size() == 3) {
        cnt_var = feel_var_list(var_list->_children[2], var_type);
        var_list->_rpn = var_list->_children[2]->_rpn;
    }
    cnt_var++;
    auto var = std::dynamic_pointer_cast<Token>(var_list->_children[0]->_children[0]->_value);
    std::string var_text = var->get_text();
    _table.set_type(var_text, var_type);
    if (var_list->_rpn.empty()) {
        var_list->_rpn += var->get_text();
    } else {
        var_list->_rpn += " " + var->get_text();
    }
    return cnt_var;
}

void semantic::feel_operators(std::shared_ptr<tree_node>& curr_operators) {
    feel_op(curr_operators->_children[0]);
    curr_operators ->_rpn = curr_operators->_children[0]->_rpn;
    if (curr_operators->_children.size() == 1) {
        return;
    }
    feel_operators(curr_operators->_children[1]);
    curr_operators->_rpn += curr_operators->_children[1]->_rpn;
}

void semantic::feel_op(std::shared_ptr<tree_node>& curr_op) {
    curr_op->_rpn = "=\n";
    auto var = std::dynamic_pointer_cast<Token>(curr_op->_children[0]->_children[0]->_value);
    curr_op->_rpn = var->get_text() + " " + curr_op->_rpn;
    std::string temp = var->get_text();
    std::string checker = _table.Find(temp).get_type_variable();

    if (checker.empty()) {
        std::cout << "Identification " << var->get_text() << " isn't declaration\n";
        _is_not_error = false;
        return;
    }
    feel_expr(curr_op->_children[2], checker);
    curr_op->_rpn = curr_op->_children[2]->_rpn + " " + curr_op->_rpn;
}

void semantic::feel_expr(std::shared_ptr<tree_node>& curr_expr, std::string& checker) {
    feel_term(curr_expr->_children[0], checker);
    curr_expr->_rpn = curr_expr->_children[0]->_rpn;
    if (curr_expr->_children.size() == 1) {
        return;
    }
    auto oper = std::dynamic_pointer_cast<Token>(curr_expr->_children[1]->_value);
    feel_expr(curr_expr->_children[2], checker);
    curr_expr->_rpn += " " + oper->get_text();
    curr_expr->_rpn = curr_expr->_children[2]->_rpn + " " + curr_expr->_rpn;
}

void semantic::feel_simple_expr(std::shared_ptr<tree_node>& curr_simple_expr, std::string& checker) {
    if (curr_simple_expr->_children.size() == 3) {
        feel_expr(curr_simple_expr->_children[1], checker);
        curr_simple_expr->_rpn = curr_simple_expr->_children[1]->_rpn;
        return;
    }
    std::shared_ptr<Token> type;
    if (curr_simple_expr->_children[0]->_children[0]->_children.empty()) {
        type = std::dynamic_pointer_cast<Token>(curr_simple_expr->_children[0]->_children[0]->_value);
        if ((checker == "int" && type->get_type() == Token::FLOAT_NUMBER)) {
            std::cout << "diff type: " << type->get_text() << " must be " << checker << '\n';
            _is_not_error = false;
            return;
        }
        if ((checker == "float" && type->get_type() == Token::INT_NUMBER)) {
            std::cout << "diff type: " << type->get_text() << " should " << checker << '\n';
            _is_not_error = false;
            return;
        }
    }
    if (type->get_type() == Token::ID) {
        std::string temp = type->get_text();
        if (_table.Find(temp).var_type() != checker) {
            std::cout << "diff type: " << temp << " should " << checker << '\n';
            _is_not_error = false;
            return;
        }
    }
    curr_simple_expr->_rpn = type->get_text();
}

void semantic::feel_term(std::shared_ptr<tree_node>& curr_term, std::string& checker) {
    feel_simple_expr(curr_term->_children[0], checker);
    curr_term->_rpn = curr_term->_children[0]->_rpn;
    if (curr_term->_children.size() == 1) {
        return;
    }
    auto oper = std::dynamic_pointer_cast<Token>(curr_term->_children[1]->_value);
    feel_term(curr_term->_children[2], checker);
    curr_term->_rpn += " " + oper->get_text();
    curr_term->_rpn = curr_term->_children[2]->_rpn + " " + curr_term->_rpn;
}