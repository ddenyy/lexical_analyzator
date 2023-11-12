#include "parser.h"

void parser::check_lexical_error(Token& curr) {
    if (curr.get_type() == Token::UNDEFINED && ! curr.get_text().empty()) {
        std::cout << "UNDEFINED lexeme " << curr.get_text() << '\n';
    }
}

bool parser::run() {
    // Добавляем в дерево главную продукцию
    _parse_tree.add_product(terminal::FUNCTION, parse_tree::FUNCTION);
    // Создаем дерево разбора для терминала Begin
    parse_tree begin_tree(terminal::BEGIN);
    // Запускаем анализ вывода Begin
    if (! begin(begin_tree)) {
        return false;
    }
    // Если анализ вывода Begin завершился без ошибок, вставляем дерево разбора Begin в главное
    // дерево
    _parse_tree.insert_tree(terminal::BEGIN, begin_tree);
    // Создаем дерево разбора для терминала Description
    parse_tree description_tree(terminal::DESCRIPTION);
    // Запускаем анализ вывода Description
    if (! description(description_tree)) {
        return false;
    }
    // Если анализ вывода Description завершился без ошибок, вставляем дерево разбора Description в
    // главное дерево
    _parse_tree.insert_tree(terminal::DESCRIPTION, description_tree);
    // Создаем дерево разбора для терминала Operators
    parse_tree operators_tree(terminal::OPERATORS);
    // Запускаем анализ вывода Operators
    if (! operators(operators_tree)) {
        return false;
    }
    // Если анализ вывода Operators завершился без ошибок, вставляем дерево разбора Operators в
    // главное дерево
    _parse_tree.insert_tree(terminal::OPERATORS, operators_tree);
    // Создаем дерево разбора для терминала End
    parse_tree end_tree(terminal::END);
    // Запускаем анализ вывода End
    if (! end(end_tree)) {
        return false;
    }
    // Если анализ вывода End завершился без ошибок, вставляем дерево разбора End в главное дерево
    _parse_tree.insert_tree(terminal::END, end_tree);
    // Далее нужно отследить, не создано ли в файле более одной функции
    Token curr = _tokens.get_next_token();
    // Если прочитали пустой токен, то файл закончился, все хорошо
    if (curr.get_type() == Token::UNDEFINED && curr.get_text().empty()) {
        return true;
    }
    // Если же было прочитано что то не пустое, то выводим ошибку
    position curr_pos = _tokens.position();
    std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
    std::cout << "Error: no more one function\n";
    return false;
}

bool parser::begin(parse_tree& begin_tree) {
    // В дерево разбора Begin добавляем главную продукцию
    begin_tree.add_product(terminal::BEGIN, parse_tree::BEGIN);

    // Отслеживаем наличие типа функции
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the type of function: the type should be int or float\n";
        return false;
    }

    // Если тип функции есть, добавляем соответствующую продукцию в дерево
    if (curr.get_type() == Token::INT) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    } else if (curr.get_type() == Token::FLOAT) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }

    // Отслеживаем наличие имени функции
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the function name: the name must be the correct identifier\n";
        return false;
    }

    // Если имя функции есть, добавляем его
    begin_tree.add_product(terminal::FUNCTION_NAME, parse_tree::FUNCTION_NAME);
    begin_tree.add_token(terminal::ID, curr);

    // Отслеживаем наличие (
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::LEFT_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find (\n";
        return false;
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RIGHT_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find )\n";
        return false;
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::LEFT_FIGURE_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int||float the function description: not find {\n";
        return false;
    }
    return true;
}

bool parser::description(parse_tree& description_tree) {
    // Создаем дерево разбора для терминала Descr
    parse_tree small_description_tree(terminal::DESCR);

    // Сначала нужно разобрать вывод терминала Descr, так как он идет в начале для обеих продукций
    // Descriptions
    if (! small_descriptions(small_description_tree)) {
        return false;
    }
    // Если Descr был корректно разобран, надо понять, идет ли дальше Description. Если он идет
    // дальше, то начинаться он будет с типа данных, поэтому отслеживаем наличия токена INT или CHAR
    // в потоке дальше
    while (true) {
        Token curr = _tokens.get_next_token();
        // Если следующий токен не тип данных, то был прочитан просто Descr.
        if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {
            // Вернем прочитанный токен, чтобы корректно отработали функции дальше.
            _tokens.return_last_word();
            // Добавляем в дерево разбора Description дерево разбора Descr.
            description_tree.add_tree(terminal::DESCRIPTION, small_description_tree);
            return true;
        }
        // Иначе получается, что в потоке идет вторая продукция Description. Добавляем это продукцию
        // в дерево разбора и добавляем дерево разбора Descr, после чего чистим его и вызываем
        // функцию разбора Descr
        description_tree.add_product(terminal::DESCRIPTION, parse_tree::DESCRIPTION_2);
        description_tree.insert_tree(terminal::DESCR, small_description_tree);
        small_description_tree.clear();
        // Вернем прочитанный токен, чтобы корректно отработала функция разбора Descr
        _tokens.return_last_word();
        if (! small_descriptions(small_description_tree)) {
            return false;
        }
    }
}

bool parser::small_descriptions(parse_tree& small_description_tree) {
    // Добавим в дерево разбора Descr главную продукцию
    small_description_tree.add_product(terminal::DESCR, parse_tree::DESCR);
    // Отслеживаем наличие типа данных
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: "
                  << "the description should start with the type of variable\n";
    }
    // Добавляем тип данных в дерево разбора
    if (curr.get_type() == Token::INT) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    } else if (curr.get_type() == Token::FLOAT) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }
    // Создаем дерево разбора Var list
    parse_tree var_list_tree(terminal::VAR_LIST);
    // Запускаем разбор Var list
    if (! var_list(var_list_tree)) {
        return false;
    }
    // Если все успешно завершилось, добавляем дерево разбора Var list в дерево разбора Descr
    small_description_tree.insert_tree(terminal::VAR_LIST, var_list_tree);
    // Отслеживаем наличие ;
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the descriptions ending: no ;\n";
        return false;
    }
    return true;
}

bool parser::var_list(parse_tree& var_list_tree) {
    // Отслеживаем наличие идентификатора в начале Var list
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: no identifier in description\n";
        return false;
    }
    // Запомним прочитанный идентификатор, чтобы позже добавить его в дерево разбора. Делаем так,
    // потому что сначала нужно определить продукцию для Var list
    Token prev = curr;
    // Отслеживаем наличие запятой
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::COMMA) {
        // Если запятая не нашлась, значит у нас просто один идентификатор и все. Вернем прочитанную
        // лексему в поток
        _tokens.return_last_word();
        // Так как был прочитан просто один идентификатор, то добавляем в дерево первую продукцию
        // Var list и сам идентификатор
        var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_1);
        var_list_tree.add_token(terminal::ID, prev);

        return true;
    }
    // Если же мы встретили запятую, то добавляем вторую продукцию, идентификатор и вызываем разбор
    // Var list
    var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_2);
    var_list_tree.add_token(terminal::ID, prev);
    return var_list(var_list_tree);
}

bool parser::operators(parse_tree& operators_tree) {
    // Создаем дерево разбора для терминала Op
    parse_tree small_operators_tree(terminal::OP);
    // Сначала нужно разобрать вывод терминала Op, так как он идет в начале для обеих продукций
    // Operators
    if (! small_operators(small_operators_tree)) {
        return false;
    }
    // Если Op был корректно разобран, надо понять, идет ли дальше Operators. Если он идет
    // дальше, то начинаться он будет с типа данных, поэтому отслеживаем наличия идентификатора
    // в потоке дальше
    while (true) {
        Token curr = _tokens.get_next_token();
        // Если следующий токен не идентификатор, то был прочитан просто Op
        if (curr.get_type() != Token::ID) {
            // Вернем прочитанный токен, чтобы корректно отработали функции дальше.
            _tokens.return_last_word();
            // Добавляем в дерево разбора Operators дерево разбора Op.
            operators_tree.add_tree(terminal::OPERATORS, small_operators_tree);
            return true;
        }
        // Иначе получается, что в потоке идет вторая продукция Operators. Добавляем эту продукцию
        // в дерево разбора и добавляем дерево разбора Descr, после чего чистим его и вызываем
        // функцию разбора Op
        operators_tree.add_product(terminal::OPERATORS, parse_tree::OPERATORS_2);
        operators_tree.insert_tree(terminal::OP, small_operators_tree);
        small_operators_tree.clear();
        // Вернем прочитанный токен, чтобы корректно отработала функция разбора Op
        _tokens.return_last_word();
        if (! small_operators(small_operators_tree)) {
            return false;
        }
    }
}

bool parser::small_operators(parse_tree& small_operators_tree) {
    // Отслеживаем наличие идентификатора в потоке
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: expression must be assigned to the variable\n";
        return false;
    }
    // Если все хорошо, запоминаем идентификатор, чтобы потом его добавить в дерево разбора, потому
    // что сначала нужно определить продукцию
    Token id_token = curr;
    // Отслеживаем наличие знака равно в потоке
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::EQUALS) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find =\n";
        return false;
    }
    // Далее считываем токен и в зависимости от него выбираем продукцию
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    // Если дальше идет идентификатор, число или открывающая скобка, то это первая продукция
    if (curr.get_type() == Token::ID || curr.get_type() == Token::INT_NUMBER || curr.get_type() == Token::FLOAT_NUMBER || curr.get_type() == Token::LEFT_BRACKET) {
        // Вернем токен для корректной работы функции разбора Num expr
        _tokens.return_last_word();
        // Добавим в дерево разбора Op первую продукцию
        small_operators_tree.add_product(terminal::OP, parse_tree::OP_1);
        // Добавим идентификатор
        small_operators_tree.add_token(terminal::ID, id_token);
        // Создадим дерево разбора для терминала expr
        parse_tree expr_tree(terminal::EXPR);
        // Вызываем функцию разбора Num expr
        if (! expr(expr_tree)) {
            return false;
        }
        // Если все хорошо, что добавляем дерево разбора expr в дерево разбора Op
        small_operators_tree.insert_tree(terminal::EXPR, expr_tree);

    } else {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: invalid value " << curr.get_text() << "\n";
        return false;
    }
    // Отслеживаем наличие ; в потоке
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find ;\n";
        return false;
    }
    return true;
}

bool parser::end(parse_tree& end_tree) {
    // Добавим главную продукцию
    end_tree.add_product(terminal::END, parse_tree::END);
    // Отслеживаем наличие ключевого слова return
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RETURN) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no keyword return\n";
        return false;
    }
    // Отслеживаем наличие идентификатора
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no identifier\n";
        return false;
    }
    // Добавим прочитанный идентификатор в дерево
    end_tree.add_token(terminal::ID, curr);
    // Отслеживаем наличие точки с запятой
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no ;\n";
        return false;
    }
    // Отслеживаем наличие закрывающей фигурной скобки
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RIGHT_FIGURE_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no }\n";
        return false;
    }
    return true;
}

bool parser::simple_expr(parse_tree& simple_expr_tree) {
    // Проверяем наличие открывающей скобки, если она есть - это третья продукция
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() == Token::LEFT_BRACKET) {
        // Добавляем третью продукцию в дерево и создаем дерево разбора для EXPR
        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_3);
        parse_tree expr_tree(terminal::EXPR);
        // Вызываем разбор expr
        if (! expr(expr_tree)) {
            return false;
        }
        // Если все хорошо, то добавляем дерево expr в дерево SimpleExpr
        simple_expr_tree.insert_tree(terminal::EXPR, expr_tree);
        // Проверяем наличие закрывающей скобки
        curr_pos = _tokens.position();
        curr = _tokens.get_next_token();
        if (curr.get_type() != Token::RIGHT_BRACKET) {
            check_lexical_error(curr);
            std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
            std::cout << "Error in simple number expression: not find )\n";
            return false;
        }
        return true;
    }
    // Другие две продукции начинаются либо с числа, либо с идентификатора
    if (curr.get_type() != Token::ID && curr.get_type() != Token::INT_NUMBER && curr.get_type() != Token::FLOAT_NUMBER ) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in simple expression: " << curr.get_text()
                  << " must be a number or a variable\n";
        return false;
    }
    // Если все хорошо, то в зависимости от прочитанного токена добавляем нужную продукцию
    if (curr.get_type() == Token::ID) {
        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_1);
        simple_expr_tree.add_token(terminal::ID, curr);
    } else if (curr.get_type() == Token::INT_NUMBER || curr.get_type() == Token::FLOAT_NUMBER) {
        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_2);
        simple_expr_tree.add_token(terminal::CONST, curr);
    }
    return true;
}

bool parser::expr(parse_tree& expr_tree) {
    // Создаем дерево разбора для term
    parse_tree term_tree(terminal::TERM);
    // Сначала нужно разобрать вывод терминала term так как он идет в начале любой
    // продукции для expr
    if (! term(term_tree)) {
        return false;
    }
    // Далее нужно понять, какую продукцию следует применить
    Token curr = _tokens.get_next_token();
    // Если мы прочитали не арифметическую операцию, значит нужно применить первую продукцию: term
    if (curr.get_type() != Token::SUM && curr.get_type() != Token::MINUS) {
        // Вернем токен для корректной работы дальше
        _tokens.return_last_word();
        // Добавим в дерево разбора expr дерево разбора term
        expr_tree.add_tree(terminal::EXPR, term_tree);
        return true;
    }
    // Если мы прочитали сумму, то добавляем вторую продукцию
    if (curr.get_type() == Token::SUM) {
        expr_tree.add_product(terminal::EXPR, parse_tree::EXPR_2);
        // Добавим в дерево разбора expr дерево разбора term
        expr_tree.insert_tree(terminal::TERM, term_tree);
        // Если мы прочитали минус, то добавляем третью продукцию
    } else if (curr.get_type() == Token::MINUS) {
        expr_tree.add_product(terminal::EXPR, parse_tree::EXPR_3);
        // Добавим в дерево разбора expr дерево разбора term
        expr_tree.insert_tree(terminal::TERM, term_tree);
    }
    // Так как был прочитан плюс или минус, то дальше должен идти expr, вызываем функцию разбора
    // expr
    return expr(expr_tree);
}


bool parser::term(parse_tree& term_tree) {
    // Создаем дерево разбора для SimpleExpr
    parse_tree simple_expr_tree(terminal::SIMPLE_EXPR);
    // Сначала нужно разобрать вывод терминала SimpleExpr так как он идет в начале любой
    // продукции для Term
    if (! simple_expr(simple_expr_tree)) {
        return false;
    }

    // Далее нужно понять, какую продукцию следует применить
    Token curr = _tokens.get_next_token();
    // Если мы прочитали не арифметическую операцию, значит нужно применить первую продукцию simpleExpr
    if (curr.get_type() != Token::MULTIPLY && curr.get_type() != Token::DIVIDE && curr.get_type() != Token::MOD) {
        // Вернем токен для корректной работы дальше
        _tokens.return_last_word();
        // Добавим в дерево разбора Term дерево разбора SimpleExpr
        term_tree.add_tree(terminal::TERM, simple_expr_tree);
        return true;
    }
    // Если мы прочитали умножение, то добавляем вторую продукцию
    if (curr.get_type() == Token::MULTIPLY) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_1);
        // Добавим в дерево разбора term_tree дерево разбора SimpleExpr
        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);
        // Если мы прочитали деление, то добавляем третью продукцию
    } else if (curr.get_type() == Token::DIVIDE) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_2);
        // Добавим в дерево разбора term дерево разбора Simple expr
        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);
    } else if (curr.get_type() == Token::MOD) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_3);
        // Добавим в дерево разбора term дерево разбора Simple expr
        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);
    }
    // Так как был прочитан плюс или минус, то дальше должен идти Num expr, вызываем функцию разбора
    //  term

    return term(term_tree);
};
