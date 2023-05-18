#include <deque>

#include "./parser.h"
#include "./token.h"
#include "./error.h"

Parser::Parser(std::deque<TokenPtr> t) : tokens(std::move(t)) {}

ValuePtr Parser::parse() {
    if (tokens.empty()) {
        throw SyntaxError("Empty tokens");
    }

    auto token = std::move(tokens.front());
    tokens.pop_front();

    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    } else if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    } else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    } else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    } else {
        throw SyntaxError("Unimplemented");
    }
}

ValuePtr Parser::parseTails() {
    
}