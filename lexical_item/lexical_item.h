#ifndef LEXICAL_ITEM_H
#define LEXICAL_ITEM_H

#include <iostream>
#include <string>

class lexical_item {
public:
    virtual std::string class_name() const = 0;
};

#endif // LEXICAL_ITEM_H
