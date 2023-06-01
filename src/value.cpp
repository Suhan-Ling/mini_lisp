#include "./value.h"
#include "./error.h"

#include <iomanip>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <typeinfo>
#include <functional>

bool Value::isNil() const {
    return (typeid(*this) == typeid(NilValue));
}

bool Value::isSelfEvaluating() const {
    return (typeid(*this) == typeid(BooleanValue)) or
           (typeid(*this) == typeid(NumericValue)) or
           (typeid(*this) == typeid(StringValue)) or
           (typeid(*this) == typeid(BuiltinProcValue));
}

bool Value::isPair() const {
    return (typeid(*this) == typeid(PairValue));
}

bool Value::isNumber() const {
    return (typeid(*this) == typeid(NumericValue));
}

std::vector<ValuePtr> Value::toVector() const {
    throw LispError("Cannot convert to vector.");
}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

double Value::asNumber() const {
    throw LispError("Not a number.");
}

ValuePtr Value::getCar() {
    throw LispError("Not a pair value.");
}

ValuePtr Value::getCdr() {
    throw LispError("Not a pair value.");
}

ValuePtr Value::apply(std::vector<ValuePtr> args) {
    throw LispError("Not a builtin procedure.");
}

std::string BooleanValue::toString() const {
    if (value) {
        return "#t";
    } else {
        return "#f";
    }
}

std::string BooleanValue::getType() const {
    return "BooleanValue";
}

std::string NumericValue::toString() const {
    if (std::floor(value) == value) {
        return std::to_string(static_cast<int>(value));
    } else {
        return std::to_string(value);
    }
}

std::string NumericValue::getType() const {
    return "NumericValue";
}

double NumericValue::asNumber() const {
    return value;
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}

std::string StringValue::getType() const {
    return "StringValue";
}

std::string NilValue::toString() const {
    return "()";
}

std::string NilValue::getType() const {
    return "NilValue";
}

std::vector<ValuePtr> NilValue::toVector() const {
    std::vector<ValuePtr> v;
    return v;
}

std::string SymbolValue::toString() const {
    return name;    
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return name;
}

std::string SymbolValue::getType() const {
    return "SymbolValue";
}

static int PairValue_toString_layer = 0;

std::string PairValue::toString() const {
    PairValue_toString_layer++;

    std::string result = "";
    if (right->toString() == "()") {
        result += left->toString();
    } else if (auto rightPair = dynamic_cast<PairValue*>(right.get())) {
        result += left->toString() + " " + rightPair->toString();
    } else {
        result += left->toString() + " . " + right->toString();
    }
    
    PairValue_toString_layer--;
    if (PairValue_toString_layer) {
        return result;
    } else {
        // printf("111\n");
        return "(" + result + ")";
    }
}

std::string PairValue::getType() const {
    return "PairValue";
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result, r;
    result.push_back(left);
    // std::cout << "toVector:" << std::endl;
    // std::cout << left->toString() << ' ' << right->toString() << std::endl;
    // std::cout << right->getType() << std::endl;
    // std::cout << std::endl;
    if (right->isPair()) {
        r = right->toVector();
        // std::cout << "isPair()" << std::endl;
        // std::cout << r.size() << std::endl;
        // for (auto i: r) {
        //     std::cout << i->toString() << std::endl;
        // }
        // std::cout << std::endl;
        for (auto i: r) {
            result.push_back(i);
        }
    } else if (right->getType() != "NilValue") {
        // std::cout << "Not NilValue" << std::endl;
        result.push_back(right);
    }
    return result;
}

ValuePtr PairValue::getCar() {
    return left;
}

ValuePtr PairValue::getCdr() {
    return right;
}


std::string BuiltinProcValue::toString() const {
    return "#<procedure>"; 
}

std::string BuiltinProcValue::getType() const {
    return "BuiltinProcValue";
}

ValuePtr BuiltinProcValue::apply(std::vector<ValuePtr> args) {
    return func(args);
}