#include <deque>
#include <string>

#include "./parser.h"
#include "./token.h"
#include "./error.h"

Parser::Parser(std::deque<TokenPtr> t) : tokens(std::move(t)) {}

ValuePtr Parser::parse() {
    if (tokens.empty()) {
        throw SyntaxError("Empty tokens.");
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
    } else if (token->getType() == TokenType::LEFT_PAREN) {
        return parseTails();
    } else if (token->getType() == TokenType::QUOTE) {
        return parseQuote(std::string("quote"));
    } else if (token->getType() == TokenType::QUASIQUOTE) {
        return parseQuote(std::string("quasiquote"));
    } else if (token->getType() == TokenType::UNQUOTE) {
        return parseQuote(std::string("unquote"));
    } else {
        throw SyntaxError("Unimplemented.");
    }
}

ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw SyntaxError("Unexpected end of input.");
    }

    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }

    auto car = this->parse();

    if (tokens.empty()) {
        throw SyntaxError("Unexpected end of input.");
    }

    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();

        auto cdr = this->parse();

        if (tokens.empty()) {
            throw SyntaxError("Unexpected end of input.");
        }

        if (tokens.front()->getType() != TokenType::RIGHT_PAREN) {
            throw SyntaxError("Expected ')' after '.'.");
        }
        tokens.pop_front();

        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();

        return std::make_shared<PairValue>(car, cdr);
    }
}

ValuePtr Parser::parseQuote(const std::string& str) {
    return std::make_shared<PairValue>(
      std::make_shared<SymbolValue>(str),
      std::make_shared<PairValue>(
          this->parse(),
          std::make_shared<NilValue>()
      )
    );
}