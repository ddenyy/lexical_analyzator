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
        case terminal::NUM_EXPR:
            out << "Expr";
            return out;
        case terminal::SIMPLE_NUM_EXPR:
            out << "Simple num expr";
            return out;
        case terminal::STRING_EXPR:
            out << "String expr";
            return out;
        case terminal::SIMPLE_STRING_EXPR:
            out << "Simple string expr";
            return out;
        case terminal::ID:
            out << "Id";
            return out;
        case terminal::CONST:
            out << "Const";
            return out;
    }
}
