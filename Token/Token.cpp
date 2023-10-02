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
