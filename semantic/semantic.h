#pragma once

#include "../parse_tree/parse_tree.h"
#include "../hash_table/Hash_table.h"

#include <string>

class semantic {
private:
    parse_tree _tree;
    Hash_table _table;
    bool _is_not_error;

private:
    void feel_begin();
    void feel_end();
    void feel_description(std::shared_ptr<tree_node>& curr_description);
    void feel_descr(std::shared_ptr<tree_node>& curr_descr);
    int feel_var_list(std::shared_ptr<tree_node>& var_list, std::string& var_type);
    void feel_operators(std::shared_ptr<tree_node>& curr_operators);
    void feel_op(std::shared_ptr<tree_node>& curr_op);
    void feel_expr(std::shared_ptr<tree_node>& curr_expr, std::string& checker);
    void feel_simple_expr(std::shared_ptr<tree_node>& curr_simple_expr, std::string& checker);
    void feel_term(std::shared_ptr<tree_node>& curr_term, std::string& checker);

public:
    semantic(const parse_tree& v, const Hash_table& h) : _tree{v}, _table{h}, _is_not_error{true} {}
    void run();
    bool is_not_error() const { return _is_not_error; }
    string get_rpn() const { return _tree._root->_rpn; }
    Hash_table get_hash_table() const { return _table; }
};
