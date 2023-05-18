#ifndef PARSE_H
#define PARSE_H

#include <deque>

#include "value.h"

class Parser {
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> t);
    ValuePtr parse();
};

#endif