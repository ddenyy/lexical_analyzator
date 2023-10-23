//
// Created by Denis Osipov on 16.10.2023.
//

#include "DFA.h"



DFA::transition DFA::get_transition(char s) {
    if (isalpha(s)) {
        return symbol_letter;
    }
    if (isdigit(s)) {
        return symbol_digit;
    }
    if (s == '.') {
        return symbol_point;
    }
    return symbol_error;
}


DFA::states DFA::function_step(DFA::states from, DFA::transition curr_symbol) {
    return _table[from][curr_symbol];
}


DFA::DFA() : _table(Table_states(COUNT_STATE, std::vector<states>(COUNT_TRANSITION)))
{
    _table[q_0_start][symbol_letter] = q_1_end_id;
    _table[q_0_start][symbol_digit] = q_2_end_int;
    _table[q_0_start][symbol_point] = q_4;

    _table[q_1_end_id][symbol_letter] = q_1_end_id;
    _table[q_1_end_id][symbol_digit] = q_1_end_id;
    _table[q_1_end_id][symbol_point] = q_4;

    _table[q_2_end_int][symbol_letter] = q_4;
    _table[q_2_end_int][symbol_digit] = q_2_end_int;
    _table[q_2_end_int][symbol_point] = q_3_end_float;

    _table[q_3_end_float][symbol_letter] = q_4;
    _table[q_3_end_float][symbol_digit] = q_3_end_float;
    _table[q_3_end_float][symbol_point] = q_4;

    _table[q_4][symbol_letter] = q_4;
    _table[q_4][symbol_digit] = q_4;
    _table[q_4][symbol_point] = q_4;
}

Token::Type_lexeme DFA::process(string& lex) {
    states cur_state = q_0_start;

    for(char symbol: lex)
    {
        // определим тип символа
        transition cur_transition = get_transition(symbol);

        if (cur_transition == symbol_error)
        {
            return Token::UNDEFINED;
        }

        cur_state = function_step(cur_state, cur_transition);
    }

    if (cur_state == q_1_end_id)
    {
        return Token::ID;
    }

    if (cur_state == q_2_end_int)
    {
        return Token::INT_NUMBER;
    }

    if (cur_state == q_3_end_float)
    {
        return Token::FLOAT_NUMBER;
    }

    return Token::UNDEFINED;
}
