//
// Created by Denis Osipov on 17.10.2023.
//
#include "lexical_analizator.h"
#include <cctype>
#include <algorithm>
#include "DFA.h"

lexical_analyzer::lexical_analyzer(string file_name) {
    ifstream fin(file_name);
    _input << fin.rdbuf();
    fin.close();
    _pos._data = std::vector<int>(1);
}

void lexical_analyzer::update_position(const char s) {
    // Если это символ перехода строки, то создаем новую строку и переходим на нее
    if (s == '\n') {
        _pos._data.push_back(0);
        // Иначе увеличиваем число символов в текущей строке
    } else {
        (*_pos._data.rbegin())++;
    }
    // Накапливаем последнее прочитанное слово
    _pos._last_word += s;
}

bool lexical_analyzer::is_separators(char s) {
    vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=', '/', '%', '*' };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_separators(string& s) {
    vector<string> spr_sym = { "+", "-", "(", ")", "{", "}", ",", ";", "=", "/", "%", "*" };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_keyword(string& s) {
    bool flag = false;

    if (s == "int")
    {
        flag = true;
    }
    if (s == "return")
    {
        flag = true;
    }
    if (s == "float")
    {
        flag = true;
    }
    return flag;
}

Token::Type_lexeme lexical_analyzer::get_separator_type(string& s) {
    if (s == "+") {
        return Token::SUM;
    } else if (s == "-") {
        return Token::MINUS;
    } else if (s == "(") {
        return Token::LEFT_BRACKET;
    } else if (s == ")") {
        return Token::RIGHT_BRACKET;
    } else if (s == "{") {
        return Token::LEFT_FIGURE_BRACKET;
    } else if (s == "}") {
        return Token::RIGHT_FIGURE_BRACKET;
    } else if (s == ",") {
        return Token::COMMA;
    } else if (s == ";") {
        return Token::SEMICOLON;
    } else if (s == "=") {
        return Token::EQUALS;
    } else if (s == "/") {
        return Token::DIVIDE;
    } else if (s == "%") {
        return Token::MOD;
    } else if (s == "*") {
        return Token::MULTIPLY;
    }
    return Token::UNDEFINED;
}

Token::Type_lexeme lexical_analyzer::get_keyword_type(string& s) {
    if (s == "return") {
        return Token::RETURN;
    } else if (s == "int") {
        return Token::INT;
    } else if (s == "float") {
        return Token::FLOAT;
    }
    return Token::UNDEFINED;
}

std::string lexical_analyzer::get_next_word() {
    std::string word;
    // Очищаем последнее считанное слово, так как сейчас будет прочитано новое
    _pos._last_word.clear();
    while (true) {
        char s;
        _input.get(s);
        // Если это конец файла, то останавливаем цикл
        if (_input.eof()) {
            break;
        }
        update_position(s);
        // Если это пробельный символ, то возвращаем накопленное слово.
        // Если накопленное слово пусто, то продолжаем накапливать
        if (isspace(s)) {
            if (word.empty()) {
                continue;
            }
            return word;
        }
        // Если это односимвольная лексема, то тогда если накопленное слово пустое, то возвращаем
        // эту односимвольную лексему. Если же уже успело накопиться какое-то слово, то возвращаем
        // прочитанный символ в поток(так как этот символ отдельная лексема) и возвращаем
        // накопленное слово
        if (is_separators(s)) {
            if (word.empty()) {
                word += s;
                return word;
            }
            // Если был прочитан символ разделитель, то так как он является отдельным словом
            // его надо вернуть в поток и удалить из последнего прочитанного слова
            _input.unget();
            // Тут такой способ удаления, потому что erase удалял тупо последнюю ячейку
            // в массиве символов, а он почему-то состоит из 23 ячеек
            _pos._last_word = _pos._last_word.substr(0, _pos._last_word.size() - 1);
            // Уменьшаем позицию в строке
            (*_pos._data.rbegin())--;
            return word;
        }
        word += s;
    }
    return word;
}

Token lexical_analyzer::get_next_token() {
    // Получаем следующее по тексту слово
    std::string curr_word = get_next_word();
    // Далее определяем тип токена
    if (curr_word.empty()) {
        return {};
    }
    if (is_separators(curr_word)) {
        Token tmp_token(get_separator_type(curr_word), curr_word);
        _tokens.Add(tmp_token);
        return tmp_token;
    }
    if (is_keyword(curr_word)) {
        _tokens.Add({get_keyword_type(curr_word), curr_word});
        return {get_keyword_type(curr_word), curr_word};
    }
    DFA automat;
    _tokens.Add({automat.process(curr_word), curr_word});
    return {automat.process(curr_word), curr_word};
}

void lexical_analyzer::return_last_word() {
    // Слово необходимо вернуть в обратном порядке, чтобы потом его считать как надо
    std::reverse(_pos._last_word.begin(), _pos._last_word.end());
    for (const char& s : _pos._last_word) {
        // Если возвращаем символ перехода строки, то удаляем из вектора последнюю строку
        // Также удаляем последнюю строку, если в ней не осталось символов
        if (s == '\n' || (*_pos._data.rbegin()) == 0) {
            _pos._data.pop_back();
            // Иначе просто уменьшаем число символов в строке
        } else {
            (*_pos._data.rbegin())--;
        }
        _input.putback(s);
    }
    // Нужно в конце отдельно проверить, нужно ли удалить последнюю строку
    if ((*_pos._data.rbegin()) == 0) {
        _pos._data.pop_back();
    }
}


Hash_table lexical_analyzer::get_all_tokens() {
    DFA automat = DFA();
    // Пока читаются токены, добавляем их в таблицу
    while (true) {
        Token curr = get_next_token();
        if (_input.eof()) {
            break;
        }
        if (curr.getText().empty()) {
            continue;
        }
        _tokens.Add(curr);
    }
    return _tokens;
}
