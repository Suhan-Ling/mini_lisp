#ifndef PARSER_H
#define PARSER_H

#include <deque>

#include "./value.h"
#include "./token.h"

class Parser {
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> t);
    ValuePtr parse();
    ValuePtr Parser::parseTails();
};

#endif