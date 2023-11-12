//
// Created by Denis Osipov on 11.11.2023.
//
#include "terminal.h"

std::ostream& operator<<(std::ostream& out, const terminal& v) {
    std::string res;
    switch (v._type) {
        case terminal::FUNCTION:
            out << "Function";
            return out;
        case terminal::BEGIN:
            out << "Begin";
            return out;
        case terminal::END:
            out << "End";
            return out;
        case terminal::FUNCTION_NAME:
            out << "Function name";
            return out;
        case terminal::DESCRIPTION:
            out << "Description";
            return out;
        case terminal::DESCR:
            out << "Descr";
            return out;
        case terminal::TYPE:
            out << "Type";
            return out;
        case terminal::VAR_LIST:
            out << "Var List";
            return out;
        case terminal::OPERATORS:
            out << "Operators";
            return out;
        case terminal::OP:
            out << "Op";
            return out;
        case terminal::ID:
            out << "Id";
            return out;
        case terminal::CONST:
            out << "Const";
            return out;
        case terminal::TERM:
            out << "Term";
            return out;
        case terminal::EXPR:
            out << "Expr";
            return out;
        case terminal::LET_NUM:
            out << "Let num";
            return out;
        case terminal::INT_NUMBER:
            out << "int number";
            return out;
        case terminal::FLOAT_NUMBER:
            out << "float number";
            return out;
        case terminal::SIMPLE_EXPR:
            out << "Simple expr";
            return out;
    }
}
