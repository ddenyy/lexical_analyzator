//#include <iostream>
//
//#include "../lexical_item/terminal.h"
//#include "../Token/Token.h"
//#include "../parse_tree/parse_tree.h"
//
//int main() {
//
//    ofstream fout_1("output.txt");
//    ofstream fout_2("output2.txt");
//
//    parse_tree tree;
//    tree.add_product(terminal::FUNCTION, parse_tree::FUNCTION);
//    tree.add_product(terminal::BEGIN, parse_tree::BEGIN);
//    tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
//    tree.add_product(terminal::FUNCTION_NAME, parse_tree::FUNCTION_NAME);
//    Token test_id_main(Token::ID, "main");
//    tree.add_token(terminal::ID, test_id_main);
//
//    parse_tree descr_tree(terminal::DESCR);
//    descr_tree.add_product(terminal::DESCR, parse_tree::DESCR);
//    descr_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
//    descr_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_1);
//    Token test_id_a(Token::ID, "a");
//    descr_tree.add_token(terminal::ID, test_id_a);
//
//
//    tree.add_product(terminal::DESCRIPTION, parse_tree::DESCRIPTION_1);
//    tree.insert_tree(terminal::DESCR, descr_tree);
//
//    tree.add_product(terminal::OPERATORS, parse_tree::OPERATORS_1);
//    tree.add_product(terminal::OP, parse_tree::OP_1);
//    tree.add_token(terminal::ID, test_id_a);
//    tree.add_product(terminal::NUM_EXPR, parse_tree::NUM_EXPR_1);
//    tree.add_product(terminal::SIMPLE_NUM_EXPR, parse_tree::SIMPLE_NUM_EXPR_2);
//    Token test_number_1 =Token(Token::NUMBER, "1");
//    tree.add_token(terminal::CONST, Token{Token::NUMBER, "1"});
//
//    tree.add_product(terminal::END, parse_tree::END);
//    tree.add_token(terminal::ID, test_id_a);
//
//    tree.print(fout_1);
//    descr_tree.print(fout_1);
//}