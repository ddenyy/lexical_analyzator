//
// Created by Denis Osipov on 25.09.2023.
//

#include "../main/main.h"
#include "Token.h"


Token::Token()
{
    text_lexeme = "";
    type_lexeme = UNDEFINED;
    type_variable = "";
}

Token::Token(Type_lexeme& type, string text)
{
    this->type_lexeme = type;
    this->text_lexeme = text;
}

Token::Token(Type_lexeme type, string text)
{
    this->type_lexeme = type;
    this->text_lexeme = text;
}

Token::Token(string text) {
    this->type_lexeme = UNDEFINED;
    this->text_lexeme = std::move(text);
}



string Token::getText() {
    return this->text_lexeme;
}


bool Token::isEqual(Token& other)
{
    if (text_lexeme == other.getText())
    {
        return true;
    }
    return false;
}

string Token::print_type_token(Token& v) {
    std::string output;
    switch (v.get_type()) {
        case Token::RETURN:
            output = "RETURN KEYWORD";
            break;
        case Token::INT:
            output = "INT_KEYWORD";
            break;
        case Token::FLOAT:
            output = "FLOAT_KEYWORD";
            break;
        case Token::ID:
            output = "ID";
            break;
        case Token::INT_NUMBER:
            output = "INT_NUMBER";
            break;
        case Token::FLOAT_NUMBER:
            output = "FLOAT_NUMBER";
            break;
        case Token::SUM:
            output = "SUM";
            break;
        case Token::MINUS:
            output = "MINUS";
            break;
        case Token::LEFT_BRACKET:
            output = "LEFT_BRACKET";
            break;
        case Token::RIGHT_BRACKET:
            output = "RIGHT_BRACKET";
            break;
        case Token::EQUALS:
            output = "EQUALS";
            break;
        case Token::SEMICOLON:
            output = "SEMICOLON";
            break;
        case Token::LEFT_FIGURE_BRACKET:
            output = "LEFT_FIGURE_BRACKET";
            break;
        case Token::RIGHT_FIGURE_BRACKET:
            output = "RIGHT_FIGURE_BRACKET";
            break;
        case Token::COMMA:
            output = "COMMA";
            break;
        case Token::UNDEFINED:
            output = "UNDEFINED";
            break;
        case Token::MOD:
            output = "MOD";
            break;
        case Token::MULTIPLY:
            output = "MULTIPLY";
            break;
        case Token::DIVIDE:
            output = "DIVIDE";
            break;
        default:
            break;
    }
    output += " " + v.get_text();

    if (! v.get_type_variable().empty()) {
        output += " Var_type:" + v.get_type_variable();
    }
    return output;
}
