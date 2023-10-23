//
// Created by Denis Osipov on 16.10.2023.
//

#ifndef YAMP_LABS_DFA_H
#define YAMP_LABS_DFA_H

#include "../main/main.h"
#include "../Token/Token.h"
#include "../utils/Utils.h"

class DFA {
    private:
        enum states {
            q_0_start,
            q_1_end_id,
            q_2_end_int,
            q_3_end_float,
            q_4  // пустое множество типо
        };

        enum transition {
            symbol_letter,
            symbol_digit,
            symbol_point,
            symbol_error
        };

        using Table_states = vector<vector<states>>;

        Table_states _table;



     /**
     * Метод совершает "шаг" автомата - из состояния по символу переходит в новое состояние.
     * \param from - состояние из которого делаем шаг
     * \param curr_symbol - по какому событию делаем переход
     * */

     states function_step(states from, transition curr_symbol);


    transition get_transition(char s);

    public:

        // конструктор по умолчанию
        DFA();

        // получаем тип токена по его текстовому представлению
        // lex - текстовое представление токена
        Token::Type_lexeme process(string& lex);

};


#endif //YAMP_LABS_DFA_H
